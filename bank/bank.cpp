#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib>
#include <iomanip>
#include <cstring>
#include <conio.h>
#include <sstream>
#include <map>

using namespace std;

// Structure for Staff and User details
struct Details {
    string username;
    string name;
    string address;
    string password;
};

struct Account {
    int accountNumber;
    string name;
    string address;
    string password;
    float balance;
};

struct Node {
    Account data;
    Node* left;
    Node* right;
};

void adminLogin();
void staffLogin();
void userLogin();
void adminMenu();
void staffMenu();
void userMenu(int accountNumber);
bool isUsernameExist(const string& username);
void createStaffRecord();
void modifyStaffRecord();
void displayStaffRecord();
void displayAllStaff();
void deleteStaffRecord();
void createAccount();
void updateAccount();
void displayAccount();
void displayAllAccounts();
void deleteAccount();
void displayUserAccount(int accountNumber);
void displayBalance(int accountNumber);
void transferAmount(int accountNumber);
void depositAmount(int accountNumber);
void withdrawAmount(int accountNumber);
void hidePassword(string&);
bool authenticate(string, string);
Node* createNode(const Account& account);
Node* insertNode(Node* root, const Account& account);
bool isAccountNumberExist(Node* root, int accountNumber);
Node* findMin(Node*);
Node* deleteNode(Node*, int);
Node* searchAccount(Node* root, int accountNumber, Account& foundAccount);
void freeBST(Node* );
void displayInorder(Node* root, ofstream& output);
void displayAllAccountsInOrder(Node* root);

const char DELIMITER = '|';

const string ADMIN_PASSWORD = "admin123";
const string STAFF_FILE = "staff.txt";
const string ACCOUNT_FILE = "account.txt";

void adminLogin() {
    string password;
    cout << "Enter admin password: ";
    hidePassword(password);

    if (password == ADMIN_PASSWORD) {
        adminMenu();
    } else {
        cout << "Invalid password. Access denied." << endl;
    }
}

void staffLogin() {
    string username, password;
    cout << "Enter username: ";
    cin >> username;
    cout << "Enter password: ";
    hidePassword(password);

    ifstream file(STAFF_FILE);
    if (!file) {
        cout << "Error opening staff file." << endl;
        return;
    }

    bool authenticated = false;
    Details staff;
    while (getline(file, staff.username, DELIMITER) && getline(file, staff.name, DELIMITER) &&
           getline(file, staff.address, DELIMITER) && getline(file, staff.password)) {
        if (staff.username == username && authenticate(staff.password, password)) {
            authenticated = true;
            break;
        }
    }

    file.close();

    if (authenticated) {
        staffMenu();
    } else {
        cout << "Invalid username or password. Access denied." << endl;
    }
}

void userLogin() {
    int accountNumber;
    string password;
    cout << "Enter account number: ";
    cin >> accountNumber;
    cout << "Enter password: ";
    hidePassword(password);

    ifstream file(ACCOUNT_FILE);
    if (!file) {
        cout << "Error opening user file." << endl;
        return;
    }

    bool authenticated = false;
    Account account;
    string line;
    while (getline(file, line)) {
        stringstream ss(line);
        string token;
        getline(ss, token, DELIMITER);
        account.accountNumber = stoi(token);
        getline(ss, account.name, DELIMITER);
        getline(ss, account.address, DELIMITER);
        getline(ss, account.password, DELIMITER);
        getline(ss, token);
        account.balance = stof(token);

        if (account.accountNumber == accountNumber && authenticate(account.password, password)) {
            authenticated = true;
            break;
        }
    }

    file.close();

    if (authenticated) {
        userMenu(accountNumber);
    } else {
        cout << "Invalid account number or password. Access denied." << endl;
    }
}

