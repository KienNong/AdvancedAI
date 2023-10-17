#include <iostream>
#include <vector>
#include <algorithm>
#include <fstream>
#include <sstream>

using namespace std;

const int MAXN = 100;
int n;
vector<vector<int>> graph;
int colors[MAXN];
int chromaticNumber;

bool readFile(const string& filename) {
    ifstream inputFile(filename, ios::in);

    if (!inputFile.is_open()) {
        cerr << "Không thể mở file: " << filename << endl;
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


bool isSafe(int v, int c) {
    for (int i = 0; i < n; i++) {
        if (graph[v][i] && c == colors[i]) {
            return false;
        }
    }
    return true;
}

bool graphColoringUtil(int v) {
    if (v == n) {
        return true;
    }

    for (int c = 1; c <= chromaticNumber; c++) {
        if (isSafe(v, c)) {
            colors[v] = c;
            if (graphColoringUtil(v + 1)) {
                return true;
            }
            colors[v] = 0;
        }
    }

    return false;
}

void graphColoring(const string& filename) {
    ofstream outputFile(filename, ios::app);

    if (outputFile.is_open())
    {
        for (chromaticNumber = 1; chromaticNumber <= n; chromaticNumber++) {
            fill(colors, colors + n, 0);
            if (graphColoringUtil(0)) {
                outputFile << "Chromatic Number: " << chromaticNumber << endl;
                outputFile << "Colors:" << endl;
                for (int i = 0; i < n; i++) {
                    outputFile << i + 1 << " " << colors[i] << endl;
                }
                outputFile << endl;
                return;
            }
        }

        outputFile.close();
    }
    else
    {
        cerr << "Không thể mở tệp để ghi: " << filename << endl;
    }
}

int main() {
    string files[] = {"color_35vertex.txt", "color_30vertex.txt", "color_20vertex.txt", "color_25vertex.txt"};
    //string files[] = {"color_40vertex.txt", "color_45vertex.txt", "color_50vertex.txt"};


    string outputFile = "nc_color_output.txt";
    // string inputFile = "color_35vertex.txt";
    // clock_t begin = clock();
    // readFile(inputFile);
    // graphColoring(outputFile);

    // clock_t end = clock();
    // cout << "\nTime run: " << (float)(end-begin)/CLOCKS_PER_SEC << "s" << endl;

    for (const string &filename : files){
        clock_t begin = clock();

        readFile(filename);
        graphColoring(outputFile);

        clock_t end = clock();
        cout << "Time run: " << (float)(end-begin)/CLOCKS_PER_SEC << "s"<<endl;
    }
    return 0;
}