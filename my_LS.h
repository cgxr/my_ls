/*
 * =====================================================================================
 *
 *       Filename:  my_LS.h
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  2013年07月20日 08时41分02秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Hurley (LiuHuan), liuhuan1992@gmail.com
 *        Company:  Class 1107 of Computer Science and Technology
 *
 * =====================================================================================
*/

void 	my_error(const char *err_string,int line);
int 	display(char *para,char *path);
void 	display_present(char *path,char judge);
void 	show(char filename[][256]);
int 	display_para(char *para,char *path);
void 	display_next(char *path);
void 	show_t(char *filename);
void 	display_property(char *path);
void  	show_mode(struct stat *buf,char *filename);
void 	change_path(char *path,char *change_name,int move);

