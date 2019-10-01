#include"shell.h"

void exit_child(int signum)
{
    pid_t wpid;
    int status;
    wpid = waitpid(-1,&status,WNOHANG);
    if(wpid == -1)
    {
        perror("wpid == -1");
        return;
    }
    else if(wpid == 0)
    {
        //fprintf(stderr,"Child not dead yet... KILL HIM!!!!\n");
        //fprintf(stdout,"Child not dead yet... KILL HIM!!!!\n");
        return;
    }
    if(WIFEXITED(status)==0)
    {
        int jobslot = pidslot(wpid);
        dprintf(stdout_fd,"%d\n",jobslot);
        fprintf(stderr,"\rChild with PID %d [%d] exited normally\n",wpid,jobs[jobslot]->job_number);
        dprintf(stdout_fd,"\rChild with PID %d [%d] exited normally\n",wpid,jobs[jobslot]->job_number);
        if(jobs[jobslot]->job_number==job_number-1)
        {
            job_number--;
        }
        free(jobs[jobslot]->name);
        free(jobs[jobslot]);
        jobs[jobslot]=NULL;
    }
    if(WIFSIGNALED(status)==0)
    {
        int jobslot = pidslot(wpid);
        dprintf(stdout_fd,"%d\n",jobslot);
        fprintf(stderr,"\rChild with PID %d [%d] exited due to signal\n",wpid,jobs[jobslot]->job_number);
        dprintf(stdout_fd,"\rChild with PID %d [%d] exited due to signal\n",wpid,jobs[jobslot]->job_number);
        if(jobs[jobslot]->job_number==job_number-1)
        {
            job_number--;
        }
        free(jobs[jobslot]->name);
        free(jobs[jobslot]);
        jobs[jobslot]=NULL;
    }
}

void sigint_handler(int signum)
{
    fflush(stdout);
    signal(SIGINT,sigint_handler);
}
void sigcont_handler(int signum)
{
    signal(SIGCONT,sigint_handler);
}

void sigtstp_handler(int signum)
{
    fflush(stdout);
    fflush(stdin);
    signal(SIGTSTP,sigtstp_handler);
    fflush(stdout);
    fflush(stdin);
}

