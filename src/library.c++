#include <iostream>
#include <string>
#include <limits>
#include "library.h++"
#include "input.h++"
#include <mysqlx/xdevapi.h>
#include <iomanip>


// =================================
//             Add Book
// =================================

void library::addBook(mysqlx::Schema& db){

    std::cout << "==== Add Book  ====" << std::endl;

    if (!input::readText("Enter book name        : ", bookName) ||
        !input::readText("Enter book author name : ", autherName) ||
        !input::readInteger("Enter book ID          : ", bookID) ||
        !input::readInteger("Enter book stock       : ", bookStock, 0))
    {
        return;
    }

    auto addBook = db.getTable("books");
    addBook.insert("book_id","title","author","total_copies","available_copies")
        .values(bookID,bookName,autherName,bookStock,bookStock)
        .execute();

    std::cout << std::endl;
    std::cout << "Book details added successfully!" << std::endl;
    std::cout << std::endl;
}

// =================================
//            Add Member
// =================================

void library::addMember(mysqlx::Schema& db){
    // taking input
    std::cout << "==== Add Member  ====" << std::endl;

    if (!input::readInteger("Enter member ID   : ", memberId) ||
        !input::readText("Enter member name : ", memberName) ||
        !input::readText("Enter contact     : ", memberContact))
    {
        return;
    }

    auto addMembers = db.getTable("members");
    addMembers.insert("member_id","name","contact")
        .values(memberId, memberName, memberContact)
        .execute();


    std::cout << std::endl;
    std::cout << "Member details added successfully!" << std::endl;
    std::cout << std::endl;

}


// =================================
//        Show All Members
// =================================


void library::showAllMembers(mysqlx::Schema& db){


    auto showAllMember = db.getTable("members");
    auto result = showAllMember.select("member_id", "name", "contact").execute();
    auto allRows = result.fetchAll();


    std::cout << std::setw(30) << "Members List" << std::endl;
    std::cout << std::string(55, '-') << '\n';

    std::cout << std::left
              << std::setw(12) << "Member ID"
              << std::setw(20) << "Name"
              << std::setw(15) << "Contact"
              << '\n';

    std::cout << std::string(55, '-') << '\n';

    for (const auto &row : allRows)
    {
        std::cout << std::left
                  << std::setw(12) << row[0].get<unsigned int>()
                  << std::setw(20) << row[1].get<std::string>()
                  << std::setw(15) << row[2].get<std::string>()
                  << '\n';
    }
}







// =================================
//        Search  Members
// =================================


void library::searchMembers(mysqlx::Schema& db){

    if (!input::readInteger("Enter member ID: ", memberId))
    {
        return;
    }

    auto searchMembers = db.getTable("members");
    auto result = searchMembers.select("*")
    .where("member_id = :memberId")
    .bind("memberId", memberId)
    .execute();
    auto row = result.fetchOne();
    if (!row)
    {
        std::cout << "No member found with that ID.\n";
        return;
    }


    std::cout << std::setw(30) << " Search  Members List" << std::endl;
    std::cout << std::string(55, '-') << '\n';

    std::cout << std::left
              << std::setw(12) << "Member ID"
              << std::setw(20) << "Name"
              << std::setw(15) << "Contact"
              << '\n';

    std::cout << std::string(55, '-') << '\n';

    std::cout << std::left
              << std::setw(12) << row[0].get<unsigned int>()
              << std::setw(20) << row[1].get<std::string>()
              << std::setw(15) << row[2].get<std::string>()
              << '\n';
}


// =================================
//        Update  Members info 
// =================================




void library::updateMembersInfo(mysqlx::Schema& db){
    std::cout << "==== Update Member ====\n";
    if (!input::readInteger("Enter Member ID: ", memberId))
    {
        return;
    }

    std::cout << "1. Update Name\n";
    std::cout << "2. Update Contact\n";
    if (!input::readInteger("Select option: ", option))
    {
        return;
    }

    auto members = db.getTable("members");

    switch (option)
    {
    case 1:
    {
        if (!input::readText("Enter New Name: ", memberName))
        {
            return;
        }
        auto result = members.update()
            .set("name", memberName)
            .where("member_id = :memberId")
            .bind("memberId", memberId)
            .execute();
        if (result.getAffectedItemsCount() == 0)
        {
            std::cout << "Member not found or the name is unchanged.\n";
            return;
        }
        break;
    }
    case 2:
    {
        if (!input::readText("Enter New Contact: ", memberContact))
        {
            return;
        }
        auto contactResult = members.update()
            .set("contact", memberContact)
            .where("member_id = :memberId")
            .bind("memberId", memberId)
            .execute();
        if (contactResult.getAffectedItemsCount() == 0)
        {
            std::cout << "Member not found or the contact is unchanged.\n";
            return;
        }
        break;
    }
    default:
        std::cout << "Please select option 1 or 2.\n";
        return;
    }

    std::cout << "Member information updated successfully.\n";
}

// =================================
//           Delete Member
// =================================

