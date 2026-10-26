#include <cstdlib>
#include <charconv>
#include <iostream>
#include <limits>
#include <stdexcept>
#include <string>
#include "library.h++"
#include "member.h++"
#include <mysqlx/xdevapi.h>

namespace
{
std::string environmentValue(const char *name, const char *fallback)
{
    const char *value = std::getenv(name);
    return value == nullptr ? fallback : value;
}

unsigned int databasePort()
{
    const std::string value = environmentValue("DB_PORT", "33060");
    unsigned int port = 0;
    const auto result = std::from_chars(
        value.data(), value.data() + value.size(), port);
    if (result.ec != std::errc{} || result.ptr != value.data() + value.size() ||
        port == 0 || port > 65535)
    {
        throw std::invalid_argument("DB_PORT must be between 1 and 65535.");
    }
    return port;
}
}

int main()
{
    library libraryManager;
    member memberManager;

    try
    {
        const std::string host = environmentValue("DB_HOST", "127.0.0.1");
        const std::string user = environmentValue("DB_USER", "root");
        const std::string password = environmentValue("DB_PASSWORD", "");
        const std::string database =
            environmentValue("DB_NAME", "library_management");
        const unsigned int port = databasePort();

        mysqlx::Session session(host, port, user, password);
        auto db = session.getSchema(database);
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
                      << "13. Active loans          14. Returned loans\n"
                      << "15. Overdue loans / fines 16. Member history\n"
                      << " 0. Exit\n"
                      << "Select an option: ";

            int option;
            if (!(std::cin >> option))
            {
                if (std::cin.eof())
                {
                    break;
                }
                std::cin.clear();
                std::cin.ignore(
                    std::numeric_limits<std::streamsize>::max(), '\n');
                std::cout << "Please enter a number from 0 to 16.\n";
                continue;
            }
            std::cin.ignore(
                std::numeric_limits<std::streamsize>::max(), '\n');

            try
            {
                switch (option)
                {
                case 1:
                    libraryManager.addMember(db);
                    break;
                case 2:
                    libraryManager.showAllMembers(db);
                    break;
                case 3:
                    libraryManager.searchMembers(db);
                    break;
                case 4:
                    libraryManager.updateMembersInfo(db);
                    break;
                case 5:
                    libraryManager.deleteMember(db);
                    break;
                case 6:
                    libraryManager.addBook(db);
                    break;
                case 7:
                    libraryManager.showAllBooks(db);
                    break;
                case 8:
                    libraryManager.searchBooks(db);
                    break;
                case 9:
                    libraryManager.updateBook(db);
                    break;
                case 10:
                    libraryManager.deleteBook(db);
                    break;
                case 11:
                    memberManager.bookIssue(db);
                    break;
                case 12:
                    memberManager.bookReturn(db);
                    break;
                case 13:
                    memberManager.showActiveLoans(db);
                    break;
                case 14:
                    memberManager.showReturnedLoans(db);
                    break;
                case 15:
                    memberManager.showOverdueLoans(db);
                    break;
                case 16:
                    libraryManager.showMemberHistory(db);
                    break;
                case 0:
                    std::cout << "Goodbye!\n";
                    running = false;
                    break;
                default:
                    std::cout << "Please select a valid option from 0 to 16.\n";
                    break;
                }
            }
            catch (const mysqlx::Error &error)
            {
                std::cerr << "Error: " << error.what() << '\n';
            }
        }
    }
    catch (const mysqlx::Error &error)
    {
        std::cerr << "Could not connect to MySQL: " << error.what() << '\n';
        return 1;
    }
    catch (const std::exception &error)
    {
        std::cerr << "Invalid database configuration: "
                  << error.what() << '\n';
        return 1;
    }

    return 0;
}
