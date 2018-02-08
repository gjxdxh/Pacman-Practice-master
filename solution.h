#ifndef SOLUTION_H
#define SOLUTION_H

/**
 * This class records the solution of a path finding.
 * It contains functions that used to draw the path.
 */

#include <vector>
#include <iostream>
#include <fstream>
#include <string>
using namespace std;
class Solution
{
  public:
    vector<int> path; // Path cosists of a series of directions
    int path_cost;    // The path cost of the solution, defined as the number of steps taken to get from the initial state to the goal state.
    int nodes;        // Number of nodes expanded by the search algorithm.
    Solution();
    Solution(Solution &other); 

    // Draw the solution path and write output to a file
    // x, y are the coordinates of the starting point
    void drawSolution(string file_name, vector<string> maze, int x,  int y);
};

#endif
