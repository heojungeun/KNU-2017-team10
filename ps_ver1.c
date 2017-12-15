/*ps_ver1.c
 *
 *
 */
#include	<stdio.h>
#include	<unistd.h>
#include	<sys/stat.h>
#include	<sys/types.h>
#include	<string.h>
#include	<dirent.h>
int getCmdline(char*,char*);
main(int ac, char **av)
{
	int pid;
	DIR *dir;
	struct dirent *info;
	struct stat filestat;
	char tempPath[256];
	char cmdline[256];
	int oflag = 0;
	char aux[3] = "aux";
	
	dir = opendir("/proc");
	
	printf("PID	CMDLINE\n");
	while((info=readdir(dir)) != NULL){
		lstat(info->d_name,&filestat);
		
		if(!S_ISDIR(filestat.st_mode))
			continue;
		pid = atoi(info->d_name);
		if(pid <=0 ) continue;
		sprintf(tempPath,"/proc/%d/cmdilne",pid);
		getCmdline(tempPath,cmdline);

		printf("%d	%s\n",pid,cmdline);
	}
	closedir(dir);
	/*if(av[1][1]='-')
		oflag = 1;
	else if(strcmp(av[1],aux)==0)
		olfag = 2;


	if(olfag == 2){
		printf("PID   CPU   MEM   VSZ   RSS   STAT   START   TIME   COMMAND\n");
		for()
		{
			printf("%d    %.1f    %.1f    %d    %d    ?    D    day    hour:min    %s");
		}	
	}
	else if(oflag == 1){
		printf("UID	PID	PPID	C	STIME	TTY	TIME	CMD\n");
		for()
		{
			printf("");
		}
	else{
		printf("PID	TTY	TIME	CMD\n");
		for()
		{
			printf("");
		}	
	}
	*/
}

int getCmdline(char *filename,char *bufstr)
{
	FILE* srcfp;
	int i;
	srcfp = fopen(filename,"r");

	memset(bufstr,0,sizeof(bufstr));
	fgets(bufstr,256,srcfp);
	fclose(srcfp);

	printf("get Cmd complite\n");
}
