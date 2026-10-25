#include <iostream>
#include <string>
#include <limits>
#include "../include/library.h++"
#include <mysqlx/xdevapi.h>
#include <iomanip>


void library::addBook(mysqlx::Schema& db){

    std::cout << "==== Add Book  ====" << std::endl;

    std::cout << "Enter book Name        : ";
    getline(std::cin,bookName);

    std::cout << "Enter book Auther Name : ";
    getline(std::cin,autherName);

    std::cout << "Enter book bookID      : ";
    std::cin >> bookID;

    std::cout << "Enter book bookStock   : ";
    std::cin >> bookStock;

    std::cout << std::endl;
    std::cout << "Book added successfully!" << std::endl;
    std::cout << std::endl;

    auto addBook = db.getTable("books");
    addBook.insert("book_id","title","author","total_copies","available_copies")
        .values(bookID,bookName,autherName,bookStock,bookStock)
        .execute();

    std::cout << std::endl;
    std::cout << "Book details added successfully!" << std::endl;
    std::cout << std::endl;
}

// fucnction for add Member 

void library::addMember(mysqlx::Schema& db){
    // taking input
    std::cout << "==== Add Member  ====" << std::endl;

    std::cout << "Enter  Member ID   : ";
    std::cin >> memberId;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    std::cout << "Enter Member Name  : ";
    getline(std::cin, memberName);

    std::cout << "Enter Contact  +91 : ";
    std::cin >> memberContact;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

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

    std::cout << "Enter Member id : ";
    std::cin >> memberId;

    auto searchMembers = db.getTable("members");
    auto result = searchMembers.select("*")
    .where("member_id = :memberId")
    .bind("memberId", memberId)
    .execute();
    auto allRows = result.fetchAll();


    std::cout << std::setw(30) << " Search  Members List" << std::endl;
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