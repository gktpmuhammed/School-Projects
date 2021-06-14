/*
Student Name: Muhammed Göktepe
Student Number: 2017400162
Compile Status: Compiling
Program Status: Working
Notes: Anything you want to say about your code that will be helpful in the grading process.
*/
#include <iostream>
#include <mpi.h>
#include <stdlib.h>
#include <string>
#include <unistd.h>
#include <sstream>
#include <fstream>
#include <vector>
#include <limits>
#include <algorithm> // for sort function
#include <set>

using namespace std;
// These are the parameters that will be read from the input file
int num_of_process_from_file, instance_number, num_of_features, num_of_iterations, resulting_num_of_features;
int num_of_lines_per_proc;      //  number of instance that will be executed in each processor
int sub_size;                   //  size of the array that will be store the features and class variables in one processor
int whole_size;                 //  size of the array that will be store the features and class variables in all processors

//  comparator for sorting weight vector
bool sortValue(const pair<int,float> &a, const pair<int, float> &b){
    return (a.second > b.second);
}

int main(int argc, char *argv[]){

    int rank;
    int num_process;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &num_process);

    int num_of_slaves = num_process - 1;                    // number of slave processors
    string input_path = argv[1];                            // reading input file path from arguments
    ifstream input_file(input_path);                        // creating input file stream object
    string inst, token1, token2, token3, token4;            // temporary variables for storing data read from input file
    getline(input_file, inst);                        // getting the first line in input file which is the number of processors
    stringstream splitline(inst);                           // parsing the line
    splitline >> token1;                                    // storing value in a temporary variable
    num_of_process_from_file = stoi(token1);                // converting the value from string to int
    getline(input_file, inst);                        // getting the second line which have four value
    stringstream splitline2(inst);                          // parsing the line
    splitline2 >> token1 >> token2 >> token3 >> token4;     // storing values in a temporary variables
    instance_number = stoi(token1);                         // stores the instance number which is the number of lines that will be read from input file
    num_of_features = stoi(token2);                         // stores the feature number
    num_of_iterations = stoi(token3);                       // stores the number of iterations for calculating weight
    resulting_num_of_features = stoi(token4);               // stores the number of features that is asked by the program
    num_of_lines_per_proc = instance_number / num_of_slaves;// number of instance that will be executed in each processor
    sub_size = num_of_lines_per_proc * (num_of_features + 1);                       //  size of the array that will be store the features and class variables in one processor
    whole_size = (instance_number + num_of_lines_per_proc) * (num_of_features + 1); //  size of the array that will be store the features and class variables in all processors

    // for sending all necessary parameters to all processors
    MPI_Bcast(&num_of_iterations, 1, MPI_INT, 0, MPI_COMM_WORLD); // broadcast
    MPI_Bcast(&resulting_num_of_features, 1, MPI_INT, 0, MPI_COMM_WORLD); // broadcast
    MPI_Bcast(&num_of_lines_per_proc, 1, MPI_INT, 0, MPI_COMM_WORLD); // broadcast
    MPI_Bcast(&num_of_features, 1, MPI_INT, 0, MPI_COMM_WORLD); // broadcast
    MPI_Bcast(&instance_number, 1, MPI_INT, 0, MPI_COMM_WORLD); // broadcast

    float array[whole_size];        // stores the whole input data in this array
    float newArray[sub_size];       // partitioned data for each processor stored in this array
    // if it is the master process than start read file.
    if(rank == 0){
        for(int a = 0; a < sub_size; a++){
            array[a] = 0;   // initializing arrays' first sub_size part with 0's for root processor
        }
        // initialize array with input data
        for(int i = sub_size; i < whole_size; i++){
            input_file >> token1;
            float temp = stof(token1);
            array[i] = temp;
        }
        input_file.close();
    }
    // this function partitioned all data equally to all processor
    MPI_Scatter(array,sub_size,MPI_FLOAT,newArray,sub_size,MPI_FLOAT,0,MPI_COMM_WORLD);


    int masterSignal = 1; // for stopping loop when root process executes
    while(masterSignal){

        int master_sorted_array[resulting_num_of_features * num_of_slaves]; // stores the features that have bigger weights in each processor together
        int sorted_array[resulting_num_of_features];                        // stores the selected features indexes in sorted order for printing
        // if the processor is not root
        if(rank!= 0){

            float weight_array[num_of_features] = {0};  // initializes weight array with zero
            float max_array[num_of_features] = {0};     // stores max values of each feature in given instances
            float min_array[num_of_features] = {0};     // stores min values of each feature in given instances
            vector<pair<int,float>> sorted_weights;     // stores selected features which have bigger weights after calculation with their id's in sorted order

            //this loop will be executed number of iteration times
            for(int i = 0; i < num_of_iterations; i++){
                float one_line_array[num_of_features+1];  // stores target instance
                // for taking target instance from partitioned array and initialize max and min array with default value
                for(int k = 0; k < num_of_features+1; k++){
                    one_line_array[k] = newArray[i*(num_of_features+1)+k];
                    max_array[k] = newArray[i*(num_of_features+1)+k];
                    min_array[k] = newArray[i*(num_of_features+1)+k];
                }

                int hit_line = 0;       // stores nearest hit feature's id
                int miss_line = 0;      // stores nearest miss feature's id
                float hit_sum = numeric_limits<float>::max();   // for calculating and comparing manhattan distance sum for each instance when there is a hit
                float miss_sum = numeric_limits<float>::max();  // for calculating and comparing manhattan distance sum for each instance when there is a miss
                // this loop will be executed number of instance that each processor has and finds nearest miss and hit
                for(int k = 0; k < num_of_lines_per_proc; k++){
                    // to prevent comparing the instance with itself
                    if(i != k){
                        float sum = 0;      // stores the manhattan distance between two instances
                        // finds manhattan distance between two instances
                        for(int a = 0; a < num_of_features; a++){
                            sum += abs(one_line_array[a] - newArray[k * (num_of_features + 1) + a]);
                            // for updating max value for each feature
                            if(newArray[k * (num_of_features + 1) + a] > max_array[a]){
                                max_array[a] = newArray[k * (num_of_features + 1) + a];
                            }
                            // for updating min value for each feature
                            if(newArray[k * (num_of_features + 1) + a] < min_array[a]){
                                min_array[a] = newArray[k * (num_of_features + 1) + a];
                            }
                        }
                        //if these two instances are in same class (hit)
                        if(one_line_array[num_of_features] == newArray[((k+1) * (num_of_features+1)) - 1]){
                            if(sum < hit_sum){
                                hit_sum = sum;
                                hit_line = k;
                            }
                        }
                        // if these two instances are not in same class (miss)
                        else {
                            if(sum < miss_sum){
                                miss_sum = sum;
                                miss_line = k;
                            }
                        }
                    }
                }
                // updates weight array and implementation of diff function
                for(int b = 0; b < num_of_features; b++){
                    float diff_hit = abs(one_line_array[b]-newArray[hit_line * (num_of_features+1) + b]) / (max_array[b]-min_array[b]);
                    diff_hit /= num_of_iterations;
                    float diff_miss = abs(one_line_array[b]-newArray[miss_line * (num_of_features+1) + b]) / (max_array[b]-min_array[b]);
                    diff_miss /= num_of_iterations;
                    weight_array[b] -= diff_hit;
                    weight_array[b] += diff_miss;
                }
            }
            // to sort these features with respect to their weights, they are stored in vector with pairs
            for(int i = 0; i < num_of_features; i++){
                sorted_weights.push_back(make_pair(i,weight_array[i]));
            }
            // sorts vector by their weights
            sort(sorted_weights.begin(),sorted_weights.end(),sortValue);

            cout << "Slave P" << rank << " :" ;
            // storing the id's of selected features with bigger weights
            for(int i = 0; i < resulting_num_of_features; i++){
                sorted_array[i] = sorted_weights[i].first;
            }
            // sorting the id's of selected features for printing
            sort(sorted_array,sorted_array + resulting_num_of_features);
            // for printing selected features
            for(int i = 0; i < resulting_num_of_features; i++){
                cout << " " << sorted_array[i];
            }
            cout << endl;

        }
        // this function combines all selected features in the root
        MPI_Gather(sorted_array,resulting_num_of_features,MPI_INT,master_sorted_array,resulting_num_of_features,MPI_INT,0,MPI_COMM_WORLD);

        // if the processor is root
        if(rank==0){
            masterSignal=0; // sends stop signal
            set<int, greater<int> > sorted_set; // for eliminating duplicate features which are sended by slave processors
            // for eliminating duplicate features which are sended by slave processors
            for(int i = resulting_num_of_features; i < resulting_num_of_features * num_of_process_from_file; i++){
                sorted_set.insert(master_sorted_array[i]);
            }
            int last_size = sorted_set.size();      // stores the number of distinct features that will be printed
            int last_sorted_array[last_size];       // final look of the feature id's

            set<int, greater<int> >::iterator itr;  // iterator for iteration over a set
            int i = 0;
            for (itr = sorted_set.begin(); itr != sorted_set.end(); ++itr){
                last_sorted_array[i] = *itr;
                i++;
            }
            // sorts the selected features with increasing order
            sort(last_sorted_array,last_sorted_array+last_size);
            // prints the selected features to the console
            cout << "Master P0 :";
            for(int i = 0; i < last_size; i++){
                cout << " " << last_sorted_array[i];
            }
        }
        // sends the stop signal to processes
        MPI_Bcast(&masterSignal, 1, MPI_INT, 0, MPI_COMM_WORLD); // broadcast

    }

    MPI_Barrier(MPI_COMM_WORLD); // synchronizing processes
    MPI_Finalize();
    return(0);
}