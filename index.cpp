#include<stdio.h>
#include<conio.h>
#include<stdlib.h>
#include<string.h>

using namespace std;

struct state{
    char l[10];
    char ins[10];
    char s[10];
    char add[10];
    char oc[10];
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

void display(struct state s[], int n){
    int i;
    for(i=0;i<n;i++)
    {
        printf("%s\t%s\t%s", s[i].l, s[i].ins, s[i].s);
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
    display(S, n);
}