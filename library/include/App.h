
#ifndef APP_H
#define APP_H

#include "User.h"
#include "LibTypes.h"
#include "UI.h"

namespace std {
	std::vector<std::string> gendsv(int digit)
	{
		std::vector<std::string> res;

		for (int i = 0; i < digit; i++)
		{
			res.push_back(std::to_string(i));
		}

		return res;
	}
}

/// @brief A class acting as a Library & UserManager view model.
/// Handles the user input and holds the backend classes that process
/// that input.
class LibraryApp
{
private:

	/// UserManager of the app. Loads from "library_users.json".
	UserManager UM;

	/// Library of the app. Loads from "library_books.json".
	LibraryTypes::Library LIB;

public:

	/// @brief Handles the App's start.
	/// Prompts the user to login and 
	/// the transitions to the App's main.
	void start()
	{
		[[maybe_unused]] bool ignored;
		ignored = this->UM.load();
		ignored = this->LIB.load();

		bool check = UM.current_user.isNULL();
		if (check)
		{
			//login the user
			UM.login();
		}

		//Shift to the app main once the user is logged in
		main();
	}

	/// @brief Handles the App's main menu.
	/// Transitions the user to the UserManager's
	/// and Library's menus 
	void main()
	{
		[[maybe_unused]] int ignored = std::system("clear");

		//@brief Standards UI Action Question Layout
		//This is the standard layout of UI actions with the Library App
		UI::ActionQuestion question;
        
		// This is where all the actual UI prompt
        std::stringstream ss;
        ss << header()
            << "What would you like to do?\n"
            << "1) UM Main\n"
            << "2) LIB Main\n"
            << "3) Exit & Save";
        question.contents = ss.str();

		// The answers to the question, in string format regardless
		question.answers = { "1", "2", "3"};

		// The input type, in this case 'digit'
		question.type = UI::INPUT_TYPE::D;

		// The actions that will be executed based on the user's input
		question.actions = {

			// Navigates to the UserManager main menu
			{"1", [this](const std::string&) { this->um_main_menu(); }},

			// Navigates to the Library main menu
			{"2", [this](const std::string&) { this->lib_main_menu(); }},

			// App Exit
			{"3", [this](const std::string&) { this->exit(); }}
		};

		// Executes/prints the question to the console
		UI::Console::print_question(question);
	}

	/// @brief Handles the App's exit
	/// Saves the UserManager's & Library's
	/// content's as files then exits the console.
	void exit()
	{
		this->UM.save();
		this->LIB.save();
		std::exit(0);
	}

	/// @brief Creates the App's header.
	/// @return The App's string header.
	std::string header()
	{
		std::stringstream ss;
		ss << "CPPII | Assignment 2 | Library: " << this->LIB.size() << "\n"
		   << UI::DIVIDER << "\n"
		   << this->UM.current_user.ToString() << "\n"
		   << UI::DIVIDER << "\n";
		return ss.str();
	}

	/// @brief Prints a vector of books to the console.
	void print_books(std::vector<LibraryTypes::Book> books)
	{
		[[maybe_unused]] int ignored = std::system("clear");

		if (books.empty())
		{
			UI::Console::print_message("No Books in list!!!");
		}
		else
		{
			for (size_t i = 0; i < books.size(); i++)
			{
				LibraryTypes::Book book = books[i];

                std::stringstream message;
                message << "Index #" << i << "\n"
                        << UI::DIVIDER << "\n"
                        << book.ToString();

                UI::Console::print_message(message.str());
            }
		}
	}

	
	/// @brief Prints all the UserManager's users to the console.
	void print_users()
	{
		size_t len = this->UM.size();
		for (size_t i = 0; i < len; i++)
		{
			User user = this->UM.at(i);
            std::stringstream message;
            message << "Index #" << i << "\n"
                    << UI::DIVIDER << "\n"
                    << user.ToString();

			UI::Console::print_message(message.str());
		}
	}
	
