#include <iostream>
#include <string>
#include <vector>
#include <fstream>
using namespace std;

const string ClientsFileName = "Clients.txt";

enum enATMMenuChoices { QuickWithdraw = 1 , NormalWithdraw = 2 , Deposit = 3 , CheckBalance = 4 , Logout = 5 };
enum enQuickWithdrawChoices { twenty = 1 , fifty = 2 , hundred = 3 , two_hundred = 4 , four_hundred = 5 , six_hundred = 6 , height_hundred = 7 , one_thousand = 8 , Exit = 9 };

struct stClient {

    string AccountNumber;
    string PinCode;
    string Name;
    string Phone;
    int AccountBalance;
    bool MarkedForDelete = false;

};

stClient currentClient;


string ReadString(string message) {

    string text = "";

    cout << message;
    getline(cin >> ws, text);

    return text;

}

float ReadPositiveNumber(string message) {

    float num = 0;

    do {

        cout << message;
        cin >> num;

    } while (num <= 0);

    return num;

}

enATMMenuChoices ReadATMMainMenuScreenChoice(string message) {

    short choice = 0;

    do {

        cout << message;
        cin >> choice;

    } while (choice < 1 || choice > 5);

    return (enATMMenuChoices)choice;

}

enQuickWithdrawChoices ReadQuickWithdrawChoice(string message) {

    short choice = 0;

    do {

        cout << message;
        cin >> choice;

    } while (choice < 1 || choice > 9);

    return (enQuickWithdrawChoices)choice;

}



void LoginHeader() {

    cout << "\n------------------------------------------------------\n";
    cout << "\t\t\tLogin Screen";
    cout << "\n------------------------------------------------------\n\n";

}

void NormalWithdrawHeader() {

    cout << "\n------------------------------------------------------\n";
    cout << "\t\tNormal Withdraw Screen";
    cout << "\n------------------------------------------------------\n\n";

}

void DepositHeader() {

    cout << "\n------------------------------------------------------\n";
    cout << "\t\t\tDeposit Screen";
    cout << "\n------------------------------------------------------\n\n";

}

void CheckBalanceHeader() {

    cout << "\n------------------------------------------------------\n";
    cout << "\t\t\tCheck Balance Screen";
    cout << "\n------------------------------------------------------\n\n";

}




vector <string> SplitString(string S1, string Seperator = "#//#") {

    vector <string> vString;

    string word = "";
    short pos = 0;

    while ((pos = S1.find(Seperator)) != string::npos) {

        word = S1.substr(0, pos);

        if (word != "")
            vString.push_back(word);

        S1.erase(0, pos + Seperator.length());

    }

    if (S1 != "")
        vString.push_back(S1);

    return vString;

}


stClient ConvertLineToRecord(string LineRecord, string Seperator = "#//#") {

    stClient Client;
    vector <string> vString;

    vString = SplitString(LineRecord, Seperator);

    Client.AccountNumber = vString[0];
    Client.PinCode = vString[1];
    Client.Name = vString[2];
    Client.Phone = vString[3];
    Client.AccountBalance = stoi(vString[4]);

    return Client;

}

string ConvertRecordToLine(stClient Client, string Seperator = "#//#") {

    string stClientRecord;

    stClientRecord += Client.AccountNumber + Seperator;
    stClientRecord += Client.PinCode + Seperator;
    stClientRecord += Client.Name + Seperator;
    stClientRecord += Client.Phone + Seperator;
    stClientRecord += to_string(Client.AccountBalance);

    return stClientRecord;

}

void AddRecordToFile(string FileName, stClient Client) {

    fstream MyFile;

    MyFile.open(FileName, ios::out | ios::app);

    if (MyFile.is_open()) {

        string Line = "";

        Line = ConvertRecordToLine(Client);

        MyFile << Line << endl;

        MyFile.close();

    }

}

vector <stClient> LoadClientsDataFromFile(string FileName) {

    vector <stClient> vClients;

    stClient Client;

    fstream MyFile;

    MyFile.open(FileName, ios::in);

    if (MyFile.is_open()) {

        string Line;

        while (getline(MyFile, Line)) {

            Client = ConvertLineToRecord(Line);
            vClients.push_back(Client);

        }

        MyFile.close();

    }

    return vClients;

}

bool IsClientExist(string AccountNumber, vector <stClient> vClients) {

    for (stClient& Client : vClients) {

        if (Client.AccountNumber == AccountNumber)
            return true;

    }

    return false;

}

bool IsClientExist(string AccountNumber, vector <stClient> vClients, stClient& Client1) {

    for (stClient& Client : vClients) {

        if (Client.AccountNumber == AccountNumber) {

            Client1 = Client;
            return true;

        }

    }

    return false;

}

bool FindClientByAccountNumberAndPinCode(string AccountNumber, string PinCode , stClient &Client1) {

    vector <stClient> vClients = LoadClientsDataFromFile(ClientsFileName);

    for (stClient& Client : vClients) {

        if (Client.AccountNumber == AccountNumber && Client.PinCode == PinCode) {

            Client1 = Client;
            return true;

        }

    }

    return false;

}

void SaveClientsDataToFile(string FileName, vector <stClient> vClients) {

    fstream MyFile;

    MyFile.open(FileName, ios::out);

    if (MyFile.is_open()) {

        string Line;

        for (stClient& Client : vClients) {

            if (Client.MarkedForDelete == false) {

                Line = ConvertRecordToLine(Client);
                MyFile << Line << endl;

            }

        }

        MyFile.close();

    }

}


void Login();

void GoBackToATMMainMenu();


