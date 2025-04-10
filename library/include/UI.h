#ifndef UI_H
#define UI_H

#include <string>
#include <vector>
#include <map>
#include <functional>
#include <iostream>
#include <sstream>

namespace UI
{
	/// Visual divider used in printed console messages.
	static const std::string DIVIDER = "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~";

	/// String representations of the input types (used in error messages).
	static const std::string IT_ENUM_STRING[] =
	{
		"Digit",
		"Character",
		"String"
	};

	/// Enum to represent different input types.
	enum INPUT_TYPE 
	{
		/// Default
		DEF,

		/// Digit
		D,
		
		/// Character
		C,

		/// String
		S
	};

	/// Basic question structure for console interaction.
	struct Question
	{
	public:

		/// The question text to display.
		std::string contents;

		/// Valid responses.
		std::vector<std::string> answers{};

		/// Expected input type.
		INPUT_TYPE type;
	};
	

	/// A question that has associated actions with each possible answer.
	struct ActionQuestion : public Question
	{
	public:
		std::map<std::string, std::function<void(const std::string&)>> actions; ///< Action callbacks tied to inputs.
	};

	/// Console utility for printing messages and handling input.
	class Console {
	private:

		/// Validates digit input using `std::stoi`.
		/// @param input The input string.
		/// @returns True if input is a valid digit.
		static bool valid_digit(const std::string& input)
		{
			try {
				(void)std::stoi(input);
				return true;
			}
			catch (const std::exception&) {
				return false;
			}
		}

		/// Validates character input.
		/// @param input The input string.
		/// @returns True if a single character (always true in this context).
		static bool valid_char(const std::string& input)
		{
			return input.length() == 1; // Accepts any single character input
		}

		/// Validates input based on type.
		/// @param type Expected input type.
		/// @param question The full question string to redisplay on invalid input.
		/// @returns A pair of (is_valid, input_value).
		static std::pair<bool, std::string> valid_input(const INPUT_TYPE& type, const std::string& question)
		{
			std::string input;
			bool validated = false;

			while (!validated)
			{
				std::cout << "INPUT: ";
				std::cin >> input;

				switch (type)
				{
				case D: validated = valid_digit(input); break;
				case C: validated = valid_char(input); break;
				case S: validated = true; break;
				default: validated = false; break;
				}

				if (!validated)
				{
					[[maybe_unused]] int ignored = std::system("clear");
					print_message(question);
					std::string invalid_message = "INVALID INPUT: " + input + " TYPE: " + IT_ENUM_STRING[type];
					print_message(invalid_message);
				}
			}

			return { validated, input };
		}

	public:

		/// Prints a stylized console message with borders.
		/// @param message The message to display.
		static void print_message(const std::string& message) {
			std::vector<std::string> lines;
			std::istringstream stream(message);
			std::string line;
			size_t max_length = 0;

			while (std::getline(stream, line)) {
				lines.push_back(line);
				max_length = std::max(max_length, line.length());
			}

			std::string border(max_length + 4, '-');
			std::cout << border << "\n";
			for (const auto& l : lines) {
				std::cout << "| " << l << std::string(max_length - l.length(), ' ') << " |\n";
			}
			std::cout << border << "\n";
		}

		/// Overload for printing C-style strings.
		/// @param message C-string message.
		static void print_message(const char* message)
		{
			print_message(std::string(message));
		}

		/// Displays a question and gets validated input.
		/// @param question The question to display.
		/// @returns Pair of (is_valid_response, user_input).
		static std::pair<bool, std::string> print_question(const Question& question)
		{
			print_message(question.contents);
			std::string input = valid_input(question.type, question.contents).second;

			bool input_found = false;
			for (const std::string& answer : question.answers)
			{
				if (answer == input)
				{
					input_found = true;
					break;
				}
			}

			return { input_found, input };
		}

		/// Displays an ActionQuestion and triggers actions based on input.
		/// @param question The action-enabled question to display.
		/// @returns Pair of (is_valid_response, user_input).
		static std::pair<bool, std::string> print_question(const ActionQuestion& question)
		{
			print_message(question.contents);
			std::string input = valid_input(question.type, question.contents).second;

			bool input_found = false;

			for (const std::string& answer : question.answers)
			{
				if (answer == input)
				{
					input_found = true;

					auto action_it = question.actions.find(input);
					if (action_it != question.actions.end())
					{
						action_it->second(input);
					}

					break;
				}
			}

			return { input_found, input };
		}
	};
}

#endif // !UI_H

