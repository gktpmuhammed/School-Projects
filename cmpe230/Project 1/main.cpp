#include <iostream>
#include <sstream>
#include <string>
#include <fstream>
#include <vector>
#include <cstring>
#include <map>
#include <cmath>
#include <algorithm>

using namespace std;

void registers();
int hexadecimalToDecimal(string hex);
string trim(string str);
int variableValue(string var);
void auxiliary(int n, int m);
void sign(int a, int b);
void overflow(int a,int b,int bb,int c);
void carry(int a,int b,int bb,int d);
void flag(int a ,int b, int bb,int d);
void mov(string a, string b, string c);
void inc(string a);
void dec(string a);
void add(string a, string b, string c);
void sub(string a, string b,string c);
void mul(string a);
void div(string a);
int and2(int a, int b, int bit);
void and1(string a, string b,string c);
int or2(int a, int b, int bit);
void or1(string a, string b,string c);
int xor2(int a, int b, int bit);
void xor1(string a, string b,string c);
int not2(int a, int bit);
void not1(string a);
int shift2(int a, int count, int bit, int c);
void shift1(string a, int count, int c);
int rot2(int a, int count, int bit, int c);
void rot1(string a, int count, int c);
void pop(string a);
void push(string a);
void cmp(string a, string b, string c);

// global variables ( memory, registers and flags )
unsigned char memory[65535] ;    // 64K memory
unsigned short ax = 0 ;
unsigned short bx = 0 ;
unsigned short cx = 0 ;
unsigned short dx = 0 ;
unsigned short di = 0 ;
unsigned short si = 0 ;
unsigned short bp = 0 ;
unsigned short sp = 65534 ;
vector<string> instructions;
map<string, pair<string, pair<int, int>>> variables;
map<string, int> labels;
map<string, unsigned short> registerMap;
map<string, unsigned char> subRegisterMap;
bool control;

bool zf = 0 ;              // zero flag
bool sf = 0 ;              // sign flag
bool cf = 0 ;              // carry flag
bool af = 0 ;              // auxillary flag
bool of = 0 ;              // overflow flag

// initialize pointers
unsigned short *pax = &ax ;
unsigned short *pbx = &bx ;
unsigned short *pcx = &cx ;
unsigned short *pdx = &dx ;
unsigned short *pdi = &di ;
unsigned short *psi = &si ;
unsigned short *pbp = &bp ;
// note that x86 uses little endian, that is, least significat byte is stored in lowest byte
unsigned char *pah = (unsigned char *) ( ( (unsigned char *) &ax) + 1) ;
unsigned char *pal = (unsigned char *) &ax  ;
unsigned char *pbh = (unsigned char *) ( ( (unsigned char *) &bx) + 1) ;
unsigned char *pbl = (unsigned char *) &bx  ;
unsigned char *pch = (unsigned char *) ( ( (unsigned char *) &cx) + 1) ;
unsigned char *pcl = (unsigned char *) &cx  ;
unsigned char *pdh = (unsigned char *) ( ( (unsigned char *) &dx) + 1) ;
unsigned char *pdl = (unsigned char *) &dx  ;

int main(int argc, char* argv[]){
    // Open the input and output files
    registerMap["ax"]=ax;
    registerMap["bx"]=bx;
    registerMap["cx"]=cx;
    registerMap["dx"]=dx;
    registerMap["di"]=di;
    registerMap["si"]=si;
    registerMap["bp"]=bp;
    subRegisterMap["ah"] = *pah;
    subRegisterMap["al"] = *pal;
    subRegisterMap["bh"] = *pbh;
    subRegisterMap["bl"] = *pbl;
    subRegisterMap["ch"] = *pch;
    subRegisterMap["cl"] = *pcl;
    subRegisterMap["dh"] = *pdh;
    subRegisterMap["dl"] = *pdl;
    ifstream inFile(argv[1]);
    string line;
    bool endCode = false;
    int count=0;
    int index;
    bool test=true;
    while(getline(inFile,line)){
        char a =line[line.length()-1];
        if (line.find(39) == line.npos){
            std::for_each(line.begin(), line.end(), [](char & c){
                c = ::tolower(c);
            });
        }
        else {
            string y = line.substr(0,line.find(39));
            std::for_each(y.begin(), y.end(), [](char & c){
                c = ::tolower(c);
            });
            line=y+line.substr(y.length());
        }
        if( a == 58){
            labels[line.substr(0,line.size()-1)]=instructions.size()+1;
            count--;
        }
        string tmp,tmp1,tmp2;
        if(endCode && trim(line) != "codeends" ){
            istringstream linestream(line);
            getline(linestream, tmp, ' ');
            getline(linestream, tmp1, ' ');
            getline(linestream, tmp2, ' ');
            if(test){
                index=count*6;
                test=false;
            }
            int value;
            if(tmp2.at(0)<58 && tmp2.at(0)>47 && tmp2.at(tmp2.length()-1)==104){
                value=hexadecimalToDecimal(tmp2);
            }
            else if(tmp2.at(0)==39 && tmp2.at(tmp2.length()-1)==39){
                value=tmp2.at(1);
            }
            else if(tmp2.at(0)<58 && tmp2.at(0)>47 && tmp2.at(tmp2.length()-1)=='d'){
                if(stoi(tmp2.substr(0,tmp2.length()-1))>65535)
                    control=false;
                else
                    value =stoi(tmp2.substr(0,tmp2.length()-1));
            }
            else if(tmp2.at(0)=='0' && tmp2.at(tmp2.length()-1)<58 && tmp2.at(tmp2.length()-1)>47){
                value=hexadecimalToDecimal(tmp2+'h');
            }
            else if(tmp2.at(0)<58 && tmp2.at(0)>47 && tmp2.at(tmp2.length()-1)<58 && tmp2.at(tmp2.length()-1)>47){
                if(stoi(tmp2)>65535)
                    control=false;
                else
                    value =stoi(tmp2);
            }
            else{
                cout<<"Error";
                return 0;
            }

            variables[tmp]=make_pair(tmp1,make_pair(value,index));
            if(tmp1=="dw"){
                int higher=value/256;
                int lower=value%256;
                memory[index]=lower;
                memory[index+1]=higher;
                index+=2;
            }
            else if(tmp1=="db"){
                memory[index]=value;
                index++;
            }
            else{
                cout<<"Error";
                return 0;
            }
        }
        instructions.push_back(line);
        if( trim(line)=="int20h"){
            endCode = true;
        }
        if(!endCode){
            count++;
        }
    }
    string parsedInst[instructions.size()][4];
    control=true;
    for(int q = 0; q < instructions.size(); q++) {
        string s = instructions[q];
        int n = s.length();// declaring character array
        char char_array[n + 1];// copying the contents of the string to char array
        strcpy(char_array, s.c_str());
        char *pch;
        pch = strtok(char_array, " ,.");
        parsedInst[q][0] = pch;
        pch = strtok(NULL, "");
        if(pch!=NULL){
            string nonTok = pch;
            string anan =trim(nonTok);
            char axa[nonTok.length() + 1];
            strcpy(axa, anan.c_str());
            pch = strtok(axa, " ,.");
            parsedInst[q][1] = pch;
            pch = strtok(NULL, " ,.");
            if(pch!=NULL)
                parsedInst[q][2] = pch;
        }
        string one=parsedInst[q][1];
        string two=parsedInst[q][2];
        if(instructions[q][instructions[q].length()-1]==39){
            if(parsedInst[q][2]==""){
                parsedInst[q][1]=to_string((int)instructions[q][instructions[q].length()-2]);
                one=parsedInst[q][1];
            }
            else{
                parsedInst[q][2]=to_string((int)instructions[q][instructions[q].length()-2]);
                two=parsedInst[q][2];
            }
        }
        if(one.substr(0,6)=="offset"){
            parsedInst[q][1]="offset";
            parsedInst[q][2]=one.substr(6);
            parsedInst[q][3]=two;
        }
        if(two.substr(0,6)=="offset"){
            parsedInst[q][2]="offset";
            parsedInst[q][3]=two.substr(6);
        }
    }
    for(int w = 0; w < instructions.size(); w++){
        string a = parsedInst[w][1];
        string b = parsedInst[w][2];
        string c = parsedInst[w][3];
        if(a.length()>1){
            if(variables.find(a) == variables.end() && variables.find(a.substr(1)) != variables.end()){
                a = a.substr(1);
            }
        }
        if(b.length()>1){
            if(variables.find(b) == variables.end() && variables.find(b.substr(1)) != variables.end() ){
                b = b.substr(1);
            }
        }
        if(c.length()>1){
            if(variables.find(c) == variables.end() && variables.find(c.substr(1)) != variables.end()){
                c = c.substr(1);
            }
        }
        if(b.length()!=0){
            if(b.at(0)>47 && b.at(0)<58 && b.at(b.length()-1) == 104){
                b = to_string(hexadecimalToDecimal(b));
            }
            else if(b.at(0) == '0' && b.at(b.length()-1)>47 && b.at(b.length()-1)<58){
                b = to_string(hexadecimalToDecimal(b+'h'));
            }
            else if(b.at(0)<58 && b.at(0)>47 && b.at(b.length()-1)=='d'){
                b =b.substr(0,b.length()-1);
                if(stoi(b)>65535)
                    control=false;
            }
            else if(b.at(0) == '[' && b.at(b.length()-1) == ']' && b.at(1)>47 && b.at(1)<58 && b.at(b.length()-2) == 104){
                b = "[" + to_string(hexadecimalToDecimal(b.substr(1,b.length()-3))) + "]";
            }
            else if(b.at(0) == '[' && b.at(b.length()-1) == ']' && b.at(1) == '0' && b.at(b.length()-2)<58 && b.at(b.length()-2) >47){
                b = "[" + to_string(hexadecimalToDecimal(b.substr(1,b.length()-2))) + "]";
            }
            else if(b.length()>3){
                if(b.at(1) == '[' && b.at(b.length()-1) == ']' && b.at(2) == '0' && b.at(b.length()-2)<58 && b.at(b.length()-2) >47){
                    b = b.substr(0,2) + to_string(hexadecimalToDecimal(b.substr(2,b.length()-3))) + "]";
                }
                else if(b.at(1) == '[' && b.at(b.length()-1) == ']' && b.at(2) <58&&b.at(2)>47 && b.at(b.length()-2)=='h'){
                    b = b.substr(0,2) + to_string(hexadecimalToDecimal(b.substr(2,b.length()-4))) + "]";
                }
                else if(b.at(1) == '[' && b.at(b.length()-1) == ']' && b.at(2) <58&&b.at(2)>47 && b.at(b.length()-2)=='d'){
                    b = b.substr(0,b.length()-2) + "]";
                    if(stoi(b.substr(2,b.length()-3))>65535)
                        control=false;
                }
            }
            else if(b=="sp"){
                b=to_string(sp);
            }
        }
        if(a.length()!=0 && a!="21h" && a!="20h") {
            if (a.at(0) > 47 && a.at(0) < 58 && a.at(a.length() - 1) == 104) {
                a = to_string(hexadecimalToDecimal(a));
            } else if (a.at(0) == '0' && a.at(a.length() - 1) > 47 && a.at(a.length() - 1) < 58) {
                a = to_string(hexadecimalToDecimal(a + 'h'));
            } else if (a.at(0) < 58 && a.at(0) > 47 && a.at(a.length() - 1) == 'd') {
                a = a.substr(0, a.length() - 1);
                if(stoi(a)>65535)
                    control=false;
            } else if (a.at(0) == '[' && a.at(a.length() - 1) == ']' && a.at(1) > 47 && a.at(1) < 58 &&
                       a.at(a.length() - 2) == 104) {
                a = "[" + to_string(hexadecimalToDecimal(a.substr(1, a.length() - 3))) + "]";
            } else if (a.at(0) == '[' && a.at(a.length() - 1) == ']' && a.at(1) == '0' && a.at(a.length() - 2) < 58 &&
                       a.at(a.length() - 2) > 47) {
                a = "[" + to_string(hexadecimalToDecimal(a.substr(1, a.length() - 2))) + "]";
            } else if (a.length() > 3) {
                if (a.at(1) == '[' && a.at(a.length() - 1) == ']' && a.at(2) == '0' && a.at(a.length() - 2) < 58 &&
                    a.at(a.length() - 2) > 47) {
                    a = a.substr(0, 2) + to_string(hexadecimalToDecimal(a.substr(2, a.length() - 3))) + "]";
                } else if (a.at(1) == '[' && a.at(a.length() - 1) == ']' && a.at(2) < 58 && a.at(2) > 47 &&
                           a.at(a.length() - 2) == 'h') {
                    a = a.substr(0, 2) + to_string(hexadecimalToDecimal(a.substr(2, a.length() - 4))) + "]";
                } else if (a.at(1) == '[' && a.at(a.length() - 1) == ']' && a.at(2) < 58 && a.at(2) > 47 &&
                           a.at(a.length() - 2) == 'd') {
                    a = a.substr(0, a.length() - 2) + "]";
                    if(stoi(a.substr(2,a.length()-3))>65535)
                        control=false;
                }
            }
        }
        else if(b=="sp"){
            b=to_string(sp);
        }
        if(c.length()!= 0){
            if(c.at(0)>47 && c.at(0)<58 && c.at(c.length()-1) == 104){
                c = to_string(hexadecimalToDecimal(c));
            }
            else if(c.at(0) == '0' && c.at(c.length()-1)>47 && c.at(c.length()-1)<58){
                c = to_string(hexadecimalToDecimal(c+'h'));
            }
            if(c.at(0) == '[' && c.at(c.length()-1) == ']' && c.at(1)>47 && c.at(1)<58 && c.at(c.length()-2) == 104){
                c = "[" + to_string(hexadecimalToDecimal(c.substr(1,c.length()-3))) + "]";
            }
            else if(c.at(0) == '[' && c.at(c.length()-1) == ']' && c.at(1) == '0' && c.at(c.length()-2)<58 && c.at(c.length()-2) >47){
                c = "[" + to_string(hexadecimalToDecimal(c.substr(1,c.length()-2))) + "]";
            }
        }
        if(!control){
            cout<<"Error";
            return 0;
        }
        if(parsedInst[w][0] == "add"){
            add(a,b,c);
        }
        else if(parsedInst[w][0] == "sub"){
            sub(a,b,c);
        }
        else if(parsedInst[w][0] == "mov"){
            if(b.length()==0){
                cout << "Error";
                return 0;
            }
            mov(a,b,c);
        }
        else if(parsedInst[w][0] == "mul"){
            mul(a);
        }
        else if(parsedInst[w][0] == "div"){
            div(a);
        }
        else if(parsedInst[w][0] == "xor"){
            xor1(a,b,c);
        }
        else if(parsedInst[w][0] == "or"){
            or1(a,b,c);
        }
        else if(parsedInst[w][0] == "and"){
            and1(a,b,c);
        }
        else if(parsedInst[w][0] == "not"){
            not1(a);
        }
        else if(parsedInst[w][0] == "rcr"){
            if(b == "cl"){
                b = to_string(subRegisterMap["cl"] % 32);
                rot1(a,stoi(b),2);
            }
            else if(b.at(0) > 47 && b.at(0) < 58 && variables.find(b) == variables.end()){
                b = to_string(stoi(b) % 32);
                rot1(a,stoi(b),2);
            }
            else{
                control = false;
            }
        }
        else if(parsedInst[w][0] == "rcl"){
            if(b == "cl"){
                b = to_string(subRegisterMap["cl"] % 32);
                rot1(a,stoi(b),1);
            }
            else if(b.at(0) > 47 && b.at(0) < 58 && variables.find(b) == variables.end()){
                b = to_string(stoi(b) % 32);
                rot1(a,stoi(b),1);
            }
            else{
                control = false;
            }
        }
        else if(parsedInst[w][0] == "shl"){
            if(b == "cl"){
                b = to_string(subRegisterMap["cl"] % 32);
                shift1(a,stoi(b),1);
            }
            else if(b.at(0) > 47 && b.at(0) < 58 && variables.find(b) == variables.end()){
                b = to_string(stoi(b) % 32);
                shift1(a,stoi(b),1);
            }
            else{
                control = false;
            }
        }
        else if(parsedInst[w][0] == "shr"){
            if(b == "cl"){
                b = to_string(subRegisterMap["cl"] % 32);
                shift1(a,stoi(b),2);
            }
            else if(b.at(0) > 47 && b.at(0) < 58 && variables.find(b) == variables.end()){
                b = to_string(stoi(b) % 32);
                shift1(a,stoi(b),2);
            }
            else{
                control = false;
            }
        }
        else if(parsedInst[w][0] == "push"){
            push(a);
        }
        else if(parsedInst[w][0] == "pop"){
            pop(a);
        }
        else if(parsedInst[w][0] == "nop"){

        }
        else if(parsedInst[w][0] == "cmp"){
            cmp(a,b,c);
        }
        else if(parsedInst[w][0] == "jz"){
            if(zf == 1){
                w = labels[a]-1;
            }
        }
        else if(parsedInst[w][0] == "jnz"){
            if(zf == 0){
                w = labels[a]-1;
            }
        }
        else if(parsedInst[w][0] == "je"){
            if(zf == 1){
                w = labels[a]-1;
            }
        }
        else if(parsedInst[w][0] == "jne"){
            if(zf == 0){
                w = labels[a]-1;
            }
        }
        else if(parsedInst[w][0] == "ja"){
            if(zf == 0 && cf == 0){
                w = labels[a]-1;
            }
        }
        else if(parsedInst[w][0] == "jae"){
            if(cf == 0){
                w = labels[a]-1;
            }
        }
        else if(parsedInst[w][0] == "jb"){
            if(cf == 1){
                w = labels[a]-1;
            }
        }
        else if(parsedInst[w][0] == "jbe"){
            if(zf == 1 || cf == 1){
                w = labels[a]-1;
            }
        }
        else if(parsedInst[w][0] == "jnae"){
            if(cf == 1){
                w = labels[a]-1;
            }
        }
        else if(parsedInst[w][0] == "jnb"){
            if(cf == 0){
                w = labels[a]-1;
            }
        }
        else if(parsedInst[w][0] == "jnbe"){
            if(zf == 0 && cf == 0){
                w = labels[a]-1;
            }
        }
        else if(parsedInst[w][0] == "jna"){
            if(zf == 1 || cf == 1){
                w = labels[a]-1;
            }
        }
        else if(parsedInst[w][0] == "jnc"){
            if(cf == 0){
                w = labels[a]-1;
            }
        }
        else if(parsedInst[w][0] == "jc"){
            if(cf == 1){
                w = labels[a]-1;
            }
        }
        else if(parsedInst[w][0] == "int" && a == "20h"){
            w = instructions.size();
        }
        else if(parsedInst[w][0] == "int" && a == "21h"){
            if(*pah == 1){
                string in;
                cin >> in;
                int c = (int)in.at(0);
                subRegisterMap["al"] = c;
            }
            if(*pah == 2){
                cout << (char)subRegisterMap["dl"];
                subRegisterMap["al"] = subRegisterMap["dl"];
            }
        }
        else if(parsedInst[w][0] == "dec"){
            dec(a);
        }
        else if(parsedInst[w][0] == "inc"){
            inc(a);
        }
        else if(parsedInst[w][0] == "code"){

        }
        else if(labels.find(parsedInst[w][0].substr(0,parsedInst[w][0].length()-1))!=labels.end() ){

        }
        else{
            control=false;
        }
        registers();

        if(!control){
            cout << "Error";
            return 0;
        }
    }
    return 0;

}

//Ths method controls values of registers and subregisters. If they changed it updates them to the new values.
void registers(){
    if(ax!=registerMap["ax"]){
        ax=registerMap["ax"];
        subRegisterMap["al"]=*pal;
        subRegisterMap["ah"]=*pah;
    }
    if(bx!=registerMap["bx"]){
        bx=registerMap["bx"];
        subRegisterMap["bl"]=*pbl;
        subRegisterMap["bh"]=*pbh;
    }
    if(cx!=registerMap["cx"]){
        cx=registerMap["cx"];
        subRegisterMap["cl"]=*pcl;
        subRegisterMap["ch"]=*pch;
    }
    if(dx!=registerMap["dx"]){
        dx=registerMap["dx"];
        subRegisterMap["dl"]=*pdl;
        subRegisterMap["dh"]=*pdh;
    }
    if(bp!=registerMap["bp"]){
        bp=registerMap["bp"];
    }
    if(di!=registerMap["di"]){
        di=registerMap["di"];
    }
    if(si!=registerMap["si"]){
        si=registerMap["si"];
    }
    if(*pal!=subRegisterMap["al"]){
        *pal=subRegisterMap["al"];
        registerMap["ax"]=ax;
    }
    if(*pah!=subRegisterMap["ah"]){
        *pah=subRegisterMap["ah"];
        registerMap["ax"]=ax;
    }
    if(*pbh != subRegisterMap["bh"]){
        *pbh=subRegisterMap["bh"];
        registerMap["bx"]=bx;
    }
    if(*pbl!=subRegisterMap["bl"]){
        *pbl=subRegisterMap["bl"];
        registerMap["bx"]=bx;
    }
    if(*pch != subRegisterMap["ch"]){
        *pch =subRegisterMap["ch"];
        registerMap["cx"]=cx;
    }
    if(*pcl!=subRegisterMap["cl"]){
        *pcl=subRegisterMap["cl"];
        registerMap["cx"]=cx;
    }
    if(*pdl!=subRegisterMap["dl"]){
        *pdl=subRegisterMap["dl"];
        registerMap["dx"]=dx;
    }
    if(*pdh!=subRegisterMap["dh"]){
        *pdh=subRegisterMap["dh"];
        registerMap["dx"]=dx;
    }
}

