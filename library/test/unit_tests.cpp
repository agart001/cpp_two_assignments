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

sha256_type UserPasswordHash(const std::string& password)
{
  hash_sha256 hash;
  hash.sha256_init();
  auto bytes = std::stobya(password);
  hash.sha256_update(bytes.data(), bytes.size());
  return hash.sha256_final();
}

User ExampleUser(std::string name, std::string pass)
{
  sha256_type password = UserPasswordHash(pass);
  User user = User(name, password);

  return user;
}

TEST(UserTests, ConstructorWithNameAndPassword)
{
  std::string name = "Example User";
  sha256_type password = UserPasswordHash("ExamplePassword");
	User user = ExampleUser("Example User", "ExamplePassword");

  EXPECT_EQ(user.name, name);
  EXPECT_EQ(user.password, password);
  EXPECT_EQ(user.books.size(), 0);
  EXPECT_EQ(user.password.size(), 32);
  EXPECT_FALSE(user.isNULL());
}

TEST(UserTests, ConstructorWithNameAndPasswordAndBooks)
{
  std::string name = "Example User";
  sha256_type password = UserPasswordHash("ExamplePassword");
  std::vector<LibraryTypes::Book> books = {
    LibraryTypes::Book("Title1", "Author1", "978-3-16-148410-0"),
    LibraryTypes::Book("Title2", "Author2", "978-3-16-148410-0")
  };


  User user = User(name, password, books);
  EXPECT_EQ(user.name, name);
  EXPECT_EQ(user.password, password);
  EXPECT_EQ(user.books.size(), 2);
  EXPECT_EQ(user.books[0].title, "Title1");
  EXPECT_EQ(user.books[1].title, "Title2");
  EXPECT_EQ(user.password.size(), 32);
  EXPECT_FALSE(user.isNULL());
}

TEST(UserTests, EqualsEqualsOperator)
{
  User user1 = ExampleUser("Example User", "ExamplePassword");
  User user2 = ExampleUser("Example User", "ExamplePassword");

  EXPECT_TRUE(user1 == user2);
}

TEST(UserTests, NotEqualsOperator)
{
  User user1 = ExampleUser("Example User", "ExamplePassword");
  User user2 = ExampleUser("Another User", "AnotherPassword");

  EXPECT_FALSE(user1 == user2);
}

TEST(UserTests, ToJsonString)
{
  User user = ExampleUser("Example User", "pass");
  nlohmann::json j = user;

  std::string expected = R"({"books":[],"name":"Example User","password":[156,67,169,96,230,45,62,132,110,0,41,190,46,123,34,112,67,249,22,173,198,141,17,83,69,17,9,34,58,204,63,195]})";
  EXPECT_EQ(j.dump(), expected);
}

TEST(UserTests, UserVectorToJsonString)
{
  std::vector<User> users = {
    ExampleUser("Example User", "pass")
  };

  nlohmann::json j = users;

  std::string expected = R"([{"books":[],"name":"Example User","password":[156,67,169,96,230,45,62,132,110,0,41,190,46,123,34,112,67,249,22,173,198,141,17,83,69,17,9,34,58,204,63,195]}])";

  EXPECT_EQ(j.dump(), expected);
}

TEST(UserTests, FromJsonString)
{
  std::string json = R"({"books":[],"name":"Example User","password":[156,67,169,96,230,45,62,132,110,0,41,190,46,123,34,112,67,249,22,173,198,141,17,83,69,17,9,34,58,204,63,195]})";

  nlohmann::json j = nlohmann::json::parse(json);
  EXPECT_TRUE(j.is_object());

  User user = j.get<User>();
  EXPECT_EQ(user.name, "Example User");
  EXPECT_EQ(user.password.size(), 32);
  EXPECT_EQ(user.books.size(), 0);
  EXPECT_FALSE(user.isNULL());
}

TEST(UserTests, UserVectorFromJsonString)
{
  std::string json = R"([{"books":[],"name":"Example User","password":[156,67,169,96,230,45,62,132,110,0,41,190,46,123,34,112,67,249,22,173,198,141,17,83,69,17,9,34,58,204,63,195]}])";
  nlohmann::json j = nlohmann::json::parse(json);
  EXPECT_TRUE(j.is_array());

  std::vector<User> users = j.get<std::vector<User>>();

  EXPECT_EQ(users.size(), 1);
  EXPECT_EQ(users[0].name, "Example User");
  EXPECT_EQ(users[0].password.size(), 32);
  EXPECT_EQ(users[0].books.size(), 0);
  EXPECT_FALSE(users[0].isNULL());
}

// UserManager Tests

TEST(UMTests, DefaultConstructor)
{
  UserManager um;
  EXPECT_EQ(um.size(), 0);
}

TEST(UMTests, AddUser)
{
  UI::TEST_MODE = true;
  UserManager um;
  User user = ExampleUser("Example User", "ExamplePassword");

  um.add(user);
  EXPECT_EQ(um.size(), 1);
  EXPECT_EQ(um.at(0).name, "Example User");
}

