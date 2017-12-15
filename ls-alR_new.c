//2017-11-29 9:10 lsalR.c

#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<dirent.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<unistd.h>
#include<grp.h>
#include<pwd.h>
#include<time.h>

typedef struct {
	char  permission[11];
	int   linkcount;
	char  userid[20];
	char  groupid[20];
	int   size;
	int   date[2];
	int   time[2];
	char  filename[255];
	char  link[4096];
} Info_st;

typedef struct element {

	void               *data;
	struct element   *next;

} element;

typedef struct List_ {

	int                size;
	int(*match)(const void *key1, const void *key2);
	void(*destroy)(void *data);
	element           *head;
	element           *tail;

}List;


void lsalRnew()
{
	char *dirname;

	dirname = getcwd(NULL, NULL);
	saveInStruct(dirname);

	free(dirname);
	return 0;
}

// 파일 권한을 검사하는 함수
void checkFile(struct stat *buf, Info_st *info)
{
	// directory or file?
	if (S_ISDIR(buf->st_mode))
		info->permission[0] = 'd';
	else if (S_ISLNK(buf->st_mode))
		info->permission[0] = 'l';
	else if (S_ISCHR(buf->st_mode))
		info->permission[0] = 'c';
	else if (S_ISBLK(buf->st_mode))
		info->permission[0] = 'b';
	else if (S_ISSOCK(buf->st_mode))
		info->permission[0] = 's';
	else if (S_ISFIFO(buf->st_mode))
		info->permission[0] = 'p';
	else
		info->permission[0] = '-';

	// user permission check
	if (buf->st_mode & S_IRUSR)
		info->permission[1] = 'r';
	else
		info->permission[1] = '-';
	if (buf->st_mode & S_IWUSR)
		info->permission[2] = 'w';
	else
		info->permission[2] = '-';
	if (buf->st_mode & S_IXUSR)
		info->permission[3] = 'x';
	else if (buf->st_mode & S_ISUID)
		info->permission[3] = 's';
	else
		info->permission[3] = '-';

	// group permission check
	if (buf->st_mode & S_IRGRP)
		info->permission[4] = 'r';
	else
		info->permission[4] = '-';
	if (buf->st_mode & S_IWGRP)
		info->permission[5] = 'w';
	else
		info->permission[5] = '-';
	if (buf->st_mode & S_IXGRP)
		info->permission[6] = 'x';
	else if (buf->st_mode & S_ISGID)
		info->permission[6] = 's';
	else
		info->permission[6] = '-';

	// other user check
	if (buf->st_mode & S_IROTH)
		info->permission[7] = 'r';
	else
		info->permission[7] = '-';
	if (buf->st_mode & S_IWOTH)
		info->permission[8] = 'w';
	else
		info->permission[8] = '-';

	//sticky bit 설정
	if (buf->st_mode & S_IXOTH)
	{
		if (buf->st_mode & S_ISVTX)
			info->permission[9] = 't';
		else
			info->permission[9] = 'x';
	}
	else
	{
		if (buf->st_mode & S_ISVTX)
			info->permission[9] = 'T';
		else
			info->permission[9] = '-';
	}


	info->permission[10] = '\0';
}

// block의 총 사이즈를 계산해주는 함수
int totalSize(char dirname[])
{
	struct dirent *input;
	struct stat buf;
	DIR *p;
	char filename[255];
	int sum = 0;

	p = opendir(dirname);

	while ((input = readdir(p)) != '\0')
	{
		strcpy(filename, dirname);
		strcat(filename, "/");
		strcat(filename, input->d_name);
		if ((lstat(filename, &buf)) == 0)
			sum += buf.st_blocks;
	}
	closedir(p);

	return sum / 2;

}

// 파일 이름 순으로 정렬하는 함수
void sortFile(List *list)
{
	int i, j, k;
	int len1, len2, min;
	int index;
	element * ptr;
	void * temp;

	for (i = list->size - 1; i > 0; i--)
	{
		for (j = 0, ptr = list->head; j < i; j++, ptr = ptr->next)
		{
			len1 = strlen(((Info_st *)ptr->data)->filename);
			len2 = strlen(((Info_st *)ptr->next->data)->filename);

			if (len1 < len2)
				min = len2;
			else
				min = len1;

			for (k = min, index = 0; k != 0; k--, index++)
			{
				if (((Info_st *)ptr->data)->filename[index] >
					((Info_st *)ptr->next->data)->filename[index])
				{
					temp = ptr->data;
					ptr->data = ptr->next->data;
					ptr->next->data = temp;
					break;
				}
				else if (((Info_st *)ptr->data)->filename[index] ==
					((Info_st *)ptr->next->data)->filename[index])
				{
					continue;
				}
				break;
			}
		}
	}
}

