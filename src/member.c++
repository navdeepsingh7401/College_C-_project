#include <iostream>
#include <string>
#include "../include/member.h++"
#include <mysqlx/xdevapi.h>     


void member::bookIssue(mysqlx::Schema& db){
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

    // Showing input  detail 
     std::cout << "Book issue saved.\n";
    std::cout << "book Issue Member Id  : " << memberId << std::endl;
    std::cout << "book Issue Id         : " << bookId << std::endl;

};

void member::bookReturn(mysqlx::Schema& db) {
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