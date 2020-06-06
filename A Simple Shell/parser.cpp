#include "Tool_Func.h"

using namespace std;

/***
  Command line parser
 ****/
bool parser(string input_string, command& temp, vector<command>& cmd, state t)
{                  

	for(size_t i=0;i < input_string.size();i++){

		switch(t){

			case Start:

				if(input_string[i]==' '){continue;}
				else if(input_string[i] ==';' || input_string[i] =='>' || input_string[i] =='<' || input_string[i] =='|' || input_string[i] =='&' || input_string[i] =='`' || input_string[i] =='*'){

					cout << input_string[i] << " : is an invalid command" << endl;
					return false;              
				}else{

					temp.type = 1;
					temp.string_command += input_string[i];
					t=Inner;
				}

				break;

			case Inner:

				if(input_string[i] == ';'){

					temp.mul_type=4; //command multi-type is Multi(;)
					cmd.push_back(temp);
					temp.string_command = "";
					temp.bg = false;
				}
				else if(input_string[i] == '|'){

					if(i==input_string.size()-1){
						cout<< "command argument not completed!" << endl;
						return false;
					}else if(input_string[i+1]=='|'){
						temp.mul_type=6; //command multi-type is Or(||)
						cmd.push_back(temp);
						temp.string_command="";
						temp.bg = false;

						t=Or;
					}else{
						temp.type = 2; //command type is pipie
						temp.string_command += input_string[i];

						t = Pipe;
					}
				}
				else if(input_string[i] == '>' || input_string[i] == '<'){

					if(i==input_string.size()-1){
						cout<< "command argument not completed!" << endl;
						return false;
					}
					temp.type = 3; //command type is redirection
					temp.string_command += input_string[i];

					t = Redirect;
				}
				else if(input_string[i] == '&'){

					if(i==input_string.size()-1){

						temp.bg = true;
						cmd.push_back(temp);
						temp.string_command="";
					}else{
						temp.mul_type=5; //command multi-type is And(&&)
						cmd.push_back(temp);
						temp.string_command="";
						temp.bg = false;

						t = And;
					}
				}
				else if(input_string[i]=='\''){

					if(i==input_string.size()-1){
						cout<< "command argument not completed!" << endl;
						return false;
					}

					t=Quote1;	
				}else if(input_string[i]=='"'){

					if(i==input_string.size()-1){
						cout<< "command argument not completed!" << endl;
						return false;
					}

					t=Quote2;	
				}
				else {

					temp.string_command += input_string[i];
					temp.type = 1; //command type is inner-command

					t = Inner;
				}

				break;

			case Pipe:

				if(input_string[i] == ';'){

					temp.mul_type=4;
					cmd.push_back(temp);
					temp.string_command = "";
					temp.bg = false;

					t = Start;
				}else if(input_string[i] == '&'){

					if(i==input_string.size()-1){

						temp.bg=true;
						cmd.push_back(temp);
						temp.string_command="";
					}else{
						temp.mul_type=5;
						cmd.push_back(temp);
						temp.string_command = "";
						temp.bg = false;

						t = And;
					}
				}else if(input_string[i]=='|'){

					if(i==input_string.size()-1){
						cout<< "command argument not completed!" << endl;
						return false;
					}
					temp.mul_type=6;
					cmd.push_back(temp);
					temp.string_command = "";
					temp.bg = false;

					t =Or;
				}
				else{

					temp.string_command += input_string[i];
				}

				break;

			case Redirect:

				if(input_string[i] == ';'){

					temp.mul_type=4;
					cmd.push_back(temp);
					temp.string_command = "";
					temp.bg = false;

					t = Start;
				}else if(input_string[i] == '&'){

					if(i==input_string.size()-1){

						temp.bg=true;						
						cmd.push_back(temp);
						temp.string_command="";
					}else{
						temp.mul_type=5;
						cmd.push_back(temp);
						temp.string_command = "";
						temp.bg = false;

						t = And;
					}
				}else if(input_string[i]=='|'){

					if(i==input_string.size()-1){
						cout<< "command argument not completed!" << endl;
						return false;
					}
					temp.mul_type=6;
					cmd.push_back(temp);
					temp.string_command = "";
					temp.bg = false;

					t =Or;
				}
				else{

					temp.string_command += input_string[i];
				}

				break;

			case Multi:

				break;

			// &&
			case And:

				if(input_string[i]== '&' && i !=(input_string.size()-1) ){ t=Start;}
				else {

					cout<< "command && can not run successfully!" << endl;
					return false;
				}

				break;

			// ||
			case Or:

				if(input_string[i]=='|' && i !=(input_string.size()-1) ){t=Start;}
				else{

					cout<< "command || can not run sucessfully!" << endl;
					return false;
				}

				break;

			// '' single quotation marks
			case Quote1:

				if(input_string[i]=='\''){

					t=Inner;	
				}
				else{

					temp.string_command += input_string[i];
					if(i==input_string.size()-1){
						cout << "command not complete!" << endl;
						return false;
					}
				}

				break;

			// "" double quotation marks
			case Quote2:

				if(input_string[i]=='"'){

					t=Inner;	
				}
				else{

					temp.string_command += input_string[i];
					if(i==input_string.size()-1){
						cout << "command not complete!" << endl;
						return false;
					}
				}

				break;

		}

	}


	/***
	  Decide which cmd could be stored in vector
	 ***/
	if(temp.string_command !=""){
		cmd.push_back(temp);
	}

	return true;   
}
