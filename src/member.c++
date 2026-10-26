#include <iostream>
#include <string>
#include "../include/member.h++"
#include "../include/input.h++"
#include <mysqlx/xdevapi.h>
#include <iomanip>

// =================================
//          book Issue
// =================================

void member::bookIssue(mysqlx::Schema &db)
{
    std::cout << "==== Book Issue ====" << std::endl;

    if (!input::readInteger("Enter book ID: ", bookId) ||
        !input::readInteger("Enter member ID: ", memberId))
    {
        return;
    }

    auto books = db.getTable("books");
    auto bookResult = books.select("available_copies")
        .where("book_id = :bookId")
        .bind("bookId", bookId)
        .execute();
    auto book = bookResult.fetchOne();
    if (!book)
    {
        std::cout << "Book not found.\n";
        return;
    }
    if (book[0].get<unsigned int>() == 0)
    {
        std::cout << "No copies of that book are currently available.\n";
        return;
    }

    auto members = db.getTable("members");
    auto memberResult = members.select("member_id")
        .where("member_id = :memberId")
        .bind("memberId", memberId)
        .execute();
    if (!memberResult.fetchOne())
    {
        std::cout << "Member not found.\n";
        return;
    }

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

    if (!input::readInteger("Enter book ID: ", bookId) ||
        !input::readInteger("Enter member ID: ", memberId))
    {
        return;
    }

    auto books = db.getTable("books");
    auto bookResult = books.select("book_id")
        .where("book_id = :bookId")
        .bind("bookId", bookId)
        .execute();
    if (!bookResult.fetchOne())
    {
        std::cout << "Book not found.\n";
        return;
    }

    auto members = db.getTable("members");
    auto memberResult = members.select("member_id")
        .where("member_id = :memberId")
        .bind("memberId", memberId)
        .execute();
    if (!memberResult.fetchOne())
    {
        std::cout << "Member not found.\n";
        return;
    }

    auto issues = db.getTable("book_issues");
    auto issueResult = issues.select("issue_id")
        .where("book_id = :bookId AND member_id = :memberId")
        .bind("bookId", bookId)
        .bind("memberId", memberId)
        .execute();
    auto issueRows = issueResult.fetchAll();

    auto returns = db.getTable("returned_books");
    auto returnResult = returns.select("return_id")
        .where("book_id = :bookId AND member_id = :memberId")
        .bind("bookId", bookId)
        .bind("memberId", memberId)
        .execute();
    auto returnRows = returnResult.fetchAll();

    std::size_t issueCount = 0;
    for (const auto &row : issueRows)
    {
        (void)row;
        ++issueCount;
    }
    std::size_t returnCount = 0;
    for (const auto &row : returnRows)
    {
        (void)row;
        ++returnCount;
    }

    if (returnCount >= issueCount)
    {
        std::cout << "This member has no active checkout for that book.\n";
        return;
    }

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
