#include "pch.h"
#include "math/M_Math.h"

#include "tools/M_Console.h"
#include "tools/M_Parse.h"
#include "assets/M_Text.h"
#include "types/M_Vector.h"
#include "types/M_Matrix.h"
#include "types/M_Quaternion.h"
#include <cmath>
#include <vector>
#include <regex>
#include <unordered_map>
#include <unordered_set>
#include <algorithm>

using namespace minty;

float minty::Math::clamp(float const value, float const minimum, float const maximum)
{
	return std::max(minimum, std::min(maximum, value));
}

int minty::Math::clamp(int const value, int const minimum, int const maximum)
{
	return std::max(minimum, std::min(maximum, value));
}

float minty::Math::lerp(float const left, float const right, float const t)
{
	return (right - left) * t + left;
}

int minty::Math::lerp(int const left, int const right, float const t)
{
	return static_cast<int>(roundf((right - left) * t + left));
}

float minty::Math::lerp_clamped(float const left, float const right, float const t)
{
	return ((right - left) * clamp(t, 0.0f, 1.0f) + left);
}

float minty::Math::round(float const value)
{
	return roundf(value);
}

float minty::Math::round(float const value, float const precision)
{
	return roundf(value / precision) * precision;
}

int minty::Math::round_to_int(float const value)
{
	return static_cast<int>(roundf(value));
}

int minty::Math::round_to_int(float const value, float const precision)
{
	return static_cast<int>(roundf(value / precision) * precision);
}

float minty::Math::floor(float const value)
{
	return floorf(value);
}

int minty::Math::floor_to_int(float const value)
{
	return static_cast<int>(floorf(value));
}

float minty::Math::ceil(float const value)
{
	return ceilf(value);
}

int minty::Math::ceil_to_int(float const value)
{
	return static_cast<int>(ceilf(value));
}

float minty::Math::abs(float const value)
{
	return fabsf(value);
}

float minty::Math::mod(float const value, float const mod)
{
	return value - floorf(value / mod) * mod;
}

float minty::Math::deg2rad(float const degree)
{
	return degree * DEG2RAD;
}

float minty::Math::rad2deg(float const rad)
{
	return rad * RAD2DEG;
}

float minty::Math::sin(float const rad)
{
	return sinf(rad);
}

float minty::Math::cos(float const rad)
{
	return cosf(rad);
}

float minty::Math::tan(float const rad)
{
	return tanf(rad);
}

float minty::Math::asin(float const rad)
{
	return asinf(rad);
}

float minty::Math::acos(float const rad)
{
	return acosf(rad);
}

float minty::Math::atan(float const rad)
{
	return atanf(rad);
}

float minty::Math::atan2(float const dy, float const dx)
{
	return atan2f(dy, dx);
}

float minty::Math::angle(float const dx, float const dy)
{
	return atan2f(dy, dx);
}

float minty::Math::sqrt(float const value)
{
	return sqrtf(value);
}

float minty::Math::pow(float const value, float const power)
{
	return powf(value, power);
}

float minty::Math::magnitude(float const x, float const y)
{
	return sqrtf(x * x + y * y);
}

float minty::Math::sign(float const value)
{
	return value < 0.0f ? -1.0f : 1.0f;
}

float minty::Math::signz(float const value)
{
	return value < 0.0f ? -1.0f : (value > 0.0f ? 1.0f : 0.0f);
}

bool is_number(String const& str)
{
	bool decimalPointFound = false;
	for (char c : str) {
		if (!isdigit(c)) {
			if (c == '.' && !decimalPointFound) {
				decimalPointFound = true;
			}
			else {
				return false;
			}
		}
	}
	return true;
}

bool try_get_constant(String const& str, float& value)
{
	static std::unordered_map<String, float> const constants =
	{
		{"PI", minty::Math::PI},
		{"FLOAT", static_cast<float>(sizeof(float))},
		{"DOUBLE", static_cast<float>(sizeof(double))},
		{"INT", static_cast<float>(sizeof(int))},
		{"ID", static_cast<float>(sizeof(minty::ID))},
		{"SIZE_T", static_cast<float>(sizeof(size_t))},
		{"BYTE", static_cast<float>(sizeof(BYTE))},
		{"CHAR", static_cast<float>(sizeof(char))},
		{"SHORT", static_cast<float>(sizeof(short))},
		{"LONG", static_cast<float>(sizeof(long))},
		{"VECTOR2", static_cast<float>(sizeof(minty::Vector2))},
		{"VECTOR3", static_cast<float>(sizeof(minty::Vector3))},
		{"VECTOR4", static_cast<float>(sizeof(minty::Vector4))},
		{"VECTOR2INT", static_cast<float>(sizeof(minty::Vector2Int))},
		{"VECTOR3INT", static_cast<float>(sizeof(minty::Vector3Int))},
		{"VECTOR4INT", static_cast<float>(sizeof(minty::Vector4Int))},
		{"MATRIX2", static_cast<float>(sizeof(minty::Matrix2))},
		{"MATRIX3", static_cast<float>(sizeof(minty::Matrix3))},
		{"MATRIX4", static_cast<float>(sizeof(minty::Matrix4))},
		{"QUATERNION", static_cast<float>(sizeof(minty::Quaternion))},
		//{"", static_cast<float>(sizeof())},
	};

	auto found = constants.find(minty::Text::to_upper(str));
	if (found == constants.end())
	{
		return false;
	}

	value = found->second;
	return true;
}

