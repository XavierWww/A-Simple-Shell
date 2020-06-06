#ifndef Tool_Func_H
#define Tool_Func_H

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

#include "enum.h"

using namespace std;

char *shell_prompt();
char **read_command(char *input);
bool parser(string input_string, command& temp, vector<command>& cmd, state t);
void multi_cmd(vector<command>& cmd, vector<string>& h, vector<int>& p, char *path, vector<Job>& jobs);
void single_cmd(vector<command>& cmd, vector<string>& h, vector<int>& p, char *path, vector<Job>& jobs, Job& newjob, int& count, int& g_pid, vector<Job>& t_jobs);

#endif
