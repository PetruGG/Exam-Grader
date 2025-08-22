#include <sys/types.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <netinet/in.h>
#include <errno.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <libgen.h>
#include <limits.h>
#include <fcntl.h>
#include <time.h>
#include <sys/select.h>
#define PORT 2000
#define MAX_CONTENT 2048
#define MAX_PATH_LENGTH 1024
extern int errno;
int randomP;
int solutions_timeout_flag=1,break_loop_flag=0;
int contorclienti=0;
int secunde_rezolvare,secunde_conectare;
int grade;
char problema_selectata[MAX_CONTENT];
char* getFullPathUntilDir(const char* filePath,const char* targetDir)
{
    char currentDir[MAX_PATH_LENGTH];
    char fileName[MAX_PATH_LENGTH];
    char fileDir[MAX_PATH_LENGTH];
    if(getcwd(currentDir,MAX_PATH_LENGTH)==NULL)
    {
        perror("[server]Eroare la gasirea directorului curent de lucru.\n");
        return NULL;
    }
    snprintf(fileName,MAX_PATH_LENGTH,"%s/%s",currentDir,filePath);
    strncpy(fileDir,fileName,MAX_PATH_LENGTH);
    char* parentDir=dirname(fileDir);
    char* targetDirPos=strstr(parentDir,targetDir);
    if(targetDirPos!=NULL)
    {
        size_t length=targetDirPos-parentDir+strlen(targetDir);
        char* fullPath=malloc(length+1);
        if(fullPath==NULL)
        {
            perror("[server]Eroare la malloc.\n");
            return NULL;
        }
        strncpy(fullPath,fileName,length);
        fullPath[length]='\0';
        return fullPath;
    }
    else
    {
        printf("[server]Eroare: directorul target %s nu a fost gasit.\n",targetDir);
        return NULL;
    }
}
int submit(int submit_time);
int corectare(int submit_time);
int main()
{
    ssize_t size=sizeof(int);
    int fd=open("/dev/zero",O_RDWR);
    int *send_problem_flag=mmap(NULL,size,PROT_READ | PROT_WRITE,MAP_SHARED,fd,0);
    if(send_problem_flag==MAP_FAILED)
    {
        perror("[server]Eroare la crearea variabilei partajate.\n");
        return(errno);
    }
    *send_problem_flag=0;
    int shm;
    struct sockaddr_in server;
    struct sockaddr_in from;
    char msg[100];
    char maxCl[100],secundeC[100],secundeRezC[100];
    int sd,maxclienti;
    const Problema[100],Output[100],Input[100];
    const char* filePath="Proiect/Server/server.c";
    const char* targetDir="Server";
    const char* problemeDir="/Probleme";
    char* path_problema=getFullPathUntilDir(filePath,targetDir);
    char* path_output=getFullPathUntilDir(filePath,targetDir);
    char* path_input=getFullPathUntilDir(filePath,targetDir);
    char* newPathProblema=malloc(strlen(path_problema)+strlen(problemeDir)+100);
    char* newPathOutput=malloc(strlen(path_output)+strlen(problemeDir)+100);
    char* newPathInput=malloc(strlen(path_input)+strlen(problemeDir)+100);
    double submit_time;
    strcpy(newPathProblema,path_problema);
    strcat(newPathProblema,problemeDir);
    strcpy(newPathOutput,path_output);
    strcat(newPathOutput,problemeDir);
    strcpy(newPathInput,path_input);
    strcat(newPathInput,problemeDir);
    srand(time(NULL));
    randomP=rand()%3+1;
    sprintf(Problema,"/problema%d.txt",randomP);
    sprintf(Output,"/outputp%d.txt",randomP);
    sprintf(Input,"/inputp%d.txt",randomP);
    strcat(newPathProblema,Problema);
    strcat(newPathOutput,Output);
    strcat(newPathInput,Input);
    free(path_problema);
    free(path_output);
    free(path_input);
    printf("[server]A fost aleasa problema: %d\n",randomP);
    fflush(stdout);
    FILE* fconfig;
    char bufconfig[MAX_CONTENT];
    fconfig=fopen("config.txt","r");
    while(fgets(bufconfig,sizeof(bufconfig),fconfig)!=NULL)
    {
        if(strstr(bufconfig,"Numarul maxim de clienti este"))
        {
            int j=0;
            for(int i=0;i<strlen(bufconfig);i++)
                if(bufconfig[i]>='0' && bufconfig[i]<='9')
                {
                    maxCl[j]=bufconfig[i];
                    j++;
                }
            maxCl[j]='\0';
            maxclienti=atoi(maxCl);
        }
        else if(strstr(bufconfig,"Clientii au la dispozitie pentru a se conecta urmatorul numar de secunde"))
        {
            int j=0;
            for(int i=0;i<strlen(bufconfig);i++)
                if(bufconfig[i]>='0' && bufconfig[i]<='9')
                {
                    secundeC[j]=bufconfig[i];
                    j++;
                }
            secundeC[j]='\0';
            secunde_conectare=atoi(secundeC);
        }
        else if(strstr(bufconfig,"Clientii au la dispozitie pentru a rezolva cerintele urmatorul numar de secunde"))
        {
            int j=0;
            for(int i=0;i<strlen(bufconfig);i++)
                if(bufconfig[i]>='0' && bufconfig[i]<='9')
                {
                    secundeRezC[j]=bufconfig[i];
                    j++;
                }
            secundeRezC[j]='\0';
            secunde_rezolvare=atoi(secundeRezC);
        }
        bzero(bufconfig,sizeof(bufconfig));
    }
    fclose(fconfig);
    FILE *fproblema;
    fproblema=fopen(newPathProblema,"r");
    fread(problema_selectata,1,sizeof(problema_selectata),fproblema);
    fclose(fproblema);
    ssize_t bytes_read1,bytes_read2;
    char buffer[MAX_CONTENT];
    int fd_input_selectat,fd_output_selectat,fd_correct_output;
    fd_output_selectat=open(newPathOutput,O_RDONLY);
    fd_correct_output=open("correct_output.out",O_CREAT | O_RDWR,0777);
    while((bytes_read2=read(fd_output_selectat,buffer,MAX_CONTENT))>0)
    {
        write(fd_correct_output,buffer,bytes_read2);
    }
    close(fd_output_selectat);
    close(fd_correct_output);
    bzero(buffer,sizeof(buffer));
    switch(randomP){
        case 1:
            fd_input_selectat=open(newPathInput,O_RDONLY);
            int fd_recyclebin=open("recyclebin.in",O_CREAT | O_RDWR,0777);
            while((bytes_read1=read(fd_input_selectat,buffer,MAX_CONTENT))>0)
            {
                write(fd_recyclebin,buffer,bytes_read1);
            }
            close(fd_recyclebin);
            break;
        case 2:
            fd_input_selectat=open(newPathInput,O_RDONLY);
            int fd_rufe=open("rufe.in",O_CREAT | O_RDWR,0777);
            while((bytes_read1=read(fd_input_selectat,buffer,MAX_CONTENT))>0)
            {
                write(fd_rufe,buffer,bytes_read1);
            }
            close(fd_rufe);
            break;
        case 3:
            fd_input_selectat=open(newPathInput,O_RDONLY);
            int fd_tairos=open("tairos.in",O_CREAT | O_RDWR,0777);
            while((bytes_read1=read(fd_input_selectat,buffer,MAX_CONTENT))>0)
            {
                write(fd_tairos,buffer,bytes_read1);
            }
            close(fd_tairos);
            break;
    }
    if(fconfig==NULL)
    {
        perror("[server]Eroare la deschiderea fisierului de configurare.\n");
        return errno;
    }
    if((sd=socket(AF_INET,SOCK_STREAM,0))==-1)
    {
        perror("[server]Eroare la socket().\n");
        return errno;
    }
    bzero(&server,sizeof(server));
    bzero(&from,sizeof(from));
    server.sin_family=AF_INET;
    server.sin_addr.s_addr=htonl(INADDR_ANY);
    server.sin_port=htons(PORT);
    if(bind(sd,(struct sockaddr *)&server,sizeof(struct sockaddr))==-1)
    {
        perror("[server]Eroare la bind().\n");
        return errno;
    }
    if(listen(sd,1)==-1)
    {
        perror("[server]Eroare la listen().\n");
        return errno;
    }
    int connection_timeout_flag=0;
    fd_set read_fds;
    FD_ZERO(&read_fds);
    FD_SET(sd,&read_fds);
    struct timeval timeout;
    timeout.tv_sec=secunde_conectare;
    timeout.tv_usec=0;
    while(1)
    {
        int result=select(sd+1,&read_fds,NULL,NULL,&timeout);
        if(result==-1)
        {
            perror("[server]Eroare la select()");
            return(errno);
        }
        else if(result==0)
        {
            connection_timeout_flag=1;
            *send_problem_flag=1;
        }
        int client;
        int length=sizeof(from);
        int pid;
        char command[100];
        if(contorclienti<maxclienti && break_loop_flag==0 && connection_timeout_flag==0)
        {
            printf("[server]Asteptam la portul %d...\n",PORT);
            fflush(stdout);
            client=accept(sd,(struct sockaddr *)&from,&length);
            contorclienti+=1;
            if(contorclienti==maxclienti)
            {
                *send_problem_flag=1;
            }
        }
        else
        {
            break_loop_flag=1;
            int status;
            pid_t child_pid=waitpid(-1,&status,WNOHANG);
            if(child_pid>0)
            {
                contorclienti--;
                if(contorclienti==0)
                {
                    remove("correct_output.out");
                    switch(randomP){
                    case 1:
                        remove("recyclebin.in");
                        break;
                    case 2:
                        remove("rufe.in");
                        break;
                    case 3:
                        remove("tairos.in");
                        break;
                    }
                    if(munmap(send_problem_flag,size)==-1)
                    {
                        perror("[server]Eroare la munmap().\n");
                        break;
                    }
                    break;
                }
                else
                {
                    continue;
                }
            }
            else if(child_pid==0)
            {
                continue;
            }
            else
            {
                perror("[server]Eroare la waitpid().\n");
                continue;
            }
        }
        if(client<0)
        {
            perror("[server]Eroare la accept().\n");
            continue;
        }
        if((pid=fork())==-1)
        {
            close(client);
            continue;
        }
        else if(pid>0)
        {
            close(client);
            continue;
        }
        else if(pid==0)
        {
            while(*send_problem_flag==0)
            {
                ;
            }
            time_t start,start_submit,end_submit;
            time(&start);
            time(&start_submit);
            int flag_stop=0,flag_wait=0,flag_submit=0;
            close(sd);
            bzero(msg,100);
            char nr_ordine[100],mesaj_secunde_rezolvare[100];
            snprintf(nr_ordine,sizeof(nr_ordine),"%d",contorclienti);
            sprintf(mesaj_secunde_rezolvare,"Aveti la dispozitie %d secunde pentru rezolvare.",secunde_rezolvare);
            strcpy(msg,nr_ordine);
            if(write(client,msg,100)<=0)
            {
                perror("[server]Eroare la write catre client.\n");
                continue;
            }
            if(write(client,problema_selectata,sizeof(problema_selectata))<=0)
            {
                perror("[server]Eroare la write catre client.\n");
                continue;
            }
            if(write(client,mesaj_secunde_rezolvare,sizeof(mesaj_secunde_rezolvare))<=0)
            {
                perror("[server]Eroare la write catre client.\n");
                continue;
            }
            fcntl(client,F_SETFL,O_NONBLOCK);
            const char* filePath="Proiect/Server/server.c";
            const char* targetDir="Proiect";
            const clientDir[100];
            sprintf(clientDir,"/Client%d",contorclienti);
            char* fullPath=getFullPathUntilDir(filePath,targetDir);
            strcat(fullPath,clientDir);
            strcat(fullPath,"/rezolvare.cpp");
            int fd=open(fullPath,O_CREAT | O_RDWR,0777);
            close(fd);
            free(fullPath);
            while(1)
            {
                time_t end;
                time(&end);
                int elapsed=(int)difftime(end,start);
                bzero(msg,100);
                ssize_t bytesRead=read(client,msg,100);
                if(bytesRead<0)
                {
                    if(errno==EWOULDBLOCK || errno==EAGAIN)
                    {
                        if(elapsed>=secunde_rezolvare && flag_stop==0)
                        {
                            const char* filePath="Proiect/Server/server.c";
                            const char* targetDir="Proiect";
                            const clientDir[100];
                            sprintf(clientDir,"/Client%d",contorclienti);
                            char* fullPath=getFullPathUntilDir(filePath,targetDir);
                            strcat(fullPath,clientDir);
                            strcat(fullPath,"/rezolvare.cpp");
                            chmod(fullPath,0555);
                            free(fullPath);
                            if(flag_submit==0)
                            {
                                grade=submit(secunde_rezolvare);
                                char grade_c[100];
                                sprintf(grade_c,"%d",grade);
                                if(write(client,grade_c,100)<=0)
                                {
                                    perror("[server]Eroare la write catre client.\n");
                                    continue;
                                }
                            }
                            flag_stop=1;
                        }
                        else if(flag_wait==0)
                        {
                            printf("[server]Asteptam o comanda...\n");
                            fflush(stdout);
                            flag_wait=1;
                        }
                    }
                    else
                    {
                        perror("[server]Eroare la read() de la client.\n");
                        break;
                    }
                }
                else if(bytesRead>0)
                {
                    flag_wait=0;
                    printf("[server]Comanda a fost receptionata...%s\n",msg);
                    if(strstr(msg,"submit"))
                    {
                        time(&end_submit);
                        submit_time=(int)difftime(end_submit,start_submit);
                        flag_submit=1;
                        const char* filePath="Proiect/Server/server.c";
                        const char* targetDir="Proiect";
                        const clientDir[100];
                        sprintf(clientDir,"/Client%d",contorclienti);
                        char* fullPath=getFullPathUntilDir(filePath,targetDir);
                        strcat(fullPath,clientDir);
                        strcat(fullPath,"/rezolvare.cpp");
                        chmod(fullPath,0555);
                        free(fullPath);
                        grade=submit(submit_time);
                        char grade_c[100];
                        sprintf(grade_c,"%d",grade);
                        if(write(client,grade_c,100)<=0)
                        {
                            perror("[server]Eroare la write catre client.\n");
                            continue;
                        }
                    }
                    else if(strstr(msg,"exit"))
                    {
                        break;
                    }
                }
            }
            close(client);
            exit(0);
        }
    }
return 0;
}
int submit(int submit_time)
{
    ssize_t bytes_read;
    char buffer[MAX_CONTENT],solution[100];
    const char* filePath="Proiect/Server/server.c";
    const char* targetDir="Proiect";
    const clientDir[100];
    int nota;
    sprintf(clientDir,"/Client%d",contorclienti);
    char* fullPath=getFullPathUntilDir(filePath,targetDir);
    strcat(fullPath,clientDir);
    strcat(fullPath,"/rezolvare.cpp");
    sprintf(solution,"solutie_client%d.cpp",contorclienti);
    int fd_destination=open(solution,O_CREAT | O_RDWR,0777);
    int fd_source=open(fullPath,O_RDONLY);
    while((bytes_read=read(fd_source,buffer,MAX_CONTENT))>0)
    {
        write(fd_destination,buffer,bytes_read);
    }
    close(fd_source);
    close(fd_destination);
    nota=corectare(submit_time);
    return nota;
}
int corectare(int submit_time)
{
    FILE *fd_out_corect,*fd_out_client;
    clock_t start_execution,end_execution;
    double execution_time,one_fourth,two_fourths,three_fourths,four_fourths;
    char solution[100],compiled[100],compilation_command[100],execution_command[100],nume_out[100];
    char buffer1[100],buffer2[100];
    int nota,ok=1,partial_ok=0;
    sprintf(solution,"solutie_client%d.cpp",contorclienti);
    sprintf(compiled,"solutie_client%d",contorclienti);
    sprintf(compilation_command,"g++ -Wall %s -o %s",solution,compiled);
    sprintf(execution_command,"./%s",compiled);
    one_fourth=secunde_rezolvare/4;
    two_fourths=2*secunde_rezolvare/4;
    three_fourths=3*secunde_rezolvare/4;
    four_fourths=4*secunde_rezolvare;
    int compilation_result=system(compilation_command);
    if(compilation_result==0)
    {
        start_execution=clock();
        int execution_result=system(execution_command);
        end_execution=clock();
        execution_time=(((double)(end_execution-start_execution))/CLOCKS_PER_SEC)*10000.0;
        if(execution_result==0)
        {
            fd_out_corect=fopen("correct_output.out","r");
            switch(randomP){
                case 1:
                    sprintf(nume_out,"recyclebin%d.out",contorclienti);
                    fd_out_client=fopen(nume_out,"r");
                    break;
                case 2:
                    sprintf(nume_out,"rufe%d.out",contorclienti);
                    fd_out_client=fopen(nume_out,"r");
                    break;
                case 3:
                    sprintf(nume_out,"tairos%d.out",contorclienti);
                    fd_out_client=fopen(nume_out,"r");
                    break;
            }
            while(fgets(buffer1,sizeof(buffer1),fd_out_corect)!=NULL && fgets(buffer2,sizeof(buffer2),fd_out_client)!=NULL)
            {
                if(strcmp(buffer1,buffer2)==0)
                {
                    partial_ok=1;
                }
                else if(strcmp(buffer1,buffer2)!=0)
                {
                    ok=0;
                    break;
                }
            }
            if(ok==1)
            {
                if(execution_time<=0.5 && submit_time<=one_fourth)
                    nota=10;
                else if(execution_time<=1.0 && (submit_time<=one_fourth || submit_time<=two_fourths))
                    nota=9;
                else if(execution_time<=2.0 && (submit_time<=one_fourth || submit_time<=two_fourths || submit_time<=three_fourths))
                    nota=8;
                else if(execution_time<=3.0 && (submit_time<=one_fourth || submit_time<=two_fourths || submit_time<=three_fourths || submit_time<=four_fourths))
                    nota=7;
                else
                    nota=6;
            }
            else if(partial_ok==1)
            {
                nota=5;
            }
            else
            {
                nota=4;
            }
        }
        else
        {
            char run_error[100];
            sprintf(run_error,"[server]Execution failed in client %d\n",contorclienti);
            perror(run_error);
        }
    }
    else
    {
        nota=3;
    }
    fclose(fd_out_corect);
    fclose(fd_out_client);
    remove(solution);
    remove(compiled);
    remove(nume_out);
    return nota;
}