TEST(UMTests, RemoveUser)
{
  UI::TEST_MODE = true;
  UserManager um;
  User user = ExampleUser("Example User", "ExamplePassword");

  um.add(user);
  EXPECT_EQ(um.size(), 1);
  um.remove(user);
  EXPECT_EQ(um.size(), 0);
}

TEST(UMTests, SigninUser)
{
  UI::TEST_MODE = true;
  UserManager um;
  User user = ExampleUser("User", "pass");

  um.add(user);
  EXPECT_EQ(um.size(), 1);
  EXPECT_EQ(um.at(0).name, "User");

  std::istringstream input("User\npass\n");
  std::streambuf* origCin = std::cin.rdbuf();
  std::cin.rdbuf(input.rdbuf());

  std::ostringstream out;
  std::streambuf* origCout = std::cout.rdbuf();
  std::cout.rdbuf(out.rdbuf());

  um.signin();

  EXPECT_EQ(um.current_user.name, "User");
  EXPECT_EQ(um.current_user.password, user.password);
  EXPECT_EQ(um.current_user.books.size(), 0);

  std::cin.rdbuf(origCin);
  std::cout.rdbuf(origCout);
}

TEST(UMTests, SignupUser)
{
  UI::TEST_MODE = true;
  UserManager um;
  std::istringstream input("User\npass\n");
  std::streambuf* origCin = std::cin.rdbuf();
  std::cin.rdbuf(input.rdbuf());

  std::ostringstream out;
  std::streambuf* origCout = std::cout.rdbuf();
  std::cout.rdbuf(out.rdbuf());

  um.signup();

  EXPECT_EQ(um.size(), 1);
  EXPECT_EQ(um.at(0).name, "User");
  EXPECT_EQ(um.at(0).password.size(), 32);
  EXPECT_EQ(um.at(0).books.size(), 0);

  std::cin.rdbuf(origCin);
  std::cout.rdbuf(origCout);
}

TEST(UMTests, LoginToSignin)
{
  UI::TEST_MODE = true;
  UserManager um;
  User user = ExampleUser("User", "pass");

  std::ostringstream out;
  std::streambuf* origCout = std::cout.rdbuf();
  std::cout.rdbuf(out.rdbuf());

  um.add(user);
  EXPECT_EQ(um.size(), 1);
  EXPECT_EQ(um.at(0).name, "User");

  std::istringstream input("1\nUser\npass\n");
  std::streambuf* origCin = std::cin.rdbuf();
  std::cin.rdbuf(input.rdbuf());

  um.login();

  EXPECT_EQ(um.current_user.name, "User");
  EXPECT_EQ(um.current_user.password, user.password);
  EXPECT_EQ(um.current_user.books.size(), 0);

  std::cin.rdbuf(origCin);
  std::cout.rdbuf(origCout);
} 

TEST(UMTests, LoginToSignup)
{
  UI::TEST_MODE = true;
  UserManager um;
  EXPECT_EQ(um.size(), 0);

  std::istringstream input("2\nUser\npass\n");
  std::streambuf* origCin = std::cin.rdbuf();
  std::cin.rdbuf(input.rdbuf());

  std::ostringstream out;
  std::streambuf* origCout = std::cout.rdbuf();
  std::cout.rdbuf(out.rdbuf());

  um.login();

  EXPECT_EQ(um.size(), 1);
  EXPECT_EQ(um.at(0).name, "User");
  EXPECT_EQ(um.at(0).password.size(), 32);
  EXPECT_EQ(um.at(0).books.size(), 0);
  EXPECT_EQ(um.current_user.name, "User");

  std::cin.rdbuf(origCin);
  std::cout.rdbuf(origCout);
}

TEST(UMTests, CurrentUserAddBook)
{
  UI::TEST_MODE = true;
  UserManager um;
  User user = ExampleUser("User", "pass");

  um.add(user);
  EXPECT_EQ(um.size(), 1);
  EXPECT_EQ(um.at(0).name, "User");

  LibraryTypes::Book book("Title1", "Author1", "978-3-16-148410-0");
  um.current_user = user;
  um.add(book);

  EXPECT_EQ(um.current_user.books.size(), 1);
  EXPECT_EQ(um.current_user.books[0].title, "Title1");
}

TEST(UMTests, CurrentUserRemoveBook)
{
  UI::TEST_MODE = true;
  UserManager um;
  User user = ExampleUser("User", "pass");

  um.add(user);
  EXPECT_EQ(um.size(), 1);
  EXPECT_EQ(um.at(0).name, "User");

  LibraryTypes::Book book("Title1", "Author1", "978-3-16-148410-0");
  um.current_user = user;
  um.add(book);

  EXPECT_EQ(um.current_user.books.size(), 1);
  EXPECT_EQ(um.current_user.books[0].title, "Title1");

  um.remove(0);
  EXPECT_EQ(um.current_user.books.size(), 0);
}

#include "../include/App.h"

