#include "utility.h"

string generate_wallet_address() {
    srand(time(NULL));

    //String of Characters 
    static const char alnum[] = 
        "0123456789"
        "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
        "abcdefghijklmnopqrstuvwxyz";

    string wallet_addr;
    wallet_addr.reserve(WALLET_LENGTH);

    for (int i = 0; i < WALLET_LENGTH; i++) {
        wallet_addr += alnum[rand() % (sizeof(alnum)-1)];
    }

    return wallet_addr;
}

string process_wallet_address(Client c) {
    ifstream wallet_file;
    string wallet;
    string new_wallet_addr;

    //Check client wallet address 
    wallet_file.open(WALLET_FILE);
    if (wallet_file.is_open()) {
        wallet_file >> wallet;
    }
    wallet_file.close();

    if (wallet.length() == 0) {
        ofstream wallet_file;
        wallet_file.open(WALLET_FILE);
        if (wallet_file.is_open()) {
            new_wallet_addr = generate_wallet_address();
            wallet_file << new_wallet_addr;
        }
        wallet_file.close();

        //Log Wallet Address to Server -> server adds to database, sets coins
        int addr_len = new_wallet_addr.length();
        char temp_addr[addr_len + 1];

        strcpy(temp_addr, new_wallet_addr.c_str());
        c.log_wallet_address(temp_addr);

        wallet = temp_addr;
    }

    return wallet;
}

string get_wallet_addr() {
    ifstream wallet_file;
    string wallet;

    wallet_file.open(WALLET_FILE);
    if (wallet_file.is_open()) {
        wallet_file >> wallet;
    }
    wallet_file.close();

    return wallet;
}

void calcoin_header() {
    system("clear");

    cout << FRED("  @@@     @@@   @@        @@@     @@@   @@@@@@@@ @@@   @@") << endl;
    cout << FRED(" @@@@@   @@@@@  @@       @@@@@   @@@@@  @@@@@@@@ @@@@  @@") << endl;
    cout << FRED("@@   @@  @@ @@  @@      @@   @@ @@   @@    @@    @@ @@ @@") << endl;
    cout << FRED("@@      @@@ @@@ @@      @@      @@   @@    @@    @@ @@ @@") << endl;
    cout << FRED("@@   @@ @@@@@@@ @@      @@   @@ @@   @@    @@    @@ @@ @@") << endl;
    cout << FRED(" @@@@@  @@   @@ @@@@@@@  @@@@@   @@@@@  @@@@@@@@ @@  @@@@") << endl;
    cout << FRED("  @@@   @@   @@ @@@@@@@   @@@     @@@   @@@@@@@@ @@   @@@") << endl;
    cout << FRED("---------------------------------------------------------") << endl;
}

int menu() {
    bool menu_status = false;
    int choice = 0;
    cout << endl;

    while(menu_status == false) {
        cout << "_____ CalCoin Menu _____" << endl;
        cout << "1.) Send CalCoin" << endl;
        cout << "2.) View Wallet Balance" << endl;
        cout << "3.) Quit" << endl;
        cout << "->";
        cin >> choice;

        if (choice >= 1 && choice <= 3) {
            menu_status = true;
        }
        else {
            system("clear");
            calcoin_header();
        }
    }

    return choice;
}

bool send_coins(Client c) {
    bool status = false;
    string wallet_addr;
    string receiver_addr;
    string amount;

    wallet_addr = get_wallet_addr();

    cout << "Sender Wallet Address: " << wallet_addr << endl;
    cout << "Enter Receiver Address: ";
    cin >> receiver_addr;
    cin.ignore();
    cout << "Enter Transfer Amount: ";
    cin >> amount;

    //Temp Variables
    int s_len = wallet_addr.length();
    int r_addr = receiver_addr.length();
    int a_len = amount.length();

    char temp_sender_addr[s_len + 1];
    char temp_receiver_addr[r_addr + 1];
    char temp_amount[a_len + 1];

    strcpy(temp_sender_addr, wallet_addr.c_str());
    strcpy(temp_receiver_addr, receiver_addr.c_str());
    strcpy(temp_amount, amount.c_str());

    //Send Transaction Data to Server
    cout << endl;
    cout << "__________ Sender Information __________" << endl;
    cout << "Sender Address: " << temp_sender_addr << endl;
    cout << "Receiver Address: " << temp_receiver_addr << endl;
    cout << "Transfer Amount: " << temp_amount << endl;

    c.send_transaction(temp_sender_addr, temp_receiver_addr, temp_amount);

    return status;
}

void view_wallet_balance(Client c) {
    int len;
    string balance;
    string temp_addr;

    temp_addr = get_wallet_addr();

    len = temp_addr.length();
    char wallet_addr[len + 1];
    strcpy(wallet_addr, temp_addr.c_str());

    balance = c.request_balance(wallet_addr);

    //Print the balance
    cout << "Balance: " << balance << endl;
}