/*
 * Subor: tail2.cc
 * Projekt: IJC DU2, priklad a)
 * Datum: 24.4.2018
 * Autor: Peter Hornak - xhorna14
 * Poznamky: Zo suboru vypise poslednych -n riadkov
 */


#include <iostream>
#include <string>
#include <queue>
#include <vector>
#include <cstring>
#include <fstream>

using namespace std;

char *parse_args(int argc, char* argv[],int* n){
    vector<string> args(argv, argv+argc);
    if (argc < 1 || argc > 4){
        cerr << "Missing arguments" << endl;
        exit(1);
    }
    if(argc == 2){
        if ((args.at(1) == "-n")){
            cerr << "Missing arguments" << endl;
            exit(1);
        }
        else
            return argv[1];
    }
    if (argc == 3){
        if(args.at(1) == "-n"){
            char *temp;
            *n = strtol(args.at(2).c_str(), &temp, 10);
            if(*n >= 0 && temp[0] == '\0')
                return NULL;
            else{
                cerr << "Wrong arguments" << endl;
                exit(1);
            }
        }
        else{
            cerr << "Wrong arguments" << endl;
            exit(1);
        }
    }
    if(argc == 4){
        if(args.at(1) == "-n"){
            char *temp;
            *n = strtol(args.at(2).c_str(), &temp, 10);
            if(*n >= 0 && temp[0] == '\0')
                return argv[3];
            else{
                cerr << "Wrong arguments" << endl;
                exit(1);
            }
        }
        else{
            cerr << "Wrong arguments" << endl;
            exit(1);
        }
    }
    return NULL; //if argc == 1
}


int main(int argc, char* argv[]){
    ios::sync_with_stdio(true);
    int n = 10;
    char *filename = parse_args(argc, argv, &n);

    if(n < 1)
        return 0;

    queue<string> buff;
    string line;
    
    /*Nacitanie z stdin*/
    if(filename == NULL)
        while(std::getline(cin, line)){
            if(buff.size() == (unsigned) n)
                buff.pop();
            buff.push(line);
        }
    /*Nacitanie zo suboru */
    else{
        fstream file;
        file.open(filename, fstream::in);
        if(file.fail()){
            cerr << "File could not be opened" << endl;
            return 1;
        }
        while(std::getline(file, line)){
            if(buff.size() == (unsigned) n)
                buff.pop();
            buff.push(line);
        }
        file.close();
    }

    /* Vypis */
    while(!buff.empty()){
        cout << buff.front() << endl;
        buff.pop();
    }
    return 0;
}
