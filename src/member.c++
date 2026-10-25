#include <iostream>
#include <string>
#include "../include/member.h++"
#include <mysqlx/xdevapi.h>
#include <iomanip>

// =================================
//          book Issue
// =================================

void member::bookIssue(mysqlx::Schema &db)
{
    std::cout << "==== Book Issue ====" << std::endl;

    std::cout << "Enter Issue book : ";
    std::cin >> bookId;

    std::cout << "Issue Member ID : ";
    std::cin >> memberId;

    std::cout << std::endl;

    // execute sql

    auto issues = db.getTable("book_issues");
    issues.insert("book_id", "member_id")
        .values(bookId, memberId)
        .execute();
};

// =================================
//          book Return
// =================================

void member::bookReturn(mysqlx::Schema &db)
{
    std::cout << "==== Book Return ====\n";

    std::cout << "Enter book ID: ";
    std::cin >> bookId;

    std::cout << "Enter member ID: ";
    std::cin >> memberId;

    auto returns = db.getTable("returned_books");

    returns.insert("book_id", "member_id")
        .values(bookId, memberId)
        .execute();
    std::cout << "Return recorded.\n";
}

// =================================
//         Total book Issue
// =================================

void member::totalBookissue(mysqlx::Schema &db)
{
    auto issues = db.getTable("book_issues");

    auto result = issues.select(
                            "issue_id",
                            "book_id",
                            "member_id",
                            "DATE_FORMAT(issue_date, '%d-%m-%Y') AS issue_date_text" // set formate of the date
                            )
                      .execute();

    auto allRows = result.fetchAll();

    std::cout << std::setw(25) << "Total Bookissue Table" << std::endl;
    // setw -> set width of text
    std::cout << std::string(53, '-') << '\n';

    std::cout << std::left
              << std::setw(12) << "Issue ID"
              << std::setw(12) << "Book ID"
              << std::setw(14) << "Member ID"
              << std::setw(15) << "Issue Date"
              << '\n';

    std::cout << std::string(53, '-') << '\n';

    for (const auto &row : allRows)
    {
        std::cout << std::left                                   // align text to left side
                  << std::setw(12) << row[0].get<unsigned int>() // .get<unsigned int>()  convert it into c++ int it is optional
                  << std::setw(12) << row[1].get<unsigned int>()
                  << std::setw(14) << row[2].get<unsigned int>()
                  << std::setw(15) << row[3].get<std::string>() // .get<std::string>() convert to stirng
                  << '\n';
    }
}
