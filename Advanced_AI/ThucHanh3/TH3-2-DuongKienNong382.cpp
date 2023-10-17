#include <iostream>
#include <vector>
#include <map>
#include <stack>
#include <fstream>
#include <sstream>
#include <ctime>
#include <chrono>
#include <algorithm>
using namespace std;

int n;
vector<vector<int>> inititalStateMatrix;
vector<vector<int>> endStateMatrix;

struct Node {
    vector<vector<int>> state; // Trạng thái của nút
    int g; // Số lần đẩy g
    int h; // Heuristic
    Node(vector<vector<int>> _state, int _g, int _h) : state(_state), g(_g), h(_h) {}
};

bool compare(const Node &a, const Node &b) {
    if ((a.g + a.h) == (b.g + b.h))
        return a.g < b.g;
    return a.g + a.h < b.g + b.h;
}

bool readFile(string filePath) {
    ifstream inputFile(filePath);

    if (!inputFile.is_open()) {
        cerr << "Can not read file: " << filePath << endl;
        return false;
    }

    inputFile >> n;

    inititalStateMatrix = vector<vector<int>>();
    inititalStateMatrix.resize(n, vector<int>(n));

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            inputFile >> inititalStateMatrix[i][j];
        }
    }

    endStateMatrix = vector<vector<int>>();
    endStateMatrix.resize(n, vector<int>(n));

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            inputFile >> endStateMatrix[i][j];
        }
    }

    inputFile.close();
    return true;
}

// Tính H1
int calculateH1(const vector<vector<int>>& state) {
    int h1 = 0;
    for (int i = 0; i < n; ++i) 
        for (int j = 0; j < n; ++j){
            if (endStateMatrix[i][j] == 0)
                continue;
            if (state[i][j] != endStateMatrix[i][j]) 
                h1++;
        }
    return h1;
}

int calculateH2(const vector<vector<int>>& state) {
    int h2 = 0;
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            int value = state[i][j];
            if (value != 0) { // bỏ qua vị trí = 0
                for (int x = 0; x < n; ++x) {
                    for (int y = 0; y < n; ++y) {
                        if (value == endStateMatrix[x][y]) {
                            h2 += abs(i - x) + abs(j - y); 
                        }
                    }
                }
            }
        }
    }
    return h2;
}

void check(Node current, stack<Node> &open_set, map<vector<vector<int>>, int> &closed_set){
    vector<Node> temp;
    int i_zero, j_zero;
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            if (current.state[i][j] == 0) {
                i_zero = i;
                j_zero = j;
                break;
            }
        }
    }
    int dx[] = {-1, 1, 0, 0};
    int dy[] = {0, 0, -1, 1};

    for (int d = 0; d < 4; ++d) {
        int i_new = i_zero + dx[d];
        int j_new = j_zero + dy[d];
        if (i_new >= 0 && i_new < n && j_new >= 0 && j_new < n) {
            vector<vector<int>> new_state = current.state; 
            swap(new_state[i_zero][j_zero], new_state[i_new][j_new]); 

            int h_new = calculateH1(new_state);

            if (closed_set.find(new_state) == closed_set.end()){
                temp.push_back(Node(new_state, current.g + 1, h_new)); 
            }
        }
        
    }
    sort(temp.begin(), temp.end(), compare);
    while(!temp.empty()){
        open_set.push(temp.back()); 
        temp.pop_back();
    }
}

int solveTACIwithH1() {
    map<vector<vector<int>>, int> closed_set; 
    int flag;
    int h_initial = calculateH1(inititalStateMatrix);
    stack<Node> open_set;
    open_set.push(Node(inititalStateMatrix, 0, h_initial));
    while (!open_set.empty()) {
        Node current = open_set.top();
        open_set.pop();
        flag = 0;
        while(closed_set.find(current.state) != closed_set.end()){
            current = open_set.top();
            open_set.pop();
            if(open_set.empty()){
                flag = 1;
                break;
            }
        }
        if(flag){
            break;
        }
        closed_set[current.state] = current.g;

        if (current.state == endStateMatrix) {
            return current.g; 
        }
        check(current, open_set, closed_set);
    }
    return -1;
}

int solveTACIwithH2() {
    map<vector<vector<int>>, int> closed_set; 
    int flag;
    int h_initial = calculateH2(inititalStateMatrix);
    stack<Node> open_set;
    open_set.push(Node(inititalStateMatrix, 0, h_initial));
    while (!open_set.empty()) {
        Node current = open_set.top();
        open_set.pop();
        flag = 0;
        while(closed_set.find(current.state) != closed_set.end()){
            current = open_set.top();
            open_set.pop();
            if(open_set.empty()){
                flag = 1;
                break;
            }
        }
        if(flag){
            break;
        }
        closed_set[current.state] = current.g;

        if (current.state == endStateMatrix) {
            return current.g; 
        }
        check(current, open_set, closed_set);
    }
    return -1;
}

int main() {
    string inputFile[] = {"taci5.txt"};
    for(string filename : inputFile){
        clock_t begin = clock();
        readFile(filename);
        int g1 = solveTACIwithH1();
        int g2 = solveTACIwithH2();
        if (g1 != -1 || g2 != -1) {
            cout << "g with H1: " << g1 << " and with H2: " << g2 << endl;
        } else {
            cout << "Can't find!" << endl;
        }
        clock_t end = clock();
        cout<<"Time run: " << (float)(end-begin)/CLOCKS_PER_SEC << "s" << endl;
    }
    return 0;
}