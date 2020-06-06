#include "execute_cmd.h"
#include "Tool_Func.h"
#include "define_cmd.h"

using namespace std;

/****
  Single command: pipe: command1 | command2
 ****/
bool exec_pipe(char *input){

	char **para = (char**) malloc(8 *sizeof(char *));
	int index=0;
	char *str;
	str = strtok(input," ");
	while(str !=0){
		para[index]=str;
		index++;
		str = strtok(NULL," ");
	}
	para[index]=NULL;

	/***
	  Process the input command (recognize |)
	 ****/
	int location;	
	for(int i=0;i<index;i++){
		if(strcmp(para[i],"|") ==0){
			location=i;
			break;
		}
	}
	if(location==index-1){
		cout<<"command argument not completed!\n";
		return false;
	}

	char **p1_argv = (char**) malloc(8 *sizeof(char *));
	char **p2_argv = (char**) malloc(8 *sizeof(char *));
	int index1=0;
	int index2=0;

	for(int i=0;i<location;i++){
		p1_argv[index1]=para[i];
		index1++;
	}
	for(int i=location+1;i<index;i++){
		p2_argv[index2]=para[i];
		index2++;
	}
	p1_argv[index1]=NULL;
	p2_argv[index2]=NULL;

	/***
	  Execute single Pipe
	 ****/
	int id;	
	int fd[2];

	if(pipe(&fd[0]) <0){
		perror("Pipe failed");
		return false;
	}
	if((id=fork()) <0){
		perror("Fprk failed");
		return false;
	}
	if(id !=0){
		close(fd[0]);
		dup2(fd[1],1);
		close(fd[1]);
		execvp(p1_argv[0],p1_argv);
		if(errno==ENOENT){		
			fprintf(stderr,"%s: command not found\n",p1_argv[0]);
			return false;
		}
	}
	if (id ==0){
		close(fd[1]);
		dup2(fd[0],0);
		close(fd[1]);
		execvp(p2_argv[0],p2_argv);
		if(errno==ENOENT){
			fprintf(stderr,"%s: command not found\n",p2_argv[0]);
			return false;
		}
	}	

	free(p1_argv);
	free(p2_argv);
	free(para);       
	exit(0);
}


/****
  Single command: redirection: >, >>, <
 ****/
bool exec_redirect(char *input){

	char **para = (char**) malloc(8 *sizeof(char *));
	char **p1_argv = (char**) malloc(8 *sizeof(char *));
	int index=0;
	char *str;
	str = strtok(input," ");
	while(str !=0){
		para[index]=str;
		index++;
		str = strtok(NULL," ");
	}
	para[index]=NULL;


	/***
	  Process the input command (recognize >, >>, <)
	 ****/
	int state;
	int location;	
	for(int i=0;i<index;i++){
		if(strcmp(para[i],"<") ==0){
			location=i;
			state = 0;
			break;
		}
		else if(strcmp(para[i],">") ==0){
			location=i;
			state = 1;
			break;
		}
		else if(strcmp(para[i],">>") ==0){
			location=i;
			state = 2;
			break;
		}
	}
	if(location==index-1){
		cout<< "command argument not completed!\n";
		return false;
	}

	/***
	  Execute redirection
	 ****/
	switch(state){

		case 0:

			{
				//command < file
				int index1=0;
				for(int i=0;i<location;i++){
					p1_argv[index1]=para[i];
					index1++;
				}
				p1_argv[index1]=NULL;

				const char *file = para[location+1];
				int fd = open(file,O_RDONLY);

				if(fd<0){
					cout << file  << ": No such file or directory" << endl;
					return false;

				}
				dup2(fd,0);

				if(fork()!=0){
					wait(NULL);
					close(fd);
				}else{
					execvp(p1_argv[0],p1_argv);
					if(errno==ENOENT){
						fprintf(stderr,"%s: command not found\n",p1_argv[0]);
						return false;

					}
				}
			}

			break;

		case 1:

			{
				//command > file
				int index1=0;
				for(int i=0;i<location;i++){
					p1_argv[index1]=para[i];
					index1++;
				}
				p1_argv[index1]=NULL;

				const char *file = para[location+1];
				int fd = open(file,O_CREAT | O_RDWR | O_TRUNC, 0666);


				if(fd<0){
					err(1,"open");
					return false;

				}
				dup2(fd,1);

				if(fork()!=0){
					wait(NULL);
					close(fd);
				}else{
					execvp(p1_argv[0],p1_argv);
					if(errno==ENOENT){
						fprintf(stderr,"%s: command not found\n",p1_argv[0]);
						return false;

					}
				}
			}

			break;

		case 2:

			{
				//command >> file
				int index1=0;
				for(int i=0;i<location;i++){
					p1_argv[index1]=para[i];
					index1++;
				}
				p1_argv[index1]=NULL;

				const char *file = para[location+1];

				int fd = open(file,O_CREAT | O_RDWR | O_APPEND, 0666);


				if(fd<0){
					err(1,"open");
					return false;

				}
				dup2(fd,1);

				if(fork()!=0){
					wait(NULL);
					close(fd);
				}else{
					execvp(p1_argv[0],p1_argv);
					if(errno==ENOENT){
						fprintf(stderr,"%s: command not found\n",p1_argv[0]); 
						return false;    

					}
				}
			}

			break;
	}

	free(p1_argv);
	free(para);
	exit(0);
}


/****
  Single command: inner and self-defined
 ****/
bool exec_inner(char *input,char *path,vector<string>& h,vector<int>& p,vector<command>& cmd)
{

	char **para;
	para = read_command(input);

	if(strcmp(para[0],"exit")==0){

		exit(0);
	}
	else if(strcmp(para[0],"history")==0){

		def_history(h);
		exit(0);
	}
	else if(strcmp(para[0],"go")==0){

		if(para[1]==NULL || strcmp(para[1],"~")==0){
			exit(0); //Return to let parent process deal
		}
		else{ 
			if(access(para[1],F_OK) !=0){
				cout << "Invalid path" << endl;
				return false;
			}else{
				exit(0); //Return to let parent process deal
			} 
		}	
	}
	else if(strcmp(para[0],"pid")==0){

		def_pid(p);
		exit(0);
	}
	else if(strcmp(para[0],"jobs")==0){
		exit(0);
	}
	else if(strcmp(para[0],"continue")==0){
		exit(0);
	}
	else if(strcmp(para[0],"fg") ==0){
		exit(0);
	}
	else if(strcmp(para[0],"bg" )==0){
		exit(0);
	}else if(strcmp(para[0],"status" )==0){
		exit(0);
	}
	else{

		execvp(para[0],para); //Execute /bin/* command 

		if(errno==ENOENT)
		{
			fprintf(stderr,"%s: command not found\n",para[0]);
			return false;
		} 
	}

	free(para);
	return true;
}
