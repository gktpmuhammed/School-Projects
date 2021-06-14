
#include <iostream>
#include <string>
#include <unistd.h>
#include <csignal>
#include <sstream>
#include <fstream>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <sys/wait.h>

using namespace std;
int processNum;         /*!< Number of processes */
string process_output;  /*!< Path of the process output file */
string watchdog_output; /*!< Path of the watcgog output file */
struct timespec delta = {0 /*secs*/, 300000000 /*nanosecs*/}; /*!< 0.3 sec */

/*When the instruction.txt file read completely and watchdog kill signal sended it prints necessary statement and exits program.
*/
void exit_signal(int sig){
    ofstream watchdog_output_file; // Out stream object for writing watchdog output to file.
    watchdog_output_file.open(watchdog_output,ios::app);    // Opening the stream object. 
    watchdog_output_file << "Watchdog is terminating gracefully" << endl;   // Writing the output to the file. 
    watchdog_output_file.close();   // Closing stream to prevent inconvinient results. 
    exit(sig);  // Exits program when it gets SIGTERM signal.
}

/*! Main part of the program. Process fork's are made in here. First it takes arguments and opens file streams for printing output.
Then, it creates the number of processes given in argument. After creating processes it waits for a child to be killed. When a child killed by executor,
watchdog detects and reinitialize it. If killed process is the head which is P1 than it kills all the processes first and then reinitialize them.
If killed process is not the head, then it first finds which process died and kills the process then reinitialize it. Also watchdog waits for its SIGTERM signal.
When it comes, it stop execution and exits the program.
*/
int main(int argc, char *argv[]){
    
    processNum = stoi(argv[1]);         // Getting process number from command line argument.
    process_output = argv[2];           // Getting process output file's path from argument.
    watchdog_output = argv[3];          // Getting watchdog output file's path from argument.
    pid_t pidArray[processNum+1] = {};  // Creating pid_t array for storing pid values of processes.

    ofstream watchdog_output_file;      // Out stream object for writing watchdog output to file.
    watchdog_output_file.open(watchdog_output); // Opening the stream object.

    
    ofstream process_output_file;// I used this initialization for clearing process_output file before each execution else open(ios::app) causes a problem.
    process_output_file.open (process_output); // Opening the stream object.

    pid_t watchdog_pid = getpid();  // Getting watcdog pid value.
    pidArray[0] = watchdog_pid;     // Adding the pid of watcdog to 0 index of the array.
    
    string w_info = "P0 " + to_string(watchdog_pid);    // Concatenation of P0 and <pid_value>.
    char* watchdog_pid_info = (char*) w_info.c_str();   // To giving the appropriate parameter type I changed the type to char.

    int newPp = open("/tmp/myfifo",O_WRONLY);           // Opening the named pipe.
    write(newPp,watchdog_pid_info,strlen(watchdog_pid_info)+1); // Writing the pid value to pipe.

    pid_t childpid; // Holds the pid values of newly forked processes.
    /*! First initializiation of processes. */
    for(int i = 1; i <= processNum; i++){
        /*! If pid value equals to zero which means child process call execl function for each process */
        if((childpid = fork()) == 0){
            char* index = (char*) to_string(i).c_str();
            char* out_process = (char*) process_output.c_str(); // Converting string to char for passing right argument to the function.
            execl("./process","process",index,out_process,NULL);    // Process initialization.
            break;
        }
        /*! If pid greater than 0 which means parent and in these case watchdog, it conntinues to create more processes. */
        else if(childpid > 0){
            pidArray[i] = childpid; // Holding the pids of processes.
            watchdog_output_file << "P" << i << " is started and it has a pid of " << childpid << endl;// Printing output file the status.
            string p_info = "P" + to_string(i) + ' ' + to_string(childpid); // String concatenation.
            char* process_pid_info = (char*) p_info.c_str();                // Changing the type to char. 
            nanosleep(&delta,&delta);                                       // To prevent mixed execution.
            write(newPp,process_pid_info,strlen(process_pid_info)+1);       // Sends the pid information to executor.
        }
    }

    signal(SIGTERM,exit_signal);// When the instruction.txt file read completely and watchdog kill signal sends it prints necessary statement and exits program.
    
    pid_t returned_pid; // It holds the pid values for killed processes.

    /*! While prograam execution continue it waits for a SIGTERM call when it gets the signal it creates new processes for killed ones. 
    */
    while(true){
        
        returned_pid = wait(NULL);  // If a precess dies it returns the pid value to returned_pid.

        /*! If died process is P1 which is head than kill all other processes and create P1 again */
        if(returned_pid == pidArray[1]){
            watchdog_output_file << "P1 is killed, all processes must be killed\nRestarting all processes" << endl; // Printing output file the status.
            /*! For killing other processes. */
            for (int i = 2; i <= processNum; i++){
                kill(pidArray[i],SIGTERM);  // Kills processes with SIGTERM signal.
                nanosleep(&delta,&delta);   // To prevent mixed execution.
            }
            
            /*! Forking new processes to replace killed ones from watchdog. */
            for (int i = 1; i <= processNum; i++){
                /*! If pid value equals to zero which means child process call execl function for each process. */
                if((childpid = fork()) == 0){
                    char* index = (char*) to_string(i).c_str();
                    char* out_process = (char*) process_output.c_str(); // Converting string to char for passing right argument to the function.
                    execl("./process","process",index,out_process,NULL);    // Process initialization.
                    break;
                }
                /* If pid greater than 0 which means parent and in these case watchdog it conntinues to create more processes. */
                else if(childpid > 0){
                    pidArray[i] = childpid; // Holding the pids of processes.
                    watchdog_output_file << "P" << i << " is started and it has a pid of " << childpid << endl;// Printing output file the status.
            
                    string p_info = "P" + to_string(i) + ' ' + to_string(childpid); // String concatenation.
                    char* process_pid_info = (char*) p_info.c_str();                // Changing the type to char. 
                    nanosleep(&delta,&delta);                                       // To prevent mixed execution.
                    write(newPp,process_pid_info,strlen(process_pid_info)+1);       // Sends the pid information to executor.
                }
            }
        }
        /*! If died process is other than P1 create P<x> again */
        else{
            /*! This for finds which process was killed and replace it with new process by using fork. */
            for(int b = 2; b <= processNum; b++){
                /*! Finds the index of the killed process. */
                if(returned_pid == pidArray[b]){
                    watchdog_output_file << "P" << b << " is killed\n" << "Restarting P" << b << endl;// Printing output file the status.
                    /*! If pid value equals to zero which means child process call execl function for each process. */
                    if((childpid = fork()) == 0){
                        string strIn = to_string(b);
                        char* index = (char*) strIn.c_str();     // Converting string to char for passing right argument to the function.
                        char* out_process = (char*) process_output.c_str();
                        execl("./process","process",index,out_process,NULL);    // Process initialization.
                        break;
                    }
                    /* If pid greater than 0 which means parent and in these case watchdog it conntinues to create more processes. */
                    else if(childpid > 0){
                        pidArray[b] = childpid; // Holding the pids of processes.
                        watchdog_output_file << "P" << b << " is started and it has a pid of " << childpid << endl; // Printing output file the status.
            
                        string p_info = "P" + to_string(b) + ' ' + to_string(childpid);
                        char* process_pid_info = (char*) p_info.c_str();            // Converting string to char for passing right argument to the function.
                        nanosleep(&delta,&delta);                                   // To prevent mixed execution.
                        write(newPp,process_pid_info,strlen(process_pid_info)+1);   //sends the pid information to executor.
                    }
                }
            }
        }
    }    
    return 0;
}

