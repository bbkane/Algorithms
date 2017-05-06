# -*- coding: utf-8 -*-
"""
Created on Mon Jul 11 21:32:46 2016

@author: Ben
"""

from collections import namedtuple


def dijkstra(start, end, get_neighbors):
    """
    Args:
        start (node): start node
        end (node): end node
        node (type): must implement __hash__, __eq__, __str__
        get_neighbors(node) -> iterable of classes which have a cost: number member
            and a finish: node member

    Returns:
        [node]: list of the nodes from start to finish (shortest path)

    Raises:
        NoPathException: No path found
    """

    class Element:
        # Can't use a namedtuple because it needs to be mutable
        def __init__(self, item, cost, parent=None):
            self.item = item
            self.cost = cost
            self.parent = parent

        def __repr__(self):
            return 'Element(item=%r, cost=%r)' % (self.item, self.cost)

    class NoPathException(Exception):
        pass

    explored = set()
    frontier = []
    frontier.append(Element(start, 0))
    while True:
        if not frontier:
            raise NoPathException(str(start))
        # treat the list like a priority queue by sorting it by cost
        frontier.sort(key=lambda e: e.cost)
        # pop the first element
        current_node = frontier.pop(0)
        print(current_node.item, current_node.cost)
        if current_node.item == end:  # We're done. Get the path and return
            path = []
            while current_node.parent:
                path.append(current_node.item)
                current_node = current_node.parent
            path.append(current_node.item)
            path.reverse()
            return path
        explored.add(current_node.item)
        node_edges = get_neighbors(current_node.item)
        for neighbor in node_edges:
            print("neighbor:", neighbor.finish, neighbor.cost)
            total_neighbor_cost = neighbor.cost + current_node.cost
            if neighbor.finish not in explored:
                for element in frontier:
                    if element.item == neighbor.finish:
                        if element.cost > total_neighbor_cost:
                            element.cost = total_neighbor_cost
                            element.parent = current_node
                        break
                else:  # no break (the item wasn't in the frontier)
                    e = Element(neighbor.finish, total_neighbor_cost)
                    frontier.append(e)
                    e.parent = current_node


Edge = namedtuple('Edge', ['start', 'finish', 'cost'])


class Graph:

    def __init__(self):
        self.nodes = set()
        self.edges = set()

    def add_two_way(self, a, b, cost):
        self.nodes.add(a)
        self.nodes.add(b)
        self.edges.add(Edge(b, a, cost))
        self.edges.add(Edge(a, b, cost))

    def __str__(self):
        node_str = 'nodes:\n' + '\n'.join(str(node) for node in self.nodes)
        edge_str = 'edges:\n' + '\n'.join(str(edge) for edge in self.edges)
        return node_str + '\n' + edge_str

    def get_neighbors(self, current_node):
        yield from {e for e in self.edges if e.start == current_node}


FIELD = """\
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
xxxxxoxoxooooo
osooooxooooeoo
"""


class Field:

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
                            neighbors.append(Edge(None, (r, c), cost))
        return neighbors

    def modify(self, *nodes):
        """turn (row, column) to 'p'"""
        for node in nodes:
            row, column = node
            self.field[row] = self.field[row][:column] + 'p' + self.field[row][column + 1:]

    def __str__(self):
        # replace all but outer 'o's with spaces
        return '\n'.join(self.field)


def test_graph():
    g = Graph()

    g.add_two_way('1', '2', 7)
    g.add_two_way('1', '3', 9)
    g.add_two_way('1', '6', 14)
    g.add_two_way('2', '3', 10)
    g.add_two_way('2', '4', 15)
    g.add_two_way('3', '4', 11)
    g.add_two_way('3', '6', 2)
    g.add_two_way('4', '5', 6)
    g.add_two_way('5', '6', 9)

    print(dijkstra('1', '5', g.get_neighbors))


def test_field():
    f = Field(FIELD2)
    print(f)
    start = f.get_location('s')
    end = f.get_location('e')
    path = dijkstra(start, end, f.get_neighbors)
    print(path)
    f.modify(*path)
    print(f)


if __name__ == "__main__":
    test_graph()
