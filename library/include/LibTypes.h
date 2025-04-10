#ifndef LIBTYPES_H
#define LIBTYPES_H

#include "json.hpp"
#include <cstdlib>
#include <stdexcept>
#include <iostream>
#include <fstream>
#include <string>
#include <unordered_map>
#include <vector>
#include <filesystem>

namespace LibraryTypes
{
	/// Library Search Types
	enum SEARCH
	{
		/// Search by title
		TITLE,

		/// Search by author
		AUTHOR,

		/// Search by code
		CODE
	};

	/// A struct representing a ISBN.
	/// The ISBN struct is code to be the 13 digit format.
	/// It use a default book code '978'.
	struct ISBN 
	{
	private:

		/// @brief Calculates the check digit of the ISBN.
		/// @param The string reference of the first 10 digts.
		/// @returns The integer check digit
		int calcCheckDigit(const std::string& digits)
		{
			int sum = 0;
			for (size_t i = 0; i < 12; i++) {
				int digit = digits[i] - '0';
				sum += (i % 2 == 0) ? digit : digit * 3;
			}

			return (10 - (sum % 10)) % 10;
		}

		/// @brief Gets the digts chars from the code.
		/// @param The string reference of the code.
		/// @returns The string of only digit chars.
		std::string getDigits(const std::string& code)
		{
			std::string digits;
			for (char c : code) {
				if (isdigit(c)) {
					digits += c;
				}
			}

			return digits;
		}

		/// @brief Create a 13 digit format ISBN code.
		/// @returns The string code.
		std::string create_format13()
		{
			//book code - 978 / 979
			std::string code;
			code += "978-";

			//registration - 1 digit
			code += std::to_string(rand() % 10);

			code += '-';

			//registrant - 2 digits
			for (size_t i = 0; i < 2; i++) {
				code += std::to_string(rand() % 10);
			}

			code += '-';

			//publication - 6 digits
			for (size_t i = 0; i < 6; i++) {
				code += std::to_string(rand() % 10);
			}

			code += '-';

			std::string digits = getDigits(code);

			int check = calcCheckDigit(digits);

			code += std::to_string(check);

			return code;
		}

		/// @brief Verifies ISBN code with the check digit.
		/// @param The string reference of the code.
		/// @returns The bool verification.
		bool verify_format13(std::string code)
		{
			std::string digits = getDigits(code);
			int checkDigit = calcCheckDigit(digits);
			int providedCheckDigit = digits[12] - '0';

			return (checkDigit == providedCheckDigit);
		}

	public:

		/// 13 digit format ISBN code
		std::string code;

		/// Default ISBN constructor.
		/// Creates a random book ISBN.
		ISBN() {
			this->code = create_format13();
		}

		/// @brief Code ISBN constructor.
		/// @param The string code to be verified & assigned.
		ISBN(std::string code) {
			if (verify_format13(code) == 0) {
				throw new std::runtime_error("Invalid ISBN code");
			}

			this->code = code;
		}

		~ISBN() {}

		bool operator==(const ISBN& other) const {
			return this->code == other.code;
		}
	};

	/// A struct representing a Book.
	/// It has an author, a title, and
	/// a ISBN. 
	struct Book 
    {
	public:
		/// The title.
		std::string title;
		
		/// The author.
		std::string author;

		/// The ISBN.
		ISBN isbn;

		/// Default Book constructor.
		/// Sets the name/author to 'None'.
		/// And generates a random ISBN.
		Book() : title("None"), author("None"), isbn(ISBN()) {}

		/// @brief Title/Author Book constructor
		/// @param The string title.
		/// @param The string author.
		Book(std::string title, std::string author)
		{
			this->title = title;
			this->author = author;
			this->isbn = ISBN();
		}

		/// @brief Title/Author/ISBN Book constructor
		/// @param The string title.
		/// @param The string author.
		/// @param The string ISBN code.
		Book(std::string title, std::string author, std::string isbn)
		{
			this->title = title;
			this->author = author;
			
			// Handles the user creation of books, if the user does not know the ISBN
			// TODO: Add error handling
			if (isbn == "new")
			{
				this->isbn = ISBN();
			}
			else
			{
				this->isbn = ISBN(isbn);
			}
		}

		~Book() { }

		/// @brief Returns the book as a string.
		/// @return The string representation of a book.
		std::string ToString() const {
			return "Title: " + this->title
				+ "\nAuthor: " + this->author
				+ "\nISBN: " + this->isbn.code;
		}

		/// @brief Compares book's using their ISBN code
		/// @param The book to compare.
		/// @result The bool result of the comparison.
		bool operator==(const Book& other) const {
			return this->isbn == other.isbn;
		}
	};

	// JSON serialization for Book
	inline void to_json(nlohmann::json& j, const Book& book) {
		j = {
			{"title", book.title},
			{"author", book.author},
			{"isbn", book.isbn.code}
		};
	}

	// JSON deserialization for Book
	inline void from_json(const nlohmann::json& j, Book& book) {
		book.title = j.at("title").get<std::string>();
		book.author = j.at("author").get<std::string>();
		book.isbn = ISBN(j.at("isbn").get<std::string>());
	}

	/// A class representing a Library.
	/// It stores books. It contains functions
	/// to manipulate/access the books it contains.
	class Library {
	private:

		/// Map of title indexes - Key: title - Value: index in books
		std::unordered_map<std::string, std::vector<size_t>> title_indexes;

