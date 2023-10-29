#ifndef STORE_DB_H
#define STORE_DB_H
#include "store.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

enum Q_TYPE {
    ERROR = -1 ,
    SELECT,
      SET,
    PRINT,
    QUIT,
};

typedef struct select_props
{
  enum fields field ;
  char operator ; 
  char arg[50] ;
}select_props;

union task_data 
{
    select_props sp ;
    client *new ;
    char error_str[200] ;
}task_data;

typedef struct Task
{
  enum Q_TYPE q_type ;
  union task_data data ;
}Task;

int set_new(client **head ,client *new ,int line,char *er_buf) ;
void orgenize_db(FILE *file,client **db_head);
void show_db(client *head,char *buf ,void(*target)(char*,void*) ,void *props) ;
void show_select(client *head,select_props sp,char *buf,void(*target)(char*,void*),void *props) ;
void free_one(client *clt);
void free_db(client *node) ;
Task parse_query(char *q_str) ;



#endif


