#include<stdio.h>
#include<stdlib.h>
#include<string.h>

typedef struct human{
 char* name;
 int age;
 int dob;
 struct human *next;
};

static void append(int i, int j, char *value , struct human *a,struct human *b);

int main(){
struct human* man;
struct human* man1;
struct human man2;
man=(struct human *) malloc(sizeof(struct human));
man1=(struct human *) malloc(sizeof(struct human));
man->age=52;
man->name="Mindy";
man->dob=2491992;
man->next=man1;
man1->age=34;
man1->name="cindy";
man1->dob=12345;
man1->next=NULL;
man2.name="CATHY"; 
//man->name="Cathy";
man2.age=42;
man2.dob=281992;
man2.next=NULL;
//printf("The name of the main is %s",manname);
//printf("The age of the man2 is %d\n",man2.age);
//printf("The dob of the man2 is %d\n",man2.dob);
printf("The age of the man is %d\n",man1->age);
printf("The name of man 1 is %s\n", (man->next)->name);
printf("The dob of man 1 is %d\n",man->dob);
printf("Appending new elements to the list\n");
int a =12;
int b=42;
char * name = "Parul";
//append(a,b,name,&man2,NULL);
struct human man3;
append(a,b,name,&man3,&man2);
return 0;
}

static void append(int i, int j, char *name, struct human *a,struct human *b)
{
struct human *tmp;
struct human *start;
tmp = a;
start=b;
tmp= (struct human*)malloc(sizeof(struct human));
tmp->age=i;
tmp->dob=j;
tmp->name=name;
tmp->next=NULL;
if(b->next==NULL)
  {
	b->next=tmp;
}
printf("The temporary value of age in this construct :%s\n",(b->next)->name);
printf("The temporary value of age in this construct :%s\n",tmp->name);

}
