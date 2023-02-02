#include "store_db.h"
#include "string_handl.h"

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
    if (clt->first_name)
    {
        free(clt->first_name) ;
    }
    if (clt->second_name)
    {
        free(clt->second_name) ;
    }
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
int set_new(client **head ,client *new ,int line,char *er_buf)
{
    client *curent = *head ;
    client *prev = NULL ;
    int code ;
    char er_line[50] = {0} ;
    if (!valid_all(new ,line,er_buf))
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
            if(!(!strcmp(curent->first_name ,new->first_name) && !strcmp(curent->second_name,new->second_name)))
            {
                sprintf(er_line,"conflict ! in line %d : same id but not same name\n",line);
                if (er_buf)
                {
                    strcpy(er_buf,er_line) ;
                }
                puts(er_line) ;
                return -1 ;
            }
            
            strcpy(curent->last_date,new->last_date) ;
            strcpy(curent->phone,new->phone) ;
            curent->dept_sum += new->dept_sum ;
            code = 1 ;
            break;
        }
        else if((!strcmp(curent->first_name ,new->first_name) && !strcmp(curent->second_name,new->second_name)))
        {
            sprintf(er_line,"conflict ! in line %d : same name but not same id\n",line);
            if (er_buf)
            {
                strcpy(er_buf,er_line) ;
            }
            else
            {
                puts(er_line) ;
            }
            
            return -1 ;
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

void insert_to_buf(client *head,char *buf ,void(*target)(char*,void*),void *props)
{
    char line[120] = {0} ;
    sprintf(line ,"%-25s %-25s %09d        %-15s %-15f %s\n",head->first_name,head->second_name,head->id ,head->phone ,head->dept_sum ,head->last_date) ;
    if (BUFER_SIZE - strlen(buf) <= strlen(line))
    {
        target(buf,props) ;
        memset(buf,0,BUFER_SIZE) ;
    }
    strcat(buf,line) ;
}

void show_db(client *head,char *buf ,void(*target)(char*,void*) ,void *props)
{   
    client *clt = head ;
    while (clt)
    {
        insert_to_buf(clt,buf,target,props) ;
        clt = clt->next ;
    }
}

void show_if(client *head ,select_props sp,int(*compair)(client *node,char *oprand),char *buf ,void(*target)(char*,void*),void *props)
{
    int x ;
    int found = 0 ;
    while (head)
    {
        x = incondition(head,sp.option,compair,sp.operator) ;
        if(x)
        {
            found = 1 ;
            insert_to_buf(head,buf,target,props) ;
        }
        head = head->next ;
    } 
    if(!found)
    {
        strcat(buf,"not found\n") ;
    }
}

void orgenize_db(FILE *file,client **db_head)
{
    client *head = NULL;
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
            int x =  set_new(&head,new,counter,NULL);
            if (x != 0)
            {
                free_one(new) ;
            }
        }
    }
    *db_head = head ;
} 

