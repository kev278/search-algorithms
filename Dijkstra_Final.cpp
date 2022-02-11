#include<bits/stdc++.h>

using namespace std;

//Class that holds the node members
class Node
{
    public:
    int row;
    int col;
    //Here the dist is 1 since it is not a weighted grid
    //If we have unequal weights, we can add those while making a new node
    //For this assignment, 1 is assumed since it was mentioned in the Readme file
    int dist{1};
    int cost;
    Node* parent;
};

//Class is created for exploration, and to have the code in a structured way
class Explore
{
    public:
    //This function simply checks for boundaries in the grid
    bool checkBounds(int row, int col, int size_row, int size_col)
    {
        if(row >= size_row || col >= size_col || row < 0 || col < 0)
        {
            return false;
        }
        return true;
    }
    Node* sort_node(queue<Node*> &node)
    {
        queue<Node*> copy_node;
        queue<Node*> temp;
        //Vector to store costs
        vector<int> costs;
        Node* parent = nullptr;
        copy_node = node;
        int size{0};
        size = copy_node.size();
        for(int i = 0; i < size; i++)
        {
            costs.push_back(copy_node.front()->cost);
            copy_node.pop();
        }
        //Fnd minimum cost member in the queue
        int min_index = min_element(costs.begin(), costs.end()) - costs.begin();
        for(int i = 0; i < min_index; i++)
        {
            temp.push(node.front());
            node.pop();
        }

        parent = node.front();
        node.pop();

        while(!node.empty())
        {
            temp.push(node.front());
            node.pop();
        }

        while(!temp.empty())
        {
            node.push(temp.front());
            temp.pop();
        }        
        return parent;
    }

    //The role of this function is to find the cell to be explored
    //based on the costs of the cells
    pair<pair<int, int>, int> Sort(queue<pair<pair<int, int>, int>> &explore)
    {
        queue<pair<pair<int, int>, int>> copy_ex;
        queue<pair<pair<int, int>, int>> temp;
        //Vector to store costs
        vector<int> costs;
        copy_ex = explore;
        int size{0};
        size = copy_ex.size();
        for(int i = 0; i < size; i++)
        {
            costs.push_back(copy_ex.front().second);
            copy_ex.pop();
        }
        //Fnd minimum cost member in the queue
        int min_index = min_element(costs.begin(), costs.end()) - costs.begin();
        for(int i = 0; i < min_index; i++)
        {
            temp.push(explore.front());
            explore.pop();
        }

        pair<pair<int, int>, int> coords_pair = explore.front();
        explore.pop();

        while(!temp.empty())
        {
            explore.push(temp.front());
            temp.pop();
        }        
        return coords_pair;
    }   

    //This is the function that has the algorithm implemented
    pair<Node*, vector<vector<bool>>> Dijkstra(vector<vector<int>> grid, int* start, int* goal)
    {
        //Grid size is extracted
        int size_row = grid.size();
        int size_col = grid[0].size();
        int cost{1};
        //A queue to add the elements to be explored
        queue<pair<pair<int, int>, int>> explore;
        //A queue to add nodes to be explored. This is needed to keep a track of nodes since 
        //the algorithm is implemented using a grid and not a graph
        queue<Node*> node;
        //Order of exploration
        vector<pair<int, int>> directions {{0, 1}, {1, 0}, {0, -1}, {-1, 0}}; 
        //Flag for checking boundaries
        bool isValid{false};
        pair<Node*, vector<vector<bool>>> output;
        //To maintain the list of explored/visited nodes
        vector<vector<bool>> visited(size_row, vector<bool> (size_col));
        Node* parent = nullptr;
        Node* current = nullptr;
        parent = new Node();
        pair<int, int> current_coords;
        pair<int, int> goal_coords {goal[0], goal[1]};
        
        //Loops are used in case they are required for other inputs where there
        //can be multiple start points. They are not required in this case but loops are to 
        //generalize the code
        for(int i = start[0]; i < size_row; i++)
        {
            for(int j = start[1]; j < size_col; j++)
            {
                current_coords = {i, j};
                //Check for obsracles and if the cell is already visited or not
                if(grid[i][j] == 0 && !visited[i][j])
                {

                    explore.push({{i, j}, parent->cost});
                    node.push(parent);
                    visited[i][j] = true;
                    //Run the iterative steps till goal is reached or the grid is explored
                    //Here, it means that the queue has to be emptied
                    while(!explore.empty())
                    {
                        //Get element with minimum cost
                        pair<pair<int, int>, int> coords_pair = Sort(explore);
                        pair<int, int> coords = coords_pair.first;
                        parent = sort_node(node);
                        for(pair<int, int> dir : directions)
                        {
                            //Get row and col
                            int row = coords.first + dir.first;
                            int col = coords.second + dir.second;
                            //Check if goal is reached
                            if(row == goal[0] && col == goal[1])
                            {
                                //Form the goal node as we need to count it
                                current = new Node();
                                current->row = row;
                                current->col = col; 
                                current->parent = parent;
                                //Calculate totoal cost
                                current->cost = current->parent->cost + current->dist;
                                visited[row][col] = true;
                                goto here;
                            }     
                            //Check for bounds                      
                            isValid = checkBounds(row, col, size_row, size_col);
                            //Check for obstacles and if the cell is visited or not
                            if(isValid && !visited[row][col] && grid[row][col] == 0)
                            {
                                //For a new node
                                current = new Node();
                                current->parent = parent;
                                current->row = row;
                                current->col = col;
                                current->cost = current->parent->cost + current->dist;
                                //Add in the queue
                                explore.push({{row, col}, current->cost});
                                node.push(current);
                                //Mark as visited
                                visited[row][col] = true;
                                isValid = false;  
                            }   
                        }    
                    }
                }
            }
        }
        //Goal not found
        return output;

        here:
        //Goal node is current node
        
        output.first = current;
        output.second = visited;
        return output;
    }
};