void library::deleteMember(mysqlx::Schema& db){
    if (!input::readInteger("Enter Member ID to delete: ", memberId))
    {
        return;
    }

    auto issues = db.getTable("book_issues").select("issue_id")
        .where("member_id = :memberId")
        .bind("memberId", memberId)
        .execute();
    auto issue = issues.fetchOne();

    auto returns = db.getTable("returned_books").select("return_id")
        .where("member_id = :memberId")
        .bind("memberId", memberId)
        .execute();
    auto returned = returns.fetchOne();

    if (issue || returned)
    {
        std::cout << "Cannot delete a member with checkout or return history.\n";
        return;
    }

    auto members = db.getTable("members");
    auto result = members.remove()
        .where("member_id = :memberId")
        .bind("memberId", memberId)
        .execute();

    if (result.getAffectedItemsCount() == 0)
    {
        std::cout << "Member not found.\n";
        return;
    }

    std::cout << "Member deleted successfully.\n";
}

// =================================
//             Update Book
// =================================

void library::updateBook(mysqlx::Schema& db){
    if (!input::readInteger("Enter Book ID: ", bookID))
    {
        return;
    }

    auto books = db.getTable("books");
    auto bookResult = books.select("book_id")
        .where("book_id = :bookID")
        .bind("bookID", bookID)
        .execute();
    auto book = bookResult.fetchOne();

    if (!book)
    {
        std::cout << "Book not found.\n";
        return;
    }

    std::cout << "1. Update Title\n";
    std::cout << "2. Update Author\n";
    std::cout << "3. Update Total Copies\n";
    if (!input::readInteger("Select option: ", option))
    {
        return;
    }

    switch (option)
    {
    case 1:
        if (!input::readText("Enter New Title: ", bookName))
        {
            return;
        }
        books.update()
            .set("title", bookName)
            .where("book_id = :bookID")
            .bind("bookID", bookID)
            .execute();
        break;
    case 2:
        if (!input::readText("Enter New Author: ", autherName))
        {
            return;
        }
        books.update()
            .set("author", autherName)
            .where("book_id = :bookID")
            .bind("bookID", bookID)
            .execute();
        break;
    case 3:
    {
        if (!input::readInteger("Enter New Total Copies: ", bookStock, 0))
        {
            return;
        }

        auto result = books.select("total_copies", "available_copies")
            .where("book_id = :bookID")
            .bind("bookID", bookID)
            .execute();
        auto copies = result.fetchOne();

        unsigned int totalCopies = copies[0].get<unsigned int>();
        unsigned int availableCopies = copies[1].get<unsigned int>();
        unsigned int checkedOutCopies = totalCopies - availableCopies;

        if (static_cast<unsigned int>(bookStock) < checkedOutCopies)
        {
            std::cout << "Total copies cannot be less than the number currently checked out.\n";
            return;
        }

        unsigned int newAvailableCopies = static_cast<unsigned int>(bookStock) - checkedOutCopies;
        books.update()
            .set("total_copies", static_cast<unsigned int>(bookStock))
            .set("available_copies", newAvailableCopies)
            .where("book_id = :bookID")
            .bind("bookID", bookID)
            .execute();
        break;
    }
    default:
        std::cout << "Please select option 1, 2, or 3.\n";
        return;
    }

    std::cout << "Book information updated successfully.\n";
}

// =================================
//          Member History
// =================================

void library::showMemberHistory(mysqlx::Schema& db){
    if (!input::readInteger("Enter Member ID: ", memberId))
    {
        return;
    }

    auto members = db.getTable("members");
    auto memberResult = members.select("member_id", "name")
        .where("member_id = :memberId")
        .bind("memberId", memberId)
        .execute();
    auto memberRow = memberResult.fetchOne();

    if (!memberRow)
    {
        std::cout << "Member not found.\n";
        return;
    }

    std::cout << "\nHistory for "
              << memberRow[1].get<std::string>()
              << " (ID " << memberRow[0].get<unsigned int>() << ")\n";

    auto issues = db.getTable("book_issues");
    auto issueResult = issues.select(
            "book_id",
            "DATE_FORMAT(issue_date, '%Y-%m-%d %H:%i:%s') AS issue_date_text")
        .where("member_id = :memberId")
        .bind("memberId", memberId)
        .execute();
    auto issueRows = issueResult.fetchAll();

    std::cout << "Checked out:\n";
    bool hasIssues = false;
    for (const auto &row : issueRows)
    {
        hasIssues = true;
        std::cout << "Book " << row[0].get<unsigned int>()
                  << " on " << row[1].get<std::string>() << '\n';
    }
    if (!hasIssues)
    {
        std::cout << "No checkout records.\n";
    }

    auto returns = db.getTable("returned_books");
    auto returnResult = returns.select(
            "book_id",
            "DATE_FORMAT(return_date, '%Y-%m-%d %H:%i:%s') AS return_date_text")
        .where("member_id = :memberId")
        .bind("memberId", memberId)
        .execute();
    auto returnRows = returnResult.fetchAll();

    std::cout << "Returned:\n";
    bool hasReturns = false;
    for (const auto &row : returnRows)
    {
        hasReturns = true;
        std::cout << "Book " << row[0].get<unsigned int>()
                  << " on " << row[1].get<std::string>() << '\n';
    }
    if (!hasReturns)
    {
        std::cout << "No return records.\n";
    }
}