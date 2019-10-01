#include"shell.h"

void exec_pinfo(char **args)
{
    int pid;
    if(len_args(args)==1)
    {
        pid = getpid();
    }
    else
    {
        pid = atoi(args[1]);
        if(pid == 0)
            pid = getpid();
    }
    printf("PID: %d\n",pid);
    
    char str_pid[12];
    sprintf(str_pid,"%d",pid);

    char pid_path[MAX_PATH+1];
    strcpy(pid_path,"/proc/");
    strcat(pid_path,str_pid);
    strcat(pid_path,"/status");
    FILE * fp = fopen(pid_path,"r");
    if(fp == NULL)
    {
        perror(pid_path);
        return ;
    }
	size_t buff_size = 1024;
	char *buffer = (char *)malloc(buff_size * sizeof(char));
	size_t chars_read;
	while(!feof(fp)&&getline(&buffer,&buff_size,fp)>0)
    {
        if(!strncmp(buffer,"State",5)||!strncmp(buffer,"VmSize",6))
            printf("%s",buffer);
    }
    char exe_path[MAX_PATH+1];
    strcpy(pid_path,"/proc/");
    strcat(pid_path,str_pid);
    strcat(pid_path,"/exe");
    printf("%s ",pid_path);
    int nbytes = readlink(pid_path,exe_path,MAX_PATH);
    if(nbytes == -1)
        perror("Couldn't get the executable path");
    else
    {
        exe_path[nbytes] = 0;
        printf("Executable path:  %s\n",exe_path);
    }
    free(buffer);
    fclose(fp);

}