#include <stdio.h>
#include <ctype.h>
int main(int argc, char *argv[]){

FILE *file;
file=fopen(argv[1],"r");

if(file==NULL){
printf("False");
return 1;
}

int c;
int count=0;
int line=1;
char p=0;
while((c=fgetc(file))!=EOF){
if (isspace(c) && !isspace(p) && p!=0){
    count+=1;
}

if(c=='\n'){
printf("The number of words on the line %d is %d\n",line,count);
count=0;
line+=1;
}
else
p=c;
}

fclose (file);
return 0;
}
