//ADVANCED OPERATING SYSTEM
//ASSIGNMENT 1 


/*

Program              : Implementation for Visiting current directory, it's subdirectories and printing file's information
Author               : © Sakeena Shahid-29, Shreshth Saxena -38
Created on           : March 22, 2018 09:20 IST
Language             : C
Library Functions    : fopen(..),fgets(..),memset(..),fclose(.) 
Unix Command         : ls -l, -R, grep, cut

*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
//#include <sys/time.h>
//#include <fcntl.h>
//#include <sys/types.h>

#define MAXCHAR 2000

void Display(char str[MAXCHAR]);


/* 

Method Name            : Read_Dir
Purpose                : To fetch the information of directory, it's subdirectory and files
Summary                : Lists the information of current directory and it's subdirectory recursively using unix     commands, redirects this information to a file. Reads the file line by line and passes the string to display procedure if it is a file(-), directory(d), or a linked directory(l).
Parameters             : None
Returns                : 
                             Zero		   - if function executes successfully.
                             Non-Zero	           - error case.	  
   
*/

int Read_Dir(){


	FILE *fp,*fptr,*fptr1;					     // A file pointer to FILE structure 
	char str[MAXCHAR],i,ch;
    
	system("ls -lR |grep . > file1.txt");		            // Redirecting the output of the command in file 

  
	fp = fopen("file1.txt", "r");		   	            // Opening file for reading.
	

	if (fp == NULL){
        
		printf("Could not open file %s","file1.txt");
		return -1;
        
	}
       
	while (fgets(str, MAXCHAR, fp) != NULL){                   // Reading a line of the file

		
		if(str[0]=='-' || str[0]=='d' || str[0]=='l'){	  

	   		Display(str);

		}

		else if(str[0]=='.'){
        
        		printf("\n\n***************A SUBDIRECTORY***************\n\n");

			for(i=0;str[i]!='\n';i++){
            
           			ch=str[i]; 
				printf("%c",ch);

     		   	}
			printf("\n");		
		}

		memset(str,0,strlen(str));                	           // Freeing the contents of str.  
        
	}
       
    
	fclose(fp);                            	           // Closing the file pointed by fp.
	return 0;
    
}

/* 

Method Name             : Display
Purpose                 : To display the information of directory,files and linked directory
Summary                 : Duplicate spaces of the listed attribute(str) is removed because space is the delimiter of 'cut' command. String with removed space is being overwritten at the start of the file(info.txt) as file discriptor is opened in 'w' mode. Further, the first character of the string is checked for a directory,a file or a linked directory. Unix command 'cut' is used for cutting sections from the file(info.txt) and displaying File name, Permissions,Link/Directory count, User, Group, File-size and Time to standard output.
Parameters              : Character Array, str: each record carries file/directory information
Returns                 : None
   
*/


void Display(char str[MAXCHAR]){

	char ch,str1[MAXCHAR];					                            // Holds Character 
	int i=0,j;				                            // Controls Loop
	FILE *fptr,*fptr1;

/* Removing duplicate spaces. */

        for(i=0,j=0;i<=strlen(str);i++){
                
		ch=str[i];
                str1[j++]=ch; 
                               
                if(ch==' '){  
		                                    		
	            while(str[i+1]==' ')						
                        ch=str[i++];

                }  		
         }  

	fptr = fopen("info.txt", "w+");

/* Writing Contents of the string to file. */ 

	if ( fptr ){

		fputs(str1,fptr);
		fclose(fptr);
	   		        	
	}
	else
	      printf("Failed to open the file\n");

/* Checking for the file(-), Directory(d) or a linked directory(l) and further displaying the content */

        if(str[0]=='-' || str[0]=='d' || str[0]=='l'){             

	if(str[0]=='-' )
        	printf("\n\n\n***************FILE***************\n");

	else if(str[0]=='l')
		printf("\n\n\n***************LINKED DIRECTORY***************\n"); 
  
	else if(str[0]=='d')
    		printf("\n\n\n***************DIRECTORY***************\n");	
	
    
	printf("\n\nName");
	system("cut -d' ' -f9 info.txt");
	

	printf("\n\nPermission");
	system("cut -d' ' -f1 info.txt");
	

	printf("\n\nLink/Directory Count");
	system("cut -d' ' -f2 info.txt");
	

	printf("\n\nGroup");
	system("cut -d' ' -f3 info.txt");
		

	printf("\n\nOthers:");
	system("cut -d' ' -f4 info.txt");
	

	printf("\n\nFile Size");
	system("cut -d' ' -f5 info.txt");
	

	printf("\n\nTime");
	system("cut -d' ' -f6-8 info.txt");
		
      }
   	
//	system("rm info.txt");        
}

/*

Prototype 	:		int main(int argc, char *argv[1])
Purpose    	:		Program execution starts from the main() function. 
Summary  	:		The main function can in-turn calls Read_Dir() function and by passing the execution control. The function returns control to main when a return statement is executed or when end of function is reached.
Parameters 	: 		Number of arguments obtained from command line input , argument array 
Returns     	: 		Integer

*/

int main(int argc, char *argv[1]) {				// Main Function 
   
    Read_Dir();
    return 0;
    
}




