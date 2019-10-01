#include"shell.h"
void exec_jobs(char **args)
{
    for(int i=0;i<MAX_JOBS;i++)
    {
        if(jobs[i])
        {
            int pid = jobs[i]->pid;
            char str_pid[12];
            sprintf(str_pid,"%d",pid);

            char pid_path[MAX_PATH+1];
            strcpy(pid_path,"/proc/");
            strcat(pid_path,str_pid);
            strcat(pid_path,"/status");
            FILE * fp = fopen(pid_path,"r");
            if(fp == NULL)
            {
                free(jobs[i]);
                jobs[i]=NULL;
            }
            size_t buff_size = 1024;
            char *buffer = (char *)malloc(buff_size * sizeof(char));
            size_t chars_read;
            while(!feof(fp)&&getline(&buffer,&buff_size,fp)>0)
            {
                if(!strncmp(buffer,"State",5))
                    break;
            }
            fclose(fp);
            int blen = strlen(buffer);
            buffer[--blen]=0;
            printf("[%d] %s %s %d\n",jobs[i]->job_number,buffer+7,jobs[i]->name,jobs[i]->pid);
            free(buffer);
        }
    }
}