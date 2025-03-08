#include <iostream>
#include <string>
#include <vector>

using namespace std;

struct table {
    int ID;
    int capacity;
    int bonus;
    int score = 0;
    vector<int> coordination;
    vector<struct student> students;
    vector<struct student> queue;
};

struct student {
    string name;
    int ID;
    int friendID;
    int enemyID;
    struct table table = NULL;
};


struct system{
    vector<struct table> tables;
    vector<struct student> students;
};

int main() {
    return 0;
}
