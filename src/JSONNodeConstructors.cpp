using namespace std;

JSONEntityNode::JSONEntityNode(Node* parent, string data, bool debug = false):
	Node(parent)
{

	if (debug == true)
	{
		cout << "processing\n" << data << endl;
	}

	/*
	Algorithm

	check pre condiitons
		starts with {
		ends with }

	states |- outside_body
		   |	{ -> inside_body
		   |
		   |
	       |- inside_body
	       |	" -> inside_key
	       |
	       |
	       |- inside_key
		   |	" -> parsed_key
		   |
		   |
	       |- parsed_key
	       |	: -> parsed_assignment
	       |
	       |
	       |- parsed_assignment
	       |	" -> PARSING_DATA_VALUE
	       |	{ -> parsing_json_value
	       |
	       |
	       |- PARSING_DATA_VALUE
	       |	" -> parsed_value
	       |
	       |
	       |- parsing_json_value
	       |	token_stack.size == 0 -> parsed_value
		   |
		   |
	       |- parsed_value
	       |	} -> parsed_body
	       |	, -> inside_body
	       |- parsed_body
	       |	unignorable-token - syntax error
	       |



	*/


	enum State {

		OUTSIDE_BODY,
		INSIDE_BODY,
		INSIDE_KEY,
		PARSED_KEY,
		PARSED_ASSIGNMENT,
		PARSING_DATA_VALUE,
		PARSING_JSON_VALUE,
		PARSED_VALUE,
		PARSED_BODY

	};

	vector<string> state_rep = {"OUTSIDE_BODY",
	                            "INSIDE_BODY",
	                            "INSIDE_KEY",
	                            "PARSED_KEY",
	                            "PARSED_ASSIGNMENT",
	                            "PARSING_DATA_VALUE",
	                            "PARSING_JSON_VALUE",
	                            "PARSED_VALUE",
	                            "PARSED_BODY"
	                           };


	State curr_state = OUTSIDE_BODY;
	const int data_len = data.size();
	char curr_token;
	string buffer_string;
	stack<char> token_stack;
	string curr_key;

	bool need_to_throw_error = false;
	char expected_token;


	// precheck
	if (
	    ( data[0] != JSONResources::ENTITY_ENCLOSER_START ) ||
	    ( data[data_len - 1] != JSONResources::ENTITY_ENCLOSER_END ) )
	{
		throw "Invalid Data :: json entity node preconditions failed \n invalid data -> " + data;
	}

	//  automata build
	for (int i = 0; i < data_len; i++)
	{
		curr_token = data[i];

		if (curr_state != PARSING_JSON_VALUE &&
		        curr_state != INSIDE_KEY &&
		        curr_state != PARSING_DATA_VALUE &&
		        JSONResources::is_ignorable(curr_token))
		{
			continue;
		}

		if (debug == true)
		{
			cout << "@ " << i << " " << curr_token << " " << state_rep[curr_state] << endl;

		}

		switch (curr_state)
		{

		case OUTSIDE_BODY:
		{

			if (curr_token == JSONResources::ENTITY_ENCLOSER_START)
			{
				curr_state = INSIDE_BODY;
			}

			else
			{
				need_to_throw_error = true;
				expected_token = JSONResources::ENTITY_ENCLOSER_START;
			}

		} break;

		case INSIDE_BODY:
		{

			if (curr_token == JSONResources::STRING_ENCLOSER_START)
			{
				curr_state = INSIDE_KEY;
				buffer_string.clear();
			}

			else
			{
				need_to_throw_error = true;
				expected_token = JSONResources::STRING_ENCLOSER_START;
			}

		} break;

		case INSIDE_KEY:
		{
			if (curr_token == JSONResources::STRING_ENCLOSER_END)
			{
				curr_state = PARSED_KEY;
				curr_key = buffer_string;
			}

			else
			{
				buffer_string.push_back(curr_token);
			}

		} break;

		case PARSED_KEY:
		{
			if (curr_token == JSONResources::ASSIGNMENT_TOKEN)
			{
				curr_state = PARSED_ASSIGNMENT;
			}

			else
			{
				need_to_throw_error = true;
				expected_token = JSONResources::ASSIGNMENT_TOKEN;
			}

		} break;

		case PARSED_ASSIGNMENT:
		{
			if (curr_token == JSONResources::STRING_ENCLOSER_START)
			{
				curr_state = PARSING_DATA_VALUE;
				buffer_string.clear();
			}

			else if ( (curr_token == JSONResources::ENTITY_ENCLOSER_START) ||
			          (curr_token == JSONResources::LIST_ENCLOSER_START))
			{
				curr_state = PARSING_JSON_VALUE;

				while (token_stack.size())
				{
					token_stack.pop();
				}

				token_stack.push(curr_token);
				buffer_string = string(1, (curr_token));
			}

			else
			{
				throw ("excepected " +
				       string(1, JSONResources::STRING_ENCLOSER_START) +
				       " or " +
				       string(1, JSONResources::LIST_ENCLOSER_START) +
				       " found " +
				       string(1, curr_token));
			}

		} break;

		case PARSING_DATA_VALUE:
		{
			if (curr_token == JSONResources::STRING_ENCLOSER_END)
			{
				curr_state = PARSED_VALUE;

				children.push_back({curr_key, new JSONDataNode(this, buffer_string)});

				buffer_string.clear();
			}

			else
			{
				buffer_string.push_back(curr_token);
			}

		} break;

		case PARSING_JSON_VALUE:
		{

			buffer_string.push_back(curr_token);

			if (curr_token == JSONResources::ENTITY_ENCLOSER_START ||
			        curr_token == JSONResources::LIST_ENCLOSER_START)
			{
				token_stack.push(curr_token);
			}

			else if (curr_token == JSONResources::ENTITY_ENCLOSER_END ||
			         curr_token == JSONResources::LIST_ENCLOSER_END)
			{

				if (token_stack.size() == 0 ||
				        curr_token != JSONResources::opp_encloser(token_stack.top()))
				{

					throw "improper json data, unbalanced parenthisses";
				}

				else
				{

					token_stack.pop();

					if (token_stack.size() == 0)
					{
						curr_state = PARSED_VALUE;

						Node* ptr = JSONUtility::build_json_node(this, buffer_string, debug);

						children.push_back({curr_key, ptr});

					}
				}
			}

		} break;

		case PARSED_VALUE:
		{
			if (curr_token == JSONResources::ENTITY_ENCLOSER_END)
			{
				curr_state = PARSED_BODY;
			}

			else if (curr_token == JSONResources::DATA_SEPERATOR)
			{
				curr_state = INSIDE_BODY;
			}

			else
			{
				// throw new ("wrong json format at "+to_string(i));
				throw  ("wrong json format at dummy_index");

			}

		} break;

		case PARSED_BODY:
		{
			if (JSONResources::is_ignorable(curr_token) == false)
			{
				throw ("Syntax error, unexpected token at ," + std::to_string(i));
			}

		} break;

		}

		if (need_to_throw_error)
		{
			// throw  "Wrong json format expected "+string(1,expected_token)+" found "+string(1,curr_token)+" at "+to_string(i);
			throw  "Wrong json format expected " + string(1, expected_token) + " found " + string(1, curr_token) + " at dummy_index";
		}

	}

	if (curr_state != PARSED_BODY)
	{
		throw  "incomplete json data";
	}
}


