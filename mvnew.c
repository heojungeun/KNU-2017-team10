//mv.c version1 2017-11-25
#include<stdio.h>
#include<unistd.h>

int mvnew(int ac, char *av[])
 {
	int result;
	
	if (ac == 1)
	{
		exit(1);
	}
	else {
		if (result = rename(av[1], av[2]) == -1) {
			perror("error");
			exit(1);
			
		}
		
	}
}