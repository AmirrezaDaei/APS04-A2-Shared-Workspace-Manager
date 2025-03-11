#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include <algorithm>

using namespace std;

const string WINDOW = "window";
const string DOOR = "door";
const string MIDDLE = "middle";
const string SHOW_TABLE_INFO = "show_table_info";
const string ENTER = "enter";
const string RESERVE_TABLE = "reserve_table";
const string EXIT = "exit";
const string SWITCH = "switch";
const string TABLE_ID = "table ID: ";
const string PEOPLE_MESSAGE = "People at the table: ";
const string TABLE_CAPACITY_MESSAGE = "Table remaining capacity: ";
const string QUEUE_MESSAGE = "Waiting queue length: ";
const string SIT_MESSAGE = " sits at table ";
const string ENTER_QUEUE_MESSAGE = " enters the waiting queue of table ";

struct table {
    int ID;
    int x;
    int y;
    int capacity;
    int bonus;
    int score = 0;
    vector<string> students;
    vector<string> queue;
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

void readTables(vector<string> &tablesData, vector<table> &tables) {
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

void readStudents(vector<string> &studentsData, vector<student> &students) {
    for(int i = 0; i < studentsData.size() / 4; i++) {
        struct student newStudent;
        newStudent.ID = stoi(studentsData[i * 4]);
        newStudent.name = studentsData[i * 4 + 1];
        newStudent.friendID = stoi(studentsData[i * 4 + 2]);
        newStudent.enemyID = stoi(studentsData[i * 4 + 3]);
        students.push_back(newStudent);
    }
}

vector<table>::iterator findTable(vector<table> &tables, int tableID) {
    auto table = find_if(tables.begin(), tables.end(), [tableID](struct table table) {return table.ID == tableID;});
    return table;
}

void showTableInfo(vector<table>::iterator tableIt, int tableID) {
    sort(tableIt->students.begin(), tableIt->students.end());
    cout << TABLE_ID << tableIt->ID << endl;
    cout << PEOPLE_MESSAGE;
    if(!tableIt->students.empty()) {
        cout << *tableIt->students.begin();
        for_each(tableIt->students.begin() + 1, tableIt->students.end(), [](string student) {cout << ", " << student;});
    }
    cout << endl << TABLE_CAPACITY_MESSAGE << tableIt->capacity << endl;
    cout << QUEUE_MESSAGE << tableIt->queue.size() << endl;
}

vector<student>::iterator findStudent(vector<student> &students, int studentID) {
    auto table = find_if(students.begin(), students.end(), [studentID](struct student student) {return student.ID == studentID;});
    return table;
}

void reserveTable(vector<table>::iterator &tableIt, vector<student>::iterator &studentIt) {
    if(tableIt->capacity > 0) {
        tableIt->students.push_back(studentIt->name);
        tableIt->capacity--;
        studentIt->table = *tableIt;
        cout << studentIt->name << SIT_MESSAGE << tableIt->ID << endl;
    }
    else {
        tableIt->queue.push_back(studentIt->name);
        cout << studentIt->name << ENTER_QUEUE_MESSAGE << tableIt->ID << endl;
    }
}

void exitStudent(vector<table>::iterator &tableIt, vector<student>::iterator &studentIt) {
    studentIt->table = table();
}

void getCommands(vector<table> &tables, vector<student> &students) {
    string command, line;
    while(getline(cin, line)) {
        stringstream ss(line);
        ss >> command;
        if(command == SHOW_TABLE_INFO) {
            int tableID;
            ss >> tableID;
            auto table = findTable(tables, tableID);
            showTableInfo(table, tableID);
        }
        else if(command == ENTER) {
            int studentID;
            ss >> studentID;
            // enter();
        }
        else if(command == RESERVE_TABLE) {
            int studentID, tableID;
            ss >> studentID;
            ss >> tableID;
            auto student = findStudent(students, studentID);
            if(ss.fail()) {
                //auto table = findBestTable();
                // reserveTable(table, student);
            }
            else {
                auto table = findTable(tables, tableID);
                reserveTable(table, student);
            }
        }
        else if(command == EXIT) {
            int studentID;
            ss >> studentID;
            auto studentIt = findStudent(students, studentID);
            auto tableIt = findTable(tables, studentIt->table.ID);
            exitStudent(tableIt, studentIt);
        }
        else if(command == SWITCH) {
            int studentID;
            ss >> studentID;
            // switch();
        }
    }
}

int main(int argc,char* argv[]) {
    vector<table> tables;
    vector<student> students;
    vector<string> tablesData = readFile(argv[1]);
    vector<string> studentsData = readFile(argv[2]);
    readTables(tablesData, tables);
    readStudents(studentsData, students);
    getCommands(tables, students);
}