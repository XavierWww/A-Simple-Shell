#define TRUE 1
#define PSIZE 100

#include "define_cmd.h"
#include "Tool_Func.h"
#include "execute_cmd.h"
#include "enum.h"
#include "jobs.h"

using namespace std;

vector<Job> jobs;   //Store background jobs
vector<Job> t_jobs; //Store foreground and background jobs
int count=0;        //Record the id of jobs in real time
int g_pid;	    //Store current pid of jobs

/***
  Handle SIGCHLD signal
 ***/
void sigchld_handler(int sig){

	int pid = waitpid(-1,0, WNOHANG);

	for(unsigned i=0; i< jobs.size(); i++){

		if(pid==jobs.at(i).pid){
			jobs.at(i).Running = false;
			jobs.at(i).Terminated= true;
			jobs.at(i).Stopped = false;
		}
	}
}

/***
  Ctrl+z
 ***/
void sigint_handler(int sig){

	cout << endl;

	for(unsigned i=0; i< t_jobs.size(); i++){

		if(g_pid== t_jobs.at(i).pid){
			kill(g_pid,SIGINT);
			break;
		}
	}

}

/***
  Ctrl+c
 ***/
void sigtstp_handler(int sig){

	cout << endl;

	for(unsigned i=0; i< t_jobs.size(); i++){

		if(g_pid==t_jobs.at(i).pid){

			kill(g_pid,SIGTSTP);
			break;
		}
	}
}


int main(){

	signal(SIGINT,sigint_handler);
	signal(SIGTSTP,sigtstp_handler);
	signal(SIGCHLD,sigchld_handler);

	char path[PSIZE];
	getcwd(path,PSIZE);     //Get the current shell path
	vector<string> h;       //Store the history of command
	vector<int> p;          //Store pid of process
	p.push_back(getpid());	//Record the main pid

	while(TRUE){


		Job newjob;
		if(jobs.size()==0){count=0;}

		/****
		  Initialize some variables about jobs
		 ****/
		newjob.cmd_line="";
		newjob.jid= newjob.pid = 0;
		newjob.Running = true;
		newjob.Terminated = false;
		newjob.Stopped = false;

		string input_string = shell_prompt();

		if(input_string.size() !=0){

			/****
			  Initialize some variables about commands
			 ****/
			command temp;
			vector<command> cmd;
			state t = Start;
			temp.string_command = "";
			temp.mul_type = temp.type = -1;
			temp.bg = false;

			h.push_back(input_string); //Record every line of input commands
			newjob.cmd_line=input_string;

			bool flag = parser(input_string,temp,cmd,t);
			if(flag==false)
			{
				continue;	
			}

			if(cmd.size()==0){
				continue; //Execute space commands
			}else if(cmd.size() ==1){
				single_cmd(cmd,h,p,path,jobs,newjob,count,g_pid,t_jobs); //Execute single commands
			}else{
				multi_cmd(cmd,h,p,path,jobs); //Execute multi commands
			}

			jobs_exec(jobs,cmd,input_string,g_pid); //Execute job control
		}
	}
	return 0;
}
