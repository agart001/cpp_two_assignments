#include <gtest/gtest.h>
#include "../include/LibTypes.h"
#include "../include/json.hpp"

// ISBN Tests
TEST(ISBNTests, DefaultConstructor)
{
  LibraryTypes::ISBN isbn;
  EXPECT_EQ(isbn.code.length(), 17);
  EXPECT_EQ(isbn.code.substr(0, 4), "978-");
}

TEST(ISBNTests, ConstructorWithValidCode)
{
  LibraryTypes::ISBN isbn("978-3-16-148410-0");
  EXPECT_EQ(isbn.code, "978-3-16-148410-0");
}

TEST(ISBNTests, ConstructorWithInvalidCode) {
  EXPECT_THROW(LibraryTypes::ISBN isbn("123-4-56-789012-3"), std::runtime_error);
}

TEST(ISBNTests, EqualsEqualsOperator) 
{
  LibraryTypes::ISBN isbn1("978-3-16-148410-0");
  LibraryTypes::ISBN isbn2("978-3-16-148410-0");
  EXPECT_TRUE(isbn1 == isbn2);
}

TEST(ISBNTests, NotEqualsOperator) 
{
  LibraryTypes::ISBN isbn1;
  LibraryTypes::ISBN isbn2;
  EXPECT_FALSE(isbn1 == isbn2);
}

// Book Tests
TEST(BookTests, DefaultConstructor)
{
  LibraryTypes::Book book;
  EXPECT_EQ(book.title, "None");
  EXPECT_EQ(book.author, "None");
  EXPECT_EQ(book.isbn.code.length(), 17);
}

TEST(BookTests, ConstructorWithTitleAndAuthor)
{
  LibraryTypes::Book book("Example Title", "Example Author");
  EXPECT_EQ(book.title, "Example Title");
  EXPECT_EQ(book.author, "Example Author");
  EXPECT_EQ(book.isbn.code.length(), 17);
}

TEST(BookTests, ConstructorWithTitleAuthorAndISBN)
{
  LibraryTypes::Book book("Example Title", "Example Author", "978-3-16-148410-0");
  EXPECT_EQ(book.title, "Example Title");
  EXPECT_EQ(book.author, "Example Author");
  EXPECT_EQ(book.isbn.code, "978-3-16-148410-0");
}

TEST(BookTests, ConstructorWithTitleAuthorAndNEW)
{
  LibraryTypes::Book book("Example Title", "Example Author", "new");
  EXPECT_EQ(book.title, "Example Title");
  EXPECT_EQ(book.author, "Example Author");
  EXPECT_EQ(book.isbn.code.length(), 17);
}

TEST(BookTests, ToString)
{
  LibraryTypes::Book book("Example Title", "Example Author", "978-3-16-148410-0");
  std::string expected = "Title: Example Title\nAuthor: Example Author\nISBN: 978-3-16-148410-0";
  EXPECT_EQ(book.ToString(), expected);
}

TEST(BookTests, EqualsEqualsOperator) 
{
  LibraryTypes::Book book1("Example Title", "Example Author", "978-3-16-148410-0");
  LibraryTypes::Book book2("Example Title", "Example Author","978-3-16-148410-0");
  EXPECT_TRUE(book1 == book2);
}

TEST(BookTests, NotEqualsOperator) 
{
  LibraryTypes::Book book1("Example Title", "Example Author", "978-3-16-148410-0");
  LibraryTypes::Book book2("Another Title", "Another Author","new");
  EXPECT_FALSE(book1 == book2);
}

TEST(BookTests, ToJsonString)
{
  LibraryTypes::Book book("Example Title", "Example Author", "978-3-16-148410-0");
  nlohmann::json j = book;
  std::string expected = R"({"author":"Example Author","isbn":"978-3-16-148410-0","title":"Example Title"})";
  EXPECT_EQ(j.dump(), expected);
}

