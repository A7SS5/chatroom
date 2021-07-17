#ifndef MYSQLC_H_
#define MYSQLC_H_
#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include <mysql/mysql.h>
void my_err(const char *error_string, int line);
MYSQL accept_mysql(void);
int use_mysql(const char *string, MYSQL mysql1);
int close_mysql(MYSQL mysql);
int judge(char name[20],char password[20]);
#endif