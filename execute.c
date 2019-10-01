#include"shell.h"
int min(int a,int b)
{
    return a<b?a:b;
}
void execute(char **args)
{
    
    pid_t pid;
    int is_bg=0;
    for(int i=0;args[i];i++)
    {
        if(strcmp(args[i],"&")==0)
        {
            is_bg = 1;
            for(int j=i;args[j];j++)
            {
                args[j]=args[j+1];
            }
        }
    }
    int in_redir=argcmp(args,"<"),out_redir=argcmp(args,">"),app_redir=argcmp(args,">>");
    int min_redir = len_args(args);
    if(in_redir!=-1)
        min_redir = min(min_redir,in_redir);
    if(out_redir!=-1)
        min_redir = min(min_redir,out_redir);
    if(app_redir!=-1)
        min_redir = min(min_redir,app_redir);
    for(int i=min_redir;args[i];i++)
        args[i]=NULL;

    int job_loc = findjobslot();
    if(job_loc<-1)
    {
        dprintf(stdout_fd,"Can't execute more than 1024 child processes at a time\n");
        return;
    }

    pid = fork();
    if(pid == 0)//child
    {
        if(is_bg)
        {
            setpgid(0,0);
        }
        int ret_code = execvp(args[0],args);
        if(ret_code==-1)
        {
            perror("Failed to execute child process");
            return ;
        }
    }
    else if(pid<0)
    {
        perror("Can't fork");
    }
    else //parent
    {
        jobs[job_loc] = (job_struct*) malloc(sizeof(job_struct));
        jobs[job_loc]->pid = pid;
        jobs[job_loc]->name = (char *)malloc(MAX_NAME*sizeof(char));
        strcpy(jobs[job_loc]->name,args[0]);
        jobs[job_loc]->job_number = job_number++;
        int status;
        if(is_bg)
        {
            fprintf(stderr,"[%d] %s %d\n",jobs[job_loc]->job_number,args[0],pid);
            dprintf(stdout_fd,"[%d] %s %d\n",jobs[job_loc]->job_number,args[0],pid);
            kill(pid,SIGTSTP);
            kill(pid,SIGCONT);
            signal(SIGCHLD,exit_child);
        }
        else
        {
            // do{
                waitpid(pid,&status,WUNTRACED);
            // }while(!WIFEXITED(status) && !WIFSIGNALED(status));

            // setpgid(0,0);
			
            if(WIFSTOPPED(status))
			{
                signal(SIGCHLD,exit_child);
                return;
            }

            if(jobs[job_loc]->job_number==job_number-1)
                job_number--;
            free(jobs[job_loc]->name);
            free(jobs[job_loc]);
            jobs[job_loc]=NULL;
        }
        
    }
}
