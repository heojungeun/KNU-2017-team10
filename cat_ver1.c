/*cat_ver1.c
 *2016114345 허윤
 *2017-12-10 17:00
 */
#include	<stdio.h>
#include	<sys/stat.h>
#include	<fcntl.h>
void catnew(int ac,char **av)
{
	char ones;
	int fd;

	if(ac!=2)
	{
		printf("argument error\n");
		printf(" how to use? :./cat_ver1 filename\n");
		exit(-1);
	}

	fd= open(av[1],O_RDONLY);

	while(read(fd,&ones,1))
		write(1,&ones,1);

	close(fd);
}
