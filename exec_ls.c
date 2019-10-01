#include"shell.h"
void exec_ls(char **args)
{
    char curr_path[MAX_PATH+1];
    int is_path, is_all, is_list,is_bg;
    is_path = is_all = is_list = is_bg = 0;
    for(int i=1;args[i];i++)
    {
        if(args[i][0]=='-')
        {
            for(int j=0;args[i][j];j++)
            {
                if(args[i][j]=='l')
                    is_list = 1;
                if(args[i][j]=='a')
                    is_all = 1;
            }
        }
        else
        {
            if(args[i][0]=='&')
                is_bg = 1;
            else
            {
                is_path = 1;
                strcpy(curr_path,args[i]);
            }
            
        }
    }
    if(!is_path)
        strcpy(curr_path,cwd);
    else
    {
        if(curr_path[0]!='~'&&curr_path[0]!='.'&&curr_path[0]!='/')
        {
            char temp[MAX_PATH+1];
            strcpy(temp,curr_path);
            strcpy(curr_path,"./");
            strcat(curr_path,temp);
        }
        abspath(curr_path);
    }
    
    struct stat st;
    stat(curr_path,&st);
    if(!S_ISDIR(st.st_mode))
    {
        printf("Not a valid directory\n");
        return ;
    }
    DIR *dir = opendir(curr_path);
    if(!dir)
    {
        perror("Error opening directory");
        return ;
    }
    finfo *files;
    int f_buffsize = 128;
    files = (finfo*)malloc(f_buffsize*sizeof(finfo));
    if(!files)
    {
        perror("Error allocating space.\nOut of Memory\n");
        exit(EXIT_FAILURE);
    }
    struct dirent *entry;
    int cnt = 0;    
    while(entry = readdir(dir))
    {
        files[cnt++].name = entry->d_name;
        if(cnt>=f_buffsize)
        {
            f_buffsize*=2;
            files = realloc(files,f_buffsize*sizeof(finfo));
            if(!files)
            {
                perror("Error allocating space.\nOut of Memory\n");
                exit(EXIT_FAILURE);
            }
        }
    }
    int num_entries = cnt;
    if(!is_list)
    {
        for(int i=0;i<num_entries;i++)
        {
            if((strcmp(files[i].name,"..")&&strcmp(files[i].name,"."))||is_all)
                printf("%s\n",files[i].name);
        }
        return;
    }
    printf("total files %d\n",num_entries);
    int blocks = 0;
    for(int i=0;i<num_entries;i++)
    {
        struct stat st;
        char file_path[MAX_PATH+1];
        strcpy(file_path,curr_path);
        strcat(file_path,"/");
        strcat(file_path,files[i].name);
        if(stat(file_path,&st)==-1)
        {
            perror("");
            return;
        }
        blocks+=st.st_blocks;
        if (S_ISREG(st.st_mode)) files[i].perm[0] = '-';
		else if (S_ISDIR(st.st_mode)) files[i].perm[0] = 'd';
		else if (S_ISCHR(st.st_mode)) files[i].perm[0] = 'c';
		else if (S_ISBLK(st.st_mode)) files[i].perm[0] = 'b';
		else if (S_ISFIFO(st.st_mode)) files[i].perm[0] = 'p';
		else if (S_ISLNK(st.st_mode)) files[i].perm[0] = 'l';
		else files[i].perm[0] = 's';

		if (st.st_mode & S_IRUSR) files[i].perm[1] = 'r';
		else files[i].perm[1] = '-';
		if (st.st_mode & S_IWUSR) files[i].perm[2] = 'w';
		else files[i].perm[2] = '-';
		if (st.st_mode & S_IXUSR) files[i].perm[3] = 'x';
		else files[i].perm[3] = '-';

		if (st.st_mode & S_IRGRP) files[i].perm[4] = 'r';
		else files[i].perm[4] = '-';
		if (st.st_mode & S_IWGRP) files[i].perm[5] = 'w';
		else files[i].perm[5] = '-';
		if (st.st_mode & S_IXGRP) files[i].perm[6] = 'x';
		else files[i].perm[6] = '-';

		if (st.st_mode & S_IROTH) files[i].perm[7] = 'r';
		else files[i].perm[7] = '-';
		if (st.st_mode & S_IWOTH) files[i].perm[8] = 'w';
		else files[i].perm[8] = '-';
		if (st.st_mode & S_IXOTH) files[i].perm[9] = 'x';
		else files[i].perm[9] = '-';

		files[i].perm[10] = '\0';
		files[i].nlinks = st.st_nlink;
		strcpy(files[i].owner,getpwuid(st.st_uid)->pw_name);
		strcpy(files[i].group,getgrgid(st.st_gid)->gr_name);
		files[i].size = st.st_size;
        strftime(files[i].mtime, 100, "%b %d %H:%M", localtime(&st.st_mtime));

        if((strcmp(files[i].name,"..")&&strcmp(files[i].name,"."))||is_all)
            printf("%s %ld\t%s\t%s\t%ld\t%s\t%s\n",files[i].perm,files[i].nlinks,files[i].owner,files[i].group,files[i].size,files[i].mtime, files[i].name);

    }
    printf("total blocks(approx): %d\n",blocks/2);
    free(files);
    return;
}