void adminMenu() {
    int choice;
    do {
        cout << "\n********** ADMIN MENU **********" << endl;
        cout << "1. Create Staff Record" << endl;
        cout << "2. Modify Staff Record" << endl;
        cout << "3. Display Staff Record" << endl;
        cout << "4. Delete Staff Record" << endl;
        cout << "5. Display All Staff" << endl;
        cout << "6. Exit" << endl;
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice) {
            case 1:
                createStaffRecord();
                break;
            case 2:
                modifyStaffRecord();
                break;
            case 3:
                displayStaffRecord();
                break;
            case 4:
                deleteStaffRecord();
                break;
            case 5:
                displayAllStaff();
                break;
            case 6:
                cout << "Exiting admin menu..." << endl;
                break;
            default:
                cout << "Invalid choice. Please try again." << endl;
        }
    } while (choice != 6);
}

void staffMenu() {
    int choice;
    do {
        cout << "\n********** STAFF MENU **********" << endl;
        cout << "1. Create Account" << endl;
        cout << "2. Update Account" << endl;
        cout << "3. Display Account" << endl;
        cout << "4. Display All Account" << endl;
        cout << "5. Delete Account" << endl;
        cout << "6. Exit" << endl;
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice) {
            case 1:
                createAccount();
                break;
            case 2:
                updateAccount();
                break;
            case 3:
                displayAccount();
                break;
            case 4:
                displayAllAccounts();
                break;
            case 5:
                deleteAccount();
                break;
            case 6:
                cout << "Exiting staff menu..." << endl;
                break;
            default:
                cout << "Invalid choice. Please try again." << endl;
        }
    } while (choice != 6);
}

void userMenu(int accountNumber) {
    int choice;
    do {
        cout << "\n********** USER MENU **********" << endl;
        cout << "1. Display Account Details" << endl;
        cout << "2. Display Balance" << endl;
        cout << "3. Transfer Amount" << endl;
        cout << "4. Deposit Amount" << endl;
        cout << "5. Withdraw Amount" << endl;
        cout << "6. Exit" << endl;
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice) {
            case 1:
                displayUserAccount(accountNumber); 
                break;
            case 2:
                displayBalance(accountNumber);
                break;
            case 3:
                transferAmount(accountNumber);
                break;
            case 4:
                depositAmount(accountNumber);
                break;
            case 5:
                withdrawAmount(accountNumber);
                break;
            case 6:
                cout << "Exiting user menu..." << endl;
                break;
            default:
                cout << "Invalid choice. Please try again." << endl;
        }
    } while (choice != 6);
}

bool isUsernameExist(const string& username) {
    ifstream file(STAFF_FILE);
    if (!file) {
        cout << "Error opening staff file." << endl;
        return false;
    }

    string existingUsername;
    while (getline(file, existingUsername, DELIMITER)) {
        file.ignore(1000, '\n'); 
        if (existingUsername == username) {
            file.close();
            return true;
        }
    }
    file.close();
    return false;
}

void createStaffRecord() {
    Details staff;
    cout << "\nEnter Staff Details:" << endl;
    cout << "Username: ";
    cin.ignore();
    getline(cin, staff.username);
    cout << "Name: ";
    if (isUsernameExist(staff.username)) {
        cout << "Username already exists. Please choose a different username." << endl;
        return;
    }
    getline(cin, staff.name);
    cout << "Address: ";
    getline(cin, staff.address);
    cout << "Password: ";
    getline(cin, staff.password);
    if (staff.password.length() < 8) {
        cout << "Password must be at least 8 characters long." << endl;
        return;
    }

     ofstream file(STAFF_FILE, ios::app);
    if (!file) {
        cout << "Error opening staff file." << endl;
        return;
    }

    file << staff.username << DELIMITER << staff.name << DELIMITER << staff.address << DELIMITER << staff.password << endl;

    file.close();

    cout << "Staff record created successfully!" << endl;
}

