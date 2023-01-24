#ifndef STORE_H
#define STORE_H
#define  FILD_COUNT 6

enum filds{
    FIRST_NAME    =  (1 << 0),
    SECOND_NAME   =  (1 << 1),
    ID            =  (1 << 2),
    POHNE         =  (1 << 3),
    DEBT          =  (1 << 4),
    DATE          =  (1 << 5),
};
typedef struct client {
    char *first_name ;
    char *second_name ;
    int id ;
    char phone[10] ;  
    float dept_sum ;
    char last_date[12] ; 
    struct client *next ;

}client;
int valid_all(client *clt ,int line) ;
int compair_fname(client *node ,char *name) ;
int compair_sname(client *node ,char *what) ;
int compair_id(client *node ,char *what);
int compair_phone(client *node ,char *what);
int compair_date(client *node ,char *what) ;
int compair_debt(client *node ,char *what) ;
int incondition(client *node,char *what ,int(*compair)(client *node,char *oprand) ,char c) ;
#endif