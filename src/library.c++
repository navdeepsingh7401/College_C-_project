#include <iostream>
#include <string>
#include "../include/library.h++"



void library::addBook(){
    // taking input 
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

    // displaying the added book details
    std::cout << "Book Name         : " << bookName << std::endl;
    std::cout << "Book Auther Name  : " << autherName << std::endl;
    std::cout << "Book bookID       : " << bookID << std::endl;
    std::cout << "Book bookStock    : " << bookStock << " books"<< std::endl;
}


void library::addMember(){
    // taking input 
    std::cout << "Enter Member Name  : ";
    getline(std::cin,memberName);

    std::cout << "Enter  Member ID   : ";
    std::cin >> memberID;

    std::cout << "Enter Contact  +91 : ";
    std::cin >> memberContact;



    std::cout << std::endl;
    std::cout << "Member details added successfully!" << std::endl;
    std::cout << std::endl;

    // displaying the added book details
    std::cout << "Member Name   : " << memberName << std::endl;
    std::cout << "Member ID     : " << memberID << std::endl;
    std::cout << "Contact       : " << memberContact << std::endl;
}


