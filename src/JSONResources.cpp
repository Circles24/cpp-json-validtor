
using namespace std;

class JSONResources
{

private:

	JSONResources()
	{

	};

public:

	static const vector<char> VALID_TOKENS;
	static const vector<char> IGNORABLE_TOKENS;
	static const char ENTITY_ENCLOSER_START;
	static const char ENTITY_ENCLOSER_END;
	static const char LIST_ENCLOSER_START;
	static const char LIST_ENCLOSER_END;
	static const char STRING_ENCLOSER_START;
	static const char STRING_ENCLOSER_END;
	static const char DATA_SEPERATOR;
	static const char ASSIGNMENT_TOKEN;

	static char opp_encloser(const char given_token)
	{
		char final_token;

		if(given_token == STRING_ENCLOSER_START )
		{
			final_token = STRING_ENCLOSER_END;
		}

		else if (given_token == STRING_ENCLOSER_END)
		{
			final_token = STRING_ENCLOSER_START;
		}

		else if (given_token == ENTITY_ENCLOSER_START)
		{
			final_token = ENTITY_ENCLOSER_END;
		}

		else if (given_token == ENTITY_ENCLOSER_END)
		{
			final_token = ENTITY_ENCLOSER_START;
		}

		else if (given_token == LIST_ENCLOSER_START)
		{
			final_token = LIST_ENCLOSER_END;
		}

		else if (given_token == LIST_ENCLOSER_END)
		{
			final_token = LIST_ENCLOSER_START;
		}

		else
		{
			throw "bad request";
		}

		return final_token;
	}

	static bool is_ignorable(const char given_token)
	{
		return ( find(IGNORABLE_TOKENS.begin(), IGNORABLE_TOKENS.end(), given_token) != IGNORABLE_TOKENS.end() );
	}

};


const vector<char> JSONResources::VALID_TOKENS = {'{', '}', '[', ']', ':', ',', '"'};
const vector<char> JSONResources::IGNORABLE_TOKENS = {'\n', ' ', '\t'};
const char JSONResources::ENTITY_ENCLOSER_START = '{';
const char JSONResources::ENTITY_ENCLOSER_END = '}';
const char JSONResources::LIST_ENCLOSER_START = '[';
const char JSONResources::LIST_ENCLOSER_END = ']';
const char JSONResources::STRING_ENCLOSER_START = '\"';
const char JSONResources::STRING_ENCLOSER_END = '\"';
const char JSONResources::DATA_SEPERATOR = ',';
const char JSONResources::ASSIGNMENT_TOKEN = ':';
