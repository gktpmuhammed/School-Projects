#include <iostream>
#include <stdlib.h>
#include <string>
#include <unistd.h>
#include <sstream>
#include <fstream>
#include <vector>
#include <cstring>
#include <map>
#include <cmath>
#include <algorithm>
using namespace std;
double registers[32] = {0}; //  stores all register values
int ifetch = -1;     // stores instruction fetch state value
int id = -1;         // stores instruction decoding value
int ex = -1;         // stores execution state value
int mem=-1;            // stores memory state value            
int wb=-1;             // stores write back state value
double totinstr = 0;    // total number of instructions that are executed
int stall = 0;          // total number of stalls
void add(int a, int b, int c);  // addition operation
void sll(int a, int b, int c);  // shift left logical operation
void srl(int a, int b, int c);  // shift right logical operation
void and1(int a, int b, int c); // and operation with immediate value
void or1(int a, int b, int c);  // or operation with immediate value
void xor1(int a, int b, int c); // xor operation with immediate value
void andi(int a, int b, int c); // and operation with registers
void xori(int a, int b, int c); // xor operation with registers
void ori(int a, int b, int c);  // or operation with registers
void addi(int a, int b, int c); // addition operation with immediate value
void sub(int a, int b, int c);  // substraction operation with registers
void subi(int a, int b, int c); // subtraction operation with immediate value
void mul(int a, int b, int c);  // multiplication operation
void div(int a, int b, int c);  // division operation
void ld(int a, int b);          // load operation
void lw(int a, int b);          // load operation
void sd(int a, int b);          // store operation
void sw(int a, int b);          // store operation
vector<string> instructions;    // stores the input instruction
string trim(string str);        // trims the spaces in input file
map<string, int> labels;        // stores labels and their program counter values
short memory[65535];           // 64kb memory for our program


