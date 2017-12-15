// 2017-11-26 19:26 mkdir.c
#include<stdio.h>
#include<stdlib.h>

void mkdirnew(int argc, char *argv[])
{
  int i;
  
  if(argc==1)
    printf("usage : mkdir dirname\n");
  /*
  else if(argc==2)
  {
    if(mkdir(argv[1], 0755)==-1)
      printf("error : cannot make dir\n");
  }
  */
  else
  {
    for(i=1; i<argc; i++)
      if(mkdir(argv[i], 0755)==-1)
        printf("error : cannot make dir '%s'\n",argv[i]);
  }
  
}
