#include "store.h" 
#include "store_db.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
// #define FILE_NAME "depts.CSV" //option


// int check_validtion(char *line)
// {
//     return 1 ;quit
// }
void trim(char *s)
{
    int i= 0 ;
    int j = strlen(s) -1;
    int x ;
    int tmp ;
    while (s[i] && (s[i] == ' ' || s[i] == '\t'))
    {
        i++ ;
    }
    if(!s[i])
    {
        s[0] = '\0' ;
        return ;
    }
    while ((s[j] == ' ' || s[j] == '\t' || s[j] == '\n'))
    {
        s[j] = '\0' ;
        j-- ;
    } 
    tmp = i ;
    i = 0 ;
    x = strlen(s) ;
    while (i <= x)
    {
       s[i] = s[tmp] ;
       i++ ;
       tmp ++ ;
    }  
}

void rm_ex_wtspace(char *str)
{
    int len = strlen(str);
    for(int i=0; i<len; i++)
    {
        if((str[i] == 32 && str[i+1]==32) || (str[i]=='\t' && str[i+1]=='\t') )
        {
            for(int j=i; j<(len-1); j++)
            {
                str[j] = str[j+1];
            }
            len--;
            str[len]='\0';
            i = 0;
        }
    }

}


void lower(char *str)
{
    for(int i=0;i<=strlen(str);i++)
    {
        if(str[i]>=65&&str[i]<=90)
        {
            str[i]=str[i]+32;
        }
    }
}

void insertSorted(client **Head, client *node)
{
    client **Next = Head;

    while (*Next && (*Next)->dept_sum < node->dept_sum)
    {
        Next = &((*Next)->next);
    }
    node->next = *Next;
    *Next = node;
}

void free_one(client *clt)
{
    free(clt->first_name) ;
    free(clt->second_name) ;
    free(clt) ;
}

void free_db(client *node)
{
    while (node)
    {
        client *tmp = node ;
        node = node->next ;
        free_one(tmp) ;
    }
    
}
int set_new(client **head ,client *new ,int line)
{
    client *curent = *head ;
    client *prev = NULL ;
    int code ;

    if (!valid_all(new ,line))
    {
        return -1 ;
    }
    
    if (!(*head))
    {
       *head = new ;
       return 0 ;
    }
    
    while (curent)
    {
        if(curent->id == new->id)
        {
            curent->dept_sum += new->dept_sum ;
            strcpy(curent->last_date,new->last_date) ;
            strcpy(curent->phone,new->phone) ;
            code = 1 ;
            break;
        }
        prev = curent ;
        curent = curent->next ;
    }

    if (!prev)
    {
        *head = curent->next ;
        insertSorted(head,curent) ;
        return  1 ;
    }

    if (!curent)
    {   
        code = 0 ;
        curent = new ;
    }

    prev->next = curent->next ;
    insertSorted(head,curent) ;
    return code ;
}



client *parse(char *line ,int line_number)
{
    client *new ;
    char *filds[6] = {NULL} ;
    char *tok = NULL ;
    int i = 0 ;
    const char d[2] = ",";
    
    tok = strtok(line, d) ;
    for (i = 0; i < FILD_COUNT && tok; i++)
    {
        filds[i] = tok ;
        tok = strtok( NULL, d) ;
    }
    
    if (i < 5)
    {
        printf("Erorr by reading line %d : mising filds.. cont add data to database\n" ,line_number) ;
        return NULL ;
    }
    
    new = malloc(sizeof(client)) ;
    if(!new)
    {
        puts("Problem.. can't allocte memery") ;
        return NULL;
    }

    new->first_name = strdup(filds[0]) ;
    new->second_name = strdup(filds[1]) ;
    new->id=atoi(filds[2]);
    sscanf(filds[3],"%s",new->phone) ;
    new->dept_sum = atof(filds[4]);
    sscanf(filds[5],"%s",new->last_date) ; 
    new->next = NULL ;

    return new ;

}


void show_one(client *head)
{
     printf("%s %s %d %s %f %s\n",head->first_name,head->second_name,head->id ,head->phone ,head->dept_sum ,head->last_date) ;
}

