/*
file: who2.c
author: 2016114146 허정은
datetime : 2017-09-13 13:00
description : read/var/adm/utmp and list info therein
*/
#include<stdio.h>
#include<utmp.h>
#include<fcntl.h>
#include<unistd.h>
#include<time.h>
#include<stdlib.h>
/*#define SHOWHOST*/

void showtime(long);
void show_info(struct utmp*);
int main()
{
	struct utmp	current_record;
	int		utmpfd;
	int		reclen = sizeof(current_record);

	if((utmpfd = open(UTMP_FILE, O_RDONLY))==-1){
		perror(UTMP_FILE);
		exit(1);
	}
	while(read(utmpfd,&current_record,reclen)==reclen)
		show_info(&current_record);
	close(utmpfd);
	return 0;
}

void show_info(struct utmp* utbufp)
{
	if(utbufp->ut_type !=USER_PROCESS)
		return;
	printf("%-8.8s", utbufp->ut_name);
	printf(" ");
	printf("%-8.8s",utbufp->ut_line);
	printf(" ");
	showtime(utbufp->ut_time);
#ifdef	SHOWHOST
	if(utbufp->ut_host[0]!='\0')
		printf(" (%s) ",utbufp->ut_host);
#endif
	printf("\n");
}

void showtime(long timeval)
{
	char *cp;
	cp=ctime(&timeval);

	printf("%12.12s",cp+4);
}

