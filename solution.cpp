#include "solution.h"
#include "test.h"
#include <stdio.h>

#define RIGHT 0
#define DOWN 1
#define LEFT 2
#define UP 3

Solution::Solution()
{
    path_cost = 0;
    nodes = 0;
}

// Copy constructor
Solution::Solution(Solution &other)
{
    path = other.path;
    path_cost = other.path_cost;
    nodes = other.nodes;
}

void Solution::drawSolution(string file_name, vector<string> maze, int x, int y)
{
    //cout << "DRAW" << endl;
    ofstream myfile;
    myfile.open(file_name);
    if (!myfile.good())
    {
        cout << "ERROR OPENING FILE" << endl;
        exit(1);
    }
    for (size_t i = 0; i < path.size(); i++)
    {
        switch (path[i])
        {
            case RIGHT:
                maze[y][x + 1] = '.';
                x++;
                break;
            case DOWN:
                maze[y + 1][x] = '.';
                y++;
                break;
            case LEFT:
                maze[y][x - 1] = '.';
                x--;
                break;
            case UP:
                maze[y - 1][x] = '.';
                y--;
                break;
            default:
                fprintf(stderr, "tracing out of bound\n");
                exit(1);
        }
    }
    //cout << "DRAW3" << endl;
    for (size_t i = 0; i < maze.size(); i++)
    {
        myfile << maze[i];
    }
    // Print other informations
    myfile << endl;
    myfile << "Path cost: " << path_cost << endl;
    myfile << "Number of nodes expanded: " << nodes << endl;
    //cout << "break" << endl;
    myfile.close();
}
