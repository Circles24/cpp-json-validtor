/*
											        Node
												    /|\
											       / | \
											      /  |  \
										         /   |   \
										        /    |    \
									           /     |     \
								              /      |      \
							           		 /       |       \
								JSONEntityNode  JSONDataNode  JSONListNode
								 	{ ... }       " ... "      [ ... ]


It is the base class of the json tree nodes inheritance tree, its pointer can be utilized for runtime  polimorphism and
can be used to point to any json node and invoke utility nodes


*/

#include <iostream>
#include <string>
#include <utility>
#include <vector>
#include <algorithm>
#include <stack>

using namespace std;


class Node
{

protected:

	Node* parent;

	Node(Node* parent)
	{
		this->parent = parent;
	}

public:

	virtual string get_type()
	{
		return "Node";
	}

	virtual string to_string() = 0;
};

