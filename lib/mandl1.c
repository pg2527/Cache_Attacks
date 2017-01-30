# include <stdio.h>
# include <dlfcn.h>

#define NUM2 1024*1024*1024
#define NUM 1024*1024
int lookup(int a, int array[], int len);

int main(){
	char *ptr;
	void *handle;
	int i,k,length;
	length=1024*1024;
	int arr[length];
	for(i=0;i<length;i++)
		{	arr[i]=i+13;
	
		}

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
	int (*fnptr)(int,int);
	int (*fptr)(int,int[], int);
//	getchar();

	/* step 2: request link-loader to load required library */
	handle = dlopen("./libxyz.so",RTLD_NOW);
	if(handle == NULL){
		printf(" Failed to load library \n");
		exit(2);
	}
	/* step 3: request link-loader to return address of the symbol 
  
                 which is to be accessed */ 
        i =0;
	for(i=0;i<NUM;i++)
       {
	fnptr = dlsym(handle,"somefunc");
//	getchar();
// step 4: invoke library function using pointer 
	i= (fnptr) (10,20);
	printf(" result of function = %d\n",i);
       }

       fptr=dlsym(handle,"lookup");
       int result;
       while(1){
       result = (fptr)(10000, arr, length);
//       sleep(2);
       printf("%d\n",result);}	
	/* step 5: request link-loader to unload/detach library when no longer needed */ 
//	dlclose(handle);
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
