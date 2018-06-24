#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define SIZE 150

int main(){
	char getStr[SIZE];
	char *token;
	char * pch;
	char *test;
	
	FILE *read;
    FILE *write;
    read = fopen("Sample.c","r");
    write = fopen("Output.txt","w");
    
    int i =0;
    int j = 0;
    int except=0;
    int line=0;
    int start=0;
    int escape=0;
    int iden=0;    
    int sign=0;
    int digit=0;
    int stop=0;
    int MC=0;
    int startLine=0;
    char error[SIZE];
    char test1[SIZE];
    
    if(read==NULL){
    	printf("ÅªÀÉ¥¢±Ñ");
	}
	else{
		while(fgets(getStr,SIZE,read)!=NULL){
			line++;	
				
			i=0;
			except=0;
			MC=0;
			while(getStr[i]!='\n'&&except>=0){				
				iden=0;				
				switch(getStr[i]){	
					case'.':
						i++;
						if(getStr[i]>='0'&&getStr[i]<='9'){
							i++;
							while(getStr[i]>='0'&&getStr[i]<='9'){
								i++;
							}
							if(getStr[i]=='E'||getStr[i]=='e'){
								i++;
								if(getStr[i]=='-'||getStr[i]=='+'){
									i++;									
								}
								while(getStr[i]>='0'&&getStr[i]<='9'){
									i++;
								}
								memset(test1, '\0', sizeof(test1));	
								strncpy(test1,getStr+start,i-start);
								fprintf(write,"%d\tFLOT\t%s\n",line,test1);
								memset(test1, '\0', sizeof(test1));								
							}
						}
						else{
							i--;
							fprintf(write,"%d\tOPER\t.\n",line);
						}
						break;										
					case '#':						
						fprintf(write,"%d\tPREP\t%s",line,getStr);
						except=-1;															
						break;					
					case ';':
					case '(':
					case ')':
					case '{':
					case '}':
						fprintf(write,"%d\tSPEC\t%c\n",line,getStr[i]);												
						break;	
					case '\'':
						start=i;
						i++;
						escape=0;						
						while(getStr[i]!='\''&&i<SIZE){
							if(getStr[i]=='\\'){
								escape--;
								i++;
								if(getStr[i]!='\'')
									escape--;
							}
							i++;
						}
						
						if(getStr[i]!='\''){
							strncpy(error,getStr,start);
							fprintf(write,"%d\tSCAN ERROR: only single ' %s\'\n",line,error);							
							i=start;							
						}					
						else if(getStr[i]=='\''){	
							memset(test1, '\0', sizeof(test1));	
							strncpy(test1,getStr+start,i-start);
							test1[i-start]='\0';
							fprintf(write,"%d\tCHAR\t%s\'\n",line,test1);	
							memset(test1, '\0', sizeof(test1));												
						}
						
						start=0;
						break;				
					case '\"':
						start=i;						
						i++;
						escape=0;
						while(getStr[i]!='\"'&&i<SIZE){
							if(getStr[i]=='\\'){
								i++;
								if(getStr[i]=='\"')
									escape--;								
							}
							i++;
						}
						if(getStr[i]!='\"'){
							strncpy(error,getStr,start);
							fprintf(write,"%d\tSCAN ERROR: only single \" %s\"\n",line,error);							
							i=start;							
						}					
						else if(getStr[i]=='\"'){
							int check=0;
							test="\"";	
							strncpy(test1,getStr+start,i-start);
							test1[i-start]='\0';
							fprintf(write,"%d\tSTR\t%s\"\n",line,test1);	
							memset(test1, '\0', sizeof(test1));				
						}
						start=0;
						break;
					case '/':
						start=i;
						i++;
						startLine=line;
						if(getStr[i]=='/'){
							test="/";
							pch = strtok(getStr,test);
							if(i==1){
								fprintf(write,"%d\tSC\t%s",line,getStr);
							}
							else{
								pch = strtok(NULL,test);
								fprintf(write,"%d\tSC\t//%s",line,pch);
																
							}
							except=-1;							
						}
						else if(getStr[i]=='='){
							fprintf(write,"%d\tOPER\t/=\n",line);
						} 
						else if(getStr[i]=='*'){
							while(getStr[i]!='\n'){
								i++;
								if(getStr[i]=='*'&&getStr[i++]=='/')
									continue;
							}
							if(getStr[i]=='*'){
								i++;
								if(getStr[i]=='/'){
									memset(test1, '\0', sizeof(test1));	
									strncpy(test1,getStr+start,i-start+1);
									fprintf(write,"%d-%d\tMC\t\n",startLine,line);									
									memset(test1, '\0', sizeof(test1));	
								}
							}
							else{								
								while(MC==0){	
									if(fgets(getStr,SIZE,read)!=NULL){
										line++;
										i=0;
										except=0;
										while(getStr[i]!='\n'){										
											if(getStr[i]=='*'){
												i++;
												if(getStr[i]=='/'){
													MC=-1;
													memset(test1, '\0', sizeof(test1));	
													strncpy(test1,getStr,i+1);
													fprintf(write,"%d-%d\tMC\t\n",startLine,line);
													memset(test1, '\0', sizeof(test1));												
													MC=-1;
													break;
												}			
											}
											else{
												i++;
											}										
										}									
									}	
									else{
										memset(test1, '\0', sizeof(test1));	
										strncpy(test1,getStr,i+1);									
										fprintf(write,"%d-%d\tMC but token */ miss \n",startLine,line);
										memset(test1, '\0', sizeof(test1));	
										except=-1;
									}							
									
								}							
							}							
						}
						else{
							fprintf(write,"%d\tOPER\t/\n",line);
							i--;
						}
						break;
					case',':
					case'[':
					case']':
					case'^':
					case':':
						fprintf(write,"%d\tOPER\t%c\n",line,getStr[i]);
						break;
					case'+':
						i++;
						switch(getStr[i]){
							case'+':
								fprintf(write,"%d\tOPER\t++\n",line);
								break;
							case'=':
								fprintf(write,"%d\tOPER\t+=\n",line);
								break;
							default:								
								fprintf(write,"%d\tOPER\t+\n",line);
								i--;
								break;
						}
						break;
					case'-':
						i++;
						switch(getStr[i]){
							case'-':
								fprintf(write,"%d\tOPER\t--\n",line);
								break;
							case'=':
								fprintf(write,"%d\tOPER\t-=\n",line);
								break;
							case'>':
								fprintf(write,"%d\tOPER\t->\n",line);
								break;
							default:
								fprintf(write,"%d\tOPER\t-\n",line);
								i--;
								break;
						}
						break;
					case'*':
						i++;
						switch(getStr[i]){							
							case'=':
								fprintf(write,"%d\tOPER\t*=\n",line);
								break;
							default:
								fprintf(write,"%d\tOPER\t*\n",line);
								i--;
								break;
						}
						break;
					case'=':
						i++;
						switch(getStr[i]){							
							case'=':
								fprintf(write,"%d\tOPER\t==\n",line);
								break;
							default:
								
								fprintf(write,"%d\tOPER\t=\n",line);
								i--;
								break;
						}
						break;
					case'%':
						i++;
						switch(getStr[i]){							
							case'=':
								fprintf(write,"%d\tOPER\t%=\n",line);
								break;							
							default:
								fprintf(write,"%d\tOPER\t%\n",line);
								i--;
								break;
						}
						break;
					case'>':
						i++;
						switch(getStr[i]){	
							case'>':
								fprintf(write,"%d\tOPER\t>>\n",line);
								break;							
							case'=':
								fprintf(write,"%d\tOPER\t>=\n",line);
								break;							
							default:
								fprintf(write,"%d\tOPER\t>\n",line);
								i--;
								break;
						}
						break;	
					case'<':
						i++;
						switch(getStr[i]){	
							case'<':
								fprintf(write,"%d\tOPER\t<<\n",line);
								break;							
							case'=':
								fprintf(write,"%d\tOPER\t<=\n",line);
								break;							
							default:
								fprintf(write,"%d\tOPER\t<\n",line);
								i--;
								break;
						}
						break;
					case'!':
						i++;
						switch(getStr[i]){							
							case'=':
								fprintf(write,"%d\tOPER\t!=\n",line);
								break;							
							default:
								fprintf(write,"%d\tOPER\t!\n",line);
								i--;
								break;
						}
						break;	
					case'&':
						i++;
						switch(getStr[i]){							
							case'&':
								fprintf(write,"%d\tOPER\t&&\n",line);
								break;							
							default:
								fprintf(write,"%d\tOPER\t&\n",line);
								i--;
								break;
						}
						break;	
					case'|':
						i++;
						switch(getStr[i]){							
							case'|':
								fprintf(write,"%d\tOPER\t||\n",line);
								break;							
							default:
								fprintf(write,"%d\tOPER\t|\n",line);
								i--;
								break;
						}
						break;
					case'a':
						start=i;						
						i++;
						while(iden==0){
							if((getStr[i]>='0'&&getStr[i]<='9')||(getStr[i]>='A'&&getStr[i]<='Z')
								||(getStr[i]>='a'&&getStr[i]<='z')||(getStr[i]=='_')){									
								i++;	
							}
							else{
								i--;
								iden=-1;
							}										
						}							
						iden=-1;
						memset(test1, '\0', sizeof(test1));	
						strncpy(test1,getStr+start,i-start+1);
						
						if(strcmp(test1,"auto")==0)
							fprintf(write,"%d\tREWD\tauto\n",line);
						else								
							fprintf(write,"%d\tIDEN\t%s\n",line,test1);				
						memset(test1, '\0', sizeof(test1));						
						break;
					case'b':
						start=i;						
						i++;
						while(iden==0){
							if((getStr[i]>='0'&&getStr[i]<='9')||(getStr[i]>='A'&&getStr[i]<='Z')
								||(getStr[i]>='a'&&getStr[i]<='z')||(getStr[i]=='_')){									
								i++;	
							}
							else{
								i--;
								iden=-1;
							}										
						}							
						iden=-1;
						memset(test1, '\0', sizeof(test1));	
						strncpy(test1,getStr+start,i-start+1);
						
						if(strcmp(test1,"break")==0)
							fprintf(write,"%d\tREWD\tbreak\n",line);
						else								
							fprintf(write,"%d\tIDEN\t%s\n",line,test1);				
						memset(test1, '\0', sizeof(test1));						
						break;
					case'c':
						start=i;						
						i++;
						while(iden==0){
							if((getStr[i]>='0'&&getStr[i]<='9')||(getStr[i]>='A'&&getStr[i]<='Z')
								||(getStr[i]>='a'&&getStr[i]<='z')||(getStr[i]=='_')){									
								i++;	
							}
							else{
								i--;
								iden=-1;
							}										
						}							
						iden=-1;
						memset(test1, '\0', sizeof(test1));	
						strncpy(test1,getStr+start,i-start+1);
						
						if(strcmp(test1,"case")==0)
							fprintf(write,"%d\tREWD\tcase\n",line);
						else if(strcmp(test1,"continue")==0)
							fprintf(write,"%d\tREWD\tcontinue\n",line);
						else if(strcmp(test1,"char")==0)
							fprintf(write,"%d\tREWD\tchar\n",line);
						else if(strcmp(test1,"const")==0)
							fprintf(write,"%d\tREWD\tconst\n",line);	
						else								
							fprintf(write,"%d\tIDEN\t%s\n",line,test1);				
						memset(test1, '\0', sizeof(test1));						
						break;
					case'd':
						start=i;						
						i++;
						while(iden==0){
							if((getStr[i]>='0'&&getStr[i]<='9')||(getStr[i]>='A'&&getStr[i]<='Z')
								||(getStr[i]>='a'&&getStr[i]<='z')||(getStr[i]=='_')){									
								i++;	
							}
							else{
								i--;
								iden=-1;
							}										
						}							
						iden=-1;
						memset(test1, '\0', sizeof(test1));	
						strncpy(test1,getStr+start,i-start+1);
						
						if(strcmp(test1,"do")==0)
							fprintf(write,"%d\tREWD\tdo\n",line);
						else if(strcmp(test1,"default")==0)
							fprintf(write,"%d\tREWD\tdefault\n",line);
						else if(strcmp(test1,"double")==0)
							fprintf(write,"%d\tREWD\tdouble\n",line);
						else								
							fprintf(write,"%d\tIDEN\t%s\n",line,test1);				
						memset(test1, '\0', sizeof(test1));						
						break;
					case'e':
						start=i;						
						i++;
						while(iden==0){
							if((getStr[i]>='0'&&getStr[i]<='9')||(getStr[i]>='A'&&getStr[i]<='Z')
								||(getStr[i]>='a'&&getStr[i]<='z')||(getStr[i]=='_')){									
								i++;	
							}
							else{
								i--;
								iden=-1;
							}										
						}							
						iden=-1;
						memset(test1, '\0', sizeof(test1));	
						strncpy(test1,getStr+start,i-start+1);
						
						if(strcmp(test1,"else")==0)
							fprintf(write,"%d\tREWD\telse\n",line);
						else if(strcmp(test1,"extern")==0)
							fprintf(write,"%d\tREWD\textern\n",line);
						else if(strcmp(test1,"enum")==0)
							fprintf(write,"%d\tREWD\tenum\n",line);	
						else								
							fprintf(write,"%d\tIDEN\t%s\n",line,test1);				
						memset(test1, '\0', sizeof(test1));						
						break;
					case'f':
						start=i;						
						i++;
						while(iden==0){
							if((getStr[i]>='0'&&getStr[i]<='9')||(getStr[i]>='A'&&getStr[i]<='Z')
								||(getStr[i]>='a'&&getStr[i]<='z')||(getStr[i]=='_')){									
								i++;	
							}
							else{
								i--;
								iden=-1;
							}										
						}							
						iden=-1;
						memset(test1, '\0', sizeof(test1));	
						strncpy(test1,getStr+start,i-start+1);
						
						if(strcmp(test1,"for")==0)
							fprintf(write,"%d\tREWD\tfor\n",line);
						else if(strcmp(test1,"float")==0)
							fprintf(write,"%d\tREWD\tfloat\n",line);						
						else								
							fprintf(write,"%d\tIDEN\t%s\n",line,test1);				
						memset(test1, '\0', sizeof(test1));						
						break;
					case'g':
						start=i;						
						i++;
						while(iden==0){
							if((getStr[i]>='0'&&getStr[i]<='9')||(getStr[i]>='A'&&getStr[i]<='Z')
								||(getStr[i]>='a'&&getStr[i]<='z')||(getStr[i]=='_')){									
								i++;	
							}
							else{
								i--;
								iden=-1;
							}										
						}							
						iden=-1;
						memset(test1, '\0', sizeof(test1));	
						strncpy(test1,getStr+start,i-start+1);
						
						if(strcmp(test1,"goto")==0)
							fprintf(write,"%d\tREWD\tgoto\n",line);												
						else								
							fprintf(write,"%d\tIDEN\t%s\n",line,test1);				
						memset(test1, '\0', sizeof(test1));						
						break;						
					case'i':
						start=i;						
						i++;
						while(iden==0){
							if((getStr[i]>='0'&&getStr[i]<='9')||(getStr[i]>='A'&&getStr[i]<='Z')
								||(getStr[i]>='a'&&getStr[i]<='z')||(getStr[i]=='_')){									
								i++;	
							}
							else{
								i--;
								iden=-1;
							}										
						}							
						iden=-1;
						memset(test1, '\0', sizeof(test1));	
						strncpy(test1,getStr+start,i-start+1);
						
						if(strcmp(test1,"int")==0)
							fprintf(write,"%d\tREWD\tint\n",line);
						else if(strcmp(test1,"if")==0)
							fprintf(write,"%d\tREWD\tif\n",line);
						else								
							fprintf(write,"%d\tIDEN\t%s\n",line,test1);				
						memset(test1, '\0', sizeof(test1));						
						break;
					case'r':
						start=i;						
						i++;
						while(iden==0){
							if((getStr[i]>='0'&&getStr[i]<='9')||(getStr[i]>='A'&&getStr[i]<='Z')
								||(getStr[i]>='a'&&getStr[i]<='z')||(getStr[i]=='_')){									
								i++;	
							}
							else{
								i--;
								iden=-1;
							}										
						}							
						iden=-1;
						memset(test1, '\0', sizeof(test1));	
						strncpy(test1,getStr+start,i-start+1);
						
						if(strcmp(test1,"return")==0)
							fprintf(write,"%d\tREWD\treturn\n",line);
						else if(strcmp(test1,"register")==0)
							fprintf(write,"%d\tREWD\tregister\n",line);						
						else								
							fprintf(write,"%d\tIDEN\t%s\n",line,test1);				
						memset(test1, '\0', sizeof(test1));						
						break;
					case's':
						start=i;						
						i++;
						while(iden==0){
							if((getStr[i]>='0'&&getStr[i]<='9')||(getStr[i]>='A'&&getStr[i]<='Z')
								||(getStr[i]>='a'&&getStr[i]<='z')||(getStr[i]=='_')){									
								i++;	
							}
							else{
								i--;
								iden=-1;
							}										
						}							
						iden=-1;
						memset(test1, '\0', sizeof(test1));	
						strncpy(test1,getStr+start,i-start+1);
						
						if(strcmp(test1,"static")==0)
							fprintf(write,"%d\tREWD\tstatic\n",line);
						else if(strcmp(test1,"switch")==0)
							fprintf(write,"%d\tREWD\tswitch\n",line);
						else if(strcmp(test1,"sizeof")==0)
							fprintf(write,"%d\tREWD\tsizeof\n",line);
						else if(strcmp(test1,"struct")==0)
							fprintf(write,"%d\tREWD\tstruct\n",line);	
						else								
							fprintf(write,"%d\tIDEN\t%s\n",line,test1);				
						memset(test1, '\0', sizeof(test1));						
						break;
					case'u':
						start=i;						
						i++;
						while(iden==0){
							if((getStr[i]>='0'&&getStr[i]<='9')||(getStr[i]>='A'&&getStr[i]<='Z')
								||(getStr[i]>='a'&&getStr[i]<='z')||(getStr[i]=='_')){									
								i++;	
							}
							else{
								i--;
								iden=-1;
							}										
						}							
						iden=-1;
						memset(test1, '\0', sizeof(test1));	
						strncpy(test1,getStr+start,i-start+1);
						
						if(strcmp(test1,"union")==0)
							fprintf(write,"%d\tREWD\tunion\n",line);
						else								
							fprintf(write,"%d\tIDEN\t%s\n",line,test1);				
						memset(test1, '\0', sizeof(test1));						
						break;
					default:						
						break;												
				}
				if(iden!=-1){
					start=i;
					if((getStr[i]>='A'&&getStr[i]<='Z')||(getStr[i]>='a'&&getStr[i]<='z')
						||(getStr[i]=='_')){
						while(iden==0){
							if((getStr[i]>='0'&&getStr[i]<='9')||(getStr[i]>='A'&&getStr[i]<='Z')
								||(getStr[i]>='a'&&getStr[i]<='z')||(getStr[i]=='_')){									
								i++;	
							}
							else{
								iden=-1;
							}										
						}
						iden=0;						
						memset(test1, '\0', sizeof(test1));	
						strncpy(test1,getStr+start,i-start);
						fprintf(write,"%d\tIDEN\t%s\n",line,test1);				
						memset(test1, '\0', sizeof(test1));			
						i--;
					}
				}
				
				start=i;				
					i++;
					start=i;
					if(getStr[i]=='+'||getStr[i]=='-'){						
						start=i;
						i++;																
					}
					if(getStr[i]>='0'&&getStr[i]<='9'){
						i++;
						while(getStr[i]>='0'&&getStr[i]<='9'){
							i++;
						}
						/*
						if((getStr[i]>='A'&&getStr[i]<='Z')||(getStr[i]>='a'&&getStr[i]<='z')
						||(getStr[i]=='_')){						
							while((getStr[i]>='A'&&getStr[i]<='Z')||(getStr[i]>='a'&&getStr[i]<='z')
								||(getStr[i]=='_')){
								i++;
							}
							memset(test1, '\0', sizeof(test1));	
							strncpy(test1,getStr+start,i-start);
							fprintf(write,"%d SCAN ERROR %s\n",line,test1);
							memset(test1, '\0', sizeof(test1));		
							
						}
						
						else */
						if(getStr[i]=='.'){
							i++;
							if(getStr[i]=='-'||getStr[i]=='+'){
								i++;									
							}
							while(getStr[i]>='0'&&getStr[i]<='9'){
								i++;
							}
							if(getStr[i]=='E'||getStr[i]=='e'){
								i++;
								if(getStr[i]=='-'||getStr[i]=='+'){
									i++;									
								}
								while(getStr[i]>='0'&&getStr[i]<='9'){
									i++;
								}
								memset(test1, '\0', sizeof(test1));	
								strncpy(test1,getStr+start,i-start);
								fprintf(write,"%d\tFLOT\t%s\n",line,test1);
								memset(test1, '\0', sizeof(test1));	
															
							}
							else{
								memset(test1, '\0', sizeof(test1));	
								strncpy(test1,getStr+start,i-start);
								fprintf(write,"%d\tFLOT\t%s\n",line,test1);
								memset(test1, '\0', sizeof(test1));	
							}							
						}
						else{
							if(i>start){
								memset(test1, '\0', sizeof(test1));	
								strncpy(test1,getStr+start,i-start);
								fprintf(write,"%d\tINTE\t%s\n",line,test1);
								memset(test1, '\0', sizeof(test1));	
							}
						}						
					}
					else{
						i=start;
					}
					
			}
				
			
		}
		fclose(write);
		fclose(read);
	}
}
