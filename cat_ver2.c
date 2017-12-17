/*cat_ver2.c
 *2016114345 허윤
 *2017-12-12 17:00
 */
#include	<stdio.h>
#include	<sys/stat.h>
#include	<fcntl.h>
#include	<string.h>

void catnew(int ac,char **av)
{
	char e='$';
	char ones;
	char op[4];
	int line_num=1;
	int fd;
	int oflag=0;
	int flag=0;
	int i=0;

	if(av[1][0]=='-')
		oflag=1;
	
	if(ac == 2)
	{	
	fd= open(av[1],O_RDONLY);

	while(read(fd,&ones,1))
		write(1,&ones,1);
	close(fd);
	}
	else if(oflag == 1)
	{
		fd = open(av[2],O_RDONLY);

		if(av[1][1]=='n'){
			printf("op %c\n",av[1][1]);
			while(read(fd,&ones,1)){
				printf(" %d	",line_num++);
				write(1,&ones,1);
				}
		}
		else if(av[1][1]=='e'){
		       	printf("op %c\n",av[1][1]);
			while(read(fd,&ones,1)){
				printf(" %c	",e);
				write(1,&ones,1);
				}
		}
		
		else{
			printf("syntax error(u can use only cat-n cat-e )\n");
			exit(0);
		}
	}
	else
	{
	printf("fatal error\n");
	exit(0);
	}

}
