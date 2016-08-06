#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <set>
#include <map>
#include <algorithm>

using CostType = int;
using NodeType = char;

// Why do I have to do this? Truly a degenerate language!
template <typename T>
std::string make_string(T value)
{
    std::stringstream ss;
    ss << value;
    return ss.str();
}


struct Element
{
    NodeType node;
    CostType cost;

    Element(NodeType node, CostType cost)
        : node(node), cost(cost)
    {}

    std::string to_string() const
    {
        return make_string(node) + " " + make_string(cost);
    }
};

struct Edge
{
    NodeType a;
    NodeType b;
    CostType cost;

    Edge(NodeType a, NodeType b, CostType cost)
        : a(a), b(b), cost(cost)
    {}

    std::string to_string() const
    {
        return make_string(a) + " " + make_string(b) + " " + make_string(cost);
    }
};

struct Graph
{
    std::vector<Edge> edges_;

    void add_two_way(NodeType a, NodeType b, CostType cost)
    {
        edges_.push_back(Edge(a, b, cost));
        edges_.push_back(Edge(b, a, cost));
    }

    std::vector<Element> get_neighbors(NodeType a)
    {
        std::vector<Element> neighbors;
        for (auto e : edges_)
        {
            if (e.a == a)
            {
                neighbors.push_back(Element(e.b, e.cost));
            }
        }
        return neighbors;
    }
    std::string to_string() const
    {
        std::string str;
        for (const auto& e : edges_)
        {
            str += e.to_string() + "\n";
        }
        return str;
    }
};

// SuccessFunc (NodeType) -> bool
// GetNeighborsFunc (NodeType) -> IterableType<Element(CostType cost, NodeType node)>
template <typename SuccessFunc, typename GetNeighborsFunc>
std::vector<NodeType> dijkstra(NodeType start, SuccessFunc success, GetNeighborsFunc get_neighbors)
{
    std::set<NodeType> explored;
    std::vector<Element> frontier;
    std::map<NodeType, NodeType> parents;

    frontier.push_back(Element(start, 0));

    while (true)
    {
        if (frontier.empty())
        {
            //an empty vector is failure here
            return std::vector<NodeType>();
        }

        // We need the cheapest thing at the back because vector has
        // a convenient pop_back method
        std::sort(std::begin(frontier), std::end(frontier),
            [](const Element& e1, const Element& e2) { return e1.cost > e2.cost; });

        Element current_node = frontier.back();
        frontier.pop_back();
        //std::cout << current_node.node << " " << current_node.cost << std::endl;
        if (success(current_node.node))
        {
            std::vector<NodeType> path;
            NodeType current = current_node.node;
            path.push_back(current);
            auto parent_it = parents.find(current);
            while (parent_it != parents.end())
            {
                current = parent_it->second;
                path.push_back(current);
                parent_it = parents.find(current);
            }
            std::reverse(std::begin(path), std::end(path));
            return path;
        }

        explored.insert(current_node.node);

        auto neighbors = get_neighbors(current_node.node);
        for (auto neighbor : neighbors)
        {
            //std::cout << "neighbor: " << neighbor.node << " " << neighbor.cost << std::endl;
            auto total_neighbor_cost = current_node.cost + neighbor.cost;

            if (explored.count(neighbor.node) == 0)
            {
                bool in_frontier = false;
                for (auto &e : frontier)
                {
                    if (e.node == neighbor.node)
                    {
                        in_frontier = true;
                        if (e.cost > total_neighbor_cost)
                        {
                            e.cost = total_neighbor_cost;
                            parents[e.node] = current_node.node;
                            break;
                        }
                    }
                }
                if (!in_frontier)
                {
                    frontier.push_back(Element(neighbor.node, total_neighbor_cost));
                    parents[neighbor.node] = current_node.node;
                }
            }
        }
    }
}

int main()
{
    auto g = Graph();

    g.add_two_way('1', '2', 7);
    g.add_two_way('1', '3', 9);
    g.add_two_way('1', '6', 14);
    g.add_two_way('2', '3', 10);
    g.add_two_way('2', '4', 15);
    g.add_two_way('3', '4', 11);
    g.add_two_way('3', '6', 2);
    g.add_two_way('4', '5', 6);
    g.add_two_way('5', '6', 9);

    std::vector<NodeType> path;

    path = dijkstra('1', [](auto node) {return node == '6'; },
                    [&g](auto node) { return g.get_neighbors(node); });

    for (auto n : path)
    {
        std::cout << make_string(n) << " ";
    }
    std::cout << std::endl;
}
