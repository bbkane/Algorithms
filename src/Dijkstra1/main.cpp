#include <fmt-3.0.0/fmt/format.h>
#include <vector>
#include <limits>
#include <array>
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
        fmt::print("Already there!\n");
        return 0;
    }

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

    auto first = cheapest;

    while (true)
    {
        cheapest->list = List::Closed;

        if (cheapest->second == end_id)
        {
            cheapest->next_connection = connections.end();
            for (auto it = first; it != connections.end(); it = it->next_connection)
            {
                std::cout << *it << std::endl;
            }
            return 0;
        }

        auto new_cheapest = connections.end();
        lowest_cost = std::numeric_limits<int>::max();
        for (auto it = connections.begin(); it != connections.end(); ++it)
        {
            if (it->first == cheapest->second && it->list == List::Unexplored)
            {
                it->total_cost = cheapest->total_cost + it->cost_between;
                it->list = List::Frontier;
                cheapest->next_connection = it;
                if (it->total_cost < lowest_cost)
                {
                    new_cheapest = it;
                    lowest_cost = it->total_cost;
                }
            }
        }

        cheapest = new_cheapest;
    }

}