#include<stdio.h>
#include<stdint.h>
#include<stdlib.h>
#include<string.h>

#define MAX(a,b) a>b?a:b
void new_letter(int *b)
{
//int a_b = 13;
 *b = 13;
//return &a;
}

void type_character(int *a){
//char a = 'a';
//return &a;
 *a=14;
}

struct function{
int a_b;
void (*x)(int*j);
void (*y)(int*k);
};

int main()
{
/*
int a = 3;
int b = 4;

int c = MAX(a,b);
printf("%d",c);
*/

/*struct function{
int *new_letter();
char *type_character();
};*/
struct function print;
print.a_b =32;
//struct function print={2,new_letter(&a),type_character(&a)};
printf("%d\n",print.a_b);
print.x = new_letter;
print.x(&print.a_b);
printf("%d\n",print.a_b);



return 0;
}

