/*
 * =====================================================================================
 *
 *       Filename:  my_ls.c
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  2013年07月17日 15时47分17秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Hurley (LiuHuan), liuhuan1992@gmail.com
 *        Company:  Class 1107 of Computer Science and Technology
 *
 * =====================================================================================
 */

#include <unistd.h>
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <errno.h>
#include <time.h>
#include <sys/types.h>
#include <linux/limits.h>
#include <dirent.h>
#include <grp.h>
#include <pwd.h>

void 	my_error(const char *err_string,int line);
int 	display(char *para,char *path);
int 	display_present(char *path);
void 	show(char filename[][256]);
int 	display_para(char *para,char *path);
int 	display_all(char *path);
void 	display_next(char *path);
void 	show_t(char *filename);

int main(int argc, char *argv[])
{
	char 	path[80] = "/home/chang/cgxr";
	char 	para[4] = "\0";
	int  	count1;
	
	//先把参数，路径存数组中，再分有参无参讨论
	for( count1 = 1; count1 < argc; count1++)
	      if(argv[count1][0] == '-')
		    strncpy(para,argv[count1],2);

	//因为最多出现两个参数，所以有参无参以2为界
	if(argc < 2)
	      display_present(path);
	else	       
	{ 
		strcpy(para,argv[1]);
		display_para(para,path);
	}	    

	return EXIT_SUCCESS;
}

void my_error(const char *err_string,int line)
{
	fprintf(stderr,"line:%d ",line);
	perror(err_string);
	exit(1);
}

void show(char filename[][256])
{
	int  	maxlen = 0;
	int  	nowlen = 0;
	int  	i = 0,linemax = 0,j;

	
	maxlen = strlen(filename[i]);
	
	//找到最长文件名
	while( strncmp(filename[i],"\0",2) != 0)
	{	
		if(maxlen < strlen(filename[i+1]))
		      maxlen = strlen(filename[i+1]);
		i++;
	}
	
	//对其输出，利用最大文件名对齐，补空格
	i = 0;
	linemax = maxlen;
	while( strncmp(filename[i],"\0",2) != 0)
	{
		nowlen = strlen(filename[i]);
		if((linemax + 2 ) > 120)
		{
			printf("\n");
			printf("%s",filename[i]);
			for( j = 0; j < maxlen - nowlen + 2; j++)
			      printf(" ");
			linemax = maxlen;
			i++;
		}
		else
		{      
			printf("%s",filename[i]);
			for( j = 0; j < maxlen - nowlen + 2; j++)
			    printf(" ");
			linemax += (maxlen + 2);
			i++;
		}
	}
	
}

int display_present(char *path)
{
	DIR 		*dir;
	struct dirent 	*perfile;
	char  		filename[4096][256]={"\0","\0"};
	int 		count1,i,j;
	char 		temp[256];

	dir = opendir(path);
	if(dir == NULL)
		my_error("opendir",__LINE__);
	
	//复制目录文件名到二位字符数组中
	for( count1 = 0; (perfile = readdir(dir)) != NULL; count1++)
		if(strncmp(perfile->d_name,".",1) != 0 && strncmp(perfile->d_name,"..",2) != 0)
		{
			strcpy(filename[count1],perfile->d_name);
		}
		else
		      count1--;
	//排序
	for( i = 0; i < count1 + 1; i++)
	      for( j = 0; j < count1 - 1 - i; j++)
		{
			if((strcmp(filename[j],filename[j+1])) > 0 )
			{
				strcpy(temp,filename[j]);
				strcpy(filename[j],filename[j+1]);
				strcpy(filename[j+1],temp);
			}
		}
	
	show(filename);
	
	printf("\n");
	return 0;
}

int display_all(char *path)
{
	DIR 		*dir;
	struct dirent 	*perfile;
	char  		filename[4096][256]={"\0","\0"};
	int 		count1,i,j;
	char 		temp[256];

	dir = opendir(path);
	if(dir == NULL)
		my_error("opendir",__LINE__);
	
	//复制目录文件名到二位字符数组中
	for( count1 = 0; (perfile = readdir(dir)) != NULL; count1++)
		strcpy(filename[count1],perfile->d_name);
	
	//排序
	for( i = 0; i < count1 + 1; i++)
	      for( j = 0; j < count1 - 1 - i; j++)
		{
			if((strcmp(filename[j],filename[j+1])) > 0 )
			{
				strcpy(temp,filename[j]);
				strcpy(filename[j],filename[j+1]);
				strcpy(filename[j+1],temp);
			}
		}
	
	show(filename);
	
	printf("\n");
	return 0;
}

int display_para(char *para,char *path)
{
	
	if(strncmp(para,"-t",2) == 0)
	      display_next(path);
	else if(strncmp(para,"-a",2) == 0)
	      display_all(path);
	else if(strncmp(para,"-l",2) == 0)
	      display_property(path);

	return 0;
}

void display_next(char *path)
{
	DIR 		*dir;
	struct dirent 	*into;
	struct stat 	buf;

	if((dir = opendir(path)) == NULL)
	      my_error("opendir error",__LINE__);

	chdir(path);
	while((into = readdir(dir)) != NULL)
	{ 
		if(lstat(into->d_name,&buf) == -1)		
		      my_error("lstat",__LINE__);

		if(S_ISDIR(buf.st_mode))
		{
			if(strcmp(".",into->d_name) == 0 || strcmp("..",into->d_name) == 0)
			{continue;}
			display_next(into->d_name);
		}
		else
		      show_t(into->d_name);	
	}

	chdir("..");

	closedir(dir);
}

void show_t(char *filename)
{
	char 	path_max[256];

	if(getcwd(path_max,256) == NULL)
		my_error("getcwd",__LINE__);

	printf("%s / %s\n",path_max,filename);
}

int display_property(char *path)
{
	DIR 		*dir;
	struct dirent 	*into;
	struct stat 	*buf;

	if((dir = opendir(path)) == NULL)
	      my_error("opendir",__LINE__);

	return 0;
}
