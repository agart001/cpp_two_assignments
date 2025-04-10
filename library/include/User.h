
#ifndef USER_H
#define USER_H

#include <string>
#include <vector>
#include <sstream>

#include "json.hpp"
#include "hash_sha256.h"
#include "LibTypes.h"
#include "UI.h"

namespace std {
	std::array<std::uint8_t, 20U> stobya(const std::string& str)
	{
		std::array<std::uint8_t, 20U> arr{};

		if (str.length() > 20)
		{
			std::cerr << "std::string length greater than 20: " << str << std::endl;
			return arr;
		}

		for (size_t i = 0; i < str.length(); i++)
		{
			arr[i] = str[i];
		}

		return arr;
	}
}

/// Represents a user of the library system.
/// Contains personal credentials and a list of borrowed books.
struct User {
public:
	/// The user's name (used for login).
	std::string name;

	/// The user's hashed password (SHA256).
	sha256_type password{};

	/// The books currently associated with this user.
	std::vector<LibraryTypes::Book> books;

	/// Default constructor.
	User() = default;

	/// Constructor with name and password.
	/// @param name The username.
	/// @param password The hashed password.
	User(std::string name, sha256_type password)
	{
		this->name = name;
		this->password = password;
	}

	/// Constructor with name, password, and book list.
	/// @param name The username.
	/// @param password The hashed password.
	/// @param books A list of the user's books.
	User(std::string name, sha256_type password, std::vector<LibraryTypes::Book> books)
	{
		this->name = name;
		this->password = password;
		this->books = books;
	}

	~User() { }

	/// Equality operator.
	/// Users are equal if their name, password, and books match.
	/// @param other The other user to compare with.
	/// @returns True if equal.
	bool operator==(const User& other) const {
		return name == other.name && password == other.password && books == other.books;
	}

	/// Checks if the user is uninitialized (null).
	/// @returns True if the name is empty.
	bool isNULL()
	{
        return name.empty() == true;
	}

	/// Converts the user to a printable string.
	/// @returns String representation of the user.
	std::string ToString() const {
		std::ostringstream oss;
		oss << "Name: " << this->name << " | Books: " << this->books.size();
		return oss.str();
	}
};

/// Serializes a User into JSON.
/// @param j The JSON object to populate.
/// @param user The user to serialize.
inline void to_json(nlohmann::json& j, const User& user) {
	j = {
		{"name", user.name},
		{"password", user.password},
		{"books", user.books},
	};
}

/// Deserializes a User from JSON.
/// @param j The JSON object to read from.
/// @param user The user object to populate.
inline void from_json(const nlohmann::json& j, User& user) {
	user.name = j.at("name").get<std::string>();
	user.password = j.at("password").get<sha256_type>();
	user.books = j.at("books").get<std::vector<LibraryTypes::Book>>();
}


namespace std {
	template<>
	struct hash<User> {
		size_t operator()(const User& u) const {
			size_t h = std::hash<std::string>{}(u.name);

			// hash the password
			for (auto b : u.password) {
				h ^= std::hash<uint8_t>{}(b)+0x9e3779b9 + (h << 6) + (h >> 2);
			}

			// hash books
			for (const auto& book : u.books) {
				h ^= std::hash<std::string>{}(book.title);
				h ^= std::hash<std::string>{}(book.author);
			}

			return h;
		}
	};
}

/// A class that manages Users.
/// Provides functionality to authenticate, add, remove,
/// and persist users across sessions.
class UserManager
{
private:
	/// List of all users.
	std::vector<User> users;

	/// Map of users to their index in the `users` vector.
	std::unordered_map<User, size_t> users_map;

	/// Utility object for generating SHA256 hashes.
	hash_sha256 hash;

	/// @brief Rebuilds the internal user index map.
	/// Called after any change to the user list.
	void re_index()
	{
		users_map.clear();

		for (size_t i = 0; i < users.size(); i++) 
		{
			users_map[users[i]] = i;
		}
	}

public:
	/// The currently signed-in user.
	User current_user;

	/// Default constructor.
	/// Loads all users from disk and re-indexes them.
	UserManager()
	{
		if (!load()) 
		{
			return;
		}
		re_index();
	}

	~UserManager() { }

