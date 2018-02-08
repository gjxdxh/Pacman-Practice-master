#include "search.h"
#include "solution.h"
#include "test.h"

#define UNVISITED false
#define VISITED true
#define RIGHT 0
#define DOWN 1
#define LEFT 2
#define UP 3

vector<vector<int>> dfsDir;
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
    else if (dir == DOWN)
    {
        if (y + 1 < hei && visited[x + wid * (y + 1)] == false && maze[y + 1][x] != '%')
            return true;
    }
    else if (dir == LEFT)
    {
        if (x - 1 >= 0 && visited[x - 1 + wid * y] == false && maze[y][x - 1] != '%')
            return true;
    }
    else
    {
        if (y - 1 >= 0 && visited[x + wid * (y - 1)] == false && maze[y - 1][x] != '%')
            return true;
    }
    return false;
}

// A helper fuction of DFS
void DFS_recursive(vector<string> maze, int x, int y, int tx, int ty)
{
    int wid = maze[0].length();
    visited[x + y * wid] = true;
    if (canTravel(maze, x, y, RIGHT))
    {
        nodes_expand++;
        dfsDir[y][x + 1] = RIGHT;
        DFS_recursive(maze, x + 1, y, tx, ty);
    }
    if (canTravel(maze, x, y, DOWN))
    {
        nodes_expand++;
        dfsDir[y + 1][x] = DOWN;
        DFS_recursive(maze, x, y + 1, tx, ty);
    }
    if (canTravel(maze, x, y, LEFT))
    {
        nodes_expand++;
        dfsDir[y][x - 1] = LEFT;
        DFS_recursive(maze, x - 1, y, tx, ty);
    }
    if (canTravel(maze, x, y, UP))
    {
        nodes_expand++;
        dfsDir[y - 1][x] = UP;
        DFS_recursive(maze, x, y - 1, tx, ty);
    }
}

Solution Search::DFS(vector<string> maze, int x, int y, int tx, int ty)
{
    nodes_expand = 0;
    int wid = maze[0].length();
    int hei = maze.size();
    visit_init(visited, wid * hei);
    for (int i = 0; i < hei; i++)
    {
        vector<int> temp;
        dfsDir.push_back(temp);
        for (int j = 0; j < wid; j++)
        {
            dfsDir[i].push_back(4);
        }
    }
    nodes_expand++;
    Solution sol;
    DFS_recursive(maze, x, y, tx, ty);
    sol.nodes = nodes_expand;
    int endX = tx, endY = ty;
    while (!(endX == x && endY == y))
    {
        sol.path.insert(sol.path.begin(), dfsDir[endY][endX]);
        sol.path_cost++;
        maze[endY][endX] = '.';
        switch (dfsDir[endY][endX])
        {
        case RIGHT:
            endX--;
            break;
        case DOWN:
            endY--;
            break;
        case LEFT:
            endX++;
            break;
        case UP:
            endY++;
            break;
        default:
            fprintf(stderr, "ERROR: out of bound\n");
            exit(1);
        }
    }
    return sol;
}

