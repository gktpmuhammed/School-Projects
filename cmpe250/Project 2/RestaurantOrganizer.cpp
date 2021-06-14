/*
Student Name:
Student Number:
Project Number: 2
Compile Status: [SUCCESS/FAIL]
Running Status: [SUCCESS/FAIL]
Notes: Anything you want to say about your code that will be helpful in the grading process.
*/
#include "RestaurantOrganizer.h"

using namespace std;

//Method for changing the two values in the arrays
void swapp(int *param, int index, int secIndex);

RestaurantOrganizer::RestaurantOrganizer(const vector<int>& tableCapacityInput){
    numberOfTables = tableCapacityInput.size();
    for(int i=0;i<numberOfTables;i++){
        tableCapacity[i] = tableCapacityInput[i];
        heap[i] = i;
        heapUp(i);
    }
}

void RestaurantOrganizer::addNewGroup(int groupSize, ofstream& outFile){
    // IMPLEMENT ME!
    if(groupSize<=tableCapacity[0]) {
        //It updates the remaining capacity according to group sizes
        tableCapacity[0] = tableCapacity[0] - groupSize;
        //It prints the table number to the file
        outFile<<heap[0]<<endl;
        //Then ıt heapify the arrays according to new capacities
        heapDown(0);
    }
    else{
        //If the group size is bigger than the maximum capacity it prints -1 to the file
        outFile<<"-1"<<endl;
    }
}

void RestaurantOrganizer::heapUp(int index){
    // IMPLEMENT ME!
    if(index==0){
        return;
    }
    int parent = (index-1)/2;
    //if given index's capacity is bigger than the parents' capacity they swapped in here
    if(tableCapacity[index]>tableCapacity[parent]){
        swapp(tableCapacity,index,parent);
        swapp(heap,index,parent);
        heapUp(parent);
    }
    //if the given index's capacity and parents' capacity are equal then look at their table number and heapUp the smaller one
    else if(tableCapacity[index]==tableCapacity[parent]){
        if(heap[index]<heap[parent]){
            swapp(tableCapacity,index,parent);
            swapp(heap,index,parent);
            heapUp(parent);
        }
    }
}

void RestaurantOrganizer::heapDown(int index){
    // IMPLEMENT ME
    int leftChild = index*2+1;
    int rightChild = index*2+2;

    if(tableCapacity[leftChild] == 0){
        return;
    }
    //ıt checks which childs' capacity is bigger
    if(tableCapacity[leftChild] > tableCapacity[rightChild]){
        if(tableCapacity[leftChild] > tableCapacity[index]){
            swapp(tableCapacity,index,leftChild);
            swapp(heap,index,leftChild);
            heapDown(leftChild);
        }
        else if(tableCapacity[leftChild] == tableCapacity[index]){
            //if capacities are equal it checks the table numbers
            if(heap[index] > heap[leftChild]){
                swapp(tableCapacity,index,leftChild);
                swapp(heap,index,leftChild);
                heapDown(leftChild);
            }
        }
    }
    //ıt checks which childs' capacity is bigger
    else if(tableCapacity[rightChild] > tableCapacity[leftChild]){
        if(tableCapacity[rightChild] > tableCapacity[index]){
            swapp(tableCapacity,index,rightChild);
            swapp(heap,index,rightChild);
            heapDown(rightChild);
        }
        else if(tableCapacity[rightChild] == tableCapacity[index]){
            //if capacities are equal it checks the table numbers
            if(heap[index] > heap[rightChild]){
                swapp(tableCapacity,index,rightChild);
                swapp(heap,index,rightChild);
                heapDown(rightChild);
            }
        }
    }
    //if left and right child have equal capacity then look at their table numbers and choose the smaller one
    else if(tableCapacity[rightChild] == tableCapacity[leftChild]){
        //if left child has smaller table number changes happen to it
        if(heap[leftChild] < heap[rightChild]){
            if(tableCapacity[leftChild] > tableCapacity[index]){
                swapp(tableCapacity,index,leftChild);
                swapp(heap,index,leftChild);
                heapDown(leftChild);
            }
            else if(tableCapacity[leftChild] == tableCapacity[index]){
                //if capacities are equal it checks the table numbers
                if(heap[index] > heap[leftChild]){
                    swapp(tableCapacity,index,leftChild);
                    swapp(heap,index,leftChild);
                    heapDown(leftChild);
                }
            }

        }
        //if right child has the smaller table number changes happen here
        else if(heap[leftChild] > heap[rightChild]){
            if(tableCapacity[rightChild] > tableCapacity[index]){
                swapp(tableCapacity,index,rightChild);
                swapp(heap,index,rightChild);
                heapDown(rightChild);
            }
            else if(tableCapacity[rightChild] == tableCapacity[index]){
                //if capacities are equal it checks the table numbers
                if(heap[index] > heap[rightChild]){
                    swapp(tableCapacity,index,rightChild);
                    swapp(heap,index,rightChild);
                    heapDown(rightChild);
                }
            }
        }
    }
}

void RestaurantOrganizer::printSorted(ofstream& outFile){
    // IMPLEMENT ME!
    for(int i = 0;i < numberOfTables;i++){
        //it prints the maximum capacity every time to the file
        outFile<<tableCapacity[0]<<" ";
        //it copies the value in the end of the array to the beginning
        tableCapacity[0]= tableCapacity[numberOfTables-i-1];
        //it is some kind of deletion it sends the last element to the far bottom of the max capacity array
        tableCapacity[numberOfTables-i-1]=-1;
        heapDown(0);
    }
}

// YOU CAN ADD YOUR HELPER FUNCTIONS
//this function swap the values in the given index in the given array
void swapp(int *param, int index, int secIndex){
    int tmp = param[index];
    param[index] = param[secIndex];
    param[secIndex] = tmp;
}
