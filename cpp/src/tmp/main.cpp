#include <iostream>
#include "fmt-3.0.0\fmt\ostream.h";

using CostType = int;
using NodeType = char;

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
//template <typename Func> // Func (NodeType) -> iterable of Element(CostType cost, NodeType node).


int main()
{
    Element e('a', 1);

    fmt::print(std::cout, "Another print: {}\n", e);
}