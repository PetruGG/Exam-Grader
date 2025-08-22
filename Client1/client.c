#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <errno.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <netdb.h>
#include <string.h>
#include <arpa/inet.h>
#define MAX_CONTENT 2048
extern int errno;
int port;
int main(char argc,char* argv[])
{
    int sd,nr_ordine;
    struct sockaddr_in server;
    char msg[100],command[100],problema_selectata[MAX_CONTENT];
    if(argc!=3)
    {
        printf("Sintaxa: %s <adresa_server> <port>\n",argv[0]);
        return -1;
    }
    port=atoi(argv[2]);
    if((sd=socket(AF_INET,SOCK_STREAM,0))==-1)
    {
        perror("[client]Eroare la socket().\n");
        return errno;
    }
    server.sin_family=AF_INET;
    server.sin_addr.s_addr=inet_addr(argv[1]);
    server.sin_port=htons(port);
    if(connect(sd,(struct sockaddr *)&server,sizeof(struct sockaddr))==-1)
    {
        perror("[client]Eroare la connect().\n");
        return errno;
    }
    bzero(msg,100);
    read(sd,msg,100);
    nr_ordine=atoi(msg);
    printf("[client]Sunteti clientul cu numarul: %d\n",nr_ordine);
    fflush(stdout);
    bzero(msg,100);
    read(sd,problema_selectata,sizeof(problema_selectata));
    printf("%s\n",problema_selectata);
    fflush(stdout);
    read(sd,msg,100);
    printf("%s\n",msg);
    fflush(stdout);
    bzero(msg,100);
    bzero(command,100);
    printf("Lista de comenzi:\n");
    printf("-submit(trimite rezolvarea catre server)\n");
    printf("-exit(inchide aplicatia)\n");
    printf("Comanda submit se va apela automat dupa expirarea timpului alocat rezolvarii!\n");
    printf("Folositi comanda exit doar dupa ce primiti rezultatele!\n");
    printf("Atentie! Cand denumiti fisierul de output in rezolvarea dvs. veti folosi urmatoarea sintaxa: numele fisierului de output din datele problemei concatenat cu numarul dvs. de ordine.Fisierul de input nu trebuie redenumit.\n");
    fflush(stdout);
    while(1)
    {
        bzero(msg,100);
        bzero(command,100);
        printf("[client]Introduceti o comanda: ");
        fflush(stdout);
        read(0,command,100);
        if(write(sd,command,100)<=0)
        {
            perror("[client]Eroare la write() spre server.\n");
            return errno;
        }
        if(strstr(command,"exit"))
        {
            break;
        }
        else if(strstr(command,"submit"))
        {
            if(read(sd,msg,100)<0)
            {
                perror("[client]Eroare la read() de la server.\n");
                return errno;
            }
            printf("[client]Nota primita este: %s\n",msg);
        }
    }
    close(sd);
}