void db_select(client *head ,select_props sp,void(*target)(char*,void*),char *buf ,void *props)
{
    int found = 0 ;
    switch (sp.fild)
    {
    case FIRST_NAME:
        show_if(head,sp,compair_fname,buf,target,props) ;
        break;
    case SECOND_NAME:
        show_if(head,sp,compair_sname,buf,target,props) ;
        break;  
    case ID:
        show_if(head,sp,compair_id,buf,target,props) ;
        break;
    case POHNE:
        show_if(head,sp,compair_phone,buf,target,props) ;
        break;   
    case DATE:
        show_if(head,sp,compair_date,buf,target,props) ;
        break;
    case DEBT: //slightly long but save run time...
        if(sp.operator == '!')
        {
            show_if(head,sp,compair_debt,buf,target,props) ;
            break;
        }
        while (head)
        {
            float dbt = (float)atof(sp.option) ;
            if(head->dept_sum > dbt) 
            {
                if (sp.operator == '>')
                {
                    show_db(head,buf,target,NULL) ;
                }
                else
                {
                    if (!found)
                    {
                       strcat(buf,"not found\n") ;
                    } 
                }
                break;
            }
            else if(head->dept_sum == dbt && sp.operator != '>')
            {
                if(sp.operator == '<') 
                {
                    if (!found)
                    {
                       strcat(buf,"not found\n") ;
                    } 
                    break;
                }
                insert_to_buf(head,buf,target,NULL) ;
                found = 1 ;
            }
            else if((sp.operator == '<'))
            {
                insert_to_buf(head,buf,target,NULL) ;
                found = 1 ;
            }
            head = head->next ;
            if (!head && !found)
            {
                strcat(buf,"not found\n") ;
            }   
        }
        break;
    default: 
        break;
    }
}
after_pars parse_query(char *q_str)
{
    after_pars af_pars = {} ;
    char q_type[10] ={ 0} ;
    char expretion[1000] = {0} ;
    char c ;
    char op1[20] = {0};
    char op2[25] = {0} ;
    const char d[2] = ",";
    int i ;
    int x = 0 ;
    char *fild_nams[6] ={"first name","second name" ,"id" ,"phone","debt","date" } ;
    enum filds fld[6] = {FIRST_NAME,SECOND_NAME,ID,POHNE,DEBT,DATE} ;

    if(q_str[strlen(q_str)- 1] == '\n')
    {
        q_str[strlen(q_str)- 1] = 0 ;
    }
        lower(q_str) ;
        rm_ex_wtspace(q_str);
        trim(q_str) ;

    x = sscanf(q_str ,"%9s %1000[^\t]", q_type,expretion) ;

    if (!strcmp(q_type,"quit"))
    {
        af_pars.q_type = QUIT ;
    }
    else if(!strcmp(q_type,"select"))
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
            af_pars.q_type = ERROR ;
            strcpy(af_pars.error_str,"Error !. syntax : < FILD >  < OPERATOR (> , < , = ,!=) > < PARAMETER >") ;
            return af_pars ;
        }

        sscanf(expretion,"%19[^=^!^>^<] %24[^\n]" ,op1 ,op2);
        op2[0] = '\t' ;
        trim(op1) ;
        trim(op2) ;

        for (i = 0; i < FILD_COUNT; i++)
        {
            if(!strcmp(fild_nams[i] ,op1))
            {
                af_pars.sp.fild = fld[i] ;
                af_pars.q_type = SELECT ;
                af_pars.sp.operator = c ;
                strcpy(af_pars.sp.option , op2) ;
                break ;
            }
        }
        if (i == FILD_COUNT)
        {
            printf("Error: Unknown type %s\n" ,op1) ;
        }
    }
    else if(!strcmp(q_type,"set"))
    {
        unsigned char all_filds =  (3 << 6) ; // 11000000 
        client *new = malloc(sizeof(client)) ;

        if(!new)
        {
            puts("Problem.. can't allocte memery") ;
            af_pars.q_type = ERROR ;
            strcpy(af_pars.error_str,"A problem occurred in the database\n") ;
            return af_pars ;
        }
        new->next = NULL ;
        new->first_name = NULL ;
        new->second_name = NULL ;
        char *exp = NULL ;

        exp = strtok(expretion,d) ;
            
        for (int ex = 0; ex < FILD_COUNT; ex++)
        {
            if (!exp)
            {
                af_pars.q_type = ERROR ;
                strcpy(af_pars.error_str,"Erorr : mising filds.. cont add data to database\n") ;
                return af_pars ;
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
                af_pars.q_type = ERROR ;
                sprintf(af_pars.error_str ,"Error : invalid operator %c . options : > , < = , != ", c) ;
                return af_pars ;
            } 
            
            sscanf(exp,"%19[^=] %24[^\t]" ,op1 ,op2);
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
                if (strlen(op2) != 9)
                {
                    af_pars.q_type = ERROR ;
                    sprintf(af_pars.error_str ,"Error : id must be 9 numbers %s\n please try again.\n" ,op1) ;
                    free_one(new) ;
                    return af_pars ;
                }
                
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
                af_pars.q_type = ERROR ;
                sprintf(af_pars.error_str ,"Error : Unknown fild %s\n please try again.\n" ,op1) ;
                free_one(new) ;
                return af_pars ;
            }    
            exp = strtok(NULL,d) ;
        }

        if(all_filds == 255)
        {
            af_pars.q_type = SET ;
            af_pars.new = new ;    
        }
        else
        {
            free_one(new) ;
            af_pars.q_type = ERROR ;
            strcpy(af_pars.error_str,"Incorrect data entry\n\t") ;
            for (i = 0; i < FILD_COUNT; i++)
            {
                if (!(all_filds & fld[i]))
                {
                    sprintf(af_pars.error_str + strlen(af_pars.error_str),"mising : %s\n" ,fild_nams[i]) ;
                } 
            } 
        }  
    }   
    else if(!strcmp(q_type,"print"))
    {
        af_pars.q_type = PRINT ;
    }
    else if (x < 2)
    {
        af_pars.q_type = ERROR ;
        strcpy(af_pars.error_str,"invalid syntsx!. syntax: COMAND < FILD > < OPERATOR(>,<,=,!=)> <PARAMETER>\n") ;
    }
    else
    {
        af_pars.q_type = ERROR ;
        sprintf(af_pars.error_str,"Unknown type %s\n" ,op1) ;
    }
    return af_pars ;
}