Solution Search::BFS(vector<string> maze, int x, int y, int tx, int ty)
{
    int wid = maze[0].length();
    int hei = maze.size();
    visit_init(visited, wid * hei);
    vector<vector<int>> dir; // The moving direction of the Pacman
    // Initialize dir
    for (int i = 0; i < hei; i++)
    {
        vector<int> temp;
        dir.push_back(temp);
        for (int j = 0; j < wid; j++)
        {
            dir[i].push_back(4);
        }
    }
    // Construct solution
    Solution sol;
    // Represent each point as a pair
    queue<pair<int, int>> q;
    pair<int, int> start(x, y);
    visited[x + wid * y] = true;
    q.push(start);
    sol.nodes++;
    int endX = tx;
    int endY = ty;
    while (!q.empty())
    {
        pair<int, int> curr = q.front();
        q.pop();
        int currX = curr.first;
        int currY = curr.second;
        if (currX == tx && currY == ty)
        {
            break;
        }
        if (canTravel(maze, currX, currY, RIGHT))
        {
            dir[currY][currX + 1] = RIGHT;
            visited[currX + 1 + wid * currY] = true;
            pair<int, int> next(currX + 1, currY);
            q.push(next);
            sol.nodes++;
        }
        if (canTravel(maze, currX, currY, DOWN))
        {
            dir[currY + 1][currX] = DOWN;
            visited[currX + wid * (currY + 1)] = true;
            pair<int, int> next(currX, currY + 1);
            q.push(next);
            sol.nodes++;
        }
        if (canTravel(maze, currX, currY, LEFT))
        {
            dir[currY][currX - 1] = LEFT;
            visited[currX - 1 + wid * currY] = true;
            pair<int, int> next(currX - 1, currY);
            q.push(next);
            sol.nodes++;
        }
        if (canTravel(maze, currX, currY, UP))
        {
            dir[currY - 1][currX] = UP;
            visited[currX + wid * (currY - 1)] = true;
            pair<int, int> next(currX, currY - 1);
            q.push(next);
            sol.nodes++;
        }
    }
    while (!(endX == x && endY == y))
    {
        sol.path.insert(sol.path.begin(), dir[endY][endX]);
        sol.path_cost++;
        maze[endY][endX] = '.';
        switch (dir[endY][endX])
        {
        case RIGHT:
            endX--;
            break;
        case DOWN:
            endY--;
            break;
        case LEFT:
            endX++;
            break;
        case UP:
            endY++;
            break;
        default:
            fprintf(stderr, "ERROR: out of bound\n");
            exit(1);
        }
    }
    return sol;
}

// Heuristic function for A* search
float heuristic(float x, float y, float goalx, float goaly)
{
    return (float)(abs(x - goalx) + abs(y - goaly));
}

Solution Search::A_star(vector<string> maze, int x, int y, int finalx, int finaly)
{
    Solution sol;
    int wid = maze[0].length();
    int hei = maze.size();
    visit_init(visited, wid * hei);
    int finalposition = finalx + finaly * wid;
    std::priority_queue<node> pq;
    vector<float> costs; // cost to go to every node
    costs.resize(wid * (hei + 1));
    for (size_t i = 0; i < costs.size(); i++)
    {
        costs[i] = std::numeric_limits<float>::infinity();
    }
    costs[x + y * wid] = 0; // initialize start state
    pq.push(node(x + y * wid, 0));
    vector<vector<int>> dir; // The moving direction of the Pacman
    // Initialize dir
    for (int i = 0; i < hei; i++)
    {
        vector<int> temp;
        dir.push_back(temp);
        for (int j = 0; j < wid; j++)
        {
            dir[i].push_back(4);
        }
    }
    while (!pq.empty())
    {
        node curr = pq.top();
        pq.pop();
        sol.nodes++;
        if (curr.position == finalposition)
            break;
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
                        costs[tempx + tempy * wid] = newcost;
                        float predict = newcost + heuristic(tempx, tempy, finalx, finaly);
                        pq.push(node(tempx + tempy * wid, predict));
                        dir[tempy][tempx] = i;
                    }
                }
            }
        }
    }
    int endX = finalx;
    int endY = finaly;
    while (!(endX == x && endY == y))
    {
        sol.path.insert(sol.path.begin(), dir[endY][endX]);
        sol.path_cost++;
        switch (dir[endY][endX])
        {
        case RIGHT:
            endX--;
            break;
        case DOWN:
            endY--;
            break;
        case LEFT:
            endX++;
            break;
        case UP:
            endY++;
            break;
        default:
            fprintf(stderr, "ERROR: out of bound\n");
            exit(1);
        }
    }
    return sol;
}

