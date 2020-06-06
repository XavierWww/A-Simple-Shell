#include "jobs.h"

/***
  Display information when stopping job
 ****/
void job_disp(Job& newjob){

	cout << "[" << newjob.jid  <<"] ";
	if (newjob.Terminated==false && newjob.Running==false && newjob.Stopped==true) {
		cout << "Stopped\t" << newjob.cmd_line << endl;
	}
}


/***
  Execute jobs, continue, bg, fg commands
 ****/
void jobs_exec(vector<Job>& jobs,vector<command> cmd,string input_string,int& g_pid){

	for(size_t i=0; i<cmd.size(); i++){

		char **para = (char**) malloc(8 *sizeof(char *));
		vector<string> cmdline;		
		string s;
		stringstream temp_s;
		temp_s << input_string;
		while(temp_s>>s){
			cmdline.push_back(s);
		}
		for(size_t i=0; i< cmdline.size(); i++){
			para[i]=(char*)cmdline[i].c_str();	
		}


		if(strcmp(para[0],"exit")==0){

			exit(0);
		}else if(strcmp(para[0],"status")==0){

			cout << "Sorry, please refer to the reason on readme.txt" <<endl; 
		}
		else if(strcmp(para[0],"jobs")==0){ //Command 'jobs' - display the information of the background jobs

			if(jobs.size()==0){cout << "There is no job now!"<< endl; continue;}

			for (size_t i = 0; i < jobs.size(); i++) {
				cout << "[" << jobs.at(i).jid << "] ";
				if (jobs.at(i).Terminated == true && jobs.at(i).Running==false && jobs.at(i).Stopped==false) {
					cout << "Terminated\t" << jobs.at(i).cmd_line << endl;
				}
				else if (jobs.at(i).Terminated==false && jobs.at(i).Running==true && jobs.at(i).Stopped==false) {
					cout << "Running\t" << jobs.at(i).cmd_line << endl;
				}
				else if (jobs.at(i).Terminated==false && jobs.at(i).Running==false && jobs.at(i).Stopped==true) {
					cout << "Stopped\t" << jobs.at(i).cmd_line << endl;
				}
			}
		}
		else if(strcmp(para[0],"continue")==0){ //Command 'continue #Job_ID' - continue the background jobs

			if(para[1]==NULL && para[2] !=0){cout << "command: continue: parameter error!" << endl;}	
			int index;
			int flag = 0;
			int temp_pid;
			stringstream ss;
			ss << para[1];
			ss >> index;

			for(size_t i=0; i<jobs.size(); i++){

				if(jobs.at(i).jid==index && jobs.at(i).Stopped==true){
					temp_pid = jobs.at(i).pid;
					kill(temp_pid,SIGCONT);
					jobs.at(i).Running = true;
					jobs.at(i).Terminated = false;
					jobs.at(i).Stopped = false;
					flag=1;
					cout<< "Continue #job" << index << " successfully!"<< endl;
					break;		
				}else if(jobs.at(i).jid==index && jobs.at(i).Running ==true){
					cout << "continue: " << "job" << index  << " already continue!" << endl;
					break;			
				}
			}
			if(flag==0){cout << "continue: " << index  <<" No such job!" << endl; continue;}

		}
		else if(strcmp(para[0],"bg")==0){ //Command 'bg #Job_ID' - put the jobs into the background to run

			if(para[1]==NULL && para[2] !=0){cout << "command: bg: parameter error!" << endl;}	
			int index;
			int flag=0;
			int temp_pid;
			stringstream ss;
			ss << para[1];
			ss >> index;

			for(size_t i=0; i < jobs.size(); i++){
				if(jobs.at(i).jid==index && jobs.at(i).Stopped==true){
					temp_pid = jobs.at(i).pid;
					kill(temp_pid,SIGCONT);
					jobs.at(i).Running = true;
					jobs.at(i).Terminated = false;
					jobs.at(i).Stopped = false;
					flag = 1;
					cout << "[" << jobs.at(i).jid << "] " << jobs.at(i).cmd_line << " &" << endl;
					cout << "Put the suspended job to the background!" << endl;
					break;
				}
				else if(jobs.at(i).jid==index && jobs.at(i).Running==true){
					flag = 1;
					cout << "bg: " << "job" << index  << " already running in the background!" <<  endl;
					break;
				}

			}
			if(flag==0){cout << "bg: " << index  <<" No such job" << endl; continue;}

		}
		else if(strcmp(para[0],"fg")==0){ //Command 'fg #Job_ID' - put the jobs into the foreground to run
			if(para[1]==NULL && para[2] !=0){cout << "command: fg: parameter error!" << endl;}	

			int index;
			int temp_pid;
			int flag = 0;
			stringstream ss;
			int status;
			ss << para[1];
			ss >> index;

			for(size_t i=0; i<jobs.size(); i++){
				if( jobs.at(i).jid==index && jobs.at(i).Terminated==false ){
					g_pid = temp_pid = jobs.at(i).pid;
					kill(temp_pid,SIGCONT);
					jobs.at(i).Running = true;
					jobs.at(i).Terminated = false;
					jobs.at(i).Stopped = false;
					cout << "Put the background to the foreground successfully!" << endl;
					flag=1;
					break;
				}
			}	

			if(flag==0){cout << "fg: " << index  <<" No such job" << endl; continue;}

			waitpid(temp_pid,&status,WUNTRACED);

			if(WIFEXITED(status)){ //exit normally
				for(size_t i=0; i< jobs.size(); i++){

					if(jobs.at(i).pid==temp_pid){
						jobs.at(i).Running = false;
						jobs.at(i).Terminated= true;
						jobs.at(i).Stopped = false;
					}
				}
			}
			else if(WIFSIGNALED(status)){ //exit abnormally

				for(size_t i=0; i< jobs.size(); i++){

					if(jobs.at(i).pid==temp_pid){
						jobs.at(i).Running = false;
						jobs.at(i).Terminated= true;
						jobs.at(i).Stopped = false;
					}

				}
				cout << "Kill the job successfully!" << endl;
			}else if(WIFSTOPPED(status)){ //job temporarily stopped

				for(size_t i=0; i< jobs.size(); i++){

					if(jobs.at(i).pid==temp_pid){
						jobs.at(i).Running = false;
						jobs.at(i).Terminated= false;
						jobs.at(i).Stopped = true;
					}
				}

			}
		}
	}

	/***
	  Clear the terminated jobs when the shell command line prompts again
	 ****/
	vector<Job>::iterator Iter;
	int i = 0;
	for(Iter = jobs.begin(); Iter != jobs.end(); Iter=Iter) {

		if(jobs.at(i).Terminated==true){
			jobs.erase(jobs.begin()+i);
			Iter = jobs.begin();
			i = 0;                                                       
		}else{
			Iter++;
			i++; 
		}
		if(Iter == jobs.end()){
			break;
		}
	}
}
