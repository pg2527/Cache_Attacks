#include <unistd.h>
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>

int main()
{

pid_t pid[2];
int i;
for(i =0; i<2;i++)
{
pid[i]=fork();
if(pid[i]==0)
{
printf("in the childprocess with pid %d\n",getpid());
exit(EXIT_FAILURE);
}
else
{
printf("Could not create the child process %d\n",i);
}
}
printf("Inside the Parent Process\n");
int wait;
int status;
for(i=0;i<2;i++)
{	sleep(3);
	wait =waitpid(pid[i],&status,NULL);
 	perror("wait");
}

return 0;
}

