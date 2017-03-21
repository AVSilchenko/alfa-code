#include <iostream>
#include <vector>
#include <algorithm>
#include <set>
#include <map>

using std::cout;
using std::cin;
using std::vector;

size_t dfs_time = 0;
int minimum_cost = -1;
vector<bool> used;
vector<size_t> time_in;
vector<size_t> tail_time;
vector<vector<int>> roads;
std::map<std::pair<size_t, size_t>, size_t> road_cost;

void CheckBridge(size_t from, size_t to) {
    size_t cost = road_cost[std::pair<size_t, size_t>(from, to)];

    if (minimum_cost == -1) {
        minimum_cost = cost;
        return;
    } 
    if (minimum_cost > cost) {
        minimum_cost = cost;
    }
}


void dfs(int vertex, int parent = -1) {
    time_in[vertex] = tail_time[vertex] = dfs_time++;
    used[vertex] = true;
    for (size_t iter = 0; iter < roads[vertex].size(); ++iter) {
        size_t to = roads[vertex][iter];
        if (to != parent) {
            if (!used[to]) {
                dfs(to, vertex);
                tail_time[vertex] = std::min(tail_time[vertex], tail_time[to]);
                if (tail_time[to] > time_in[vertex])
                    CheckBridge(vertex, to);
            } else {
                tail_time[vertex] = std::min(tail_time[vertex], time_in[to]);
            }
        }
    }
}

int main() {
    size_t number_of_cities;
    cin >> number_of_cities;
    used.resize(number_of_cities, false);
    time_in.resize(number_of_cities);
    tail_time.resize(number_of_cities);

    size_t number_of_roads;
    cin >> number_of_roads;
    roads.resize(number_of_cities);

    for (size_t iter = 0; iter < number_of_roads; ++iter) {
        size_t from, to, cost;
        cin >> from >> to >> cost;
        --from;
        --to;
        road_cost[std::pair<size_t, size_t>(from, to)] = cost;
		road_cost[std::pair<size_t, size_t>(to, from)] = cost;
        roads[from].push_back(to);
		roads[to].push_back(from);
    }

    dfs(0);

    cout << minimum_cost;
    return 0;
}