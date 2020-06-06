#ifndef enum_H
#define enum_H

#include<iostream>
#include<stdlib.h>
#include<string.h>
#include<vector>
#include<stack>
#include<unistd.h>
#include<sys/wait.h>
#include<sys/types.h>
#include<sys/io.h>
#include<stdio.h>
#include<sys/errno.h>
#include<fcntl.h>
#include<err.h>
#include<sstream>
#include<cstddef>

using namespace std;

enum state {Start, Inner, Pipe, Redirect, Multi, And, Or, Quote1, Quote2};


class command{

	public:

		string string_command;
		int mul_type; // multi-command type
		int type;     // single command type
		bool bg;	  // Is in background or not

};                

class Job{
	
	public:
		
		int pid;
		int jid;
		string cmd_line;
	
		bool Running;
		bool Terminated;
		bool Stopped;
		
};

#endif