// LibraryApp Tests
TEST(LibraryAppTests, AppLoginToSignup)
{
  LibraryApp app;
  UI::TEST_MODE = true;

  std::istringstream input("2\nUser\npass\n3\n");
  std::streambuf* origCin = std::cin.rdbuf();
  std::cin.rdbuf(input.rdbuf());

  std::ostringstream out;
  std::streambuf* origCout = std::cout.rdbuf();
  std::cout.rdbuf(out.rdbuf());

  app.start();

  EXPECT_EQ(app.size(true), 1);

  std::cin.rdbuf(origCin);
  std::cout.rdbuf(origCout);
}

TEST(LibraryAppTests, AppLoginToSignin)
{
  UI::TEST_MODE = true;
  User user = ExampleUser("User","pass");
  UserManager um;
  um.add(user);
  LibraryTypes::Library lib;

  LibraryApp app = LibraryApp(um,lib);

  std::istringstream input("1\nUser\npass\n3\n");
  std::streambuf* origCin = std::cin.rdbuf();
  std::cin.rdbuf(input.rdbuf());

  std::ostringstream out;
  std::streambuf* origCout = std::cout.rdbuf();
  std::cout.rdbuf(out.rdbuf());

  app.start();

  EXPECT_EQ(app.current_user(), user);

  std::cin.rdbuf(origCin);
  std::cout.rdbuf(origCout);
}

TEST(LibraryAppTests, SearchBookByTitle)
{
  UI::TEST_MODE = true;

  LibraryTypes::Library lib;
  LibraryTypes::Book book("Title", "Author", "978-3-16-148410-0");
  lib.add(book);

  UserManager um;
  User user = ExampleUser("User", "pass");
  um.add(user);
  LibraryApp app(um, lib);
  EXPECT_EQ(app.size(false), 1);
  EXPECT_EQ(app.size(true), 1);

  // 1 - Signin
  // User 
  // pass
  // 2 - Library Main Menu
  // 1 - Inventory
  // 2 - Search
  // 1 - Title
  // Test Title
  // 3 - Exit
  std::istringstream input("1\nUser\npass\n2\n1\n2\n1\nTitle\n3\n");
  std::streambuf *origCin = std::cin.rdbuf();
  std::cin.rdbuf(input.rdbuf());

  std::ostringstream out;
  std::streambuf *origCout = std::cout.rdbuf();
  std::cout.rdbuf(out.rdbuf());

  app.start();

  EXPECT_FALSE(app.search_res().empty());
  EXPECT_EQ(app.search_res()[0].title, "Title");

  std::cin.rdbuf(origCin);
  std::cout.rdbuf(origCout);
}

/*TEST(LibraryAppTests, SearchBookByAuthor)
{
  UI::TEST_MODE = true;

  LibraryTypes::Library lib;
  LibraryTypes::Book book("Title", "Author", "978-3-16-148410-0");
  lib.add(book);

  UserManager um;
  User user = ExampleUser("User", "pass");
  um.add(user);
  um.current_user = user;
  LibraryApp app(um, lib);

  // 1 - Signin
  // User 
  // pass
  // 2 - Library Main Menu
  // 1 - Inventory
  // 2 - Search
  // 2 - Author
  // Test Author
  // 3 - Exit
  std::istringstream input("1\nUser\npass\n2\n1\n2\n2\nAuthor\n3\n");
  std::streambuf *origCin = std::cin.rdbuf();
  std::cin.rdbuf(input.rdbuf());

  std::ostringstream out;
  std::streambuf *origCout = std::cout.rdbuf();
  std::cout.rdbuf(out.rdbuf());

  app.start();

  EXPECT_FALSE(app.search_res().empty());
  EXPECT_EQ(app.search_res()[0].author, "Author");

  std::cin.rdbuf(origCin);
  std::cout.rdbuf(origCout);
}*/

/*TEST(LibraryAppTests, SearchBookByISBN)
{
  UI::TEST_MODE = true;

  LibraryTypes::Library lib;
  LibraryTypes::Book book("Test Title", "Test Author", "978-3-16-148410-0");
  lib.add(book);

  UserManager um;
  User user = ExampleUser("User", "pass");
  um.add(user);
  um.current_user = user;
  LibraryApp app(um, lib);

  // 1 - Signin
  // User 
  // pass
  // 2 - Library Main Menu
  // 1 - Inventory
  // 2 - Search
  // 3 - ISBN
  // 978-3-16-148410-0
  // 3 - Exit
  std::istringstream input("1\nUser\npass\n2\n1\n2\n3\n978-3-16-148410-0\n3\n");
  std::streambuf *origCin = std::cin.rdbuf();
  std::cin.rdbuf(input.rdbuf());

  std::ostringstream out;
  std::streambuf *origCout = std::cout.rdbuf();
  std::cout.rdbuf(out.rdbuf());

  app.start();

  EXPECT_FALSE(app.search_res().empty());
  EXPECT_EQ(app.search_res()[0].isbn.code, "978-3-16-148410-0");

  std::cin.rdbuf(origCin);
  std::cout.rdbuf(origCout);
}*/