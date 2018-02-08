#include "test.h"

void print_maze(vector<string> maze)
{
    for (size_t i = 0; i < maze.size(); i++)
    {
        cout << maze[i] << endl;
    }
}

void print_dir(vector<vector<int>> dir)
{
    for (size_t i = 0; i < dir.size(); i++)
    {
        for (size_t j = 0; j < dir[i].size(); j++)
        {
            cout << dir[i][j];
        }
        cout << endl;
    }
}