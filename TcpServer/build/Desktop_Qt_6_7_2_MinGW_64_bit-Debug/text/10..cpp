#include <stdio.h>
#include <string.h>
#include<stdlib.h>
#define N 150                		/* �ַ������� */
void SortString(char *str[], int n);
int main()
{ 
	int i,m,n;
	char *name=NULL;                 /* �����ά�ַ����� */
	char *strp[N];
	for(i=0;i<N;i++)
      strp[i]=name;
	printf("������Ҹ���\n");
	scanf("%d",&n);
	getchar();                               /* �������뻺�����еĻس��� */
	for (i=0; i<n; i++)   
	{
		printf("�����ַ�����\n");
		scanf("%d",&m);
		printf("�����������\n");
		getchar();
		name=(char *)calloc(m*n,sizeof(char));
		strp[i]=name;
		gets(name);                       	/* ����n���ַ��� */ 
	}
	SortString(strp,n);                    /* �ַ������ֵ�˳������ */
	printf("���ֵ�������:\n");
	for (i=0; i<n; i++)                     
	{
		puts(strp[i]);/* ���������n���ַ��� */
		
	}
	return 0;
	free(name);
}
/* �������ܣ�������ʵ���ַ������ֵ�˳������ */
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
