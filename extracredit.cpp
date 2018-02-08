#include <vector>
#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <queue>
#include <limits>
#include <cmath>
#include <vector>
#include <string>
#include <utility>
#include <stdio.h>
using namespace std;

vector<string> maze;

#define UNVISITED false
#define VISITED true
#define RIGHT 0
#define DOWN 1
#define LEFT 2
#define UP 3

// Used in A* search
class node
{
  public:
    int position;
    float cost;
    node(int i, float c) : position(i), cost(c) {}
};
bool operator<(const node n1, const node n2)
{
    return n1.cost > n2.cost;
}

vector<bool> visited; // Keep track of visiting status
int nodes_expand = 0; // Keep track of total numbers of nodes expanded
/**
 * A helper function that reset the visited vector
 */
void visit_init(vector<bool> &visit, int n)
{
    visit.clear();
    visit.resize(n, UNVISITED);
    nodes_expand = 0;
}

/**
 * A helper function that tells whether the pacman can travel to a certain direction
 */
bool canTravel(vector<string> maze, int x, int y, int dir)
{
    int wid = maze[0].length();
    int hei = maze.size();
    if (dir == RIGHT)
    {
        if (x + 1 < wid && visited[x + 1 + wid * y] == false && maze[y][x + 1] != '%')
            return true;
    }
    else if (dir == UP)
    {
        if (y - 1 > 0 && visited[x + wid * (y - 1)] == false && maze[y - 1][x] != '%')
            return true;
    }
    else if (dir == LEFT)
    {
        if (x - 1 >= 0 && visited[x - 1 + wid * y] == false && maze[y][x - 1] != '%')
            return true;
    }
    else
    {
        if (y + 1 < hei && visited[x + wid * (y + 1)] == false && maze[y + 1][x] != '%')
            return true;
    }
    return false;
}

float heuristic(float x, float y, float goalx, float goaly)
{
    return (float)(abs(x - goalx) + abs(y - goaly));
}

int A_star(vector<string> maze, int x, int y, int finalx, int finaly)
{
    int ret = 0;
    int wid = maze[0].length();
    int hei = maze.size();
    int finalposition = finalx + finaly * wid;
    std::priority_queue<node> pq;
    vector<float> costs; // cost to go to every node
    costs.resize(wid * (hei + 1));
    for (int i = 0; i < costs.size(); i++)
    {
        costs[i] = std::numeric_limits<float>::infinity();
    }
    costs[x + y * wid] = 0; // initialize start state
    pq.push(node(x + y * wid, 0));

    while (!pq.empty())
    {
        node curr = pq.top();
        pq.pop();
        nodes_expand++;
        if (curr.position == finalposition)
        {
            break;
        }
        else
        {
            int currx = curr.position % wid;
            int curry = curr.position / wid;
            for (int i = 0; i < 4; i++)
            {
                int tempx = currx;
                int tempy = curry;
                if (canTravel(maze, currx, curry, i))
                {
                    if (i == RIGHT)
                        tempx++;
                    if (i == DOWN)
                        tempy++;
                    if (i == LEFT)
                        tempx--;
                    if (i == UP)
                        tempy--;
                    float newcost = costs[curr.position] + 1;
                    if (newcost < costs[tempx + tempy * wid])
                    {
                        ret++;
                        costs[tempx + tempy * wid] = newcost;
                        float predict = newcost + heuristic(tempx, tempy, finalx, finaly);
                        pq.push(node(tempx + tempy * wid, predict));
                    }
                }
            }
        }
    }// end of big while

    return ret;

}



int dumb(vector<string> &copy, int x, int y){
  nodes_expand++;

          if (canTravel(copy, x, y, RIGHT) && copy[y][x+1] == '.'){
              copy[y][x+1] = 'o';
              return dumb(copy, x+1, y)+1;
              }
          if (canTravel(copy, x, y, DOWN) && copy[y+1][x] == '.'){
              copy[y+1][x] = 'o';
              return dumb(copy, x, y+1)+1;
              }
          if (canTravel(copy, x, y, LEFT) && copy[y][x-1] == '.'){
              copy[y][x-1] = 'o';
              return dumb(copy, x-1, y)+1;
              }
          if (canTravel(copy, x, y, UP) && copy[y-1][x] == '.'){
              copy[y-1][x] = 'o';
              return dumb(copy, x, y-1)+1;
              }
              if (canTravel(copy, x, y, DOWN) && canTravel(copy, x, y+1, DOWN) && copy[y+2][x] == '.'){
                  copy[y+2][x] = 'o';
                  return dumb(copy, x, y+2)+2;
                  }
              if (canTravel(copy, x, y, UP) && canTravel(copy, x, y-1, UP) && copy[y-2][x] == '.'){
                  copy[y-2][x] = 'o';
                  return dumb(copy, x, y-2)+2;
                  }



    for (int i = 0; i < copy.size(); i++)
    {
        for (int j = 0; j < copy[i].length(); j++)
        {
          if (copy[i][j] == '.')
          {
            copy[i][j] = 'o';
            return dumb(copy, j, i) + A_star(copy, x, y, j, i);
          }
        }
    }

    return 0;
}


int dumbwrap(vector<string> maze, int x, int y){

  visit_init(visited, maze.size() * maze[0].length());

  vector<string>  mcopy;

  for (int i = 0; i < maze.size(); i++){
      mcopy.push_back(maze[i]);
  }
  int ret = dumb(mcopy, x, y);

  for (int i = 0; i < mcopy.size(); i++)
  cout << mcopy[i] << endl;


  return ret;
}





int main()
{
    // Read input file
    ifstream inFile;
    inFile.open("maze.txt");
    string temp;
    int startx = 0;
    int starty = 0;

    while (inFile.good())
    {
        getline(inFile, temp);
        if (temp.compare("") == 0)
            break;
        maze.push_back(temp);
    }
    // Find start point and end point
    for (int i = 0; i < maze.size(); i++)
    {
        // i - y coordinate
        for (int j = 0; j < maze[i].length(); j++)
        {
            // j - x coordinate
            if (maze[i][j] == 'P')
            {
                starty = i;
                startx = j;
            }
        }
    }
    for (int i = 0; i < maze.size(); i++)
    cout << maze[i] << endl;

    cout << endl;

    int total_walked = dumbwrap(maze, startx, starty);
    //sol.drawSolution("mp1_1_sol.txt", maze, startx, starty);

    cout<<"total path cost: "<<total_walked<<endl;
    cout<<"nodes expanded: "<<nodes_expand<<endl;
    return 0;
}
