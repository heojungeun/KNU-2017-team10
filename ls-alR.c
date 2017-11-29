//2017-11-29 9:10 lsalR.c

#include<stdio.h>
#include<stdlib.h>


char *getString(); // 동적할당으로 문자입력받기
void checkFile(struct stat *buf, Info_st *info); // 파일권한 검사함수
int totalSize(char dirname[]); // 디렉토리 크기 구하는 함수
void sortFile(List *list); // 파일 이름으로 정렬하는 함수
void printList(List *list); // 리스트에 저장된 자료 출력 함수
char *copyPath(char *dirname, char *filename); // 파일경로 복사하는 함수
int saveInStruct(char dirname[]); // 디렉토리 자료를 info_st에 저장하는 함수

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
}
void checkFile(struct stat *buf, Info_st *info)
{
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


