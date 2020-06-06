project3: project3.o Tool_Func.o define_cmd.o execute_cmd.o parser.o jobs.o
	g++ -Wall -o project3 project3.o Tool_Func.o define_cmd.o execute_cmd.o parser.o jobs.o

project3.o: project3.cpp 
	g++ -c -Wall project3.cpp -o project3.o 

Tool_Func.o: Tool_Func.cpp Tool_Func.h
	g++ -c -Wall Tool_Func.cpp -o Tool_Func.o

define_cmd.o: define_cmd.cpp define_cmd.h 
	g++ -c -Wall define_cmd.cpp -o define_cmd.o

execute_cmd.o: execute_cmd.cpp execute_cmd.h 
	g++ -c -Wall execute_cmd.cpp -o execute_cmd.o

parser.o: parser.cpp Tool_Func.h 
	g++ -c -Wall parser.cpp -o parser.o

jobs.o: jobs.cpp jobs.h
	g++ -c -Wall jobs.cpp -o jobs.o

clean:
	rm -rf *.o project3
