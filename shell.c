#include"shell.h"
char **split_str(char *str,char *delim)
{
    int buff_size = 100;
    int cnt = 0;
    char **tokens = (char **)malloc(buff_size * sizeof(char *));
    if(!tokens)
    {
        dprintf(stdout_fd,"Out of Memory\n");
        exit(EXIT_FAILURE);
    }
    char *token = strtok(str,delim);
    while(token)
    {
        tokens[cnt] = token;
        cnt++;
        token = strtok(NULL,delim);
        if(cnt==buff_size-1)
        {
            buff_size*=2;
            tokens = realloc(tokens,buff_size*sizeof(char *));
            if(!tokens)
            {
                dprintf(stdout_fd,"Out of Memory\n");
                exit(EXIT_FAILURE);
            }
        }
    }
    tokens[cnt] = NULL;
    return tokens;
}

void free_args(char **args)
{
    free(args);
}

int len_args(char **args)
{
    int cnt=0;
    for(; args[cnt]; cnt++ );

    return cnt;
}

int argcmp(char **args,char *val)
{
    for(int i=0;args[i];i++)
    {
        if(!strcmp(args[i],val))
            return i;
    }
    return -1;
}

char *relative_path(char *path)
{
    char *rel_path = (char*)malloc((MAX_PATH+1)*sizeof(char));
    snprintf(home_dir, MAX_PATH, "%s", getenv("HOME"));
    strcpy(rel_path,path);
    if(strncmp(path,home_dir,strlen(home_dir))==0)
    {
        rel_path[0]='~';
        rel_path[1]=0;
        strcat(rel_path,path+strlen(home_dir)*sizeof(char));
    }
}

char *abspath(char *rel_path)
{
    char new_path[MAX_PATH+1];
    if(strncmp(rel_path,"~",1)==0)
    {
        strcpy(new_path,home_dir);
        strcat(new_path,rel_path+1*sizeof(char));
    }
    else if(strncmp(rel_path,"..",2)==0)
    {
        char temp[MAX_PATH+1];
        strcpy(temp,cwd);
        int curr=strlen(temp);
        while(curr>0 && temp[curr-1]!='/')
        {
            curr--;
            temp[curr] = 0;
        }
        temp[curr-1]=0;
        strncpy(new_path,temp,curr);
        strcat(new_path,rel_path+2*sizeof(char));
    }
    else if(strncmp(rel_path,".",1)==0)
    {
        strcpy(new_path,cwd);
        strcat(new_path,rel_path+1*sizeof(char));
    }
    else
    {
        strcpy(new_path,rel_path);
    }
    strcpy(rel_path,new_path);
    return rel_path;
}

void initjobslots()
{
    job_number=1;
    for(int i=0;i<MAX_JOBS;i++)
    {
        jobs[i] = NULL;
    }
}

int findjobslot()
{
    int ret=0;
    for(int i=0;jobs[i];i++,ret++);
    return ret<MAX_JOBS?ret:-1;
}

int pidslot(pid_t pid)
{
    int ret=0;
    for(int i=0;i<MAX_JOBS;i++,ret++)
    {
        if(jobs[i] && jobs[i]->pid == pid)
            break;
    }
    return ret<MAX_JOBS?ret:-1;
}

int numberslot(int n)
{
    int ret=0;
    for(int i=0;i<MAX_JOBS;i++,ret++)
    {
        if(jobs[i] && jobs[i]->job_number == n)
            break;
    }
    return ret<MAX_JOBS?ret:-1;
}