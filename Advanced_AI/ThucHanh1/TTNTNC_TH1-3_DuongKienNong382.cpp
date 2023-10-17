#include <iostream>
#include <fstream>
#include <climits>
#include <string>
#include <chrono>
#define START 0

using namespace std;
using namespace std::chrono;

int **a, n;
int *x;
int total;
int *best;
int totalBest;
bool *d;

void readData(const string &filename){
    ifstream f;
    f.open(filename.c_str(), ios::in);
    f >> n;
    a = new int*[n];
    for(int i = 0; i < n; i++){
        a[i] = new int[n];
    }
    for(int i = 0; i < n; i++){
        for(int j = 0; j < n; j++){
            f >> a[i][j];
        }
    }
    f.close();
}

void initData(){
    x = new int[n];
    best = new int[n];
    totalBest = INT_MAX;
    total = 0;
    d = new bool[n];

    for(int i = 0; i < n; i++){
        d[i] = false;
    }
    x[0] = START;
    d[START] = true;
}

void output(){
    ofstream f;
    f.open("tsp_output.txt", ios::out | ios::app);
    f << totalBest << endl;
    for(int i = 0; i < n; i++){
        f << best[i] + 1 << " ";
    }
    f << START + 1 << endl;
    f.close();
}

void update(){
    if(a[x[n-1]][START] > 0 && total + a[x[n-1]][START] < totalBest){
        totalBest = total + a[x[n-1]][START];
        for(int i = 0; i < n; i++){
            best[i] = x[i];  
        }
    }
}

void tsp(int idx){
    // Điều kiện nhánh cận
    if(total > totalBest){
        return;
    }
    for(int i = 0; i < n; i++){
        if(d[i] == false && a[x[idx-1]][i] > 0){
            x[idx] = i;
            d[i] = true;
            total += a[x[idx-1]][i];
            
            if(idx == n - 1){
                update();
            }
            else{
                tsp(idx + 1);
            }
            d[i] = false;
            total -= a[x[idx-1]][i];
        }
    }
}
int main(){
    string files[] = {"tsp_12vertex.txt", "tsp_15vertex.txt", "tsp_20vertex.txt", "tsp_25vertex.txt"};
    //string files[] = {"tsp_30vertex.txt", "tsp_30vertex.txt", "tsp_35vertex.txt", "tsp_40vertex.txt","tsp_45vertex.txt"};

    for(const string &filename : files){
        //Đếm thời gian thực thi
        auto start_time = high_resolution_clock::now();

        readData(filename);
        initData();
        tsp(1);
        output();

            
        auto end_time = high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time);
        cout << "Time: " << duration.count() / 1000.0 << "s" << endl;

        delete[] d;
        delete[] x;
    
        for(int i = 0; i < n; i++){
            delete[] a[i];
        }
        delete[] a;
    }
    return 0;
}