int main(int argc, char *argv[])
{
    string inputpath = argv[1]; // takes the input file path as first argument
    string outfile = argv[2];   // takes the output file path as second argument
    ifstream inFile(inputpath); // opens input file stream for reading the instructions
    string line;                // temporary variable for storing the line that is read from input file
    registers[0] = 0;           // hardwireing the x0 register
    while (getline(inFile, line))   // reads a line from input file
    {
        char a = line[line.length() - 1];   // takes the last character of the line

        if (a == 58)                        // if the last character of a line is a colon (:) than add it to labels map
        {
            labels[line.substr(0, line.size() - 1)] = instructions.size() + 1;  // stores the label and its program counter value in map structure
        }

        instructions.push_back(line);           // adds instructions to the instructions array
    }
    string parsedInst[instructions.size()][4];  // zeroth index stores the instruction name, first index stores destination register,
                                                // second index stores source register one, third index stores source register two or immediate value.

    for (int q = 0; q < instructions.size(); q++)   // for tokenizing instructions
    {
        string s = instructions[q];                 // takes the instruction from array
        int n = s.length();                         // declaring character array
        char char_array[n + 1];                     // copying the contents of the string to char array
        strcpy(char_array, s.c_str());
        char *pch;
        pch = strtok(char_array, " ,.");            // tokenizes the string based on spaces dots and commas
        parsedInst[q][0] = pch;                     // takes the instruction name
        pch = strtok(NULL, "");
        if (pch != NULL)
        {
            string nonTok = pch;
            string tok = trim(nonTok);
            char axa[nonTok.length() + 1];          // declaring character array
            strcpy(axa, tok.c_str());              // copying the contents of the string to char array
            pch = strtok(axa, " ,.");               // tokenizes the string based on spaces dots and commas
            parsedInst[q][1] = pch;                 // takes the destination register name
            pch = strtok(NULL, " ,.");
            if (pch != NULL)
            {
                parsedInst[q][2] = pch;             // takes the source register one name
                pch = strtok(NULL, " ,.");          // tokenizes the string based on spaces dots and commas
                if (pch != NULL)
                {
                    parsedInst[q][3] = pch;         // takes the source register two name
                }
            }
        }
    }
    registers[2] = 2000;                            // stack pointers initial place. We can change the size of the stack.
    int hazards[instructions.size()] = {0};         // stores which lines causes hazards and how many times it is causes
    for (int w = 0; w < instructions.size()+2; w++)   // starts the program execution
    {
        wb = mem;                                   // data passing between states
        mem = ex;                                   // data passing between states
        ex = id;                                    // data passing between states
        id = ifetch;                                // data passing between states
        ifetch = w;                                 // data passing between states
        registers[0] = 0;                           // hardwireing x0 register to 0
        if(ex!=-1){                        
        if (parsedInst[mem][0] == "ld" || parsedInst[mem][0] == "lw")               // if instruction in memory state is load 
        {
            // control for load use hazard. if one of the source registers of this instruction is destination register of previous instruction then there is a hazard
            if (parsedInst[mem][1] == parsedInst[ex][2] || parsedInst[mem][1] == parsedInst[ex][3])
            {
                stall++;        // increases number of stalls by one
                hazards[mem]++;   // count each instructions stall number
            }
        }
        if (parsedInst[ex][0] == "add")                  // if instruction is addition with registers
        {
            int a = stoi(parsedInst[ex][1].substr(1));   // takes the destination register number
            int b = stoi(parsedInst[ex][2].substr(1));   // takes the source register one number
            int c = stoi(parsedInst[ex][3].substr(1));   // takes the source register two number
            add(a, b, c);                               // calls necessary function to implement instruction
            totinstr++;                                 // increments executed instruction number
        }
        if (parsedInst[ex][0] == "addi")                  // if instruction is addition with immediate value
        {
            int a = stoi(parsedInst[ex][1].substr(1));   // takes the destination register number
            int b = stoi(parsedInst[ex][2].substr(1));   // takes the source register one number
            int c = stoi(parsedInst[ex][3]);             // takes the immediate value as second argument
            
            addi(a, b, c);                              // calls necessary function to implement instruction
            totinstr++;                                 // increments executed instruction number
        }
        if (parsedInst[ex][0] == "sub")                  // if instruction is subtraction with registers
        {
            int a = stoi(parsedInst[ex][1].substr(1));   // takes the destination register number
            int b = stoi(parsedInst[ex][2].substr(1));   // takes the source register one number
            int c = stoi(parsedInst[ex][3].substr(1));   // takes the source register two number
            sub(a, b, c);                               // calls necessary function to implement instruction
            totinstr++;                                 // increments executed instruction number
        }
        if (parsedInst[ex][0] == "subi")                  // if instruction is subtraction with immediate value
        {
            int a = stoi(parsedInst[ex][1].substr(1));   // takes the destination register number
            int b = stoi(parsedInst[ex][2].substr(1));   // takes the source register one number
            int c = stoi(parsedInst[ex][3]);             // takes the immediate value as second argument
            subi(a, b, c);                              // calls necessary function to implement instruction
            totinstr++;                                 // increments executed instruction number
        }
        if (parsedInst[ex][0] == "mul")                  // if instruction is multiplication with registers
        {
            int a = stoi(parsedInst[ex][1].substr(1));   // takes the destination register number
            int b = stoi(parsedInst[ex][2].substr(1));   // takes the source register one number
            int c = stoi(parsedInst[ex][3].substr(1));   // takes the source register two number
            mul(a, b, c);                               // calls necessary function to implement instruction
            totinstr++;                                 // increments executed instruction number
        }
        if (parsedInst[ex][0] == "div")                  // if instruction is division with registers
        {
            int a = stoi(parsedInst[ex][1].substr(1));   // takes the destination register number
            int b = stoi(parsedInst[ex][2].substr(1));   // takes the source register one number
            int c = stoi(parsedInst[ex][3].substr(1));   // takes the source register two number
            div(a, b, c);                               // calls necessary function to implement instruction
            totinstr++;                                 // increments executed instruction number
        }
        if (parsedInst[ex][0] == "ld")                  // if instruction is load 
        {
            int a = stoi(parsedInst[ex][1].substr(1));   // takes the destination register number
            int b = stoi(parsedInst[ex][2].substr(0, parsedInst[ex][2].find('(')));   // takes the offset value for accessing memory
            int c = stoi(parsedInst[ex][2].substr(parsedInst[ex][2].find('x') + 1, parsedInst[ex][2].size() - 1)); // takes the source register one number
            if (parsedInst[mem][0] == "ld" || parsedInst[mem][0] == "lw")               // control load use hazard 
            {
                // control for load use hazard. if one of the source registers of this instruction is destination register of previous instruction then there is a hazard
                string ctr = "x" + to_string(c);
                if (parsedInst[mem][1] == ctr)
                {
                    stall++;                            // increases number of stalls by one
                    hazards[mem]++;                       // count each instructions stall number
                }
            }
            ld(a, b + registers[c]);                    // calls necessary function to implement instruction
            totinstr++;                                 // increments executed instruction number
        }
        if (parsedInst[ex][0] == "lw")                  // if instruction is load 
        {
            int a = stoi(parsedInst[ex][1].substr(1));   // takes the destination register number
            int b = stoi(parsedInst[ex][2].substr(0, parsedInst[ex][2].find('(')));   // takes the offset value for accessing memory
            int c = stoi(parsedInst[ex][2].substr(parsedInst[ex][2].find('x') + 1, parsedInst[ex][2].size() - 1)); // takes the source register one number
            if (parsedInst[mem][0] == "ld"||parsedInst[mem][0] == "lw")               // control load use hazard 
            {
                // control for load use hazard. if one of the source registers of this instruction is destination register of previous instruction then there is a hazard
                string ctr = "x" + to_string(c);
                if (parsedInst[mem][1] == ctr)
                {
                    stall++;                            // increases number of stalls by one
                    hazards[mem]++;                       // count each instructions stall number
                }
            }
            lw(a, b + registers[c]);                    // calls necessary function to implement instruction
            totinstr++;                                 // increments executed instruction number
        }
        if (parsedInst[ex][0] == "sd")                  // if instruction is store 
        {
            int a = stoi(parsedInst[ex][1].substr(1));   // takes the destination register number
            int b = stoi(parsedInst[ex][2].substr(0, parsedInst[ex][2].find('(')));   // takes the offset value for accessing memory
            int c = stoi(parsedInst[ex][2].substr(parsedInst[ex][2].find('x') + 1, parsedInst[ex][2].size() - 1)); // takes the source register one number
            sd(a, b + registers[c]);                    // calls necessary function to implement instruction
            totinstr++;                                 // increments executed instruction number
        }
        if (parsedInst[ex][0] == "sw")                  // if instruction is store 
        {
            int a = stoi(parsedInst[ex][1].substr(1));   // takes the destination register number
            int b = stoi(parsedInst[ex][2].substr(0, parsedInst[ex][2].find('(')));   // takes the offset value for accessing memory
            int c = stoi(parsedInst[ex][2].substr(parsedInst[ex][2].find('x') + 1, parsedInst[ex][2].size() - 1)); // takes the source register one number
            sw(a, b + registers[c]);                    // calls necessary function to implement instruction
            totinstr++;                                 // increments executed instruction number
        }
        if (parsedInst[ex][0] == "srl")                  // if instruction is shift right logical with registers
        {
            int a = stoi(parsedInst[ex][1].substr(1));   // takes the destination register number
            int b = stoi(parsedInst[ex][2].substr(1));   // takes the source register one number
            int c = stoi(parsedInst[ex][3].substr(1));   // takes the source register two number
            srl(a, b, c);                               // calls necessary function to implement instruction
            totinstr++;                                 // increments executed instruction number
        }
        if (parsedInst[ex][0] == "sll")                  // if instruction is shift left logical with registers
        {
            int a = stoi(parsedInst[ex][1].substr(1));   // takes the destination register number
            int b = stoi(parsedInst[ex][2].substr(1));   // takes the source register one number
            int c = stoi(parsedInst[ex][3].substr(1));   // takes the source register two number
            sll(a, b, c);                               // calls necessary function to implement instruction
            totinstr++;                                 // increments executed instruction number
        }
        if (parsedInst[ex][0] == "and")                  // if instruction is and with registers
        {
            int a = stoi(parsedInst[ex][1].substr(1));   // takes the destination register number
            int b = stoi(parsedInst[ex][2].substr(1));   // takes the source register one number
            int c = stoi(parsedInst[ex][3].substr(1));   // takes the source register two number
            and1(a, b, c);                              // calls necessary function to implement instruction
            totinstr++;                                 // increments executed instruction number
        }
        if (parsedInst[ex][0] == "andi")                 // if instruction is and with immediate value
        {
            int a = stoi(parsedInst[ex][1].substr(1));   // takes the destination register number
            int b = stoi(parsedInst[ex][2].substr(1));   // takes the source register one number
            int c = stoi(parsedInst[ex][3]);             // takes the immediate value as second argument
            andi(a, b, c);                              // calls necessary function to implement instruction
            totinstr++;                                 // increments executed instruction number
        }
        if (parsedInst[ex][0] == "or")                  // if instruction is or with registers
        {
            int a = stoi(parsedInst[ex][1].substr(1));   // takes the destination register number
            int b = stoi(parsedInst[ex][2].substr(1));   // takes the source register one number
            int c = stoi(parsedInst[ex][3].substr(1));   // takes the source register two number
            or1(a, b, c);                               // calls necessary function to implement instruction
            totinstr++;                                 // increments executed instruction number
        }
        if (parsedInst[ex][0] == "ori")                  // if instruction is or with immediate value
        {
            int a = stoi(parsedInst[ex][1].substr(1));   // takes the destination register number
            int b = stoi(parsedInst[ex][2].substr(1));   // takes the source register one number
            int c = stoi(parsedInst[ex][3]);             // takes the immediate value as second argument
            ori(a, b, c);                               // calls necessary function to implement instruction
            totinstr++;                                 // increments executed instruction number
        }
        if (parsedInst[ex][0] == "xor")                  // if instruction is xor with registers
        {
            int a = stoi(parsedInst[ex][1].substr(1));   // takes the destination register number
            int b = stoi(parsedInst[ex][2].substr(1));   // takes the source register one number
            int c = stoi(parsedInst[ex][3].substr(1));   // takes the source register two number
            xor1(a, b, c);                              // calls necessary function to implement instruction
            totinstr++;                                 // increments executed instruction number
        }
        if (parsedInst[ex][0] == "xori")                 // if instruction is xor with immediate value
        {
            int a = stoi(parsedInst[ex][1].substr(1));   // takes the destination register number
            int b = stoi(parsedInst[ex][2].substr(1));   // takes the source register one number
            int c = stoi(parsedInst[ex][3]);             // takes the immediate value as second argument
            xori(a, b, c);                              // calls necessary function to implement instruction
            totinstr++;                                 // increments executed instruction number
        }
        if (parsedInst[ex][0] == "beq")                  // if instruction is branch if equal
        {
            int a = stoi(parsedInst[ex][1].substr(1));   // takes the destination register number
            int b = stoi(parsedInst[ex][2].substr(1));   // takes the source register one number
            if (registers[a] == registers[b])           // compares the values in destination registers
            {
                w = labels[parsedInst[ex][3]] - 1;       // sets the program counter to new value
                id=-1;                                   //flush 
                ifetch=-1;
                hazards[ex]++;                               // count each instructions stall number
                stall+=2;                                    // increases number of stalls by one
            }
            totinstr++;                                 // increments executed instruction number

        }
        if (parsedInst[ex][0] == "bge")
        {
            int a = stoi(parsedInst[ex][1].substr(1));   // takes the destination register number
            int b = stoi(parsedInst[ex][2].substr(1));   // takes the source register one number
            if (registers[a] >= registers[b])           // compares the values in destination registers
            {
                w = labels[parsedInst[ex][3]] - 1;       // sets the program counter to new value
                id=-1;                                   //flush values in id and if
                ifetch=-1;
                hazards[ex]++;                               // count each instructions stall number
                stall+=2;                                    // increases number of stalls by one
            }
            totinstr++;                                 // increments executed instruction number
        }
        if (parsedInst[ex][0] == "blt")
        {
            int a = stoi(parsedInst[ex][1].substr(1));   // takes the destination register number
            int b = stoi(parsedInst[ex][2].substr(1));   // takes the source register one number
            if (registers[a] < registers[b])            // compares the values in destination registers
            {
                w = labels[parsedInst[ex][3]] - 1;       // sets the program counter to new value
                id=-1;                                   //flush values in id and if
                ifetch=-1;
                hazards[ex]++;                               // count each instructions stall number
                stall+=2;                                    // increases number of stalls by one
            }
            
            totinstr++;                                 // increments executed instruction number
        }
        if (parsedInst[ex][0] == "bne")
        {
            int a = stoi(parsedInst[ex][1].substr(1));   // takes the destination register number
            int b = stoi(parsedInst[ex][2].substr(1));   // takes the source register one number
            if (registers[a] != registers[b])           // compares the values in destination registers
            {
                w = labels[parsedInst[ex][3]] - 1;       // sets the program counter to new value
                id=-1;                                   //flush values in id and if
                ifetch=-1;
                hazards[ex]++;                               // count each instructions stall number
                stall+=2;                                    // increases number of stalls by one
            }
            totinstr++;                                 // increments executed instruction number
        }
        if (parsedInst[ex][0] == "jal")
        {
            int a = stoi(parsedInst[ex][1].substr(1));   // takes the destination register number
            ifetch=-1;
            id=-1;
            registers[a] = ex + 1;                       // stores the return address of program
            w = labels[parsedInst[ex][2]] - 1;           // sets the program counter to new value
            totinstr++;                                 // increments executed instruction number
        }
        if (parsedInst[ex][0] == "jalr")
        {
            int a = stoi(parsedInst[ex][1].substr(1));   // takes the destination register number
            int b = stoi(parsedInst[ex][2].substr(1));   // takes the source register one number
            ifetch=-1;
            id=-1;
            registers[a] = ex + 1;                       // stores the return address of program
            w = registers[b] - 1;                       // sets the program counter to new value
            totinstr++;                                 // increments executed instruction number
        }
        }
    }
    ofstream output(outfile);                           // opens output stream for printing output
    double cpi = (totinstr + 4 + stall) / totinstr;     // stores the clock cycles per instructions
    for (int i = 0; i < 32; i++)                        // prints all registers with final values
    {
        output << "Register x" << i << ": " << registers[i] << endl;
    }
    output << "Total number of stalls: " << stall << endl;              // prints total number of stalls
    output << "Total clock cycle: " << totinstr + 4 + stall << endl;    // prints total clock cycles
    output << "Total instructions: " << totinstr << endl;               // prints total instruction number
    output << "CPI: " << cpi << endl;                                   // prints clock cycles per instruction number
    // checks whether there is any data hazard happend in program or not. If there is any than it prints how many times it occured.
    for (int i = 0; i < instructions.size(); i++)           
    {
        if (hazards[i] != 0)                // if this lines causes any hazard than print the number
        {
            if (parsedInst[i][0] == "ld")   // if this instruction is load
            {
                output << hazards[i] << " times load use hazard in line " << i + 1 << " : " << instructions[i] << endl;
            }
            else                            // if this instruction is branch
            {
                output << hazards[i] << " times branch hazard in line " << i + 1 << " : " << instructions[i] << endl;
            }
        }
    }
    output.close();
}

