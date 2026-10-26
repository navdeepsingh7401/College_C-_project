#include <iostream>
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

        int option;
        bool running = true;

        while (running)
        {
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
                if (cin.eof())
                {
                    break;
                }
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                cout << "Please enter a number from 0 to 16.\n";
                continue;
            }
            cin.ignore(numeric_limits<streamsize>::max(), '\n');

            // =================================
            //          Menu Option Dispatch
            // =================================

            try
            {
                switch (option)
                {
                case 1:
                    l.addMember(db);
                    break;
                case 2:
                    l.showAllMembers(db);
                    break;
                case 3:
                    l.searchMembers(db);
                    break;
                case 4:
                    l.updateMembersInfo(db);
                    break;
                case 5:
                    l.deleteMember(db);
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
                    l.updateBook(db);
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
                    l.showMemberHistory(db);
                    break;
                case 0:
                    cout << "Goodbye!\n";
                    running = false;
                    break;
                default:
                    cout << "Please select a valid option from 0 to 16.\n";
                    break;
                }
            }
            catch (const mysqlx::Error &e)
            {
                std::cerr << "Error: " << e.what() << '\n';
            }
        }
    }
    catch (const mysqlx::Error &e)
    {
        std::cerr << "Could not connect to MySQL: " << e.what() << '\n';
        return 1;
    }

    return 0;
}