/*
 * The following is part of the ICS231 Operating Systems
 * Course at IIIT-Hyderabad
 * It is part of the operating systems project.
 * Author : Rajesh Vakkalagadda
 * Roll No: 201001064
 *
 *
 * Some Assumptions Taken:
 * 	  other commands  : hist , !hist , hist+number (like hist1 , hist2) , pid , pidall , quit
 *
 */



#include<stdio.h>
#include<string.h>
#include<sys/types.h>
#include<sys/wait.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<stdlib.h>
#include<unistd.h>
#include<signal.h>

char *value,cwd[100],host[100],hist[100][100],home[100],pidname[100][100];
int histlen,pidnum[100],pidlen=0,flag[100],fore[100];
int running_foreground=0;
char hst[100];
int getnumber(char a[100])
{
    int i,l=0;
    char b[10];
    for(i=0;i<strlen(a);i++) { if(a[i]=='h'&&a[i+1]=='i'&&a[i+3]=='t')  { i=i+4; break; } }
    while(a[i]>='0'&&a[i]<='9') b[l++]=a[i++];
    b[l]='\0';
    return atoi(b);
}
void handler(int sig)
{
    int status;
    int pid=wait(&status);
    int i=0;
    while (pidnum[i]!=pid) i++;
    flag[i]=0;
    if (fore[i]==0) printf("\n%s : Pid %d exited with status %d.\n", pidname[i],pid,status);
    else running_foreground--;
    signal(SIGCHLD,handler);
}
void pidall() { int i; for(i=0;i<pidlen;i++) { printf("%d: %s %d\n",i+1,pidname[i],pidnum[i]);} }
void pidcurrent() { int i; for(i=0;i<pidlen;i++) { if(flag[i]==1) { printf("%s %d\n",pidname[i],pidnum[i]); }  } }
void history(char command[100])
{
    int num,i,j,k;
    if(strstr(command,"hist")!=NULL && strlen(command)==4) {
        for(i=0;i<histlen-1;i++)
            printf("%d %s\n",i+1,hist[i]);
    }
    else if(strstr(command,"!")==NULL) {
        num=getnumber(command);
        k=histlen-num-1;
        if(k < 0) k=0;
        else if(num > histlen)  k=0;
        for(i=k;i<histlen-1;i++) printf("%d %s\n",i-(k-1),hist[i]);
    }
    else {
        histlen--;
        num=getnumber(command);
        char com[100];
        if(num > histlen) num=histlen;
        strcpy(com,hist[num-1]);
        strcpy(hst,com);
    }
}
void cd(char command[100])
{
    char tempstr[100],*test,*str[100];
    int stlen=0;
    strcpy(tempstr,command);
    test=strtok(tempstr," \t");
    while(test!=NULL) { str[stlen++]=test; test=strtok(NULL," \t"); }
    str[stlen]=test;
    int i,j,k;
    char t[100];
    if(str[1]==NULL) str[1]=home;
    int r=chdir(str[1]);
    if(r!=0) printf("path not found or the file has no suitable permisiions\n");
    getcwd(cwd,100);
    if(strstr(cwd,home)!=NULL) {
        k=0;
        t[k++]='~';
        i=strlen(home);
        for(j=i;j<strlen(cwd);j++) t[k++]=cwd[j];
        t[k]='\0';
        strcpy(cwd,t);
    }
}

void exvp(char **str)
{
    execvp(str[0],str);
    printf("%s is wrong or it has occured an error\n",str[0]);
    exit(EXIT_FAILURE);
}

