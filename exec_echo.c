#include"shell.h"
void exec_echo(char **args)
{
    if(len_args(args)==1)
    {
        args[1]="";
        args[2]=NULL;
    }

    printf("%s\n",args[1]);
    
}