//This method deletes the spaces in instruction lines.
string trim(string str){
    string newStr = "";
    for(int i = 0; i < str.length(); i++){
        if(str.at(i) != ' '){
            newStr += (char)str.at(i);
        }
    }
    return newStr;
}

//This method calculates the variables value from memory.
int variableValue(string var){
    if(variables[var].first=="db"){
        return memory[variables[var].second.second];
    } else{
        int a=memory[variables[var].second.second];
        int b=memory[variables[var].second.second+1];
        return a+b*256;
    }
}

//This method converts hexadecimal numbers to decimal numbers.
int hexadecimalToDecimal(string hex){
    int len = hex.length();
    // Initializing base value to 1, i.e 16^0
    int base = 1;
    int dec_val = 0;

    // Extracting characters as digits from last character
    for (int i=len-2; i>=0; i--){
        // if character lies in '0'-'9', converting
        // it to integral 0-9 by subtracting 48 from
        // ASCII value.
        if (hex.at(i)>=48 && hex.at(i)<=57){
            dec_val += (hex.at(i) - 48)*base;
            // incrementing base by power
            base = base * 16;
        }
            // if character lies in 'A'-'F' , converting
            // it to integral 10 - 15 by subtracting 55
            // from ASCII value
        else if (hex.at(i)>='A' && hex.at(i)<='F'){
            dec_val += (hex.at(i) - 55)*base;
            // incrementing base by power
            base = base*16;
        }
        else if (hex.at(i)>='a' && hex.at(i)<='f'){
            dec_val += (hex.at(i) - 87)*base;
            // incrementing base by power
            base = base*16;
        }
    }
    if(dec_val < 65536){
        return dec_val;
    }
    else {
        control = false;
        return 0;
    }

}

//This method changes auxiliary flag when its necessary.
void auxiliary(int n, int m){
    int h,c;
    int d=0;
    for(int i=0;i<4;i++){
        h +=(n%2)*(pow(2,d));
        c +=(m%2)*(pow(2,d));
        if(n>0)
            n=n/2;
        if(m>0)
            m=m/2;
        d++;
    }
    if(h + c >= 16){
        af = 1;
    }
    else {
        af = 0;
    }
}

//This method changes sign flag when its necessary.
void sign(int a, int b){
    if (b == 8){
        if(a >= 128){
            sf = 1;
        }
        else {
            sf = 0;
        }
    }
    else if(b == 16){
        if(a >= 32768){
            sf = 1;
        }
        else{
            sf = 0;
        }
    }
}

//This method changes carry flag when its necessary.
void carry(int a,int b,int bb,int d){
    if(bb>16){
        if(d==1){
            if(a + b>65536)
                cf=1;
            else
                cf=0;
        }
        else if(d==2){
            if(a<b)
                cf=1;
            else
                cf=0;
        }
    }
    else if(bb==16){
        if(d==1){
            if(a + b>256)
                cf=1;
            else
                cf=0;
        }
        else if(d==2){
            if(a<b)
                cf=1;
            else
                cf=0;
        }
    }
}

//This method changes overflow flag when its necessary.
void overflow(int a,int b,int bb,int c){
    if(bb>16){
        if(a<32768 && b<32768){
            if(c>=32768)
                of=1;
            else
                of=0;
        }
        else if(a>=32768 && b>=32768){
            if(c<32768)
                of=1;
            else
                of=0;
        }
        else{
            of = 0;
        }
    }
    if(bb==16){
        if(a<128 && b<128){
            if(c>=128)
                of=1;
            else
                of=0;
        }
        else if(a>=128 && b>=128){
            if(c<128)
                of=1;
            else
                of=0;
        }
        else{
            of = 0;
        }
    }
}

//This method used in add,sub,cmp methods. It changes all flags when its necessary.
void flag(int a ,int b, int bb,int d){
    if(d == 1){
        auxiliary(a,b);
        overflow(a,b,bb,a+b);
        sign(a+b,bb);
        if(a+b == 0){
            zf = 1;
        }
        else {
            zf = 0;
        }
        carry(a,b,bb,d);
    }
    else if(d == 2){
        auxiliary(a-b,b);
        overflow(a,b,bb,a-b);
        sign(a-b,bb);
        if(a-b == 0){
            zf = 1;
        }
        else {
            zf = 0;
        }
        carry(a,b,bb,d);
    }
    else if(d==3){
        if(bb==16){
            if(subRegisterMap["ah"]==0){
                of=0;
                cf=0;
            }
            else{
                of=1;
                cf=1;
            }
        }
        else{
            if(registerMap["dx"]==0){
                of=0;
                cf=0;
            }
            else{
                of=1;
                cf=1;
            }
        }
    }
    else if(d == 4){

    }

}

//This method controls all possibilities of mov command and make changes.
void mov(string a, string b, string c){
    if(registerMap.find(a) != registerMap.end()){
        if(registerMap.find(b) != registerMap.end()){
            registerMap[a] = registerMap[b];
        }
        else if(b.at(0)>47 && b.at(0)<58){
            if(stoi(b) > 65535){
                control = false;
            }
            else{
                registerMap[a] = stoi(b);
            }
        }
        else if(b.at(0) == '[' && b.at(b.length()-1) == ']'){
            if(b == "[bx]" || b == "[di]" || b == "[si]" || b == "[bp]"){
                int mem = registerMap[b.substr(1, 2)];
                registerMap[a] = memory[mem] + memory[mem+1]*256;
            }
            else if (b.at(1)>47 && b.at(1)<58){
                int mem = stoi(b.substr(1,b.length()-2));
                registerMap[a] = memory[mem] + memory[mem+1] *256;
            }
            else{
                control = false;
            }
        }
        else if(b.at(0) == 'w' && b.at(1) == '[' && b.at(b.length()-1) == ']'){
            if(b == "w[bx]" || b == "w[di]" || b == "w[si]" || b == "w[bp]"){
                int mem = registerMap[b.substr(2, 2)];
                registerMap[a] = memory[mem] + memory[mem+1]*256;
            }
            else if (b.at(2)>47 && b.at(2)<58){
                int mem = stoi(b.substr(2,b.length()-3));
                registerMap[a] = memory[mem] + memory[mem+1] *256;
            }
            else{
                control = false;
            }
        }
        else if(b == "offset"){
            if(variables.find(c) != variables.end()){
                registerMap[a] = variables[c].second.second;
            }
            else{
                control = false;
            }
        }
        else if(variables.find(b) != variables.end()){
            registerMap[a] = variableValue(b);
        }
        else{
            control = false;
        }
    }
    else if(subRegisterMap.find(a) != subRegisterMap.end()){
        if(subRegisterMap.find(b) != subRegisterMap.end()){
            subRegisterMap[a] = subRegisterMap[b];
        }
        else if(b.at(0)>47 && b.at(0)<58){
            if(stoi(b) > 255){
                control = false;
            }
            else{
                subRegisterMap[a] = stoi(b);
            }
        }
        else if(b.at(0) == '[' && b.at(b.length()-1) == ']'){
            if(b == "[bx]" || b == "[di]" || b == "[si]" || b == "[bp]"){
                int mem = registerMap[b.substr(1, 2)];
                subRegisterMap[a] = memory[mem];
            }
            else if(b.at(1)>47 && b.at(1)<58){
                int mem = stoi(b.substr(1,b.length()-2));
                subRegisterMap[a] = memory[mem];
            }
            else {
                control = false;
            }
        }
        else if(b.at(0) == 'b' && b.at(1) == '[' && b.at(b.length()-1) == ']'){
            if(b == "b[bx]" || b == "b[di]" || b == "b[si]" || b == "b[bp]"){
                int mem = registerMap[b.substr(2, 2)];
                subRegisterMap[a] = memory[mem];
            }
            else if(b.at(2)>47 && b.at(2)<58){
                int mem = stoi(b.substr(2,b.length()-3));
                subRegisterMap[a] = memory[mem];
            }
            else {
                control = false;
            }
        }
        else if(b == "offset"){
            if(variables.find(c) != variables.end()){
                if(variables[c].second.second < 256){
                    subRegisterMap[a] = variables[c].second.second;
                }
                else{
                    control = false;
                }
            }
            else{
                control = false;
            }
        }
        else if(variables.find(b) != variables.end()){
            if(variables[b].first == "db"){
                subRegisterMap[a] = variableValue(b);
            }
            else{
                control = false;
            }
        }
        else{
            control = false;
        }
    }
    else if(a.at(0) == '[' && a.at(a.length()-1) == ']'){
        if(a == "[bx]" || a == "[di]" || a == "[si]" || a == "[bp]"){
            if(registerMap.find(b) != registerMap.end()){
                memory[registerMap[a.substr(1, 2)]] = registerMap[b] % 256;
                memory[registerMap[a.substr(1, 2)] + 1] = registerMap[b] / 256;
            }
            else if(b.at(0)>47 && b.at(0)<58){
                if(stoi(b) < 256){
                    memory[registerMap[a.substr(1, 2)]] = stoi(b);
                }
                else if(stoi(b) < 65536){
                    memory[registerMap[a.substr(1, 2)]] = stoi(b) % 256;
                    memory[registerMap[a.substr(1, 2)] + 1] = stoi(b) / 256;
                }
                else {
                    control = false;
                }
            }
            else if(b.at(0) == '[' && b.at(b.length()-1) == ']'){
                control = false;
            }
            else if(b == "offset"){
                if(variables.find(c) != variables.end()){
                    if(variables[c].second.second < 256){
                        memory[registerMap[a.substr(1, 2)]] = variables[c].second.second;
                    }
                    else if(variables[c].second.second < 65536){
                        memory[registerMap[a.substr(1, 2)]] = variables[c].second.second % 256;
                        memory[registerMap[a.substr(1, 2)] + 1] = variables[c].second.second / 256;
                    }
                    else {
                        control = false;
                    }
                }
                else{
                    control = false;
                }
            }
            else if(subRegisterMap.find(b) != subRegisterMap.end()){
                memory[registerMap[a.substr(1, 2)]] = subRegisterMap[b];
            }
            else if(variables.find(b) != variables.end()){
                if(variables[b].first == "db"){
                    memory[registerMap[a.substr(1, 2)]] = variableValue(b);
                }
                else if(variables[b].first == "dw"){
                    memory[registerMap[a.substr(1, 2)]] = variableValue(b) % 256;
                    memory[registerMap[a.substr(1, 2)] + 1] = variableValue(b) / 256;
                }
                else {
                    control = false;
                }
            }
            else{
                control = false;
            }
        }
        else if(a.at(1)>47 && a.at(1)<58){
            if(registerMap.find(b) != registerMap.end()){
                memory[stoi(a.substr(1, a.length()-2))] = registerMap[b] % 256;
                memory[stoi(a.substr(1, a.length()-2)) + 1] = registerMap[b] / 256;
            }
            else if(b.at(0)>47 && b.at(0)<58){
                if(stoi(b) < 256){
                    memory[stoi(a.substr(1, a.length()-2))] = stoi(b);
                }
                else if(stoi(b) < 65536){
                    memory[stoi(a.substr(1, a.length()-2))] = stoi(b) % 256;
                    memory[stoi(a.substr(1, a.length()-2)) + 1] = stoi(b) / 256;
                }
            }
            else if(b.at(0) == '[' && b.at(b.length()-1) == ']'){
                control = false;
            }
            else if(b == "offset"){
                if(variables.find(c) != variables.end()){
                    if(variables[c].second.second < 256){
                        memory[stoi(a.substr(1, a.length()-2))] = variables[c].second.second;
                    }
                    else if(variables[c].second.second < 65536){
                        memory[stoi(a.substr(1, a.length()-2))] = variables[c].second.second % 256;
                        memory[stoi(a.substr(1, a.length()-2)) + 1] = variables[c].second.second / 256;
                    }
                    else {
                        control = false;
                    }
                }
                else{
                    control = false;
                }
            }
            else if(subRegisterMap.find(b) != subRegisterMap.end()){
                memory[stoi(a.substr(1, a.length()-2))] = subRegisterMap[b];
            }
            else if(variables.find(b) != variables.end()){
                if(variables[b].first == "db"){
                    memory[stoi(a.substr(1, a.length()-2))] = variableValue(b);

                }
                else if(variables[b].first == "dw"){
                    memory[stoi(a.substr(1, a.length()-2))] = variableValue(b) % 256;
                    memory[stoi(a.substr(1, a.length()-2)) + 1] = variableValue(b) / 256;
                }
                else {
                    control = false;
                }
            }
            else{
                control = false;
            }
        }
    }
    else if(a.at(0) == 'b' && a.at(1) == '[' && a.at(a.length()-1) == ']'){
        if(a == "b[bx]" || a == "b[di]" || a == "b[si]" || a == "b[bp]"){
            if (subRegisterMap.find(b) != subRegisterMap.end()) {
                memory[registerMap[a.substr(2, 2)]] = subRegisterMap[b];
            }
            else if (b.at(0) < 58 && b.at(0) > 47) {
                if(stoi(b) /256 == 0){
                    memory[registerMap[a.substr(2, 2)]] = stoi(b);
                }
                else {
                    control = false;
                }
            }
            else if (b.at(0) == '[' && b.at(b.length() - 1) == ']') {
                control = false;
            }
            else if (b == "offset") {// tekrar bak
                if(variables.find(c)!=variables.end()){
                    if(variables[c].second.second < 256){
                        memory[registerMap[a.substr(2, 2)]]=variables[c].second.second;
                    }
                    else {
                        control = false;
                    }
                }
                else{
                    control = false;
                }
            }
            else if(variables.find(b)!=variables.end()){
                if(variables[c].first == "db"){
                    memory[registerMap[a.substr(2, 2)]] = variableValue(c);
                }
                else {
                    control = false;
                }
            }
            else{
                control =false;
            }
        }
        else if(a.at(2)>47 && a.at(2)<58 ){
            a = a.substr(2,a.length()-3);
            if (subRegisterMap.find(b) != subRegisterMap.end()) {
                memory[stoi(a)] = subRegisterMap[b];
            }
            else if (b.at(0) < 58 && b.at(0) > 47) {
                if(stoi(b) /256 == 0){
                    memory[stoi(a)] = stoi(b);
                }
                else {
                    control = false;
                }
            }
            else if (b.at(0) == '[' && b.at(b.length() - 1) == ']') {
                control = false;
            }
            else if (b == "offset") {// tekrar bak
                if(variables.find(c)!=variables.end()){
                    if(variables[c].second.second < 256){
                        memory[stoi(a)]=variables[c].second.second;
                    }
                    else{
                        control = false;
                    }
                }
                else{
                    control = false;
                }
            }
            else if(variables.find(b)!=variables.end()){
                if(variables[c].first == "db"){
                    memory[stoi(a)] = variableValue(c);
                }
                else {
                    control = false;
                }
            }
            else{
                control =false;
            }
        }
    }
    else if(a.at(0) == 'w' && a.at(1) == '[' && a.at(a.length()-1) == ']'){
        if( a.substr(1) == "[bx]" || a.substr(1) == "[di]" ||a.substr(1) == "[si]" ||a.substr(1) == "[bp]"){
            if (registerMap.find(b) != registerMap.end()) {
                int higher = registerMap[b] / 256;
                int lower = registerMap[b] % 256;
                memory[registerMap[a.substr(2, 2)]] = lower;
                memory[registerMap[a.substr(2, 2)]+1] = higher;
            }
            else if (b.at(0) < 58 && b.at(0) > 47) {
                int higher = stoi(b) / 256;
                int lower = stoi(b) % 256;
                memory[registerMap[a.substr(2, 2)]] = lower;
                memory[registerMap[a.substr(2, 2)] + 1] = higher;
            }
            else if (b.at(0) == '[' && b.at(b.length() - 1) == ']') {
                control = false;
            }
            else if (b == "offset") {
                if(variables.find(c)!=variables.end()){
                    memory[registerMap[a.substr(2, 2)]] = variables[c].second.second % 256;
                    memory[registerMap[a.substr(2, 2)] + 1] = variables[c].second.second / 256;
                }
                else{
                    control = false;
                }
            }
            else if(variables.find(b)!=variables.end()){
                int higher = variableValue(b) / 256;
                int lower = variableValue(b) % 256;
                memory[registerMap[a.substr(2, 2)]] = lower;
                memory[registerMap[a.substr(2, 2)]+1] = higher;
            }
            else{
                control =false;
            }
        }
        else if( a.at(2)>47 && a.at(2)<58 ){
            a = a.substr(2,a.length()-3);
            if (registerMap.find(b) != registerMap.end()) {
                int higher = registerMap[b] / 256;
                int lower = registerMap[b] % 256;
                memory[stoi(a)] = lower;
                memory[stoi(a)+1] = higher;
            }
            else if (b.at(0) < 58 && b.at(0) > 47) {
                int higher = stoi(b) / 256;
                int lower = stoi(b) % 256;
                memory[stoi(a)] = lower;
                memory[stoi(a)+1] = higher;
            }
            else if (b.at(0) == '[' && b.at(b.length() - 1) == ']') {
                control = false;
            }
            else if (b == "offset") {
                if(variables.find(c)!=variables.end()){
                    memory[stoi(a)] = variables[c].second.second % 256;
                    memory[stoi(a)+1] = variables[c].second.second / 256;
                }
                else{
                    control = false;
                }
            }
            else if(variables.find(b)!=variables.end()){
                int higher = variableValue(b) / 256;
                int lower = variableValue(b) % 256;
                memory[stoi(a)] = lower;
                memory[stoi(a)+1] = higher;
            }
            else{
                control =false;
            }
        }
    }
    else if(a == "offset" && variables.find(b)!=variables.end()){//tekrar dön buna
        if(variables[b].first == "db"){
            if (c == "bx" || c == "ax" || c == "cx" || c == "di" || c == "dx" || c == "si" || c == "bp") {
                if(registerMap[c]/256==0)
                    memory[variables[b].second.second] = registerMap[c];
                else{
                    control=false;
                }
            }
            else if (c.at(0) < 58 && c.at(0) > 47 && c.at(c.length() - 1) > 47 && c.at(c.length() - 1) < 58) {
                if(stoi(c) /256 == 0){
                    memory[variables[b].second.second] = registerMap[c];
                }
                else {
                    control = false;
                }
            }
            else if (c.at(0) == '[' && c.at(c.length() - 1) == ']') {
                if (c == "[bx]" || c == "[ax]" || c == "[cx]" || c == "[di]" || c == "[dx]" || c == "[si]" || c == "[bp]") {
                    int mem = registerMap[c.substr(1, 2)];
                    memory[variables[b].second.second] = memory[mem];
                }
                else {
                    int mem = stoi(c.substr(1, 2));
                    memory[variables[b].second.second] = memory[mem];
                }
            }
            else if(variables.find(c)!=variables.end()){
                if(variables[c].first == "db"){
                    memory[registerMap[a.substr(2, 2)]]= variableValue(c);
                }
                else {
                    control = false;
                }
            }
            else{
                control =false;
            }
        }
        else if(variables[b].first=="dw"){
            if (c == "bx" || c == "ax" || c == "cx" || c == "di" || c == "dx" || c == "si" || c == "bp") {
                int higher=registerMap[c]/256;
                int lower=registerMap[c]%256;
                memory[variables[b].second.second]=lower;
                memory[variables[b].second.second+1]=higher;
            } else if (c.at(0) < 58 && c.at(0) > 47 && c.at(c.length() - 1) > 47 && c.at(c.length() - 1) < 58) {
                int higher=stoi(c)/256;
                int lower=stoi(c)%256;
                memory[variables[b].second.second]=lower;
                memory[variables[b].second.second+1]=higher;
            } else if (c.at(0) == '[' && c.at(c.length() - 1) == ']') {
                if (c == "[bx]" || c == "[ax]" || c == "[cx]" || c == "[di]" || c == "[dx]" || c == "[si]" ||
                    c == "[bp]") {
                    int mem = registerMap[c.substr(1, 2)];
                    memory[variables[b].second.second] = memory[mem];
                    memory[variables[b].second.second+1]=0;
                } else {
                    int mem = stoi(c.substr(1, c.length() - 2));
                    memory[variables[b].second.second] = memory[mem];
                    memory[variables[b].second.second+1]=0;
                }
            } else if (variables.find(b) != variables.end()) {
                int higher=variableValue(b)/256;
                int lower=variableValue(b)%256;
                memory[registerMap[a.substr(2, 2)]] = lower;
                memory[registerMap[a.substr(2, 2)]+1] = higher;
            } else {
                control = false;
            }
        }
    }
    else if(variables.find(a) != variables.end()){
        if(variables[a].first == "db"){
            if(subRegisterMap.find(b) != subRegisterMap.end()){
                memory[variables[a].second.second] = subRegisterMap[b];
            }
            else if(b == "offset"){
                if(variables.find(c) != variables.end()){
                    if(variables[c].second.second < 256){
                        memory[variables[a].second.second] = variables[c].second.second;
                    }
                }
            }
            else if(b.at(0) > 47 && b.at(0)<58 && variables.find(b) == variables.end()){
                if(stoi(b) < 256){
                    memory[variables[a].second.second] = stoi(b);
                }
                else{
                    control = false;
                }
            }
        }
        else if (variables[a].first == "dw"){
            if(registerMap.find(b) != registerMap.end()){
                memory[variables[a].second.second] = registerMap[b];
            }
            else if(b == "offset"){
                if(variables.find(c) != variables.end()){
                    if(variables[c].second.second < 256){
                        memory[variables[a].second.second] = variables[c].second.second;
                    }
                    else {
                        memory[variables[a].second.second] = variables[c].second.second % 256;
                        memory[variables[a].second.second + 1] = variables[c].second.second / 256;
                    }
                }
            }
            else if(b.at(0) > 47 && b.at(0)<58 && variables.find(b) == variables.end()){
                memory[variables[a].second.second] = stoi(b) % 256;
                memory[variables[a].second.second + 1] = stoi(b)/ 256;
            }
        }
    }
}

