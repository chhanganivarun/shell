#include"shell.h"



int main(int argc, char const *argv[])
{
    stdin_fd = dup(STDIN_FILENO);
    stdout_fd = dup(STDOUT_FILENO);

	signal(SIGINT,sigint_handler);
	signal(SIGTSTP,sigtstp_handler);
	getcwd(cwd,MAX_PATH);

	initjobslots();

	while(1)
	{
		fflush(stdin);
		prompt();
		char *str_delim = " \n\t";
		size_t buff_size = 200;
		char *buffer = (char *)malloc(buff_size * sizeof(char));
		size_t chars_read;
		chars_read = getline(&buffer,&buff_size,stdin);
		
		if(chars_read == -1)
		{
			//*
			dprintf(stdout_fd,"quit\n");
			//exit(EXIT_SUCCESS);
			fflush(stdin);
			//*/
			//dprintf(stdout_fd,"\nDue to specifications of Assignment 3, Not exiting.\nEnter quit to continue\n");
			continue;
		}
		add_history(buffer);
		
		run_query(buffer);
		free(buffer);
	}
	return 0;
}