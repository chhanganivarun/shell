#include"shell.h"
void exec_kjob(char **args)
{
    int len = len_args(args);
    if(len<3)
    {
        dprintf(stdout_fd,"Usage: fjob <job_number> <signal>\n");
        return;
    }
    int jnumber = atoi(args[1]);
    int jsig = atoi(args[2]);
    int jslot = numberslot(jnumber);
    if(jslot==-1)
    {
        dprintf(stdout_fd,"Job doesn't exist\n");
        return;
    }
    if(kill(jobs[jslot]->pid,jsig)==-1)
    {
        perror("Couldn't send signal");
    }
}