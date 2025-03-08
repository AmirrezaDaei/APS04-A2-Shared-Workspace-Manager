#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>

using namespace std;

const string WINDOW = "window";
const string DOOR = "door";
const string MIDDLE = "middle";

struct table {
    int ID;
    int x;
    int y;
    int capacity;
    int bonus;
    int score = 0;
    vector<struct student> students;
    vector<struct student> queue;
};

struct student {
    int ID;
    string name;
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
    file.close();
    return datas;
}

void readTables(vector<string> tablesData, vector<table> &tables) {
    for(int i = 0; i < tablesData.size() / 5; i++) {
        struct table newTable;
        newTable.ID = stoi(tablesData[i * 5]);
        newTable.x = stoi(tablesData[i * 5 + 1]);
        newTable.y = stoi(tablesData[i * 5 + 2]);
        newTable.capacity = stoi(tablesData[i * 5 + 3]);
        if(tablesData[i * 5 + 4] == WINDOW)
            newTable.bonus = 6;
        else if(tablesData[i * 5 + 4] == DOOR)
            newTable.bonus = 4;
        else if(tablesData[i * 5 + 4] == MIDDLE)
            newTable.bonus = 2;
        tables.push_back(newTable);
    }
}

void readStudents(vector<string> studentsData, vector<student> &students) {
    for(int i = 0; i < studentsData.size() / 4; i++) {
        struct student newStudent;
        newStudent.ID = stoi(studentsData[i * 4]);
        newStudent.name = studentsData[i * 4 + 1];
        newStudent.friendID = stoi(studentsData[i * 4 + 2]);
        newStudent.enemyID = stoi(studentsData[i * 4 + 3]);
        students.push_back(newStudent);
    }
}

int main(int argc,char* argv[]) {
    vector<table> tables;
    vector<student> students;
    vector<string> tablesData = readFile(argv[1]);
    vector<string> studentsData = readFile(argv[2]);
    readTables(tablesData, tables);
    readStudents(studentsData, students);
    for(struct student student : students) {
        cout << student.ID << " " << student.name << " " << student.friendID << " " << student.enemyID << endl;
    }
}