int is_function(String const& str)
{
	static std::unordered_map<String, int> const functionNames =
	{

	};

	auto found = functionNames.find(str);
	if (found == functionNames.end())
	{
		return 0;
	}

	return found->second;
}

// https://en.cppreference.com/w/c/language/operator_precedence
int operator_precedence(String const& str)
{
	static std::unordered_map<String, int> const precedence =
	{
		{"^", 4},
		{"*", 3},
		{"/", 3},
		{"+", 2},
		{"-", 2}
	};

	auto found = precedence.find(str);

	if (found == precedence.end())
	{
		return 0; // not an operator
	}
	else
	{
		return found->second;
	}
}

// from chatgpt
std::vector<String> split_into_tokens(String const& expression) {
	std::regex tokenRegex(R"([[:digit:]]+(\.[[:digit:]]+)?|[a-zA-Z]+|\+|\-|\*|\/|\(|\))");
	std::vector<String> tokens;

	auto words_begin = std::sregex_iterator(expression.begin(), expression.end(), tokenRegex);
	auto words_end = std::sregex_iterator();

	for (std::sregex_iterator i = words_begin; i != words_end; ++i) {
		tokens.push_back((*i).str());
	}

	return tokens;
}

// https://en.wikipedia.org/wiki/Shunting_yard_algorithm
void sort_infix_to_postfix(std::vector<String>& tokens)
{
	// copy over to new list
	std::vector<String> unsortedTokens(tokens);

	// clear output
	tokens.clear();

	// temp stack of operators
	std::vector<String> operators;

	String token;
	float value;
	for(auto const& token : unsortedTokens)
	{
		if (is_number(token) || try_get_constant(token, value))
		{
			// push value to tokens
			tokens.push_back(token);
		}
		else if (int tokenPrecedence = operator_precedence(token))
		{
			if (!operators.empty())
			{
				int operatorPrecedence = operator_precedence(operators.back());
				while (!operators.empty() && operators.back() != "(" && (operatorPrecedence > tokenPrecedence || (operatorPrecedence == tokenPrecedence && token != "^")))
				{
					tokens.push_back(operators.back());
					operators.pop_back();
				}
			}
			operators.push_back(token);
		}
		else if (token == ",")
		{
			while (operators.back() != "(")
			{
				tokens.push_back(operators.back());
				operators.pop_back();
			}
		}
		else if (token == "(")
		{
			operators.push_back(token);
		}
		else if (token == ")")
		{
			while (!operators.empty() && operators.back() != "(")
			{
				tokens.push_back(operators.back());
				operators.pop_back();
			}

			MINTY_ASSERT_MESSAGE(!operators.empty(), "Mismatch parenthesis.");

			operators.pop_back();

			if (!operators.empty() && is_function(operators.back()))
			{
				tokens.push_back(operators.back());
				operators.pop_back();
			}
		}
		else
		{
			MINTY_ABORT(std::format("Invalid token: {}", token));
		}
	}

	while (!operators.empty())
	{
		MINTY_ASSERT_MESSAGE(operators.back() != "(", "Mismatch parenthesis.");

		tokens.push_back(operators.back());
		operators.pop_back();
	}
}

float minty::Math::evaluate(String const& expression)
{
	// get tokens
	std::vector<String> tokens = split_into_tokens(expression);

	// sort into postfix
	sort_infix_to_postfix(tokens);

	// evaluate
	std::vector<float> stack;

	float left, right;
	for (String const& token : tokens)
	{
		if (Parse::try_float(token, left) || try_get_constant(token, left))
		{
			// operand, push value onto stack
			stack.push_back(left);
		}
		else if (operator_precedence(token))
		{
			right = stack.back();
			stack.pop_back();

			left = stack.back();
			stack.pop_back();

			// operator
			if (token == "^")
			{
				stack.push_back(pow(left, right));
			}
			else if (token == "*")
			{
				stack.push_back(left * right);
			}
			else if (token == "/")
			{
				MINTY_ASSERT_MESSAGE(right != 0.0f, "Attempt to divide by zero.");
				stack.push_back(left / right);
			}
			else if (token == "+")
			{
				stack.push_back(left + right);
			}
			else if (token == "-")
			{
				stack.push_back(left - right);
			}
			else
			{
				MINTY_ABORT(std::format("Invalid operator: {}", token));
			}
		}
		else if (is_function(token))
		{
			
		}
		else
		{
			// something else
			MINTY_ABORT(std::format("Invalid token: {}", token));
		}
	}

	MINTY_ASSERT_FORMAT(stack.size() == 1, "Invalid evaluation, resulted with {} values. (\"{}\").", stack.size(), expression);

	// last value left on stack should be the result
	return stack.back();
}
