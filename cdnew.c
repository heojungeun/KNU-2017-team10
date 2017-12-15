#include<stdio.h>
#include<stdlib.h>
#include<dirent.h>
#include<sys/types.h>

void cdnew(int argc, char *argv[])
{

	if(argc ==1)
	{
		chdir(getenv("HOME"));
		// cd => home으로 이동
	}
	else if(argc==2)
	{
		if(chdir(argv[1]==-1))
			printf("No such directory\n");
		
		// 해당 dir가 존재하지 않을 경우
	}
	else
	{	
		printf("usage: cd dirname\n");

		// 형식이 잘못되었을 경우
	}

}
			
