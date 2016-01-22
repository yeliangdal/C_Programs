/*
   Ye Liang
   A program that solves classic N queen puzzle using backtracking method, created for CSCI2132 Assignment
   
*/
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

/*Prototypes*/
char** new2Dchar(int, int);
void free2D(char**);
void refresh(char**, int);
void print2D(char**, int);
int valid(int*, int);
bool queenConfig(int*, int, int);

//main function
int main(){
   
   int n;
   char **a;
   int *p;
   
   
   while(scanf("%d",&n)==1&&n!=0)
   {
      //create a 2-d array of char of size nxn 
      a=new2Dchar(n,n);
      
      //create an array of int of size n, it represents the configuration of queens
      p =(int*) malloc(n*sizeof(int));
            
      //initialize the 2-d array, set all elements to '.'
      refresh(a,n);
      int i;
      
      //initialize the array with value -1
      for(i=0;i<n;i++)
      {
         p[i]=-1;
      }
      
      //check if there is a solution
      bool chk=queenConfig(p, n, 0);
      //print result
      printf("%d x %d board:\n", n,n);
      if(chk==false)
         printf("No valid configuration found\n");
      else
      {
         for(i=0;i<n;i++)
         {
            if(p[i]>=0)
               a[n-1-p[i]][i]='Q';
         }
         print2D(a,n);
          
      }
      printf("\n");
      free2D(a);
      free(p);
   } 
   return 0;
}

char** new2Dchar(int n, int m) {
   int i;
   char **ret = (char**) malloc(n*sizeof(char*));
   char *a = (char*) malloc(n*m*sizeof(char));
   for (i=0; i<n; ++i) 
      ret[i] = &a[i*m];
   return ret;
}

void free2D(char **a) { 
   free(a[0]); 
   free(a); 
}
//helper function that refills a 2-D array of char with "."
void refresh(char **a, int n){
   int i=0,j=0;
   for(i=0;i<n;i++)
   {
      for(j=0;j<n;j++)
      {
         a[i][j]='.';
      }
   }
}
//helper function that prints out a 2-d array 
void print2D(char **a, int n){
   int i=0,j=0;
   for(i=0;i<n;i++)
   {
      for(j=0;j<n;j++)
      {
         printf("%c ",a[i][j]);
      }
      printf("\n");
   }
}

//this function validates whether there are two queens on the board attacking each other or not in a given configuration
//it returns 0 if it is valid, otherwise it returns 1
int valid(int *p, int n){
   int i,j,check=0;;
   //check same row
   for(i=0;i<n-1&&check==0;i++)
   {
      for(j=i+1;j<n&&check==0;j++)
      {
         if(p[i]==p[j]&&p[i]!=-1)
            check=1;
      }
   }
   //check diagonal
   for(i=0;i<n-1&&check==0;i++)
   {
      for(j=i+1;j<n&&check==0;j++)
      {
         if(p[i]!=-1&&p[j]!=-1&&((p[i]-p[j])==(j-i)||(p[i]-p[j])==(i-j)))
            check=1;
      }
   }
   return check;
}

//method that returns true if there is a solution to n queen problem 
bool queenConfig(int* p, int n, int col)
{
   int i;
    //base case
   if (col >= n)
        return true;
 
   // for each colomn, try to place a queen in all rows one by one
   for ( i= 0; i < n; i++)
   {
      p[col]=i;
      if(valid(p,n)==0)
      {
         if ( queenConfig(p,n,col+1) == true )
            return true; 
         else
            p[col]=-1;
      }
      else
         p[col]=-1;
        
   }
   //no solution found
   return false;
}
