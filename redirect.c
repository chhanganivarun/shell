#include"shell.h"
int redir(char **args)
{
    int argsc = len_args(args);
    int in_redir = argcmp(args,"<");
    if(in_redir!=-1)
    {
        if(in_redir+1>=argsc)
        {
            dprintf(stdout_fd,"Invalid redirection\n");
            return -1;
        }
        int fd_read = open(abspath(args[in_redir+1]),O_RDONLY);
        if(!fd_read)
        {
            perror("Couldn't read from input redirection: ");
            return -1;
        }

        dup2(fd_read,STDIN_FILENO);
        close(fd_read);
    }
    int out_redir = argcmp(args,">");
    int app_redir = argcmp(args,">>");
    if(out_redir!=-1)
    {
        if(out_redir+1>=argsc)
        {
            dprintf(stdout_fd,"Invalid redirection\n");
            return -1;
        }
        int fd_out = open(abspath(args[out_redir+1]),O_WRONLY|O_CREAT|O_TRUNC,0644);
        if(!fd_out)
        {
            perror("Couldn't write at output redirection: ");
            return -1;
        }

        dup2(fd_out,STDOUT_FILENO);
        close(fd_out);

    }
    else if(app_redir!=-1)
    {
        if(app_redir+1>=argsc)
        {
            dprintf(stdout_fd,"Invalid redirection\n");
            return -1;
        }
        int fd_app = open(abspath(args[app_redir+1]),O_WRONLY|O_CREAT|O_APPEND,0644);
        if(!fd_app)
        {
            perror("Couldn't write at output redirection: ");
            return -1;
        }

        dup2(fd_app,STDOUT_FILENO);
        close(fd_app);

    }
    return 0;
}