#include <iostream>
#include <stdlib.h>
#include <string>
#include <unistd.h>
#include <sstream>
#include <fstream>
#include <pthread.h>
#include <queue>
#include <algorithm>

#define NUM_THREADS 3

using namespace std;

string output_path; //  Holds the output path in global for accessing it in threads
int num_of_clients; //  Number of clients that will be read from file
pthread_mutex_t teller_mutex = PTHREAD_MUTEX_INITIALIZER;   //  For locking critical sections in teller threads
pthread_mutex_t client_mutex = PTHREAD_MUTEX_INITIALIZER;   //  For locking critical sections in client threads
pthread_mutex_t print_mutex = PTHREAD_MUTEX_INITIALIZER;    //  For locking printing statements in teller threads
int *seat_array;    //  Seat array that holds whether it is reserved or not
pthread_t teller_thread[NUM_THREADS];   //  Holds teller thread ids
int countt = 0; //  Used in while loop to iterate number of client times
int salon_size; //  Stores theater room size based on theater hole
int teller_A = 1;   //  For synchronizing tellers which one is available when client comes if available 1 else 0
int teller_B = 1;   //  For synchronizing tellers which one is available when client comes if available 1 else 0
int teller_C = 1;   //  For synchronizing tellers which one is available when client comes if available 1 else 0

//  Stores every clients information
typedef struct {
    pthread_t thread_id;
    string  name;   //  Client name
    int arrival;    //  Client arrival time
    int service;    //  Client service time
    int seat;       //  Clients seat number
}client_data;

queue<client_data*> waiting_queue;  //  Stores the clients that are waiting for a service

//  teller thread function
void *teller_thread_func(void *param){

    string signed_teller;   // For printing which teller signs the ticket
    //  teller thread 0 stores Teller A
    if(pthread_self() == teller_thread[0]){
        signed_teller = "A";
    }
    //  teller thread 1 stores Teller B
    if(pthread_self() == teller_thread[1]){
        signed_teller = "B";
    }
    //  teller thread 2 stores Teller C
    if(pthread_self() == teller_thread[2]){
        signed_teller = "C";
    }
    ofstream output(output_path,ios::app);  //  opens the output file where it left
    output << "Teller " << signed_teller << " has arrived." << endl;
    output.close();

    //  Executes number of client times for processing each clients requests
    while (countt < num_of_clients){

        //  locks the thread to prevent inconsistent results because it deals with global variables
        pthread_mutex_lock(&teller_mutex);
        //  if all clients requests handled then stop
        if(countt == num_of_clients){
            pthread_mutex_unlock(&teller_mutex);
            break;
        }
        //  waits for clients to arrive the queue
        while (waiting_queue.empty()){

        }
        //  if Teller A is available and this is Teller B then do nothing and make sure Teller A gets the client
        if(pthread_self() == teller_thread[1] && teller_A == 1){
            pthread_mutex_unlock(&teller_mutex);
            continue;
        }
        //  if Teller A or Teller B is available and this is Teller C then do nothing and make sure Teller A or Teller B gets the client
        if(pthread_self() == teller_thread[2] && (teller_A == 1 || teller_B == 1)){
            pthread_mutex_unlock(&teller_mutex);
            continue;
        }
        //  if Teller A gets the client make Teller A look busy
        if(pthread_self() == teller_thread[0]){
            teller_A = 0;
        }
        //  if Teller B gets the client make Teller B look busy
        if(pthread_self() == teller_thread[1]){
            teller_B = 0;
        }
        //  if Teller C gets the client make Teller C look busy
        if(pthread_self() == teller_thread[2]){
            teller_C = 0;
        }

        client_data *current_client;    //  stores the current clients data
        current_client = waiting_queue.front(); //  gets the client from the front of the queue
        waiting_queue.pop();    //  pops the client from queue
        countt++;   //  increments processed clients number
        int requested_seat = current_client->seat;  //  takes the requested seat number from client
        int reserved_seat = 0;  //  stores the reserved seat number after checking conditions

        //  if requested seat number is valid and it is not reserved before it reserves the seat for this client
        if(requested_seat <= salon_size && seat_array[requested_seat] == 0){
            reserved_seat = requested_seat;
            seat_array[requested_seat] = 1;
        }
        //  if requested seat number reserved before this client than it gives smallest available seat number to ths client
        else{

            for (int i = 1; i <= salon_size; i++) {
                if(seat_array[i] == 0){
                    reserved_seat = i;
                    seat_array[i] = 1;
                    break;
                }
            }
        }
        //  releases the lock
        pthread_mutex_unlock(&teller_mutex);
        //  it waits the teller for service time to make ticketing work
        usleep(current_client->service*1000);
        //  it makes Teller A available again
        if(pthread_self() == teller_thread[0]){
            teller_A = 1;
        }
        //  it makes Teller B available again
        if(pthread_self() == teller_thread[1]){
            teller_B = 1;
        }
        //  it makes Teller C available again
        if(pthread_self() == teller_thread[2]){
            teller_C = 1;
        }

        pthread_mutex_lock(&print_mutex);
        //  if there is no available seat in the theatre hole than it prints none for seat number
        if(reserved_seat == 0){
            ofstream output(output_path,ios::app);
            output << current_client->name << " requests seat " << requested_seat << ", reserves None. Signed by Teller " << signed_teller << "." << endl;
            output.close();
        }
        else{
            ofstream output(output_path,ios::app);
            output << current_client->name << " requests seat " << requested_seat << ", reserves seat " << reserved_seat << ". Signed by Teller " << signed_teller << "." << endl;
            output.close();
        }
        pthread_mutex_unlock(&print_mutex);

    }
    pthread_exit(NULL);
}
//  client thread function. Takes client data as parameter
void *client_thread_func(void *param){

    client_data *my_data = (client_data *)param;    //  stores client data
    usleep(my_data->arrival*1000);  //  sleeps client for arrival time to the queue

    pthread_mutex_lock(&client_mutex);

    waiting_queue.push(my_data);    //  pushes client to the queue after waiting for their arrival time

    pthread_mutex_unlock(&client_mutex);

    pthread_exit(NULL);
}

