#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <queue>
#include <cmath>

using namespace std;

struct Node {
    int x, y;
    int g, h, f;
    Node* parent;
    Node(int x, int y, int g, int h, Node* parent = nullptr)
            : x(x), y(y), g(g), h(h), f(g + h), parent(parent) {}
};

struct Compare {
    bool operator()(Node* a, Node* b) {
        return a->f > b->f;
    }
};

vector<pair<int, int>> directions{{-1, 0}, {1, 0}, {0, -1}, {0, 1}};

int heuristic(int x1, int y1, int x2, int y2) {
    return abs(x1 - x2) + abs(y1 - y2);
}

void printMap(vector<vector<char>>& map, Node* endNode) {
    while (endNode != nullptr) {
        if (map[endNode->x][endNode->y] != 'S' && map[endNode->x][endNode->y] != 'E')
            map[endNode->x][endNode->y] = '*';
        endNode = endNode->parent;
    }

    for (const auto& row : map) {
        for (const auto& cell : row) {
            cout << cell << " ";
        }
        cout << endl;
    }
}

bool isValid(int x, int y, const vector<vector<char>>& map) {
    return x >= 0 && x < map.size() && y >= 0 && y < map[0].size() && map[x][y] != '0';
}

void AStar(vector<vector<char>>& map, int sx, int sy, int ex, int ey) {
    priority_queue<Node*, vector<Node*>, Compare> openSet;
    openSet.push(new Node(sx, sy, 0, heuristic(sx, sy, ex, ey)));

    vector<vector<bool>> closedSet(map.size(), vector<bool>(map[0].size(), false));

    while (!openSet.empty()) {
        Node* current = openSet.top();
        openSet.pop();

        if (current->x == ex && current->y == ey) {
            cout << "Goal Reached" << endl;
            printMap(map, current);
            return;
        }

        closedSet[current->x][current->y] = true;

        for (const auto& dir : directions) {
            int nx = current->x + dir.first;
            int ny = current->y + dir.second;

            if (isValid(nx, ny, map) && !closedSet[nx][ny]) {
                int g = current->g + 1;
                int h = heuristic(nx, ny, ex, ey);
                openSet.push(new Node(nx, ny, g, h, current));
            }
        }
    }

    cout << "No Path Found" << endl;
}
vector<vector<char>> readMapFromFile(const string& filename) {
    ifstream file(filename);
    vector<vector<char>> map;
    string line;

    if (file.is_open()) {
        while (getline(file, line)) {
            vector<char> row;
            for (char& ch : line) {
                if (ch == '0' || ch == '1' || ch == 'S' || ch == 'E') {
                    row.push_back(ch);
                }
            }
            if (!row.empty()) {
                map.push_back(row);
            }
        }
        file.close();
    } else {
        cerr << "Unable to open file: " << filename << std::endl;
    }

    return map;
}

int main() {
    vector<vector<char>> map = readMapFromFile("easy.txt");

    int sx = 0, sy = 0, ex = 0, ey = 0;
    for (int i = 0; i < map.size(); i++) {
        for (int j = 0; j < map[i].size(); j++) {
            if (map[i][j] == 'S') {
                sx = i;
                sy = j;
            } else if (map[i][j] == 'E') {
                ex = i;
                ey = j;
            }
        }
    }

    AStar(map, sx, sy, ex, ey);

    return 0;
}