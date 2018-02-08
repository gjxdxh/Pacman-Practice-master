#include <vector>
#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <string>
#include "solution.h"
#include "search.h"
#include "test.h"
using namespace std;

#define BFS_SEARCH 0
#define DFS_SEARCH 1
#define A_STAR_SEARCH 2
#define GREEDY_SEARCH 3

vector<string> maze;

//TODO: find a better way to output all four results into one .txt file
Solution do_search(vector<string> m, int x, int y, int tx, int ty, int search)
{
    Solution sol;
    if (search == BFS_SEARCH)
        sol = Search::BFS(m, x, y, tx, ty);
    else if (search == DFS_SEARCH)
        sol = Search::DFS(m, x, y, tx, ty);
    else if (search == A_STAR_SEARCH)
        sol = Search::A_star(m, x, y, tx, ty);
    else
        sol = Search::greedy(m, x, y, tx, ty);
    return sol;
}

int main()
{
    // Read input file
    ifstream inFile;
    inFile.open("maze.txt");
    string temp;
    int startx = 0;
    int starty = 0;
    int finalx = 0;
    int finaly = 0;
    while (inFile.good())
    {
        getline(inFile, temp);
        if (temp.compare("") == 0)
            break;
        maze.push_back(temp);
    }
    // Find start point and end point
    for (int i = 0; i < (int)maze.size(); i++)
    {
        // i - y coordinate
        for (int j = 0; j < (int)maze[i].length(); j++)
        {
            // j - x coordinate
            if (maze[i][j] == 'P')
            {
                startx = j;
                starty = i;
            }
            if (maze[i][j] == '.')
            {
                finalx = j;
                finaly = i;
            }
        }
    }
    Solution sol;
    int search_method;
    cout << "Please choose search method: " << endl;
    cout << "0.BFS 1.DFS 2.A* 3.Greedy" << endl;
    cin >> search_method;
    if (search_method > 3 || search_method < 0)
    {
        cout << "Invalid search method." << endl;
        exit(1);
    }
    sol = do_search(maze, startx, starty, finalx, finaly, search_method);
    sol.drawSolution("mp1_1_sol.txt", maze, startx, starty);

    ifstream inFile2;
    inFile2.open("maze2.txt");
    startx = 0;
    starty = 0;
    vector<string> maze2;
    vector<pair<int, int>> targets;
    while (inFile2.good())
    {
        getline(inFile2, temp);
        if (temp.compare("") == 0)
            break;
        maze2.push_back(temp);
    }
    for (int i = 0; i < (int)maze2.size(); i++)
    {
        // i - y coordinate
        for (int j = 0; j < (int)maze2[i].length(); j++)
        {
            // j - x coordinate
            if (maze2[i][j] == 'P')
            {
                startx = j;
                starty = i;
            }
            if (maze2[i][j] == '.')
            {
                pair<int, int> newNode(j, i);
                targets.push_back(newNode);
            }
        }
    }
    Solution sol2;
    sol2 = Search::super_Astar(maze2, startx, starty, targets);
    return 0;
}
