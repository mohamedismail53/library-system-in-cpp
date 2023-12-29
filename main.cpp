#include <iostream>
#include <algorithm>


using namespace std;

/* !Comment: Macros for library capacity */
#define MAX_BOOKS           1000
#define MAX_USERS           1000
#define MAX_BORROWED_BOOKS  10

enum menu_options {
    ADD_BOOK = 1,
    SEARCH_BOOK_BY_PREFIX,
    PRINT_WHO_BORROW_BOOK_BY_NAME,
    PRINT_LIBRARY_BY_ID,
    PRINT_LIBRARY_BY_NAME,
    ADD_USER,
    USER_BORROWS_BOOK,
    USER_RETURNS_BOOK,
    PRINT_ALL_USERS,
    EXIT
};

/* !Comment: Library types */
struct Book {
    int id = -1;
    int quantity = 0;
    int total_borrowed = 0;
    string name = "";
};

struct User {
    int national_id = -1;
    int num_borrowed_books = 0;
    string name = "";
    string borrowed_books[MAX_BORROWED_BOOKS];
};

struct Library {
    Book books[MAX_BOOKS];
    User users[MAX_USERS];
    int last_added_book_position = 0;
    int last_added_user_position = 0;
};


int menu() {
    int choice = -1;
    while(choice == -1) {
        cout << "Library Menu:\n";

        cout << "1) Add book\n";
        cout << "2) Search books by prefix\n";
        cout << "3) Print who borrowed book by name\n";
        cout << "4) Print library by id\n";
        cout << "5) Print library by name\n";
        cout << "6) Add User\n";
        cout << "7) User borrow a book\n";
        cout << "8) User returns a book\n";
        cout << "9) Print all users\n";
        cout << "10) Exit\n";

        cout << "\nEnter your menu choice [1 - 10]: ";

        cin >> choice;

        if(!(1 <= choice && choice <= 10))
            choice = -1;

        cout << endl;
    }
    return choice;
}

void add_book(Library& library) {

    if(library.last_added_book_position < MAX_BOOKS) {
        int id, quantity;
        string name;

        cout << "Entering book info ...\n";
        cout << "Please enter the book id: ";
        cin >> id;
        cout << "Please enter the book name: ";
        cin >> name;
        cout << "Please enter the book quantity: ";
        cin >> quantity;

        library.books[library.last_added_book_position].id = id;
        library.books[library.last_added_book_position].name = name;
        library.books[library.last_added_book_position].quantity = quantity;
        library.books[library.last_added_book_position].total_borrowed = 0;

        library.last_added_book_position++;

    } else {
        cout << "You can't add more books, Library is full\n";
    }

    cout << endl;
}

bool prefix_match(const string& book_name, const string& prefix) {
    if((int)prefix.size() > (int)book_name.size())
        return false;

    for(int i = 0; i < (int)prefix.size(); ++i)
        if(book_name[i] != prefix[i])
            return false;


    return true;
}

void search_book_by_prefix(const Library& library) {
    string prefix;
    cout << "Please enter book name prefix: ";
    cin >> prefix;

    for(int i = 0; i < library.last_added_book_position; ++i)
        if(prefix_match(library.books[i].name, prefix))
            cout << library.books[i].name << "\n";


    cout << "\n";
}

void add_user(Library& library) {
    if(library.last_added_user_position < MAX_USERS) {
        int id;
        string name;

        cout << "Please enter the user name: ";
        cin >> name;
        cout << "Please enter the user national id: ";
        cin >> id;

        library.users[library.last_added_user_position].name = name;
        library.users[library.last_added_user_position].national_id = id;

        for(int i = 0; i < MAX_BORROWED_BOOKS; ++i) {
            library.users[library.last_added_user_position].borrowed_books[i] = "";
        }

        library.last_added_user_position++;

    } else {
        cout << "You can't add more users, no more available place\n";
    }
    cout << endl;
}

void does_user_exist(const Library& library, const string& user_name, int& idx) {
    for(int i = 0; i < library.last_added_user_position; ++i) {
        if(library.users[i].name == user_name) {
            idx = i;
            break;
        }
    }

    if(idx == -1)
        cout << "Please enter a valid user name\n\n";

}

void does_book_exist(const Library& library, const string& book_name, int& idx) {
    for(int i = 0; i < library.last_added_book_position; ++i) {
        if(library.books[i].name == book_name) {
            idx = i;
            break;
        }
    }

    if(idx == -1)
        cout << "Please enter a valid book name\n\n";

}

