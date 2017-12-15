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

	if(ac==1)
	{	
		perror("check the filename");
		exit(1);	
	}
	else{	
		fd1=open(av[1],O_RDONLY);
		fd2=open(av[2],O_RDWR|O_CREAT|O_EXCL,0664);

		r_size=read(fd1,buf,100);
		w_size=write(fd2,buf,r_size);
		while(r_size ==100){
			r_size=read(fd1,buf,100);
			w_size=write(fd2,buf,r_size);
		}
	}
	unlink(av[1]);
	return 0;
}
