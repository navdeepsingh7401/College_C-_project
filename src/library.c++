#include <iostream>
#include <string>
#include "../include/library.h++"
#include <mysqlx/xdevapi.h> 




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
    std::cout << "Enter Member Name  : ";
    getline(std::cin,memberName);

    std::cout << "Enter  Member ID   : ";
    std::cin >> memberId;

    std::cout << "Enter Contact  +91 : ";
    std::cin >> memberContact;



    auto addMembers = db.getTable("members");       // point to the data base tabe and store in the addMembers variable 
    addMembers.insert("member_id","name","contact")     // using .insert("columName","columname") method to insert data 
        .values(memberId,memberName,memberContact)      // use  .values(variable , virable )  to set variable 
        .execute();  // execute the sql coe 


    std::cout << std::endl;
    std::cout << "Member details added successfully!" << std::endl;
    std::cout << std::endl;

}


