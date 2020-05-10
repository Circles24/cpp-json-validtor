#include "Node.cpp"
#include "JSONResources.cpp"
#include "JSONDataNode.cpp"
#include "JSONEntityNode.cpp"
#include "JSONListNode.cpp"
#include "JSONUtility.cpp"
#include "JSONNodeConstructors.cpp"

#include <fstream>

using namespace std;

int main()
{
	cout << "This is the testing script wait while we test the json parser" << endl;

	try
	{

		int n;
		string test_name;
		string json_data;
		bool result;
		fstream fin;

		cout << "enter the number of tests to validate" << endl;
		cin >> n;

		cout << "testing has started" << endl;

		for (int i = 0; i <= n; i++)
		{
			test_name = "tests/test_" + to_string(n) + ".json";

			cout << test_name << " -> ";

			try
			{
				fin.open(test_name);
				json_data = string((std::istreambuf_iterator<char>(fin)),
				                   std::istreambuf_iterator<char>());

				result = JSONUtility::is_valid(json_data);

				cout << (result ? " passed " : " failed ") << endl;

			}

			catch (string ex)
			{
				cout << ex << endl;
			}

			fin.close();

		}


	}

	catch (string ex)
	{
		cout << "@ ex" << endl;
		cout << ex << endl;
	}

	cout << "done with testing" << endl;

	return 0;

}

