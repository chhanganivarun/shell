#include"shell.h"
void exec_cd(char **args)
{
    if(len_args(args)==1)
    {
        args[1] = "~";
        args[2] = NULL;
    }
    char new_path[MAX_PATH+1];
    strcpy(new_path,abspath(args[1]));
    printf("Trying to go to %s\n",new_path);
    if(chdir(new_path)==-1)
    {
        perror("");
    }
    else
    {
        getcwd(cwd,MAX_PATH);
        printf("%s\n",cwd);
    }
    
    getcwd(cwd,MAX_PATH);
}