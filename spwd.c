//pwd 명령어
#include<stdio.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<dirent.h>
#include<stdlib.h>
#include<string.h>

ino_t get_inode(char *);
void printpathto(ino_t);
void inum_to_name(ino_t,char *,int);

int main(){

	printpathto(get_inode("."));//현재 directory의 inode값을 이용하여
	putchar('\n');              //pritnpathto 함수를 call한다
	return 0;           

}
void printpathto(ino_t this_inode){

	ino_t my_inode;
	char its_name[BUFSIZ];

	if(get_inode("..")!=this_inode){//현재의 위치가 최상위 디렉토리인지 확인

		chdir("..");//아닐경우 상위 디렉토리로 이동
		inum_to_name(this_inode,its_name,BUFSIZ);//
		my_inode = get_inode(".");//현재 directory의 inode값을 저장
		printpathto(my_inode);//최상위 directory에 도달 하기까지 반복
		printf("/%s",its_name);//현재 directory의 이름을 출력함

	}
}
void inum_to_name(ino_t inode_to_find,char *namebuf,int buflen)
{
	DIR *dir_ptr;
	struct dirent *direntp;
	dir_ptr = opendir(".");//현재 dirrctory를 open함
	if(dir_ptr ==NULL){
		perror(".");
		exit(1);
	}

	while((direntp=readdir(dir_ptr))!=NULL)//현재 directory에 있는 요소를
		if(direntp->d_ino==inode_to_find) // 읽어옮
		{
			strncpy(namebuf,direntp->d_name,buflen); //namebuf에 이름을 붙임
			namebuf[buflen-1]='\0';//이름 사이에 공백을 넣음
			closedir(dir_ptr);
			return;
		}
	fprintf(stderr,"error looking for inum %d\n",inode_to_find);
	exit(1);
}
ino_t get_inode(char *fname)//directory의 inode값을 가져오기 위해 사용
{
	struct stat info;
	if(stat(fname,&info)==-1){
		fprintf(stderr,"Cannot stat");
		perror(fname);
		exit(1);
	}
	return info.st_ino;
}