void user_borrow_book(Library& library) {
    string user_name, book_name;

    cout << "Please enter the user name: ";
    cin >> user_name;
    cout << "Please enter the book name: ";
    cin >> book_name;

    int book_idx = -1, user_idx = -1;

    does_user_exist(library, user_name, user_idx);

    if(user_idx == -1)
        return;

    does_book_exist(library, book_name, book_idx);

    if(book_idx == -1)
        return;

    if(library.books[book_idx].quantity <= 0) {
        cout << "The book can't be borrowed\n\n";
        return;
    }

    if(library.users[user_idx].num_borrowed_books >= MAX_BORROWED_BOOKS) {
        cout << "You can't borrow any more books as you reach the limit\n\n";
        return;
    }

    library.books[book_idx].quantity--;

    for(int i = 0; i < MAX_BORROWED_BOOKS; ++i) {
        if(library.users[user_idx].borrowed_books[i] == "") {
            library.users[user_idx].borrowed_books[i] = book_name;
            library.users[user_idx].num_borrowed_books++;
            break;
        }
    }

    cout << endl;
}

void user_return_book(Library& library) {
    string user_name, book_name;

    cout << "Please enter the user name: ";
    cin >> user_name;
    cout << "Please enter the book name: ";
    cin >> book_name;

    int user_idx = -1, book_idx = -1;

    does_user_exist(library, user_name, user_idx);

    if(user_idx == -1)
        return;

    does_book_exist(library, book_name, book_idx);

    if(book_idx == -1)
        return;

    for(int i = 0; i < MAX_BORROWED_BOOKS; ++i)
        if(library.users[user_idx].borrowed_books[i] == book_name)
            library.users[user_idx].borrowed_books[i] = "";

    library.users[user_idx].num_borrowed_books--;
    library.books[book_idx].quantity++;

    cout << endl;
}

void print_who_borrowed_book_by_name(const Library& library) {
    string book_name;
    int book_idx = -1;
    bool borrowe_flag = false;

    cout << "Please Enter the book name: ";
    cin >> book_name;

    does_book_exist(library, book_name, book_idx);

    if(book_idx == -1)
        return;

    for(int i = 0; i < library.last_added_user_position; ++i)
        for(int j = 0; j < MAX_BORROWED_BOOKS; ++j)
            if(library.users[i].borrowed_books[j] == book_name)
                cout << library.users[i].name << "\n", borrowe_flag = true;

    if(borrowe_flag == false)
        cout << "No one borrowed this book!\n";

    cout << endl;
}

int get_book_id(const Library& library, const string& book_name) {
    for(int i = 0; i < library.last_added_book_position; ++i) {
        if(library.books[i].name == book_name)
            return library.books[i].id;
    }
    return -1;
}

void print_all_users(const Library& library) {
    for(int i = 0; i < library.last_added_user_position; ++i) {
        cout << library.users[i].name << " has id " << library.users[i].national_id;

        bool borrow_flag = false;
        for(int j = 0; j < MAX_BORROWED_BOOKS; ++j) {
            if(borrow_flag == false && library.users[i].borrowed_books[j] != "") {
                cout << " borrowed books with the following ids: " << get_book_id(library, library.users[i].borrowed_books[j]);
                borrow_flag = true;
            } else if(library.users[i].borrowed_books[j] != "") {
                cout << ", " << get_book_id(library, library.users[i].borrowed_books[j]);
            }
        }

        if(borrow_flag == false)
            cout << ", and has no borrowed books";

        cout << "\n";
    }
    cout << endl;
}

void print_library_by_id(Library& library) {

    sort(library.books, library.books + library.last_added_book_position,
         [](const Book& book1, const Book& book2) {
            return book1.id < book2.id;
         });

    for(int i = 0; i < library.last_added_book_position; ++i)
        cout << "User ID = " << library.books[i].id << " , user name = " << library.books[i].name << " , total quantity = " << library.books[i].quantity + library.books[i].total_borrowed << " , number of borrowed books = " << library.books[i].total_borrowed << "\n";

    cout << endl;
}

void print_library_by_name(Library& library) {

    sort(library.books, library.books + library.last_added_book_position,
         [](const Book& book1, const Book& book2) {
            return book1.name < book2.name;
         });

    for(int i = 0; i < library.last_added_book_position; ++i)
        cout << "User ID = " << library.books[i].id << " , user name = " << library.books[i].name << " , total quantity = " << library.books[i].quantity + library.books[i].total_borrowed << " , number of borrowed books = " << library.books[i].total_borrowed << "\n";

    cout << endl;
}

int main()
{
    Library library;
    bool app_running = true;

    while(app_running) {

        int choice = menu();

        switch(choice) {
        case ADD_BOOK:
            add_book(library);
            break;

        case SEARCH_BOOK_BY_PREFIX:
            search_book_by_prefix(library);
            break;

        case PRINT_WHO_BORROW_BOOK_BY_NAME:
            print_who_borrowed_book_by_name(library);
            break;

        case PRINT_LIBRARY_BY_ID:
            print_library_by_id(library);
            break;

        case PRINT_LIBRARY_BY_NAME:
            print_library_by_name(library);
            break;

        case ADD_USER:
            add_user(library);
            break;

        case USER_BORROWS_BOOK:
            user_borrow_book(library);
            break;

        case USER_RETURNS_BOOK:
            user_return_book(library);
            break;

        case PRINT_ALL_USERS:
            print_all_users(library);
            break;

        case EXIT:
            app_running = false;
            break;
        }
    }

    return 0;
}
