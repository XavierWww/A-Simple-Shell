#ifndef jobs_H
#define jobs_H

#include "enum.h"

void job_disp(Job& newjob);
void jobs_exec(vector<Job>& jobs,vector<command> cmd,string input_string,int& g_pid);

#endif

