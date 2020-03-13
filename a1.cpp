#include<iostream>
#include<windows.h>
using namespace std;
struct node
{
	char item[256];
	struct node *next;
};
node *head,*tail;
void createnode(char *item);
void deletelinklist();
int nodecount = 0,complete = 0;
HANDLE hMutex1,hMutex2;
int gcount = 1,turn1 = 1,turn2 = 0;
DWORD WINAPI thread1(LPVOID lpParam)
{
	char data[256];
	FILE * fr;
	fr = fopen("C:\\zv\\updspy.log","r");
	while(fgets(data,sizeof(data),fr))
	{
		createnode(data);
		if(nodecount >= 5)
		{
			turn1--;
			while(turn1 == 0);
			//WaitForSingleObject(hMutex1,INFINITE);
		}
		if(nodecount == 1)
		{
			turn2 = 1;
			//ReleaseMutex(hMutex2);
		}
	}
	complete = 1;
	fclose(fr);
	return 0;
}
DWORD WINAPI thread2(LPVOID lpParam)
{
	char data[256];
	FILE *fw;
	fw = fopen("C:\\zv\\output_file.txt","w");
	while(1)
	{
		if(head == NULL && complete == 1)
			break;
		if(head == NULL && complete == 0)
		{
			turn2--;
			while(turn2 == 0);
			//WaitForSingleObject(hMutex2,INFINITE);
		}
		else
		{
			fputs(head->item,fw);
			deletelinklist();
			if(nodecount == 4)
			{
				turn1 = 1;
				//ReleaseMutex(hMutex1);
			}
		}
	}
	fclose(fw);
	return 0;
}
void main()
{
	
	HANDLE hthread1,hthread2;
	hMutex1 = CreateMutex(0,1,"Global\\{1}");
	
	hMutex2 = CreateMutex(0,1,"Global\\{2}");
	hthread1 = CreateThread(NULL,0,thread1,NULL,0,NULL);
	hthread2 = CreateThread(NULL,0,thread2,NULL,0,NULL);
	CloseHandle(hthread1);
	CloseHandle(hthread2);
	system("pause");
}
void deletelinklist()
{
	struct node *temp = head;
	head = head->next;
	free(temp);
	nodecount --;
}
void createnode(char *item)
{
	struct node *temp = (struct node*) malloc(sizeof(struct node));
	strcpy(temp->item, item);
	temp->next = NULL;

    if(head==NULL)
    {
		head=temp;
		tail=temp;
		temp=NULL;
	}
    else
    {	
		tail->next=temp;
        tail=temp;
    }
	nodecount ++;
}