		/// Map of author indexes - Key: author - Value: index in books
		std::unordered_map<std::string, std::vector<size_t>> author_indexes;

		/// Map of ISBN indexes - Key: ISBN code - Value: index in books
		std::unordered_map<std::string, size_t> isbn_indexes;

		/// @brief Converts a string to lowercase.
		/// @param The string to be converted.
		/// @returns The lowercase version of the string.
		std::string toLC(const std::string& str) const {
			std::string lower = str;
			std::transform(lower.begin(), lower.end(), lower.begin(),
				[](unsigned char c) { return std::tolower(c); });
			return lower;
		}

		/// @brief Rebuilds all internal indexes.
		/// Called after modifying book vector.
		void re_index() {
			title_indexes.clear();
			author_indexes.clear();
			isbn_indexes.clear();

			for (size_t index = 0; index < books.size(); index++) {
				Book book = books[index];
				title_indexes[toLC(book.title)].push_back(index);
				author_indexes[toLC(book.author)].push_back(index);
				isbn_indexes[book.isbn.code] = index;
			}
		}

		/// @brief Appends matching books by partial key to result list.
		/// @param map The index map to search.
		/// @param res The result vector to append books into.
		/// @param term The lowercase search term.
		void append_indexes(
			const std::unordered_map<std::string, std::vector<size_t>>& map,
			std::vector<Book>& res,
			std::string& term)
		{
			for (const auto& [keys, indices] : map) {
				if (keys.find(term) != std::string::npos) {
					for (size_t index : indices) {
						res.push_back(books[index]);
					}
				}
			}
		}

		/// @brief Searches by title.
		/// @param term The search term.
		/// @returns A vector of matched books.
		std::vector<Book> title_search(std::string& term)
		{
			std::vector<Book> res;
			std::string title = toLC(term);

			append_indexes(title_indexes, res, title);

			return res;
		}

		/// @brief Searches by author.
		/// @param term The search term.
		/// @returns A vector of matched books.
		std::vector<Book> author_search(std::string& term)
		{
			std::vector<Book> res;
			std::string author = toLC(term);

			append_indexes(author_indexes, res, author);

			return res;
		}

		/// @brief Searches by ISBN code.
		/// @param term The ISBN to match.
		/// @returns A vector of 1 or 0 matched books.
		std::vector<Book> isbn_search(std::string& term)
		{
			std::vector<Book> res;

			auto pair = isbn_indexes.find(term);

			if (pair != isbn_indexes.end()) {
				res.push_back(books[pair->second]);
			}

			return res;
		}

	public:
		/// All books stored in the library.
		std::vector<Book> books;

		/// The result list of the last search.
		std::vector<Book> search_res;

		/// Default Library constructor.
		/// Loads saved books from disk.
		Library() 
        {
			if (!load()) 
            {
				return;
			}
			re_index();
		}

		~Library() { }

		/// @brief Adds a new book to the library.
		/// @param book The book to add.
		void add(const Book& book)
		{
			books.push_back(book);
			size_t index = books.size() - 1;

			title_indexes[toLC(book.title)].push_back(index);
			author_indexes[toLC(book.author)].push_back(index);
			isbn_indexes[book.isbn.code] = index;

			//save();
		}

		/// @brief Removes a book by ISBN.
		/// @param book The book to remove.
		/// @returns True if removed, false if not found.
		bool remove(const Book& book)
		{
			auto pair = isbn_indexes.find(book.isbn.code);

			if (pair == isbn_indexes.end()) {
				return false;
			}

			size_t index = pair->second;

			books.erase(books.begin() + index);

			re_index();

			//save();

			return true;
		}

		/// @brief Searches books by a given term and type.
		/// @param term The search keyword.
		/// @param type The type of search (TITLE, AUTHOR, ISBN).
		/// @returns A list of books that match the search.
		std::vector<Book> search(std::string term, SEARCH type)
		{
            std::vector<Book> res;
			switch (type)
			{
			case SEARCH::TITLE:
				res = title_search(term);
                break;
			case SEARCH::AUTHOR:
				res = author_search(term);
                break;
			case SEARCH::CODE:
				res = isbn_search(term);
                break;
			default:
				break;
			}

            return res;
		}

		/// @brief Gets the number of books in the library.
		/// @returns The count of books.
		size_t size() const {
			return books.size();
		}

		/// @brief Saves all books to a JSON file.
		void save()
		{
			std::filesystem::path books_path = std::filesystem::current_path() / "data" / "library_books.json";


			nlohmann::json books_j = books;
			std::ofstream o(books_path);
			o << std::setw(4) << books_j << std::endl;
			o.close();
		}

		/// @brief Loads books from a JSON file.
		/// @returns True if loaded, false otherwise.
		bool load() 
        {
            std::filesystem::path books_path = std::filesystem::current_path() / "data" / "library_books.json";

			if (!std::filesystem::exists(books_path)) {
				return false;
			}

			std::ifstream i(books_path);
			if (!i.is_open()) {
				return false;
			}

			nlohmann::json j;
			i >> j;
			i.close();

			books = j;

			return true;
		}

	};

}

namespace std {
	template <>
	struct hash<LibraryTypes::ISBN> {
		size_t operator()(const LibraryTypes::ISBN& isbn) const {
			return hash<std::string>()(isbn.code);
		}
	};
}

#endif //LIBTYPES_H