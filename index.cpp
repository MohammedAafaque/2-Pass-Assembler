#include<stdio.h>
#include<conio.h>
#include<stdlib.h>
#include<string.h>

using namespace std;

struct state{
    char l[10];
    char ins[10];
    char s[10];
    int add;
    int oc;
}S[100];

void disect(char line[], struct state *s){
    int i, j=0, k=0;
    for(i=0; i<strlen(line); i++)
    {
       if(line[i]!=' ')
       {
           s->l[i] = line[i];
       }
       else{
           break;
       }
    }
    i++;
    for(; i<strlen(line); i++)
    { 
       if(line[i]!=' ')
       {
           s->ins[j] = line[i];
           j++;
       }
       else{
           break;
       }
    }
    i++;
    for(; i<strlen(line); i++)
    { 
       if(line[i]!=' ')
       {
           s->s[k] = line[i];
           k++;
       }
       else{
           break;
       }
    }
}

void pass1(struct state *s, int n){
    int i,loc;
    char start[10] = "START";
    if(strcmp(s[0].ins, "START")==0)
     {     
        loc = atoi(s[0].s);
        s[1].add = loc;
        s[0].add = loc;
     }
        else{
        printf("No start location for the program.");        
        }

    for(i=1; i<n; i++)
    {
        if(strcmp(s[i].ins,"WORD")==0)
        {
            loc=loc+3;
        }
        else if(strcmp(s[i].ins, "RESW")==0)
        {
            int x = atoi(s[i].s);
            loc = loc+(x*3);
        }
        else if(strcmp(s[i].ins, "BYTE")==0)
        {   if(s[i].s[0]=='C')
            {   int c=0;
                int j=2;
                while(s[i].s[j]!='\'')
                {
                    c++;
                    j++;
                }
                loc = loc+(c*1);
            }
            else if(s[i].s[0]=='X')
            {
                loc = loc+1;
            }
        }
        else if(strcmp(s[i].ins, "RESB")==0)
        {
            int x = atoi(s[i].s);
            loc = loc+(x*1);
        }
        else if(strcmp(s[i].ins, "END")==0)
        {

        }
        else{
            loc=loc+3;
        }
        
    s[i+1].add = loc;
    

    }
}



void display(struct state s[], int n){
    int i;
    for(i=0;i<n;i++)
    {
        printf("%d\t%s\t%s\t%s",s[i].add, s[i].l, s[i].ins, s[i].s);
    }
}

int main(){
    int n=0;
    char line[100];
    FILE *f = fopen("input.txt", "r");
    while(fgets(line, 100, f))
    {   
        disect(line, &S[n]);
        n++;
    }
    pass1(S, n);
    display(S, n);
}