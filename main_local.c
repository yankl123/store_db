#include "store_db.h" 

void print_buf(char *buf ,void *props)
{
    if (!props)
    {
        printf("%s",buf) ;
    }
}

int main(int argc, char **argv)
{
    FILE *file ;
    client *db = NULL;
    int work = 1 ;
    char buffer[BUFFER_SIZE] = {0} ;
    int x ;
    char *field_names[] ={"FIRST_NAME","SECOND_NAME" ,"ID" ,"PHONE" ,"DEBT","DATE"} ;
    int points[] = {26,26,17,16,16,15}; 
    
    char header[120] = {0} ;
    file = fopen(argv[1],"a+") ;
    if (!file)
    {
        return -1 ;
    }
    
    if (argc < 2)
    {
        puts("Error! usage : ./main_local <file_neme>") ;
        return -1 ;
    }

    
    orgenize_db(file,&db) ;
    printf("*****************************************WELCOM TO STORE DATABASE********************************************\n\n") ;
   
    puts("############################################################################################################") ;
    
    for (int i = 0 ,point = 0; i < FIELD_COUNT; i++)
    {
        strcpy(header + point,field_names[i]) ;
        memset(header + point + strlen(field_names[i]),' ', points[i] - strlen(field_names[i])) ;
        point += points[i] ;
    }
    strcat(header,"\n\n") ;
    strcpy(buffer,header) ;

    show_db(db,buffer,print_buf,NULL) ;
    print_buf(buffer,NULL) ;
    memset(buffer,0,BUFFER_SIZE) ;
    
    puts("--------------------------------------------Enter your comand------------------------------------------------") ;
    while (work)
    {
        char query_string[BUFFER_SIZE] = {0};
        Task task = {} ;
        printf("Please enter the comand :\n-->") ;
        
        fgets(query_string,300,stdin) ;
        task =  parse_query(query_string) ;
        switch (task.q_type)
        {
            case SELECT:
                strcpy(buffer,header) ;
                show_select(db,task.data.sp,buffer,print_buf,NULL) ;
                break;
            case SET:
                x = set_new(&db,task.data.new,0,buffer) ;
                if(x > -1)
                {
                    client *new = task.data.new ;
                    fprintf(file,"%s,%s,%d,%s,%f,%s\n",
                    new->first_name,new->second_name,new->id,
                    new->phone,new->dept_sum,new->last_date) ;
                    
                    strcpy(buffer,"comand completed succesfuly\n") ;
                }
                if (x == -1)
                {
                    free_one(task.data.new) ;
                }
                break;
            case PRINT:
                strcpy(buffer,header) ;
                show_db(db,buffer,print_buf,NULL) ;
                break;
            case ERROR :
                strcpy(buffer,task.data.error_str) ;
                break;
            case QUIT:
                strcpy(buffer,"thank you\n") ;
                work = 0 ;
                break;
            default:
                break;
        }
        print_buf(buffer,NULL) ;
        memset(buffer,0,BUFFER_SIZE) ;
    }
    fclose(file) ;
    free_db(db) ;
}
