#include <string.h> 

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