void pidpro(){   printf("Command name: a.out process id %d\n",getpid()); }
void lessmore(char *command)
{
    int i,j,k,len=0,f=0;
    char *test,*a[100];
    if(strstr(command,">") && strstr(command,"<")) f=2;
    else if(strstr(command,">")!=NULL) f=1;
    test=strtok(command,"<>");
    while(test!=NULL) { a[len++]=test; test=strtok(NULL,"<>"); }
    a[len]=test;
    char *b[100],l=0;
    test=strtok(a[0]," \t");
    while(test!=NULL)
    {
        b[l++]=test;
        test=strtok(NULL," \t");
    }
    b[l]=test;
    if(f==0||f==2) { a[1]=strtok(a[1]," "); FILE *in; in=freopen(a[1],"r",stdin); }
    if(f==1) { a[1]=strtok(a[1]," "); FILE *out; out=freopen(a[1],"w",stdout); }
    else if(f==2) { a[2]=strtok(a[2]," "); FILE *out; out=freopen(a[2],"w",stdout); }
    if(strstr(b[0],"hist")!=NULL) history(b[0]);
    else if(strstr(b[0],"pid")!=NULL) {
        if(l==1) pidpro();
        else if(l==2 && strstr(b[1],"all")!=NULL) pidall();
        else pidcurrent();
    }
    else exvp(b);
    exit(0);
}
void check(char command[100])
{
    int i,j,k;
    char t[100];
    int stlen=0;
    char *test,tempstr[100],*str[100];
    strcpy(t,command);
    for(i=0;i<strlen(command);i++) {
        if(command[i]=='~') {
            k=i+1;
            j=0;
            while(home[j]!='\0')
                command[i++]=home[j++];
            for(j=k;j<=strlen(command);j++)
                command[i++]=t[j];
            i=k-1;
        }
    }
    strcpy(tempstr,command);
    test=strtok(tempstr," \t");
    while(test!=NULL) { str[stlen++]=test; test=strtok(NULL," \t"); }
    str[stlen]=test;
    if(strstr(command,">")==NULL && strstr(command,"<")==NULL && strstr(command,"|")==NULL) {
        if(strstr(command,"hist")!=NULL)  history(str[0]);
        else if(strstr(command,"pid")==NULL&&strstr(command,"&")==NULL)
            exvp(str);
        else if(strstr(command,"&")!=NULL){
            str[stlen-1] = NULL;
            exvp(str);
        }
        else if(strstr(str[0],"pid")!=NULL){
            if(stlen==1) pidpro();
            else if(stlen==2 && strcmp(str[1],"all")==0) pidall();
            else pidcurrent();
        }
    }
    else if((strstr(command,"<")!=NULL || strstr(command,">")!=NULL) && strstr(command,"|")==NULL)  lessmore(command);
}

int maint()
{
    signal(SIGCHLD, handler);
    signal(SIGQUIT, SIG_IGN);
    signal(SIGABRT, SIG_IGN);
    signal(SIGINT, SIG_IGN);
    signal(SIGTSTP, SIG_IGN);
    histlen=0;
    pidlen=0;
    int u=0;
    getcwd(home,100);
    strcpy(cwd,"~");
    gethostname(host,100);
    value=getenv("USER");
    while(1)
    {
        int i,j;
        char command[100];
        printf("<%s@%s:%s> ",value,host,cwd);
        command[0]='\0';
        i=scanf("%[^\n]",command);
        getchar();
        if (i!=1) continue;
    x:		strcpy(hist[histlen++],command);
        int foreground=(strstr(command,"&")==NULL?1:0);
        if (strstr(command,"quit")!=NULL) exit(1);
        else if (strstr(command,"cd")!=NULL) cd(command);
        else if (strstr(command,"!hist")!=NULL) { history(command); strcpy(command,hst); goto x;}
        else{
            char temp[100],*arr[100];
            int alen=0;
            strcpy(temp,command);
            char *tok;
            tok=strtok(temp,"|");
            while(tok!=NULL)
            {
                arr[alen] = (char*)malloc(1000*sizeof(char));
                strcpy(arr[alen++],tok);
                tok=strtok(NULL,"|");
            }
            arr[alen]=NULL;
            if (foreground) running_foreground=alen;
            else running_foreground=0;
            int **pipefd;
            if (alen>1) pipefd=(int **)malloc(sizeof(int *)*alen-1);
            for (i=0;i<alen-1;i++) { pipefd[i]=(int *)malloc(sizeof(int)*2); pipe(pipefd[i]); }
            for(i=0;i<alen;i++) {
                int pid=fork();
                if(pid==0) {
                    for (j=0; j<alen-1; j++) {
                        if (j!=i) close(pipefd[j][1]);
                        else dup2(pipefd[j][1],STDOUT_FILENO);
                        if (j!=i-1) close(pipefd[j][0]);
                        else dup2(pipefd[j][0],STDIN_FILENO);
                    }
                    check(arr[i]);
                    exit(EXIT_SUCCESS);
                }
                else {
                    arr[i]=strtok(arr[i]," \t");
                    strcpy(pidname[pidlen],arr[i]);
                    flag[pidlen]=1;
                    if (foreground) fore[pidlen]=1;
                    pidnum[pidlen++]=pid;
                }
            }
            for(i=0;i<alen-1;i++) { close(pipefd[i][0]); close(pipefd[i][1]); free(pipefd[i]); }
            if (alen>1) free(pipefd);
            while(running_foreground) pause();
        }
    }
    return 0;
}
