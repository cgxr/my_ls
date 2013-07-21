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

#include "my_LS.h"

void change_path(char *path,char *change_name,int move)
{
	char 	*pathmove;
	char 	pathchange[80];

	for( pathmove = path; *(pathmove + move) != '\0'; pathmove++)
		*pathmove = *(pathmove + move);	

	*pathmove = '\0';	
	strcpy(pathchange,path);
	strcpy(path,change_name);
	strcat(path,pathchange);
	
}
int main(int argc, char *argv[])
{
	char 	path[80] = {'\0'};
	char 	*pathmove;
	char 	para[4] = "\0";
	int  	count1;
	char 	judge = 'a';
	char 	now[80] = "/home/chang/cgxr/";
	char 	father[80] = "/home/chang/";

	printf("input the path like this\n\t/home/chang or ./aaa\n");
	scanf("%s",path);

	//路径可以输入，绝对路径不用管，相对路径或错误路径的处理
	if(strncmp(path,"./",2) == 0)
	{    
		//相对路径去掉./并加上文件执行的路径，使之成为绝对路径
		change_path(path,now,2);	
	}
	else if((strncmp(path,"../",3)) == 0)
	 //转化父目录
	      change_path(path,father,3);
	else if((strncmp(path,"/home",5) != 0) && (strncmp(path,"./",2) != 0))
	{ 
		printf("error 无效的路径\n");
		exit(0);
	}
	else
	{
		pathmove = path;
		while(*(pathmove + 1) != '\0') 
		      pathmove++;
		if(*pathmove == '/')
		      *pathmove = '\0';
	}
	printf("now path is %s\n",path);

	//先把参数，路径存数组中，再分有参无参讨论
	for ( count1 = 1; count1 < argc; count1++)
	      if(argv[count1][0] == '-')
		    strncpy(para,argv[count1],2);

	//因为最多出现两个参数，所以有参无参以2为界
	if (argc < 2)
	      display_present(path,judge);
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
	while ( strncmp(filename[i],"\0",2) != 0)
	{	
		if (maxlen < strlen(filename[i+1]))
		      maxlen = strlen(filename[i+1]);
		i++;
	}
	
	//对其输出，利用最大文件名对齐，补空格
	i = 0;
	linemax = maxlen;
	while (strncmp(filename[i],"\0",2) != 0)
	{
		nowlen = strlen(filename[i]);
		if ((linemax + 2 ) > 120)
		{
			printf("\n");
			printf("%s",filename[i]);
			for ( j = 0; j < maxlen - nowlen + 2; j++)
			      printf(" ");
			linemax = maxlen;
			i++;
		}
		else
		{      
			printf("%s",filename[i]);
			for ( j = 0; j < maxlen - nowlen + 2; j++)
			    printf(" ");
			linemax += (maxlen + 2);
			i++;
		}
	}
	printf("\n");
}

void display_present(char *path,char judge)
{
	DIR 		*dir;
	struct dirent 	*perfile;
	char  		filename[4096][256]={"\0","\0"};
	int 		count1,i,j;
	char 		temp[256];

	dir = opendir(path);
	if (dir == NULL)
		my_error("opendir",__LINE__);
	
	//复制目录文件名到二位字符数组中
	for ( count1 = 0; (perfile = readdir(dir)) != NULL; count1++)	
	{
		if(judge == 'a')	      	      
		{    
			if (strncmp(perfile->d_name,".",1) != 0 && strncmp(perfile->d_name,"..",2) != 0)				
			{			
				strcpy(filename[count1],perfile->d_name);				    
			}		    
			else
		      		count1--;		
		}
		else
		      strcpy(filename[count1],perfile->d_name);		
	}

	//排序
	for ( i = 0; i < count1 + 1; i++)
	      for ( j = 0; j < count1 - 1 - i; j++)
		{
			if ((strcmp(filename[j],filename[j+1])) > 0 )
			{
				strcpy(temp,filename[j]);
				strcpy(filename[j],filename[j+1]);
				strcpy(filename[j+1],temp);
			}
		}
	
	show(filename);

	closedir(dir);
}

int display_para(char *para,char *path)
{
	if (strncmp(para,"-t",2) == 0)
	      display_next(path);
	else if (strncmp(para,"-a",2) == 0)
	      display_present(path,'-');
	else if (strncmp(para,"-l",2) == 0)
	      display_property(path);

	return 0;
}

void display_next(char *path)
{
	DIR 		*dir;
	struct dirent 	*into;
	struct stat 	buf;

	if ((dir = opendir(path)) == NULL)
	      my_error("opendir error",__LINE__);

	chdir(path);
	while ((into = readdir(dir)) != NULL)
	{ 
		if (lstat(into->d_name,&buf) == -1)		
		      my_error("lstat",__LINE__);

		if (S_ISDIR(buf.st_mode))
		{
			if (strcmp(".",into->d_name) == 0 || strcmp("..",into->d_name) == 0)
			{
				continue;
			}
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

	if (getcwd(path_max,256) == NULL)
		my_error("getcwd",__LINE__);

	printf("%s / %s\n",path_max,filename);
}

void display_property(char *path)
{
	DIR 		*dir;
	struct dirent 	*into;
	struct stat 	buf[4096];
	int i;

	chdir(path);
	if ((dir = opendir(path)) == NULL)
	      my_error("opendir",__LINE__);

	while ((into = readdir(dir)) != NULL)
	{
		if (lstat(into->d_name,&buf[i]) == -1)
		      my_error("lstat",__LINE__);
	
		show_mode(&buf[i],into->d_name);
		i++;
	}

	closedir(dir);
}


void show_mode(struct stat * buf,char *filename)
{ 	
	struct group 	*gr;
	struct passwd 	*pass;
	char 		time[32];
	//权限第一项，是否为目录，字符设备，块设备，符号链接
	if (S_ISDIR(buf->st_mode))
	      putchar('d');
	else if (S_ISCHR(buf->st_mode))
	      putchar('c');
	else if (S_ISBLK(buf->st_mode))
	      putchar('b');
	else if (S_ISLNK(buf->st_mode))
	      putchar('l');
	else  putchar('-');

	//用户权限属性
	if(buf->st_mode & S_IRUSR) putchar('r');
	else putchar('-');
	if(buf->st_mode & S_IWUSR) putchar('w');
	else putchar('-');
	if(buf->st_mode & S_IXUSR) putchar('x');
	else putchar('-');
	
	//组权限属性
	if(buf->st_mode & S_IRGRP) putchar('r');
	else putchar('-');
	if(buf->st_mode & S_IWGRP) putchar('w');
	else putchar('-');
	if(buf->st_mode & S_IXGRP) putchar('x');
	else putchar('-');

	//其他人权限属性
	if(buf->st_mode & S_IROTH) putchar('r');
	else putchar('-');
	if(buf->st_mode & S_IWOTH) putchar('w');
	else putchar('-');
	if(buf->st_mode & S_IXOTH) putchar('x');
	else putchar('-');

	putchar(' ');

	pass = getpwuid(buf->st_uid);
	gr  = getgrgid(buf->st_gid);
	printf("%-4d",(int)buf->st_nlink);
	printf("%-8s",pass->pw_name);
	printf("%-8s",gr->gr_name);
	printf("%-10d",(int)buf->st_size);
	strcpy(time,ctime((time_t*)&buf->st_mtim));
	time[strlen(time)-1] = '\0'; //由于函数在转化时自动加上了\n
	printf("%-20s",time);
	printf("   %s",filename);

	putchar('\n');
} 
