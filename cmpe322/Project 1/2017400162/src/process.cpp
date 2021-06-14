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
struct timespec delta = {0 /*secs*/, 300000000 /*nanosecs*/}; /*!< 0.3 sec */
int id;                 /*!< It stores the id of processes which is P<id>. */
string process_output;  /*!< It stores process output file path. */

/*! This method handles signals for processes. For every signal it prints different outputs to file. */
void coming_signal(int sig){
    ofstream process_output_file;   // Out stream object for writing process output to file.
    process_output_file.open (process_output,ios::app); // Opening the stream object in append mode.
    /*! If process gets SIGHUP signal then it prints given message. */
    if(sig == 1){
        process_output_file << "P" << id << " received signal " << sig << endl;
    }
    /*! If process gets SIGINT signal then it prints given message. */
    else if(sig == 2){
        process_output_file << "P" << id << " received signal " << sig << endl;
    }
    /*! If process gets SIGILL signal then it prints given message. */
    else if(sig == 4){
        process_output_file << "P" << id << " received signal " << sig << endl;
    }
    /*! If process gets SIGTRAP signal then it prints given message. */
    else if(sig == 5){
        process_output_file << "P" << id << " received signal " << sig << endl;
    }
    /*! If process gets SIGFPE signal then it prints given message. */
    else if(sig == 8){
        process_output_file << "P" << id << " received signal " << sig << endl;
    }
    /*! If process gets SIGSEGV signal then it prints given message. */
    else if(sig == 11){
        process_output_file << "P" << id << " received signal " << sig << endl;
    }
    /*! If process gets SIGTERM signal then it prints given message. And terminates the process.*/
    else if(sig == 15){
        process_output_file << "P" << id << " received signal " << sig << ", terminating gracefully"<< endl;
        exit(sig);
    }
    /*! If process gets SIGXCPU signal then it prints given message. */
    else if(sig == 24){
        process_output_file << "P" << id << " received signal " << sig << endl;
    }
    
    process_output_file.close();    // Closing stream to prevent inconvinient results. 
}

/*! This main initializes each process. First it takes arguments from execl() function written in watchdog. Then opens the output stream writes given message
After that it goes to sleep mode and waits for signals. */
int main(int argc, char *argv[]){
    id = atoi(argv[1]); // It stores the id of the process which is P<id>.
    process_output = argv[2];   // It stores the path of the output file.
    ofstream process_output_file;   // Out stream object for writing process output to file.
    process_output_file.open (process_output,ios::app); // Opening the stream object in append mode.
    process_output_file << "P" << id << " is waiting for a signal" << endl;
    process_output_file.close();    // Closing stream to prevent inconvinient results. 

    /*! If process gets SIGHUP signal then it goes to coming_signal function with the SIGHUP parameter . */
    signal(SIGHUP,coming_signal);
    /*! If process gets SIGINT signal then it goes to coming_signal function with the SIGINT parameter . */
    signal(SIGINT,coming_signal);
    /*! If process gets SIGILL signal then it goes to coming_signal function with the SIGILL parameter . */
    signal(SIGILL,coming_signal);
    /*! If process gets SIGTRAP signal then it goes to coming_signal function with the SIGTRAP parameter . */
    signal(SIGTRAP,coming_signal);
    /*! If process gets SIGFPE signal then it goes to coming_signal function with the SIGFPE parameter . */
    signal(SIGFPE,coming_signal);
    /*! If process gets SIGSEGV signal then it goes to coming_signal function with the SIGSEGV parameter . */
    signal(SIGSEGV,coming_signal);
    /*! If process gets SIGTERM signal then it goes to coming_signal function with the SIGTERM parameter . */
    signal(SIGTERM,coming_signal);
    /*! If process gets SIGXCPU signal then it goes to coming_signal function with the SIGXCPU parameter . */
    signal(SIGXCPU,coming_signal);

    /*! To keep process idle. Sleep function does not worked here. I don't know why. */
    while(true){
        
    }
}