// 리스트 출력하는 함수
void printList(List *list)
{
	int i;
	int max[4] = { 0 }; //[0]하드링크수,[1]유저아이디,[2]그룹아이디,[3]크기 
	element * ptr;

	if (list->size == 0) // size가 0일 경우 출력
	{
		puts("Nothing in here.");
		return -1;
	}

	space(list, max); // 공간 계산해서 max 배열에 넣어줌


	for (i = 0, ptr = list->head; i < list->size; i++, ptr = ptr->next)
	{
		printf("%s", ((Info_st *)ptr->data)->permission);
		printf("%*d", max[0], ((Info_st *)ptr->data)->linkcount);
		printf(" %-*s", max[1], ((Info_st *)ptr->data)->userid);
		printf(" %-*s", max[2], ((Info_st *)ptr->data)->groupid);
		printf("%*d", max[3], ((Info_st *)ptr->data)->size);
		printf(" %2d월 %2d", ((Info_st *)ptr->data)->date[0]
			, ((Info_st *)ptr->data)->date[1]);
		printf(" %02d:%02d", ((Info_st *)ptr->data)->time[0]
			, ((Info_st *)ptr->data)->time[1]);
		printf(" %s", ((Info_st *)ptr->data)->filename);

		if (((Info_st *)ptr->data)->permission[0] == 'l')
			printf(" -> %s\n", ((Info_st *)ptr->data)->link);
		else
			puts("");
	}
}

// path( dirname/filename ) 리턴
char *copyPath(char *dirname, char *filename)
{
	int len = 20;
	char * path = (char *)malloc(sizeof(char)*len);
	int i = 0, j = 0;

	while (1)
	{
		if (dirname[i] == '\0')
			break;
		path[i] = dirname[i];
		i++;
	}

	path[i++] = '/';

	while (1)
	{
		if (filename[j] == '\0')
		{
			path[i] = '\0';
			break;
		}
		path[i] = filename[j];
		i++, j++;
	}

	return path;
}

// 메모리 해제 함수
void delete(void *data)
{
	free((Info_st *)data);
}

// 구조체에 정보 저장
int saveInStruct(char dirname[])
{
	List list;
	element * elmt = { '\0' };
	Info_st * info;
	DIR * dirpt;
	struct dirent * entry;
	struct stat buf;
	struct group * grp;
	struct passwd * pwd;
	struct tm * time;
	char fileName[NAME_MAX];
	int readc = 0, total = 0;
	char * dirPath;

	list_init(&list, delete); // list를 초기화함

	if ((dirpt = opendir(dirname)) == '\0') // directory open 실패
	{
		puts("Directory open fail");
		return -1;
	}

	printf("%s:\n", dirname); // (1) dirname 출력

	total = totalSize(dirname); // size계산
	printf("total %d\n", total); // (2) 계산한 size 출력

	while ((entry = readdir(dirpt)) != NULL)
	{
		strcpy(fileName, dirname);
		strcat(fileName, "/");
		strcat(fileName, entry->d_name);
		if ((lstat(fileName, &buf) == 0))
		{
			info = (Info_st *)malloc(sizeof(Info_st));
			pwd = getpwuid(buf.st_uid);
			grp = getgrgid(buf.st_gid);
			strcpy(info->userid, pwd->pw_name);
			strcpy(info->groupid, grp->gr_name);
			
			info->linkcount = buf.st_nlink;
			info->size = buf.st_size;

			time = localtime(&buf.st_mtime);

			info->date[0] = (time->tm_mon) + 1;
			info->date[1] = time->tm_mday;
			info->time[0] = time->tm_hour;
			info->time[1] = time->tm_min;

			checkFile(&buf, info);	//파일 권한 검사
			strcpy(info->filename, entry->d_name);

			if (S_ISLNK(buf.st_mode))
			{
				readc = readlink(fileName, info->link, sizeof(info->link));
				info->link[readc] = '\0';
			}

			if ((list_ins_next(&list, elmt, info)) == -1)
			{
				puts("list삽입실패");
				return -1;
			}
		}
	}

	sortFile(&list); // 파일 정렬한 후
	printList(&list); // 파일 리스트 출력

	elmt = list.head;

	// 하위 목록들 재귀적으로 출력
	while (elmt->next != 0)
	{


		if (((Info_st *)elmt->data)->permission[0] == 'd'
			&& (strcmp(((Info_st *)elmt->data)->filename, "..")) != 0
			&& (strcmp(((Info_st *)elmt->data)->filename, ".")) != 0)


		{
			dirPath = copyPath(dirname, ((Info_st *)elmt->data)->filename);
			puts("");
			saveInStruct(dirPath);
			free(dirPath);
		}
		elmt = elmt->next;
	}

	list_destroy(&list); // list를 destroy함
	closedir(dirpt); // close directory

	return 0;
}

