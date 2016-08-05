#include <iostream>
#include <vector>
#include <list>

#include "fmt-3.0.0\fmt\ostream.h";

struct Element;

using CostType = int;
using NodeType = char;
//using IterableType = std::vector<Element>;
using IterableType = std::list<Element>;

struct Element
{
    NodeType node;
    CostType cost;

    Element(NodeType node, CostType cost)
        : node(node), cost(cost)
    {}
};

std::ostream& operator<<(std::ostream& os, const Element& e)
{
    return os << "Element(node=" << e.node << ", cost=" << e.cost << ")";
}

template <typename Func> // Func (NodeType) -> IterableType<Element(CostType cost, NodeType node)>.
void dijkstra_shim(Func f)
{
    NodeType a = 'a';
    auto iterable = f(a);
    for (auto& n : iterable)
    {
        fmt::print(std::cout, "{}\n", n);
    }
}

IterableType f_shim(NodeType n)
{
    return IterableType{ Element(n, 1), Element(n, 2) };
}

int main()
{
    dijkstra_shim(f_shim);
}