JSONListNode::JSONListNode(Node* parent, string data, bool debug = false):
	Node(parent)
{
	/*
	Algorithm

	check pre condiitons

		preconditions
			starts with - [
			ends with   - ]

	states |- OUTSIDE_BODY
		   |	[ -> INSIDE_BODY
		   |
		   |
	       |- INSIDE_BODY
	       |	{ -> PARSING_JSON_CHILD
	       |    ] -> PARSED_BODY
	       |
	       |
	       |- PARSING_JSON_CHILD
	       |	empty token stack -> parsed_child
	       |
	       |
	       |- PARSED_CHILD
	       |	, -> INSIDE_BODY
	       |	] -> PARSED_BODY
	       |
	       |- PARSED_BODY
	       |	unignorable-token -> syntax error


	*/


	enum State {

		OUTSIDE_BODY,
		INSIDE_BODY,
		PARSING_DATA_CHILD,
		PARSING_JSON_CHILD,
		PARSED_CHILD,
		PARSED_BODY

	};

	vector<string> state_rep = {"OUTSIDE_BODY",
	                            "INSIDE_BODY",
	                            "PARSING_DATA_CHILD",
	                            "PARSING_JSON_CHILD",
	                            "PARSED_CHILD",
	                            "PARSED_BODY"
	                           };


	State curr_state = OUTSIDE_BODY;
	const int data_len = data.size();
	char curr_token;
	string buffer_string;
	stack<char> token_stack;


	// precheck
	if (
	    ( data[0] != JSONResources::LIST_ENCLOSER_START ) ||
	    ( data[data_len - 1] != JSONResources::LIST_ENCLOSER_END ) )
	{
		throw "Invalid Data :: json list entity node preconditions failed \n invalid data -> " + data;
	}

	//  automata build
	for (int i = 0; i < data_len; i++)
	{
		curr_token = data[i];


		if (curr_state != PARSING_JSON_CHILD && JSONResources::is_ignorable(curr_token))
		{
			continue;
		}

		if (debug == true)
		{
			cout << "@ " << i << " " << curr_token << " " << state_rep[curr_state] << endl;
		}

		switch (curr_state)
		{

		case OUTSIDE_BODY:
		{

			if ((curr_token == JSONResources::LIST_ENCLOSER_START))
			{
				curr_state = INSIDE_BODY;
			}

			else
			{
				throw ("expected " +
				       string(1, JSONResources::LIST_ENCLOSER_START) +
				       " , got " +
				       string(1, curr_token));
			}

		} break;

		case INSIDE_BODY:
		{

			if (curr_token == JSONResources::STRING_ENCLOSER_START)
			{
				curr_state = PARSING_DATA_CHILD;
				buffer_string = string(1, curr_token);
			}

			else if ((curr_token == JSONResources::ENTITY_ENCLOSER_START) ||
			         (curr_token == JSONResources::LIST_ENCLOSER_START))
			{
				curr_state = PARSING_JSON_CHILD;

				while (token_stack.size())
				{
					token_stack.pop();
				}

				buffer_string = string(1, curr_token);
				token_stack.push(curr_token);
			}

			else if (curr_token == JSONResources::LIST_ENCLOSER_END)
			{
				curr_state = PARSED_BODY;
			}

			else
			{
				throw 	("expected " +
				         string(1, JSONResources::ENTITY_ENCLOSER_START) +
				         " or " +
				         string(1, JSONResources::LIST_ENCLOSER_END) +
				         ", found " +
				         curr_token);
			}

		} break;

		case PARSING_DATA_CHILD:
		{
			buffer_string.push_back(curr_token);

			if (curr_token == JSONResources::STRING_ENCLOSER_END)
			{
				curr_state = PARSED_CHILD;
				children.push_back(JSONUtility::build_json_node(this, buffer_string, debug));
			}


		} break;

		case PARSING_JSON_CHILD:
		{
			buffer_string.push_back(curr_token);

			if ( (curr_token == JSONResources::ENTITY_ENCLOSER_START) ||
			        (curr_token == JSONResources::LIST_ENCLOSER_START))
			{
				token_stack.push(curr_token);
			}

			else if ((curr_token == JSONResources::ENTITY_ENCLOSER_END ) ||
			         ( curr_token == JSONResources::LIST_ENCLOSER_END))
			{
				if (token_stack.size() == 0 ||
				        token_stack.top() != JSONResources::opp_encloser(curr_token))
				{
					throw ("syntax error :: unmatching brackets detected :: expected " +
					       string(1, token_stack.top()) +
					       " got " +
					       string(1, curr_token) +
					       "\n" +
					       data);
				}

				else
				{
					token_stack.pop();

					if (token_stack.size() == 0)
					{
						curr_state = PARSED_CHILD;

						children.push_back(JSONUtility::build_json_node(this, buffer_string, debug));
					}
				}
			}


		} break;

		case PARSED_CHILD:
		{
			if (curr_token == JSONResources::DATA_SEPERATOR)
			{
				curr_state = INSIDE_BODY;
			}

			else if (curr_token == JSONResources::LIST_ENCLOSER_END)
			{
				curr_state = PARSED_BODY;
			}

			else
			{
				throw ("expected " +
				       string(1, JSONResources::DATA_SEPERATOR) +
				       ", or " +
				       string(1, JSONResources::LIST_ENCLOSER_END) +
				       ", got " +
				       string(1, curr_token));
			}

		} break;

		case PARSED_BODY:
		{

			if (JSONResources::is_ignorable(curr_token) == false)
			{
				throw ("expected " +
				       string(1, JSONResources::LIST_ENCLOSER_END) +
				       " got " +
				       string(1, curr_token));
			}

		} break;

		}

	}

	if (curr_state != PARSED_BODY)
	{
		throw  "incomplete json data";
	}
}

