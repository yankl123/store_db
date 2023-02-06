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
  enum filds fild ;
  char operator ; 
  char option[50] ;
}select_props;

typedef struct after_pars
{
  enum Q_TYPE q_type ;
  union 
  {
    select_props sp ;
    client *new ;
    char error_str[200] ;
  };
}after_pars;

int set_new(client **head ,client *new ,int line,char *er_buf) ;
void orgenize_db(FILE *file,client **db_head);
void show_db(client *head,char *buf ,void(*target)(char*,void*) ,void *props) ;
void show_select(client *head,select_props sp,char *buf,void(*target)(char*,void*),void *props) ;
void free_one(client *clt);
void free_db(client *node) ;
after_pars parse_query(char *q_str) ;



#endif


