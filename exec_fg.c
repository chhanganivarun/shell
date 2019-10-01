#include"shell.h"
void exec_fg(char **args)
{
    int len = len_args(args);
    if(len<2)
    {
        dprintf(stdout_fd,"Usage: fjob <job_number> <signal>\n");
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
    tcsetpgrp(tcgetpgrp(stdin_fd),jobs[jslot]->pid);
    //setpgid(jobs[jslot]->pid,getpgid(getpid()));
    // setpgid(jobs[jslot]->pid,getpgid(getpid()));
    signal(SIGTTOU,SIG_IGN);
    signal(SIGTTIN,SIG_IGN);
    int status;
    waitpid(jobs[jslot]->pid,&status,WUNTRACED);
    //tcsetpgrp(stdin_fd,jobs[jslot]->pid);
    //signal(SIGCHLD,exit_child);
}