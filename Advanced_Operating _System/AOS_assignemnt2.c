//ADVANCED OPERATING SYSTEM

/*
PROBLEM STATEMENT

Objective: Implementation of Distributed Linear Search using Linux System calls - An array is
split up into sub-arrays and searching the sub-arrays for an integer is delegated to other processes.

Input: The program should take two arguments as input - an array of integers to be read from a
file (first argument is the path to the file) and an integer to be searched (second argument).
Output: Print index of the number to be searched if found, or ‘Number not found’.

Method:
The main program reads the complete input array as a global array. The main program creates 2
processes and gives one half of the input array to the 2 child processes to search the required
number. If the size of the segment is small enough (say <= 5), then it searches the number in the
segment by itself. Otherwise, the task is delegated by creating more child processes wherein each
process is given a segment of the array to handle and the number to find.
If any process finds the required number, then it sends a signal to the main process and also returns
the index. After this, the main process kills all the other processes and prints the index. The code
should also handle the case when the number is not present in the array.
Assume PIPE as the chosen method of IPC mechanism.


Author               : © Shivani Tiwary-36, Surajit Das-41
Created on           : April 12, 2018 09:20 IST
Language             : C
Library Functions    : fork(),waitpid(..),kill(..),fgetc(..),fopen(..),setpgrp();
*/

/* Libraries */

#include <stdlib.h>
#include <stdio.h>		//For standard input output  
#include<sys/types.h>	        //For process IDs and process group IDs.
#include<unistd.h>		//Function definition of fork()
#include <sys/wait.h>		//Function definition of waitpid(..),WEXITSTATUS(..)
#include<signal.h>		//Function definition of kill(..),setpgrp();
#include <fcntl.h>		//File control options

#define MAX 20
int a[MAX];			//Global Array

int search(int start,int last,int s){

/* 

Method Name            :  search

Purpose                :  To search for an integer in given subarray

Summary                :  An element is searched from a subarray with start and end index. Returns the index of the element if successfully found or return a negative value if element not found.

Parameters             :  start	 :  Start index of subarray
		          last   :  Last index and also serves the purpose for the size of subarray
		          s      :  The element to search

Returns                 : 
                          index		- if the element to search is found
                          Negative	- if element to search is not found	  
   
*/

    
    int index,flag=0;
    
    for(index=start;index<=last;index++){

	if(a[index]==s){
		    
            flag=1;
            break;
            
        }                      
    }					  
    if(flag==1)
        return index+1;
    else{
        
        printf("Element not found!!!\n");
        return(-1);
        
    }
}

int partition(int a[],int start,int last,int s){

/* 

Method Name       : partition

Purpose           : Breaks the array into subarray recursively until array size is (<=5). If size of array is less than 5 it calls the search procedure to search for a particular element.

Summary           : This function uses concept of fork() function to assign the tasks to child process, pipe() and signals(kill) for Interprocess Communication between parent and child process. At first the function checks if size of array is (<=5), then call search procedure else child process breaks the array recursively until size is (<=5) and further searches for the given element. The array is partitioned in such a manner that first half of the array is given to one child and other half is given to other.

Parameters          : a      	:   Global array 
                      start 	:   Start index of subarray
		      last  	:    Last index and also serves the purpose for the size of subarray
		      s      	:   The element to search

Variables           : c	        :   stores the value returned by search procedure
                      status 	:   contains the exit status returned by child process 
                      mpipe[2]  :   mpipe[0] will be the fd(file descriptor) for the read end of pipe and pp[1] will be the fd for the write end of pipe               
                      buff[2]   :  buffer for storing start and end index
                      pid       :  contains process id of a process

Returns             : integer
   
*/

    int i,c=-1,status,mpipe[2],buff[2];
    pid_t pid,wpid;
    pipe(mpipe);
     
    setpgrp();
    
    if(start-last<=5){
        
        c=search(start,last,s);
        
        if(c>=1){
            
            printf("Element %d found at Position %d\n",s,c);
            exit(c);
            
        }
        else
            exit(0);
            
    }
    else{

/* Process calls fork(), creates two child process which further works on the subarray.*/

        buff[0]=start;
	buff[1]=((start+last)/2)-1;
	write(mpipe[1],buff,2*sizeof(int));         /* Parent process writing in pipe. */
	pid=fork();
	if(pid==0){
		    
	    read(mpipe[0],buff,2*sizeof(int));      /* Child process reading from pipe. */

	    start=buff[0];
    	    last=buff[1];
	    partition(a,start,last,s);
			
	}
    	else{
            
            wpid=waitpid(-1,&status,0);            /* Parent process waiting for the child to terminate. */
            
/*	    if(WEXITSTATUS(status)>0){      

            	printf("\nExit status: %d\n",WEXITSTATUS(status));    
    		kill(0,SIGINT);  
    			
            }*/
            
            buff[0]=(start+last)/2;
	    buff[1]=last;
	    write(mpipe[1],buff,2*sizeof(int)); 
	    pid=fork();
			
	    if(pid==0){
			        
	        read(mpipe[0],buff,2*sizeof(int));
	        start=buff[0];
                last=buff[1];
                partition(a,start,last,s);
                    
            }
	    else{
	 		        
		wpid=waitpid(-1,&status,0);
			    
/*              if(WEXITSTATUS(status)>0){  

                           printf("\nExit status: %d\n",WEXITSTATUS(status));     
 		    kill(0,SIGINT); 
 			            
                }*/
	   }
       }
    }
}

int main(int argc,char *argv[]){
  
/* 

Method Name       :  main 

Purpos            :  To read the file and call procedure to search for the given element

Parameters        :  None

Returns           :  Integer
   
*/


   FILE *fp;					   /* A file pointer to FILE structure */
   int num=0,s,i,size,len=0,flag=0;
   int p=-1;
   char ch;

   fp=fopen(argv[1],"r");                           /* Opening an existing file for reading passed through cmd */
  
   if (fp == NULL){
       
        perror("Error while opening the file.\n");
        exit(1);
        
       
   }
  len=0;

 /* Reading from File */

  while((ch = fgetc(fp)) != EOF){
      
        if(ch>='0' && ch<='9'){
        
            num=num*10+(ch-'0');
            flag=1;
        
        }
        else{
        
            if(flag!=0)
            a[len++]=num;
            num=0;
            flag=0;
        }
    }

    printf("\nEnter search element: ");
    scanf("%d",&s);

    partition(a,0,len-1,s);
 
    return 0;
    
}