TEST(BookTests, FromJsonString)
{
  nlohmann::json j = nlohmann::json::parse(R"({"title":"Example Title","author":"Example Author","isbn":"978-3-16-148410-0"})");
  EXPECT_TRUE(j.is_object());
  EXPECT_TRUE(j.contains("title"));
  EXPECT_TRUE(j.contains("author"));
  EXPECT_TRUE(j.contains("isbn"));

  LibraryTypes::Book book = j.get<LibraryTypes::Book>();

  EXPECT_EQ(book.title, "Example Title");
  EXPECT_EQ(book.author, "Example Author");
  EXPECT_EQ(book.isbn.code, "978-3-16-148410-0");
}

TEST(BookTests, FromJsonStringInvalid)
{
  nlohmann::json j = nlohmann::json::parse(R"({"title":"Example Title","author":"Example Author","isbn":"123-4-56-789012-3"})");
  EXPECT_TRUE(j.is_object());
  EXPECT_TRUE(j.contains("title"));
  EXPECT_TRUE(j.contains("author"));
  EXPECT_TRUE(j.contains("isbn"));

  EXPECT_THROW(LibraryTypes::Book book = j.get<LibraryTypes::Book>(), std::runtime_error);
}

TEST(BookTests, BookVectorToJsonString)
{
  std::vector<LibraryTypes::Book> books = {
    LibraryTypes::Book("Title1", "Author1", "978-3-16-148410-0"),
    LibraryTypes::Book("Title2", "Author2", "978-3-16-148410-0")
  };

  nlohmann::json j = books;
  std::string expected = R"([{"author":"Author1","isbn":"978-3-16-148410-0","title":"Title1"},{"author":"Author2","isbn":"978-3-16-148410-0","title":"Title2"}])";
  EXPECT_EQ(j.dump(), expected);
}

TEST(BookTests, BookVectorFromJsonString)
{
  nlohmann::json j = nlohmann::json::parse(R"([{"title":"Title1","author":"Author1","isbn":"978-3-16-148410-0"},{"title":"Title2","author":"Author2","isbn":"978-3-16-148410-0"}])");
  EXPECT_TRUE(j.is_array());

  std::vector<LibraryTypes::Book> books = j.get<std::vector<LibraryTypes::Book>>();

  EXPECT_EQ(books.size(), 2);
  EXPECT_EQ(books[0].title, "Title1");
  EXPECT_EQ(books[1].title, "Title2");
}

// Library Tests

TEST(LibraryTests, DefaultConstructor)
{
  LibraryTypes::Library lib;
  EXPECT_EQ(lib.size(), 0);
}

TEST(LibraryTests, AddBook)
{
  LibraryTypes::Library lib;
  LibraryTypes::Book book("Example Title", "Example Author", "978-3-16-148410-0");
  lib.add(book);
  EXPECT_EQ(lib.size(), 1);
  EXPECT_EQ(lib.books[0].title, "Example Title");
}

TEST(LibraryTests, RemoveBook)
{
  LibraryTypes::Library lib;
  LibraryTypes::Book book("Example Title", "Example Author", "978-3-16-148410-0");
  lib.add(book);
  EXPECT_EQ(lib.size(), 1);
  EXPECT_TRUE(lib.remove(book));
  EXPECT_EQ(lib.size(), 0);
}

TEST(LibraryTests, RemoveBookNotFound)
{
  LibraryTypes::Library lib;
  LibraryTypes::Book book("Example Title", "Example Author", "978-3-16-148410-0");
  EXPECT_FALSE(lib.remove(book));
  EXPECT_EQ(lib.size(), 0);
}

TEST(LibraryTests, LoadBooks)
{
  std::string json = R"([{"author":"Author1","isbn":"978-3-16-148410-0","title":"Title1"},{"author":"Author2","isbn":"978-3-16-148410-0","title":"Title2"}])";
  LibraryTypes::Library lib;
  EXPECT_TRUE(lib.load(json));
  EXPECT_EQ(lib.size(), 2);
  EXPECT_EQ(lib.books[0].title, "Title1");
  EXPECT_EQ(lib.books[1].title, "Title2");
}

TEST(LibraryTests, LoadBooksInvalid)
{
  std::string json = R"([{"author":"Author1","isbn":"123-4-56-789012-3","title":"Title1"},{"author":"Author2","isbn":"978-3-16-148410-0","title":"Title2"}])";
  LibraryTypes::Library lib;
  EXPECT_THROW(lib.load(json), std::runtime_error);
}

