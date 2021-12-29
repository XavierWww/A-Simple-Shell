##  A Simple Shell

##	Author 
Xiang WU

##	1.Achieved Features I

Our OS shell currently only supports Phase I, but it has implemented all built-in commands (which are to be handled by the shell itself) and executed any ohter external programs.

For example: ls, mkdir, who, whoami, cp, mv, rm, rmdir, cal, chmod, pwd, 
             go(go command is used to switch the current working directory to dirName (directory parameter).),
             exit, pid(displays the pids of the 5 recent child processes), history, etc. 
			 
In addition, OS Shell can also parse these parameters, such as: ls -a , ls -al , ls -l ,etc and execute any other external programs, e.g., "./hello"

Finally, our team also completed Bonus Part. When the shell is called, OS Shell will display the host name and current working directory.


##	2.Achieved Features II
At present, our team has completed Phase II on the basis of Phase I, which can not only support Internal commands, but also support Shell constructs (I/O Redirection and Pipes) and multiple commands(;).

The redirection is represented by "<" ">" ">>", and the pipe is represented by "|";

Redirection: ">" ">>" The difference between these two is that ">" is redirecting to a file, ">>" is appending content to the file, and the file is created if the file does not exist;
             "<" is changing from standard input to Enter from the designated place.

Pipe: command 1 | command 2 (command 1 as stdout, sent to command 2 stdin)

multiple commands：You can use multiple commands by splitting different commands with a semicolon(;);for example:mkdir ddd; ls > text.txt; mv text.txt ddd.

In addition, our team also completed Bonus Part. Our OS shell current support “AND” Operator (&&) and “OR” Operator (||), even "multi-And" and "multi-Or".
For example, when you inpout ls && echo "<< Success >>" in the OS shell, The OS shell will output the contents of the specified working directory and << Success >>.
However, our shell currently only supports one of [ multi(;), and(&&), or(||) ] three cases, mixed executing inner-command, defined-command, pipe and redirection, but does not support multi(;), and(&&), or(||) nesting with each other.


##	3.Achieved Features III
Our team completed Phase III (jobs control) based on Phase I and Phase II. Currently the OS shell can support internal commands, constructs (I / O Redirection and Pipes), multiple commands and job control.

Basic job controls include six commands (Ctrl-c, Crtl-z, bg, fg, continue, jobs. 
Ctrl-c is used to terminate the job, Crtl-z is used to temporarily suspend the running Frontground command.
The bg command is used to put the suspended job to background. 
The fg command is used to transfer the commands in the background to the foreground and continue to run. 
The jobs command displays all the current running jobs. 
The continue command lets the suspended job continue to run.

Attention:
The operation of job control does not consider multiple commands for the time being. 
The shell command line prompt will automatically clear the termed jobs, which may cause the terminated jobs to not be displayed in time.
Ctrl-c will directly end the current job without displaying the job information.
Ctrl-c and Ctrl-z will not affect the background of running jobs.
No zoombie generated in our shell.


##	4.How to compile and run code
In the folder, there is a makefile. Our group said that the compilation process is written in the makefile, just use the "make" command to automatically compile and generate an executable file.
Then you can execute the file in the OS Shell through "./project3". When you enter the our "OS server", you can check and test it.

If you have any problems, please contact us in time! Thank you.
