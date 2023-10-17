#include <bits/stdc++.h>
#include <vector>
#include <climits>

using namespace std;

int n; // Biến toàn cục n lưu số thành phố (đỉnh)
vector<vector<int>> graph; //Mảng 2 chiều lưu ma trận khoảng cách giữa các đỉnh

// Hàm để đọc ma trận khoảng cách từ file
bool readFile(const string& filename) {
    ifstream inputFile(filename, ios::in);

    if (!inputFile.is_open()) {
        cout << "Không thể mở file: " << filename << endl;
        return false;
    }

    inputFile >> n;
    graph.resize(n, vector<int>(n)); //Chuẩn bị sẵn không gian để lưu ma trận khoảng cách

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            inputFile >> graph[i][j];
        }
    }

    inputFile.close();
    return true;
}

// pos: đỉnh hiện tại đang xét, visited: biến theo dõi 'được thăm', state: lưu đường đi tốt nhất
int tsp(int pos, int visited, vector<vector<int>>& state) {
    if (visited == ((1 << n) - 1)) {
        return graph[pos][0];
    }
    if (state[pos][visited] != -1) {
        return state[pos][visited];
    }
    int ans = INT_MAX;
    for (int i = 0; i < n; i++) {
        if ((visited & (1 << i)) == 0) {
            int newAns = graph[pos][i] + tsp(i, visited | (1 << i), state);
            ans = min(ans, newAns); //lưu khoảng cách ngắn nhất từ pos đến 1 đỉnh chưa thăm
        }
    }
    state[pos][visited] = ans; //Lưu giá trị ans để tái sử dụng cho những vòng lặp sau
    return ans; // Khoảng cách ngắn nhất từ đỉnh pos xuất phát đến các đỉnh chưa được thăm
}

void printPath() {
    vector<vector<int>> state(n, vector<int>(1 << n, -1));
    int result = tsp(0, 1, state);
    cout << "The optimal path is: ";
    int curr = 0;
    int visited = 1;
    cout << curr << " ";
    for (int i = 0; i < n - 1; i++) {
        int index = -1;
        for (int j = 0; j < n; j++) {
            // Tìm đỉnh tiếp theo để thăm trong đường đi tối ưu (kiểm  tra đỉnh j chưa được thăm, khoảng cách từ curr đến j cộng với trạng thái tối ưu của j và visited < khoảng cách từ curr đến index cộng với trạng thái tối ưu của index)
            if ((visited & (1 << j)) == 0 && (index == -1 || graph[curr][j] + state[j][visited | (1 << j)] < graph[curr][index] + state[index][visited | (1 << index)])) {
                index = j;
            }
        }
        cout << index << " ";
        visited |= 1 << index; // đánh dấu đỉnh được thăm bằng cách set visited = 1
        curr = index;
    }
    cout << "0" << endl;
    cout << "The optimal cost is: " << result << endl;
}

void writeFile(const string& filePath)
{
    ofstream outputFile(filePath, ios::app);
    if (outputFile.is_open()){
        vector<vector<int>> state(n, vector<int>(1 << n, -1));
        int result = tsp(0, 1, state);
        outputFile << result << endl;        
        int curr = 0;
        int visited = 1;
        outputFile << curr+1 << " ";
        for (int i = 0; i < n - 1; i++) {
            int index = -1;
            for (int j = 0; j < n; j++) {
                if ((visited & (1 << j)) == 0 && (index == -1 || graph[curr][j] + state[j][visited | (1 << j)] < graph[curr][index] + state[index][visited | (1 << index)])) {
                    index = j;
                }
            }
            outputFile << index+1 << " ";
            visited |= 1 << index;
            curr = index;
        }
        outputFile << "1" << endl;
        outputFile.close();
    }
    else{
        cout << "Không thể mở tệp để ghi: " << filePath << endl;
    }
}

int main() {
    string files[] = {"tsp_12vertex.txt", "tsp_15vertex.txt", "tsp_20vertex.txt", "tsp_25vertex.txt"};
    //string files[] = {"tsp_30vertex.txt", "tsp_35vertex.txt", "tsp_40vertex.txt", "tsp_45vertex.txt", "tsp_50vertex.txt"};
    string opFile = "qhd_tsp_output.txt";
    for(const string &filename : files){
        clock_t begin = clock();
        readFile(filename);
        writeFile(opFile);
        clock_t end = clock();
        cout<<"Time run: " << (float)(end-begin)/CLOCKS_PER_SEC << "s" << endl;
    }
}
