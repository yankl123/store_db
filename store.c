#include "store.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
unsigned int date_code(const char *date_string)
{
    char date_str[12] ;
    unsigned int day ;
    unsigned int month   ;
    unsigned int yare ;
    unsigned int date = 0 ;
    int v ;
    strcpy(date_str ,date_string);
    for (int i = 0; i < strlen(date_string); i++)
    {
        if(date_str[i] == '/')
        {
            date_str[i] = '\t' ;
        }
    }
    v = sscanf(date_str,"%u %u %u", &day ,&month ,&yare) ;
    if (v != 3 || !(day > 0 && day < 32) || !(month > 0 && month < 13) || !(yare > 1970 && yare < 2024)) 
    {
        return 0 ;
    }

    date |= yare << 9 ;
    date |= month << 5 ;
    date |= day ; 
    return date ;
}

int compair_fname(client *node ,char *what)
{
    int x = strcmp(node->first_name,what) ;
    if (x == 0)
    {
        return x ;
    }
    return (x > 0) ? 1 : -1 ;
}

int compair_sname(client *node ,char *what)
{
    int x = strcmp(node->second_name,what) ;
    if (x == 0)
    {
        return x ;
    }
    return (x > 0) ? 1 : -1 ;
}

int compair_id(client *node ,char *what)
{
    int id = atoi(what) ;
    int x = node->id - id ;
    if (x == 0)
    {
        return 0;
    }
    return (x > 0) ? 1 : -1 ;
}

int compair_phone(client *node ,char *what)
{
    int x = strcmp(node->phone,what) ;
    if (x == 0)
    {
        return x ;
    }
    return (x > 0) ? 1 : -1 ;
}

int compair_debt(client *node ,char *what)
{
    float debt = atof(what) ;
    float x = node->dept_sum - debt ;
    if (x == 0)
    {
        return 0;
    }
    return (x > 0) ? 1 : -1 ;
}

int compair_date(client *node ,char *what)
{
    unsigned int x = date_code(node->last_date) ;
    unsigned int y = date_code(what) ;

    if (x == y)
    {
        return 0 ;
    }
    return (x < y) ?  -1 : 1 ;
}
int incondition(client *node,char *what ,int(*compair)(client*,char*) ,char c) 
{
    int result ;
    result = compair(node,what) ;
    switch (c)
    {
    case '=':
        return result == 0 ? 1 : 0;
        break;
    case '<':
        return result == -1 ? 1 : 0;
        break;
    case '>':
        return result == 1 ? 1 : 0;
        break;
    case '!':
        return result != 0 ? 1 : 0;
        break;
    default:
        break;
        return 0 ;
    }

    return 0 ;
}

int ispos_nume(char *num_str)
{
    for (int i = 0; i < strlen(num_str); i++)
    {
        if (!(num_str[i] > 47 && num_str[i] < 58))
        {
            return 0 ;
        }
    }
    return 1 ;
}

unsigned int valid_name(char *name)
{
    int len = strlen(name) ;
    char shtuyot[45] ;
    sscanf(name,"%[^\'^\"^%^{^}^;^[^]\\^?^=^]",shtuyot) ;
    if (len > strlen(shtuyot))
    {
       return 0 ;
    }
    
    return FIRST_NAME ;
}

unsigned int valid_Sname(char *sname)
{
    if (!valid_name(sname))
    {
        return 0 ;
    }
    return SECOND_NAME ;
}

unsigned int valid_id(int id)
{
   if (!id)
   {
        return 0 ;
   }
   return ID ;
}

unsigned int valid_pohne(char *phone)
{
    if (!ispos_nume(phone)|| strlen(phone) != 10 || phone[0] != '0')
    {
        return 0 ;
    }
    return POHNE ;
}

unsigned int valid_date(char *dt_str)
{
    int x = date_code(dt_str) ;
    if(!x)
    {
        return 0 ;
    }
    return DATE ;
}

unsigned int valid_debt(float debt)
{
    if(!debt)
    {
        return 0 ;
    }
    return DEBT ;
}

int valid_all(client *clt ,int line,char *err_buf)
{
    unsigned char all_valid = (3 << 6) ; //11000000
    enum filds fld[6] = {FIRST_NAME,SECOND_NAME,ID,POHNE,DEBT,DATE} ;
    char *ermsg[6] = {
        "\tinvalide first name : Strange letters\n",
        "\tinvalide second name : Strange letters\n",
        "\tinvalid id : must be number int range 1-999999999\n",
        "\tinvalide phone number :must be number with length 10 and first number 0\n",
        "\tinvalid debt sum must be a number GREATE OR LOWER then 0\n",
         "\tinvalide date :\n",
        
    } ;
    char error_str[500] = {0};

    all_valid |= valid_name(clt->first_name) ;
    all_valid |= valid_Sname(clt->second_name) ;
    all_valid |= valid_id(clt->id) ;
    all_valid |= valid_pohne(clt->phone) ;
    all_valid |= valid_debt(clt->dept_sum) ;
    all_valid |= valid_date(clt->last_date) ;
  
    if (all_valid == 255)
    {
        return 1 ;
    }
    else
    {
        sprintf(error_str,"found error in data of line %d\n" ,line) ;
        for (int i = 0; i < FILD_COUNT; i++)
        {
            if(!(all_valid & fld[i]))
            {
                strcat(error_str ,ermsg[i]) ;
            }
        } 
        if (err_buf)
        {
            strcpy(err_buf,error_str) ;
        }
        else
        {
            puts(error_str) ;
        }
        return 0 ;
    }

}