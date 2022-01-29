#ifndef __FileEncrypt_H__
#define __FileEncrypt_H__

#include<stdio.h> 
#include<stdlib.h> 
#include<string.h> 

class FileEncrypt
{

public:
/*子*/ 
static void encfile(char *in_filename,char *pwd,char *out_file) 
{ 
	FILE *fp1,*fp2; 
	register char ch; 
	int j=0; 
	int j0=0; 
	fp1=fopen(in_filename,"r");/*打开要的*/ 
	if(fp1==NULL){ 
		printf("cannot open in-file./n"); 
		return ;//exit(1);/*打开要的,便退出程序*/ 
	} 
	fp2=fopen(out_file,"w"); 
	if(fp2==NULL){ 
		printf("cannot open or create out-file./n"); 
		return ;//exit(1);/*建立后的,便退出*/ 
	} 

	while(pwd[++j0]);  

	ch=fgetc(fp1); 

	/*算法*/ 
	while(!feof(fp1)){ 
		if(j0 >7)
			j0 = 0;
		ch += pwd[j0++];
		fputc(ch,fp2); 
		ch=fgetc(fp1); 
	} 
	fclose(fp1);/*关闭源*/ 
	fclose(fp2);/*关闭*/ 
} 

/*子*/ 
static void decryptfile(char *in_filename,char *pwd,char *out_file) 
{ 
	FILE *fp1,*fp2; 
	register char ch; 
	int j=0; 
	int j0=0; 
	fp1=fopen(in_filename,"r");/*打开要的*/ 
	if(fp1==NULL){ 
		printf("cannot open in-file./n"); 
		return ;//exit(1);/*打开要的,便退出程序*/ 
	} 
	fp2=fopen(out_file,"w"); 
	if(fp2==NULL){ 
		printf("cannot open or create out-file./n"); 
		return ;//exit(1);/*建立后的,便退出*/ 
	} 

	while(pwd[++j0]);  	
	ch=fgetc(fp1); 
	/*算法*/ 
	while(!feof(fp1)){ 
		if(j0 >7)
			j0 = 0;
		ch -= pwd[j0++];
		fputc(ch,fp2);/*我的算法*/
		ch=fgetc(fp1); 
	} 
	fclose(fp1);/*关闭源*/ 
	fclose(fp2);/*关闭*/ 
} 
};

#endif