int main()
{
    //Add the grid here
    vector<vector<int>> grid 
/*  
                        {{0,0,0,1},
                        {0,0,0,1},
                        {0,1,0,0},
                        {0,0,0,0}};
 */                       

                       {{0,0,0,0,0,0,0,0,1,0},
                        {0,1,1,1,1,0,0,0,1,0},
                        {0,0,0,0,0,0,0,0,1,0},
                        {0,0,0,1,1,1,1,1,1,0},
                        {0,1,0,1,0,0,0,0,0,0},
                        {0,1,0,1,0,1,1,1,1,0},
                        {0,1,0,0,0,1,1,1,1,0},
                        {0,1,1,1,0,0,0,1,1,0},
                        {0,0,0,0,0,0,0,1,0,0},
                        {0,0,0,0,0,0,0,0,0,0}};


    int start[2] {0, 0};
    int goal[2] {6, 9};
    //steps is a variable that tells us the number of cells that were traversed
    //to reach the goal. This is different from the python code presented
    int steps{1};
    //Path contains a list of all the cells that are traversed to reach the goal
    vector<vector<int>> path;
    Node* output = nullptr;
    vector<vector<bool>> visited;
    //This variable is made since we cannot return more than 1 value at a time
    //A pair was made to return the path and the nodes visited 
    pair<Node*, vector<vector<bool>>> output_pair;
    Explore Ex;
    int size_row = grid.size();
    int size_col = grid[0].size();
    //Check if goal is in the grid
    if(goal[0] >= size_row || goal[1] >= size_col)
    {
        cout << "Invalid";
    }
    //Check if goal is obstacle
    else if(grid[goal[0]][goal[1]] == 1)
    {
        cout << "Goal is obtacle";
    }
    else
    {
        output_pair = Ex.Dijkstra(grid, start, goal);
        output = output_pair.first;
        visited = output_pair.second;
        //Check if output is empty
        if(output == 0)
        {
            cout << "Goal not found";
        }
        else
        {
            //Loop to extract the path from the list of nodes that was formed
            while(output->parent != 0)
            {
                int row = output->row;
                int col = output->col;
                vector<int> coord {row, col};
                path.push_back(coord);
                steps++;
                output = output->parent;
            }
            //Add the start coords as in the path
            int row = start[0];
            int col = start[1];
            vector<int> coord {row, col};
            path.push_back(coord);
            //Reverse the path since we used push_back
            reverse(path.begin(), path.end());
            //nodes_visited is the 'steps' in the python code
            int nodes_visited{0};
            //Extract the number of nodes visited from the list
            for(int i = 0; i < visited.size(); i++)
            {
                for(int j = 0; j < visited[0].size(); j++)
                {
                    if(visited[i][j] == 1)
                    {
                        nodes_visited++;
                    }
                }
            }
            //Add a breakpoint here and debug to get the output
            cout << "Done";
        }
        
    }
}