void modifyStaffRecord() {
    string username;
    cout << "\nEnter the username of the staff to modify: ";
    cin.ignore();
    getline(cin, username);

    ifstream inputFile(STAFF_FILE);
    if (!inputFile) {
        cout << "Error opening staff file." << endl;
        return;
    }

    ofstream outputFile(STAFF_FILE + ".tmp");
    if (!outputFile) {
        cout << "Error creating temporary file." << endl;
        return;
    }

    bool found = false;
    Details staff;
    string line;
    while (getline(inputFile, line)) {
        stringstream ss(line);
        getline(ss, staff.username, DELIMITER);
        getline(ss, staff.name, DELIMITER);
        getline(ss, staff.address, DELIMITER);
        getline(ss, staff.password);

        if (staff.username == username) {
            found = true;
            cout << "\nEnter new details for the staff:" << endl;
            cout << "Name: ";
            getline(cin, staff.name);
            cout << "Address: ";
            getline(cin, staff.address);
            cout << "Password: ";
            getline(cin, staff.password);
            if (staff.password.length() < 8) {
                cout << "Password must be at least 8 characters long." << endl;
                return;
            }  
            
            outputFile << staff.username << DELIMITER << staff.name << DELIMITER << staff.address << DELIMITER << staff.password << endl;

            cout << "Staff record modified successfully!" << endl;
        } else {
            outputFile << line << endl;
        }
    }

    inputFile.close();
    outputFile.close();

    if (found) {
        if (remove(STAFF_FILE.c_str()) != 0) {
            cout << "Error deleting staff file." << endl;
            return;
        }
        if (rename((STAFF_FILE + ".tmp").c_str(), STAFF_FILE.c_str()) != 0) {
            cout << "Error renaming temporary file." << endl;
            return;
        }
    } else {
        cout << "Staff with the username '" << username << "' not found." << endl;
        remove((STAFF_FILE + ".tmp").c_str());  
    }
}

void displayStaffRecord() {
    string username;
    cout << "\nEnter the username of the staff to display: ";
    cin.ignore();
    getline(cin, username);

    ifstream file(STAFF_FILE);
    if (!file) {
        cout << "Error opening staff file." << endl;
        return;
    }

    bool found = false;
    Details staff;
    while (getline(file, staff.username, DELIMITER) && getline(file, staff.name, DELIMITER) &&
           getline(file, staff.address, DELIMITER) && getline(file, staff.password)) {
        if (staff.username == username) {
            found = true;
            break;
        }
    }

    if (found) {
        cout << "\nStaff Details:" << endl;
        cout << "Username: " << staff.username << endl;
        cout << "Name: " << staff.name << endl;
        cout << "Address: " << staff.address << endl;
        cout << "Password: "<< staff.password << endl;
    } else {
        cout << "Staff with the username '" << username << "' not found." << endl;
    }

    file.close();
}

void displayAllStaff() {
    ifstream file(STAFF_FILE);
    if (!file) {
        cout << "Error opening staff file." << endl;
        return;
    }

    multimap<string, Details> sortedStaff;

    Details staff;
    int count = 0;

    while (getline(file, staff.username, DELIMITER) && getline(file, staff.name, DELIMITER) &&
           getline(file, staff.address, DELIMITER) && getline(file, staff.password)) {
        sortedStaff.insert(make_pair(staff.username, staff));
    }

    file.close();

    if (sortedStaff.empty()) {
        cout << "No staff records found." << endl;
        return;
    }

    cout << "\n********** ALL STAFF **********" << endl;
    for (const auto& entry : sortedStaff) {
        cout << "\nStaff " << ++count << ":" << endl;
        cout << "Username: " << entry.second.username << endl;
        cout << "Name: " << entry.second.name << endl;
        cout << "Address: " << entry.second.address << endl;
        cout << "Password: " << entry.second.password << endl;
    }

    cout << "\nTotal staff count: " << count << endl;
}