void show_db(client *head)
{   
    client *clt = head ;
    while (clt)
    {
        show_one(clt) ;
        clt = clt->next ;
    }
}

void show_if(client *head ,char *what ,int(*compair)(client *node,char *oprand), char op )
{
    int x ;
    int found = 0 ;
    while (head)
    {
        x = incondition(head,what,compair,op) ;
        if(x)
        {
            found = 1 ;
            show_one(head) ;
        }
        head = head->next ;
    } 
    if(!found)
    {
        puts("not found") ;
    }
}

void orgenize_db(char *file_name ,client **db_head)
{
    client *head = NULL;
    FILE *file ;
    file = fopen(file_name,"r") ;
    int counter = 0 ;
    while (1)
    {
        client *new = NULL ;
        char line[300] = {0};
       
        if(!fgets(line, sizeof(line), file))
		{
			break;
		}
        lower(line) ;
        counter ++ ;
        new = parse(line,counter) ;
        if(new)
        {
            int x =  set_new(&head,new,counter);
            if (x != 0)
            {
                free_one(new) ;
            }
            
        }
    }
    *db_head = head ;
    fclose(file) ;
} 

int db_select(client *head, enum filds op1 ,char operator ,char *op2)
{
    int found = 0 ;
    switch (op1)
    {
    case FIRST_NAME:
        show_if(head,op2,compair_fname,operator) ;
        break;
    case SECOND_NAME:
        show_if(head,op2,compair_sname,operator) ;
        break;  
    case ID:
        show_if(head,op2,compair_id,operator) ;
        break;
    case POHNE:
         show_if(head,op2,compair_phone,operator) ;
        break;   
    case DATE:
        show_if(head,op2,compair_date,operator) ;
        break;
    case DEBT: //slightly long but save run time...
        if( operator == '!')
        {
            show_if(head,op2,compair_debt,operator) ;
            break;
        }
        while (head)
        {
            float dbt = (float)atof(op2) ;
            if(head->dept_sum > dbt) 
            {
                if (operator == '>')
                {
                    show_db(head) ;
                }
                else
                {
                    (found) ? printf(" \b") : printf("not found");
                }
                break;
            }
            else if(head->dept_sum == dbt && operator != '>')
            {
                if(operator == '<') 
                {
                    (found) ? printf(" \b") : printf("not found");
                    break;
                }
                show_one(head) ;
                found = 1 ;
            }
            else if((operator == '<'))
            {
                show_one(head) ;
                found = 1 ;
            }
            head = head->next ;
            if (!head && !found)
            {
                puts("not found") ;
            }   
        }
        break;
    default: 
        break;
    }
    return found ;
}

