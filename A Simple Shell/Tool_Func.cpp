#include "Tool_Func.h"
#include "execute_cmd.h"
#include "define_cmd.h"
#include "jobs.h"

using namespace std;

/****
  Prompt user to enter command
 ****/   
char *shell_prompt()
{
	static char input[80];
	char buff[100];
	getcwd(buff,100);
	cout<<"["<<"OS server:"<<buff<<"]"<<"$";
	cin.getline(input,80);

	return input;
}

/****
  Split the user input
 ****/ 
char **read_command(char *input)
{     
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

	return para;
}

/**** 
  Execute multiple (; && ||) command in the command line
 ****/
void multi_cmd(vector<command>& cmd,vector<string>& h, vector<int>& p, char *path, vector<Job>& jobs)
{
	switch(cmd.at(0).mul_type){

		/*****
		  Multi-command using ";" 
		  Which can execute [inner, defined, pipe and redirection]  four mixed commands
		  For example:  whoami; who | wc -l; ls > output.txt; go file
		 ******/
		case 4:

			for(size_t i=0;i<cmd.size();i++){

				int pid=fork();

				if(pid<0){

					perror("fork failed:");
					exit(1);
				}
				if(pid >0){

					int status;
					p.push_back(pid);
					waitpid(pid,&status,0);

					if(WIFEXITED(status) !=0){       					                               
						def_go(cmd,path,i,jobs);
					}

				}
				if(pid ==0){

					char *input = (char *)cmd.at(i).string_command.c_str();
					if(cmd.at(i).type==1){
						exec_inner(input,path,h,p,cmd);
					}else if(cmd.at(i).type==2){
						exec_pipe(input);
					}else if(cmd.at(i).type==3){
						exec_redirect(input);		
					}

					exit(errno);
				}
			}

			break;

		/*****
		  Multi-command using "&&" 
		  Which can execute [inner, defined, pipe and redirection] four mixed commands
		******/
		case 5:

			for(size_t i=0;i<cmd.size();i++){

				int pid=fork();

				if(pid<0){

					perror("fork failed:");
					exit(1);
				}
				if(pid >0){

					int status;
					p.push_back(pid);
					waitpid(pid,&status,0);

					if(WIFEXITED(status) !=0){

						if(WEXITSTATUS(status)){
							break;
						}else{

							def_go(cmd,path,i,jobs);
							continue;
						}

					}
					else{
						break;
					}

				}
				if(pid ==0){

					char *input = (char *)cmd.at(i).string_command.c_str();
					if(cmd.at(i).type==1){
						exec_inner(input,path,h,p,cmd);                         
					}else if(cmd.at(i).type==2){
						exec_pipe(input);
					}else if(cmd.at(i).type==3){
						exec_redirect(input);		
					}

					abort();    
				}

			}

			break;

		/*****
		  Multi-command using "||" 
		  Which can execute [inner, defined, pipe and redirection] four mixed commands
		******/
		case 6:

			for(size_t i=0;i<cmd.size();i++){

				int pid=fork();

				if(pid<0){

					perror("fork failed:");
					exit(1);
				}
				if(pid >0){

					int status; 
					p.push_back(pid);
					waitpid(pid,&status,0);

					if(WIFEXITED(status) !=0){

						if(WEXITSTATUS(status)){
							continue;
						}else{
							def_go(cmd,path,i,jobs);
							break;
						}

					}
					else{
						continue;
					}

				}
				if(pid ==0){

					char *input = (char *)cmd.at(i).string_command.c_str();
					if(cmd.at(i).type==1){												
						exec_inner(input,path,h,p,cmd);
					}else if(cmd.at(i).type==2){
						exec_pipe(input);
					}else if(cmd.at(i).type==3){
						exec_redirect(input);		
					}

					abort();
				}

			}

			break;
	}

}


/**** 
  Execute single command (inner, defined, pipe and redirection) in the command line
 ****/
void single_cmd(vector<command>& cmd, vector<string>& h, vector<int>& p, char *path, vector<Job>& jobs, Job& newjob, int& count, int& g_pid, vector<Job>& t_jobs){

	int pid;

	if((pid=fork()) !=0){

		g_pid = newjob.pid = pid;
		int status;
		p.push_back(pid);

		if(cmd.at(0).bg ==true){ //Command & running in the background

			g_pid = 0;
			newjob.jid=(++count);
			t_jobs.push_back(newjob);
			cout << "[" << newjob.jid  << "] " << newjob.pid << " command running in the background" << endl;
			jobs.push_back(newjob);

		}
		else{

			t_jobs.push_back(newjob);

			waitpid(pid,&status,WUNTRACED | WCONTINUED);
			if(WIFEXITED(status)) { //exit normally

				def_go(cmd,path,0,jobs);
			}
			else if(WIFSIGNALED(status)) { //exit abnormally

				cout << "Kill the job successfully!" << endl;
			} else if(WIFSTOPPED(status)) { //job temporarily stopped

				newjob.Stopped = true;
				newjob.Running = false;
				newjob.Terminated = false;
				newjob.jid=(++count);
				job_disp(newjob);
				jobs.push_back(newjob);
			}
		}   
	}
	else{

		setpgid(getpid(),getpid()); //Set process group id 
		char *input = (char *)cmd.at(0).string_command.c_str();
		if(cmd.at(0).type==1){
			exec_inner(input,path,h,p,cmd);
		}else if(cmd.at(0).type==2){
			exec_pipe(input);
		}else if(cmd.at(0).type==3){
			exec_redirect(input);
		}

		exit(errno);
	}                                                                                                                                                                
}
