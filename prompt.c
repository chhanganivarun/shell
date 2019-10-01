#include"shell.h"

void prompt()
{
    snprintf(home_dir, MAX_PATH, "%s", getenv("HOME"));
    char rel_cwd[MAX_PATH+1];
    strcpy(rel_cwd,cwd);
    if(strncmp(cwd,home_dir,strlen(home_dir))==0)
    {
        rel_cwd[0]='~';
        rel_cwd[1]=0;
        strcat(rel_cwd,cwd+strlen(home_dir)*sizeof(char));
    }
    
    strcpy(username,getpwuid(geteuid())->pw_name);
    gethostname(hostname,MAX_PATH);
    printf("<\033[01;32m%s@%s\033[00m:\033[01;34m%s\033[00m> ",username,hostname,rel_cwd);
}