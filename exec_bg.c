#include"shell.h"
void exec_bg(char **args)
{
    int len = len_args(args);
    if(len<2)
    {
        dprintf(stdout_fd,"Usage: kjob <job_number> <signal>\n");
        return;
    }
    int jnumber = atoi(args[1]);
    int jslot = numberslot(jnumber);
    if(jslot==-1)
    {
        dprintf(stdout_fd,"Job doesn't exist\n");
        return;
    }
    if(kill(jobs[jslot]->pid,SIGCONT)==-1)
    {
        perror("Couldn't send signal");
        return ;
    }
}