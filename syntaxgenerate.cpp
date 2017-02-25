/*  

    This C++ program takes an input of a csv file with two columns
    one for an identier which should be unduplicated and the other for an
    attribute.


    
*/



#include <algorithm>
#include <iostream>
#include <fstream>
#include <string>


using namespace std;


/*outputfiles function takes the name of the file for output, the code that will be generated and the type of file.  

  This function then writes the syntax to the file  */

string outputfiles(string outfile,string output, string type)
{
  ofstream outputfile;

  //make the file name here
  outfile=outfile.substr(0,(outfile.length()-3))+type;

  //check to see if this file exists
  ifstream outfilecheck(outfile.c_str());
			 

  //if it does exist the file is not written
  if(outfilecheck.good())

    {
      return "\n"+outfile+" already exists, file not written.\n";
      
      
    }else{

    //write out files if they do not exist
    
    outputfile.open (outfile.c_str());
    outputfile <<output;
    outputfile.close();

    return "\n "+ type+" written to "+outfile+"\n";
  
  }
 
}
    


int main () {

  /*define global variables for labels, attributes and files   */
  
  ifstream inputfile;
  ofstream outputfile;
  string line;
  string id_label;
  string id_full;
  string att_label;
  string id_quote;
  string att_quote;
  string spss="//Check for latest file and submit updates @ https://github.com/UTSA-OIR/SyntaxGen \n";
  string sas="//Check for latest file @ https://github.com/UTSA-OIR/SyntaxGen \n \n";
  string r="//Check for latest file @ https://github.com/UTSA-OIR/SyntaxGen \n\n";
  string r_dataframe;
  string sql="//Check for latest file @ https://github.com/UTSA-OIR/SyntaxGen \n\n\n\nCase\n";
  string outputdirectory="";
  string inputfilename="";
  string outputfilename;
  string saveas;
  int linecount;
  string dups;

  //describe program and file type needed

  cout<<"This C++ program takes an input of a csv file with two columns one for an identfier and the other for an attribute. The file is assumed to have unduplicated identifiers and be in the format:\nID,Attribute.\n\n";


  //this will always fail to start, however it is needed to make the while loop below work
  
  inputfile.open(inputfilename.c_str());


  //while loop to get file open
  
  while (!inputfile.good()){

    cout<<"Enter input file name including directory (ex: C:\\attributefile.csv)?\n";
    cout<<"File Path and Name:";
    cin>>inputfilename;

  
    inputfile.open(inputfilename.c_str());

    if(inputfile.good()){break;}

    else{cout<<"\nFile "+inputfilename+" does not exist\n";}

  }

  //enter variable name, this should be what it is called in the data file

  
  cout<<"Enter ID Variable Name:";
  cin>>id_label;

  //enter attribute name, this should be what it is called in the data file


  cout<<"Enter Attribute Variable Name:";
  cin>>att_label;


  //set quotes indicator, sql will default to single quotes (is this true for all implementations)
  
  cout<<"\n\nTypically string ids and attributes will need quotes while numeric ids and attributes will not.\n\n";

  cout<<"Does "<< id_label<<" Need Quotes(y/n)?";
  cin>>id_quote;

  cout<<"Does "<<att_label<<" Need Quotes(y/n)?";
  cin>>att_quote;


  

  cout<<"Enter R Dataframe name:";

  cin>>r_dataframe;
  r_dataframe=r_dataframe+"$";

  //read in all lines and convert to syntax




  
  if (att_quote=="y")

    {
      //code written to intialize variables for spss, sas and R if string
      
      r=r+"\n"+r_dataframe+att_label+"<-\"\"\n";

      spss=spss+"string "+att_label+"(A100).\n";
     
      //initalize in sas
      sas=sas+"compute "+att_label+"=\"\";\n";
    }
  else

    {//code written to intialize variables for spss, sas and R if numeric
      r=r_dataframe+att_label+"<-0\n";

      spss=spss+"compute "+att_label+"=-99.\n";
      spss=spss+"MISSING VALUES "+att_label+"-99.\n";

      //initalize in sas
      sas=sas+"compute "+att_label+"=0;";
	  
    }
    
  
  if (id_quote=="y"){id_quote="\"";}else{id_quote="";}
  if (att_quote=="y"){att_quote="\"";}else{att_quote="";}
       
  
  while(getline(inputfile,line)){


    line.erase(remove( line.begin(), line.end(), '\"' ), line.end());
    line.erase(remove( line.begin(), line.end(), '\'' ), line.end());

    cout<<"line "<<linecount<<line<<"\n";
    
    string id;
    string att;
    string rline;
    
    string::size_type commafind= line.find(",");
    
    id=line.substr(0,commafind);
    att=line.erase(0,(commafind+1));

    //check for duplicate IDs  here

    string idfind=" "+id+" ";
    string::size_type dupcheck=id_full.find(idfind);

    
    if (dupcheck!=string::npos)
      {
	dups=dups+" "+id+"\n";
      } 

    //Add Duplicate ID to string, need to make this an array or list in the long term
    
    id_full=id_full+" "+id+" ";

    
    id=id_quote+id+id_quote;

    att=att_quote+att+att_quote;

    if(dupcheck!=string::npos)
      {

	spss=spss+"if "+id_label+"="+id+" "+att_label+"="+att+".\n";

	sas=sas+"if "+id_label+"="+id+" then "+att_label+"="+att+";\n";

	r= r+r_dataframe+att_label+"<-ifelse("+r_dataframe+id_label+"=="+id+", "+att+","+r_dataframe+att_label+")\n";
 
	sql=sql+"\twhen "+id_label+"="+id+" then "+att+"\n";
    
      }
    else
      {


      spss=spss+"if "+id_label+"="+id+" "+att_label+"="+att+".\n";
      sas=sas+"if "+id_label+"="+id+" then "+att_label+"="+att+";\n";
      r=r+r_dataframe+att_label+"<-ifelse("+r_dataframe+id_label+"=="+id+", "+att+","+r_dataframe+att_label+")\n";
 
      sql=sql+"\twhen "+id_label+"="+id+" then "+att+"\n";
    }
    
    
   
    linecount++;
  }


  cout<<"\nLines Read: "<<linecount<<"\n\nThe file has duplicate records:\n"+dups;


  
  
  //end SQL case statement here
    
  sql=sql+"End as "+att_label+",";

  

  inputfile.close();
  
 
  outputfile.open (outputdirectory.c_str());



  cout<<"\nEnter Output file name (no extension):";
  cin>>saveas;

  while (!outputfile.good()){

    //get working directory, file and set options for exporting files
    cout<<"Input the  directory to save output files to (sps, sas, r, sql).All output files (.sps,.sas,.r,.sql) will be saved to this directory:\n";




  
    //get output directory from user
  
    cout<<"Output Directory:";
    cin>>outputdirectory;

    //create dummyfile to test that it works, if it fails then ask again

    outputdirectory=outputdirectory+saveas+".tem";


    outputfile.open (outputdirectory.c_str());

    //if file exists then close test file and create all files
  
    if(outputfile.good())
      {
	
	outputfile.close();
      
	break;
      }
    
    else
      {cout<<"\nInvalid directory please try again.\n\n";}

  }


  
  cout<<outputfiles(outputdirectory,spss,"sps");
  cout<<outputfiles(outputdirectory,sas,"sas");
  cout<<outputfiles(outputdirectory,r,"r");
  cout<<outputfiles(outputdirectory,sql,"sql");
  
  
  return 0;
}