	/// @brief Handles the UserManager Main Menu UI.
	/// This method handles the UserManager's main menu, allowing the user to
	/// navigate to:
	/// - App Main Menu
	/// - Add & Remove
	/// - Exit & Save
	void um_main_menu()
	{
		[[maybe_unused]] int ignored = std::system("clear");

		UI::ActionQuestion question;
        std::stringstream ss;
        ss << header()
           << "What would you like to do?\n"
           << "1) Add & Remove\n"
           << "2) App Main\n"
           << "3) Exit & Save";
        question.contents = ss.str();
		question.answers = { "1", "2", "3"};
		question.type = UI::INPUT_TYPE::D;
		question.actions = {

		// Navigates to the UserManager Add & Remove Menu
		{"1", [this](const std::string&) { this->um_add_rem_menu(); }},

		// Navigates to the App's Main Menu
		{"2", [this](const std::string&) { this->main(); }},

		// Exit & Save
		{"3", [this](const std::string&) { this->exit(); }}

		};

		UI::Console::print_question(question);
	}

	/// @brief Handles the UserManager's Reset Menu UI.
	/// This method handles the reset of the UserManager menu,
	/// allowing the user to navigate to:
	/// - UserManager's Main Menu
	/// - App Main Menu
	/// - Exit & Save
	void um_reset_menu()
	{
		UI::ActionQuestion question;
        std::stringstream ss;
        ss << header()
            << "What would you like to do?\n"
            << "1) UserManager Main\n"
            << "2) App Main\n"
            << "3) Exit & Save";
        question.contents = ss.str();
		question.answers = { "1", "2", "3"};
		question.type = UI::INPUT_TYPE::D;
		question.actions = {

		// Navigates to the UserManager Main Menu
		{"1", [this](const std::string&) { this->um_main_menu(); }},

		// Navigates to the App Main Menu
		{"2", [this](const std::string&) { this->main(); }},

		// Exit & Save
		{"3", [this](const std::string&) { this->exit(); }}

		};

		UI::Console::print_question(question);
	}

	/// @brief Handles the UserManager's Add & Remove Menu UI.
	/// This method handles the adding & removing UserManager menu,
	/// allowing the user to navigate to:
	/// - Add Prompt
	/// - Remove Prompt
	/// - Exit & Save
	void um_add_rem_menu()
	{
		/// @brief Handles the UserManager's Add Prompt.
		std::pair<std::string, std::function<void(const std::string&)>> 
			add =
		{ 
			// Input
			"1", 

			// Function
			[this](const std::string&)
			{
				[[maybe_unused]] int ignored = std::system("clear");
                
                std::stringstream message;
                message << header()
                        << "Adding a user requires a name & a password";

				UI::Console::print_message(message.str());
				User added = this->UM.input_user();

				ignored = std::system("clear");

                message.str("");
                message << header()
                        << "Added user:\n"
                        << UI::DIVIDER << "\n"
                        << added.ToString();
                UI::Console::print_message(message.str());

				this->um_reset_menu();
			}
		};

		/// @brief Handles the UserManager's Remove Prompt.
		std::pair<std::string, std::function<void(const std::string&)>> 
			remove =
		{
			// Input
			"2",

			// Function
			[this](const std::string&)
			{
				[[maybe_unused]] int ignored = std::system("clear");
				this->print_users();

				UI::Question question;
                std::stringstream ss;
                ss << header()
                   << "Removing a user requires the index # of the user to remove";
                question.contents = ss.str();
				question.answers = std::gendsv(this->UM.size());
				question.type = UI::INPUT_TYPE::D;

				auto res = UI::Console::print_question(question);

				if (!res.first)
				{
					return;
				}

				int cast = std::stoi(res.second);

				size_t index = static_cast<size_t>(cast);

				User removed = this->UM.at(index);

				this->UM.remove(removed);

				ignored = std::system("clear");

                std::stringstream message;
                message << header()
                        << "Removed user:\n"
                        << UI::DIVIDER << "\n"
                        << removed.ToString();
                UI::Console::print_message(message.str());

				this->um_reset_menu();
			} 
		};

		[[maybe_unused]] int ignored = std::system("clear");

		UI::ActionQuestion question;
        std::stringstream ss;
        ss << header()
           << "Adding & Removing a user requires the index # of the user to remove";
        question.contents = ss.str();
		question.answers = { "1", "2", };
		question.type = UI::INPUT_TYPE::D;
		question.actions = {

		// Handles the Add Prompt
		add,

		// Handles the Remove Prompt
		remove,

		// Exit & Save
		{"3", [this](const std::string&) { this->exit(); }},

		};

		UI::Console::print_question(question);
	}

