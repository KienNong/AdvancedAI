#include <iostream>
#include <fstream>
#include <vector>
#include <cmath>


using namespace std;

string inputFile[] = {"taci4.txt", "taci3.txt", "taci2.txt", "taci1.txt", "taci5.txt"};

int n;

vector<vector<int>> matrix;

vector<vector<int>> initialStateMatrix; //Trạng thái ban đầu
vector<vector<int>> endStateMatrix; //Trạng thái kết thúc

bool readFile(const string& filename){
    ifstream inputFile(filename, ios::in);
    if (!inputFile.is_open()) {
        cout << "Can not open file: " << filename << endl;
        return false;
    }
    inputFile >> n;

    initialStateMatrix.resize(n, vector<int>(n));
    endStateMatrix.resize(n, vector<int>(n));

    for(int i = 0; i < n * 2; i++){
        for(int j = 0; j < n; j++){
            if(i < n){
                inputFile >> initialStateMatrix[i][j];
            }
            else{
                inputFile >> endStateMatrix[i - n][j];
            }
        }
    }

    inputFile.close();
    return true;
}

int calculateH1(const vector<vector<int>>& state){
    int h1 = 0;
    for (int i = 0; i < n; ++i) 
        for (int j = 0; j < n; ++j){
            if(endStateMatrix[i][j] == 0) // bỏ qua vị trí = 0
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


int main(){

    for(string filename : inputFile){
        clock_t begin = clock();
        readFile(filename);
        
        cout << filename << endl;
        int h1 = calculateH1(initialStateMatrix);
        int h2 = calculateH2(initialStateMatrix);
        cout << "H1: " << h1 << endl;
        cout << "H2: " << h2 << endl;
        
        clock_t end = clock();
        cout<<"Time run: " << (float)(end-begin)/CLOCKS_PER_SEC << "s\n" << endl;
    }

    return 0;
}