#include"shell.h"
void exec_overkill(char **args)
{
    for(int i=0;i<MAX_JOBS;i++)
    {
        if(jobs[i])
        {
            int pid = jobs[i]->pid;
            printf("Killed [%d] with sigterm (output %d) %s %d\n",jobs[i]->job_number,kill(pid,SIGINT),jobs[i]->name,jobs[i]->pid);
            free(jobs[i]);
            jobs[i]=NULL;
        }
    }
    setpgid(0,getpgid(getpid()));

}