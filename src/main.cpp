#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include <algorithm>

using namespace std;

const char COMMA = ',';
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
const string EXIT_MESSAGE = " exits!";
const string TABLE_MESSAGE = "Table ";
const string COLON = ": ";
const string SPACE = " ";
const string SWITCH_MESSAGE = " switches seats with ";
const string EXCLAMATION_MARK = "!";

struct table {
    int ID = 0;
    int x;
    int y;
    int capacity;
    int bonus;
    int score = 0;
    vector<string> students;
    vector<int> queue;
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
        while(getline(row, value, COMMA)) {
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

vector<student>::iterator findStudent(vector<student> &students, int studentID) {
    auto table = find_if(students.begin(), students.end(), [studentID](struct student student) {return student.ID == studentID;});
    return table;
}

void showTableInfo(vector<table>::iterator tableIt, int tableID) {
    sort(tableIt->students.begin(), tableIt->students.end());
    cout << TABLE_ID << tableIt->ID << endl;
    cout << PEOPLE_MESSAGE;
    if(!tableIt->students.empty()) {
        cout << *tableIt->students.begin();
        for_each(tableIt->students.begin() + 1, tableIt->students.end(), [](string student) {cout << COMMA << SPACE << student;});
    }
    cout << endl << TABLE_CAPACITY_MESSAGE << tableIt->capacity << endl;
    cout << QUEUE_MESSAGE << tableIt->queue.size() << endl;
}

void printTables(vector<table> &tables) {
    for(auto table = tables.begin(); table != tables.end(); table++) {
        cout << TABLE_MESSAGE << table->ID << COLON << table->capacity << SPACE << table->queue.size() << SPACE << table->score << endl;
    }
}

void calculateTablesScore(vector<table> &tables, vector<student> &students, vector<student>::iterator &student) {
    auto friendIt = findStudent(students, student->friendID);
    auto enemyIt = findStudent(students, student->enemyID);
    int friendDistance, enemyDistance;
    for(auto table = tables.begin(); table != tables.end(); table++) {
        friendDistance = 0;
        enemyDistance = 0;
        if(friendIt->table.ID != 0)
            friendDistance = abs(table->x - friendIt->table.x) + abs(table->y - friendIt->table.y);
        if(enemyIt->table.ID != 0)
            enemyDistance = abs(table->x - enemyIt->table.x) + abs(table->y - enemyIt->table.y);
        table->score = enemyDistance - friendDistance + table->bonus;
    }
}

vector<table>::iterator findBestTable(vector<table> &tables) {
    auto bestTable = tables.begin();
    for(vector<table>::iterator table = tables.begin(); table != tables.end(); table++) {
        if(table->score > bestTable->score)
        bestTable = table;
        else if(table->score == bestTable->score) {
            if(table->ID < bestTable->ID)
                bestTable = table;
        }
    }
    return bestTable;
}

void reserveTable(vector<table>::iterator &tableIt, vector<student>::iterator &studentIt) {
    if(tableIt->capacity > 0) {
        tableIt->students.push_back(studentIt->name);
        tableIt->capacity--;
        studentIt->table = *tableIt;
        cout << studentIt->name << SIT_MESSAGE << tableIt->ID << endl;
    }
    else {
        tableIt->queue.push_back(studentIt->ID);
        cout << studentIt->name << ENTER_QUEUE_MESSAGE << tableIt->ID << endl;
    }
}

void replaceQueue(vector<student> &students, vector<table>::iterator &tableIt, vector<student>::iterator &friendIt) {
    for(int student : tableIt->queue) {
        if(friendIt->ID == student) {
            tableIt->students.push_back(friendIt->name);
            tableIt->queue.erase(find(tableIt->queue.begin(), tableIt->queue.end(), friendIt->ID));
            friendIt->table = *tableIt;
            return;
        }
    }
    auto newStudent = findStudent(students, *(tableIt->queue.begin()));
    tableIt->students.push_back(newStudent->name);
    tableIt->queue.erase(tableIt->queue.begin());
    newStudent->table = *tableIt;
    return;
}

void exitStudent(vector<student> &students, vector<table>::iterator &tableIt, vector<student>::iterator &studentIt, vector<student>::iterator &friendIt) {
    studentIt->table = table();
    tableIt->students.erase(find(tableIt->students.begin(), tableIt->students.end(), studentIt->name));
    cout << studentIt->name << EXIT_MESSAGE << endl;
    if(tableIt->queue.size() > 0) {
        replaceQueue(students, tableIt, friendIt);
        return;
    }
    tableIt->capacity++;
}

void switchStudents(vector<table> &tables, vector<student> &students, vector<student>::iterator &studentIt, vector<student>::iterator &friendIt) {
    auto studentTableIt = findTable(tables, studentIt->table.ID);
    auto friendTableIt = findTable(tables, friendIt->table.ID);
    studentTableIt->students.erase(find(studentTableIt->students.begin(), studentTableIt->students.end(), studentIt->name));
    friendTableIt->students.erase(find(friendTableIt->students.begin(), friendTableIt->students.end(), studentIt->name));
    studentTableIt->students.push_back(friendIt->name);
    friendTableIt->students.push_back(studentIt->name);
    studentIt->table = *friendTableIt;
    friendIt->table = *studentTableIt;
    cout << studentIt->name << SWITCH_MESSAGE << friendIt->name << EXCLAMATION_MARK << endl;
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
            auto student = findStudent(students, studentID);
            calculateTablesScore(tables, students, student);
            sort(tables.begin(), tables.end(), [](table table1, table table2){return table1.score > table2.score;});
            printTables(tables);
        }
        else if(command == RESERVE_TABLE) {
            int studentID, tableID;
            ss >> studentID;
            ss >> tableID;
            auto student = findStudent(students, studentID);
            if(ss.fail()) {
                calculateTablesScore(tables, students, student);
                auto table = findBestTable(tables);
                reserveTable(table, student);
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
            auto friendIt = findStudent(students, studentIt->friendID);
            auto tableIt = findTable(tables, studentIt->table.ID);
            exitStudent(students, tableIt, studentIt, friendIt);
        }
        else if(command == SWITCH) {
            int studentID;
            ss >> studentID;
            auto studentIt = findStudent(students, studentID);
            auto friendIt = findStudent(students, studentIt->friendID);
            switchStudents(tables, students, studentIt, friendIt);
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