void deleteStaffRecord() {
    string username;
    cout << "\nEnter the username of the staff to delete: ";
    cin.ignore();
    getline(cin, username);

    ifstream file(STAFF_FILE);
    if (!file) {
        cout << "Error opening staff file." << endl;
        return;
    }

    bool found = false;
    Details staff;
    while (getline(file, staff.username, DELIMITER) && getline(file, staff.name, DELIMITER) &&
           getline(file, staff.address, DELIMITER) && getline(file, staff.password)) {
        if (staff.username == username) {
            found = true;
            break;
        }
    }

    file.close();

    if (found) {
        ifstream inputFile(STAFF_FILE);
        ofstream tempFile("temp.txt");

        while (getline(inputFile, staff.username, DELIMITER) && getline(inputFile, staff.name, DELIMITER) &&
               getline(inputFile, staff.address, DELIMITER) && getline(inputFile, staff.password)) {
            if (staff.username != username) {
                tempFile << staff.username << DELIMITER << staff.name << DELIMITER << staff.address << DELIMITER << staff.password << endl;
            }
        }

        inputFile.close();
        tempFile.close();

        remove(STAFF_FILE.c_str());
        rename("temp.txt", STAFF_FILE.c_str());

        cout << "Staff record deleted successfully!" << endl;
    } else {
        cout << "Staff with the username '" << username << "' not found." << endl;
    }
}

void createAccount() {
    Account account;
    cout << "\nEnter Account Details:" << endl;
    cout << "Account Number: ";
    cin >> account.accountNumber;
    ifstream file(ACCOUNT_FILE);
    if (file) {
        int existingAccountNumber;
        while (file >> existingAccountNumber) {
            if (existingAccountNumber == account.accountNumber) {
                cout << "Account Number already exists. Please choose a different Account Number." << endl;
                return;
            }
            file.ignore(1000, '\n'); 
        }
    }
    file.close();
    cout << "Name: ";
    cin.ignore();
    getline(cin, account.name);
    cout << "Address: ";
    getline(cin, account.address);
    cout << "Password: ";
    getline(cin, account.password);
    if (account.password.length() < 8) {
        cout << "Password must be at least 8 characters long." << endl;
        return;
    }
    cout << "Initial Balance: ";
    cin >> account.balance;

    Node* root = nullptr;
    root = insertNode(root, account);

    ofstream outfile(ACCOUNT_FILE, ios::app);
    if (!outfile) {
        cout << "Error opening account file." << endl;
        return;
    }

    outfile << account.accountNumber << DELIMITER << account.name << DELIMITER << account.address << DELIMITER << account.password << DELIMITER << account.balance << endl;

    outfile.close();

    cout << "Account created successfully!" << endl;
}

void updateAccount() {
    int accountNumber;
    cout << "\nEnter the account number to update: ";
    cin >> accountNumber;

    ifstream file(ACCOUNT_FILE);
    if (!file) {
        cout << "Error opening account file." << endl;
        return;
    }

    bool found = false;
    Account account;
    string line;
    Node* root = nullptr;

    while (getline(file, line)) {
        stringstream ss(line);
        string token;
        getline(ss, token, DELIMITER);
        account.accountNumber = stoi(token);
        getline(ss, account.name, DELIMITER);
        getline(ss, account.address, DELIMITER);
        getline(ss, account.password, DELIMITER);
        getline(ss, token);
        account.balance = stof(token);

        root = insertNode(root, account);

        if (account.accountNumber == accountNumber) {
            found = true;
            break;
        }
    }

    file.close();

    if (found) {
        cout << "\nEnter new details for the account:" << endl;
        cin.ignore(); 
        cout << "Name: ";
        getline(cin, account.name);
        cout << "Address: ";
        getline(cin, account.address);
        cout << "Password: ";
        getline(cin, account.password);
        if (account.password.length() < 8) {
            cout << "Password must be at least 8 characters long." << endl;
            return;
        }

        root = deleteNode(root, accountNumber);
        root = insertNode(root, account);

        ofstream output(ACCOUNT_FILE);
        if (!output) {
            cout << "Error opening account file." << endl;
            return;
        }

        displayInorder(root, output);
        output.close();

        freeBST(root);

        cout << "Account updated successfully!" << endl;
    } else {
        cout << "Account with the number '" << accountNumber << "' not found." << endl;
    }
}