int main(int argc, char *argv[]) {

    string input_path = argv[1];    //  stores input file path
    output_path = argv[2];          //  stores output file path

    ifstream input_file(input_path);    // opens input file for reading
    string salon;   // stores which theatre hole is currently processing
    getline(input_file,salon);
    std::for_each(salon.begin(), salon.end(), [](char & c) {
        c = ::toupper(c);
    });
    //  if it is ODA TIYATROSU then set salon_size to 60
    if(salon.substr(0,1) == "O"){
        salon_size = 60;
    }
        //  if it is USKUDAR STUDYO SAHNE then set salon_size to 80
    else if (salon.substr(0,1) == "U"){
        salon_size = 80;
    }
        //  if it is KUCUK SAHNE, then set salon_size to 200
    else if (salon.substr(0,1)  == "K"){
        salon_size = 200;
    }
    seat_array = new int[salon_size+1]; //  creates seats with the number of salon_size
    //  initializes all seats with zero which says available
    for (int i = 1; i <= salon_size; i++) {
        seat_array[i] = 0;
    }

    string temp;
    getline(input_file,temp);
    num_of_clients = stoi(temp);    //  gets the number of cients
    ofstream output_file(output_path);
    output_file << "Welcome to the Sync-Ticket!" << endl;
    output_file.close();

    client_data client_array[num_of_clients];   //  creates client array

    for (int i = 0; i < num_of_clients; i++){
        string client, name, arrival, service, seat;
        getline(input_file,client);
        stringstream delimstr(client);
        getline(delimstr,name,',');
        client_array[i].name = name;    //  stores client name
        getline(delimstr,arrival,',');
        client_array[i].arrival = stoi(arrival);    //  stores client arrival time
        getline(delimstr,service,',');
        client_array[i].service = stoi(service);    //  stores client service time
        getline(delimstr,seat,',');
        client_array[i].seat = stoi(seat);          //  stores clients requested seat number
    }

    input_file.close(); //  closes input file. Reading is done.

    pthread_attr_t attr;
    pthread_attr_init(&attr);   // initializes thread attributes

    pthread_t client_thread[num_of_clients];    //  creates client thread array for all clients
    pthread_attr_t cattr;                       //  initializes thread attributes
    pthread_attr_init(&cattr);

    //  creates teller threads and sends them to teller thread function
    for(long a = 0; a < NUM_THREADS; a++) {
        pthread_create(&teller_thread[a], &attr, teller_thread_func, NULL);
        //usleep(100);    //  sleeps a bit for synchronization
    }
    //  creates clients thread and send them to client thread function with current client data
    for(long a = 0; a < num_of_clients; a++) {
        pthread_create(&client_thread[a], &cattr, client_thread_func, (void *) &client_array[a]);
        client_array[a].thread_id = client_thread[a];
    }
    //  waits for all clients to finish their execution
    for(int i = 0; i < num_of_clients; i++){
        pthread_join(client_thread[i],NULL);
    }
    //  waits for all tellers to finish their execution
    for(int i = 0; i < NUM_THREADS; i++){
        pthread_join(teller_thread[i],NULL);
    }

    output_file.open(output_path,ios::app);
    output_file << "All clients received service." << endl;
    output_file.close();

    //  destroys all mutexes
    pthread_mutex_destroy(&client_mutex);
    pthread_mutex_destroy(&teller_mutex);
    pthread_mutex_destroy(&print_mutex);

    delete[] seat_array;    //  deletes array from heap to prevent memory leak
    return 0;
}

