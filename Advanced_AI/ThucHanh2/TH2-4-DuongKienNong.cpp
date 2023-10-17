#include <iostream>
#include <vector>
#include <fstream>
#include <algorithm>
#include <set>
using namespace std;

string inputFiles[] = {"color4.txt", "color3.txt", "color2.txt", "color1.txt"};
string outputFile = "DSATUR_output.txt";

int n;
vector<vector<int>> graph;

bool readFile(const string& filename) {

    ifstream inputFile(filename, ios::in);

    if (!inputFile.is_open()) {
        cout << "Can not open file: " << filename << endl;
        return false;
    }
    inputFile >> n;

    graph.resize(n, vector<int>(n));

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            inputFile >> graph[i][j];
        }
    }

    inputFile.close();
    return true;
}

void writeFile(const string& filePath, vector<int> color, int numColors)
{
    ofstream outputFile(filePath, ios::app);
    if (outputFile.is_open()){
        outputFile << "Chromatic number: " << numColors + 1 << endl;
        int temp = 0;
        while(temp != numColors){
            outputFile << "\nColor " << temp + 1 << ": ";
            for (int i = 0; i < n; i++) {
            if(color[i] == temp)
                outputFile << i + 1 << " ";
            }
            temp++;
        }
        outputFile << "\n-------\n";
        outputFile.close();
    }
    else{
        cout << "Can not open file to write: " << filePath << endl;
    }
}

// Hàm hỗ trợ hàm sort (so sánh để sắp xếp đỉnh theo thứ tự bậc cao nhất)
bool customComparator(const pair<int, int>& a, const pair<int, int>& b) {
    // Nếu 2 đỉnh có bậc bằng nhau thì so sánh chỉ số phần tử
    if (a.second == b.second) {
        return a.first < b.first;
    }
    return a.second > b.second;
}

void DSATUR(vector<vector<int>> graph, int V) {
    vector<pair<int, int>> degree(V);
    vector<int> color(V, -1);
    vector<int> *adj = new vector<int> [V];
    int chromaticNumber = -1;

    // Tìm các đỉnh kề đỉnh đang xét và bậc của các đỉnh đang xét
    for(int i = 0; i < V; i++){
        for(int j = 0; j < V; j++){
            if(graph[i][j] == 1){
                adj[i].push_back(j); //Các đỉnh kề đỉnh đang xét
                degree[i].second++; // Bậc của các đỉnh đang xét
            }
        }
    }
    for (int i = 0; i < V; i++) {
        degree[i].first = i;
        degree[i].second = adj[i].size();
    }

    // Sắp xếp bậc của các đỉnh để phần tử mảng đầu tiên là đỉnh có bậc lớn nhất
    sort(degree.begin(), degree.end(), customComparator);

    // for(int i = 0; i < n; i++){
    //     cout << "Vertex " << degree[i].first + 1 << " Degree " << degree[i].second << endl;
    // }
    
    // Tìm màu để tô đỉnh đầu tiên là màu nhỏ nhất, màu đầu tiên là màu 0 
    color[degree[0].first] = 0;

    // Duyệt từ đỉnh thứ 2 đến các đỉnh còn lại
    for (int i = 1; i < V; i++) {

        // Tạo vector theo dõi màu đã được sử dụng
        vector<bool> used(V, false);

        for (int j : adj[degree[i].first]) {
            // Duyệt các đỉnh kề, nghiêm cấm tô màu j cho các đỉnh kề đỉnh đang xét
            if (color[j] != -1) {
                used[color[j]] = true;
            }
        }

        // Tìm màu bé nhất (cr) mà chưa được sử dụng 
        int cr;
        for (cr = 0; cr < V; cr++) {
            if (!used[cr]) {
                break;
            }
        }

        // Tô màu đỉnh đang xét là cr 
        color[degree[i].first] = cr;

        // Cập nhật bậc đỉnh đang xét là 0
        degree[i].second = 0;

        for (int j : adj[degree[i].first]) {
            if (color[j] == -1) {
                // Cập nhật độ bão hoà (saturation) 
                // Cập nhật bậc của các đỉnh kề với đỉnh được tô giảm đi 1 đơn vị
                degree[j].second--;
            }
        }
    }

    // Duyệt danh sách màu để tìm màu lớn nhất ~ sắc số của đồ thị
    for (int c : color) {
        if (c > chromaticNumber) {
            chromaticNumber = c;
        }
    }

    // cout << "Vertex\tColor\n";
    // for (int i = 0; i < V; i++) {
    //     cout << i + 1 << "\t" << color[i] + 1 << "\n";
    // }

    // //In màu
    // int temp = 0;
    // while(temp != chromaticNumber + 1){
    //     cout << "\nColor " << temp + 1 << ": ";
    //     for (int i = 0; i < V; i++) {
    //         if(color[i] == temp)
    //             cout << i + 1 << " ";
    //     }
    //     temp++;
    // }
    cout << "\nChromatic number: " << chromaticNumber + 1 << endl;

    writeFile(outputFile, color, chromaticNumber);
}

int main() {
    for(const string &filename : inputFiles){
        clock_t begin = clock();
        readFile(filename);
        DSATUR(graph, n);
        clock_t end = clock();
        cout<<"Time run: " << (float)(end-begin)/CLOCKS_PER_SEC << "s" << endl;
    }
    return 0;
}