	/// @brief Handles the Library's Main Menu UI.
	/// This method handles the Library's main menu, allowing the user to
	/// navigate to:
	/// - Inventory
	/// - Add & Remove
	/// - Borrow & Return
	/// - App Main
	/// - Exit & Save
	void lib_main_menu()
	{
		[[maybe_unused]] int ignored = std::system("clear");

		UI::ActionQuestion question;
        std::stringstream ss;
        ss << header()
           << "What would you like to do?\n"
           << "1) Inventory\n"
           << "2) Add & Remove\n"
           << "3) Borrow & Return\n"
           << "4) App Main\n"
           << "5) Exit & Save";
        question.contents = ss.str();
		question.answers = { "1", "2", "3", "4", "5"};
		question.type = UI::INPUT_TYPE::D;
		question.actions = {

		// Navigates to the Library's Inventory Menu
		{"1", [this](const std::string&) { this->lib_inv_menu(); }},

		// Navigates to the Library's Add & Remove Menu
		{"2", [this](const std::string&) { this->lib_add_rem_menu(); }},

		// Navigates to the Library's Borrow & Return Menu
		{"3", [this](const std::string&) { this->lib_bor_ret_menu(); }},

		// Navigates to the App Main Menu
		{"4", [this](const std::string&) { this->main(); }},

		// Exit & Save
		{"5", [this](const std::string&) { this->exit(); }},
		};

		UI::Console::print_question(question);
	}

	/// @brief Handles the Library's Reset Menu UI.
	/// This method handles the reset of the Library menu,
	/// allowing the user to navigate to:
	/// - Library's Main Menu
	/// - App Main Menu
	/// - Exit & Save
	void lib_reset_menu()
	{
		UI::ActionQuestion question;
        std::stringstream ss;
        ss << header()
           << "What would you like to do?\n"
           << "1) Main Menu\n"
           << "2) App Main\n"
           << "3) Exit & Save";
        question.contents = ss.str();
		question.answers = { "1", "2" };
		question.type = UI::INPUT_TYPE::D;
		question.actions = {

		// Navigates to the Library's Main Menu
		{"1", [this](const std::string&) { this->lib_main_menu(); }},

		// Navigates to the App Main Menu
		{"2", [this](const std::string&) { this->main(); }},

		// Exit & Save
		{"3", [this](const std::string&) { this->exit(); }},
		};

		UI::Console::print_question(question);
	}

	/// @brief Handles the Library's Inventory Menu UI.
	/// This method handles the Library's inventory menu, allowing the user to
	/// navigate to:
	/// - View All Prompt
	/// - Search Prompt
	/// - Library's Main Menu
	/// - Exit & Save
	void lib_inv_menu()
	{
		/// @brief Handles the Library's View All Prompt.
		std::pair<std::string, std::function<void(const std::string&)>>
			viewall =
		{ 
			// Input
			"1", 

			// Function
			[this](const std::string&)
			{
				this->print_books(this->LIB.books);
				this->lib_reset_menu();
			} 
		};

		/// @brief Handles the Library's Search Prompt.
		std::pair<std::string, std::function<void(const std::string&)>>
			search =
		{
			// Input
			"2",

			// Function
			[this](const std::string&)
			{
				this->search_menu();

				if (this->LIB.search_res.empty())
				{
					this->lib_reset_menu();
				}

				this->print_books(this->LIB.search_res);
				this->lib_reset_menu();
			}
		};
	
		[[maybe_unused]] int ignored = std::system("clear");

		UI::ActionQuestion question;
        std::stringstream ss;
        ss << header()
           << "What would you like to do?\n"
           << "1) View All\n"
           << "2) Search\n"
           << "3) Main Menu\n"
           << "4) Exit & Save";
        question.contents = ss.str();
		question.answers = { "1", "2", "3", "4" };
		question.type = UI::INPUT_TYPE::D;
		question.actions = {

		// Handles the View All Prompt.
		viewall,

		// Handles the Search Prompt.
		search,

		// Navigates to the Library's Main Menu.
		{"3", [this](const std::string&) { this->lib_main_menu(); }},

		// Exit & Save
		{"4", [this](const std::string&) { this->exit(); }},

		};

		UI::Console::print_question(question);
	}