	/// @brief Prompts the user to sign in.
	/// Repeats on failure.
	void signin()
	{
		std::string name;
		std::string pass;

		[[maybe_unused]] int ignored = std::system("clear");
		std::ostringstream oss;
		oss << "CPPII | Assignment 2 | Library: #\n" << UI::DIVIDER << "\nSign In\nPlease enter your name & password";
		std::string f_contents = oss.str();
		UI::Console::print_message(f_contents);

		std::cout << "Name: ";
		std::cin >> name;
		std::cout << "Password: ";
		std::cin >> pass;

		auto bytes = std::stobya(pass);
		hash.sha256_init();
		hash.sha256_update(bytes.data(), bytes.size());
		sha256_type password = hash.sha256_final();

		bool user_found = false;
		for (User u : users) 
		{
			if (u.name == name && u.password == password) 
			{
				current_user = u;
				user_found = true;
			}
		}

		if (!user_found) 
		{
			signin();
		}
	}

	/// @brief Prompts the user to sign up by creating a new user.
	void signup()
	{
		[[maybe_unused]] int ignored = std::system("clear");
		std::ostringstream oss;
		oss << "CPPII | Assignment 2 | Library: #\n" << UI::DIVIDER << "\nSign Up\nPlease enter your name & password";
		std::string f_contents = oss.str();
		UI::Console::print_message(f_contents);

		User u = input_user();
	}

	/// @brief Prompts input for creating a new user.
	/// Hashes the password, adds user to the list.
	/// @returns The created User.
	User input_user()
	{
		std::string name;
		std::string pass;

		std::cout << "Name: ";
		std::cin >> name;
		std::cout << "Password: ";
		std::cin >> pass;

		auto bytes = std::stobya(pass);
		hash.sha256_init();
		hash.sha256_update(bytes.data(), bytes.size());
		sha256_type password = hash.sha256_final();

		User user = User(name, password);

		add(user);

		return user;
	}

	/// @brief Entry point for user login or signup.
	/// Displays prompt to choose action.
	void login()
	{
		std::ostringstream oss;
		oss << "CPPII | Assignment 2 | Library: #\n" << UI::DIVIDER << "\nDo you have an account?\nDo you need to signin or signup?\n1) signin\n2) signup";
		std::string f_contents = oss.str();
		UI::ActionQuestion question;
		question.contents = f_contents;
		question.answers = { "1", "2" };
		question.type = UI::INPUT_TYPE::D;
		question.actions = 
		{
			{"1", [this](const std::string&) { this->signin(); }},
			{"2", [this](const std::string&) { this->signup(); }}
		};

		std::pair<bool, std::string> res = UI::Console::print_question(question);

		if (!res.first) 
		{
			std::cout << "Unknown command.\n";
		}
	}

	/// @brief Adds a new user to the system.
	/// @param user The user to add.
	void add(const User& user)
	{
		users.push_back(user);
		size_t index = users.size() - 1;
		users_map[user] = index;

		save();
	}

	/// @brief Removes a user from the system.
	/// @param user The user to remove.
	void remove(const User& user)
	{
		size_t index = users_map[user];
		users.erase(users.begin() + index);
		re_index();

		save();
	}

	/// @brief Adds a book to the currently signed-in user.
	/// @param book The book to add.
	void add(const LibraryTypes::Book& book)
	{
		current_user.books.push_back(book);
		size_t index = users_map[current_user];
		users.at(index) = current_user;
		save();
	}

	/// @brief Removes a book from the current user by index.
	/// @param index The index of the book to remove.
	/// @returns Always returns true.
	bool remove(int index)
	{
		current_user.books.erase(current_user.books.begin() + index);
		size_t pos = users_map[current_user];
		users.at(pos) = current_user;
		save();

		return true;
	}

	/// @brief Gets a user at the specified index.
	/// @param index The index of the user.
	/// @returns The user at the given index.
	User at(size_t index)
	{
		return users.at(index);
	}

	/// @brief Returns the number of users.
	size_t size() const
	{
		return users.size();
	}

	/// @brief Saves all users to a JSON file on disk.
	void save()
	{
		std::filesystem::path users_path = std::filesystem::current_path() / "data" / "library_users.json";

		nlohmann::json users_j = users;
		std::ofstream o(users_path);
		o << std::setw(4) << users_j << std::endl;
		o.close();
	}

	/// @brief Loads users from disk.
	/// @returns True if successful, false otherwise.
	bool load()
	{
		std::filesystem::path users_path = std::filesystem::current_path() / "data" / "library_users.json";

		if (!std::filesystem::exists(users_path)) 
		{
			return false;
		}

		std::ifstream i(users_path);
		if (!i.is_open()) 
		{
			return false;
		}

		nlohmann::json j;
		i >> j;
		i.close();

		users = j;

		return true;
	}

};


#endif // !USER_H
