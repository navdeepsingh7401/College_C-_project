#include <iostream>
#include <fstream>
#include <limits>
#include "library.h++"
#include "member.h++"
using namespace std;

int main() {
    library l;
    member m;
    int option;

    cout << "==== Operation ====" << endl;

    cout << "1 Add Member " << endl;
    cout << "2 Add books " << endl;
    cout << "3 Book Issue" << endl;
    cout << "4 Book Submition" << endl;
    cout << "select option : ";
    cin >> option;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');    // remove the left over white space of getline 


    switch (option)
    {
    case 1:
        l.addMember();
        break;

    case 2:
        l.addBook();
        break;

    case 3:
        m.bookIssue();
        break;

    case 4:
        m.bookReturn();
        break;

    default:
        cout << "please Select correct option " << endl;
        break;
    }
    return 0;
}