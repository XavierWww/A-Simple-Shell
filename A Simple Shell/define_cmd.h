#ifndef define_cmd_H
#define define_cmd_H

#include<iostream>
#include<stdlib.h>
#include<string.h>
#include<vector>
#include<stack>
#include<unistd.h>
#include<stdio.h>

#include "enum.h"

using namespace std;

void def_history(vector<string>& h);
void def_pid(vector<int>& p);
void def_go(vector<command>& cmd, char* path, int i, vector<Job>& jobs);

#endif

