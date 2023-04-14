#include <iostream>
#include <map>
#include <vector>
#define _USE_MATH_DEFINES
#include <math.h>
#include <string>

enum class BasicOperation
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

enum class TrigonometryOperation
{
	sin,
	cos,
	tan,
	cot,
	arcsin,
	arccos,
	arctan,
	arccot
};

enum class LogarithmOperation
{
	log,
	ln
};

const std::map<char, BasicOperation> TOKEN_BASIC_OPERATORS = {
	{ '+', BasicOperation::plus },
	{ '-', BasicOperation::minus },
	{ '*', BasicOperation::multiply },
	{ '/', BasicOperation::divide },
	{ '(', BasicOperation::col_open },
	{ ')', BasicOperation::col_close },
	{ '^', BasicOperation::power },
	{ 'r', BasicOperation::square_root }
};

const std::map<std::string, TrigonometryOperation> TOKEN_TRIGONOMETRY_OPERATORS = {
	{ "sin", TrigonometryOperation::sin },
	{ "cos", TrigonometryOperation::cos },
	{ "tan", TrigonometryOperation::tan },
	{ "cot", TrigonometryOperation::cot },
	{ "arcsin", TrigonometryOperation::arcsin },
	{ "arcsin", TrigonometryOperation::arccos },
	{ "arcsin", TrigonometryOperation::arctan },
	{ "arcsin", TrigonometryOperation::arccot }
};

const std::map<std::string, LogarithmOperation> TOKEN_LOGARITHM_OPERATORS = {
	{ "log", LogarithmOperation::log },
	{ "ln", LogarithmOperation::ln },
};

const std::map<std::string, float> TOKEN_CONSTANT_NUMBERS = {
	{ "pi", M_PI },
	{ "eul", M_E },
	{ "gld", 1.618033988749 }
};

//const std::map<std::string, >

constexpr bool is_number(const char ch)
{
	return ('0' <= ch) && (ch <= '9');
}

void output_operation(const BasicOperation oper)
{
	switch (oper)
	{
	case BasicOperation::plus:
		std::clog << "log: " << "+\n";
		break;
	case BasicOperation::minus:
		std::clog << "log: " << "-\n";
		break;
	case BasicOperation::multiply:
		std::clog << "log: " << "*\n";
		break;
	case BasicOperation::divide:
		std::clog << "log: " << "/\n";
		break;
	default:
		std::clog << "log: " << "0\n";
		break;
	}
}

void output_numbers(const std::vector<float>& numbers)
{
	for(auto& n : numbers)
	{
		std::clog << "log: " << "numbers: " << n << '\n';
	}
}

bool operate(const std::string& expression, std::vector<float>& numbers, const BasicOperation oper, char previous_char)
{
	//output_operation(oper);

	switch (oper)
	{
	case BasicOperation::plus:
		numbers.push_back(0);
		break;
	case BasicOperation::minus:
		*numbers.rbegin() *= -1;
		if(previous_char != *expression.begin())
			numbers.push_back(0);
		break;
	case BasicOperation::multiply:
		*(numbers.rbegin() + 1) *= *(numbers.rbegin());
		numbers.pop_back();
		break;
	case BasicOperation::divide:
		if(*(numbers.rbegin()) == 0)
		{
			std::cerr << "error: " << "can not divide by 0" << '\n';
			return false;
		}
		*(numbers.rbegin() + 1) /= *(numbers.rbegin());
		numbers.pop_back();
		break;
	case BasicOperation::power:
		{
			auto& expr1 = *(numbers.rbegin() + 1);
			auto& expr2 = *(numbers.rbegin());
			expr1 = pow(expr1, expr2);
			numbers.pop_back();
		}
		break;
	case BasicOperation::square_root:
		if(*(numbers.rbegin()) < 0)
		{
			std::cerr << "error: " << "can not be negative sign in square root" << '\n';
			return false;
		}
		*numbers.rbegin() = sqrt(*numbers.rbegin());
		break;
	default:
		break;
	}
	return true;
}

void handle_push_numbers(std::vector<float>& numbers, std::string& number_string, bool need_clear = true)
{
	if(number_string.empty())
		return;

	try
	{
		numbers.push_back(std::stof(number_string));
	}
	catch(const std::exception& e)
	{
		std::cerr << "error: " << e.what() << ": the number wasn't recognized as \"" << number_string << '\"' << '\n';
	}

	if(need_clear)
		number_string = "";
}

float express(const std::string& expression)
{
	std::vector<float> numbers;
	std::string number_string = "0";

	//std::clog << "log: " << expression << '\n';

	BasicOperation hold = BasicOperation::none;

	int16_t col_depth = 0;

	// parse expression string
	for(const auto& ch: expression)
	{
		//std::clog << "log: " << "iter" << '\n';
		if(ch == '\0')
			break;

		if(TOKEN_BASIC_OPERATORS.find(ch)->second == BasicOperation::col_close)
		{
			col_depth--;
			if(!col_depth)
			{
				number_string.erase(number_string.begin());
				float res = express(number_string);
				number_string = std::to_string(res);
			}
			continue;
		}
		else if(TOKEN_BASIC_OPERATORS.find(ch)->second == BasicOperation::col_open)
		{
			col_depth++;
			continue;
		}
		else if(!is_number(ch) && ch != '.' && !col_depth)
		{
			
			handle_push_numbers(numbers, number_string);

			if(!operate(expression, numbers, hold, ch))
				return NAN;

			try
			{
				hold = TOKEN_BASIC_OPERATORS.find(ch)->second;
			}
			catch(const std::exception& e)
			{
				std::cerr << e.what() << '\n';
			}
		}
		else
			number_string += ch;
	}

	handle_push_numbers(numbers, number_string, false);
	if(!operate(expression, numbers, hold, *expression.crbegin()))
				return NAN;

	// parse stack and summary
	float res = 0;
	for(auto& n : numbers)
		res += n;
	//std::clog << "log: " << "return: " << res << '\n';
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


