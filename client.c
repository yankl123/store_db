#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define MAX_LEN 1024

int main(int argc, char **argv)
{
    int sockfd[5];
    struct sockaddr_in servaddr;
    char buffer[5][MAX_LEN];
    int i , j , n ,r ;
    
    if (argc < 3)
    {
        printf("Usage: %s <ip> <port>\n", argv[0]);
        return 1;
    }

    /* Connect to the server */
    memset(&servaddr, 0, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = inet_addr(argv[1]);
    servaddr.sin_port = htons(atoi(argv[2]));

     /* Send data to the server */
    puts("*****************************************WELCOM TO STORE DATABASE********************************************\n") ;
   
    puts("#############################################################################################################") ;
    puts("Enter up to 5 requests (tap enter for next query)\n.");
    puts("To finish with less than five requests enter \"send\":") ;
    
    while (1)
    {
        /* Create a sockets */ 
        puts("enter a query\n-->") ;
        for (i = 0; i < 5; i++)
        {
            char word[5] = {0};
            fgets(buffer[i], MAX_LEN, stdin);
            if (buffer[i][0] == '\n')
            {
                i-- ;
                continue;
            }
            
            sscanf(buffer[i],"%s5",word) ;
            *(int*)word |= 0x20202020 ;
            if(!strcmp(word,"send")) 
            {
                break;
            }
            if (!strcmp(word,"quit"))
            { 
               puts("thanke you") ;
               return 0 ;
            }
            
        }

        for (j = 0; j < i; j++)
        {
            sockfd[j] = socket(AF_INET, SOCK_STREAM, 0);
            if (sockfd[j] < 0)
            {
                perror("Error creating socket");
                return 1;
            }
            if (connect(sockfd[j], (struct sockaddr *) &servaddr, sizeof(servaddr)) < 0)
            {
                perror("Error connecting");
                return 1;
            }
        }


        for (j = 0; j < i; j++)
        {
            n = send(sockfd[j], buffer[j], strlen(buffer[j]) - 1, 0);
            shutdown(sockfd[j],SHUT_WR) ;
            if (n < 0)
            {
                perror("Client error sending data");
                return 1;
            }
        }

        for (j = 0; j < i; j++)
        {  
            r = 0 ;
            while (1)
            {
                n = recv(sockfd[j], buffer[j], MAX_LEN , 0);
                if (n < 0)
                {
                    perror("Client error receiving data");
                    return 1;
                }   
                if (n > 0)
                {
                    r += n ;
                    buffer[j][n] = '\0';
                   (r - n) ? printf("%s", buffer[j]) : printf("respomse for query %d :\n%s",(j + 1), buffer[j]) ;
                }
                else
                {
                    break;
                }  
            } 
            
            close(sockfd[j]);
        }
    }

    return 0;
}
