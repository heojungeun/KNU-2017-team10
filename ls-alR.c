//2017-11-29 9:10 lsalR.c

#include<stdio.h>
#include<stdlib.h>
#include<dirent.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<unistd.h>
#include<grp.h>
#include<pwd.h>
#include<time.h>



char *getString(); // 동적할당으로 문자입력받기
void checkFile(struct stat *buf, Info_st *info); // 파일권한 검사함수
int totalSize(char dirname[]); // 디렉토리 크기 구하는 함수
void sortFile(List *list); // 파일 이름으로 정렬하는 함수
void printList(List *list); // 리스트에 저장된 자료 출력 함수
char *copyPath(char *dirname, char *filename); // 파일경로 복사하는 함수
int saveInStruct(char dirname[]); // 디렉토리 자료를 info_st에 저장하는 함수

typedef struct{
  char  permission[11];
  int   linkcount;
  char  userid[20];
  char  groupid[20];
  int   size;
  int   date[2];
  int   time[2];
  char  filename[255];
  char  link[4096];
} Info_st;

void main()
{
  char *dirname;
  
  dirname = getcwd(NULL, NULL);
  saveInStruct(dirname);
  
  free(dirname);
  return 0;
}

char *getString()
{
  int max = 20;
  char *line=(char*)malloc(sizeof(char)*max);
  int i=0;
  
  while(1)
  {
    line[i]=getchar();
    if(line[i]=='\n')
    {
      line[i]='\0';
      break;
    }
    i++;
  }
  
  return line;
}
void checkFile(struct stat *buf, Info_st *info)
{
  // directory or file?
  if(S_ISDIR(buf->st_mode))
    info->permission[0]='d';
  else if(S_ISLNK(buf->st_mode))
    info->permission[0]='l';
  else if(S_ISCHR(buf->st_mode))
    info->permission[0]='c';
  else if(S_ISBLK(buf->st_mode))
    info->permission[0]='b';
  else if(S_ISSOCK(buf->st_mode))
    info->permission[0]='s';
  else if(S_ISFIFO(buf->st_mode))
    info->permission[0]='p';
  else
    info->permission[0]='-';
  
  // user permission check
  if(buf->st_mode & S_IRUSR)
		info->permission[1]='r';
	else
		info->permission[1]='-';
	if(buf->st_mode & S_IWUSR)
		info->permission[2]='w';
	else
		info->permission[2]='-';
	if(buf->st_mode & S_IXUSR)
		info->permission[3]='x';
	else if(buf->st_mode & S_ISUID)
		info->permission[3]='s';
	else
		info->permission[3]='-';
  
  // group permission check
  if(buf->st_mode & S_IRGRP)
		info->permission[4]='r';
	else
		info->permission[4]='-';
	if(buf->st_mode & S_IWGRP)
		info->permission[5]='w';
	else
		info->permission[5]='-';
	if(buf->st_mode & S_IXGRP)
		info->permission[6]='x';
	else if(buf->st_mode & S_ISGID)
		info->permission[6]='s';
	else
		info->permission[6]='-'; 
  
  // other user check
  if(buf->st_mode & S_IROTH)
		info->permission[7]='r'; 
	else 
		info->permission[7]='-';
	if(buf->st_mode & S_IWOTH)
		info->permission[8]='w';
	else
		info->permission[8]='-';
  
  //sticky bit 설정
  if(buf->st_mode & S_IXOTH)
	{
		if(buf->st_mode & S_ISVTX)
			info->permission[9]='t';
		else
			info->permission[9]='x';
	}
	else
	{
		if(buf->st_mode & S_ISVTX)
			info->permission[9]='T';
		else
			info->permission[9]='-';
	}	
  
  
  info->permission[10]='\0';
}
int totalSize(char dirname[])
{
}
void sortFile(List *list)
{
}
void printList(List *list)
{
}
char *copyPath(char *dirname, char *filename)
{
}
int saveInStruct(char dirname[])
{
}


