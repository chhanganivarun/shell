#ifndef VSH_H

#define VSH_H

#include<stdio.h>
#include<fcntl.h>
#include<unistd.h>
#include<string.h>
#include<stdlib.h>
#include<stdbool.h>
#include<sys/types.h>
#include<sys/wait.h>
#include<dirent.h>
#include<sys/stat.h>
#include<time.h>
#include<grp.h>
#include<pwd.h>
#include <sys/utsname.h>
#include<signal.h>
#include<error.h>
#include<readline/readline.h>
#include<readline/history.h>
#include<curses.h>

#define MAX_PATH 1023
#define MAX_NAME 1023
#define MAX_JOBS 1024
//


char home_dir[MAX_PATH+1];
char cwd[MAX_PATH+1];
char username[MAX_NAME+1];
char hostname[MAX_NAME+1];
int stdin_fd,stdout_fd;

void prompt();
char **split_str(char *,char *);
void free_args(char **args);
int len_args(char **args);
int argcmp(char **args,char *val);
char *relative_path(char *path);
char *abspath(char *rel_path);
int findjobslot();
void initjobslots();
int pidslot(pid_t);
int numberslot(int);

void exec_cd(char **);
void exec_echo(char **);
void exec_ls(char **);
void exec_pwd(char **);
void exec_quit(char **);
void exec_pinfo(char **);
void exec_nightswatch(char **);
void exec_setenv(char **);
void exec_unsetenv(char **);
void exec_getenv(char **);
void exec_jobs(char **);
void exec_kjob(char **);
void exec_fg(char **);
void exec_bg(char **);
void exec_overkill(char **);
void execute(char **);

void exec_history(char **);

int redir(char **args);

void exit_child(int );
void sigterm_handler(int signum);
void sigint_handler(int signum);
void sigcont_handler(int signum);
void sigtstp_handler(int signum);

void run_query(char *command);
void run_command(char **args);

typedef struct file_info{
    char perm[10];
    nlink_t nlinks;
    char owner[MAX_NAME+1];
    char group[MAX_NAME+1];
    off_t size;
    char mtime[100];
    char *name;
}finfo;
typedef struct{
    pid_t pid;
    char *name;
    int job_number;
}job_struct;
job_struct *jobs[MAX_JOBS];
int job_number;
#endif