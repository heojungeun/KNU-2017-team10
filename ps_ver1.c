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
void psnew(int ac, char **av)
{
	int pid;
	DIR *dir;
	struct dirent *info;
	struct stat filestat;
	char tempPath[256];
	char cmdline[256];
	int oflag = 0;
	char aux[3] = "aux";
	char time[256];
	char TTY[256];
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
	
}

int getCmdline(char *filename,char *bufstr)
{
	FILE* srcfp;
	int i;
	srcfp = fopen(filename,"r");

	memset(bufstr,0,sizeof(bufstr));
	fgets(bufstr,256,srcfp);
	fclose(srcfp);

}
