<<<<<<< HEAD
// 2017-11-25 17:13 cd.c
=======
// 2017-11-25 19:13 cd.c
>>>>>>> b3ece02bc67047414d53f818e6641c2c04933809
#include<stdio.h>
#include<stdlib.h>
#include<dirent.h>
#include<sys/types.h>

void cd(int argc, char *argv[])
{

	if(argc ==1)
	{
		chdir(getenv("HOME"));
		// cd => home으로 이동
	}
	else if(argc==2)
	{
		if(chdir(argv[1]==-1)
			printf("No such directory\n");
		
		// 해당 dir가 존재하지 않을 경우
	}
	else
	{	
		printf("usage: cd dirname\n");

		// 형식이 잘못되었을 경우
	}

}
			