void displayAccount() {
    int accountNumber;
    cout << "\nEnter the account number to display: ";
    cin >> accountNumber;

    ifstream file(ACCOUNT_FILE);
    if (!file) {
        cout << "Error opening account file." << endl;
        return;
    }

    bool found = false;
    Account account;
    string line;
    while (getline(file, line)) {
        stringstream ss(line);
        string token;
        getline(ss, token, DELIMITER);
        account.accountNumber = stoi(token);
        getline(ss, account.name, DELIMITER);
        getline(ss, account.address, DELIMITER);
        getline(ss, account.password, DELIMITER);
        getline(ss, token);
        account.balance = stof(token);

        if (account.accountNumber == accountNumber) {
            found = true;
            break;
        }
    }

    file.close();

    if (found) {
        cout << "\nAccount Details:" << endl;
        cout << "Account Number: " << account.accountNumber << endl;
        cout << "Name: " << account.name << endl;
        cout << "Address: " << account.address << endl;
        cout << "Password: " << account.password << endl;
        cout << "Balance: " << fixed << setprecision(2) << account.balance << endl;
    } else {
        cout << "Account with the number '" << accountNumber << "' not found." << endl;
    }
}

void displayAllAccounts() {
    ifstream file(ACCOUNT_FILE);
    if (!file) {
        cout << "Error opening account file." << endl;
        return;
    }

    Node* root = nullptr;
    string line;
    while (getline(file, line)) {
        stringstream ss(line);
        string token;
        Account account;

        getline(ss, token, DELIMITER);
        account.accountNumber = stoi(token);
        getline(ss, account.name, DELIMITER);
        getline(ss, account.address, DELIMITER);
        getline(ss, account.password, DELIMITER);
        getline(ss, token);
        account.balance = stof(token);

        root = insertNode(root, account);
    }

    file.close();

    cout << "\n********** ALL ACCOUNTS **********" << endl;
    if (root) {
        displayAllAccountsInOrder(root);
        freeBST(root);
    } else {
        cout << "No accounts found." << endl;
    }
}

void deleteAccount() {
    int accountNumber;
    cout << "\nEnter the account number to delete: ";
    cin >> accountNumber;

    ifstream file(ACCOUNT_FILE);
    if (!file) {
        cout << "Error opening account file." << endl;
        return;
    }

    bool found = false;
    Account account;
    string line;
    while (getline(file, line)) {
        stringstream ss(line);
        string token;
        getline(ss, token, DELIMITER);
        account.accountNumber = stoi(token);
        getline(ss, account.name, DELIMITER);
        getline(ss, account.address, DELIMITER);
        getline(ss, account.password, DELIMITER);
        getline(ss, token);
        account.balance = stof(token);

        if (account.accountNumber == accountNumber) {
            found = true;
            break;
        }
    }

    file.close();

    if (found) {
        ifstream inputFile(ACCOUNT_FILE);
        ofstream tempFile("temp.txt");

        while (getline(inputFile, line)) {
            stringstream ss(line);
            string token;
            getline(ss, token, DELIMITER);
            account.accountNumber = stoi(token);
            getline(ss, account.name, DELIMITER);
            getline(ss, account.address, DELIMITER);
            getline(ss, account.password, DELIMITER);
            getline(ss, token);
            account.balance = stof(token);

            if (account.accountNumber != accountNumber) {
                tempFile << account.accountNumber << DELIMITER << account.name << DELIMITER << account.address << DELIMITER << account.password << DELIMITER << account.balance << endl;
            }
        }

        inputFile.close();
        tempFile.close();

        remove(ACCOUNT_FILE.c_str());
        rename("temp.txt", ACCOUNT_FILE.c_str());

        cout << "Account deleted successfully!" << endl;
    } else {
        cout << "Account with the number '" << accountNumber << "' not found." << endl;
    }
}

