# SyntaxGen
In January AIR Tech Tips featured:EXCEL CONCATENATE FUNCTION: CREATING LONG STATA COMMANDS By Ainur Satekova.  This tech tip made me think about how a program could be written to write out syntax files using inputs. One issue that has caused trouble in the past if the concatenate function is not correct then the formula will have to be rewritten and copied back into the syntax. 

This lead me to write a simple C++ program that runs from the command line and accepts a csv file to write out syntax files for R, SPSS, SAS, SQL and STATA.  It does this by reading the csv file and cutting up the input file to write the lines into a separate file.

Take the example below where the first column is the ID and the second column is the attribute the student needs to be tagged with.

12345678,TRIO_UpwardBound

At the time the program is run the user will enter the name of the ID variable (StudentID) and the Attribute variable (TRIO) This program will split out the line above to look like this in each of the various software syntax files.

SPSS example: if StudentID=12345678 TRIO=”TRIO_UpwardBound.

The program will loop through the full csv file and output the syntax files with the proper formatting and extensions to a directory of the users choosing.





