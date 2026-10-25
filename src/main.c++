#include <iostream>
#include <fstream>
#include <limits>           // set litmit of code
#include "library.h++"      // custom header file 1
#include "member.h++"       //custom header file 2
#include <mysqlx/xdevapi.h> // help to connect with data base provide api
using namespace std;

int main()
{
    library l;
    member m;

    // connecting database

    try
    {
        mysqlx::Session session(
            "mysqlx://root:sumqes-jyGgi0-wodsuv@127.0.0.1:33060");
        // connect variable with schema
        auto db = session.getSchema("library_management");

        int option = -1;

        std::cout << "\n========== Library Management ==========\n"
              << " 1. Add member             2. List members\n"
              << " 3. Search members         4. Update member\n"
              << " 5. Delete member          6. Add book\n"
              << " 7. List books             8. Search books\n"
              << " 9. Update book           10. Delete book\n"
              << "11. Check out book        12. Return book\n"
              << "13. Active Fine          14. Returned Fine\n"
              << "15. Overdue Fine         16. Member history\n"
              << " 0. Exit\n";

        cout << "select option : ";
        if (!(cin >> option))
        {
            std::cerr << "Invalid option. Please enter a number from 0 to 16.\n";
            return 1;
        }
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        switch (option)
        {
        case 1:
            l.addMember(db);
            break;
        case 2:
            l.showAllMembers(db);
            break;
        case 3:
            cout << "Search members is not implemented yet.\n";
            break;
        case 4:
            cout << "Update member is not implemented yet.\n";
            break;
        case 5:
            cout << "Delete member is not implemented yet.\n";
            break;
        case 6:
            l.addBook(db);
            break;
        case 7:
            cout << "List books is not implemented yet.\n";
            break;
        case 8:
            cout << "Search books is not implemented yet.\n";
            break;
        case 9:
            cout << "Update book is not implemented yet.\n";
            break;
        case 10:
            cout << "Delete book is not implemented yet.\n";
            break;
        case 11:
            m.bookIssue(db);
            break;
        case 12:
            m.bookReturn(db);
            break;
        case 13:
            cout << "Active loans is not implemented yet.\n";
            break;
        case 14:
            cout << "Returned loans is not implemented yet.\n";
            break;
        case 15:
            cout << "Overdue loans is not implemented yet.\n";
            break;
        case 16:
            cout << "Member history is not implemented yet.\n";
            break;
        case 0:
            cout << "Goodbye!\n";
            break;
        default:
            cout << "Please select a valid option from 0 to 16.\n";
            break;
        }
    }
    catch (const mysqlx::Error &e)
    {
        std::cerr << "MySQL error: " << e.what() << '\n';
        return 1;
    }

    return 0;
}