#include <iostream>
#include <vector>
using namespace std;

struct student {
    string name;
    int ID;
    int friendID;
    int enemyID;
};

struct table {
    int ID;
    vector<int> coordination;
    int capacity;
    int bonus;
    vector<student> students;
    vector<student> queue;
    int score;
};

int main() {
    return 0;
}