	void search_menu()
	{

		/// @brief Handles the Title Search Prompt.
		std::pair<std::string, std::function<void(const std::string&)>>
			title_search =
		{
			// Input
			"1",

			// Function
			[this](const std::string&)
			{
				[[maybe_unused]] int ignored = std::system("clear");

				std::string title;

				UI::Console::print_message("Enter the title of your book");

				std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
				std::cout << "NAME: ";
				std::getline(std::cin, title);

				this->LIB.search_res = this->LIB.search(title, LibraryTypes::SEARCH::TITLE);


				this->print_books(this->LIB.search_res);
			} 
		};

		/// @brief Handles the Author Search Prompt.
		std::pair<std::string, std::function<void(const std::string&)>>
			author_search =
		{
			// Input
			"2",

			// Function
			[this](const std::string&)
			{
				[[maybe_unused]] int ignored = std::system("clear");

				std::string author;

				UI::Console::print_message("Enter the author of your book");

				std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
				std::cout << "AUTHOR: ";
				std::getline(std::cin, author);

				this->LIB.search_res = this->LIB.search(author, LibraryTypes::SEARCH::AUTHOR);

				this->print_books(this->LIB.search_res);
			} 
		};

		/// @brief Handles the ISBN Search Prompt.
		std::pair<std::string, std::function<void(const std::string&)>>
			isbn_search =
		{ 
			// Input
			"3", 

			// Function
			[this](const std::string&)
			{
				[[maybe_unused]] int ignored = std::system("clear");

				std::string isbn;

				UI::Console::print_message("Enter the ISBN of your book");

				std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
				std::cout << "ISBN: ";
				std::getline(std::cin, isbn);

				this->LIB.search_res = this->LIB.search(isbn, LibraryTypes::SEARCH::CODE);

				this->print_books(this->LIB.search_res);
			} 
		};

		[[maybe_unused]] int ignored = std::system("clear");

		UI::ActionQuestion question;
        std::stringstream ss;
        ss << header()
           << "How would you like to search?\n"
           << "1) Name\n"
           << "2) Author\n"
           << "3) ISBN";
        question.contents = ss.str();
		question.answers = { "1", "2", "3" };
		question.type = UI::INPUT_TYPE::D;
		question.actions = {

		// Handles the Title Search Prompt.
		title_search,

		// Handles the Author Search Prompt.
		author_search,

		// Handles the ISBN Search Prompt.
		isbn_search,

		};

		UI::Console::print_question(question);
	}

	void lib_add_rem_menu()
	{
		/// @brief Handles the Library's Add Prompt.
		std::pair<std::string, std::function<void(const std::string&)>>
			add =
		{ 
			// Input 
			"1", 

			// Function
			[this](const std::string&)
			{
				[[maybe_unused]] int ignored = std::system("clear");

				std::string name, author, isbn;
                
                std::stringstream message;
                message << header()
                        << "Adding a book requires a name, an author,\n"
                        << "and a ISBN. If you do not know the ISBN\n"
                        << "enter 'new' and one will be generated";
                UI::Console::print_message(message.str());

				std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

				std::cout << "Name: ";
				std::getline(std::cin, name);

				std::cout << "Author: ";
				std::getline(std::cin, author);

				std::cout << "ISBN: ";
				std::getline(std::cin, isbn);

				LibraryTypes::Book book = LibraryTypes::Book(name, author, isbn);

				this->LIB.add(book);

				ignored = std::system("clear");

                message.str("");
                message << header()
                        << "Added book:\n"
                        << UI::DIVIDER << "\n"
                        << book.ToString();
                UI::Console::print_message(message.str());

				this->lib_reset_menu();
			} 
		};

		/// @brief Handles the Library's Remove Prompt.
		std::pair<std::string, std::function<void(const std::string&)>>
			remove =
		{ 
			// Input 
			"2", 

			// Function
			[this](const std::string&)
			{
				this->search_menu();

				if (this->LIB.search_res.empty())
				{
					this->lib_reset_menu();
				}

				UI::Question question;
                std::stringstream message;
                message << header()
                        << "Removing a book requires the index # of the book to remove";
                question.contents = message.str();
				question.answers = std::gendsv(this->LIB.search_res.size());
				question.type = UI::INPUT_TYPE::D;

				auto res = UI::Console::print_question(question);

				if (!res.first)
				{
					return;
				}

				int cast = std::stoi(res.second);

				size_t index = static_cast<size_t>(cast);

				LibraryTypes::Book removed = this->LIB.search_res.at(index);

				this->LIB.remove(removed);

				[[maybe_unused]] int ignored = std::system("clear");

                message.str("");
                message << header()
                        << "Removed book:\n"
                        << UI::DIVIDER << "\n"
                        << removed.ToString();
                UI::Console::print_message(message.str());

				this->lib_reset_menu();
			} 
		};

		[[maybe_unused]] int ignored = std::system("clear");

		UI::ActionQuestion question;
        std::stringstream ss;
        ss << header()
           << "What would you like to do?\n"
           << "1) Add Book\n"
           << "2) Remove Book\n"
           << "3) Main Menu\n"
           << "4) Exit & Save";
        question.contents = ss.str();
		question.answers = { "1", "2", "3", "4" };
		question.type = UI::INPUT_TYPE::D;
		question.actions = {

		// Handles the Add Prompt.
		add,

		// Handles the Remove Prompt.
		remove,

		// Navigates to the Library's Main Menu.
		{"3", [this](const std::string&) { this->lib_main_menu(); }},

		// Exit & Save.
		{"4", [this](const std::string&) { this->exit(); }},

		};

		UI::Console::print_question(question);
	}