void displayUserAccount(int accountNumber) {
    ifstream file(ACCOUNT_FILE);
    if (!file) {
        cout << "Error opening account file." << endl;
        return;
    }

    bool found = false;
    Account account;
    string line;
    while (getline(file, line)) {
        stringstream ss(line);
        string token;
        getline(ss, token, DELIMITER);
        account.accountNumber = stoi(token);
        getline(ss, account.name, DELIMITER);
        getline(ss, account.address, DELIMITER);
        getline(ss, account.password, DELIMITER);
        getline(ss, token);
        account.balance = stof(token);

        if (account.accountNumber == accountNumber) {
            found = true;
            break;
        }
    }

    file.close();

    if (found) {
        cout << "\nYour Account Details:" << endl;
        cout << "Account Number: " << account.accountNumber << endl;
        cout << "Name: " << account.name << endl;
        cout << "Address: " << account.address << endl;
        cout << "Password: "<<account.password << endl;
        cout << "Balance: " << fixed << setprecision(2) << account.balance << endl;
    } else {
        cout << "Your account not found." << endl;
    }
}

void displayBalance(int accountNumber) {
    ifstream file(ACCOUNT_FILE);
    if (!file) {
        cout << "Error opening account file." << endl;
        return;
    }

    bool found = false;
    Account account;
    string line;
    while (getline(file, line)) {
        stringstream ss(line);
        string token;
        getline(ss, token, DELIMITER);
        account.accountNumber = stoi(token);
        getline(ss, account.name, DELIMITER);
        getline(ss, account.address, DELIMITER);
        getline(ss, account.password, DELIMITER);
        getline(ss, token);
        account.balance = stof(token);

        if (account.accountNumber == accountNumber) {
            found = true;
            break;
        }
    }

    file.close();

    if (found) {
        cout << "\nAccount Balance: " << fixed << setprecision(2) << account.balance << endl;
    } else {
        cout << "Your account not found." << endl;
    }
}

void transferAmount(int accountNumber) {
    int destinationAccountNumber;
    float amount;

    cout << "\nEnter the destination account number: ";
    cin >> destinationAccountNumber;

    if (destinationAccountNumber == accountNumber) {
        cout << "Cannot transfer to the same account." << endl;
        return;
    }

    Account sourceAccount, destinationAccount;
    Node* root = nullptr;

    ifstream file(ACCOUNT_FILE);
    if (!file) {
        cout << "Error opening account file." << endl;
        return;
    }

    bool sourceFound = false, destinationFound = false;
    string line;
    while (getline(file, line)) {
        stringstream ss(line);
        string token;
        getline(ss, token, DELIMITER);
        sourceAccount.accountNumber = stoi(token);
        getline(ss, sourceAccount.name, DELIMITER);
        getline(ss, sourceAccount.address, DELIMITER);
        getline(ss, sourceAccount.password, DELIMITER);
        getline(ss, token);
        sourceAccount.balance = stof(token);

        root = insertNode(root, sourceAccount);

        if (sourceAccount.accountNumber == accountNumber) {
            sourceFound = true;
        }

        if (sourceAccount.accountNumber == destinationAccountNumber) {
            destinationFound = true;
            destinationAccount = sourceAccount;
        }

        if (sourceFound && destinationFound) {
            break; 
        }
    }

    file.close();

    if (!sourceFound) {
        cout << "Source account not found." << endl;
        freeBST(root);
        return;
    }

    if (!destinationFound) {
        cout << "Destination account not found." << endl;
        freeBST(root);
        return;
    }

    cout << "Enter the amount to transfer: ";
    cin >> amount;

    if (amount <= 0) {
        cout << "Invalid amount. Amount must be greater than 0." << endl;
        freeBST(root);
        return;
    }

    if (sourceAccount.balance < amount) {
        cout << "Insufficient balance in the source account." << endl;
        freeBST(root);
        return;
    }

    sourceAccount.balance -= amount;
    destinationAccount.balance += amount;

    root = deleteNode(root, sourceAccount.accountNumber);
    root = deleteNode(root, destinationAccount.accountNumber);

    root = insertNode(root, sourceAccount);
    root = insertNode(root, destinationAccount);

    ofstream output(ACCOUNT_FILE);
    if (!output) {
        cout << "Error opening account file." << endl;
        freeBST(root);
        return;
    }

    displayInorder(root, output);
    output.close();
    freeBST(root);

    cout << "Amount transferred successfully!" << endl;
}

