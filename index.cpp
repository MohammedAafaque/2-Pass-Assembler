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

struct opcode{
    char m[5];
    char val[10];
    int ind;
}O[100];

void disect(char line[], struct state *s){
    int i, j=0, k=0,len=strlen(line);
    for(i=0; i<len; i++)
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
    for(; i<len; i++)
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
    for(; i<len; i++)
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
    int i,loc, st;
    if(strcmp(s[0].ins, "START")==0)
     {     
        FILE *f1 = fopen("startaddress.txt", "r");
        fscanf(f1, "%x", &st);
        loc = st;
        s[1].add = loc;
        s[0].add =loc;
        fclose(f1);

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

void pass2(struct state *s, int n){
	int i;
	char instruction[10];
    char label[10];
    char symbol[10];
	for(i=1;i<n;i++)
	{   strcpy(label, s[i].l);
		strcpy(instruction, s[i].ins);
        strcpy(symbol, s[i].s);
		if(strcmp(instruction,"RESW")==0)
		{
			s[i].oc = -0x1;
		}
		else if(strcmp(instruction,"WORD")==0)
		{
				int val = (int)strtol(symbol, NULL, 16);
				s[i].oc = val;
		}
		else if(strcmp(instruction,"RESB")==0)
		{
				s[i].oc = -0x1;
		}
		else if(strcmp(instruction,"BYTE")==0)
		{
				if(symbol[0]=='C')
				{
					s[i].oc = 0x454F46;
				}
				else if(symbol[0] == 'X')
				{
					int j=2,k=0,val;
					char dev[10];
					while(symbol[j]!='\'')
					{
						dev[k] = symbol[j];
						k++;
						j++;
					}
					val = (int)strtol(dev,NULL,16);
					s[i].oc=val;
				}
		}
		else if(strcmp(instruction,"END")==0)
		{
			s[i].oc = -0x1;
		}
		else
		{
			int j=0, k=0, sop, val, vbit;
			char *ret, bit[5], temp[4], source[10], destination[10];
            // char t[10];
            // t = s[k].l;
			while(strcmp(instruction, O[j].m)!=0)
			{
				j++;
			}
			strcpy(destination, O[j].val);
			
			while (strcmp(symbol,s[k].l)!=0)
            {  
                k++;
            }
            sop = s[k].add;
            itoa(sop, source, 16);

			ret = strstr(symbol, ",X");
			if(ret)
			{
				bit[0]=source[0];
				vbit = (int)strtol(bit, NULL, 16);
				vbit = vbit + 8;
				itoa(vbit, bit, 16);
				source[0] = bit[0];
			}
			else
			{
				strcpy(temp, source);
			}
			strcat(destination, source);
			val = (int)strtol(destination, NULL, 16);
			s[i].oc = val;
		}
		
	}
	
}

void createSymtab(struct state s[], int n){
    int i;
    FILE *f = fopen("symtab.txt", "w");
    
    for(i=1; i<n; i++)
    {
        if(strcmp(s[i].l,"****")!=0)
        {
            fprintf(f, "%x\t%s\n", s[i].add, s[i].l);
        }
    }
    fclose(f);
}

void disect2(char line[], struct opcode *o){
    int i, j=0,len=strlen(line);
    for(i=0; i<len; i++)
    {   if(line[i]!=' ')
        o->m[i]=line[i];
        else
        break;
    }
    i++;
    for(; i<len; i++)
    {
        if(line[i]!=' ')
        {o->val[j]=line[i];
         j++;
        }
        else
        break;
    }
}

void createOptab(struct state s[], int n){
    char line[10];
    int m=0, j, i;
    FILE *f1 = fopen("opcodes.txt", "r");
    FILE *f2 = fopen("optab.txt", "w");
    while(fgets(line, 10, f1))
    {
        disect2(line, &O[m]);
        m++;
    }

    for(i=0; i<n; i++)
    {
        for(j=0; j<m; j++)
        {
            if(strcmp(s[i].ins, O[j].m)==0)
            {
                fprintf(f2, "%s\t%s" ,O[j].m, O[j].val);
                
            }
            else{

            }
        }
    }
    fclose(f1);
    fclose(f2);

}

void display(struct state s[], int n){
    int i;
    for(i=0;i<n;i++)
    if((strcmp(s[i].ins, "START")==0) || (strcmp(s[i].ins, "END")==0) || (strcmp(s[i].ins, "RESW")==0) || (strcmp(s[i].ins, "RESB")==0)) 
    {
        printf("%x\t", s[i].add);
        printf("%s\t", s[i].l);
        printf("%s\t", s[i].ins);
        printf("%s\n", s[i].s);
    }
    else
    {
        printf("%x\t", s[i].add);
        printf("%s\t", s[i].l);
        printf("%s\t", s[i].ins);
        printf("%s\t", s[i].s);
        printf("%06x\n",s[i].oc);
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
    createSymtab(S, n);
    createOptab(S,n);
    pass2(S, n);
    display(S, n);
}


