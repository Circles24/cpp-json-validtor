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



JSONDataEntityNode represents json data node the is itself a valid json data with opeaning and ending curly braces { ... }
it contains multiple nodes as its children which represents various key value pairs that are present in the data

											 JSONDataEntityNode

										   			 |
										   			 |
										   		    \|/

					       { {key:Node*}, {Key:Node*}, {Key:Node*}, {Key:Node*} }


*/


using namespace std;


class JSONEntityNode: public Node
{
private:

	vector<pair<string, Node* > > children;

public:

	JSONEntityNode(Node* , string , bool);

	string to_string()
	{
		string result = string(1, JSONResources::ENTITY_ENCLOSER_START);

		const int children_len = children.size();

		for (int i = 0; i < children_len; i++)
		{
			result
			.append(children[i].first)
			.append(":")
			.append(children[i].second->to_string());

			if (i != children_len - 1)
			{
				result.append(string(1, ','));
			}
		}

		result.push_back(JSONResources::ENTITY_ENCLOSER_END);

		return result;
	}

	void show_children()
	{
		for (pair<string, Node*> p : children)
		{
			if (p.second)cout << p.first << " : " << p.second->to_string() << endl;

			else cout << p.first << " : NULL" << endl;
		}

	}

	string get_type()
	{
		return Node::get_type()
		       .append("::JSONEntityNode");
	}

};
