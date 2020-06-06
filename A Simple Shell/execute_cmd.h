#ifndef execute_cmd_H
#define execute_cmd_H

#include<iostream>
#include<stdlib.h>
#include<string.h>
#include<vector>
#include<stack>
#include<unistd.h>
#include<stdio.h>

#include "enum.h"

using namespace std;


bool exec_pipe(char *input);
bool exec_redirect(char *input);
bool exec_inner(char *input, char *path, vector<string>& h, vector<int>& p, vector<command>& cmd);

#endif

