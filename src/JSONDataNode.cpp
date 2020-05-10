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


												
JSONJSONDataNode represents json data node the is just a valid string it can be associated with some other key in the tree hierarchy 
it doesn't contain any childern just a plain utf-8 encoded string
 
											     JSONDataNode

								   		 	       	 |
										   			 |
										   		    \|/
													
			     							  	  " ... "


*/


using namespace std;

class JSONDataNode: public Node
{
private:

	string value;

public:

	JSONDataNode(Node* parent, string value):
		Node(parent),
		value(value)
	{

	}

	string to_string()
	{
		return value;
	}

	string get_type()
	{
		return Node::get_type().append("::JSONDataNode");
	}

};
