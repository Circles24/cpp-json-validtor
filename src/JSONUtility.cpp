using namespace std;

class JSONUtility
{

private:
	JSONUtility()
	{

	}

public:

	static Node* build_json_node(Node* parent, string json_data,bool debug = false)
	{

		int n = json_data.length();

		Node* node = NULL;

		try
		{
			if (json_data[0] == JSONResources::opp_encloser(json_data[n - 1]))
			{

				if (json_data[0] == JSONResources::STRING_ENCLOSER_START)
				{
					node = new JSONDataNode(parent, json_data);
				}

				else if (json_data[0] == JSONResources::ENTITY_ENCLOSER_START)
				{
					node = new JSONEntityNode(parent, json_data,debug);
				}

				else
				{
					node = new JSONListNode(parent, json_data,debug);
				}

			}

		}

		catch (string ex)
		{
			throw "syntax error, invalid json data provided";
		}

		return node;
	}

	static bool is_valid(string json_data)
	{
		try
		{
			build_json_node(NULL,json_data);
		}

		catch(string ex)
		{
			return false;
		}

		return true;
	}

	static string prettify(string json_data)
	{
		return "{\n\t\"key\" : \"dummy_pretty_data\"\n}";
	}

};
