#include"shell.h"

void exec_setenv(char **args)
{
    int argsc = len_args(args);
    if(argsc!=3 && argsc!=2)
    {
        printf("Invalid usage of setenv\nUsage: setenv var value\n");
        return ;
    }
    int ret;
    if(argsc==3)
        ret = setenv(args[1],args[2],1);    //overwrite always set to true
    else
        ret = setenv(args[1],"",1);    //overwrite always set to true
    if(ret)
    {
        perror("Error while executing setenv");
        return ;
    }
}

void exec_unsetenv(char **args)
{
    int argsc = len_args(args);
    if(argsc!=2)
    {
        printf("Invalid usage of unsetenv\nUsage: unsetenv var\n");
        return ;
    }
    char *value = getenv(args[1]);
    if(!value)
    {
        printf("Environment variable %s doesn't exist\n",args[1]);
        return ;
    }
    printf("The value of environment variable %s was %s\n",args[1],value);
    int ret = unsetenv(args[1]);
    if(ret)
    {
        perror("Error while executing unsetenv");
        return ;
    }
}
void exec_getenv(char **args)
{
    int argsc = len_args(args);
    if(argsc!=2)
    {
        printf("Invalid usage of getenv\nUsage: getenv var\n");
        return ;
    }
    char *value = getenv(args[1]);
    if(!value)
    {
        printf("Environment variable %s doesn't exist\n",args[1]);
        return ;
    }
    printf("The value of environment variable %s is %s\n",args[1],value);
}
