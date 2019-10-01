#include"shell.h"
char *get_inter()
{
    FILE* fp = fopen("/proc/interrupts","r");
    if(!fp)
    {
        perror("Couldn't open file /proc/interrupts");
        return NULL;
    }
    char *ret = (char *)malloc(1024*sizeof(char));
    size_t buffer_size=1023;
    while(!feof(fp)&&getline(&ret,&buffer_size,fp)>=1)
    {
        if(strstr(ret,"1-edge")&&strstr(ret,"i8042"))
        {
            fclose(fp);
            return ret;
        }
    }
}
char *get_dirty()
{
    FILE* fp = fopen("/proc/meminfo","r");
    if(!fp)
    {
        perror("Couldn't open file /proc/meminfo");
        return NULL;
    }
    char *ret = (char *)malloc(1024*sizeof(char));
    size_t buffer_size=1023;
    while(!feof(fp)&&getline(&ret,&buffer_size,fp)>=1)
    {
        if(strstr(ret,"Dirty"))
        {
            fclose(fp);
            return ret;
        }
    }
}

void exec_nightswatch(char **args)
{
    initscr();
    timeout(0);
    int secs = 5;
    char command[20];
    int is_dirty = 0,is_inter = 0;
    for(int i=1;args[i];i++)
    {
        if(strcmp(args[i],"-n")==0&& args[i+1])
        {
            secs = atoi(args[i+1]);
            i+=1;
        }
        else
        {
            strcpy(command,args[i]);
        }
    }
    if(strcmp(command,"dirty")==0) is_dirty=1;
    if(strcmp(command,"interrupt")==0) is_inter=1;
    if(!is_dirty&&!is_inter)
    {
        printf("Command %s not valid\r\n",command);
    }
    char * (*command_func)(void);
    if(is_dirty)
    {
        command_func = &get_dirty;
    }
    else
    {
        command_func = &get_inter;
    }
    char *s;
    while((s=(*command_func)()))
    {
        printf("%s\n\r",s);
        free(s);
        sleep(secs);
        if(getch()=='q')
        {
            endwin();
            return ;
        }
    }
    printf("Null aaya\n");
}
