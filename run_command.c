#include"shell.h"

char *valid_commands[] = {"cd", "ls", "pwd", "echo","quit","pinfo","nightswatch","setenv","unsetenv","getenv","jobs","kjob","fg","bg","overkill",NULL};
void (*command_handlers[])(char **) = {&exec_cd, &exec_ls, &exec_pwd, &exec_echo, &exec_quit, &exec_pinfo, &exec_nightswatch,&exec_setenv,&exec_unsetenv,&exec_getenv,&exec_jobs,&exec_kjob,&exec_fg,&exec_bg,&exec_overkill,NULL};
char *str_delim = " \n\t";

void run_query(char *command)
{
    char **commands = split_str(command,";");
    for(int i=0;commands[i]!=NULL;i++)
    {
        char **piped_commands = split_str(commands[i],"|");
        int pipes = len_args(piped_commands)-1;
        /*
        for(int j=0;piped_commands[j];j++)
        {
            dprintf(stdout_fd,"%s ",piped_commands[j]);
        }
        dprintf(stdout_fd,"\n");
        */
        if(pipes<0)
        {
            //dprintf(stdout_fd,"Nothing here\n");
            continue;
        }
        int fds[2];
        for(int j=0;piped_commands[j];j++)
        {
            if(j!=0)
            {
                close(fds[1]);
                if(dup2(fds[0],STDIN_FILENO)==-1)
                    perror(NULL);
                close(fds[0]);
            }
            if(piped_commands[j+1]) // not the last element
            {
                pipe(fds);
                if(dup2(fds[1],STDOUT_FILENO)==-1)
                    perror(NULL);
            }
            
            char **args = split_str(piped_commands[j],str_delim);
            if(args[0]==NULL)
                continue;
            /*
            for(int k=0;args[k];k++)
            {
                dprintf(stdout_fd,"%s ",args[k]);
            }
            dprintf(stdout_fd,"\n");
            */
            redir(args);
            run_command(args);
            close(STDOUT_FILENO);
            close(STDIN_FILENO);
            dup2(stdin_fd,STDIN_FILENO);
            dup2(stdout_fd,STDOUT_FILENO);            
        }
        dup2(stdin_fd,STDIN_FILENO);
        dup2(stdout_fd,STDOUT_FILENO);
    }
}
void run_command(char **args)
{

    for(int j=0;valid_commands[j];j++)
    {
        if(strcmp(args[0],valid_commands[j])==0)
        {
            (*command_handlers[j])(args);
            free_args(args);
            return ;
        }

    }
    execute(args);
}