void database(char *file_name)
{
    printf("WELCOM TO STORE DATABASE\n\n") ;
    client *db_head = NULL;
    FILE *file ; 
    file = fopen(file_name,"a+") ;
    orgenize_db(file_name,&db_head) ;
    show_db(db_head) ;
    printf("\n") ;
    
    
    while (1)
    {
        char query_string[1024] ={0};
        char q_type[10]         ={0};
        char expretion[1000]    ={0};
        char c ;
        char op1[20] = {0};
        char op2[50] = {0} ;
        const char d[2] = ",";
        int i ;
        int x = 0 ;
        char *fild_nams[6] ={"first name","second name" ,"id" ,"phone" ,"date" ,"debt"} ;
        enum filds fld[6] = {FIRST_NAME,SECOND_NAME,ID,POHNE,DATE,DEBT} ;
        printf("Please enter the comand (select ,set ,show ,or 'quit' to exit)--> ") ;
        
        fgets(query_string,300,stdin) ;
        
        if(query_string[strlen(query_string)- 1] == '\n')
        {
            query_string[strlen(query_string)- 1] = 0 ;
        }
        lower(query_string) ;
        rm_ex_wtspace(query_string);
        trim(query_string) ;
        puts(expretion) ;
        x = sscanf(query_string ,"%9s %1000[^\t]", q_type,expretion) ;

        if(!strcmp(q_type,"quit"))
        {
           break;
        }
        else if (!strcmp(q_type,"show"))
        {
            show_db(db_head) ;
        }
        else if(x < 2)
        {
            puts("invalid syntsx ! syntax : COMAND < FILD >  < OPERATOR (> , < , = ,!=) > < PARAMETER > ") ;
            continue;
        }
        else if (!strcmp(q_type,"select"))
        {
            for (i = 0; i < strlen(expretion) -1; i++)
            {
                c = expretion[i] ;
                if(c == '=' || c == '<' || c == '>' ||(c == '!' && expretion[i+1] == '=' && expretion[i+2] ))  
                {
                    if ((c == '!'))
                    {
                        expretion[i+1] = '\t' ;
                    }
                    break;
                }
            }


            if ( i == strlen(expretion) -1)
            {
                puts("Error ! syntax : < FILD >  < OPERATOR (> , < , = ,!=) > < PARAMETER >") ;
                continue;
            }

            sscanf(expretion,"%19[^=^!^>^<] %49[^\n]" ,op1 ,op2);
            op2[0] = '\t' ;
            trim(op1) ;
            trim(op2) ;
            
            for (i = 0; i < FILD_COUNT; i++)
            {
                if(!strcmp(fild_nams[i] ,op1))
                {
                    db_select(db_head,fld[i],c,op2) ;
                    break;
                }
            }

            if (i == FILD_COUNT)
            {
                printf("Error Unknown type %s\n" ,op1) ;
            }
            
        }
        else if(!strcmp(q_type,"set"))
        {
           unsigned char all_filds =  (3 << 6) ; // 11000000 
           client *new = malloc(sizeof(client)) ;
           new->next = NULL ;
           char *exp = NULL;

           exp = strtok(expretion,d) ;
            
           for (int ex = 0; ex < FILD_COUNT; ex++)
           {
                if (!exp)
                {
                    printf("Erorr : mising filds.. cont add data to database\n") ;
                    break;
                }
                
                for (i = 0; i < strlen(exp) -1 ; i++)
                {
                    c = exp[i] ;
                    if(c == '=') 
                    {
                        break;
                    }
                }
                
                if ( i == strlen(exp)-1)
                {
                    printf("Error : invalid operator %c . options : > , < = , != ", c) ;
                    break;
                } 
                
                sscanf(exp,"%19[^=] %49[^\t]" ,op1 ,op2);
                op2[0] = '\t' ;
                trim(op1) ;
                trim(op2) ;

                if(!strcmp("first name" ,op1))
                {
                   new->first_name = strdup(op2)  ;
                   all_filds |= FIRST_NAME;
                }
                else if (!strcmp("second name" ,op1))
                {
                   new->second_name = strdup(op2) ;
                   all_filds |= SECOND_NAME;
                }
                else if(!strcmp("id" ,op1))
                {
                   new->id = atoi(op2) ;
                   all_filds |= ID ;
                }
                else if(!strcmp("phone" ,op1))
                {
                   strcpy(new->phone,op2) ;
                   all_filds |= POHNE ;
                }
                else if(!strcmp("debt" ,op1))
                {
                   new->dept_sum = atof(op2) ;
                   all_filds |= DEBT ;
                }
                else if(!strcmp("date" ,op1))
                {
                   strcpy(new->last_date,op2) ;
                   all_filds |= DATE ;
                }
                else
                {
                    printf("Error Unknown type %s\n please try again.\n" ,op1) ;
                    break;
                }    
                exp = strtok(NULL,d) ;
           }
           if(all_filds == 255)
           {
                show_one(new) ;
                int x = set_new(&db_head, new ,0) ;
                if(x > -1)
                {
                    fprintf(file,"%s,%s,%d,%s,%f,%s\n",new->first_name,new->second_name,new->id ,new->phone ,new->dept_sum ,new->last_date) ;
                }

                if (x != 0)
                {
                    free_one(new) ;
                }
           }
           else
           {
                free_one(new) ;
                for (i = 0; i < FILD_COUNT; i++)
                {
                    if (!(all_filds & fld[i]))
                    {
                        printf("mising : %s\n" ,fild_nams[i]) ;
                    }
                    
                } 
           }  
        }
        else
        {
            printf("Unknown type %s\n" ,op1) ;
        }
    }
    fclose(file) ;
   
}


