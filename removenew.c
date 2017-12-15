//rm version1 2017-11-25
#include<stdio.h>
#include<unistd.h>

void removenew(int ac, char *av[])
{
	if(ac==1){
		printf("there is not target file\n");
		exit(1);
	}
	else{
		if(unlink(av[1])==-1){
			printf("fail to remove %s\n",av[1]);
			exit(1);
		}
	}

}
