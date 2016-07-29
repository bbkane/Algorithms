//procedure UniformCostSearch(Graph, start, goal)
//  node ← start
//  cost ← 0
//  frontier ← priority queue containing node only
//  explored ← empty set
//  do
//      if frontier is empty
//          return failure
//      node ← frontier.pop()
//      if node is goal
//          return solution
//      explored.add(node)
//      for each of node's neighbors n
//          if n is not in explored
//              if n is not in frontier
//                  frontier.add(n)
//              else if n is in frontier with higher cost
//                  replace existing node with n

#include <iostream>
#include <vector>
#include <string>
#include <iostream>
#include <memory>
#include <set>
#include <algorithm>

struct Edge
{
    char first;
    char second;
    int cost_between;
    Edge* next{ nullptr };

    Edge(char first, char second, int cost)
        : first(first), second(second), cost_between(cost)
    {}

    friend std::ostream& operator<<(std::ostream &os, const Edge &c)
    {
        return os << c.first << " " << c.second << " " << c.cost_between;
    }
};

struct Node
{
    char id;
    Node *next{ nullptr };
    int cost;
    Node(char id)
        : id(id)
    {}
};

int main()
{
    std::vector<Edge> edges;

    auto add = [&edges](char first, char second, int cost) {
        edges.push_back(Edge(first, second, cost));
        edges.push_back(Edge(second, first, cost));
    };

    add('o', 'a', 2);
    add('o', 'b', 5);
    add('o', 'c', 4);
    add('a', 'b', 2);

    //procedure UniformCostSearch(Graph, start, goal)
    auto start = 'o';
    auto end = 'c';
    //  node ← start
    Node node(start);
    //  cost ← 0
    node.cost = 0;
    //  frontier ← priority queue containing node only
    std::vector<Node> frontier;
    frontier.push_back(node);
    //  explored ← empty set
    std::set<char> explored;
    // do
    while (true)
    {
        // if frontier is empty
        if (frontier.empty())
        {
            // return failure
            std::cout << "empty frontier" << std::endl;
            return 0;
        }
        // node ← frontier.pop()
        auto node_it = std::min_element(frontier.begin(), frontier.end(), [](Node& a, Node& b) {return a.cost < b.cost; });
        node = *node_it;
        frontier.erase(node_it);

        // if node is goal
        if (node.id == end)
        {
            // return solution
            std::cout << "goddamn it worked!\n";
            return 0;
        }
        // explored.add(node)
        explored.insert(node.id);
        // for each of node's neighbors n
        for (Edge& edge : edges)
        {
            if (edge.first == node.id)
            {
                Node next(edge.first);
                next.cost = node.cost + edge.cost_between;
                // if n is not in explored
                if (explored.find(next.id) == explored.end())
                {
                    // if n is not in frontier
                    std::vector<Node>::iterator node_f = 
                        std::find_if(frontier.begin(), frontier.end(), [&next](Node& n) { return n.id == next.id; });
                    if (node_f == frontier.end())
                    {
                        // frontier.add(n)
                        frontier.push_back(next);
                    }
                    // else if n is in frontier with higher cost
                    else if (node_f->cost > next.cost)
                    {
                        // replace existing node with n
                        frontier.erase(node_f);
                        frontier.push_back(next);
                    }

                }

            }

        }
    }
}