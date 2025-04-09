# C++ Library Management System Project Rubric

Your next assignment is a Library Mangement System:

- Your code must compile with `CMake`
- You Must Write Unit Tests using `gtest`
- Your code must conform to the C++17 standard
- Your code must compile with NO warnings (-Werror must be enabled as a grading requirement)
- The rubric specifies "Class Designs". If you want to use procedural style c/c++ go for it. just beware that it makes unit testing... complicated

## **Functionality (40 points)**
| Criteria                          | Points | Description                                                                 |
|-----------------------------------|--------|-----------------------------------------------------------------------------|
| **Add/Remove Books**              | 5      | Can add and remove books correctly.                                         |
| **Search Books**                  | 5      | Can search books by title, author, or ISBN.                                |
| **Display Books**                 | 5      | Displays all books in a readable format.                                    |
| **Add/Remove Users**              | 5      | Can add and remove users correctly.                                         |
| **Borrow/Return Books**           | 10     | Allows users to borrow and return books, updating quantities appropriately. |
| **File Handling**                 | 5      | Saves and loads data to/from files correctly.                               |
| **Error Handling**                | 5      | Handles invalid inputs and edge cases gracefully.                           |

---

## **Code Quality (30 points)**
| Criteria                          | Points | Description                                                                 |
|-----------------------------------|--------|-----------------------------------------------------------------------------|
| **Class Design**                  | 10     | Classes are well-designed, with appropriate attributes and methods.         |
| **Modularity**                    | 10     | Code is modular, with functions and classes serving clear purposes.         |
| **Readability**                   | 10     | Code is clean, well-formatted, and easy to follow.                          |

---

## **Documentation (15 points)**
| Criteria                          | Points | Description                                                                 |
|-----------------------------------|--------|-----------------------------------------------------------------------------|
| **Comments**                      | 10     | Code is adequately commented, explaining key logic and functions.          |

---

## **Unit Test Coverage (15 points)**
| Criteria                                | Points | Description                                                               |
|-----------------------------------------|--------|---------------------------------------------------------------------------|
| **Test Cases for Core Features**        | 10     | Includes unit tests for adding/removing books, searching, borrowing, etc. |
| **Test Cases for File Reading/Writing** | 5      | Includes integration tests for IO bound code                              |
| **Edge Case Testing**                   | 5      | Tests edge cases (e.g., borrowing when no books are available).           |


---

## **Notes for Evaluation**
- Deduct points for incomplete or non-functional features.
- Ensure the program compiles and runs without critical errors.
- Unit tests should be clearly documented and cover a significant portion of the codebase.

## FAQ
- What should the user interface be?
Unit tests are a fine enough interface, but if you want to write a text based interface go for it
- What should the save file be format wise?
Dealers choice
