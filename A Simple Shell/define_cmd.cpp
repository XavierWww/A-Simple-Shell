#include "define_cmd.h"
#include "Tool_Func.h"
#include "enum.h"

using namespace std;

/****
  Self-defined command history
 ****/
void def_history(vector<string>& h){
	int size =h.size();
	if(size>5000){ //record only 5000 history command
		int index1=1;
		for( size_t i=size-5000;i<h.size();i++){
			cout<< index1 <<". " << h[i] << endl;
			index1++;
		}
	}else{
		for( size_t i=0;i<h.size();i++){
			cout<< i+1 <<". " << h[i] << endl;
		}
	}
}

/****
  Self-defined command pid
 ****/
void def_pid(vector<int>& p)
{
	if(p.size()>5){ //print pid of 5 recent processes
		int l=5;
		for(size_t i=p.size()-1;i>=p.size()-5;i--){
			cout << "[" << l <<"] " << p[i]  <<endl;
			l--;
		}
	}else{
		int l=p.size();
		for(int i=l-1;i>=0;i--){
			cout << "[" << l <<"] " << p[i]  <<endl;
			l--;
		}
	}

}

/****
  Self-defined command go
 ****/
void def_go(vector<command>& cmd, char* path, int i, vector<Job>& jobs)
{ 
	char *input = (char*)cmd.at(i).string_command.c_str();
	char **para = read_command(input);

	if(strcmp(para[0],"go") ==0){

		if(para[1]==NULL || strcmp(para[1],"~")==0){chdir(path);}
		else{
			chdir(para[1]);
		}
	}else if(strcmp(para[0],"exit")==0){
		for(unsigned int i=0; i < jobs.size(); i++){
			kill(jobs.at(i).pid,SIGTERM);
		}
		exit(0);
	}

	free(para);
}
