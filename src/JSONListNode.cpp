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



JSONDataListNode represents json data node the is itself a valid json array with opeaning and ending square braces [ ... ]
it contains multiple nodes, and its children represents json nodes themselves


											 JSONDataListNode

													 |
													 |
												    \|/

									  [ Node* , Node*, Node*, Node* ]


*/



using namespace std;


class JSONListNode: public Node
{
private:

	vector<Node*> children;

public:

	JSONListNode(Node*, string, bool);

	string to_string()
	{
		string result = string(1, JSONResources::LIST_ENCLOSER_START);

		const int children_len = children.size();

		for (int i = 0; i < children_len; i++)
		{
			result
			.append(children[i]->to_string());

			if (i != children_len - 1)
			{
				result.append(string(1, ','));
			}
		}

		result.push_back(JSONResources::LIST_ENCLOSER_END);

		return result;
	}

	void show_children()
	{

		cout << JSONResources::LIST_ENCLOSER_START << endl;

		for (Node* ptr : children)
		{
			cout << ptr->to_string() << endl;
		}

		cout << JSONResources::LIST_ENCLOSER_END << endl;

	}

	string get_type()
	{
		return Node::get_type()
		       .append("::JSONListNode");
	}
};

