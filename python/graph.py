# -*- coding: utf-8 -*-
"""
Created on Mon Jul 11 21:32:46 2016

@author: Ben
"""

from collections import namedtuple


class NoPathException(Exception):
    pass


# TODO: Can I combine this and the Edge class?
# NOTE: This is only used in dijkstra()
class MSPNode:
    """Minimum Spanning Tree Node"""

    # Can't use a namedtuple because it needs to be mutable
    def __init__(self, value, cost, parent=None):
        self.value = value
        self.cost = cost
        self.parent = parent

    def __repr__(self):
        return 'Element(value=%r, cost=%r)' % (self.value, self.cost)


def dijkstra(start_value, end_value, graph_iterator):
    """
    Args:
        start_value, end_value
        type value must implement __hash__, __eq__, __str__
        graph_iterator: GraphIterator

    Returns:
        [node]: list of the nodes from start to finish (shortest path)

    Raises:
        NoPathException: No path found
    """

    explored = set()
    frontier = []
    frontier.append(MSPNode(start_value, 0))
    while True:
        if not frontier:
            raise NoPathException(str(start_value))
        # treat the list like a priority queue by sorting it by cost
        frontier.sort(key=lambda e: e.cost)
        # pop the first element
        current_node = frontier.pop(0)
        # print(current_node.value, current_node.cost)
        if current_node.value == end_value:  # We're done. Get the path and return
            path = []
            while current_node.parent:
                path.append(current_node.value)
                current_node = current_node.parent
            path.append(current_node.value)
            path.reverse()
            return path
        explored.add(current_node.value)
        node_edges = graph_iterator.get_neighbors(current_node.value)
        for neighbor in node_edges:
            # print("neighbor:", neighbor.to_value, neighbor.cost)
            total_neighbor_cost = neighbor.cost + current_node.cost
            if neighbor.to_value not in explored:
                # Note: this only really works because I'm not getting the same neighbor
                # twice. Otherwise, I would have to re-sort the frontier
                for element in frontier:
                    if element.value == neighbor.to_value:
                        if element.cost > total_neighbor_cost:
                            element.cost = total_neighbor_cost
                            element.parent = current_node
                        break
                else:  # no break (the value wasn't in the frontier)
                    e = MSPNode(neighbor.to_value, total_neighbor_cost, parent=current_node)
                    frontier.append(e)


GraphNeighbor = namedtuple('Neigbor', ['value', 'cost'])


# Note: is this really necessary? I need a consistent interface for dijkstra,
# and I can imagine wanting state of its own, but this might be boilerplate
class GraphIterator:
    """Iterate over a graph

    This is the interface consumed by dijkstra.

    Methods:
        The end_value is for A* so I can call a heuristic if I want to
        __init__(self, graph, end_value=None)
        get_neighbors(self, value) -> Iterable[GraphNeighbor]
    """
    pass


GraphEdge = namedtuple('Edge', ['from_value', 'to_value', 'cost'])


class EdgeGraph:

    def __init__(self):
        self.edges = set()

    def add_two_way(self, a, b, cost):
        self.edges.add(GraphEdge(b, a, cost))
        self.edges.add(GraphEdge(a, b, cost))

    def __str__(self):
        edge_str = 'edges:\n' + '\n'.join(str(edge) for edge in self.edges)
        return '\n' + edge_str

    def get_neighbors(self, current_value):
        yield from (e for e in self.edges if e.from_value == current_value)


class EdgeGraphIterator:

    def __init__(self, graph, end_value=None):
        self.graph = graph

    def get_neighbors(self, value):
        return self.graph.get_neighbors(value)


class FieldGraph:

    def __init__(self, field):
        self.field = [f.strip() for f in field.strip().split('\n')]

    def get_location(self, char):
        for row_num, row in enumerate(self.field):
            for col_num, col in enumerate(row):
                if col == char:
                    return (row_num, col_num)
        raise ValueError("%r not in Field" % char)

    def get_neighbors(self, node):
        row, column = node
        neighbors = []
        # get possible neighbors
        for r in (row - 1, row, row + 1):
            for c in (column - 1, column, column + 1):
                # make sure everything is in bounds
                if (r, c) != (row, column) and r >= 0 and c >= 0:
                    try:
                        value = self.field[r][c]
                    except IndexError:
                        continue
                    else:
                        # make sure nothing is in the way
                        if value != 'x':
                            # get the cost and add to neighbors

                            # the diagonal positions are the rows and columns
                            # shifted by two and the 't' positions are shifted
                            # by one. Summing everything and dividing by two
                            # is an easy check for both cases
                            if (abs(r + row + c + column) % 2 == 0):
                                cost = 1.4  # diagonal
                            else:
                                cost = 1  # 't'

                            # This is a bit of a hack because I don't need a from_value
                            # in the actual algorithm
                            neighbors.append(GraphEdge(None, (r, c), cost))
        return neighbors

    def modify(self, *nodes):
        """turn (row, column) to 'p'"""
        for node in nodes:
            row, column = node
            self.field[row] = self.field[row][:column] + 'p' + self.field[row][column + 1:]

    def __str__(self):
        # replace  'o's with spaces and add some boundaries
        boundary = ' ' + '-' * len(self.field[0])
        field = '\n'.join('|' + f.replace('o', ' ') + '|' for f in self.field)
        return boundary + '\n' + field + '\n' + boundary


class FieldGraphIterator:

    def __init__(self, field, end_value=None):
        self.field = field

    def get_neighbors(self, value):
        return self.field.get_neighbors(value)


FIELD1 = """\
ooooooooooeooo
xxxxoooooooooo
ooooooxxooxxxx
xxoxxooooooooo
ooooxooooooooo
osoooooooooooo
"""

FIELD2 = """\
oooooooooooooo
oooooooooooooo
ooooooxxooxxxx
ooooooxooooooo
xxxxxoxoxxxxoo
osooooxooooeoo
"""


def test_graph():
    g = EdgeGraph()
    gi = EdgeGraphIterator(g)
    g.add_two_way('1', '2', 7)
    g.add_two_way('1', '3', 9)
    g.add_two_way('1', '6', 14)
    g.add_two_way('2', '3', 10)
    g.add_two_way('2', '4', 15)
    g.add_two_way('3', '4', 11)
    g.add_two_way('3', '6', 2)
    g.add_two_way('4', '5', 6)
    g.add_two_way('5', '6', 9)

    print(g)
    print(dijkstra('1', '5', gi))


def test_field(field):
    f = FieldGraph(field)
    fi = FieldGraphIterator(f)
    start = f.get_location('s')
    end = f.get_location('e')
    path = dijkstra(start, end, fi)
    # print(path)
    f.modify(*path[1:-1])
    print(f)


if __name__ == "__main__":
    test_graph()
    test_field(FIELD1)
    test_field(FIELD2)
