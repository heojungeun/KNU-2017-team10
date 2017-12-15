//mv.c version2 2017-12-15
#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
#include<fcntl.h>
#include<sys/stat.h>

int main(int ac ,char *av[])
{	
	int fd1,fd2;
	int r_size,w_size;
	char buf[100];

	if(ac==1)//mv command 실행시 파일 이름이 입력되는지 확인
	{	
		perror("check the filename");
		exit(1);	
	}
	else if(ac==3){	
		fd1=open(av[1],O_RDONLY);//original
		fd2=open(av[2],O_RDWR|O_CREAT|O_EXCL,0664);//이동된 파일

		r_size=read(fd1,buf,100);
		w_size=write(fd2,buf,r_size);
		while(r_size ==100){
			r_size=read(fd1,buf,100);
			w_size=write(fd2,buf,r_size);
		}
	}제
	unlink(av[1]);//원본파일 삭제
	return 0;
}
