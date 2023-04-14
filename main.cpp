#include <iostream>
#include <map>
#include <vector>
#include <math.h>
#include <string>

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

const std::map<char, Operation> TOKEN_OPERATORS = {
	{ '+', Operation::plus },
	{ '-', Operation::minus },
	{ '*', Operation::multiply },
	{ '/', Operation::divide },
	{ '(', Operation::col_open },
	{ ')', Operation::col_close },
	{ '^', Operation::power },
	{ 'r', Operation::square_root }
};

constexpr bool is_number(const char ch)
{
	return ('0' <= ch) && (ch <= '9');
}

void output_operation(const Operation oper)
{
	switch (oper)
	{
	case Operation::plus:
		std::clog << "log: " << "+\n";
		break;
	case Operation::minus:
		std::clog << "log: " << "-\n";
		break;
	case Operation::multiply:
		std::clog << "log: " << "*\n";
		break;
	case Operation::divide:
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

bool operate(const std::string& expression, std::vector<float>& numbers, const Operation oper, char previous_char)
{
	//output_operation(oper);

	switch (oper)
	{
	case Operation::plus:
		numbers.push_back(0);
		break;
	case Operation::minus:
		*numbers.rbegin() *= -1;
		if(previous_char != *expression.begin())
			numbers.push_back(0);
		break;
	case Operation::multiply:
		*(numbers.rbegin() + 1) *= *(numbers.rbegin());
		numbers.pop_back();
		break;
	case Operation::divide:
		if(*(numbers.rbegin()) == 0)
		{
			std::cerr << "error: " << "can not divide by 0" << '\n';
			return false;
		}
		*(numbers.rbegin() + 1) /= *(numbers.rbegin());
		numbers.pop_back();
		break;
	case Operation::power:
		{
			auto& expr1 = *(numbers.rbegin() + 1);
			auto& expr2 = *(numbers.rbegin());
			expr1 = pow(expr1, expr2);
			numbers.pop_back();
		}
		break;
	case Operation::square_root:
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
	try
	{
		numbers.push_back(std::stof(number_string));
	}
	catch(const std::exception& e)
	{
		std::cerr << "error: " << e.what() << ": the number wasn't recognized" << '\n';
	}

	if(need_clear)
		number_string = "";
}

float express(const std::string& expression)
{
	std::vector<float> numbers;
	std::string number_string = "0";

	Operation hold = Operation::none;

	// parse expression string
	for(const auto& ch: expression)
	{
		if(ch == '\0')
			break;

		if(!is_number(ch) && ch != '.')
		{
			//std::clog << "log: " << "oper\n";
			handle_push_numbers(numbers, number_string);

			if(!operate(expression, numbers, hold, ch))
				return NAN;

			try
			{
				hold = TOKEN_OPERATORS.find(ch)->second;
			}
			catch(const std::exception& e)
			{
				std::cerr << e.what() << '\n';
			}
		}
		else
		{
			//std::clog << "log: " << "numb\n";
			number_string += ch;
		}
	}

	handle_push_numbers(numbers, number_string, false);
	if(!operate(expression, numbers, hold, *expression.crbegin()))
				return NAN;

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


