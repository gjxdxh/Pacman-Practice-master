#ifndef SEARCH_H
#define SEARCH_H

#include <vector>
#include <iostream>
#include <string>
#include <queue>
#include <utility>
#include <stdio.h>
#include <limits>
#include <cmath>
#include <stdlib.h>
#include <unordered_map>
#include "solution.h"
#include <climits>
#include <cfloat>
using namespace std;

/**
 * This class contains all four searching methods.
*/
class Search
{
  public:
    static Solution DFS(vector<string> maze, int x, int y, int tx, int ty);
    static Solution BFS(vector<string> maze, int x, int y, int tx, int ty);
    static Solution greedy(vector<string> maze, int x, int y, int finalx, int finaly);
    static Solution A_star(vector<string> maze, int x, int y, int finalx, int finaly);
    static Solution super_Astar(vector<string> maze, int x, int y, vector<pair<int, int>> targets);
};

#endif
