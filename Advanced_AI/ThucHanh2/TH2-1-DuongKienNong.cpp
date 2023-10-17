#include <iostream>
#include <vector>
#include <cmath>
#include <algorithm>
#include <limits>
#include <fstream>

using namespace std;

string files[] = {"GTS1c.txt", "GTS1b.txt", "GTS1a.txt"};
string output_file = "gts1_output.txt";

int startCity; // Thành phố bắt đầu
vector<vector<int>> graph; //Mảng 2 chiều lưu ma trận chi phí
int n; // Số lượng thành phố 

bool readFile(const string& filename) {
    ifstream inputFile(filename, ios::in);

    if (!inputFile.is_open()) {
        cout << "Can not open file: " << filename << endl;
        return false;
    }
    inputFile >> n >> startCity;

    graph.resize(n, vector<int>(n)); //Chuẩn bị sẵn không gian để lưu ma trận chi phí

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
        outputFile << "\nTotal cost of the Tour: " << cost << endl;
        for (int city : tour) {
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
    tour.push_back(startCity);

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
                if (graph[current][j] < minCost) {
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

    // Chi phí quay về thành phố xuất phát
    totalCost += graph[tour.back()][startCity];

    writeFile(output_file, tour, totalCost);

    cout << "Total cost of the Tour: " << totalCost << endl;
}

int main() {
    for(const string &filename : files){
        clock_t begin = clock();
        readFile(filename);
        GTS1(graph, startCity);
        clock_t end = clock();
        cout<<"Time run: " << (float)(end-begin)/CLOCKS_PER_SEC << "s" << endl;
    }
    return 0;
}
