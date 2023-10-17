#include <iostream>
#include <vector>
#include <cmath>
#include <algorithm>
#include <limits>
#include <fstream>

using namespace std;

string files[] = {"GTS2c.txt", "GTS2b.txt", "GTS2a.txt"};
string output_file = "gts2_output.txt";
int numOfStartCity; // Thành phố bắt đầu    
vector<vector<int>> graph; //Mảng 2 chiều lưu ma trận chi phí
vector<int> p;
int n; // Số lượng thành phố

int bestCost = INT_MAX; //Chi phí tốt nhất
vector<int> bestTour; //Tour tốt nhất

bool readFile(const string& filename) {
    ifstream inputFile(filename, ios::in);

    if (!inputFile.is_open()) {
        cout << "Can not open file: " << filename << endl;
        return false;
    }
    
    // Đọc file 2 số đầu (Số lượng thành phố, số lượng thành phố bắt đầu)
    inputFile >> n >> numOfStartCity;

    p.resize(numOfStartCity);

    // Các thành phố bắt đầu trong vector p
    for (int i = 0; i < numOfStartCity; i++){
        inputFile >> p[i];
    }

    //Chuẩn bị sẵn không gian để lưu ma trận chi phí
    graph.resize(n, vector<int>(n));

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            inputFile >> graph[i][j];
        }
    }
    inputFile.close();
    return true;
}

void writeFile(const string& filePath, vector<int> tour, int cost)
{
    ofstream outputFile(filePath, ios::app);
    if (outputFile.is_open()){
        outputFile << "\nBest cost: " << bestCost << endl;
        outputFile << "Best tour: ";
        for (int city : bestTour) {
            outputFile << city + 1 << " ";
        }
        outputFile.close();
    }
    else{
        cout << "Can not open file to write: " << filePath << endl;
    }
}

void GTS1(vector<vector<int>> graph, int startCity) {
    vector<int> tour;

    // Thành phố xuất phát u
    startCity = startCity - 1;

    // Chi phí ứng với tour tìm được
    int totalCost = 0;

    // tour = {u}
    tour.push_back(startCity);\

    // Đặt vector visited đánh dấu thành phố được thăm
    vector<bool> visited(n, false);

    // Đánh dấu thành phố đầu tiên
    visited[startCity] = true;

    for (int i = 1; i < n; ++i) {
        // Đặt biến current (thành phố đang xét v);
        // nearest (thành phố kề sau thành phố v); minCost (chi phí thấp nhất);
        int current = tour.back();
        int nearest = -1;
        int minCost = INT_MAX;

        for (int j = 0; j < n; ++j) {
            // Nếu thành phố kế tiếp chưa được thăm
            if (!visited[j]) {
                // Cập nhật chi phí thấp nhất từ thành phố hiện tại đến thành phố tiếp theo
                if (graph[current][j] < minCost) {\
                    minCost = graph[current][j];
                    nearest = j;
                }
            }
        }
        // Thêm thành phố kề sau vào tour và đánh dấu đã được thăm
        // Cập nhật chi phí ứng với tour
        tour.push_back(nearest);
        visited[nearest] = true;
        totalCost += minCost;
    }

    totalCost += graph[tour.back()][startCity]; // Chi phí quay về thành phố xuất phát

    // ---- GTS 2 ----
    // Cập nhật GTS 2 với chi phí tốt nhất và đường đi tốt nhất
    if(totalCost < bestCost){
        bestCost = totalCost;
        bestTour = tour;
    }
}

void GTS2(vector<vector<int>> graph, vector<int> p){
    for(int i = 0; i < numOfStartCity; i++){
        GTS1(graph, p[i]);
    }
}

int main() {
    // for(const string &filename : files){
    //     clock_t begin = clock();
    //     readFile(filename);
    //     vector<int> tour = GTS2(graph, p);
    //     clock_t end = clock();
    //     cout<<"Time run: " << (float)(end-begin)/CLOCKS_PER_SEC << "s" << endl;
    // }
    for (const string &filename : files) {

        readFile(filename);
        clock_t begin = clock();
        GTS2(graph, p);
        writeFile(output_file, bestTour, bestCost);

        cout << "Best Total Cost: " << bestCost << endl;

        // cout << "Best Tour: ";
        // for(int city : bestTour){
        //     cout << city + 1 << " ";
        // }

        clock_t end = clock();
        cout << "Time run: " << (float)(end-begin)/CLOCKS_PER_SEC << "s" << endl;
    }

    return 0;
}
