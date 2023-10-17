#include <iostream>
#include <vector>
#include <algorithm>
#include <fstream>

using namespace std;

string inputFiles[] = {"color4.txt", "color3.txt", "color2.txt", "color1.txt"};
string outputFile = "WelshPowell_output.txt";

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
        outputFile << "Chromatic number: " << numColors << endl;
        int temp = 0;
        while(temp != numColors){
            outputFile << "\nColor " << temp + 1 << ": ";
            for (int i = 0; i < n; i++) {
            if(color[i] == temp)
                outputFile << i << " ";
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

// Hàm kiểm tra màu c có thể tô được cho đỉnh hiện tại đang xét hay không
bool isSafe(vector<int>& color, int vertex, int c) {
    for (int i = 0; i < n; i++) {
        // Kiểm tra có cạnh giữa vertex và i
        // Kiểm tra đỉnh i đã được tô màu c chưa
        if (graph[vertex][i] && color[i] == c) {
            return false;
        }
    }
    return true;
}

// Hàm hỗ trợ hàm sort (so sánh để sắp xếp đỉnh theo thứ tự bậc cao nhất)
bool customComparator(const pair<int, int>& a, const pair<int, int>& b) {
    // Nếu 2 đỉnh có bậc bằng nhau thì so sánh chỉ số phần tử
    if (a.first == b.first) {
        return a.second > b.second;
    }
    return a < b;
}

void welshPowell(vector<vector<int>>& graph, int n) {
    vector<int> degree(n, 0);

    // Bước 1: Tìm  bậc của các đỉnh
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            if (graph[i][j]) {
                degree[i]++;
            }
        }
    }

    // Bước 2: Sắp xếp các đỉnh theo bậc giảm dần
    vector<pair<int, int>> order;
    for (int i = 0; i < n; i++) {
        order.push_back({degree[i], i});
    }
    sort(order.rbegin(), order.rend(), customComparator);

    // // In ra bảng màu
    // for(int i = 0; i < n; i++){
    //     cout << "Vertex " << order[i].second + 1 << " Degree " << order[i].first << endl;
    // }

    // Bước 3: Tô màu
    vector<int> color(n, -1);
    int c = 0;
    for (int i = 0; i < n; i++) {
        int vertex = order[i].second;
        // Dùng màu thứ nhất tô cho đỉnh có bậc cao nhất
        if (color[vertex] == -1) {
            color[vertex] = c;
            for(int j = i + 1; j < n; j++){
                // Tìm các đỉnh không kề với đỉnh đang xét có thể tô và tô nó
                if(color[order[j].second] == -1 && graph[vertex][order[j].second] == 0 && isSafe(color, order[j].second, c)){
                    color[order[j].second] = c;
                }
            }
            // Tô xong màu hiện tại và các đỉnh có thể tô, tăng màu lên để tô các đỉnh kế tiếp
            c++;
        }
    }

    cout << "\nChromatic number: " << c << endl; // Sắc số
    writeFile(outputFile, color, c);
}


int main() {
    for(const string &filename : inputFiles){
        clock_t begin = clock();
        readFile(filename);
        welshPowell(graph, n);
        clock_t end = clock();
        cout<<"Time run: " << (float)(end-begin)/CLOCKS_PER_SEC << "s" << endl;
    }
    return 0;
}
