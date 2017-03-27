# include <stdio.h>
# include <dlfcn.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdlib.h>

#define NUM2 1024*1024*1024
#define NUM 1
//int lookup(int a, int array[], int len);


int main(){
	char *ptr;
	void *handle;
	int index,input,key;
/*
	length=1024*1024;
	int arr[length];
	for(i=0;i<length;i++)
		{	arr[i]=i+13;
	
		}
*/
	/*struct entry {
        int key;
	int value;
	struct entry next_entry*;}
	struct entry *entry_s;
	create(&entry_s,1024*1024);
	entry_s = (struct_entry) malloc (sizeof(struct entry));
*/
/*
	for(i=0;i<length;i++)
        {printf("%d\n",arr[i]);
        }
*/
	/* step 1: declare pointer that can hold library symbols we intend to access */
//	int (*fnptr)(int,int);
//	int (*fptr)(int,int[], int);
        int (*fptr)(int);
	input =21 ; 
	key = 1190;
       // index = ((input)*(~key)) | ((~input)*(key));
	index = ( input & ~key) | ( ~input & key);
//        getchar();

	/* step 2: request link-loader to load required library */
	handle = dlopen("./libtest.so",RTLD_NOW);
	if(handle == NULL){
		printf(" Failed to load library \n");
		exit(EXIT_FAILURE);
	}
	/* step 3: request link-loader to return address of the symbol 
  
                 which is to be accessed */ 
//        i =0;
	//for(i=0;i<NUM;i++)
       //while(1)
      // {
/*	fnptr = dlsym(handle,"somefunc");
	if(!fnptr)
	{ fprintf(stderr,"dlsym failure :%s\n",dlerror());
	  exit(EXIT_FAILURE);
	  }	*/
	fprintf(stdout,"The dynamic library function has been loaded successfully\n");
	fprintf(stdout,"Executing.......\n");
//	getchar();
// step 4: invoke library function using pointer 
/*
	while(1)
	{
	i= (fnptr) (10,20);
	printf(" result of function = %d\n",i);
	sleep(1);	
         }
  */    
       fptr=dlsym(handle,"lookup");
       if(!fptr)
        { fprintf(stderr,"dlsym failure :%s\n",dlerror());
          exit(EXIT_FAILURE);
          }
       int result,result1, random;
       time_t t;
       srand((unsigned) time(&t));
       random = rand() % 4095;
       fprintf(stdout,"The random index is %d\n ",index);
       getchar();
       while(1){
//       result = (fptr)(163);
       result1 = (fptr)(index);
       //result = (fnptr)(20,50);
//       sleep(2);
  //     printf("%d\n",result);
       printf("%d\n",result1);
	}	
	/* step 5: request link-loader to unload/detach library when no longer needed */ 
	dlclose(handle);
//	getchar();
	return 0;
}
/*
int lookup(int num, int a[], int length){
int i;
int flag =0;
for(i=0;i<length;i++)
{
        if(num==a[i])
        {
        flag=1;
        printf("Number is found at %d\n",i);
}

}
return flag;
}
*/	
