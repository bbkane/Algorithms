#include <iostream>
#include <vector>
#include <memory>
#include <limits>
#include <algorithm>

// This doesn't work because I'm completely ignoring the closed list. Oops.

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

struct DEdge
{
    Edge* edge{ nullptr };
    int cost{ INT_MAX };
    DEdge(Edge* edge, int cost)
        : edge(edge), cost(cost)
    {}
};

int main()
{
    std::vector<std::unique_ptr<Edge>> edges;

    auto add = [&edges](char first, char second, int cost) {
        edges.push_back(std::make_unique<Edge>(first, second, cost));
        edges.push_back(std::make_unique<Edge>(second, first, cost));
    };

    add('o', 'a', 2);
    add('o', 'b', 5);
    add('o', 'c', 4);
    add('a', 'b', 2);

    auto start = 'o';
    auto end = 'c';
    
    std::vector<DEdge> frontier;

    if (start == end)
    {
        std::cout << "it worked\n";
        return 0;
    }

    // add the first edges
    for (auto& i : edges)
    {
        if (i->first == start)
        {
            frontier.push_back(DEdge(i.get(), 0 + i->cost_between));
        }
    }

    while (true)
    {
        // get cheapest from frontier
        std::vector<DEdge>::iterator cheapest = std::min_element(frontier.begin(), frontier.end(),
            [](DEdge& a, DEdge& b) { return a.cost < b.cost; });

        if (cheapest == frontier.end())
        {
            std::cout << "nothing left" << std::endl;
            return 0;
        }

        if (cheapest->edge->second == end)
        {
            std::cout << "done!" << std::endl;
            return 0;
        }

        for (auto& i : edges)
        {
            if (i->first == cheapest->edge->second)
            {
                frontier.push_back(DEdge(i.get(), cheapest->cost + i->cost_between));
                cheapest->edge->next = i.get();
            }
        }

        frontier.erase(cheapest);
    }
}