TEST(LibraryTests, SaveBooks)
{
  LibraryTypes::Library lib;
  LibraryTypes::Book book("Title1", "Author1", "978-3-16-148410-0");
  LibraryTypes::Book book2("Title2", "Author2", "978-3-16-148410-0");
  lib.add(book);
  lib.add(book2);
  EXPECT_EQ(lib.size(), 2);

  std::string expected_json = R"([{"author":"Author1","isbn":"978-3-16-148410-0","title":"Title1"},{"author":"Author2","isbn":"978-3-16-148410-0","title":"Title2"}])";
  EXPECT_EQ(lib.save_as_json(), expected_json);
}

TEST(LibraryTests, SearchByTitle)
{
  LibraryTypes::Library lib;
  LibraryTypes::Book book("Example Title", "Example Author", "978-3-16-148410-0");
  lib.add(book);
  EXPECT_EQ(lib.size(), 1);

  std::string search_term = "Example Title";
  auto result = lib.search(search_term, LibraryTypes::SEARCH::TITLE);
  EXPECT_EQ(result.size(), 1);
  EXPECT_EQ(result[0].title, "Example Title");
}

TEST(LibraryTests, SearchByAuthor)
{
  LibraryTypes::Library lib;
  LibraryTypes::Book book("Example Title", "Example Author", "978-3-16-148410-0");
  lib.add(book);
  EXPECT_EQ(lib.size(), 1);

  std::string search_term = "Example Author";
  auto result = lib.search(search_term, LibraryTypes::SEARCH::AUTHOR);
  EXPECT_EQ(result.size(), 1);
  EXPECT_EQ(result[0].author, "Example Author");
}

TEST(LibraryTests, SearchByISBN)
{
  LibraryTypes::Library lib;
  LibraryTypes::Book book("Example Title", "Example Author", "978-3-16-148410-0");
  lib.add(book);
  EXPECT_EQ(lib.size(), 1);

  std::string search_term = "978-3-16-148410-0";
  auto result = lib.search(search_term, LibraryTypes::SEARCH::CODE);
  EXPECT_EQ(result.size(), 1);
  EXPECT_EQ(result[0].isbn.code, "978-3-16-148410-0");
}

TEST(LibraryTests, SearchEmptyLibrary)
{
  LibraryTypes::Library lib;
  EXPECT_EQ(lib.size(), 0);

  std::string search_term = "Example Title";
  auto result = lib.search(search_term, LibraryTypes::SEARCH::TITLE);
  EXPECT_EQ(result.size(), 0);
}

TEST(LibraryTests, SearchEmptyResult)
{
  LibraryTypes::Library lib;
  LibraryTypes::Book book("Example Title", "Example Author", "978-3-16-148410-0");
  lib.add(book);
  EXPECT_EQ(lib.size(), 1);

  std::string search_term = "Nonexistent Title";
  auto result = lib.search(search_term, LibraryTypes::SEARCH::TITLE);
  EXPECT_EQ(result.size(), 0);
}

#include "../include/hash_sha256.h"
#include "../include/User.h"

// User Tests
TEST(UserTests, DefaultConstructor)
{
  hash_sha256 hash;
  hash.sha256_init();
  sha256_type empty_hash{};

  User user;
  EXPECT_EQ(user.name, "");
  EXPECT_EQ(user.password, empty_hash);
  EXPECT_EQ(user.books.size(), 0);
}

TEST(UserTests, ConstructorWithNameAndPassword)
{
  hash_sha256 hash;
  hash.sha256_init();

  std::string name = "Example User";
  std::string pass = "ExamplePassword";

  auto bytes = std::stobya(pass);
	hash.sha256_init();
	hash.sha256_update(bytes.data(), bytes.size());
	sha256_type password = hash.sha256_final();

	User user = User(name, password);

  EXPECT_EQ(user.name, name);
  EXPECT_EQ(user.password, password);
  EXPECT_EQ(user.books.size(), 0);
  EXPECT_EQ(user.password.size(), 32);
  EXPECT_FALSE(user.isNULL());
}
