#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>

using namespace std;

struct table {
    int ID;
    int capacity;
    int bonus;
    int score = 0;
    int x;
    int y;
    vector<struct student> students;
    vector<struct student> queue;
};

struct student {
    string name;
    int ID;
    int friendID;
    int enemyID;
    struct table table;
};

vector<string> readFile(string fileAddress) {
    ifstream file(fileAddress);
    vector<string> datas;
    string line;
    getline(file, line);
    while(getline(file, line)) {
        stringstream row(line);
        string value;
        while(getline(row, value, ',')) {
            datas.push_back(value);
        }
    }
    return datas;
}

void readTables(vector<string> tables) {
    
}

int main(int argc,char* argv[]) {
    vector<table> tables;
    vector<student> students;
    vector<string> tablesData = readFile(argv[1]);
    vector<string> studentsData = readFile(argv[2]);
}