//This method controls all possibilities of dec command and make changes.
void dec(string a){
    if(registerMap.find(a) != registerMap.end()){
        overflow(registerMap[a],1,17,registerMap[a]-1);
        registerMap[a]--;
        sign(registerMap[a],16);
        if(registerMap[a]==0){
            zf = 1;
        } else
            zf=0;
        auxiliary(registerMap[a],1);
    }
    else if(subRegisterMap.find(a) != subRegisterMap.end()){
        overflow(subRegisterMap[a],1,16,registerMap[a]-1);
        subRegisterMap[a]--;
        sign(subRegisterMap[a],8);
        if(subRegisterMap[a]==0){
            zf = 1;
        }else
            zf=0;
        auxiliary(subRegisterMap[a],1);
    }
    else if( a.at(0) == '[' && a.at(a.length() - 1) == ']'){
        string cv = a.substr(1,a.length()-2);
        if(cv=="bx" || cv=="bp" || cv=="di" || cv=="si"){
            overflow(memory[registerMap[a.substr(1,2)]],1,16,memory[registerMap[a.substr(1,2)]]-1);
            memory[registerMap[a.substr(1,2)]]--;
            sign(memory[registerMap[a.substr(1,2)]],8);
            if(memory[registerMap[a.substr(1,2)]]==0){
                zf=1;
            }else
                zf=0;
            auxiliary(memory[registerMap[a.substr(1,2)]],1);
        }
        else if (a.at(1) < 58 && a.at(1) > 47 && variables.find(cv)==variables.end()){
            overflow(memory[stoi(a.substr(1,a.length()-2))],1,16,memory[stoi(a.substr(1,a.length()-2))]-1);
            memory[stoi(a.substr(1,a.length()-2))]--;
            sign(memory[stoi(a.substr(1,a.length()-2))],8);
            if(memory[stoi(a.substr(1,a.length()-2))]==0){
                zf=1;
            }else
                zf=0;
            auxiliary(memory[stoi(a.substr(1,a.length()-2))],1);
        }
    }
    else if( a.substr(0,2) == "w[" && a.at(a.length() - 1) == ']'){
        string cv = a.substr(2,a.length()-3);
        if(cv=="bx" || cv=="bp" || cv=="di" || cv=="si"){
            int val=memory[registerMap[cv]]+memory[registerMap[cv]+1]*256;
            overflow(val,1,32,val-1);
            val--;
            memory[registerMap[cv]]=val%256;
            memory[registerMap[cv]+1]=val/256;
            sign(val,16);
            if(val==0){
                zf=1;
            }else
                zf=0;
            auxiliary(val,1);
        }
        else if (a.at(2) < 58 && a.at(2) > 47 && variables.find(cv)==variables.end()){
            int val=memory[stoi(cv)]+memory[stoi(cv)+1]*256;
            overflow(val,1,32,val-1);
            val--;
            memory[stoi(cv)]=val%256;
            memory[stoi(cv)+1]=val/256;
            sign(val,16);
            if(val==0){
                zf=1;
            }else
                zf=0;
            auxiliary(val,1);
        }
    }
    else if( a.substr(0,2) == "b[" && a.at(a.length() - 1) == ']'){
        string cv = a.substr(2,a.length()-3);
        if(cv=="bx" || cv=="bp" || cv=="di" || cv=="si"){
            overflow(memory[registerMap[cv]],1,16,memory[registerMap[cv]]-1);
            memory[registerMap[cv]]--;
            sign(memory[registerMap[cv]],8);
            if(memory[registerMap[cv]]==0){
                zf=1;
            }else
                zf=0;
            auxiliary(memory[registerMap[cv]],1);
        }
        else if (a.at(2) < 58 && a.at(2) > 47 && variables.find(cv)==variables.end()){
            overflow(memory[stoi(cv)],1,16,memory[stoi(cv)]-1);
            memory[stoi(cv)]--;
            sign(memory[stoi(cv)],8);
            if(memory[stoi(cv)]==0){
                zf=1;
            }else
                zf=0;
            auxiliary(memory[stoi(cv)],1);
        }
    }
    else if(variables.find(a)!=variables.end()){
        if(variables[a].first=="db" ){
            overflow(memory[variables[a].second.second],1,16,memory[variables[a].second.second]-1);
            memory[variables[a].second.second]--;
            sign(memory[variables[a].second.second],8);
            if(variableValue(a)==0){
                zf=1;
            }else
                zf=0;
            auxiliary(memory[variables[a].second.second],1);
        }
        else{
            overflow(variableValue(a),1,32,variableValue(a)-1);
            int val=variableValue(a)-1;
            memory[variables[a].second.second]=val%256;
            memory[variables[a].second.second+1]=val/256;
            sign(memory[variables[a].second.second],32);
            if(variableValue(a)==0){
                zf=1;
            }else
                zf=0;
            auxiliary(memory[variables[a].second.second],1);
        }
    }
}
//This method controls all possibilities of inc command and make changes.
void inc(string a){
    if(registerMap.find(a) != registerMap.end()){
        overflow(registerMap[a],1,17,registerMap[a]+1);
        auxiliary(registerMap[a],1);
        registerMap[a]++;
        sign(registerMap[a],16);
        if(registerMap[a]==0){
            zf = 1;
        } else
            zf=0;
    }
    else if(subRegisterMap.find(a) != subRegisterMap.end()){
        overflow(subRegisterMap[a],1,16,registerMap[a]+1);
        auxiliary(subRegisterMap[a],1);
        subRegisterMap[a]++;
        sign(subRegisterMap[a],8);
        if(subRegisterMap[a]==0){
            zf = 1;
        }else
            zf=0;
    }
    else if( a.at(0) == '[' && a.at(a.length() - 1) == ']'){
        string cv = a.substr(1,a.length()-2);
        if(cv=="bx" || cv=="bp" || cv=="di" || cv=="si"){
            overflow(memory[registerMap[cv]],1,16,memory[registerMap[cv]]+1);
            auxiliary(memory[registerMap[cv]],1);
            memory[registerMap[cv]]++;
            sign(memory[stoi(cv)],8);
            if(memory[registerMap[cv]]==0){
                zf=1;
            }else
                zf=0;
        }
        else if (a.at(1) < 58 && a.at(1) > 47 ){
            overflow(memory[stoi(cv)],1,16,memory[stoi(cv)]+1);
            auxiliary(memory[stoi(cv)],1);
            memory[stoi(cv)]++;
            sign(memory[stoi(cv)],8);
            if(memory[stoi(cv)]==0){
                zf=1;
            }else
                zf=0;
        }
    }
    else if( a.substr(0,2) == "w[" && a.at(a.length() - 1) == ']'){
        string cv = a.substr(2,a.length()-3);
        if(cv=="bx" || cv=="bp" || cv=="di" || cv=="si"){
            int val=memory[registerMap[cv]]+memory[registerMap[cv]+1]*256;
            overflow(val,1,32,val+1);
            auxiliary(val,1);
            val++;
            memory[registerMap[cv]]=val%256;
            memory[registerMap[cv]+1]=val/256;
            sign(val,16);
            if(val==0){
                zf=1;
            }else
                zf=0;
        }
        else if (a.at(2) < 58 && a.at(2) > 47 && variables.find(cv)==variables.end()){
            int val=memory[stoi(cv)]+memory[stoi(cv)+1]*256;
            overflow(val,1,32,val+1);
            auxiliary(val,1);
            val++;
            memory[stoi(cv)]=val%256;
            memory[stoi(cv)+1]=val/256;
            sign(val,16);
            if(val==0){
                zf=1;
            }else
                zf=0;
        }
    }
    else if( a.substr(0,2) == "b[" && a.at(a.length() - 1) == ']'){
        string cv = a.substr(2,a.length()-3);
        if(cv=="bx" || cv=="bp" || cv=="di" || cv=="si"){
            overflow(memory[registerMap[cv]],1,16,memory[registerMap[cv]]+1);
            auxiliary(memory[registerMap[cv]],1);
            memory[registerMap[cv]]++;
            sign(memory[registerMap[cv]],8);
            if(memory[registerMap[cv]]==0){
                zf=1;
            }else
                zf=0;
        }
        else if (a.at(2) < 58 && a.at(2) > 47 && variables.find(cv)==variables.end()){
            overflow(memory[stoi(cv)],1,16,memory[stoi(cv)]+1);
            auxiliary(memory[stoi(cv)],1);
            memory[stoi(cv)]++;
            sign(memory[stoi(cv)],8);
            if(memory[stoi(cv)]==0){
                zf=1;
            }else
                zf=0;
        }
    }
    else if(variables.find(a)!=variables.end()){
        if(variables[a].first=="db"){
            overflow(memory[variables[a].second.second],1,16,memory[variables[a].second.second]+1);
            auxiliary(memory[variables[a].second.second],1);
            memory[variables[a].second.second]++;
            sign(memory[variables[a].second.second],8);
            if(memory[variables[a].second.second]==0){
                zf=1;
            }else
                zf=0;
        }
        else{
            overflow(variableValue(a),1,32,variableValue(a)+1);
            auxiliary(memory[variables[a].second.second],1);
            int val=variableValue(a)+1;
            memory[variables[a].second.second]=val%256;
            memory[variables[a].second.second+1]=val/256;
            sign(memory[variables[a].second.second],32);
            if(memory[variables[a].second.second]==0){
                zf=1;
            }else
                zf=0;

        }
    }
}
//This method controls all possibilities of add command and make changes.
void add(string a, string b,string c){
    if(registerMap.find(a) != registerMap.end()){
        if(registerMap.find(b) != registerMap.end()){
            flag(registerMap[a],registerMap[b],17,1);
            registerMap[a] += registerMap[b];
        }
        else if(b.at(0)>47 && b.at(0)<58){
            if(stoi(b) > 65535){
                control = false;
            }
            else{
                flag(registerMap[a],stoi(b),32, 1);
                registerMap[a] += stoi(b);
            }
        }
        else if(b.at(0) == '[' && b.at(b.length()-1) == ']'){
            b = b.substr(1,b.length()-2);
            if(b == "bx" || b == "di" || b == "si" || b == "bp"){
                int x = memory[registerMap[b]]+ memory[registerMap[b]+1] * 256;
                flag(registerMap[a],x,32, 1);
                registerMap[a] += x;
            }
            else if(b.at(0)>47 && b.at(0)<58){
                int x = memory[stoi(b)]+ memory[stoi(b)+1] * 256;
                flag(registerMap[a],x,32, 1);
                registerMap[a] += x;
            }
        }
        else if(b.at(0) == 'w' && b.at(1) == '[' && b.at(b.length()-1) == ']'){
            b = b.substr(2,b.length()-3);
            if(b == "bx" || b == "di" || b == "si" || b == "bp"){
                int x = memory[registerMap[b]]+ memory[registerMap[b]+1] * 256;
                flag(registerMap[a],x,32, 1);
                registerMap[a] += x;
            }
            else if(b.at(0)>47 && b.at(0)<58){
                if(stoi(b) > 65535){
                    control = false;
                }
                else {
                    int x = memory[stoi(b)]+ memory[stoi(b)+1] * 256;
                    flag(registerMap[a],x,32, 1);
                    registerMap[a] += x;
                }
            }
        }
        else if(variables.find(b) != variables.end()){
            if(variables[b].first == "dw"){
                flag(registerMap[a],variableValue(b),32, 1);
                registerMap[a] += variableValue(b);
            }
            else {
                control = false;
            }
        }
        else if(b == "offset"){
            if(variables.find(c) != variables.end()){
                flag(registerMap[a],variables[c].second.second, 32, 1);
                registerMap[a] += variables[c].second.second;
            }
            else{
                control = false;
            }
        }
    }
    else if(subRegisterMap.find(a) != subRegisterMap.end()){
        if(subRegisterMap.find(b) != subRegisterMap.end()){
            flag(subRegisterMap[a],subRegisterMap[b],16, 1);
            subRegisterMap[a] += subRegisterMap[b];
        }
        else if(b.at(0)>47 && b.at(0)<58){
            if(stoi(b) > 255){
                control = false;
            }
            else {
                flag(subRegisterMap[a],stoi(b),16, 1);//bb 32 mi olacak kontrol et
                subRegisterMap[a] += stoi(b);
            }
        }
        else if(b.at(0) == '[' && b.at(b.length()-1) == ']'){
            b = b.substr(1,b.length()-2);
            if(b == "bx" || b == "di" || b == "si" || b == "bp"){
                flag(subRegisterMap[a],memory[registerMap[b]],16, 1);
                subRegisterMap[a] += memory[registerMap[b]];
            }
            else if(b.at(0)>47 && b.at(0)<58){
                flag(subRegisterMap[a],memory[stoi(b)],16, 1);
                subRegisterMap[a] += memory[stoi(b)];
            }
        }
        else if(b.at(0) == 'b' && b.at(1) == '[' && b.at(b.length()-1) == ']'){
            b = b.substr(2,b.length()-3);
            if(b == "bx" || b == "di" || b == "si" || b == "bp"){
                flag(subRegisterMap[a],memory[registerMap[b]],16, 1);
                subRegisterMap[a] += memory[registerMap[b]];
            }
            else if(b.at(0)>47 && b.at(0)<58){
                flag(subRegisterMap[a],memory[stoi(b)],16, 1);
                subRegisterMap[a] += memory[stoi(b)];
            }
        }
        else if(variables.find(b) != variables.end()){
            if(variables[b].first == "db"){
                flag(subRegisterMap[a],variableValue(b),16, 1);
                subRegisterMap[a] += variableValue(b);
            }
            else {
                control = false;
            }
        }
        else if(b == "offset"){
            if(variables.find(c) != variables.end()){
                if(variables[c].second.second < 256){
                    flag(subRegisterMap[a],variables[c].second.second, 16, 1);
                    subRegisterMap[a] += variables[c].second.second;
                }
                else{
                    control = false;
                }
            }
            else{
                control = false;
            }
        }
    }
    else if(a.at(0) == '[' && a.at(a.length()-1) == ']'){
        if(a == "[bx]" || a == "[di]" || a == "[si]" || a == "[bp]"){
            if(registerMap.find(b) != registerMap.end()){
                int mem  = memory[registerMap[a.substr(1, 2)]] + memory[registerMap[a.substr(1,2)] + 1] * 256;
                flag(mem, registerMap[b], 32, 1);
                memory[registerMap[a.substr(1, 2)]] = (mem+registerMap[b]) % 256;
                memory[registerMap[a.substr(1, 2)] + 1] = (mem+registerMap[b]) / 256;
            }
            else if(b.at(0)>47 && b.at(0)<58){
                if(stoi(b) < 256){
                    flag(memory[registerMap[a.substr(1, 2)]], stoi(b),16,1);
                    memory[registerMap[a.substr(1, 2)]] += stoi(b);
                }
                else if(stoi(b) < 65536){
                    int mem  = memory[registerMap[a.substr(1, 2)]] + memory[registerMap[a.substr(1,2)] + 1] * 256;
                    flag(mem, stoi(b), 32, 1);
                    memory[registerMap[a.substr(1, 2)]] = (mem + stoi(b)) % 256;
                    memory[registerMap[a.substr(1, 2)] + 1] = (mem + stoi(b)) / 256;
                }
                else {
                    control = false;
                }
            }
            else if(b.at(0) == '[' && b.at(b.length()-1) == ']'){
                control = false;
            }
            else if(b == "offset"){
                if(variables.find(c) != variables.end()){
                    if(variables[c].second.second < 256){
                        flag(memory[registerMap[a]], variables[c].second.second,16,1);
                        memory[registerMap[a.substr(1, 2)]] += variables[c].second.second;
                    }
                    else if(variables[c].second.second < 65536){
                        int mem  = memory[registerMap[a.substr(1, 2)]] + memory[registerMap[a.substr(1,2)] + 1] * 256;
                        flag(mem, variables[c].second.second, 32, 1);
                        memory[registerMap[a.substr(1, 2)]] = (mem + variables[c].second.second) % 256;
                        memory[registerMap[a.substr(1, 2)] + 1] = (mem + variables[c].second.second) / 256;
                    }
                    else {
                        control = false;
                    }
                }
                else{
                    control = false;
                }
            }
            else if(subRegisterMap.find(b) != subRegisterMap.end()){
                flag(memory[registerMap[a.substr(1, 2)]],subRegisterMap[b], 16,1);
                memory[registerMap[a.substr(1, 2)]] += subRegisterMap[b];
            }
            else if(variables.find(b) != variables.end()){
                control = false;
            }
            else{
                control = false;
            }
        }
        else if(a.at(1)>47 && a.at(1)<58){
            if(registerMap.find(b) != registerMap.end()){
                int mem  = memory[stoi(a.substr(1, a.length()-2))] + memory[stoi(a.substr(1, a.length()-2)) + 1] * 256;
                flag(mem, registerMap[b], 32, 1);
                memory[stoi(a.substr(1, a.length()-2))] = (mem+registerMap[b]) % 256;
                memory[stoi(a.substr(1, a.length()-2)) + 1] = (mem+registerMap[b]) / 256;
            }
            else if(b.at(0)>47 && b.at(0)<58){
                if(stoi(b) < 256){
                    flag(memory[stoi(a.substr(1,a.length()-2))], stoi(b),16,1);
                    memory[stoi(a.substr(1, a.length()-2))] += stoi(b);
                }
                else if(stoi(b) < 65536){
                    int mem  = memory[stoi(a.substr(1, a.length()-2))] + memory[stoi(a.substr(1, a.length()-2)) + 1] * 256;
                    flag(mem, stoi(b), 32, 1);
                    memory[stoi(a.substr(1, a.length()-2))] = (mem + stoi(b)) % 256;
                    memory[stoi(a.substr(1, a.length()-2)) + 1] = (mem + stoi(b)) / 256;
                }
            }
            else if(b.at(0) == '[' && b.at(b.length()-1) == ']'){
                control = false;
            }
            else if(b == "offset"){
                if(variables.find(c) != variables.end()){
                    if(variables[c].second.second < 256){
                        flag(memory[stoi(a.substr(1,a.length()-2))], variables[c].second.second,16,1);
                        memory[stoi(a.substr(1, a.length()-2))] += variables[c].second.second;
                    }
                    else if(variables[c].second.second < 65536){
                        int mem  = memory[stoi(a.substr(1, a.length()-2))] + memory[stoi(a.substr(1, a.length()-2)) + 1] * 256;
                        flag(mem, variables[c].second.second, 32, 1);
                        memory[stoi(a.substr(1, a.length()-2))] = (mem + variables[c].second.second) % 256;
                        memory[stoi(a.substr(1, a.length()-2)) + 1] = (mem + variables[c].second.second) / 256;
                    }
                    else {
                        control = false;
                    }
                }
                else{
                    control = false;
                }
            }
            else if(subRegisterMap.find(b) != subRegisterMap.end()){
                flag(memory[stoi(a.substr(1, a.length()-2))],subRegisterMap[b], 16,1);
                memory[stoi(a.substr(1, a.length()-2))] += subRegisterMap[b];
            }
            else if(variables.find(b) != variables.end()){
                control = false;
            }
            else{
                control = false;
            }
        }
    }
    else if(a.at(0) == 'b' && a.at(1) == '[' && a.at(a.length()-1) == ']'){
        if(a == "b[bx]" || a == "b[di]" || a == "b[si]" || a == "b[bp]"){
            if (subRegisterMap.find(b) != subRegisterMap.end()) {
                flag(memory[registerMap[a.substr(2, 2)]],subRegisterMap[b], 16,1);
                memory[registerMap[a.substr(2, 2)]] += subRegisterMap[b];
            }
            else if (b.at(0) < 58 && b.at(0) > 47) {
                if(stoi(b) /256 == 0){
                    flag(memory[registerMap[a.substr(2, 2)]],stoi(b), 16,1);
                    memory[registerMap[a.substr(2, 2)]] += stoi(b);
                }
                else {
                    control = false;
                }
            }
            else if (b.at(0) == '[' && b.at(b.length() - 1) == ']') {
                control = false;
            }
            else if (b == "offset") {// tekrar bak
                if(variables.find(c)!=variables.end()){
                    if(variables[c].second.second <256){
                        flag(memory[registerMap[a.substr(2, 2)]], variables[c].second.second,16,1);
                        memory[registerMap[a.substr(2, 2)]] += variables[c].second.second;
                    }
                    else {
                        control = false;
                    }
                }
                else{
                    control = false;
                }
            }
            else if(variables.find(b)!=variables.end()){
                control = false;
            }
            else{
                control =false;
            }
        }
        else if(a.at(2) < 58 && a.at(2) > 47){
            a = a.substr(2,a.length()-3);
            if (subRegisterMap.find(b) != subRegisterMap.end()) {
                flag(memory[stoi(a)],subRegisterMap[b], 16,1);
                memory[stoi(a)] += subRegisterMap[b];
            }
            else if (b.at(0) < 58 && b.at(0) > 47) {
                if(stoi(b) /256 == 0){
                    flag(memory[stoi(a)],stoi(b), 16,1);
                    memory[stoi(a)] += stoi(b);
                }
                else {
                    control = false;
                }
            }
            else if (b.at(0) == '[' && b.at(b.length() - 1) == ']') {
                control = false;
            }
            else if (b == "offset") {// tekrar bak
                if(variables.find(c)!=variables.end()){
                    if(variables[c].second.second <256){
                        flag(memory[stoi(a)], variables[c].second.second,16,1);
                        memory[stoi(a)] += variables[c].second.second;
                    }
                    else {
                        control = false;
                    }
                }
                else{
                    control = false;
                }
            }
            else if(variables.find(b)!=variables.end()){
                control = false;
            }
            else{
                control =false;
            }
        }
    }
    else if(a.at(0) == 'w' && a.at(1) == '[' && a.at(a.length()-1) == ']'){
        if( a.substr(1) == "[bx]" || a.substr(1) == "[di]" ||a.substr(1) == "[si]" ||a.substr(1) == "[bp]"){
            if (registerMap.find(b) != registerMap.end()) {
                int mem  = memory[registerMap[a.substr(2, 2)]] + memory[registerMap[a.substr(2,2)] + 1] * 256;
                flag(mem, registerMap[b], 32, 1);
                memory[registerMap[a.substr(2, 2)]] = (mem+registerMap[b]) % 256;
                memory[registerMap[a.substr(2, 2)] + 1] = (mem+registerMap[b]) / 256;
            }
            else if (b.at(0) < 58 && b.at(0) > 47) {
                int mem  = memory[registerMap[a.substr(2, 2)]] + memory[registerMap[a.substr(2,2)] + 1] * 256;
                flag(mem, stoi(b), 32, 1);
                memory[registerMap[a.substr(2, 2)]] = (mem + stoi(b)) % 256;
                memory[registerMap[a.substr(2, 2)] + 1] = (mem + stoi(b)) / 256;
            }
            else if (b.at(0) == '[' && b.at(b.length() - 1) == ']') {
                control = false;
            }
            else if (b == "offset") {
                if(variables.find(c)!=variables.end()){
                    int mem  = memory[registerMap[a.substr(2, 2)]] + memory[registerMap[a.substr(2,2)] + 1] * 256;
                    flag(mem, variables[c].second.second, 32, 1);
                    memory[registerMap[a.substr(2, 2)]] = (mem + variables[c].second.second) % 256;
                    memory[registerMap[a.substr(2, 2)] + 1] = (mem + variables[c].second.second) / 256;
                }
                else{
                    control = false;
                }
            }
            else{
                control =false;
            }
        }
        else if(a.at(2) < 58 && a.at(2) > 47){
            a = a.substr(2,a.length()-3);
            if (registerMap.find(b) != registerMap.end()) {
                int mem  = memory[stoi(a)] + memory[stoi(a)+1] * 256;
                flag(mem, registerMap[b], 32, 1);
                memory[stoi(a)] = (mem+registerMap[b]) % 256;
                memory[stoi(a)+1] = (mem+registerMap[b]) / 256;
            }
            else if (b.at(0) < 58 && b.at(0) > 47) {
                int mem  = memory[stoi(a)] + memory[stoi(a)+1] * 256;
                flag(mem, stoi(b), 32, 1);
                memory[stoi(a)] = (mem + stoi(b)) % 256;
                memory[stoi(a)+1] = (mem + stoi(b)) / 256;
            }
            else if (b.at(0) == '[' && b.at(b.length() - 1) == ']') {
                control = false;
            }
            else if (b == "offset") {
                if(variables.find(c)!=variables.end()){
                    int mem  = memory[stoi(a)] + memory[stoi(a)+1] * 256;
                    flag(mem, variables[c].second.second, 32, 1);
                    memory[stoi(a)] = (mem + variables[c].second.second) % 256;
                    memory[stoi(a)+1] = (mem + variables[c].second.second) / 256;
                }
                else{
                    control = false;
                }
            }
            else{
                control =false;
            }
        }
    }
    else if(variables.find(a) != variables.end()){
        if(variables[a].first == "db"){
            if(subRegisterMap.find(b) != subRegisterMap.end()){
                flag(memory[variables[a].second.second], subRegisterMap[b], 16, 1);
                memory[variables[a].second.second] += subRegisterMap[b];
            }
            else if(b == "offset"){
                if(variables.find(c) != variables.end()){
                    if(variables[c].second.second < 256){
                        flag(memory[variables[a].second.second], variables[c].second.second, 16, 1);
                        memory[variables[a].second.second] += variables[c].second.second;
                    }
                }
            }
            else if(b.at(0) > 47 && b.at(0) < 58 && variables.find(b) == variables.end()){
                if(stoi(b)< 256){
                    flag(memory[variables[a].second.second], stoi(b), 16, 1);
                    memory[variables[a].second.second] += stoi(b);
                }
                else {
                    control = false;
                }
            }
        }
        else if (variables[a].first == "dw"){
            if(registerMap.find(b) != registerMap.end()){
                int mem = variableValue(a) + registerMap[b];
                flag(variableValue(a), registerMap[b], 32, 1);
                memory[variables[a].second.second] = (mem) % 256;
                memory[variables[a].second.second + 1] = (mem) / 256;
            }
            else if(b == "offset"){
                if(variables.find(c) != variables.end()){
                    int mem = variableValue(a) + variables[c].second.second;
                    flag(variableValue(a),variables[c].second.second, 32, 1);
                    memory[variables[a].second.second] = (mem) % 256;
                    memory[variables[a].second.second + 1] = (mem) / 256;
                }
            }
            else if(b.at(0) > 47 && b.at(0) < 58 && variables.find(b) == variables.end()){
                int mem = variableValue(a) + stoi(b);
                flag(variableValue(a),stoi(b), 32, 1);
                memory[variables[a].second.second] = (mem) % 256;
                memory[variables[a].second.second + 1] = (mem) / 256;
            }
        }
    }
}
//This method controls all possibilities of sub command and make changes.
void sub(string a, string b,string c){
    if(registerMap.find(a) != registerMap.end()){
        if(registerMap.find(b) != registerMap.end()){
            flag(registerMap[a],registerMap[b],32,2);
            registerMap[a] -= registerMap[b];
        }
        else if(b.at(0)>47 && b.at(0)<58){
            if(stoi(b) > 65535){
                control = false;
            }
            else{
                flag(registerMap[a],stoi(b),32, 2);
                registerMap[a] -= stoi(b);
            }
        }
        else if(b.at(0) == '[' && b.at(b.length()-1) == ']'){
            b = b.substr(1,b.length()-2);
            if(b == "bx" || b == "di" || b == "si" || b == "bp"){
                int x = memory[registerMap[b]]+ memory[registerMap[b]+1] * 256;
                flag(registerMap[a],x,32, 2);
                registerMap[a] -= x;
            }
            else if(b.at(0)>47 && b.at(0)<58){
                int x = memory[stoi(b)]+ memory[stoi(b)+1] * 256;
                flag(registerMap[a],x,32, 2);
                registerMap[a] -= x;
            }
        }
        else if(b.at(0) == 'w' && b.at(1) == '[' && b.at(b.length()-1) == ']'){
            b = b.substr(2,b.length()-3);
            if(b == "bx" || b == "di" || b == "si" || b == "bp"){
                int x = memory[registerMap[b]]+ memory[registerMap[b]+1] * 256;
                flag(registerMap[a],x,32, 2);
                registerMap[a] -= x;
            }
            else if(b.at(0)>47 && b.at(0)<58){
                if(stoi(b) > 65535){
                    control = false;
                }
                else {
                    int x = memory[stoi(b)]+ memory[stoi(b)+1] * 256;
                    flag(registerMap[a],x,32, 2);
                    registerMap[a] -= x;
                }
            }
        }
        else if(variables.find(b) != variables.end()){
            if(variables[b].first == "db"){
                flag(registerMap[a],variableValue(b),32, 2);
                registerMap[a] -= variableValue(b);
            }
            else {
                control = false;
            }
        }
        else if(b == "offset"){
            if(variables.find(c) != variables.end()){
                flag(registerMap[a],variables[c].second.second, 32, 2);
                registerMap[a] -= variables[c].second.second;
            }
            else{
                control = false;
            }
        }
    }
    else if(subRegisterMap.find(a) != subRegisterMap.end()){
        if(subRegisterMap.find(b) != subRegisterMap.end()){
            flag(subRegisterMap[a],subRegisterMap[b],16, 2);
            subRegisterMap[a] -= subRegisterMap[b];
        }
        else if(b.at(0)>47 && b.at(0)<58){
            if(stoi(b) > 255){
                control = false;
            }
            else {
                flag(subRegisterMap[a],stoi(b),16, 2);//bb 32 mi olacak kontrol et
                subRegisterMap[a] -= stoi(b);
            }
        }
        else if(b.at(0) == '[' && b.at(b.length()-1) == ']'){
            b = b.substr(1,b.length()-2);
            if(b == "bx" || b == "di" || b == "si" || b == "bp"){
                flag(subRegisterMap[a],memory[registerMap[b]],16, 2);
                subRegisterMap[a] -= memory[registerMap[b]];
            }
            else if(b.at(0)>47 && b.at(0)<58){
                flag(subRegisterMap[a],memory[stoi(b)],16, 2);
                subRegisterMap[a] -= memory[stoi(b)];
            }
        }
        else if(b.at(0) == 'b' && b.at(1) == '[' && b.at(b.length()-1) == ']'){
            b = b.substr(2,b.length()-3);
            if(b == "bx" || b == "di" || b == "si" || b == "bp"){
                flag(subRegisterMap[a],memory[registerMap[b]],16, 2);
                subRegisterMap[a] -= memory[registerMap[b]];
            }
            else if(b.at(0)>47 && b.at(0)<58){
                flag(subRegisterMap[a],memory[stoi(b)],16, 2);
                subRegisterMap[a] -= memory[stoi(b)];
            }
        }
        else if(variables.find(b) != variables.end()){
            if(variables[b].first == "db"){
                flag(subRegisterMap[a],variableValue(b),16, 2);
                subRegisterMap[a] -= variableValue(b);
            }
            else {
                control = false;
            }
        }
        else if(b == "offset"){
            if(variables.find(c) != variables.end()){
                if(variables[c].second.second < 256){
                    flag(subRegisterMap[a],variables[c].second.second, 16, 2);
                    subRegisterMap[a] -= variables[c].second.second;
                }
                else{
                    control = false;
                }

            }
            else{
                control = false;
            }
        }
    }
    else if(a.at(0) == '[' && a.at(a.length()-1) == ']'){
        if(a == "[bx]" || a == "[di]" || a == "[si]" || a == "[bp]"){
            if(registerMap.find(b) != registerMap.end()){
                int mem  = memory[registerMap[a.substr(1, 2)]] + memory[registerMap[a.substr(1,2)] + 1] * 256;
                flag(mem, registerMap[b], 32, 2);
                memory[registerMap[a.substr(1, 2)]] = (mem - registerMap[b]) % 256;
                memory[registerMap[a.substr(1, 2)] + 1] = (mem - registerMap[b]) / 256;
            }
            else if(b.at(0)>47 && b.at(0)<58){
                if(stoi(b) < 256){
                    flag(memory[registerMap[a.substr(1, 2)]], stoi(b),16,2);
                    memory[registerMap[a.substr(1, 2)]] -= stoi(b);
                }
                else if(stoi(b) < 65536){
                    int mem  = memory[registerMap[a.substr(1, 2)]] + memory[registerMap[a.substr(1,2)] + 1] * 256;
                    flag(mem, stoi(b), 32, 2);
                    memory[registerMap[a.substr(1, 2)]] = (mem - stoi(b)) % 256;
                    memory[registerMap[a.substr(1, 2)] + 1] = (mem - stoi(b)) / 256;
                }
                else {
                    control = false;
                }
            }
            else if(b.at(0) == '[' && b.at(b.length()-1) == ']'){
                control = false;
            }
            else if(b == "offset"){
                if(variables.find(c) != variables.end()){
                    if(variables[c].second.second < 256){
                        flag(memory[registerMap[a]], variables[c].second.second,16,2);
                        memory[registerMap[a.substr(1, 2)]] -= variables[c].second.second;
                    }
                    else if(variables[c].second.second < 65536){
                        int mem  = memory[registerMap[a.substr(1, 2)]] + memory[registerMap[a.substr(1,2)] + 1] * 256;
                        flag(mem, variables[c].second.second, 32, 2);
                        memory[registerMap[a.substr(1, 2)]] = (mem - variables[c].second.second) % 256;
                        memory[registerMap[a.substr(1, 2)] + 1] = (mem - variables[c].second.second) / 256;
                    }
                    else {
                        control = false;
                    }
                }
                else{
                    control = false;
                }
            }
            else if(subRegisterMap.find(b) != subRegisterMap.end()){
                flag(memory[registerMap[a.substr(1, 2)]],subRegisterMap[b], 16,2);
                memory[registerMap[a.substr(1, 2)]] -= subRegisterMap[b];
            }
            else if(variables.find(b) != variables.end()){
                control = false;
            }
            else{
                control = false;
            }
        }
        else if(a.at(1)>47 && a.at(1)<58){
            if(registerMap.find(b) != registerMap.end()){
                int mem  = memory[stoi(a.substr(1, a.length()-2))] + memory[stoi(a.substr(1, a.length()-2)) + 1] * 256;
                flag(mem, registerMap[b], 32, 2);
                memory[stoi(a.substr(1, a.length()-2))] = (mem-registerMap[b]) % 256;
                memory[stoi(a.substr(1, a.length()-2)) + 1] = (mem-registerMap[b]) / 256;
            }
            else if(b.at(0)>47 && b.at(0)<58){
                if(stoi(b) < 256){
                    flag(memory[stoi(a.substr(1,a.length()-2))], stoi(b),16,2);
                    memory[stoi(a.substr(1, a.length()-2))] -= stoi(b);
                }
                else if(stoi(b) < 65536){
                    int mem  = memory[stoi(a.substr(1, a.length()-2))] + memory[stoi(a.substr(1, a.length()-2)) + 1] * 256;
                    flag(mem, stoi(b), 32, 2);
                    memory[stoi(a.substr(1, a.length()-2))] = (mem - stoi(b)) % 256;
                    memory[stoi(a.substr(1, a.length()-2)) + 1] = (mem - stoi(b)) / 256;
                }
            }
            else if(b.at(0) == '[' && b.at(b.length()-1) == ']'){
                control = false;
            }
            else if(b == "offset"){
                if(variables.find(c) != variables.end()){
                    if(variables[c].second.second < 256){
                        flag(memory[stoi(a.substr(1,a.length()-2))], variables[c].second.second,16,2);
                        memory[stoi(a.substr(1, a.length()-2))] -= variables[c].second.second;
                    }
                    else if(variables[c].second.second < 65536){
                        int mem  = memory[stoi(a.substr(1, a.length()-2))] + memory[stoi(a.substr(1, a.length()-2)) + 1] * 256;
                        flag(mem, variables[c].second.second, 32, 2);
                        memory[stoi(a.substr(1, a.length()-2))] = (mem - variables[c].second.second) % 256;
                        memory[stoi(a.substr(1, a.length()-2)) + 1] = (mem - variables[c].second.second) / 256;
                    }
                    else {
                        control = false;
                    }
                }
                else{
                    control = false;
                }
            }
            else if(subRegisterMap.find(b) != subRegisterMap.end()){
                flag(memory[stoi(a.substr(1, a.length()-2))],subRegisterMap[b], 16,2);
                memory[stoi(a.substr(1, a.length()-2))] -= subRegisterMap[b];
            }
            else if(variables.find(b) != variables.end()){
                control = false;
            }
            else{
                control = false;
            }
        }
    }
    else if(a.at(0) == 'b' && a.at(1) == '[' && a.at(a.length()-1) == ']'){
        if(a == "b[bx]" || a == "b[di]" || a == "b[si]" || a == "b[bp]"){
            if (subRegisterMap.find(b) != subRegisterMap.end()) {
                flag(memory[registerMap[a.substr(2, 2)]],subRegisterMap[b], 16,2);
                memory[registerMap[a.substr(2, 2)]] -= subRegisterMap[b];
            }
            else if (b.at(0) < 58 && b.at(0) > 47) {
                if(stoi(b) /256 == 0){
                    flag(memory[registerMap[a.substr(2, 2)]],stoi(b), 16,2);
                    memory[registerMap[a.substr(2, 2)]] -= stoi(b);
                }
                else {
                    control = false;
                }
            }
            else if (b.at(0) == '[' && b.at(b.length() - 1) == ']') {
                control = false;
            }
            else if (b == "offset") {// tekrar bak
                if(variables.find(c)!=variables.end()){
                    if(variables[c].second.second <256){
                        flag(memory[registerMap[a.substr(2, 2)]], variables[c].second.second,16,2);
                        memory[registerMap[a.substr(2, 2)]] -= variables[c].second.second;
                    }
                    else {
                        control = false;
                    }
                }
                else{
                    control = false;
                }
            }
            else if(variables.find(b)!=variables.end()){
                control = false;
            }
            else{
                control =false;
            }
        }
        else if(a.at(2) < 58 && a.at(2) > 47){
            a = a.substr(2,a.length()-3);
            if (subRegisterMap.find(b) != subRegisterMap.end()) {
                flag(memory[stoi(a)],subRegisterMap[b], 16,2);
                memory[stoi(a)] -= subRegisterMap[b];
            }
            else if (b.at(0) < 58 && b.at(0) > 47) {
                if(stoi(b) /256 == 0){
                    flag(memory[stoi(a)],stoi(b), 16,2);
                    memory[stoi(a)] -= stoi(b);
                }
                else {
                    control = false;
                }
            }
            else if (b.at(0) == '[' && b.at(b.length() - 1) == ']') {
                control = false;
            }
            else if (b == "offset") {// tekrar bak
                if(variables.find(c)!=variables.end()){
                    if(variables[c].second.second <256){
                        flag(memory[stoi(a)], variables[c].second.second,16,2);
                        memory[stoi(a)] -= variables[c].second.second;
                    }
                    else {
                        control = false;
                    }
                }
                else{
                    control = false;
                }
            }
            else if(variables.find(b)!=variables.end()){
                control = false;
            }
            else{
                control =false;
            }
        }
    }
    else if(a.at(0) == 'w' && a.at(1) == '[' && a.at(a.length()-1) == ']'){
        if( a.substr(1) == "[bx]" || a.substr(1) == "[di]" ||a.substr(1) == "[si]" ||a.substr(1) == "[bp]"){
            if (registerMap.find(b) != registerMap.end()) {
                int mem  = memory[registerMap[a.substr(2, 2)]] + memory[registerMap[a.substr(2,2)] + 1] * 256;
                flag(mem, registerMap[b], 32, 2);
                memory[registerMap[a.substr(2, 2)]] = (mem - registerMap[b]) % 256;
                memory[registerMap[a.substr(2, 2)] + 1] = (mem - registerMap[b]) / 256;
            }
            else if (b.at(0) < 58 && b.at(0) > 47) {
                int mem  = memory[registerMap[a.substr(2, 2)]] + memory[registerMap[a.substr(2,2)] + 1] * 256;
                flag(mem, stoi(b), 32, 2);
                memory[registerMap[a.substr(2, 2)]] = (mem - stoi(b)) % 256;
                memory[registerMap[a.substr(2, 2)] + 1] = (mem - stoi(b)) / 256;
            }
            else if (b.at(0) == '[' && b.at(b.length() - 1) == ']') {
                control = false;
            }
            else if (b == "offset") {
                if(variables.find(c)!=variables.end()){
                    int mem  = memory[registerMap[a.substr(2, 2)]] + memory[registerMap[a.substr(2,2)] + 1] * 256;
                    flag(mem, variables[c].second.second, 32, 2);
                    memory[registerMap[a.substr(2, 2)]] = (mem - variables[c].second.second) % 256;
                    memory[registerMap[a.substr(2, 2)] + 1] = (mem - variables[c].second.second) / 256;
                }
                else{
                    control = false;
                }
            }
            else{
                control =false;
            }
        }
        else if(a.at(2) < 58 && a.at(2) > 47){
            a = a.substr(2,a.length()-3);
            if (registerMap.find(b) != registerMap.end()) {
                int mem  = memory[stoi(a)] + memory[stoi(a) + 1] * 256;
                flag(mem, registerMap[b], 32, 2);
                memory[stoi(a)] = (mem - registerMap[b]) % 256;
                memory[stoi(a) + 1] = (mem - registerMap[b]) / 256;
            }
            else if (b.at(0) < 58 && b.at(0) > 47) {
                int mem  = memory[stoi(a)] + memory[stoi(a) + 1] * 256;
                flag(mem, stoi(b), 32, 2);
                memory[stoi(a)] = (mem - stoi(b)) % 256;
                memory[stoi(a) + 1] = (mem - stoi(b)) / 256;
            }
            else if (b.at(0) == '[' && b.at(b.length() - 1) == ']') {
                control = false;
            }
            else if (b == "offset") {
                if(variables.find(c)!=variables.end()){
                    int mem  = memory[stoi(a)] + memory[stoi(a) + 1] * 256;
                    flag(mem, variables[c].second.second, 32, 2);
                    memory[stoi(a)] = (mem - variables[c].second.second) % 256;
                    memory[stoi(a) + 1] = (mem - variables[c].second.second) / 256;
                }
                else{
                    control = false;
                }
            }
            else{
                control =false;
            }
        }
    }
    else if(variables.find(a) != variables.end()){
        if(variables[a].first == "db"){
            if(subRegisterMap.find(b) != subRegisterMap.end()){
                flag(memory[variables[a].second.second], subRegisterMap[b], 16, 2);
                memory[variables[a].second.second] -= subRegisterMap[b];
            }
            else if(b == "offset"){
                if(variables.find(c) != variables.end()){
                    if(variables[c].second.second < 256){
                        flag(memory[variables[a].second.second], variables[c].second.second, 16, 2);
                        memory[variables[a].second.second] -= variables[c].second.second;
                    }
                }
            }
            else if(b.at(0) > 47 && b.at(0) < 58 && variables.find(b) == variables.end()){
                if(stoi(b)< 256){
                    flag(memory[variables[a].second.second], stoi(b), 16, 2);
                    memory[variables[a].second.second] -= stoi(b);
                }
                else {
                    control = false;
                }
            }
        }
        else if (variables[a].first == "dw"){
            if(registerMap.find(b) != registerMap.end()){
                int mem = variableValue(a) - registerMap[b];
                flag(variableValue(a), registerMap[b], 32, 2);
                memory[variables[a].second.second] = (mem) % 256;
                memory[variables[a].second.second + 1] = (mem) / 256;
            }
            else if(b == "offset"){
                if(variables.find(c) != variables.end()){
                    int mem = variableValue(a) - variables[c].second.second;
                    flag(variableValue(a),variables[c].second.second, 32, 2);
                    memory[variables[a].second.second] = (mem) % 256;
                    memory[variables[a].second.second + 1] = (mem) / 256;
                }
            }
            else if(b.at(0) > 47 && b.at(0) < 58 && variables.find(b) == variables.end()){
                int mem = variableValue(a) - stoi(b);
                flag(variableValue(a),stoi(b), 32, 2);
                memory[variables[a].second.second] = (mem) % 256;
                memory[variables[a].second.second + 1] = (mem) / 256;
            }
        }
    }
}
//This method controls all possibilities of mul command and make changes.
void mul(string a){
    if(registerMap.find(a) != registerMap.end()){
        int val = registerMap["ax"] * registerMap[a];
        registerMap["dx"] = val / 65536;
        registerMap["ax"] = val % 65536;
        flag(registerMap["ax"],registerMap[a],32,3);
    }
    else if(subRegisterMap.find(a) != subRegisterMap.end()){
        int val = subRegisterMap["al"] * subRegisterMap[a];
        registerMap["ax"] = val;
        flag(subRegisterMap["al"],subRegisterMap[a],16,3);
    }
    else if(a.at(0) == 'w' &&a.at(1) == '[' && a.at(a.length()-1) == ']'){
        a = a.substr(2,a.length()-3);
        if(a == "bx" || a == "di" || a == "si" || a == "bp"){
            int mem = memory[registerMap[a]] + memory[registerMap[a]+1] * 256;
            int val = registerMap["ax"] * mem;
            registerMap["dx"] = val / 65536;
            registerMap["ax"] = val % 65536;
            flag(registerMap["ax"],mem,32,3);
        }
        else if(a.at(0)>47 && a.at(0)<58){
            int mem = memory[stoi(a)] + memory[stoi(a)+1] * 256;
            int val = registerMap["ax"] * mem;
            registerMap["dx"] = val / 65536;
            registerMap["ax"] = val % 65536;
            flag(registerMap["ax"],mem,32,3);
        }
    }
    else if(a.at(0) == 'b' &&a.at(1) == '[' && a.at(a.length()-1) == ']'){
        a = a.substr(2,a.length()-3);
        if(a == "bx" || a == "di" || a == "si" || a == "bp"){
            int mem = memory[registerMap[a]];
            int val = subRegisterMap["al"] * mem;
            registerMap["ax"] = val;
            flag(subRegisterMap["al"],mem,16,3);
        }
        else if(a.at(0)>47 && a.at(0)<58){
            int mem = memory[stoi(a)];
            int val = subRegisterMap["al"] * mem;
            registerMap["ax"] = val;
            flag(subRegisterMap["al"],mem,16,3);
        }
    }
    else if(variables.find(a) != variables.end()){
        if(variables[a].first == "db"){
            int val = subRegisterMap["al"] * variableValue(a);
            registerMap["ax"] = val;
            flag(subRegisterMap["al"],variableValue(a),16,3);
        }
        else if(variables[a].first == "dw"){
            int val = registerMap["ax"] * variableValue(a);
            registerMap["dx"] = val / 65536;
            registerMap["ax"] = val % 65536;
            flag(registerMap["ax"],variableValue(a),32,3);
        }
    }
}
//This method controls all possibilities of div command and make changes.
void div(string a){
    if(registerMap.find(a) != registerMap.end()){
        int mem = registerMap["dx"] * 65536 + registerMap["ax"];
        flag(mem,registerMap[a],32,4);
        int val1 = mem / registerMap[a];
        if(val1 > 65535){
            cout << "Error"<< endl;
        }
        else{
            int val2 = mem % registerMap[a];
            registerMap["dx"] = val2;
            registerMap["ax"] = val1;
        }
    }
    else if(subRegisterMap.find(a) != subRegisterMap.end()){
        flag(registerMap["ax"],subRegisterMap[a],16,4);
        int val1 = registerMap["ax"] / subRegisterMap[a];
        if(val1 > 255){
            cout << "Error"<< endl;
        }
        else {
            int val2 = registerMap["ax"] % subRegisterMap[a];
            subRegisterMap["ah"] = val2;
            subRegisterMap["al"] = val1;
        }
    }
    else if(a.at(0) == 'w' &&a.at(1) == '[' && a.at(a.length()-1) == ']'){
        a = a.substr(2,a.length()-3);
        if(a == "bx" || a == "di" || a == "si" || a == "bp"){
            int mem = registerMap["dx"] * 65536 + registerMap["ax"];
            flag(mem,memory[registerMap[a]],32,4);
            int val1 = mem / memory[registerMap[a]];
            if(val1 > 65535){
                cout << "Error"<< endl;
            }
            else{
                int val2 = mem % memory[registerMap[a]];
                registerMap["dx"] = val2;
                registerMap["ax"] = val1;
            }
        }
        else if(a.at(0)>47 && a.at(0)<58){
            int mem = registerMap["dx"] * 65536 + registerMap["ax"];
            flag(mem,memory[stoi(a)],32,4);
            int val1 = mem / memory[stoi(a)];
            if(val1 > 65535){
                cout << "Error"<< endl;
            }
            else{
                int val2 = mem % memory[stoi(a)];
                registerMap["dx"] = val2;
                registerMap["ax"] = val1;
            }
        }
    }
    else if(a.at(0) == 'b' &&a.at(1) == '[' && a.at(a.length()-1) == ']'){
        a = a.substr(2,a.length()-3);
        if(a == "bx" || a == "di" || a == "si" || a == "bp"){
            flag(registerMap["ax"],memory[registerMap[a]],16,4);
            int val1 = registerMap["ax"] / memory[registerMap[a]];
            if(val1 > 255){
                cout << "Error"<< endl;
            }
            else {
                int val2 = registerMap["ax"] % memory[registerMap[a]];
                subRegisterMap["ah"] = val2;
                subRegisterMap["al"] = val1;
            }
        }
        else if(a.at(0)>47 && a.at(0)<58){
            flag(registerMap["ax"],memory[stoi(a)],16,4);
            int val1 = registerMap["ax"] / memory[stoi(a)];
            if(val1 > 255){
                cout << "Error"<< endl;
            }
            else {
                int val2 = registerMap["ax"] % memory[stoi(a)];
                subRegisterMap["ah"] = val2;
                subRegisterMap["al"] = val1;
            }
        }
    }
    else if(variables.find(a) != variables.end()){
        if(variables[a].first == "db"){
            flag(registerMap["ax"],variableValue(a),16,4);
            int val1 = registerMap["ax"] / variableValue(a);
            if(val1 > 255){
                cout << "Error"<< endl;
            }
            else {
                int val2 = registerMap["ax"] % variableValue(a);
                subRegisterMap["ah"] = val2;
                subRegisterMap["al"] = val1;
            }
        }
        else if(variables[a].first == "dw"){
            int mem = registerMap["dx"] * 65536 + registerMap["ax"];
            flag(mem,variableValue(a),32,4);
            int val1 = mem / variableValue(a);
            if(val1 > 65535){
                cout << "Error"<< endl;
            }
            else{
                int val2 = mem % variableValue(a);
                registerMap["dx"] = val2;
                registerMap["ax"] = val1;
            }
        }
    }
}
//This method helps and1 method. It changes flags and returns values.
int and2(int a, int b, int bit){
    int c = 0;
    for(int i = 0; i < bit; i++){
        if(a % 2 == 1 && b % 2 == 1){
            c += pow(2,i);
        }
        a = a / 2;
        b = b / 2;
    }
    if(c == 0){
        zf = 1;
    }
    else{
        zf = 0;
    }
    cf = 0;
    of = 0;
    sf = c / (int)pow(2,bit-1);
    return c;
}
//This method controls all possibilities of and command and make changes.
void and1(string a, string b,string c){
    if(registerMap.find(a) != registerMap.end()){
        if(registerMap.find(b) != registerMap.end()){
            registerMap[a] = and2(registerMap[a],registerMap[b],16);
        }
        else if(b.at(0)>47 && b.at(0)<58 && variables.find(b) == variables.end()){
            if(stoi(b) > 65535){
                control = false;
            }
            else{
                registerMap[a] = and2(registerMap[a],stoi(b),16);
            }
        }
        else if(b.at(0) == '[' && b.at(b.length()-1) == ']'){
            b = b.substr(1,b.length()-2);
            if(b == "bx" || b == "di" || b == "si" || b == "bp"){
                int x = memory[registerMap[b]]+ memory[registerMap[b]+1] * 256;
                registerMap[a] = and2(registerMap[a], x,16);
            }
            else if(b.at(0)>47 && b.at(0)<58){
                int x = memory[stoi(b)]+ memory[stoi(b)+1] * 256;
                registerMap[a] = and2(registerMap[a], x,16);
            }
        }
        else if(b.at(0) == 'w' && b.at(1) == '[' && b.at(b.length()-1) == ']'){
            b = b.substr(2,b.length()-3);
            if(b == "bx" || b == "di" || b == "si" || b == "bp"){
                int x = memory[registerMap[b]]+ memory[registerMap[b]+1] * 256;
                registerMap[a] = and2(registerMap[a], x,16);
            }
            else if(b.at(0)>47 && b.at(0)<58){
                if(stoi(b) > 65535){
                    control = false;
                }
                else {
                    int x = memory[stoi(b)]+ memory[stoi(b)+1] * 256;
                    registerMap[a] = and2(registerMap[a], x,16);
                }
            }
        }
        else if(variables.find(b) != variables.end()){
            if(variables[b].first == "dw"){
                registerMap[a] = and2(registerMap[a], variableValue(b),16);
            }
            else {
                control = false;
            }
        }
        else if(b == "offset"){
            if(variables.find(c) != variables.end()){
                registerMap[a] = and2(registerMap[a], variables[c].second.second,16);
            }
            else{
                control = false;
            }
        }
    }
    else if(subRegisterMap.find(a) != subRegisterMap.end()){
        if(subRegisterMap.find(b) != subRegisterMap.end()){
            subRegisterMap[a] = and2(subRegisterMap[a], subRegisterMap[b],8);
        }
        else if(b.at(0)>47 && b.at(0)<58 && variables.find(b) == variables.end()){// bunu diğer tüm metodlarda ekle
            if(stoi(b) > 255){
                control = false;
            }
            else {
                subRegisterMap[a] = and2(subRegisterMap[a], stoi(b),8);
            }
        }
        else if(b.at(0) == '[' && b.at(b.length()-1) == ']'){
            b = b.substr(1,b.length()-2);
            if(b == "bx" || b == "di" || b == "si" || b == "bp"){
                subRegisterMap[a] = and2(subRegisterMap[a], memory[registerMap[b]],8);
            }
            else if(b.at(0)>47 && b.at(0)<58){
                subRegisterMap[a] = and2(subRegisterMap[a], memory[stoi(b)],8);
            }
        }
        else if(b.at(0) == 'b' && b.at(1) == '[' && b.at(b.length()-1) == ']'){
            b = b.substr(2,b.length()-3);
            if(b == "bx" || b == "di" || b == "si" || b == "bp"){
                subRegisterMap[a] = and2(subRegisterMap[a], memory[registerMap[b]],8);
            }
            else if(b.at(0)>47 && b.at(0)<58){
                subRegisterMap[a] = and2(subRegisterMap[a], memory[stoi(b)],8);
            }
        }
        else if(variables.find(b) != variables.end()){
            if(variables[b].first == "db"){
                subRegisterMap[a] = and2(subRegisterMap[a], variableValue(b),8);
            }
            else {
                control = false;
            }
        }
        else if(b == "offset"){
            if(variables.find(c) != variables.end()){
                if(variables[c].second.second < 256){
                    subRegisterMap[a] = and2(subRegisterMap[a], variables[c].second.second,8);
                }
                else{
                    control = false;
                }
            }
            else{
                control = false;
            }
        }
    }
    else if(a.at(0) == '[' && a.at(a.length()-1) == ']'){
        if(a == "[bx]" || a == "[di]" || a == "[si]" || a == "[bp]"){
            if(registerMap.find(b) != registerMap.end()){
                int mem  = memory[registerMap[a.substr(1, 2)]] + memory[registerMap[a.substr(1,2)] + 1] * 256;
                int val =  and2(mem, registerMap[b],16);
                memory[registerMap[a.substr(1, 2)]] = (val) % 256;
                memory[registerMap[a.substr(1, 2)] + 1] = (val) / 256;
            }
            else if(b.at(0)>47 && b.at(0)<58 && variables.find(b) == variables.end()){
                if(stoi(b) < 256){
                    memory[registerMap[a.substr(1, 2)]] = and2(memory[registerMap[a.substr(1, 2)]], stoi(b), 8);
                }
                else if(stoi(b) < 65536){
                    int mem  = memory[registerMap[a.substr(1, 2)]] + memory[registerMap[a.substr(1,2)] + 1] * 256;
                    int val =  and2(mem, stoi(b),16);
                    memory[registerMap[a.substr(1, 2)]] = (val) % 256;
                    memory[registerMap[a.substr(1, 2)] + 1] = (val) / 256;
                }
                else {
                    control = false;
                }
            }
            else if(b.at(0) == '[' && b.at(b.length()-1) == ']'){
                control = false;
            }
            else if(b == "offset"){
                if(variables.find(c) != variables.end()){
                    if(variables[c].second.second < 256){
                        memory[registerMap[a.substr(1, 2)]] = and2(memory[registerMap[a.substr(1, 2)]],variables[c].second.second,8);
                    }
                    else if(variables[c].second.second < 65536){
                        int mem  = memory[registerMap[a.substr(1, 2)]] + memory[registerMap[a.substr(1,2)] + 1] * 256;
                        int val =  and2(mem, variables[c].second.second,16);
                        memory[registerMap[a.substr(1, 2)]] = (val) % 256;
                        memory[registerMap[a.substr(1, 2)] + 1] = (val) / 256;
                    }
                    else {
                        control = false;
                    }
                }
                else{
                    control = false;
                }
            }
            else if(subRegisterMap.find(b) != subRegisterMap.end()){
                memory[registerMap[a.substr(1, 2)]] = and2( memory[registerMap[a.substr(1, 2)]], subRegisterMap[b],8);
            }
            else if(variables.find(b) != variables.end()){
                control = false;
            }
            else{
                control = false;
            }
        }
        else if(a.at(1)>47 && a.at(1)<58){
            if(registerMap.find(b) != registerMap.end()){
                int mem  = memory[stoi(a.substr(1, a.length()-2))] + memory[stoi(a.substr(1, a.length()-2)) + 1] * 256;
                int val =  and2(mem, registerMap[b],16);
                memory[stoi(a.substr(1, a.length()-2))] = (val) % 256;
                memory[stoi(a.substr(1, a.length()-2)) + 1] = (val) / 256;
            }
            else if(b.at(0)>47 && b.at(0)<58 && variables.find(b) == variables.end()){
                if(stoi(b) < 256){
                    memory[stoi(a.substr(1, a.length()-2))] = and2(memory[stoi(a.substr(1, a.length()-2))], stoi(b),8);
                }
                else if(stoi(b) < 65536){
                    int mem  = memory[stoi(a.substr(1, a.length()-2))] + memory[stoi(a.substr(1, a.length()-2)) + 1] * 256;
                    int val =  and2(mem, stoi(b),16);
                    memory[stoi(a.substr(1, a.length()-2))] = (val) % 256;
                    memory[stoi(a.substr(1, a.length()-2)) + 1] = (val) / 256;
                }
            }
            else if(b.at(0) == '[' && b.at(b.length()-1) == ']'){
                control = false;
            }
            else if(b == "offset"){
                if(variables.find(c) != variables.end()){
                    if(variables[c].second.second < 256){
                        memory[stoi(a.substr(1, a.length()-2))] = and2(memory[stoi(a.substr(1, a.length()-2))],variables[c].second.second,8);
                    }
                    else if(variables[c].second.second < 65536){
                        int mem  = memory[stoi(a.substr(1, a.length()-2))] + memory[stoi(a.substr(1, a.length()-2)) + 1] * 256;
                        int val =  and2(mem, variables[c].second.second,16);
                        memory[stoi(a.substr(1, a.length()-2))] = (val) % 256;
                        memory[stoi(a.substr(1, a.length()-2)) + 1] = (val) / 256;
                    }
                    else {
                        control = false;
                    }
                }
                else{
                    control = false;
                }
            }
            else if(subRegisterMap.find(b) != subRegisterMap.end()){
                memory[stoi(a.substr(1, a.length()-2))] = and2(memory[stoi(a.substr(1, a.length()-2))],subRegisterMap[b],8);
            }
            else if(variables.find(b) != variables.end()){
                control = false;
            }
            else{
                control = false;
            }
        }
    }
    else if(a.at(0) == 'b' && a.at(1) == '[' && a.at(a.length()-1) == ']'){
        if(a == "b[bx]" || a == "b[di]" || a == "b[si]" || a == "b[bp]"){
            if (subRegisterMap.find(b) != subRegisterMap.end()) {
                memory[registerMap[a.substr(2, 2)]] = and2(memory[registerMap[a.substr(2, 2)]],subRegisterMap[b],8);
            }
            else if (b.at(0) < 58 && b.at(0) > 47) {
                if(stoi(b) /256 == 0){
                    memory[registerMap[a.substr(2, 2)]] = and2(memory[registerMap[a.substr(2, 2)]],stoi(b),8);
                }
                else {
                    control = false;
                }
            }
            else if (b.at(0) == '[' && b.at(b.length() - 1) == ']') {
                control = false;
            }
            else if (b == "offset") {// tekrar bak
                if(variables.find(c)!=variables.end()){
                    if(variables[c].second.second <256){
                        memory[registerMap[a.substr(2, 2)]] = and2(memory[registerMap[a.substr(2, 2)]],variables[c].second.second,8);
                    }
                    else {
                        control = false;
                    }
                }
                else{
                    control = false;
                }
            }
            else if(variables.find(b)!=variables.end()){
                control = false;
            }
            else{
                control =false;
            }
        }
        else if(a.at(2) < 58 && a.at(2) > 47){
            a = a.substr(2,a.length()-3);
            if (subRegisterMap.find(b) != subRegisterMap.end()) {
                memory[stoi(a)] = and2(memory[stoi(a)],subRegisterMap[b],8);
            }
            else if (b.at(0) < 58 && b.at(0) > 47) {
                if(stoi(b) /256 == 0){
                    memory[stoi(a)] = and2(memory[stoi(a)],stoi(b),8);
                }
                else {
                    control = false;
                }
            }
            else if (b.at(0) == '[' && b.at(b.length() - 1) == ']') {
                control = false;
            }
            else if (b == "offset") {// tekrar bak
                if(variables.find(c)!=variables.end()){
                    if(variables[c].second.second <256){
                        memory[stoi(a)] = and2(memory[stoi(a)],variables[c].second.second,8);
                    }
                    else {
                        control = false;
                    }
                }
                else{
                    control = false;
                }
            }
            else if(variables.find(b)!=variables.end()){
                control = false;
            }
            else{
                control =false;
            }
        }

    }
    else if(a.at(0) == 'w' && a.at(1) == '[' && a.at(a.length()-1) == ']'){
        if( a.substr(1) == "[bx]" || a.substr(1) == "[di]" ||a.substr(1) == "[si]" ||a.substr(1) == "[bp]"){
            if (registerMap.find(b) != registerMap.end()) {
                int mem  = memory[registerMap[a.substr(2, 2)]] + memory[registerMap[a.substr(2,2)] + 1] * 256;
                int val =  and2(mem, registerMap[a],16);
                memory[registerMap[a.substr(2, 2)]] = val % 256;
                memory[registerMap[a.substr(2, 2)] + 1] = val / 256;
            }
            else if (b.at(0) < 58 && b.at(0) > 47 && variables.find(b) == variables.end()) {
                int mem  = memory[registerMap[a.substr(2, 2)]] + memory[registerMap[a.substr(2,2)] + 1] * 256;
                int val =  and2(mem, stoi(b),16);
                memory[registerMap[a.substr(2, 2)]] = val % 256;
                memory[registerMap[a.substr(2, 2)] + 1] = val / 256;
            }
            else if (b.at(0) == '[' && b.at(b.length() - 1) == ']') {
                control = false;
            }
            else if (b == "offset") {
                if(variables.find(c)!=variables.end()){
                    int mem  = memory[registerMap[a.substr(2, 2)]] + memory[registerMap[a.substr(2,2)] + 1] * 256;
                    int val =  and2(mem, variables[c].second.second,16);
                    memory[registerMap[a.substr(2, 2)]] = val % 256;
                    memory[registerMap[a.substr(2, 2)] + 1] = val / 256;
                }
                else{
                    control = false;
                }
            }
            else{
                control =false;
            }
        }
        if(a.at(2) < 58 && a.at(2) > 47){
            a = a.substr(2,a.length()-3);
            if (registerMap.find(b) != registerMap.end()) {
                int mem  = memory[stoi(a)] + memory[stoi(a) + 1] * 256;
                int val =  and2(mem, registerMap[a],16);
                memory[stoi(a)] = val % 256;
                memory[stoi(a) + 1] = val / 256;
            }
            else if (b.at(0) < 58 && b.at(0) > 47 && variables.find(b) == variables.end()) {
                int mem  = memory[stoi(a)] + memory[stoi(a) + 1] * 256;
                int val =  and2(mem, stoi(b),16);
                memory[stoi(a)] = val % 256;
                memory[stoi(a) + 1] = val / 256;
            }
            else if (b.at(0) == '[' && b.at(b.length() - 1) == ']') {
                control = false;
            }
            else if (b == "offset") {
                if(variables.find(c)!=variables.end()){
                    int mem  = memory[stoi(a)] + memory[stoi(a) + 1] * 256;
                    int val =  and2(mem, variables[c].second.second,16);
                    memory[stoi(a)] = val % 256;
                    memory[stoi(a) + 1] = val / 256;
                }
                else{
                    control = false;
                }
            }
            else{
                control =false;
            }
        }
    }
    else if(variables.find(a) != variables.end()){
        if(variables[a].first == "db"){
            if(subRegisterMap.find(b) != subRegisterMap.end()){
                memory[variables[a].second.second] = and2(memory[variables[a].second.second],subRegisterMap[b],8);
            }
            else if(b == "offset"){
                if(variables.find(c) != variables.end()){
                    if(variables[c].second.second < 256){
                        memory[variables[a].second.second] = and2(memory[variables[a].second.second],variables[c].second.second,8);
                    }
                }
            }
            else if(b.at(0) < 58 && b.at(0) > 47 && variables.find(b) == variables.end()){
                if(stoi(b) < 256){
                    memory[variables[a].second.second] = and2(memory[variables[a].second.second],stoi(b),8);
                }
                else{
                    control = false;
                }
            }
        }
        else if (variables[a].first == "dw"){
            if(registerMap.find(b) != registerMap.end()){
                int val =  and2(variableValue(a),registerMap[b],16);
                memory[variables[a].second.second] = (val) % 256;
                memory[variables[a].second.second + 1] = (val) / 256;
            }
            else if(b == "offset"){
                if(variables.find(c) != variables.end()){
                    int val = and2(variableValue(a),variables[c].second.second,16);
                    memory[variables[a].second.second] = (val) % 256;
                    memory[variables[a].second.second + 1] = (val) / 256;
                }
            }
            else if(b.at(0) < 58 && b.at(0) > 47 && variables.find(b) == variables.end()){
                int val = and2(variableValue(a),stoi(b),16);
                memory[variables[a].second.second] = (val) % 256;
                memory[variables[a].second.second + 1] = (val) / 256;
            }
        }
    }
}
//This method helps or1 method. It changes flags and returns values.
int or2(int a, int b, int bit){
    int c = 0;
    for(int i = 0; i < bit; i++){
        if(a % 2 == 1 || b % 2 == 1){
            c += pow(2,i);
        }
        a = a / 2;
        b = b / 2;
    }
    if(c == 0){
        zf = 1;
    }
    else{
        zf = 0;
    }
    cf = 0;
    of = 0;
    sf = c / (int)pow(2,bit-1);
    return c;
}
//This method controls all possibilities of or command and make changes.
void or1(string a, string b,string c){
    if(registerMap.find(a) != registerMap.end()){
        if(registerMap.find(b) != registerMap.end()){
            registerMap[a] = or2(registerMap[a],registerMap[b],16);
        }
        else if(b.at(0)>47 && b.at(0)<58 && variables.find(b) == variables.end()){
            if(stoi(b) > 65535){
                control = false;
            }
            else{
                registerMap[a] = or2(registerMap[a],stoi(b),16);
            }
        }
        else if(b.at(0) == '[' && b.at(b.length()-1) == ']'){
            b = b.substr(1,b.length()-2);
            if(b == "bx" || b == "di" || b == "si" || b == "bp"){
                int x = memory[registerMap[b]]+ memory[registerMap[b]+1] * 256;
                registerMap[a] = or2(registerMap[a], x,16);
            }
            else if(b.at(0)>47 && b.at(0)<58){
                int x = memory[stoi(b)]+ memory[stoi(b)+1] * 256;
                registerMap[a] = or2(registerMap[a], x,16);
            }
        }
        else if(b.at(0) == 'w' && b.at(1) == '[' && b.at(b.length()-1) == ']'){
            b = b.substr(2,b.length()-3);
            if(b == "bx" || b == "di" || b == "si" || b == "bp"){
                int x = memory[registerMap[b]]+ memory[registerMap[b]+1] * 256;
                registerMap[a] = or2(registerMap[a], x,16);
            }
            else if(b.at(0)>47 && b.at(0)<58){
                if(stoi(b) > 65535){
                    control = false;
                }
                else {
                    int x = memory[stoi(b)]+ memory[stoi(b)+1] * 256;
                    registerMap[a] = or2(registerMap[a], x,16);
                }
            }
        }
        else if(variables.find(b) != variables.end()){
            if(variables[b].first == "dw"){
                registerMap[a] = or2(registerMap[a], variableValue(b),16);
            }
            else {
                control = false;
            }
        }
        else if(b == "offset"){
            if(variables.find(c) != variables.end()){
                registerMap[a] = or2(registerMap[a], variables[c].second.second,16);
            }
            else{
                control = false;
            }
        }
    }
    else if(subRegisterMap.find(a) != subRegisterMap.end()){
        if(subRegisterMap.find(b) != subRegisterMap.end()){
            subRegisterMap[a] = or2(subRegisterMap[a], subRegisterMap[b],8);
        }
        else if(b.at(0)>47 && b.at(0)<58 && variables.find(b) == variables.end()){// bunu diğer tüm metodlarda ekle
            if(stoi(b) > 255){
                control = false;
            }
            else {
                subRegisterMap[a] = or2(subRegisterMap[a], stoi(b),8);
            }
        }
        else if(b.at(0) == '[' && b.at(b.length()-1) == ']'){
            b = b.substr(1,b.length()-2);
            if(b == "bx" || b == "di" || b == "si" || b == "bp"){
                subRegisterMap[a] = or2(subRegisterMap[a], memory[registerMap[b]],8);
            }
            else if(b.at(0)>47 && b.at(0)<58){
                subRegisterMap[a] = or2(subRegisterMap[a], memory[stoi(b)],8);
            }
        }
        else if(b.at(0) == 'b' && b.at(1) == '[' && b.at(b.length()-1) == ']'){
            b = b.substr(2,b.length()-3);
            if(b == "bx" || b == "di" || b == "si" || b == "bp"){
                subRegisterMap[a] = or2(subRegisterMap[a], memory[registerMap[b]],8);
            }
            else if(b.at(0)>47 && b.at(0)<58){
                subRegisterMap[a] = or2(subRegisterMap[a], memory[stoi(b)],8);
            }
        }
        else if(variables.find(b) != variables.end()){
            if(variables[b].first == "db"){
                subRegisterMap[a] = or2(subRegisterMap[a], variableValue(b),8);
            }
            else {
                control = false;
            }
        }
        else if(b == "offset"){
            if(variables.find(c) != variables.end()){
                if(variables[c].second.second < 256){
                    subRegisterMap[a] = or2(subRegisterMap[a], variables[c].second.second,8);
                }
                else{
                    control = false;
                }
            }
            else{
                control = false;
            }
        }
    }
    else if(a.at(0) == '[' && a.at(a.length()-1) == ']'){
        if(a == "[bx]" || a == "[di]" || a == "[si]" || a == "[bp]"){
            if(registerMap.find(b) != registerMap.end()){
                int mem  = memory[registerMap[a.substr(1, 2)]] + memory[registerMap[a.substr(1,2)] + 1] * 256;
                int val =  or2(mem, registerMap[b],16);
                memory[registerMap[a.substr(1, 2)]] = (val) % 256;
                memory[registerMap[a.substr(1, 2)] + 1] = (val) / 256;
            }
            else if(b.at(0)>47 && b.at(0)<58 && variables.find(b) == variables.end()){
                if(stoi(b) < 256){
                    memory[registerMap[a.substr(1, 2)]] = or2(memory[registerMap[a.substr(1, 2)]], stoi(b), 8);
                }
                else if(stoi(b) < 65536){
                    int mem  = memory[registerMap[a.substr(1, 2)]] + memory[registerMap[a.substr(1,2)] + 1] * 256;
                    int val =  or2(mem, stoi(b),16);
                    memory[registerMap[a.substr(1, 2)]] = (val) % 256;
                    memory[registerMap[a.substr(1, 2)] + 1] = (val) / 256;
                }
                else {
                    control = false;
                }
            }
            else if(b.at(0) == '[' && b.at(b.length()-1) == ']'){
                control = false;
            }
            else if(b == "offset"){
                if(variables.find(c) != variables.end()){
                    if(variables[c].second.second < 256){
                        memory[registerMap[a.substr(1, 2)]] = or2(memory[registerMap[a.substr(1, 2)]],variables[c].second.second,8);
                    }
                    else if(variables[c].second.second < 65536){
                        int mem  = memory[registerMap[a.substr(1, 2)]] + memory[registerMap[a.substr(1,2)] + 1] * 256;
                        int val =  or2(mem, variables[c].second.second,16);
                        memory[registerMap[a.substr(1, 2)]] = (val) % 256;
                        memory[registerMap[a.substr(1, 2)] + 1] = (val) / 256;
                    }
                    else {
                        control = false;
                    }
                }
                else{
                    control = false;
                }
            }
            else if(subRegisterMap.find(b) != subRegisterMap.end()){
                memory[registerMap[a.substr(1, 2)]] = or2( memory[registerMap[a.substr(1, 2)]], subRegisterMap[b],8);
            }
            else if(variables.find(b) != variables.end()){
                control = false;
            }
            else{
                control = false;
            }
        }
        else if(a.at(1)>47 && a.at(1)<58){
            if(registerMap.find(b) != registerMap.end()){
                int mem  = memory[stoi(a.substr(1, a.length()-2))] + memory[stoi(a.substr(1, a.length()-2)) + 1] * 256;
                int val =  or2(mem, registerMap[b],16);
                memory[stoi(a.substr(1, a.length()-2))] = (val) % 256;
                memory[stoi(a.substr(1, a.length()-2)) + 1] = (val) / 256;
            }
            else if(b.at(0)>47 && b.at(0)<58 && variables.find(b) == variables.end()){
                if(stoi(b) < 256){
                    memory[stoi(a.substr(1, a.length()-2))] = or2(memory[stoi(a.substr(1, a.length()-2))], stoi(b),8);
                }
                else if(stoi(b) < 65536){
                    int mem  = memory[stoi(a.substr(1, a.length()-2))] + memory[stoi(a.substr(1, a.length()-2)) + 1] * 256;
                    int val =  or2(mem, stoi(b),16);
                    memory[stoi(a.substr(1, a.length()-2))] = (val) % 256;
                    memory[stoi(a.substr(1, a.length()-2)) + 1] = (val) / 256;
                }
            }
            else if(b.at(0) == '[' && b.at(b.length()-1) == ']'){
                control = false;
            }
            else if(b == "offset"){
                if(variables.find(c) != variables.end()){
                    if(variables[c].second.second < 256){
                        memory[stoi(a.substr(1, a.length()-2))] = or2(memory[stoi(a.substr(1, a.length()-2))],variables[c].second.second,8);
                    }
                    else if(variables[c].second.second < 65536){
                        int mem  = memory[stoi(a.substr(1, a.length()-2))] + memory[stoi(a.substr(1, a.length()-2)) + 1] * 256;
                        int val =  or2(mem, variables[c].second.second,16);
                        memory[stoi(a.substr(1, a.length()-2))] = (val) % 256;
                        memory[stoi(a.substr(1, a.length()-2)) + 1] = (val) / 256;
                    }
                    else {
                        control = false;
                    }
                }
                else{
                    control = false;
                }
            }
            else if(subRegisterMap.find(b) != subRegisterMap.end()){
                memory[stoi(a.substr(1, a.length()-2))] = or2(memory[stoi(a.substr(1, a.length()-2))],subRegisterMap[b],8);
            }
            else if(variables.find(b) != variables.end()){
                control = false;
            }
            else{
                control = false;
            }
        }
    }
    else if(a.at(0) == 'b' && a.at(1) == '[' && a.at(a.length()-1) == ']'){
        if(a == "b[bx]" || a == "b[di]" || a == "b[si]" || a == "b[bp]"){
            if (subRegisterMap.find(b) != subRegisterMap.end()) {
                memory[registerMap[a.substr(2, 2)]] = or2(memory[registerMap[a.substr(2, 2)]],subRegisterMap[b],8);
            }
            else if (b.at(0) < 58 && b.at(0) > 47) {
                if(stoi(b) /256 == 0){
                    memory[registerMap[a.substr(2, 2)]] = or2(memory[registerMap[a.substr(2, 2)]],stoi(b),8);
                }
                else {
                    control = false;
                }
            }
            else if (b.at(0) == '[' && b.at(b.length() - 1) == ']') {
                control = false;
            }
            else if (b == "offset") {// tekrar bak
                if(variables.find(c)!=variables.end()){
                    if(variables[c].second.second <256){
                        memory[registerMap[a.substr(2, 2)]] = or2(memory[registerMap[a.substr(2, 2)]],variables[c].second.second,8);
                    }
                    else {
                        control = false;
                    }
                }
                else{
                    control = false;
                }
            }
            else if(variables.find(b)!=variables.end()){
                control = false;
            }
            else{
                control =false;
            }
        }
        else if(a.at(2) < 58 && a.at(2) > 47){
            a = a.substr(2,a.length()-3);
            if (subRegisterMap.find(b) != subRegisterMap.end()) {
                memory[stoi(a)] = or2(memory[stoi(a)],subRegisterMap[b],8);
            }
            else if (b.at(0) < 58 && b.at(0) > 47) {
                if(stoi(b) /256 == 0){
                    memory[stoi(a)] = or2(memory[stoi(a)],stoi(b),8);
                }
                else {
                    control = false;
                }
            }
            else if (b.at(0) == '[' && b.at(b.length() - 1) == ']') {
                control = false;
            }
            else if (b == "offset") {// tekrar bak
                if(variables.find(c)!=variables.end()){
                    if(variables[c].second.second <256){
                        memory[stoi(a)] = or2(memory[stoi(a)],variables[c].second.second,8);
                    }
                    else {
                        control = false;
                    }
                }
                else{
                    control = false;
                }
            }
            else if(variables.find(b)!=variables.end()){
                control = false;
            }
            else{
                control =false;
            }
        }
    }
    else if(a.at(0) == 'w' && a.at(1) == '[' && a.at(a.length()-1) == ']'){
        if( a.substr(1) == "[bx]" || a.substr(1) == "[di]" ||a.substr(1) == "[si]" ||a.substr(1) == "[bp]"){
            if (registerMap.find(b) != registerMap.end()) {
                int mem  = memory[registerMap[a.substr(2, 2)]] + memory[registerMap[a.substr(2,2)] + 1] * 256;
                int val =  or2(mem, registerMap[b],16);
                memory[registerMap[a.substr(2, 2)]] = val % 256;
                memory[registerMap[a.substr(2, 2)] + 1] = val / 256;
            }
            else if (b.at(0) < 58 && b.at(0) > 47 && variables.find(b) == variables.end()) {
                int mem  = memory[registerMap[a.substr(2, 2)]] + memory[registerMap[a.substr(2,2)] + 1] * 256;
                int val =  or2(mem, stoi(b),16);
                memory[registerMap[a.substr(2, 2)]] = val % 256;
                memory[registerMap[a.substr(2, 2)] + 1] = val / 256;
            }
            else if (b.at(0) == '[' && b.at(b.length() - 1) == ']') {
                control = false;
            }
            else if (b == "offset") {
                if(variables.find(c)!=variables.end()){
                    int mem  = memory[registerMap[a.substr(2, 2)]] + memory[registerMap[a.substr(2,2)] + 1] * 256;
                    int val =  or2(mem, variables[c].second.second,16);
                    memory[registerMap[a.substr(2, 2)]] = val % 256;
                    memory[registerMap[a.substr(2, 2)] + 1] = val / 256;
                }
                else{
                    control = false;
                }
            }
            else{
                control =false;
            }
        }
        else if( a.at(2) < 58 && a.at(2) > 47){
            a = a.substr(2,a.length()-3);
            if (registerMap.find(b) != registerMap.end()) {
                int mem  = memory[stoi(a)] + memory[stoi(a) + 1] * 256;
                int val =  or2(mem, registerMap[b],16);
                memory[stoi(a)] = val % 256;
                memory[stoi(a) + 1] = val / 256;
            }
            else if (b.at(0) < 58 && b.at(0) > 47 && variables.find(b) == variables.end()) {
                int mem  = memory[stoi(a)] + memory[stoi(a) + 1] * 256;
                int val =  or2(mem, stoi(b),16);
                memory[stoi(a)] = val % 256;
                memory[stoi(a) + 1] = val / 256;
            }
            else if (b.at(0) == '[' && b.at(b.length() - 1) == ']') {
                control = false;
            }
            else if (b == "offset") {
                if(variables.find(c)!=variables.end()){
                    int mem  = memory[stoi(a)] + memory[stoi(a) + 1] * 256;
                    int val =  or2(mem, variables[c].second.second,16);
                    memory[stoi(a)] = val % 256;
                    memory[stoi(a) + 1] = val / 256;
                }
                else{
                    control = false;
                }
            }
            else{
                control =false;
            }
        }
    }
    else if(variables.find(a) != variables.end()){
        if(variables[a].first == "db"){
            if(subRegisterMap.find(b) != subRegisterMap.end()){
                memory[variables[a].second.second] = or2(memory[variables[a].second.second],subRegisterMap[b],8);
            }
            else if(b == "offset"){
                if(variables.find(c) != variables.end()){
                    if(variables[c].second.second < 256){
                        memory[variables[a].second.second] = or2(memory[variables[a].second.second],variables[c].second.second,8);
                    }
                }
            }
            else if(b.at(0) < 58 && b.at(0) >47 && variables.find(b) == variables.end()){
                if(variables[c].second.second < 256){
                    memory[variables[a].second.second] = or2(memory[variables[a].second.second],stoi(b),8);
                }
                else{
                    control = false;
                }
            }
        }
        else if (variables[a].first == "dw"){
            if(registerMap.find(b) != registerMap.end()){
                int val =  or2(variableValue(a),registerMap[b],16);
                memory[variables[a].second.second] = (val) % 256;
                memory[variables[a].second.second + 1] = (val) / 256;
            }
            else if(b == "offset"){
                if(variables.find(c) != variables.end()){
                    int val = or2(variableValue(a),variables[c].second.second,16);
                    memory[variables[a].second.second] = (val) % 256;
                    memory[variables[a].second.second + 1] = (val) / 256;
                }
            }
            else if(b.at(0) < 58 && b.at(0) >47 && variables.find(b) == variables.end()){
                int val = or2(variableValue(a),stoi(b),16);
                memory[variables[a].second.second] = (val) % 256;
                memory[variables[a].second.second + 1] = (val) / 256;
            }
        }
    }
}
//This method helps xor1 method. It changes flags and returns values.
int xor2(int a, int b, int bit){
    int c = 0;
    for(int i = 0; i < bit; i++){
        if((a % 2) xor (b % 2)){
            c += pow(2,i);
        }
        a = a / 2;
        b = b / 2;
    }
    if(c == 0){
        zf = 1;
    }
    else{
        zf = 0;
    }
    cf = 0;
    of = 0;
    sf = c / (int)pow(2,bit-1);
    return c;
}
//This method controls all possibilities of xor command and make changes.
void xor1(string a, string b,string c){
    if(registerMap.find(a) != registerMap.end()){
        if(registerMap.find(b) != registerMap.end()){
            registerMap[a] = xor2(registerMap[a],registerMap[b],16);
        }
        else if(b.at(0)>47 && b.at(0)<58 && variables.find(b) == variables.end()){
            if(stoi(b) > 65535){
                control = false;
            }
            else{
                registerMap[a] = xor2(registerMap[a],stoi(b),16);
            }
        }
        else if(b.at(0) == '[' && b.at(b.length()-1) == ']'){
            b = b.substr(1,b.length()-2);
            if(b == "bx" || b == "di" || b == "si" || b == "bp"){
                int x = memory[registerMap[b]]+ memory[registerMap[b]+1] * 256;
                registerMap[a] = xor2(registerMap[a], x,16);
            }
            else if(b.at(0)>47 && b.at(0)<58){
                int x = memory[stoi(b)]+ memory[stoi(b)+1] * 256;
                registerMap[a] = xor2(registerMap[a], x,16);
            }
        }
        else if(b.at(0) == 'w' && b.at(1) == '[' && b.at(b.length()-1) == ']'){
            b = b.substr(2,b.length()-3);
            if(b == "bx" || b == "di" || b == "si" || b == "bp"){
                int x = memory[registerMap[b]]+ memory[registerMap[b]+1] * 256;
                registerMap[a] = xor2(registerMap[a], x,16);
            }
            else if(b.at(0)>47 && b.at(0)<58){
                if(stoi(b) > 65535){
                    control = false;
                }
                else {
                    int x = memory[stoi(b)]+ memory[stoi(b)+1] * 256;
                    registerMap[a] = xor2(registerMap[a], x,16);
                }
            }
        }
        else if(variables.find(b) != variables.end()){
            if(variables[b].first == "dw"){
                registerMap[a] = xor2(registerMap[a], variableValue(b),16);
            }
            else {
                control = false;
            }
        }
        else if(b == "offset"){
            if(variables.find(c) != variables.end()){
                registerMap[a] = xor2(registerMap[a], variables[c].second.second,16);
            }
            else{
                control = false;
            }
        }
    }
    else if(subRegisterMap.find(a) != subRegisterMap.end()){
        if(subRegisterMap.find(b) != subRegisterMap.end()){
            subRegisterMap[a] = xor2(subRegisterMap[a], subRegisterMap[b],8);
        }
        else if(b.at(0)>47 && b.at(0)<58 && variables.find(b) == variables.end()){// bunu diğer tüm metodlarda ekle
            if(stoi(b) > 255){
                control = false;
            }
            else {
                subRegisterMap[a] = xor2(subRegisterMap[a], stoi(b),8);
            }
        }
        else if(b.at(0) == '[' && b.at(b.length()-1) == ']'){
            b = b.substr(1,b.length()-2);
            if(b == "bx" || b == "di" || b == "si" || b == "bp"){
                subRegisterMap[a] = xor2(subRegisterMap[a], memory[registerMap[b]],8);
            }
            else if(b.at(0)>47 && b.at(0)<58){
                subRegisterMap[a] = xor2(subRegisterMap[a], memory[stoi(b)],8);
            }
        }
        else if(b.at(0) == 'b' && b.at(1) == '[' && b.at(b.length()-1) == ']'){
            b = b.substr(2,b.length()-2);
            if(b == "bx" || b == "di" || b == "si" || b == "bp"){
                subRegisterMap[a] = xor2(subRegisterMap[a], memory[registerMap[b]],8);
            }
            else if(b.at(0)>47 && b.at(0)<58){
                subRegisterMap[a] = xor2(subRegisterMap[a], memory[stoi(b)],8);
            }
        }
        else if(variables.find(b) != variables.end()){
            if(variables[b].first == "db"){
                subRegisterMap[a] = xor2(subRegisterMap[a], variableValue(b),8);
            }
            else {
                control = false;
            }
        }
        else if(b == "offset"){
            if(variables.find(c) != variables.end()){
                if(variables[c].second.second < 256){
                    subRegisterMap[a] = xor2(subRegisterMap[a], variables[c].second.second,8);
                }
                else{
                    control = false;
                }
            }
            else{
                control = false;
            }
        }
    }
    else if(a.at(0) == '[' && a.at(a.length()-1) == ']'){
        if(a == "[bx]" || a == "[di]" || a == "[si]" || a == "[bp]"){
            if(registerMap.find(b) != registerMap.end()){
                int mem  = memory[registerMap[a.substr(1, 2)]] + memory[registerMap[a.substr(1,2)] + 1] * 256;
                int val =  xor2(mem, registerMap[b],16);
                memory[registerMap[a.substr(1, 2)]] = (val) % 256;
                memory[registerMap[a.substr(1, 2)] + 1] = (val) / 256;
            }
            else if(b.at(0)>47 && b.at(0)<58 && variables.find(b) == variables.end()){
                if(stoi(b) < 256){
                    memory[registerMap[a.substr(1, 2)]] = xor2(memory[registerMap[a.substr(1, 2)]], stoi(b), 8);
                }
                else if(stoi(b) < 65536){
                    int mem  = memory[registerMap[a.substr(1, 2)]] + memory[registerMap[a.substr(1,2)] + 1] * 256;
                    int val =  xor2(mem, stoi(b),16);
                    memory[registerMap[a.substr(1, 2)]] = (val) % 256;
                    memory[registerMap[a.substr(1, 2)] + 1] = (val) / 256;
                }
                else {
                    control = false;
                }
            }
            else if(b.at(0) == '[' && b.at(b.length()-1) == ']'){
                control = false;
            }
            else if(b == "offset"){
                if(variables.find(c) != variables.end()){
                    if(variables[c].second.second < 256){
                        memory[registerMap[a.substr(1, 2)]] = xor2(memory[registerMap[a.substr(1, 2)]],variables[c].second.second,8);
                    }
                    else if(variables[c].second.second < 65536){
                        int mem  = memory[registerMap[a.substr(1, 2)]] + memory[registerMap[a.substr(1,2)] + 1] * 256;
                        int val =  xor2(mem, variables[c].second.second,16);
                        memory[registerMap[a.substr(1, 2)]] = (val) % 256;
                        memory[registerMap[a.substr(1, 2)] + 1] = (val) / 256;
                    }
                    else {
                        control = false;
                    }
                }
                else{
                    control = false;
                }
            }
            else if(subRegisterMap.find(b) != subRegisterMap.end()){
                memory[registerMap[a.substr(1, 2)]] = xor2( memory[registerMap[a.substr(1, 2)]], subRegisterMap[b],8);
            }
            else if(variables.find(b) != variables.end()){
                control = false;
            }
            else{
                control = false;
            }
        }
        else if(a.at(1)>47 && a.at(1)<58){
            if(registerMap.find(b) != registerMap.end()){
                int mem  = memory[stoi(a.substr(1, a.length()-2))] + memory[stoi(a.substr(1, a.length()-2)) + 1] * 256;
                int val =  xor2(mem, registerMap[b],16);
                memory[stoi(a.substr(1, a.length()-2))] = (val) % 256;
                memory[stoi(a.substr(1, a.length()-2)) + 1] = (val) / 256;
            }
            else if(b.at(0)>47 && b.at(0)<58 && variables.find(b) == variables.end()){
                if(stoi(b) < 256){
                    memory[stoi(a.substr(1, a.length()-2))] = xor2(memory[stoi(a.substr(1, a.length()-2))], stoi(b),8);
                }
                else if(stoi(b) < 65536){
                    int mem  = memory[stoi(a.substr(1, a.length()-2))] + memory[stoi(a.substr(1, a.length()-2)) + 1] * 256;
                    int val =  xor2(mem, stoi(b),16);
                    memory[stoi(a.substr(1, a.length()-2))] = (val) % 256;
                    memory[stoi(a.substr(1, a.length()-2)) + 1] = (val) / 256;
                }
            }
            else if(b.at(0) == '[' && b.at(b.length()-1) == ']'){
                control = false;
            }
            else if(b == "offset"){
                if(variables.find(c) != variables.end()){
                    if(variables[c].second.second < 256){
                        memory[stoi(a.substr(1, a.length()-2))] = xor2(memory[stoi(a.substr(1, a.length()-2))],variables[c].second.second,8);
                    }
                    else if(variables[c].second.second < 65536){
                        int mem  = memory[stoi(a.substr(1, a.length()-2))] + memory[stoi(a.substr(1, a.length()-2)) + 1] * 256;
                        int val =  xor2(mem, variables[c].second.second,16);
                        memory[stoi(a.substr(1, a.length()-2))] = (val) % 256;
                        memory[stoi(a.substr(1, a.length()-2)) + 1] = (val) / 256;
                    }
                    else {
                        control = false;
                    }
                }
                else{
                    control = false;
                }
            }
            else if(subRegisterMap.find(b) != subRegisterMap.end()){
                memory[stoi(a.substr(1, a.length()-2))] = xor2(memory[stoi(a.substr(1, a.length()-2))],subRegisterMap[b],8);
            }
            else if(variables.find(b) != variables.end()){
                control = false;
            }
            else{
                control = false;
            }
        }
    }
    else if(a.at(0) == 'b' && a.at(1) == '[' && a.at(a.length()-1) == ']'){
        if(a == "b[bx]" || a == "b[di]" || a == "b[si]" || a == "b[bp]"){
            if (subRegisterMap.find(b) != subRegisterMap.end()) {
                memory[registerMap[a.substr(2, 2)]] = xor2(memory[registerMap[a.substr(2, 2)]],subRegisterMap[b],8);
            }
            else if (b.at(0) < 58 && b.at(0) > 47) {
                if(stoi(b) /256 == 0){
                    memory[registerMap[a.substr(2, 2)]] = xor2(memory[registerMap[a.substr(2, 2)]],stoi(b),8);
                }
                else {
                    control = false;
                }
            }
            else if (b.at(0) == '[' && b.at(b.length() - 1) == ']') {
                control = false;
            }
            else if (b == "offset") {// tekrar bak
                if(variables.find(c)!=variables.end()){
                    if(variables[c].second.second <256){
                        memory[registerMap[a.substr(2, 2)]] = xor2(memory[registerMap[a.substr(2, 2)]],variables[c].second.second,8);
                    }
                    else {
                        control = false;
                    }
                }
                else{
                    control = false;
                }
            }
            else if(variables.find(b)!=variables.end()){
                control = false;
            }
            else{
                control =false;
            }
        }
        else if(a.at(2)<58&&a.at(2)>47){
            a=a.substr(2,a.length()-3);
            if (subRegisterMap.find(b) != subRegisterMap.end()) {
                memory[stoi(a)] = xor2(memory[stoi(a)],subRegisterMap[b],8);
            }
            else if (b.at(0) < 58 && b.at(0) > 47) {
                if(stoi(b) /256 == 0){
                    memory[stoi(a)] = xor2(memory[stoi(a)],stoi(b),8);
                }
                else {
                    control = false;
                }
            }
            else if (b.at(0) == '[' && b.at(b.length() - 1) == ']') {
                control = false;
            }
            else if (b == "offset") {// tekrar bak
                if(variables.find(c)!=variables.end()){
                    if(variables[c].second.second <256){
                        memory[stoi(a)] = xor2(memory[stoi(a)],variables[c].second.second,8);
                    }
                    else {
                        control = false;
                    }
                }
                else{
                    control = false;
                }
            }
            else if(variables.find(b)!=variables.end()){
                control = false;
            }
            else{
                control =false;
            }
        }
    }
    else if(a.at(0) == 'w' && a.at(1) == '[' && a.at(a.length()-1) == ']'){
        if( a.substr(1) == "[bx]" || a.substr(1) == "[di]" ||a.substr(1) == "[si]" ||a.substr(1) == "[bp]"){
            if (registerMap.find(b) != registerMap.end()) {
                int mem  = memory[registerMap[a.substr(2, 2)]] + memory[registerMap[a.substr(2,2)] + 1] * 256;
                int val =  xor2(mem, registerMap[a],16);
                memory[registerMap[a.substr(2, 2)]] = val % 256;
                memory[registerMap[a.substr(2, 2)] + 1] = val / 256;
            }
            else if (b.at(0) < 58 && b.at(0) > 47 && variables.find(b) == variables.end()) {
                int mem  = memory[registerMap[a.substr(2, 2)]] + memory[registerMap[a.substr(2,2)] + 1] * 256;
                int val =  xor2(mem, stoi(b),16);
                memory[registerMap[a.substr(2, 2)]] = val % 256;
                memory[registerMap[a.substr(2, 2)] + 1] = val / 256;
            }
            else if (b.at(0) == '[' && b.at(b.length() - 1) == ']') {
                control = false;
            }
            else if (b == "offset") {
                if(variables.find(c)!=variables.end()){
                    int mem  = memory[registerMap[a.substr(2, 2)]] + memory[registerMap[a.substr(2,2)] + 1] * 256;
                    int val =  xor2(mem, variables[c].second.second,16);
                    memory[registerMap[a.substr(2, 2)]] = val % 256;
                    memory[registerMap[a.substr(2, 2)] + 1] = val / 256;
                }
                else{
                    control = false;
                }
            }
            else{
                control =false;
            }
        }
        else if(a.at(2)<58&&a.at(2)>47){
            a=a.substr(2,a.length()-3);
            if (registerMap.find(b) != registerMap.end()) {
                int mem  = memory[stoi(a)] + memory[stoi(a) + 1] * 256;
                int val =  xor2(mem, registerMap[b],16);
                memory[stoi(a)] = val % 256;
                memory[stoi(a) + 1] = val / 256;
            }
            else if (b.at(0) < 58 && b.at(0) > 47 && variables.find(b) == variables.end()) {
                int mem  = memory[stoi(a)] + memory[stoi(a) + 1] * 256;
                int val =  xor2(mem, stoi(b),16);
                memory[stoi(a)] = val % 256;
                memory[stoi(a) + 1] = val / 256;
            }
            else if (b.at(0) == '[' && b.at(b.length() - 1) == ']') {
                control = false;
            }
            else if (b == "offset") {
                if(variables.find(c)!=variables.end()){
                    int mem  = memory[stoi(a)] + memory[stoi(a) + 1] * 256;
                    int val =  xor2(mem, variables[c].second.second,16);
                    memory[stoi(a)] = val % 256;
                    memory[stoi(a) + 1] = val / 256;
                }
                else{
                    control = false;
                }
            }
            else{
                control =false;
            }
        }
    }
    else if(variables.find(a) != variables.end()){
        if(variables[a].first == "db"){
            if(subRegisterMap.find(b) != subRegisterMap.end()){
                memory[variables[a].second.second] = xor2(memory[variables[a].second.second],subRegisterMap[b],8);
            }
            else if(b == "offset"){
                if(variables.find(c) != variables.end()){
                    if(variables[c].second.second < 256){
                        memory[variables[a].second.second] = xor2(memory[variables[a].second.second],variables[c].second.second,8);
                    }
                }
            }
            else if(b.at(0) < 58 && b.at(0) >47 && variables.find(b) == variables.end()){
                if(variables[c].second.second < 256){
                    memory[variables[a].second.second] = xor2(memory[variables[a].second.second],stoi(b),8);
                }
                else{
                    control = false;
                }
            }
        }
        else if (variables[a].first == "dw"){
            if(registerMap.find(b) != registerMap.end()){
                int val =  xor2(variableValue(a),registerMap[b],16);
                memory[variables[a].second.second] = (val) % 256;
                memory[variables[a].second.second + 1] = (val) / 256;
            }
            else if(b == "offset"){
                if(variables.find(c) != variables.end()){
                    int val = xor2(variableValue(a),variables[c].second.second,16);
                    memory[variables[a].second.second] = (val) % 256;
                    memory[variables[a].second.second + 1] = (val) / 256;
                }
            }
            else if(b.at(0) < 58 && b.at(0) >47 && variables.find(b) == variables.end()){
                int val = xor2(variableValue(a),stoi(b),16);
                memory[variables[a].second.second] = (val) % 256;
                memory[variables[a].second.second + 1] = (val) / 256;
            }
        }
    }
}
//This method helps not1 method. It changes flags and returns values.
int not2(int a, int bit){
    int c = 0;
    for(int i = 0; i < bit; i++){
        if(a % 2 == 0){
            c += pow(2,i);
        }
        a = a / 2;
    }
    if(c == 0){
        zf = 1;
    }
    else{
        zf = 0;
    }
    cf = 0;
    of = 0;
    sf = c / (int)pow(2,bit-1);
    return c;
}
//This method controls all possibilities of not command and make changes.
void not1(string a){
    if(registerMap.find(a) != registerMap.end()){
        registerMap[a] = not2(registerMap[a],16);
    }
    else if(subRegisterMap.find(a) != subRegisterMap.end()){
        subRegisterMap[a] = not2(subRegisterMap[a],8);
    }
    else if(a.at(0) == '[' && a.at(a.length()-1) == ']'){
        a = a.substr(1,a.length()-2);
        if(a == "bx" || a == "di" || a == "si" || a == "bp"){
            memory[registerMap[a]] = not2(memory[registerMap[a]],8);
        }
        else if(subRegisterMap.find(a) != subRegisterMap.end()){
            control = false;
        }
        else if(a.at(0)>47 && a.at(0)<58){
            memory[stoi(a)] = not2(memory[stoi(a)],8);
        }
    }
    else if(a.at(0) == 'w' && a.at(1) == '[' && a.at(a.length()-1) == ']'){
        a = a.substr(2,a.length()-3);
        if(a == "bx" || a == "di" || a == "si" || a == "bp"){
            int mem = memory[registerMap[a]] + memory[registerMap[a]+1] * 256;
            int val =  not2(mem,16);
            memory[registerMap[a]] = (val) % 256;
            memory[registerMap[a]+1] = (val) / 256;
        }
        else if(subRegisterMap.find(a) != subRegisterMap.end()){
            control = false;
        }
        else if(a.at(0)>47 && a.at(0)<58){
            int mem = memory[stoi(a)] + memory[stoi(a) + 1] * 256;
            int val =  not2(mem,16);
            memory[stoi(a)] = (val) % 256;
            memory[stoi(a) + 1] = (val) / 256;
        }
    }
    else if(a.at(0) == 'b' && a.at(1) == '[' && a.at(a.length()-1) == ']'){
        a = a.substr(2,a.length()-3);
        if(a == "bx" || a == "di" || a == "si" || a == "bp"){
            memory[registerMap[a]] = not2(memory[registerMap[a]],8);
        }
        else if(subRegisterMap.find(a) != subRegisterMap.end()){
            control = false;
        }
        else if(a.at(0)>47 && a.at(0)<58){
            memory[stoi(a)] = not2(memory[stoi(a)],8);
        }
    }
    else if(variables.find(a) != variables.end()){
        if(variables[a].first == "db"){
            memory[variables[a].second.second] = not2(memory[variables[a].second.second],8);
        }
        else if (variables[a].first == "dw"){
            int val = not2(variableValue(a),16);
            memory[variables[a].second.second] = (val) % 256;
            memory[variables[a].second.second + 1] = (val) / 256;
        }
    }
}
//This method helps shift1 method. It changes flags and returns values.
int shift2(int a,int count,int bit,int c){
    string binary;
    for(int i=0;i<bit;i++){
        binary=to_string(a%2)+binary;
        a /= 2;
    }
    char sg=binary.at(0);
    for(int i=0;i<count;i++){
        if(c==1){
            cf=stoi(binary.substr(0,1));
            binary=binary.substr(1)+"0";
        }
        if(c==2){
            cf=stoi(binary.substr(binary.length()-1,1));
            binary="0"+binary.substr(0,binary.length()-1);
        }
    }
    a=0;
    for(int i=0;i<bit;i++){
        if(binary.at(bit-i-1) == '1'){
            a+=pow(2,i);
        }
    }
    if(a==0)
        zf=1;
    else
        zf=0;
    sign(a,bit);
    if(count==1) {
        if (binary.at(0) != sg) {
            of = 1;
        } else
            of = 0;
    }
    return a;
}
//This method controls all possibilities of shl and shr commands and make changes.
void shift1(string a, int count, int c){
    if(registerMap.find(a) != registerMap.end()){
        registerMap[a] = shift2(registerMap[a], count, 16, c);
    }
    else if(subRegisterMap.find(a) != subRegisterMap.end()){
        subRegisterMap[a] = shift2(subRegisterMap[a], count, 8, c);
    }
    else if(a.at(0) == '[' && a.at(a.length()-1) == ']'){
        a = a.substr(1,a.length()-2);
        if(a == "bx" || a == "di" || a == "si" || a == "bp"){
            memory[registerMap[a]] = shift2(memory[registerMap[a]], count, 8, c);
        }
        else if(subRegisterMap.find(a) != subRegisterMap.end()){
            control = false;
        }
        else if(a.at(0)>47 && a.at(0)<58){
            memory[stoi(a)] = shift2(memory[stoi(a)],count, 8, c);
        }
    }
    else if(a.at(0) == 'w' && a.at(1) == '[' && a.at(a.length()-1) == ']'){
        a = a.substr(2,a.length()-3);
        if(a == "bx" || a == "di" || a == "si" || a == "bp"){
            int mem = memory[registerMap[a]] + memory[registerMap[a]+1] * 256;
            int val =  shift2(mem,count, 16, c);
            memory[registerMap[a]] = (val) % 256;
            memory[registerMap[a]+1] = (val) / 256;
        }
        else if(subRegisterMap.find(a) != subRegisterMap.end()){
            control = false;
        }
        else if(a.at(0)>47 && a.at(0)<58){
            int mem = memory[stoi(a)] + memory[stoi(a) + 1] * 256;
            int val =  shift2(mem,count, 16, c);
            memory[stoi(a)] = (val) % 256;
            memory[stoi(a) + 1] = (val) / 256;
        }
    }
    else if(a.at(0) == 'b' && a.at(1) == '[' && a.at(a.length()-1) == ']'){
        a = a.substr(2,a.length()-3);
        if(a == "bx" || a == "di" || a == "si" || a == "bp"){
            memory[registerMap[a]] = shift2(memory[registerMap[a]],count, 8, c);
        }
        else if(subRegisterMap.find(a) != subRegisterMap.end()){
            control = false;
        }
        else if(a.at(0)>47 && a.at(0)<58){
            memory[stoi(a)] = shift2(memory[stoi(a)], count,8,c);
        }
    }
    else if(variables.find(a) != variables.end()){
        if(variables[a].first == "db"){
            memory[variables[a].second.second] = shift2(memory[variables[a].second.second],count,8,c);
        }
        else if (variables[a].first == "dw"){
            int val = shift2(variableValue(a),count,16,c);
            memory[variables[a].second.second] = (val) % 256;
            memory[variables[a].second.second + 1] = (val) / 256;
        }
    }
}
//This method helps rot1 method. It changes flags and returns values.
int rot2(int a, int count, int bit, int c){
    string binary;
    for(int i=0;i<bit;i++){
        binary=to_string(a%2)+binary;
        a /= 2;
    }
    char sg=binary.at(0);
    for(int i=0;i<count;i++){
        if(c==1){
            binary = binary + to_string(cf);
            cf = stoi(binary.substr(0,1));
            binary = binary.substr(1);
        }
        if(c==2){
            binary=to_string(cf) + binary;
            cf = stoi(binary.substr(binary.length()-1,1));
            binary = binary.substr(0,binary.length()-1);
        }
    }
    a=0;
    for(int i=0;i<bit;i++){
        if(binary.at(bit-i-1) == '1'){
            a+=pow(2,i);
        }
    }
    if(a==0)
        zf=1;
    else
        zf=0;
    sign(a,bit);
    if(count==1) {
        if (binary.at(0) != sg) {
            of = 1;
        } else
            of = 0;
    }
    return a;
}
//This method controls all possibilities of rcr and rcl commands and make changes.
void rot1(string a, int count, int c){
    if(registerMap.find(a) != registerMap.end()){
        registerMap[a] = rot2(registerMap[a], count, 16, c);
    }
    else if(subRegisterMap.find(a) != subRegisterMap.end()){
        subRegisterMap[a] = rot2(subRegisterMap[a], count, 8, c);
    }
    else if(a.at(0) == '[' && a.at(a.length()-1) == ']'){
        a = a.substr(1,a.length()-2);
        if(a == "bx" || a == "di" || a == "si" || a == "bp"){
            memory[registerMap[a]] = rot2(memory[registerMap[a]], count, 8, c);
        }
        else if(subRegisterMap.find(a) != subRegisterMap.end()){
            control = false;
        }
        else if(a.at(0)>47 && a.at(0)<58){
            memory[stoi(a)] = rot2(memory[stoi(a)],count, 8, c);
        }
    }
    else if(a.at(0) == 'w' && a.at(1) == '[' && a.at(a.length()-1) == ']'){
        a = a.substr(2,a.length()-3);
        if(a == "bx" || a == "di" || a == "si" || a == "bp"){
            int mem = memory[registerMap[a]] + memory[registerMap[a]+1] * 256;
            int val =  rot2(mem,count, 16, c);
            memory[registerMap[a]] = (val) % 256;
            memory[registerMap[a]+1] = (val) / 256;
        }
        else if(subRegisterMap.find(a) != subRegisterMap.end()){
            control = false;
        }
        else if(a.at(0)>47 && a.at(0)<58){
            int mem = memory[stoi(a)] + memory[stoi(a) + 1] * 256;
            int val =  rot2(mem,count, 16, c);
            memory[stoi(a)] = (val) % 256;
            memory[stoi(a) + 1] = (val) / 256;
        }
    }
    else if(a.at(0) == 'b' && a.at(1) == '[' && a.at(a.length()-1) == ']'){
        a = a.substr(2,a.length()-3);
        if(a == "bx" || a == "di" || a == "si" || a == "bp"){
            memory[registerMap[a]] = rot2(memory[registerMap[a]],count, 8, c);
        }
        else if(subRegisterMap.find(a) != subRegisterMap.end()){
            control = false;
        }
        else if(a.at(0)>47 && a.at(0)<58){
            memory[stoi(a)] = rot2(memory[stoi(a)], count,8,c);
        }
    }
    else if(variables.find(a) != variables.end()){
        if(variables[a].first == "db"){
            memory[variables[a].second.second] = rot2(memory[variables[a].second.second],count,8,c);
        }
        else if (variables[a].first == "dw"){
            int val = rot2(variableValue(a),count,16,c);
            memory[variables[a].second.second] = (val) % 256;
            memory[variables[a].second.second + 1] = (val) / 256;
        }
    }
}
//This method controls all possibilities of push command and make changes.
void push(string a){
    if(registerMap.find(a) != registerMap.end()){
        memory[sp] = registerMap[a] % 256;
        memory[sp+1] = registerMap[a] /256;
    }
    else if(subRegisterMap.find(a) != subRegisterMap.end()){
        control = false;
    }
    else if(a.at(0) == '[' && a.at(a.length()-1) == ']'){
        a = a.substr(1,a.length()-2);
        if(a == "bx" || a == "di" || a == "si" || a == "bp"){
            memory[sp] = memory[registerMap[a]];
            memory[sp+1] = memory[registerMap[a] + 1];
        }
        else if(subRegisterMap.find(a) != subRegisterMap.end()){
            control = false;
        }
        else if(a.at(0)>47 && a.at(0)<58){
            memory[sp] = memory[stoi(a)];
            memory[sp+1] = memory[stoi(a) + 1];
        }
    }
    else if(a.at(0) == 'w' && a.at(1) == '[' && a.at(a.length()-1) == ']'){
        a = a.substr(2,a.length()-3);
        if(a == "bx" || a == "di" || a == "si" || a == "bp"){
            memory[sp] = memory[registerMap[a]];
            memory[sp+1] = memory[registerMap[a] + 1];
        }
        else if(subRegisterMap.find(a) != subRegisterMap.end()){
            control = false;
        }
        else if(a.at(0)>47 && a.at(0)<58){
            memory[sp] = memory[stoi(a)];
            memory[sp+1] = memory[stoi(a) + 1];
        }
    }
    else if(a.at(0) == 'b' && a.at(1) == '[' && a.at(a.length()-1) == ']'){
        control = false;
    }
    else if(variables.find(a) != variables.end()){
        if(variables[a].first == "db"){
            control = false;
        }
        else if (variables[a].first == "dw"){
            memory[sp] = memory[variables[a].second.second];
            memory[sp+1] = memory[variables[a].second.second + 1];
        }
    }
    sp -= 2;
}
//This method controls all possibilities of pop command and make changes.
void pop(string a){
    sp += 2;
    if(registerMap.find(a) != registerMap.end()){
        registerMap[a] = memory[sp] + memory[sp+1] * 256;
    }
    else if(subRegisterMap.find(a) != subRegisterMap.end()){
        control = false;
    }
    else if(a.at(0) == '[' && a.at(a.length()-1) == ']'){
        a = a.substr(1,a.length()-2);
        if(a == "bx" || a == "di" || a == "si" || a == "bp"){
            memory[registerMap[a]] = memory[sp];
            memory[registerMap[a] + 1] = memory[sp+1];
        }
        else if(subRegisterMap.find(a) != subRegisterMap.end()){
            control = false;
        }
        else if(a.at(0)>47 && a.at(0)<58){
            memory[stoi(a)] = memory[sp];
            memory[stoi(a) + 1] = memory[sp+1];
        }
    }
    else if(a.at(0) == 'w' && a.at(1) == '[' && a.at(a.length()-1) == ']'){
        a = a.substr(2,a.length()-3);
        if(a == "bx" || a == "di" || a == "si" || a == "bp"){
            memory[registerMap[a]] = memory[sp];
            memory[registerMap[a] + 1] = memory[sp+1];
        }
        else if(subRegisterMap.find(a) != subRegisterMap.end()){
            control = false;
        }
        else if(a.at(0)>47 && a.at(0)<58){
            memory[stoi(a)] = memory[sp];
            memory[stoi(a) + 1] = memory[sp+1];
        }
    }
    else if(a.at(0) == 'b' && a.at(1) == '[' && a.at(a.length()-1) == ']'){
        control = false;
    }
    else if(variables.find(a) != variables.end()){
        if(variables[a].first == "db"){
            control = false;
        }
        else if (variables[a].first == "dw"){
            memory[variables[a].second.second] = memory[sp];
            memory[variables[a].second.second + 1] = memory[sp+1];
        }
    }
}
//This method controls all possibilities of cmp command and make changes.
void cmp(string a, string b, string c){
    if(registerMap.find(a) != registerMap.end()){
        if(registerMap.find(b) != registerMap.end()){
            flag(registerMap[a],registerMap[b],32,2);
        }
        else if(b.at(0)>47 && b.at(0)<58){
            if(stoi(b) > 65535){
                control = false;
            }
            else{
                flag(registerMap[a],stoi(b),32, 2);
            }
        }
        else if(b.at(0) == '[' && b.at(b.length()-1) == ']'){
            b = b.substr(1,b.length()-2);
            if(b == "bx" || b == "di" || b == "si" || b == "bp"){
                int x = memory[registerMap[b]]+ memory[registerMap[b]+1] * 256;
                flag(registerMap[a],x,32, 2);
            }
            else if(b.at(0)>47 && b.at(0)<58){
                int x = memory[stoi(b)]+ memory[stoi(b)+1] * 256;
                flag(registerMap[a],x,32, 2);
            }
        }
        else if(b.at(0) == 'w' && b.at(1) == '[' && b.at(b.length()-1) == ']'){
            b = b.substr(2,b.length()-3);
            if(b == "bx" || b == "di" || b == "si" || b == "bp"){
                int x = memory[registerMap[b]]+ memory[registerMap[b]+1] * 256;
                flag(registerMap[a],x,32, 2);
            }
            else if(b.at(0)>47 && b.at(0)<58){
                if(stoi(b) > 65535){
                    control = false;
                }
                else {
                    int x = memory[stoi(b)]+ memory[stoi(b)+1] * 256;
                    flag(registerMap[a],x,32, 2);
                }
            }
        }
        else if(variables.find(b) != variables.end()){
            if(variables[b].first == "db"){
                flag(registerMap[a],variableValue(b),32, 2);
            }
            else {
                control = false;
            }
        }
        else if(b == "offset"){
            if(variables.find(c) != variables.end()){
                flag(registerMap[a],variables[c].second.second, 32, 2);
            }
            else{
                control = false;
            }
        }
    }
    else if(subRegisterMap.find(a) != subRegisterMap.end()){
        if(subRegisterMap.find(b) != subRegisterMap.end()){
            flag(subRegisterMap[a],subRegisterMap[b],16, 2);
        }
        else if(b.at(0)>47 && b.at(0)<58){
            if(stoi(b) > 255){
                control = false;
            }
            else {
                flag(subRegisterMap[a],stoi(b),16, 2);//bb 32 mi olacak kontrol et
            }
        }
        else if(b.at(0) == '[' && b.at(b.length()-1) == ']'){
            b = b.substr(1,b.length()-2);
            if(b == "bx" || b == "di" || b == "si" || b == "bp"){
                flag(subRegisterMap[a],memory[registerMap[b]],16, 2);
            }
            else if(b.at(0)>47 && b.at(0)<58){
                flag(subRegisterMap[a],memory[stoi(b)],16, 2);
            }
        }
        else if(b.at(0) == 'b' && b.at(1) == '[' && b.at(b.length()-1) == ']'){
            b = b.substr(2,b.length()-3);
            if(b == "bx" || b == "di" || b == "si" || b == "bp"){
                flag(subRegisterMap[a],memory[registerMap[b]],16, 2);
            }
            else if(b.at(0)>47 && b.at(0)<58){
                flag(subRegisterMap[a],memory[stoi(b)],16, 2);
            }
        }
        else if(variables.find(b) != variables.end()){
            if(variables[b].first == "db"){
                flag(subRegisterMap[a],variableValue(b),16, 2);
            }
            else {
                control = false;
            }
        }
        else if(b == "offset"){
            if(variables.find(c) != variables.end()){
                flag(subRegisterMap[a],variables[c].second.second, 16, 2);
            }
            else{
                control = false;
            }
        }
    }
    else if(a.at(0) == '[' && a.at(a.length()-1) == ']'){
        if(a == "[bx]" || a == "[di]" || a == "[si]" || a == "[bp]"){
            if(registerMap.find(b) != registerMap.end()){
                int mem  = memory[registerMap[a.substr(1, 2)]] + memory[registerMap[a.substr(1,2)] + 1] * 256;
                flag(mem, registerMap[b], 32, 2);
            }
            else if(b.at(0)>47 && b.at(0)<58){
                if(stoi(b) < 256){
                    flag(memory[registerMap[a.substr(1, 2)]], stoi(b),16,2);
                }
                else if(stoi(b) < 65536){
                    int mem  = memory[registerMap[a.substr(1, 2)]] + memory[registerMap[a.substr(1,2)] + 1] * 256;
                    flag(mem, stoi(b), 32, 2);
                }
                else {
                    control = false;
                }
            }
            else if(b.at(0) == '[' && b.at(b.length()-1) == ']'){
                control = false;
            }
            else if(b == "offset"){
                if(variables.find(c) != variables.end()){
                    if(variables[c].second.second < 256){
                        flag(memory[registerMap[a]], variables[c].second.second,16,2);
                    }
                    else if(variables[c].second.second < 65536){
                        int mem  = memory[registerMap[a.substr(1, 2)]] + memory[registerMap[a.substr(1,2)] + 1] * 256;
                        flag(mem, variables[c].second.second, 32, 2);
                    }
                    else {
                        control = false;
                    }
                }
                else{
                    control = false;
                }
            }
            else if(subRegisterMap.find(b) != subRegisterMap.end()){
                flag(memory[registerMap[a.substr(1, 2)]],subRegisterMap[b], 16,2);
            }
            else if(variables.find(b) != variables.end()){
                control = false;
            }
            else{
                control = false;
            }
        }
        else if(a.at(1)>47 && a.at(1)<58){
            if(registerMap.find(b) != registerMap.end()){
                int mem  = memory[stoi(a.substr(1, a.length()-2))] + memory[stoi(a.substr(1, a.length()-2)) + 1] * 256;
                flag(mem, registerMap[b], 32, 2);
            }
            else if(b.at(0)>47 && b.at(0)<58){
                if(stoi(b) < 256){
                    flag(memory[stoi(a.substr(1,a.length()-2))], stoi(b),16,2);
                }
                else if(stoi(b) < 65536){
                    int mem  = memory[stoi(a.substr(1, a.length()-2))] + memory[stoi(a.substr(1, a.length()-2)) + 1] * 256;
                    flag(mem, stoi(b), 32, 2);
                }
            }
            else if(b.at(0) == '[' && b.at(b.length()-1) == ']'){
                control = false;
            }
            else if(b == "offset"){
                if(variables.find(c) != variables.end()){
                    if(variables[c].second.second < 256){
                        flag(memory[stoi(a.substr(1,a.length()-2))], variables[c].second.second,16,2);
                    }
                    else if(variables[c].second.second < 65536){
                        int mem  = memory[stoi(a.substr(1, a.length()-2))] + memory[stoi(a.substr(1, a.length()-2)) + 1] * 256;
                        flag(mem, variables[c].second.second, 32, 2);
                    }
                    else {
                        control = false;
                    }
                }
                else{
                    control = false;
                }
            }
            else if(subRegisterMap.find(b) != subRegisterMap.end()){
                flag(memory[stoi(a.substr(1, a.length()-2))],subRegisterMap[b], 16,2);
            }
            else if(variables.find(b) != variables.end()){
                control = false;
            }
            else{
                control = false;
            }
        }
    }
    else if(a.at(0) == 'b' && a.at(1) == '[' && a.at(a.length()-1) == ']'){
        if(a == "b[bx]" || a == "b[di]" || a == "b[si]" || a == "b[bp]"){
            if (subRegisterMap.find(b) != subRegisterMap.end()) {
                flag(memory[registerMap[a.substr(2, 2)]],subRegisterMap[b], 16,2);
            }
            else if (b.at(0) < 58 && b.at(0) > 47) {
                if(stoi(b) /256 == 0){
                    flag(memory[registerMap[a.substr(2, 2)]],stoi(b), 16,2);
                }
                else {
                    control = false;
                }
            }
            else if (b.at(0) == '[' && b.at(b.length() - 1) == ']') {
                control = false;
            }
            else if (b == "offset") {// tekrar bak
                if(variables.find(c)!=variables.end()){
                    if(variables[c].second.second <256){
                        flag(memory[registerMap[a.substr(2, 2)]], variables[c].second.second,16,2);
                    }
                    else {
                        control = false;
                    }
                }
                else{
                    control = false;
                }
            }
            else if(variables.find(b)!=variables.end()){
                control = false;
            }
            else{
                control =false;
            }
        }
        else if(a.at(2) < 58 && a.at(2) > 47){
            a = a.substr(2,a.length()-3);
            if (subRegisterMap.find(b) != subRegisterMap.end()) {
                flag(memory[stoi(a)],subRegisterMap[b], 16,2);
            }
            else if (b.at(0) < 58 && b.at(0) > 47) {
                if(stoi(b) /256 == 0){
                    flag(memory[stoi(a)],stoi(b), 16,2);
                }
                else {
                    control = false;
                }
            }
            else if (b.at(0) == '[' && b.at(b.length() - 1) == ']') {
                control = false;
            }
            else if (b == "offset") {// tekrar bak
                if(variables.find(c)!=variables.end()){
                    if(variables[c].second.second <256){
                        flag(memory[stoi(a)], variables[c].second.second,16,2);
                    }
                    else {
                        control = false;
                    }
                }
                else{
                    control = false;
                }
            }
            else if(variables.find(b)!=variables.end()){
                control = false;
            }
            else{
                control =false;
            }
        }
    }
    else if(a.at(0) == 'w' && a.at(1) == '[' && a.at(a.length()-1) == ']'){
        if( a.substr(1) == "[bx]" || a.substr(1) == "[di]" ||a.substr(1) == "[si]" ||a.substr(1) == "[bp]"){
            if (registerMap.find(b) != registerMap.end()) {
                int mem  = memory[registerMap[a.substr(2, 2)]] + memory[registerMap[a.substr(2,2)] + 1] * 256;
                flag(mem, registerMap[b], 32, 2);
            }
            else if (b.at(0) < 58 && b.at(0) > 47) {
                int mem  = memory[registerMap[a.substr(2, 2)]] + memory[registerMap[a.substr(2,2)] + 1] * 256;
                flag(mem, stoi(b), 32, 2);
            }
            else if (b.at(0) == '[' && b.at(b.length() - 1) == ']') {
                control = false;
            }
            else if (b == "offset") {
                if(variables.find(c)!=variables.end()){
                    int mem  = memory[registerMap[a.substr(2, 2)]] + memory[registerMap[a.substr(2,2)] + 1] * 256;
                    flag(mem, variables[c].second.second, 32, 2);
                }
                else{
                    control = false;
                }
            }
            else{
                control =false;
            }
        }
        else if(a.at(2) < 58 && a.at(2) > 47){
            a = a.substr(2,a.length()-3);
            if (registerMap.find(b) != registerMap.end()) {
                int mem  = memory[stoi(a)] + memory[stoi(a) + 1] * 256;
                flag(mem, registerMap[b], 32, 2);
            }
            else if (b.at(0) < 58 && b.at(0) > 47) {
                int mem  = memory[stoi(a)] + memory[stoi(a) + 1] * 256;
                flag(mem, stoi(b), 32, 2);
            }
            else if (b.at(0) == '[' && b.at(b.length() - 1) == ']') {
                control = false;
            }
            else if (b == "offset") {
                if(variables.find(c)!=variables.end()){
                    int mem  = memory[stoi(a)] + memory[stoi(a) + 1] * 256;
                    flag(mem, variables[c].second.second, 32, 2);
                }
                else{
                    control = false;
                }
            }
            else{
                control =false;
            }
        }
    }
    else if(variables.find(a) != variables.end()){
        if(variables[a].first == "db"){
            if(subRegisterMap.find(b) != subRegisterMap.end()){
                flag(memory[variables[a].second.second], subRegisterMap[b], 16, 2);
            }
            else if(b == "offset"){
                if(variables.find(c) != variables.end()){
                    if(variables[c].second.second < 256){
                        flag(memory[variables[a].second.second], variables[c].second.second, 16, 2);
                    }
                }
            }
            else if(b.at(0) > 47 && b.at(0) < 58 && variables.find(b) == variables.end()){
                if(stoi(b)< 256){
                    flag(memory[variables[a].second.second], stoi(b), 16, 2);
                }
                else {
                    control = false;
                }
            }
        }
        else if (variables[a].first == "dw"){
            if(registerMap.find(b) != registerMap.end()){
                int mem = variableValue(a) - registerMap[b];
                flag(variableValue(a), registerMap[b], 32, 2);
            }
            else if(b == "offset"){
                if(variables.find(c) != variables.end()){
                    int mem = variableValue(a) - variables[c].second.second;
                    flag(variableValue(a),variables[c].second.second, 32, 2);
                }
            }
            else if(b.at(0) > 47 && b.at(0) < 58 && variables.find(b) == variables.end()){
                int mem = variableValue(a) - stoi(b);
                flag(variableValue(a),stoi(b), 32, 2);
            }
        }
    }
}