Solution Search::greedy(vector<string> maze, int x, int y, int finalx, int finaly)
{
    Solution sol;
    visit_init(visited, maze.size() * maze[0].length());
    int wid = maze[0].length();
    int hei = maze.size();
    int finalposition = finalx + finaly * wid;
    std::priority_queue<node> pq;
    pq.push(node(x + y * wid, heuristic(x, y, finalx, finaly)));
    vector<vector<int>> dir; // The moving direction of the Pacman
    // Initialize dir
    for (int i = 0; i < hei; i++)
    {
        vector<int> v;
        dir.push_back(v);
        for (int j = 0; j < wid; j++)
        {
            dir[i].push_back(4);
        }
    }
    while (!pq.empty())
    {
        node curr = pq.top();
        pq.pop();
        nodes_expand++;
        //  sol.nodes++;
        if (curr.position == finalposition)
        {
            cout << "found" << endl;
            break;
        }
        else
        {
            int currx = curr.position % wid;
            int curry = curr.position / wid;
            if (!visited[currx + curry * wid])
            {
                visited[currx + curry * wid] = VISITED;
                for (int i = 0; i < 4; i++)
                {
                    int tempx = currx;
                    int tempy = curry;
                    if (canTravel(maze, currx, curry, i))
                    {
                        if (i == RIGHT)
                        {
                            tempx++;
                            if (!visited[tempx + tempy * wid])
                            {
                                pq.push(node(tempx + tempy * wid, heuristic(tempx, tempy, finalx, finaly)));
                                dir[tempy][tempx] = i;
                            }
                        }
                        if (i == DOWN)
                        {
                            tempy++;
                            if (!visited[tempx + tempy * wid])
                            {
                                pq.push(node(tempx + tempy * wid, heuristic(tempx, tempy, finalx, finaly)));
                                dir[tempy][tempx] = i;
                            }
                        }
                        if (i == LEFT)
                        {
                            tempx--;
                            if (!visited[tempx + tempy * wid])
                            {
                                pq.push(node(tempx + tempy * wid, heuristic(tempx, tempy, finalx, finaly)));
                                dir[tempy][tempx] = i;
                            }
                        }
                        if (i == UP)
                        {
                            tempy--;
                            if (!visited[tempx + tempy * wid])
                            {
                                pq.push(node(tempx + tempy * wid, heuristic(tempx, tempy, finalx, finaly)));
                                dir[tempy][tempx] = i;
                            }
                        }
                    } //end of if canTravel
                }
            } //end of if not-visited
        }
    }
    int endX = finalx;
    int endY = finaly;
    while (!(endX == x && endY == y))
    {
        sol.path.insert(sol.path.begin(), dir[endY][endX]);
        sol.path_cost++;
        maze[endY][endX] = '.';
        switch (dir[endY][endX])
        {
        case RIGHT:
            endX--;
            break;
        case DOWN:
            endY--;
            break;
        case LEFT:
            endX++;
            break;
        case UP:
            endY++;
            break;
        default:
            fprintf(stderr, "ERROR: out of bound\n");
            exit(1);
        }
    }
    sol.nodes = nodes_expand;
    return sol;
} //end of greedy, cost is manhattan distance in greedy

// pushing function for vector-based pqueue
void pq_push(vector<node> &q, node n)
{
    for (size_t i = 0; i < q.size(); i++)
    {
        if (q[i].position == n.position)
        {
            q[i].cost = n.cost;
            return;
        }  
    }
    q.push_back(n);
}

// top() function for vector-based pqueue
node pq_top(vector<node> q)
{
    float min = FLT_MAX;
    int mini = 0;
    for (size_t i = 0; i < q.size(); i++)
    {
        if (min > q[i].cost)
        {
            min = q[i].cost;
            mini = i;
        }
    }
    return q[mini];
}

// pop function for vector-based pqueue
void pq_pop(vector<node> &q)
{
    float min = FLT_MAX;
    int mini = 0;
    for (size_t i = 0; i < q.size(); i++)
    {
        if (min > q[i].cost)
        {
            min = q[i].cost;
            mini = i;
        }
    }
    q.erase(q.begin() + mini);
}

/**
 * This is the A* search for 1.2.
 */