// 공간 계산 함수
void space(List * list, int max[])
{
	int i, j;
	element * ptr;
	char string[2][10];

	for (i = 0, ptr = list->head; i < list->size; i++, ptr = ptr->next)
	{
		if (((Info_st *)ptr->data)->linkcount > max[0])
			max[0] = ((Info_st *)ptr->data)->linkcount;

		if (max[1] < (strlen(((Info_st *)ptr->data)->userid)))
			max[1] = strlen(((Info_st *)ptr->data)->userid);

		if (max[2] < (strlen(((Info_st *)ptr->data)->groupid)))
			max[2] = strlen(((Info_st *)ptr->data)->groupid);

		if (max[3] < ((Info_st *)ptr->data)->size)
			max[3] = ((Info_st *)ptr->data)->size;
	}
	sprintf(string[0], "%d", max[0]);
	sprintf(string[1], "%d", max[3]);

	max[0] = strlen(string[0]) + 1;
	max[3] = strlen(string[1]) + 1;
}

// 리스트 초기화 함수
void list_init(List *list, void(*destroy)(void *data))
{
	list->size = 0;
	list->destroy = destroy;
	list->head = NULL;
	list->tail = NULL;

	return;
}

// 리스트 destroy 함수
void list_destroy(List *list)
{
	void               *data;

	while (list->size > 0)
	{
		if (list_rem_next(list, NULL, (void **)&data) == 0 && list->destroy != NULL)//data free함수가 있다면 함수호출 
		{			//두번째 인제가 NULL일 경우 element앞에서 부터 전부 remove
			list->destroy(data);
		}
	}

	memset(list, 0, sizeof(List));

	return;
}

// 리스트에 insert하는 함수
int list_ins_next(List *list, element *elmnt, const void *data)
{
	element           *new_element;

	if ((new_element = (element *)malloc(sizeof(element))) == NULL)
		return -1;	//메모리 할당 실패시 -1을 리턴

	new_element->data = (void *)data;	//사용자 data 주소를 Elmt data연결

	if (elmnt == NULL) 	//NULL이면 head로 삽입.
	{
		if (list->size == 0)
			list->tail = new_element;

		new_element->next = list->head;
		list->head = new_element;
	}

	else 			//아니면 tail로 삽입.
	{
		if (elmnt->next == NULL)
			list->tail = new_element;

		new_element->next = elmnt->next;
		elmnt->next = new_element;
	}
	list->size++; //element 갯수 증가

	return 0;
}

// 리스트에서 element를 remove하는 함수
int list_rem_next(List *list, element *elmnt, void **data)
{
	element           *old_element;

	if (list->size == 0)	//삭제할 element가 없는지 검사
		return -1;

	if (elmnt == NULL)	//head 삭제 
	{
		*data = list->head->data;
		old_element = list->head;
		list->head = list->head->next;

		if (list->size == 0)
			list->tail = NULL;
	}
	else 	//두번째 인자로 넘어온 element 삭제
	{
		if (elmnt->next == NULL)
			return -1;

		*data = elmnt->next->data;
		old_element = elmnt->next;
		elmnt->next = elmnt->next->next;

		if (elmnt->next == NULL)
			list->tail = elmnt;
	}

	free(old_element);

	list->size--; //element 갯수 감소

	return 0;
}

