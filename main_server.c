#include "store_db.h" 
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <pthread.h>

typedef struct Con_props
{
    int sock ;
    client **db ;
    FILE *file ;
}con_props;

typedef struct Target_props
{
    int sock ;
    int *status ;
    
}target_props;

void send_buf(char *buf ,void *props)
{
    target_props *tarps = (target_props*)props ;
    if (*(tarps->status) < 1) 
    {
        return ;
    }
    *(tarps->status) = send(tarps->sock, buf, strlen(buf), 0);
}

void *conn_handler(void *args)
{
    char query_string[BUFER_SIZE] = {0};
    char buffer[BUFER_SIZE] = {0};
    char header[120] = {0} ;
    char *fild_nams[6] ={"FIRST_NAME","SECOND_NAME" ,"ID" ,"PHONE" ,"DEBT","DATE" } ;
    con_props *conps = (con_props*)args ;
    int n ,r,x ,send_status;
    r = 0 ;
    after_pars ap = {} ;
    sprintf(header,"%-25s %-25s %-15s  %-15s %-15s %s\n\n" ,fild_nams[0],fild_nams[1],fild_nams[2],fild_nams[3],fild_nams[4],fild_nams[5]) ;
    target_props tarps ={
        .sock = conps->sock ,
        .status = &send_status
        } ;

    send_status = 1;
    client **db = conps->db ;
    int new_sock = conps->sock ;
    
    while (1)
    {
        n = recv(new_sock, query_string + r, BUFER_SIZE - r, 0);
        if (n < 0)
        {
            perror("Server error receiving data");
            goto exit;
        }
        if (n < 1)
        {
            break;
        }
        r+=n ;
    } 
    printf("Server received\n: %s\n", query_string);

    ap =  parse_query(query_string) ;
    switch (ap.q_type)
        {
            case SELECT:
                strcpy(buffer,header) ;
                show_select(*db,ap.sp,buffer,send_buf,&tarps) ;
                if (send_status < 0)
                {
                    perror("Server error sending data");
                    goto exit;
                }
                break;
            case SET:
                x = set_new(db,ap.new,0,buffer) ;
                if(x > -1)
                {
                    fprintf(conps->file,"%s,%s,%d,%s,%f,%s\n",ap.new->first_name,ap.new->second_name,ap.new->id,
                    ap.new->phone,ap.new->dept_sum,ap.new->last_date) ;
                    strcpy(buffer,"comand completed succesfuly\n") ;
                }
                if (x == -1)
                {
                    free_one(ap.new) ;
                }
                break;
            case PRINT:
                strcpy(buffer,header) ;
                show_db(*db,buffer,send_buf,&tarps) ;
                if (send_status < 0)
                {
                    perror("Server error sending data");
                    goto exit;
                }
                break;
            case ERROR :
                strcpy(buffer,ap.error_str) ;
                break;
            default:
                break;
        }
        send_buf(buffer,&tarps) ;

    if (send_status < 0)
    {
        perror("Server error sending data");
        goto exit;
    }
exit:
    close(new_sock);
    return NULL;
}


int main(int argc, char **argv)
{
    int sockfd;
    struct sockaddr_in servaddr, cliaddr;
    socklen_t len = sizeof(cliaddr);
    FILE *file ;
    client *db = NULL ;
    if (argc < 3)
    {
        printf("Usage: %s <CSV file> <port>\n", argv[0]);
        return 1;
    }
    file = fopen(argv[1],"a+") ;
    orgenize_db(file,&db) ;
    /* Create a socket */
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0)
    {
        perror("Error creating socket");
        return 1;
    }

    /* Bind the socket to a specific port */
    memset(&servaddr, 0, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = htons(atoi(argv[2]));
    if (bind(sockfd, (struct sockaddr*) &servaddr, sizeof(servaddr)) < 0)
    {
        perror("Error binding socket");
        return 1;
    }

    if (listen(sockfd, 5) < 0)
    {
        perror("Error listenning");
        return 1;
    }

    /* Receive data from clients */
    while (1)
    {
        con_props conps = {} ;
        pthread_t tid;
        int new_sock = accept(sockfd, (struct sockaddr *)&cliaddr, (socklen_t*)&len);
        
        if (new_sock < 0)
        {
            perror("accept failed");
            return 1;
        }
        conps.db = &db ;
        conps.file = file ;
        conps.sock = new_sock ;
        pthread_create(&tid, NULL, conn_handler, &conps);
        pthread_join(tid, NULL);
    }

    return 0;
}

