#include "pch.h"
#include "Math.h"

#include "Minty/Core/Console.h"
#include "Minty/Core/Parse.h"
#include "Minty/Core/String.h"
#include "Minty/Core/Math.h"
#include <cmath>
#include <vector>
#include <regex>
#include <unordered_map>
#include <unordered_set>
#include <algorithm>

using namespace Minty;

Bool is_number(String const& str)
{
	Bool decimalPointFound = false;
	for (Char c : str) {
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

Bool try_get_constant(String const& str, float& value)
{
	static std::unordered_map<String, float> const constants =
	{
		{"PI", Minty::Math::PI},
		{"FLOAT", static_cast<float>(sizeof(float))},
		{"DOUBLE", static_cast<float>(sizeof(Double))},
		{"INT", static_cast<float>(sizeof(Int))},
		{"SIZE_T", static_cast<float>(sizeof(Size))},
		{"BYTE", static_cast<float>(sizeof(Byte))},
		{"CHAR", static_cast<float>(sizeof(Char))},
		{"SHORT", static_cast<float>(sizeof(Short))},
		{"LONG", static_cast<float>(sizeof(Long))},
		{"FLOAT2", static_cast<float>(sizeof(Minty::Float2))},
		{"FLOAT3", static_cast<float>(sizeof(Minty::Float3))},
		{"FLOAT4", static_cast<float>(sizeof(Minty::Float4))},
		{"INT2", static_cast<float>(sizeof(Minty::Int2))},
		{"INT3", static_cast<float>(sizeof(Minty::Int3))},
		{"INT4", static_cast<float>(sizeof(Minty::Int4))},
		{"MATRIX2", static_cast<float>(sizeof(Minty::Matrix2))},
		{"MATRIX3", static_cast<float>(sizeof(Minty::Matrix3))},
		{"MATRIX4", static_cast<float>(sizeof(Minty::Matrix4))},
		{"QUATERNION", static_cast<float>(sizeof(Minty::Quaternion))},
		//{"", static_cast<float>(sizeof())},
	};

	auto found = constants.find(to_upper(str));
	if (found == constants.end())
	{
		return false;
	}

	value = found->second;
	return true;
}

Int is_function(String const& str)
{
	static std::unordered_map<String, Int> const functionNames =
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
Int operator_precedence(String const& str)
{
	static std::unordered_map<String, Int> const precedence =
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
	for (auto const& token : unsortedTokens)
	{
		if (is_number(token) || try_get_constant(token, value))
		{
			// push value to tokens
			tokens.push_back(token);
		}
		else if (Int tokenPrecedence = operator_precedence(token))
		{
			if (!operators.empty())
			{
				Int operatorPrecedence = operator_precedence(operators.back());
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

float Minty::Math::evaluate(String const& expression)
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

Float3 Minty::forward(Quaternion const& value)
{
	return glm::normalize(value * Float3(0.0f, 0.0f, 1.0f));
}

Float3 Minty::backward(Quaternion const& value)
{
	return glm::normalize(value * Float3(0.0f, 0.0f, -1.0f));
}

Float3 Minty::right(Quaternion const& value)
{
	return glm::normalize(value * Float3(1.0f, 0.0f, 0.0f));
}

Float3 Minty::left(Quaternion const& value)
{
	return glm::normalize(value * Float3(-1.0f, 0.0f, 0.0f));
}

Float3 Minty::up(Quaternion const& value)
{
	return glm::normalize(value * Float3(0.0f, -1.0f, 0.0f));
}

Float3 Minty::down(Quaternion const& value)
{
	return glm::normalize(value * Float3(0.0f, 1.0f, 0.0f));
}

float fix_angle(float angle)
{
	if (angle == 0.0f)
	{
		// fix negative zero
		return 0.0f;
	}
	//else if(angle < 0.0f)
	//{
	//    // fix negative angle
	//    return angle += 360.0f;
	//}
	else
	{
		// nothing to fix
		return angle;
	}
}

Float3 Minty::to_euler(Quaternion const& value)
{
	Float3 euler = glm::eulerAngles(value) * Math::RAD2DEG;
	euler = Float3(fix_angle(euler.x), fix_angle(euler.y), fix_angle(euler.z));

	return euler;
}

Quaternion Minty::from_euler(Float3 const& value)
{
	return Quaternion(value * Math::DEG2RAD);
}

Float3 Minty::matrix4_get_position(Matrix4 const& value)
{
	return value[3];
}

Quaternion Minty::matrix4_get_rotation(Matrix4 const& value)
{
	return matrix4_get_rotation(value, matrix4_get_scale(value));
}

Quaternion Minty::matrix4_get_rotation(Matrix4 const& value, Float3 const scale)
{
	Matrix3 rotationMatrix{};
	if (scale.x == 0.0f)
	{
		rotationMatrix[0] = value[0];
	}
	else
	{
		rotationMatrix[0] = value[0] / scale.x;
	}
	if (scale.y == 0.0f)
	{
		rotationMatrix[1] = value[1];
	}
	else
	{
		rotationMatrix[1] = value[1] / scale.y;
	}
	if (scale.z == 0.0f)
	{
		rotationMatrix[2] = value[2];
	}
	else
	{
		rotationMatrix[2] = value[2] / scale.z;
	}

	return glm::quat_cast(rotationMatrix);
}

Float3 Minty::matrix4_get_scale(Matrix4 const& value)
{
	return Float3(glm::length(value[0]), glm::length(value[1]), glm::length(value[2]));
}

void Minty::matrix4_set_position(Matrix4& value, Float3 const position)
{
	value[3][0] = position.x;
	value[3][1] = position.y;
	value[3][2] = position.z;
}

void Minty::matrix4_set_rotation(Matrix4& value, Quaternion const position)
{
	MINTY_NOT_IMPLEMENTED();
}

void Minty::matrix4_set_scale(Matrix4& value, Float3 const position)
{
	MINTY_NOT_IMPLEMENTED();
}