void depositAmount(int accountNumber) {
    float amount;
    cout << "\nEnter the amount to deposit: ";
    cin >> amount;

    if (amount <= 0) {
        cout << "Invalid amount. Amount must be greater than 0." << endl;
        return;
    }

    Account account;
    Node* root = nullptr;

    ifstream file(ACCOUNT_FILE);
    if (!file) {
        cout << "Error opening account file." << endl;
        return;
    }

    bool found = false;
    string line;
    while (getline(file, line)) {
        stringstream ss(line);
        string token;
        getline(ss, token, DELIMITER);
        account.accountNumber = stoi(token);
        getline(ss, account.name, DELIMITER);
        getline(ss, account.address, DELIMITER);
        getline(ss, account.password, DELIMITER);
        getline(ss, token);
        account.balance = stof(token);

        root = insertNode(root, account);

        if (account.accountNumber == accountNumber) {
            found = true;
            break;
        }
    }

    file.close();

    if (!found) {
        cout << "Account not found." << endl;
        freeBST(root);
        return;
    }

    account.balance += amount;

    root = deleteNode(root, account.accountNumber);
    root = insertNode(root, account);

    ofstream output(ACCOUNT_FILE);
    if (!output) {
        cout << "Error opening account file." << endl;
        freeBST(root);
        return;
    }

    displayInorder(root, output);
    output.close();

    freeBST(root);

    cout << "Amount deposited successfully!" << endl;
}

void withdrawAmount(int accountNumber) {
    float amount;
    cout << "\nEnter the amount to withdraw: ";
    cin >> amount;

    if (amount <= 0) {
        cout << "Invalid amount. Amount must be greater than 0." << endl;
        return;
    }

    Account account;
    Node* root = nullptr;

    ifstream file(ACCOUNT_FILE);
    if (!file) {
        cout << "Error opening account file." << endl;
        return;
    }

    bool found = false;
    string line;
    while (getline(file, line)) {
        stringstream ss(line);
        string token;
        getline(ss, token, DELIMITER);
        account.accountNumber = stoi(token);
        getline(ss, account.name, DELIMITER);
        getline(ss, account.address, DELIMITER);
        getline(ss, account.password, DELIMITER);
        getline(ss, token);
        account.balance = stof(token);

        root = insertNode(root, account);

        if (account.accountNumber == accountNumber) {
            found = true;
            break;
        }
    }

    file.close();

    if (!found) {
        cout << "Account not found." << endl;
        freeBST(root);
        return;
    }

    if (account.balance < amount) {
        cout << "Insufficient balance." << endl;
        freeBST(root);
        return;
    }

    account.balance -= amount;

    root = deleteNode(root, account.accountNumber);
    root = insertNode(root, account);

    ofstream output(ACCOUNT_FILE);
    if (!output) {
        cout << "Error opening account file." << endl;
        freeBST(root);
        return;
    }

    displayInorder(root, output);
    output.close();

    freeBST(root);

    cout << "Amount withdrawn successfully!" << endl;
}

void hidePassword(string& password) {
    char ch;
    password.clear();
    ch = _getch();
    while (ch != 13) { 
        if (ch == 8) { 
            if (!password.empty()) {
                password.pop_back();
                cout << "\b \b"; 
            }
        } else {
            password.push_back(ch);
            cout << '*'; 
        }
        ch = _getch();
    }
    cout << endl;
}

