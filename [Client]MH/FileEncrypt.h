#ifndef __FileEncrypt_H__
#define __FileEncrypt_H__

#include<stdio.h> 
#include<stdlib.h> 
#include<string.h> 

class FileEncrypt
{

public:
/*��*/ 
static void encfile(char *in_filename,char *pwd,char *out_file) 
{ 
	FILE *fp1,*fp2; 
	register char ch; 
	int j=0; 
	int j0=0; 
	fp1=fopen(in_filename,"r");/*��Ҫ��*/ 
	if(fp1==NULL){ 
		printf("cannot open in-file./n"); 
		return ;//exit(1);/*��Ҫ��,���˳�����*/ 
	} 
	fp2=fopen(out_file,"w"); 
	if(fp2==NULL){ 
		printf("cannot open or create out-file./n"); 
		return ;//exit(1);/*�������,���˳�*/ 
	} 

	while(pwd[++j0]);  

	ch=fgetc(fp1); 

	/*�㷨*/ 
	while(!feof(fp1)){ 
		if(j0 >7)
			j0 = 0;
		ch += pwd[j0++];
		fputc(ch,fp2); 
		ch=fgetc(fp1); 
	} 
	fclose(fp1);/*�ر�Դ*/ 
	fclose(fp2);/*�ر�*/ 
} 

/*��*/ 
static void decryptfile(char *in_filename,char *pwd,char *out_file) 
{ 
	FILE *fp1,*fp2; 
	register char ch; 
	int j=0; 
	int j0=0; 
	fp1=fopen(in_filename,"r");/*��Ҫ��*/ 
	if(fp1==NULL){ 
		printf("cannot open in-file./n"); 
		return ;//exit(1);/*��Ҫ��,���˳�����*/ 
	} 
	fp2=fopen(out_file,"w"); 
	if(fp2==NULL){ 
		printf("cannot open or create out-file./n"); 
		return ;//exit(1);/*�������,���˳�*/ 
	} 

	while(pwd[++j0]);  	
	ch=fgetc(fp1); 
	/*�㷨*/ 
	while(!feof(fp1)){ 
		if(j0 >7)
			j0 = 0;
		ch -= pwd[j0++];
		fputc(ch,fp2);/*�ҵ��㷨*/
		ch=fgetc(fp1); 
	} 
	fclose(fp1);/*�ر�Դ*/ 
	fclose(fp2);/*�ر�*/ 
} 
};

#endif