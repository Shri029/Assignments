#include <stdio.h>
#include <stdlib.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<unistd.h>
#include<signal.h>
#include <sys/wait.h>
#include<string.h>
#include <fcntl.h>

#define MAX 20

int a[MAX];

int search(int start,int last,int s){
    
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
    
    int i,c=-1,status,flag=1,mpipe[2],buff[2];
    pid_t pid,wpid,ppid=getpid();
     
    setpgrp();
    
    if(start-last<=5){
        
        c=search(start,last,s);
        
        if(c>1){
            
            printf("Element %d found at Position %d\n",s,c);
            exit(c);
            
        }
        else
            exit(0);
            
    }
    else{

        buff[0]=start;
	buff[1]=((start+last)/2)-1;
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
  
   FILE *fp;
   int num=0,s,i,size,len=0,flag=0;
   int p=-1;
   char ch;
   pid_t pid;
   fp=fopen(argv[1],"r");
  
   if (fp == NULL){
       
        perror("Error while opening the file.\n");
        exit(1);
        
       
   }
  len=0;

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
