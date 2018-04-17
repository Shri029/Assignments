//ADVANCED OPERATING SYSTEM
//ASSIGNMENT 1 


/*

Program              : Implementation for Visiting current directory, it's subdirectories and printing file's information
Author               : © Shivani Tiwary-36, Surajit Das-41
Created on           : March 22, 2018 09:20 IST
Language             : C
Library Functions    : fopen(..),fgets(..),memset(..),fclose(.) 
Unix Command         : ls -l, -R, grep

*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAXCHAR 2000

void Display(char str[MAXCHAR]);

int Read_Dir(){

/* 

Method Name       : Read_Dir
Purpose                : To fetch the information of directory, it's subdirectory and files
Summary              : Lists the information of current directory and it's subdirectory recursively using unix     commands, redirects this information to a file. Reads the file line by line and passes the string to user defined method for display.
Parameters           : None
Returns                 : 
                             Zero		- if function executes successfully.
                             Non-Zero	           - error case.	  
   
*/


    FILE *fp;					                     // A file pointer to FILE structure 
    char str[MAXCHAR];
    
    system("ls -lR |grep . > file1.txt");		            // Redirecting the output of the command in file 

  
    fp = fopen("file1.txt", "r");		   	            // Opening an existing file for reading.

    if (fp == NULL){
        
        printf("Could not open file %s","file1.txt");
        return -1;
        
    }
     
    while (fgets(str, MAXCHAR, fp) != NULL){                       // To read file line by line into the string.
    
        Display(str);
        memset(str,0,strlen(str));                	           // Freeing the contents of str.  
        
    }
    
    fclose(fp);                               	                  // Closing the file pointed by fp.
    return 0;
    
}

void Display(char str[MAXCHAR]){
/* 

Method Name            : Display
Purpose                : To display the information of directory, it's subdirectory and files
Summary                : Checks whether string is directory or a file and displays File name, Permissions,Link/Directory count, User, Group, File-size and Time of a particular file.
Parameters              : Character Array, str: each record carries file/directory information
Returns                 : None
   
*/

    char ch;					                            // Holds Character 
    int i=0,count=0,j;				                            // Controls Loop
                     
    if(str[0]=='-'|| str[0]=='d'){ 
        
        if(str[0]=='-' )
            printf("\n\n-----------A FILE-----------");   
        else
    	    printf("\n\n-----------A DIRECTORY-----------");

        printf("\n\nName:\t\t\t\t");

        for(i=strlen(str);ch!=' ';i--){		                          // Length of Filename 
        
            ch=str[i];
            count++;
            
        }
      
        for(i=(strlen(str)-count)+1;str[i]!='\n';i++){	                  // Displaying Filename
        
            ch=str[i]; 
            printf("%c",ch); 
            
        }

/*

Outer loop chooses an attribute of file/directory and inner loop display the corresponding information.

*/     
   
        i=0;
        for(j=0;j<6;){        
            
            switch(j){
         
                case 0: printf("\nPermission:\t\t\t");
                        break;
                        
                case 1: printf("\nLink/Directory Count:\t\t\t");
                        break;
                        
                case 2: printf("\nUser:\t\t\t\t");
                        break;
                        
                case 3: printf("\nGroup:\t\t\t\t");
                        break;
                        
                case 4: printf("\nFile Size:\t\t\t\t");
                        break;
                        
                case 5: printf("\nTime:\t\t\t\t");
                        break;
                        
                default:break;
                
            }

            for(;i<=(strlen(str)-count);i++){
                
                ch=str[i]; 
                printf("%c",ch);
                
                if(ch==' ' && j<=5){
                    
     	     	    ++j;                                   		
	                while(ch==' ')						
                        ch=str[++i];
                    if(j<6)
                        break;
                        
                }        
            }         
        }
        count=0;
        
    }
    else if(str[0]=='.'){
        
        printf("\n\n-----------A SUBDIRECTORY-----------\n\n");
        
        for(i=0;str[i]!='\n';i++){
            
            ch=str[i]; 
            printf("%c",ch);
            
        }
    } 
    printf("\n");
    
}

int main(int argc, char *argv[1]) {				// Main Function 
   
    Read_Dir();
    return 0;
    
}




