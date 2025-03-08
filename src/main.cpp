#include <iostream>
#include <string>
#include <vector>


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

int main(int argc,char* argv[]) {

}
