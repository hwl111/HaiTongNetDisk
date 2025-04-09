#include <stdio.h>
#include <string.h>
#include<stdlib.h>
#define N 150                		/* 字符串个数 */
void SortString(char *str[], int n);
int main()
{ 
	int i,m,n;
	char *name=NULL;                 /* 定义二维字符数组 */
	char *strp[N];
	for(i=0;i<N;i++)
      strp[i]=name;
	printf("输入国家个数\n");
	scanf("%d",&n);
	getchar();                               /* 读走输入缓冲区中的回车符 */
	for (i=0; i<n; i++)   
	{
		printf("输入字符长度\n");
		scanf("%d",&m);
		printf("输入国家名字\n");
		getchar();
		name=(char *)calloc(m*n,sizeof(char));
		strp[i]=name;
		gets(name);                       	/* 输入n个字符串 */ 
	}
	SortString(strp,n);                    /* 字符串按字典顺序排序 */
	printf("按字典排序结果:\n");
	for (i=0; i<n; i++)                     
	{
		puts(strp[i]);/* 输出排序后的n个字符串 */
		
	}
	return 0;
	free(name);
}
/* 函数功能：交换法实现字符串按字典顺序排序 */
void SortString(char *str[], int n)
{
	int    i, j;
	char *temp=NULL;
	for (i=0; i<n-1; i++)         			
	{
		for (j = i+1; j<n; j++)
		{
			if (strcmp(str[j], str[i]) < 0)     
			{ 
				temp=str[i];        
				str[i]=str[j];
				str[j]=temp;
			}  
		}    
	}  
}