void add(int a, int b, int c)
{
    registers[a] = registers[b] + registers[c];         // performs addition operation with registers
}

void addi(int a, int b, int c)
{
    registers[a] = registers[b] + c;                    // performs addition operation with immediate value
}

void sub(int a, int b, int c)
{
    registers[a] = registers[b] - registers[c];         // performs subtraction operation with registers
}

void subi(int a, int b, int c)
{
    registers[a] = registers[b] - c;                    // performs subtraction operation with immediate value
}

void mul(int a, int b, int c)
{
    registers[a] = registers[b] * registers[c];         // performs multiplication operation with registers
}

void div(int a, int b, int c)
{
    registers[a] = registers[b] / registers[c];         // performs division operation with registers
}

void ld(int a, int b)
{
    registers[a] = memory[b];                           // performs load operation from memory
}

void sd(int a, int b)
{
    memory[b] = registers[a];                           // performs store operation to memory
}
void sw(int a, int b)
{
    if(b%8==0){
        short x=memory[b] %256;                            // performs store operation to memory
        memory[b]=memory[b]-x+registers[a];
        
    }
    else{   
        short x=memory[b]/256;            
        memory[b]=x+registers[a]*256;                   // performs store operation to memory

    }
}
void lw(int a, int b)
{
    if(b%8==0){
        short x=memory[b] %256;                            // performs store operation to memory
        registers[a]=x;
        
    }
    else{   
        short x=memory[b]/256;            
        registers[a]=x;                   // performs store operation to memory

    }
}
// performs shift right logical operation with registers
void srl(int a, int b, int c)                           
{
    int first = registers[b];                           // takes the value that will be shifted
    int second = registers[c];                          // takes the number of bits that will be shifted
    string binary;                                      // stores the binary value of the current number before shift
    for (int i = 0; i < 64; i++)                        // calculates binary value before shift
    {
        binary = to_string(first % 2) + binary;
        first /= 2;
    }   
    for (int i = 0; i < second; i++)                    // shifts the binary number by given number of bits
    {
        binary = "0" + binary.substr(0, binary.length() - 1);
    }
    first = 0;
    for (int i = 0; i < 64; i++)                        // calculates integer value of the shifted number
    {
        if (binary.at(64 - i - 1) == '1')
        {
            first += pow(2, i);
        }
    }
    registers[a] = first;                               // stores the result to the destination register
}
// performs shift left logical operation with registers
void sll(int a, int b, int c)                           
{
    int first = registers[b];                           // takes the value that will be shifted
    int second = registers[c];                          // takes the number of bits that will be shifted
    string binary;                                      // stores the binary value of the current number before shift
    for (int i = 0; i < 64; i++)                        // calculates binary value before shift
    {
        binary = to_string(first % 2) + binary;
        first /= 2;
    }
    for (int i = 0; i < second; i++)                    // shifts the binary number by given number of bits
    {
        binary = binary.substr(1) + "0";
    }
    first = 0;
    for (int i = 0; i < 64; i++)                        // calculates integer value of the shifted number
    {
        if (binary.at(64 - i - 1) == '1')
        {
            first += pow(2, i);
        }
    }
    registers[a] = first;                               // stores the result to the destination register
}
// performs and operation with registers
void and1(int a, int b, int c)                          // performs and operation with registers
{
    int k = registers[b];                               // takes the first value that will be compared
    int l = registers[c];                               // takes the second value that will be compared
    int result = 0;                                     // stores the value after the comparison
    for (int i = 0; i < 64; i++)                        // compares 64 bits of the numbers in binary representation
    {
        if (k % 2 == 1 && l % 2 == 1)                   // performs and operation for each bit
        {
            result += pow(2, i);                        // updates each bit according to comparison
        }
        k = k / 2;
        l = l / 2;
    }
    registers[a] = result;                              // stores the result in destination register
}
// performs and operation with immediate value
void andi(int a, int b, int c)                          
{
    int k = registers[b];                               // takes the first value that will be compared
    int l = c;                                          // takes the second value that will be compared
    int result = 0;                                     // stores the value after the comparison
    for (int i = 0; i < 64; i++)                        // compares 64 bits of the numbers in binary representation
    {
        if (k % 2 == 1 && l % 2 == 1)                   // performs and operation for each bit
        {
            result += pow(2, i);                        // updates each bit according to comparison
        }
        k = k / 2;
        l = l / 2;
    }
    registers[a] = result;                              // stores the result in destination register
}
// performs or operation with registers
void or1(int a, int b, int c)                           
{
    int k = registers[b];                               // takes the first value that will be compared
    int l = registers[c];                               // takes the second value that will be compared
    int result = 0;                                     // stores the value after the comparison
    for (int i = 0; i < 64; i++)                        // compares 64 bits of the numbers in binary representation
    {
        if (k % 2 == 1 || l % 2 == 1)                   // performs or operation for each bit
        {
            result += pow(2, i);                        // updates each bit according to comparison
        }
        k = k / 2;
        l = l / 2;
    }
    registers[a] = result;                              // stores the result in destination register
}
// performs or operation with immediate value
void ori(int a, int b, int c)                           
{
    int k = registers[b];                               // takes the first value that will be compared
    int l = c;                                          // takes the second value that will be compared
    int result = 0;                                     // stores the value after the comparison
    for (int i = 0; i < 64; i++)                        // compares 64 bits of the numbers in binary representation
    {
        if (k % 2 == 1 || l % 2 == 1)                   // performs or operation for each bit
        {
            result += pow(2, i);                        // updates each bit according to comparison
        }
        k = k / 2;
        l = l / 2;
    }
    registers[a] = result;                              // stores the result in destination register
}
// performs xor operation with registers
void xor1(int a, int b, int c)
{
    int k = registers[b];                               // takes the first value that will be compared
    int l = registers[c];                               // takes the second value that will be compared
    int result = 0;                                     // stores the value after the comparison
    for (int i = 0; i < 64; i++)                        // compares 64 bits of the numbers in binary representation
    {
        if ((k % 2) xor (l % 2))                        // performs xor operation for each bit
        {
            result += pow(2, i);                        // updates each bit according to comparison
        }
        k = k / 2;
        l = l / 2;
    }
    registers[a] = result;                              // stores the result in destination register
}
// performs xor operation with immediate value
void xori(int a, int b, int c)                          
{
    int k = registers[b];                               // takes the first value that will be compared
    int l = c;                                          // takes the second value that will be compared
    int result = 0;                                     // stores the value after the comparison
    for (int i = 0; i < 64; i++)                        // compares 64 bits of the numbers in binary representation
    {
        if ((k % 2) xor (l % 2))                        // performs xor operation for each bit
        {
            result += pow(2, i);                        // updates each bit according to comparison
        }
        k = k / 2;
        l = l / 2;
    }
    registers[a] = result;                              // stores the result in destination register
}
// trims the string 
string trim(string str)
{
    string newStr = "";
    for (int i = 0; i < str.length(); i++)
    {
        if (str.at(i) != ' ')
        {
            newStr += (char)str.at(i);                  // deletes every spaces from string
        }
    }
    return newStr;
}