void Withdraw(int withdrawAmount) {

    vector <stClient> vClients = LoadClientsDataFromFile(ClientsFileName);

    char answer;

    for (stClient &Client : vClients ) {

        if (Client.AccountNumber == currentClient.AccountNumber) {

            if (Client.AccountBalance < withdrawAmount) {

                cout << "\nThe amount exceeds your balance, make another choice.";

            }
            else {

                cout << "\nAre you sure you want to perform this transaction : (Y/N) ? ";
                cin >> answer;

                if (toupper(answer) == 'Y') {

                    Client.AccountBalance = Client.AccountBalance + withdrawAmount * -1;
                    currentClient.AccountBalance = Client.AccountBalance;

                    cout << "\nDone successfully, New balance is " << currentClient.AccountBalance << endl;

                }

            }

        }

    }

    SaveClientsDataToFile(ClientsFileName, vClients);

    GoBackToATMMainMenu();

}

void QuickWithdrawScreenChoices(enQuickWithdrawChoices choice) {

    switch (choice) {

    case enQuickWithdrawChoices::twenty:
        Withdraw(20);
        break;
    case enQuickWithdrawChoices::fifty:
        Withdraw(50);
        break;
    case enQuickWithdrawChoices::hundred:
        Withdraw(100);
        break;
    case enQuickWithdrawChoices::two_hundred:
        Withdraw(200);
        break;
    case enQuickWithdrawChoices::four_hundred:
        Withdraw(400);
        break;
    case enQuickWithdrawChoices::six_hundred:
        Withdraw(600);
        break;
    case enQuickWithdrawChoices::height_hundred:
        Withdraw(800);
        break;
    case enQuickWithdrawChoices::one_thousand:
        Withdraw(1000);
        break;
    case enQuickWithdrawChoices::Exit:
        GoBackToATMMainMenu();
        break;

    }

}

void QuickWithdrawScreen() {

    cout << "\n=================================================================================\n";
    cout << "\t\t\tQuick Withdraw";
    cout << "\n=================================================================================\n";
    cout << "\t\t[1] 20\t\t[2] 50";
    cout << "\n\t\t[3] 100\t\t[4] 200";
    cout << "\n\t\t[5] 400\t\t[6] 600";
    cout << "\n\t\t[7] 800\t\t[8] 1000";
    cout << "\n\t\t[9] Exit";
    cout << "\n=================================================================================\n";

}

void ShowQuickWithdrawScreen() {

    QuickWithdrawScreen();
    cout << "\nYour Balance is " << currentClient.AccountBalance;
    QuickWithdrawScreenChoices(ReadQuickWithdrawChoice("\n\nChoose what to withdraw from 1 to 8 : "));

}



void ShowNormalWithdrawScreen() {

    NormalWithdrawHeader();

    int WithdrawAmount = 0;

    cout << "\nEnter an amount multiple of 5's : ";
    cin >> WithdrawAmount;

    while (WithdrawAmount % 5 != 0) {

        cout << "\nEnter an amount multiple of 5's : ";
        cin >> WithdrawAmount;

    }

    Withdraw(WithdrawAmount);
    GoBackToATMMainMenu();

}



void ShowDepositScreen() {

    DepositHeader();

    int DepositAmount = 0;

    cout << "\nEnter a positive deposit amount : ";
    cin >> DepositAmount;

    Withdraw(DepositAmount * -1);
    GoBackToATMMainMenu();

}


void CheckBalanceScreen() {

    CheckBalanceHeader();
    cout << "\nYour Balance is " << to_string(currentClient.AccountBalance) << endl;
    GoBackToATMMainMenu();

}



void  ATMMainMenuScreenChoice(enATMMenuChoices choice) {

    switch (choice) {

    case enATMMenuChoices::QuickWithdraw:
        system("cls");
        system("color 0f");
        ShowQuickWithdrawScreen();
        break;
    case enATMMenuChoices::NormalWithdraw:
        system("cls");
        system("color 0f");
        ShowNormalWithdrawScreen();
        break;
    case enATMMenuChoices::Deposit:
        system("cls");
        system("color 0f");
        ShowDepositScreen();
        break;
    case enATMMenuChoices::CheckBalance:
        system("cls");
        system("color 0f");
        CheckBalanceScreen();
        break;
    case enATMMenuChoices::Logout:
        system("cls");
        system("color 0f");
        Login();
        break;

    }

}


void ShowATMMainMenuScreen() {

    cout << "\n=================================================================================\n";
    cout << "\t\t\tATM Main Menu Screen";
    cout << "\n=================================================================================\n";
    cout << "\t\t[1] Quick Withdraw.";
    cout << "\n\t\t[2] Normal Withdraw.";
    cout << "\n\t\t[3] Deposit.";
    cout << "\n\t\t[4] Check Balance.";
    cout << "\n\t\t[5] Logout.";
    cout << "\n=================================================================================\n";

}

void StartProject() {

    system("cls");
    system("color 0f");
    ShowATMMainMenuScreen();
    ATMMainMenuScreenChoice(ReadATMMainMenuScreenChoice("Choose what do you want to do : [1 to 5] ? "));

}




int main()
{
    
    Login();

    return 0;
}

void Login() {

    LoginHeader();

    string AccountNumber = ReadString("\nEnter account number : ");
    string PinCode = ReadString("\nEnter PinCode : ");

    while (!FindClientByAccountNumberAndPinCode(AccountNumber, PinCode, currentClient)) {

        system("cls");
        system("color 0f");

        LoginHeader();

        cout << "\nInvalid AccountNumber/PinCode ! ";
        AccountNumber = ReadString("\nEnter account number : ");
        PinCode = ReadString("\nEnter PinCode : ");

    }

    StartProject();

}

void GoBackToATMMainMenu() {

    cout << "\nPress any key to continue...";
    system("pause");
    system("cls");
    StartProject();

}