#include <iostream>
#include <map>
#include <vector>
#include <math.h>

enum class Operation
{
	none,
	plus,
	minus,
	multiply,
	divide,
	col_open,
	col_close,
	power,
	square_root
};

const std::map<Operation, char> TOKEN_OPERATORS = {
	{ Operation::plus, '+' },
	{ Operation::minus, '-' },
	{ Operation::multiply, '*' },
	{ Operation::divide, '/' },
	{ Operation::col_open, '(' },
	{ Operation::col_close, ')' },
	{ Operation::power, '^' },
	{ Operation::square_root, 'r' }
};

constexpr bool is_number(const char ch)
{
	return ('0' <= ch) && (ch <= '9');
}

float express(const std::string& expression)
{
	std::vector<float> numbers {0};
	std::string number_string;

	Operation hold = Operation::none;

	// parse expression string
	for(auto& ch: expression)
	{
		if(ch == '\0')
			break;

		if(!is_number(ch) && ch != '.')
		{
			switch (hold)
			{
			case Operation::plus:
				numbers.push_back(0);
				break;
			case Operation::minus:
				*numbers.rbegin() *= -1;
				if(ch != *expression.begin())
					numbers.push_back(0);
				break;
			default:
				break;
			}

			for(auto& oper : TOKEN_OPERATORS)
			{
				if(ch != oper.second)
					continue;
				hold = oper.first;
				break;
			}				
		}
		else
			number_string += ch;
	}

	// parse stack and summary
	float res = 0;

	for(auto& n : numbers)
		res += n;

	return res;	
}

int main(int argc, char const *argv[])
{
	if (argc < 2)
	{
		std::cerr << "error: invalid expression\n";
		std::cerr << "note: try to launch the application in terminal and add expression\n";
		return 1;
	}

	std::cout << express(argv[1]) << '\n';
	return 0;
}