Solution Search::super_Astar(vector<string> maze, int x, int y, vector<pair<int, int>> targets)
{
    vector<string> newmaze = maze;
    int lastX = x, lastY = y;
    Solution sol;
    int wid = maze[0].length();
    int hei = maze.size();
    visit_init(visited, wid * hei);
    unordered_map<int, int> dots;
    for (size_t i = 0; i < targets.size(); i++)
    {
        int _position = targets[i].first + targets[i].second * wid;
        dots[_position] = (int)i;
    }
    //std::priority_queue<node> pq;
    vector<node> pq;
    vector<float> costs; // cost to go to every node
    costs.resize(wid * (hei + 1));
    for (size_t i = 0; i < costs.size(); i++)
    {
        costs[i] = std::numeric_limits<float>::infinity();
    }
    costs[x + y * wid] = 0; // initialize start state
    //pq.push(node(x + y * wid, 0));
    pq_push(pq, node(x + y * wid, 0));
    vector<vector<int>> dir; // The moving direction of the Pacman
    // Initialize dir
    for (int i = 0; i < hei; i++)
    {
        vector<int> temp;
        dir.push_back(temp);
        for (int j = 0; j < wid; j++)
        {
            dir[i].push_back(4);
        }
    }
    int endX = -1, endY = -1;
    vector<vector<int>> _path;
    int count = 0;
    while (!pq.empty())
    {
        node curr = pq_top(pq);
        int currx = curr.position % wid;
        int curry = curr.position / wid;
        pq_pop(pq);
        pair<int, int> currNode(currx, curry);
        int currNode_position = currx + curry * wid;
        if (dots.find(currNode_position) != dots.end())
        {
            if (targets.size() == 1)
                sol.path_cost = costs[currx + curry * wid];
            targets.erase(targets.begin() + dots[currNode_position]);
            dots.erase(currNode_position);
            for (size_t i = 0; i < costs.size(); i++)
            {
                if ((int)i != currx + curry * wid)
                    costs[i] = std::numeric_limits<float>::infinity();
            }
            //costs[currx + curry * wid] = 0; // initialize start state
            while (!pq.empty())
                pq_pop(pq);
            pq_push(pq, node(currx + curry * wid, costs[currx + curry * wid]));
            endX = currx;
            endY = curry;
            if (count <= 8)
                newmaze[curry][currx] = '1' + count;
            else
                newmaze[curry][currx] = 'a' + (count - 9);
            vector<int> temp_vec;
            _path.push_back(temp_vec);
            //print_dir(dir);
            // while (!(endX == lastX && endY == lastY))
            // {

            //     //TODO: path output
            //     //sol.path.insert(sol.path.begin(), dir[endY][endX]);
            //     _path[count].insert(_path[count].begin(), dir[endY][endX]);
            //     sol.path_cost++;
            //     switch (dir[endY][endX])
            //     {
            //     case RIGHT:
            //         endX--;
            //         break;
            //     case DOWN:
            //         endY--;
            //         break;
            //     case LEFT:
            //         endX++;
            //         break;
            //     case UP:
            //         endY++;
            //         break;
            //     default:
            //         fprintf(stderr, "ERROR: out of bound\n");
            //         exit(1);
            //     }
            // }
            count++;
            lastX = currx;
            lastY = curry;
            for (size_t k = 0; k < dir.size(); k++)
            {
                for (size_t l = 0; l < dir[k].size(); l++)
                    dir[k][l] = 4;
            }
        }
        if (targets.size() == 0)
            break;
        else
        {
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
                    // if (newcost < costs[tempx + tempy * wid])
                    // {
                    costs[tempx + tempy * wid] = newcost;
                    float h = 0;
                    // Use the min(dist(currloc, eachDot)) as heuristic function
                    int min = INT_MAX;
                    for (size_t i = 0; i < targets.size(); i++)
                    {
                        h = heuristic(tempx, tempy, (float)targets[i].first, (float)targets[i].second);
                        if (h < min)
                            min = h;
                    }
                    float predict = newcost + min;
                    pq_push(pq, node(tempx + tempy * wid, predict));
                    dir[tempy][tempx] = i;
                    sol.nodes++;
                    //}
                }
            }
        }
    }
    for(size_t i = 0; i < newmaze.size(); i++)
    {
        cout << newmaze[i] << endl;
    }
    cout << endl;
    cout << sol.path_cost << endl;
    cout << sol.nodes << endl;
    return sol;
}