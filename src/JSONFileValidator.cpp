#include "Node.cpp"
#include "JSONResources.cpp"
#include "JSONDataNode.cpp"
#include "JSONEntityNode.cpp"
#include "JSONListNode.cpp"
#include "JSONUtility.cpp"
#include "JSONNodeConstructors.cpp"

#include <fstream>

using namespace std;

int main(int argc, char ** argv)
{
	try
	{
		string input_file_name, debug_choice;

		cout << "enter the file name for json validity" << endl;
		cin >> input_file_name;
		cout << "wanna se internal states (1/0)" << endl;
		cin >> debug_choice;

		fstream fin(input_file_name);

		string json_data((std::istreambuf_iterator<char>(fin)),
		                 std::istreambuf_iterator<char>());

		string linear_json_data = "";

		for (auto i : json_data)
		{
			if (i != ' ' && i != '\n' && i != '\t')
			{
				linear_json_data.push_back(i);
			}
		}

		cout << "precessing -> " << linear_json_data << endl;

		Node* ptr = JSONUtility::build_json_node(NULL, json_data, debug_choice == "1");

		cout << ptr->to_string() << endl;
		cout << ptr->get_type() << endl;

		cout << "validity :: " << JSONUtility::is_valid(json_data) << endl;

	}

	catch (string ex)
	{
		cout << "@ ex" << endl;
		cout << ex << endl;
	}

	return 0;
}
