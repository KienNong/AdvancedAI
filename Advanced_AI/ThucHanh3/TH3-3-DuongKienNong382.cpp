#include <iostream>
#include <vector>
#include <map>
#include <stack>
#include <cmath>
#include <algorithm>
#include <fstream>
#include <sstream>
#include <ctime>
#include <chrono>

using namespace std;

struct State {
    vector<vector<int>> towers;
    int g; 
    int h; 

    State(vector<vector<int>> _towers, int _g, int _h) : towers(_towers), g(_g), h(_h) {}
};


vector<vector<int>> initialState;
map<vector<int>, int> H;
int n;

bool compare(const State &a, const State &b) {
    if ((a.g + a.h) == (b.g + b.h))
        return a.g < b.g;
    return a.g + a.h < b.g + b.h;
}

void recursionR(vector<vector<int>> &towers, int number, int l, int r, int c);
void recursionL(vector<vector<int>> &towers, int number, int l, int r, int c);

void recursion (vector<vector<int>> &towers, int number, int l, int r, int c){
    int mid = r / 2;
    recursionL(towers, number, l, mid, c);
    recursionR(towers, number, mid + 1, r, c - 1);
}

void recursionL(vector<vector<int>> &towers, int number, int l, int r, int c){ 
    for (int i = 0; i <= r; i++){
        towers[i][number] = c;
    }
    if(r == 1){
        towers[0][number+1] = 1;
        return;
    }
    recursion(towers, number + 1, l, r, c - 1);
}

void recursionR(vector<vector<int>> &towers, int number, int l, int r, int c){
    if((r - l) == 1){
        towers[r][number] = 1;
        return;
    }

    int mid = (l + r) / 2;
    for (int i = mid + 1; i <= r; i++){
        towers[i][number] = c;
    }
    recursionR(towers, number, l, mid, c - 1);
    recursionR(towers, number + 1, mid + 1, r, c - 1);
}

void heuristic(){
    vector<vector<int>> towers;
    towers.resize(pow(2, n), vector<int>(n));

    recursion(towers, 0, 0, pow(2, n) - 1, n);

    for(int i = 0; i < pow(2, n); i++){
        H[towers[i]] = i;
    }
}

int calculateHeuristic(vector<vector<int>>& state){
    vector<int> temp = state[2];
    if (H[temp] != 0) 
        return H[temp];
    return -1;
}

bool readFile(string filePath) {
    ifstream inputFile(filePath);

    if (!inputFile.is_open()) {
        cerr << "can't read file: " << filePath << endl;
        return false;
    }

    string line;
    int i = 0;
    int j = 0;
    int number;
    inputFile >> n;    

    initialState = vector<vector<int>>();
    initialState.resize(n, vector<int>(n));
  
    getline(inputFile, line);

    while (getline(inputFile, line)) {
        if (line == "0")
            break;

        istringstream iss(line); 
        while (iss >> number) {
            initialState[i][j] = number;
            j++;
        }
        j = 0;
        i++;
    }

    inputFile.close();
    return true;
}

int check(State state, int a, int b, map<vector<vector<int>>, int> &closed_set, vector<State> &temp){
    vector<vector<int>> new_tower = state.towers;
    int a_i = -1;
    int b_i = -1;
    for(int i = n - 1; i >= 0; i--){
        if(new_tower[a][i] != 0 && a_i == -1)
            a_i = i;
        if(new_tower[b][i] != 0 && b_i == -1)
            b_i = i;
        if(a_i != -1 && b_i != -1)
            break;
    }
    if(a_i == -1 || b_i == (n - 1)){
        return false;
    }
    if(new_tower[a][a_i] < new_tower[b][b_i] || b_i == -1){
        swap(new_tower[a][a_i], new_tower[b][b_i+1]);
        if (closed_set.find(new_tower) == closed_set.end()){
            temp.push_back(State(new_tower, state.g + 1, calculateHeuristic(new_tower)));
        }
    }
    return false;
}

void child_state(State current, stack<State> &open_set, map<vector<vector<int>>, int> &closed_set){
    int flag = 0;
    vector<State> temp;

    while (flag < 3){ 
        if(flag == 0){
            check(current, 0, 1, closed_set, temp);
            check(current, 0, 2, closed_set, temp);
        }
        else if(flag == 1){
            check(current, 1, 2, closed_set, temp);
            check(current, 1, 0, closed_set, temp);
        }
        else {
            check(current, 2, 1, closed_set, temp);
            check(current, 2, 0, closed_set, temp);
        }
        flag++;
    }
    sort(temp.begin(), temp.end(), compare);

    while(!temp.empty()){
        open_set.push(temp.back()); 
        temp.pop_back();           
    }
}

int solveHanoiTower() {
    heuristic();
    map<vector<vector<int>>, int> closed_set;
    int flag;
    int h_initial = calculateHeuristic(initialState);

    stack<State> open_set;
    open_set.push(State(initialState, 0, h_initial));
    
    while (!open_set.empty()) {

        State current = open_set.top(); 
        open_set.pop();
        flag = 0;

        while(closed_set.find(current.towers) != closed_set.end()){ 
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

        closed_set[current.towers] = current.g;

        if (H[current.towers[2]] == 0) { 
            return current.g; 
        }
        
        child_state(current, open_set, closed_set);
    }

    return -1; 
}

int main() {

    string inputFile[] = {"HNtower3.txt","HNtower2.txt","HNtower1.txt"};

    for(string filename : inputFile){
        clock_t begin = clock();
        readFile(filename);

        cout << solveHanoiTower() << endl;
        
        clock_t end = clock();
        cout<<"Time run: " << (float)(end-begin)/CLOCKS_PER_SEC << "s" << endl;
    }

    return 0;
}