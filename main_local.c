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
    file = fopen(argv[1],"a+") ;
    client *db = NULL;
    int work = 1 ;
    orgenize_db(file,&db) ;
    char bufer[1024] = {0} ;
    int x ;
    char *fild_nams[6] ={"FIRST_NAME","SECOND_NAME" ,"ID" ,"PHONE" ,"DATE" ,"DEBT"} ;
    char header[120] = {0} ;
    printf("*****************************************WELCOM TO STORE DATABASE********************************************\n\n") ;
   
    puts("#############################################################################################################") ;
    
    sprintf(header,"%-25s %-25s %-15s %-15s  %-15s %s\n\n" ,fild_nams[0],fild_nams[1],fild_nams[2],fild_nams[3],fild_nams[4],fild_nams[5]) ;
    strcpy(bufer,header) ;
    show_db(db,bufer,print_buf,NULL) ;
    print_buf(bufer,NULL) ;
    memset(bufer,0,BUFER_SIZE) ;
    
    puts("--------------------------------------------Enter your comand------------------------------------------------") ;
    while (work)
    {
        char query_string[BUFER_SIZE] = {0};
        after_pars ap = {} ;
        printf("Please enter the comand :\n-->") ;
        
        fgets(query_string,300,stdin) ;
        ap =  parse_query(query_string) ;
        switch (ap.q_type)
        {
            case SELECT:
                strcpy(bufer,header) ;
                show_select(db,ap.sp,bufer,print_buf,NULL) ;
                break;
            case SET:
                x = set_new(&db,ap.new,0,bufer) ;
                if(x > -1)
                {
                    fprintf(file,"%s,%s,%d,%s,%f,%s\n",ap.new->first_name,ap.new->second_name,ap.new->id,
                    ap.new->phone,ap.new->dept_sum,ap.new->last_date) ;
                    strcpy(bufer,"comand completed succesfuly\n") ;
                }
                if (x == -1)
                {
                    free_one(ap.new) ;
                }
                break;
            case PRINT:
                strcpy(bufer,header) ;
                show_db(db,bufer,print_buf,NULL) ;
                break;
            case ERROR :
                strcpy(bufer,ap.error_str) ;
                break;
            case QUIT:
                strcpy(bufer,"thank you\n") ;
                work = 0 ;
                break;
            default:
                break;
        }
        print_buf(bufer,NULL) ;
        memset(bufer,0,BUFER_SIZE) ;
    }
    fclose(file) ;
    free_db(db) ;
}