//This code seems to work, but is bad for two reasons:
//- it uses iterators to connections. Iterators get invalidated when their containers are modified.
//  This means I can't modify my containers after I start the traversal
//- it stores everything in one big vector, and uses an enum class (List) to determine list.
//  This means that all searches through the list search the whole thing
//Improvements for next time:
//- use a vector of Connections and and lists of DijkstraConnections (with pointers to the underlying connections)
//  so I can avoid that linear search. I still won't be able to modify the underlying list, but I can't think of a way around it.
// Error handling
// - final node not reachable

#include <fmt-3.0.0/fmt/format.h>
#include <vector>
#include <limits>
#include <iostream>

enum class List
{
    Unexplored,
    Frontier,
    Closed
};

struct Connection
{
    char first;
    char second;
    int cost_between;
    int total_cost{ 0 };
    List list{ List::Unexplored };
    std::vector<Connection>::iterator next_connection;

    Connection(char first, char second, int cost)
        : first(first), second(second), cost_between(cost)
    {}

    friend std::ostream& operator<<(std::ostream &os, const Connection &c)
    {
        os << c.first << " " << c.second << " " << c.cost_between;
        return os;
    }
};

int main()
{
    //NOTE: don't push or pull from this after init!
    // I'm setting iterators to it in my code!
    std::vector<Connection> connections{
        Connection('a', 'b', 1),
        Connection('a', 'c', 3),
        Connection('b', 'c', 1)
    };

    char start_id = 'a';
    char end_id = 'c';

    if (start_id == end_id)
    {
        std::cout << "Already at end!" << std::endl;
        return 0;
    }

    // Add all starting connections to Frontier and get the cheapest one
    auto cheapest = connections.end();
    int lowest_cost = std::numeric_limits<int>::max();
    for (auto it = connections.begin(); it != connections.end(); ++it)
    {
        if (it->first == start_id)
        {
            it->total_cost = 0 + it->cost_between;
            it->list = List::Frontier;
            if (it->total_cost < lowest_cost)
            {
                cheapest = it;
                lowest_cost = it->total_cost;
            }
        }
    }

    //for printing later
    auto first = cheapest;

    while (true)
    {
        cheapest->list = List::Closed;

        // If we're done, print
        if (cheapest->second == end_id)
        {
            cheapest->next_connection = connections.end();
            for (auto it = first; it != connections.end(); it = it->next_connection)
            {
                std::cout << *it << std::endl;
            }
            return 0;
        }
        // Add all of cheapest's kids to frontier
        for (auto it = connections.begin(); it != connections.end(); ++it)
        {
            if (it->first == cheapest->second && it->list == List::Unexplored)
            {
                it->total_cost = cheapest->total_cost + it->cost_between;
                it->list = List::Frontier;
                //set the child relationship here for printing later
                cheapest->next_connection = it;
            }
        }

        // get the next cheapest
        lowest_cost = std::numeric_limits<int>::max();
        for (auto it = connections.begin(); it != connections.end(); ++it)
        {
            if (it->total_cost < lowest_cost && it->list == List::Frontier)
            {
                cheapest = it;
                lowest_cost = it->total_cost;
            }
        }
        
    }

}