	void lib_bor_ret_menu()
	{
		/// @brief Handles the Library's Borrow Prompt.
		std::pair<std::string, std::function<void(const std::string&)>>
			bor =
		{ 
			// Input
			"1", 

			// Function
			[this](const std::string&)
			{
				this->print_books(this->LIB.books);

				UI::Question question;
                std::stringstream message;
                message << header()
                        << "Borrowing a book requires the index # of the book to borrow";
                question.contents = message.str();
				question.answers = std::gendsv(this->LIB.books.size());
				question.type = UI::INPUT_TYPE::D;

				auto res = UI::Console::print_question(question);

				if (!res.first)
				{
					return;
				}

				int cast = std::stoi(res.second);

				size_t index = static_cast<size_t>(cast);

				LibraryTypes::Book checkout = this->LIB.books.at(index);

				this->LIB.remove(checkout);

				this->UM.add(checkout);

				[[maybe_unused]] int ignored = std::system("clear");

                message.str("");
                message << header()
                        << "Borrowed book:\n"
                        << UI::DIVIDER << "\n"
                        << checkout.ToString();
                UI::Console::print_message(message.str());

				this->lib_reset_menu();
			} 
		};

		/// @brief Handles the Library's Return Prompt.
		std::pair<std::string, std::function<void(const std::string&)>>
			ret =
		{
			// Input
			"2",

			// Function
			[this](const std::string&)
			{
				this->print_books(this->UM.current_user.books);

				UI::Question question;
                std::stringstream message;
                message << header()
                        << "Returning a book requires the index # of the book to return";
                question.contents = message.str();
				question.answers = std::gendsv(this->UM.current_user.books.size());
				question.type = UI::INPUT_TYPE::D;

				auto res = UI::Console::print_question(question);

				if (!res.first)
				{
					return;
				}

				int cast = std::stoi(res.second);

				size_t index = static_cast<size_t>(cast);

				LibraryTypes::Book checkin = this->UM.current_user.books.at(index);

				this->UM.remove(index);

				this->LIB.add(checkin);

				[[maybe_unused]] int ignored = std::system("clear");

                message.str("");
                message << header()
                        << "Returned book:\n"
                        << UI::DIVIDER << "\n"
                        << checkin.ToString();
                UI::Console::print_message(message.str());

				this->lib_reset_menu();
			} 
		};

		[[maybe_unused]] int ignored = std::system("clear");

		UI::ActionQuestion question;
        std::stringstream ss;
        ss << header()
           << "What would you like to do?\n"
           << "1) Borrow Book\n"
           << "2) Return Book\n"
           << "3) Main Menu\n"
           << "4) Exit & Save";
        question.contents = ss.str();
		question.answers = { "1", "2", "3", "4" };
		question.type = UI::INPUT_TYPE::D;
		question.actions = {

		// Handles the Borrow Prompt.
		bor,

		// Handles the Return Prompt.
		ret,

		// Navigates to the Library's Main Menu
		{"3", [this](const std::string&) { this->lib_main_menu(); }},

		// Exit & Save
		{"4", [this](const std::string&) { this->exit(); }},
		};

		UI::Console::print_question(question);
	}

};


#endif // !APP_H