bool authenticate(string storedPassword, string enteredPassword) {
    return storedPassword == enteredPassword;
}

Node* createNode(const Account& account) {
    Node* newNode = new Node;
    newNode->data = account;
    newNode->left = nullptr;
    newNode->right = nullptr;
    return newNode;
}

Node* insertNode(Node* root, const Account& account) {
    if (root == nullptr) {
        return createNode(account);
    }

    if (account.accountNumber < root->data.accountNumber) {
        root->left = insertNode(root->left, account);
    } else if (account.accountNumber > root->data.accountNumber) {
        root->right = insertNode(root->right, account);
    }

    return root;
}

bool isAccountNumberExist(Node* root, int accountNumber) {
    if (root == nullptr) {
        return false;
    }
    if (accountNumber == root->data.accountNumber) {
        return true;
    } else if (accountNumber < root->data.accountNumber) {
        return isAccountNumberExist(root->left, accountNumber);
    } else {
        return isAccountNumberExist(root->right, accountNumber);
    }
}

Node* findMin(Node* root) {
    while (root->left != nullptr) {
        root = root->left;
    }
    return root;
}

Node* deleteNode(Node* root, int accountNumber) {
    if (root == nullptr) {
        return root;
    }

    if (accountNumber < root->data.accountNumber) {
        root->left = deleteNode(root->left, accountNumber);
    } else if (accountNumber > root->data.accountNumber) {
        root->right = deleteNode(root->right, accountNumber);
    } else {
        if (root->left == nullptr) {
            Node* temp = root->right;
            delete root;
            return temp;
        } else if (root->right == nullptr) {
            Node* temp = root->left;
            delete root;
            return temp;
        }

        Node* temp = root->right;
        while (temp->left != nullptr) {
            temp = temp->left;
        }

        root->data = temp->data;
        root->right = deleteNode(root->right, temp->data.accountNumber);
    }
    return root;
}

Node* searchAccount(Node* root, int accountNumber, Account& foundAccount) {
    if (root == nullptr) {
        return nullptr;
    }

    if (root->data.accountNumber == accountNumber) {
        foundAccount = root->data;
        return root;
    } else if (accountNumber < root->data.accountNumber) {
        return searchAccount(root->left, accountNumber, foundAccount);
    } else {
        return searchAccount(root->right, accountNumber, foundAccount);
    }
}

void freeBST(Node* root) {
    if (root != nullptr) {
        freeBST(root->left);
        freeBST(root->right);
        delete root;
    }
}

void displayInorder(Node* root, ofstream& output) {
    if (root) {
        displayInorder(root->left, output);
        output << root->data.accountNumber << DELIMITER << root->data.name << DELIMITER
               << root->data.address << DELIMITER << root->data.password << DELIMITER
               << root->data.balance << endl;
        displayInorder(root->right, output);
    }
}

void displayAllAccountsInOrder(Node* root) {
    if (root) {
        displayAllAccountsInOrder(root->left);

        cout << "\nAccount Number: " << root->data.accountNumber << endl;
        cout << "Name: " << root->data.name << endl;
        cout << "Address: " << root->data.address << endl;
        cout << "Password: " << root->data.password << endl;
        cout << "Balance: " << fixed << setprecision(2) << root->data.balance << endl;

        displayAllAccountsInOrder(root->right);
    }
}
int main() {
    int choice;
    do {
        cout << "\n********** BANK MANAGEMENT SYSTEM **********" << endl;
        cout << "1. Admin Login" << endl;
        cout << "2. Staff Login" << endl;
        cout << "3. User Login" << endl;
        cout << "4. Exit" << endl;
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice) {
            case 1:
                adminLogin();
                break;
            case 2:
                staffLogin();
                break;
            case 3:
                userLogin();
                break;
            case 4:
                cout << "Exiting bank management system..." << endl;
                break;
            default:
                cout << "Invalid choice. Please try again." << endl;
        }
    } while (choice != 4);

    return 0;
}
