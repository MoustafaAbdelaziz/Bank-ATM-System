#include <iostream>
#include "MyLib.h"
#include <string>
#include <vector>
#include <fstream>
#include <iomanip>
#include <ctime>
using namespace std;
#pragma warning(disable : 4996)
const string ClientsFileName = "Clients.txt";
const string UsersFileName = "Users.txt";
const string ClinetsTransactionsFile = "ClientsTransactions.txt";

enum enMainMenueChooses {
    eShowClientList = 1, eAddNewClients, eDeleteClient, eDeleteAllClients
    , eUpdateClientInfo, eFindClient, eTransactions, eManageUsers, eLogout
};
enum enMainMenuePermissions {
    ePAll = -1, ePShowClientList = 1, ePAddNewClients = 2, ePDeleteClient = 4, ePDeleteAllClients = 8
    , ePUpdateClientInfo = 16, ePFindClient = 32, ePTransactions = 64, ePManageUsers = 128, ePLogout
};
enum enTransactionsOptions { enDeposit = 1, enWithdraw, enTransfer, eTotalBalance, eMainMenue };
enum enTransactionType { eDeposit = 1, eWithdraw, eSender, eReciever };
enum enTransactionDescription { eBranch = 1, eATM, eBranchTransfer, eOnlineTransfer };
enum enLoginOptions { eUser = 1, eClient = 2, eExit };
enum enClientMenueOptions { enDeposit2 = 1, enWithdraw2, enTransfer2, eUpdateAccount, eShowAccDetails, eTotalTransactions, eLogout2 };
enum enAccStatus { Blocked = 1, Active };
enum enAdminMoreOperationOnClient { endeposit = 1, enwithdraw, enTotalTransactions, enChangeAccStatus, enBacktoMainMenue };
enum enManageUsersOptions { enShowUsersList = 1, enAddUser, enDeleteUser, UpdateUser, enFindUser, enMainMenue };

struct stClientData
{
    string FirstName = "";
    string LastName = "";
    string Email = "";
    string PhoneNumber = "";
    string AccNumber = "";
    string PinCode = "";
    double AccBalance = 0;
    bool MarkForDelete = false;
    enAccStatus AccStatus = enAccStatus::Active;
    enTransactionType TransactionType = enTransactionType::eDeposit;
    enTransactionDescription TransactionDescription = enTransactionDescription::eBranch;
    string Date = "";
};
struct stTransactions
{
    string AccNumber = "";
    double TransactionAmount = 0;
    string TransactionType = "";
    string TransactionDescription = "";
    string Date = "";
    bool MarkForDelete = false;
};
struct stUsers
{
    string Username = "";
    string FirstName = "";
    string LastName = "";
    string Email = "";
    string PhoneNumber = "";
    string Password = "";
    short Permissions = 0;
    bool MarkForDelete = false;
    enAccStatus UserAccStatus = enAccStatus::Active;
};

stUsers CurrentUser;

void UserLogin();
bool ClientLogin();
void ShowMainMenueScreen();
void ShowClientScreen();
void ShowUserTypeScreen();
void ShowManageUsersMenueScreen();
void ShowTransactionsMenueScreen();
void ShowMoreOperationOnClientScreen();
void PerformMoreOperationOnClientScreen(enAdminMoreOperationOnClient, vector <stClientData>&, stClientData&, vector <stTransactions>&);

string LeftTrim(string str)
{
    for (short i = 0; i < str.length(); i++)
    {
        if (str[i] != ' ')
            return str.substr(i, str.length() - i);
    }
    return "";
}
string RightTrim(string str)
{
    for (short i = (str.length() - 1); i >= 0; i--)
    {
        if (str[i] != ' ')
            return str.substr(0, i + 1);
    }
    return "";
}
string Trim(string str)
{
    return LeftTrim(RightTrim(str));
}

enLoginOptions               ReadUserChoiceForLogin()
{
    cout << "\nChoose What You Want To Do [1:3] ";
    short LoginChooses = MyLib::ReadNumber(1, 3);
    return (enLoginOptions)LoginChooses;
}
enMainMenueChooses           ReadUserChoiceForMainMenu()
{
    cout << "\nChoose What You Want To Do [1:9] ";
    short MainMenueChooses = MyLib::ReadNumber(1, 9);
    return (enMainMenueChooses)MainMenueChooses;
}
enTransactionsOptions        ReadUserChoiceForTransaction()
{
    cout << "\nChoose What You Want To Do [1:5] ";
    short MainMenueChooses = MyLib::ReadNumber(1, 5);
    return (enTransactionsOptions)MainMenueChooses;
}
enClientMenueOptions         ReadClientChoiceForClientMenue()
{
    cout << "\nChoose What You Want To Do [1:7] ";
    short ClientOperationsChooses = MyLib::ReadNumber(1, 7);
    return (enClientMenueOptions)ClientOperationsChooses;
}
enTransactionDescription     ReadTransactionDescription()
{
    cout << "\nChoose What You Want To Do [1:2] ";
    short TransactionDescription = MyLib::ReadNumber(1, 2);
    return (enTransactionDescription)TransactionDescription;
}
enManageUsersOptions         ReadUserChoiceForManageUserMainMenue()
{
    cout << "\nChoose What You Want To Do [1:6] ";
    return (enManageUsersOptions)MyLib::ReadNumber(1, 6);
}
enAdminMoreOperationOnClient ReadAdminChoiceDoToMoreOperationsOnClient()
{
    cout << "\nChoose What You Want To Do [1:5] ";
    short AdminChoiceForMoreOperationOnClient = MyLib::ReadNumber(1, 5);
    return (enAdminMoreOperationOnClient)AdminChoiceForMoreOperationOnClient;
}

vector <string> SplitString(string str, string delim = " ")
{
    vector <string> vString;
    short pos = 0;
    string word = "";
    while ((pos = str.find(delim)) != str.npos)
    {
        word = str.substr(0, pos);
        if (word != "")
        {
            vString.push_back(word);
        }
        str.erase(0, pos + delim.length());
    }
    if (str != "")
    {
        vString.push_back(str);
    }
    return vString;
}

stClientData ConvertClientToRecord(string Line, string delim = "#//#")
{
    stClientData ClientData;
    vector <string> vClientData = SplitString(Line, delim);

    ClientData.FirstName = vClientData[0];
    ClientData.LastName = vClientData[1];
    ClientData.Email = vClientData[2];
    ClientData.PhoneNumber = vClientData[3];
    ClientData.AccNumber = vClientData[4];
    ClientData.PinCode = vClientData[5];
    ClientData.AccBalance = stod(vClientData[6]);
    if (vClientData[7] == "Blocked")
        ClientData.AccStatus = enAccStatus::Blocked;
    else
        ClientData.AccStatus = enAccStatus::Active;
    return ClientData;
}
string ConvertRecordToClient(stClientData& ClientData, string delim = "#//#")
{
    string stClientRecord = "";

    stClientRecord += ClientData.FirstName + delim;
    stClientRecord += ClientData.LastName + delim;
    stClientRecord += ClientData.Email + delim;
    stClientRecord += ClientData.PhoneNumber + delim;
    stClientRecord += ClientData.AccNumber + delim;
    stClientRecord += ClientData.PinCode + delim;
    stClientRecord += to_string(ClientData.AccBalance) + delim;
    if (ClientData.AccStatus == enAccStatus::Blocked)
        stClientRecord += "Blocked";
    else
        stClientRecord += "Active";
    return stClientRecord;
}

stUsers ConvertUserToRecord(string Line, string delim = "#//#")
{
    stUsers UserData;
    vector <string> vUsersData = SplitString(Line, delim);

    UserData.Username = vUsersData[0];
    UserData.FirstName = vUsersData[1];
    UserData.LastName = vUsersData[2];
    UserData.Email = vUsersData[3];
    UserData.PhoneNumber = vUsersData[4];
    UserData.Password = vUsersData[5];
    if (vUsersData[6] == "Blocked")
        UserData.UserAccStatus = enAccStatus::Blocked;
    else
        UserData.UserAccStatus = enAccStatus::Active;
    UserData.Permissions = stoi(vUsersData[7]);

    return UserData;
}
string ConvertRecordToUserLine(stUsers& UserData, string delim = "#//#")
{
    string strUserRecord = "";

    strUserRecord += UserData.Username + delim;
    strUserRecord += UserData.FirstName + delim;
    strUserRecord += UserData.LastName + delim;
    strUserRecord += UserData.Email + delim;
    strUserRecord += UserData.PhoneNumber + delim;
    strUserRecord += UserData.Password + delim;
    if (UserData.UserAccStatus == enAccStatus::Blocked)
        strUserRecord += "Blocked";
    else
        strUserRecord += "Active";
    strUserRecord += to_string(UserData.Permissions);

    return strUserRecord;
}

stTransactions ConvertTransactionLineIntoRecord(string Line, string delim = "#//#")
{
    stTransactions Transaction;
    vector <string> vTransactions = SplitString(Line, delim);

    Transaction.AccNumber = vTransactions[0];
    Transaction.TransactionAmount = stod(vTransactions[1]);
    Transaction.TransactionType = vTransactions[2];
    Transaction.TransactionDescription = vTransactions[3];
    Transaction.Date = vTransactions[4];
    return Transaction;
}
string ConvertClientTransactionIntoLine(stClientData Client, double Amount, string Delim = "#//#")
{
    string stClientTransactionRecord = "";
    time_t t = time(0);
    char* dt = ctime(&t);
    stClientTransactionRecord += Client.AccNumber + Delim;
    stClientTransactionRecord += to_string(Amount) + Delim;

    if (Client.TransactionType == enTransactionType::eDeposit)
        stClientTransactionRecord += "Deposit" + Delim;
    else if (Client.TransactionType == enTransactionType::eWithdraw)
        stClientTransactionRecord += "Withdraw" + Delim;
    else if (Client.TransactionType == enTransactionType::eSender)
        stClientTransactionRecord += "Transfer (Sender)" + Delim;
    else
        stClientTransactionRecord += "Transfer (Reciever)" + Delim;

    if (Client.TransactionDescription == enTransactionDescription::eATM)
        stClientTransactionRecord += "ATM" + Delim;
    else if (Client.TransactionDescription == enTransactionDescription::eBranch)
        stClientTransactionRecord += "Branch" + Delim;
    else if (Client.TransactionDescription == enTransactionDescription::eBranchTransfer)
        stClientTransactionRecord += "Branch Transfer" + Delim;
    else if (Client.TransactionDescription == enTransactionDescription::eOnlineTransfer)
        stClientTransactionRecord += "Online Transfer" + Delim;
    stClientTransactionRecord += Trim(dt);


    return stClientTransactionRecord;
}
string ConvertClientTransactionIntoLine(stTransactions& Transaction, string delim = "#//#")
{
    string strTransaction = "";

    strTransaction += Transaction.AccNumber + delim;
    strTransaction += to_string(Transaction.TransactionAmount) + delim;
    strTransaction += Transaction.TransactionType + delim;
    strTransaction += Transaction.Date + delim;
    strTransaction += Transaction.TransactionDescription;

    return strTransaction;
}

void PrintClientCard(stClientData ClientData)
{
    cout << "\n\nThe following is the client details:";
    cout << "\n____________________________________________________________\n\n";
    cout << "Name           : " << setw(18) << left << ClientData.FirstName + " " + ClientData.LastName << endl;
    cout << "Email          : " << setw(18) << left << ClientData.Email << endl;
    cout << "Phone Number   : " << setw(18) << left << ClientData.PhoneNumber << endl;
    cout << "Account Number : " << setw(18) << left << ClientData.AccNumber << endl;
    cout << "PinCode        : " << setw(18) << left << ClientData.PinCode << endl;
    cout << "Account Balance: " << setw(18) << left << ClientData.AccBalance << endl;
    if (ClientData.AccStatus == enAccStatus::Blocked)
        cout << "Account Status : " << setw(18) << left << "Blocked";
    else
        cout << "Account Status : " << setw(18) << left << "Active";
    cout << "\n____________________________________________________________\n" << endl;
}
void PrintUserCard(stUsers UserData)
{
    cout << "\n\nThe following is the User details:";
    cout << "\n____________________________________________________________\n\n";
    cout << "Username        : " << setw(18) << left << UserData.Username << endl;
    cout << "Password        : " << setw(18) << left << UserData.Password << endl;
    cout << "Permissions     : " << setw(18) << left << UserData.Permissions << endl;

    cout << "\n____________________________________________________________\n" << endl;
}

vector <stClientData> LoadClientsDataFromFileToVector(string FileName)
{
    fstream MyFile;
    vector <stClientData> vClients;
    MyFile.open(FileName, ios::in);

    if (MyFile.is_open())
    {
        string Line;
        stClientData Client;
        while (getline(MyFile, Line))
        {
            Client = ConvertClientToRecord(Line);
            vClients.push_back(Client);
        }
        MyFile.close();
    }
    return vClients;
}
vector <stUsers> LoadUsersDataFromFileToVector(string FileName)
{
    fstream MyFile;
    vector <stUsers> vUsers;
    MyFile.open(FileName, ios::in);

    if (MyFile.is_open())
    {
        string Line;
        stUsers User;
        while (getline(MyFile, Line))
        {
            User = ConvertUserToRecord(Line);
            vUsers.push_back(User);
        }
        MyFile.close();
    }
    return vUsers;
}
vector <stTransactions> LoadTransactionsFromFileToVector(string FileName)
{
    fstream MyFile;
    vector <stTransactions> vTransactions;
    MyFile.open(FileName, ios::in);

    if (MyFile.is_open())
    {
        string Line;
        stTransactions Transaction;
        while (getline(MyFile, Line))
        {
            Transaction = ConvertTransactionLineIntoRecord(Line);
            vTransactions.push_back(Transaction);
        }
        MyFile.close();
    }
    return vTransactions;
}

bool IsClientExist(string AccNumber, vector <stClientData>& vClients)
{
    for (stClientData& C : vClients)
    {
        if (C.AccNumber == AccNumber)
            return true;
    }
    return false;
}
bool IsTransferTransactionExist(string AccNumber, vector <stTransactions>& vTransactions)
{
    for (stTransactions& T : vTransactions)
    {
        if (T.AccNumber == AccNumber && (T.TransactionType == "Transfer1" || T.TransactionType == "Transfer2"))
            return true;
    }
    return false;
}
bool FindClientByAccNumber(string AccNumber, vector <stClientData> vClients, stClientData& Client)
{

    for (stClientData& C : vClients)
    {
        if (C.AccNumber == AccNumber)
        {
            Client = C;
            return true;
        }
    }
    return false;
}
bool FindUserByUsernameAndPassword(string Password, string UserName, stUsers& User)
{
    vector <stUsers> vUsers = LoadUsersDataFromFileToVector(UsersFileName);
    for (stUsers& C : vUsers)
    {
        if ((C.Username == UserName) && (C.Password == Password))
        {
            User = C;
            return true;
        }
    }
    return false;
}

void PrintClientRecord(stClientData Client)
{
    cout << "| " << setw(21) << left << Client.FirstName + " " + Client.LastName;
    cout << "| " << setw(21) << left << Client.Email;
    cout << "| " << setw(15) << left << Client.PhoneNumber;
    cout << "| " << setw(12) << left << Client.AccNumber;
    cout << "| " << setw(10) << left << Client.PinCode;
    cout << "| " << setw(10) << left << Client.AccBalance;
    if (Client.AccStatus == enAccStatus::Blocked)
        cout << "| " << setw(8) << left << "Blocked";
    else
        cout << "| " << setw(8) << left << "Active";
}
void PrintAllClientsData(vector <stClientData> vClients)
{
    cout << endl << setw(60) << "Client List [" << vClients.size() << "] Client(s)";
    cout << "\n_______________________________________________________________________________________________________________________\n" << endl;
    cout << "| " << setw(21) << left << "Name";
    cout << "| " << setw(21) << left << "Email";
    cout << "| " << setw(15) << left << "Phone Number";
    cout << "| " << setw(12) << left << "Acc. Number";
    cout << "| " << setw(10) << left << "Pin Code";
    cout << "| " << setw(10) << left << "Balance";
    cout << "| " << setw(8) << left << "Status";
    cout << "\n_______________________________________________________________________________________________________________________\n" << endl;

    if (vClients.size() == 0)
    {
        cout << "\n\n\t\t\t\t\t   There Are No Clients In The System!.\n";
        cout << "\n_______________________________________________________________________________________________________________________\n" << endl;
    }
    else
    {
        for (stClientData& Client : vClients)
        {
            PrintClientRecord(Client);
            cout << endl;
        }
        cout << "\n_______________________________________________________________________________________________________________________\n" << endl;
    }
}

void PrintUserRecord(stUsers User)
{
    cout << "| " << setw(15) << left << User.Username;
    cout << "| " << setw(9) << left << User.Password;
    cout << "| " << setw(42) << left << User.Permissions;
}
void PrintAllUsersData(vector <stUsers> vUsers)
{
    cout << endl << setw(60) << "User List [" << vUsers.size() << "] Client(s)";
    cout << "\n_______________________________________________________________________________________________________________________\n" << endl;
    cout << "| " << setw(15) << left << "Name";
    cout << "| " << setw(9) << left << "Password";
    cout << "| " << setw(42) << left << "Permission";
    cout << "\n_______________________________________________________________________________________________________________________\n" << endl;
    for (stUsers& User : vUsers)
    {
        PrintUserRecord(User);
        cout << endl;
    }
    cout << "\n_______________________________________________________________________________________________________________________\n" << endl;
}

void PrintClientTransaction(stTransactions Transaction)
{
    cout << "| " << setw(10) << left << Transaction.TransactionAmount;
    cout << "| " << setw(20) << left << Transaction.TransactionType;
    cout << "| " << setw(19) << left << Transaction.TransactionDescription;
    cout << "| " << setw(30) << left << Transaction.Date;
}
void PrintAllClientTransactions(string AccNumber, vector <stTransactions> vTransactions)
{
    bool IsExist = false;
    cout << "\n\n_______________________________________________________________________________________________________________________\n" << endl;
    cout << "\t\t\t\t\t    Client's Transaction(s) List.";
    cout << "\n_______________________________________________________________________________________________________________________\n" << endl;
    cout << "| " << setw(10) << left << "Amount";
    cout << "| " << setw(20) << left << "Type";
    cout << "| " << setw(19) << left << "Description";
    cout << "| " << setw(30) << left << "Date";
    cout << "\n_______________________________________________________________________________________________________________________\n" << endl;

    for (stTransactions& T : vTransactions)
    {
        if (T.AccNumber == AccNumber)
        {
            PrintClientTransaction(T);
            IsExist = true;
            cout << endl;
        }
    }
    if (!IsExist)
    {
        cout << "\n\t\t\t\t\tThere Aren't Transactions For This Client!\n";
    }
    cout << "\n_______________________________________________________________________________________________________________________\n" << endl;

}

void PrintClientBalance(stClientData Client)
{
    cout << "| " << setw(18) << left << Client.AccNumber;
    cout << "| " << setw(45) << left << Client.FirstName + " " + Client.LastName;
    cout << "| " << setw(15) << left << Client.AccBalance;
}
void PrintAllClientsBalances(vector <stClientData> vClients)
{
    cout << endl << setw(60) << "Balances List [" << vClients.size() << "] Client(s)";
    cout << "\n_______________________________________________________________________________________________________________________\n" << endl;
    cout << "| " << setw(18) << left << "Account Number";
    cout << "| " << setw(45) << left << "Client Name";
    cout << "| " << setw(15) << left << "Account Balance";
    cout << "\n_______________________________________________________________________________________________________________________\n" << endl;

    double TotalBalances = 0;
    if (vClients.size() == 0)
    {
        cout << "\n\n\t\t\t\t\t   There Are No Clients In The System!.\n";
        cout << "\n_______________________________________________________________________________________________________________________\n" << endl;
    }
    else
    {
        for (stClientData& Client : vClients)
        {
            PrintClientBalance(Client);
            TotalBalances += Client.AccBalance;
            cout << endl;
        }
        cout << "\n_______________________________________________________________________________________________________________________\n";
        printf("\t\t\t\t\t\t    Total Balances = %.5f\n", TotalBalances);
    }


}

short ReadPermissions()
{
    short Permission = 0;
    char Ans = 'n';
    cout << "\nDo u want to give there FULL access [y/n]? ";
    cin >> Ans;
    if (MyLib::CheckChar(Ans))
        return enMainMenuePermissions::ePAll;

    cout << "\nDo you want to give access to:\n\nShow Client List?[y/n]. ";
    cin >> Ans;
    if (MyLib::CheckChar(Ans))
        Permission += enMainMenuePermissions::ePShowClientList;

    cout << "Add New Client? [y/n]. ";
    cin >> Ans;
    if (MyLib::CheckChar(Ans))
        Permission += enMainMenuePermissions::ePAddNewClients;

    cout << "Delete Client? [y/n]. ";
    cin >> Ans;
    if (MyLib::CheckChar(Ans))
        Permission += enMainMenuePermissions::ePDeleteClient;

    cout << "Delete All Client? [y/n]. ";
    cin >> Ans;
    if (MyLib::CheckChar(Ans))
        Permission += enMainMenuePermissions::ePDeleteAllClients;

    cout << "Update Client? [y/n]. ";
    cin >> Ans;
    if (MyLib::CheckChar(Ans))
        Permission += enMainMenuePermissions::ePUpdateClientInfo;

    cout << "Find Client? [y/n]. ";
    cin >> Ans;
    if (MyLib::CheckChar(Ans))
        Permission += enMainMenuePermissions::ePFindClient;

    cout << "Transactions? [y/n]. ";
    cin >> Ans;
    if (MyLib::CheckChar(Ans))
        Permission += enMainMenuePermissions::ePTransactions;

    cout << "Manage users? [y/n]. ";
    cin >> Ans;
    if (MyLib::CheckChar(Ans))
        Permission += enMainMenuePermissions::ePManageUsers;

    return Permission;
}

bool IsUserExistByUserName(string UserName, stUsers& sUser)
{
    vector <stUsers> vUsers = LoadUsersDataFromFileToVector(UsersFileName);
    for (stUsers& User : vUsers)
    {
        if (User.Username == UserName)
        {
            sUser = User;
            return true;
        }
    }
    return false;
}
stClientData ReadNewClient()
{
    stClientData ClientData;
    vector <stClientData> vClients = LoadClientsDataFromFileToVector(ClientsFileName);
    cout << "\nEnter Account Number: ";
    string AccNumber = MyLib::ReadString();
    while (IsClientExist(AccNumber, vClients))
    {
        cout << "\n\nClient with Account Number [" << AccNumber << "] is already exists\n";
        cout << "\nEnter Account Number: ";
        AccNumber = MyLib::ReadString();
    }
    ClientData.AccNumber = AccNumber;

    cout << "Enter PinCode: ";
    ClientData.PinCode = MyLib::ReadString();

    cout << "Enter First Name: ";
    ClientData.FirstName = MyLib::ReadString();

    cout << "Enter Last Name: ";
    ClientData.LastName = MyLib::ReadString();

    cout << "Enter Phone Number: ";
    ClientData.PhoneNumber = MyLib::ReadString();

    cout << "Enter Account Balance: ";
    ClientData.AccBalance = MyLib::ReadPositiveNumber<double>();

    return ClientData;
}

void AddDataToFile(string FileName, string Client)
{
    fstream MyFile;
    MyFile.open(FileName, ios::out | ios::app);

    if (MyFile.is_open())
    {
        MyFile << Client << endl;
    }
    MyFile.close();
}
void AddClients()
{
    char AddMore = 'Y';
    do
    {
        stClientData Client = ReadNewClient();
        cout << "\nAdding A New Client:\n";
        AddDataToFile(ClientsFileName, ConvertRecordToClient(Client));
        cout << "\nClient Added Successfully, do you want to add more clients ? [Y / N] ? \n";
        cin >> AddMore;
    } while (MyLib::CheckChar(AddMore));
}

stUsers ReadNewUser()
{
    stUsers UserData;
    cout << "\nEnter Thr Username: ";
    string UserName = MyLib::ReadString();
    while (IsUserExistByUserName(UserName, UserData) || MyLib::LowerString(UserName) == "admin")
    {
        cout << "\nUser With UserName [" << UserName << "] already exists\n";
        cout << "\nEnter Thr Username: ";
        UserName = MyLib::ReadString();
    }
    UserData.Username = UserName;
    cout << "Enter Password: ";
    UserData.Password = MyLib::ReadString();

    UserData.Permissions = ReadPermissions();

    return UserData;
}

void AddUsers()
{
    char AddMore = 'Y';
    do
    {
        stUsers User = ReadNewUser();
        cout << "\nAdding A New User:\n";
        AddDataToFile(UsersFileName, ConvertRecordToUserLine(User));
        cout << "\nUser Added Successfully, do you want to add more Users ? [Y / N] ? \n";
        cin >> AddMore;
    } while (MyLib::CheckChar(AddMore));
}

void SaveClientsDataToFile(string FileName, vector <stClientData>& vClientsData)
{
    fstream MyFile;
    MyFile.open(FileName, ios::out);
    if (MyFile.is_open())
    {
        string DataLine;
        for (stClientData& Line : vClientsData)
        {
            if (Line.MarkForDelete == false)
            {
                DataLine = ConvertRecordToClient(Line);
                MyFile << DataLine << endl;
            }
        }
        MyFile.close();
    }
}
void SaveUsersDataToFile(string FileName, vector <stUsers>& vUsers)
{
    fstream MyFile;
    MyFile.open(FileName, ios::out);
    if (MyFile.is_open())
    {
        string DataLine;
        for (stUsers& Line : vUsers)
        {
            if (Line.MarkForDelete == false)
            {
                DataLine = ConvertRecordToUserLine(Line);
                MyFile << DataLine << endl;
            }
        }
        MyFile.close();
    }
}
void SaveTransactionsAfterDeleteClient(string FileName, vector <stTransactions>& vTransactions)
{
    fstream MyFile;
    MyFile.open(FileName, ios::out);
    if (MyFile.is_open())
    {
        string Line = "";
        for (stTransactions& T : vTransactions)
        {
            if (T.MarkForDelete == false)
            {
                Line = ConvertClientTransactionIntoLine(T);
                MyFile << Line << endl;
            }
        }
        MyFile.close();
    }
}

bool DepositBalanceToClientByAccountNumber(string AccNumber, double Amount, vector <stClientData>& vClients)
{
    char Ans = 'N';

    cout << "\n\nAre you sure you want to Perform This Transaction? [y/n]. ";
    cin >> Ans;
    if (MyLib::CheckChar(Ans))
    {
        for (stClientData& C : vClients)
        {
            if (C.AccNumber == AccNumber)
            {
                C.AccBalance += Amount;
                SaveClientsDataToFile(ClientsFileName, vClients);
                cout << "\n\nDone Successfully. New Balance is: " << C.AccBalance;
                return true;
            }
        }
    }
    return false;
}

void MarkClientTransactionsDeletedByAccNumber(string AccNumber, vector <stTransactions>& vTransactions)
{
    for (stTransactions& T : vTransactions)
    {
        if (T.AccNumber == AccNumber)
        {
            T.MarkForDelete = true;
        }
    }
}
bool MarkClientDeletedByAccNumber(string AccNumber, vector <stClientData>& vClient)
{
    for (stClientData& Line : vClient)
    {
        if (Line.AccNumber == AccNumber)
        {
            Line.MarkForDelete = true;
            return true;
        }
    }
    return false;
}
bool MarkUserDeletedByUserName(string UserName, vector <stUsers>& vUsers)
{
    for (stUsers& Line : vUsers)
    {
        if (Line.Username == UserName)
        {
            Line.MarkForDelete = true;
            return true;
        }
    }
    return false;
}
void MarkAllClientsDeleted(vector <stClientData>& vClient)
{
    for (stClientData& C : vClient)
    {
        C.MarkForDelete = true;
    }
}
void MarkAllTransactionsDeleted(vector <stTransactions>& vTransactions)
{
    for (stTransactions& T : vTransactions)
    {
        T.MarkForDelete = true;
    }
}

bool DeleteClientByAccountNumber(string AccNumber, vector <stClientData>& vClients)
{
    vector <stTransactions> vTransactions = LoadTransactionsFromFileToVector(ClinetsTransactionsFile);
    stClientData Client;
    char Ans = 'N';
    if (FindClientByAccNumber(AccNumber, vClients, Client))
    {
        PrintClientCard(Client);
        cout << "Are you sure you want to delete this client? [y/n].";
        cin >> Ans;
        if (MyLib::CheckChar(Ans))
        {
            MarkClientDeletedByAccNumber(AccNumber, vClients);
            SaveClientsDataToFile(ClientsFileName, vClients);
            MarkClientTransactionsDeletedByAccNumber(AccNumber, vTransactions);
            SaveTransactionsAfterDeleteClient(ClinetsTransactionsFile, vTransactions);
            cout << "\n\nClient Deleted Successfully.\n";

            vClients = LoadClientsDataFromFileToVector(ClientsFileName);
            vTransactions = LoadTransactionsFromFileToVector(ClinetsTransactionsFile);
            return true;
        }
    }
    else
    {
        cout << "\n\nClient with Account Number [" << AccNumber << "] is not found!\n";
        return false;
    }
}
bool DeleteUserByUserName(string UserName, vector <stUsers>& vUsers)
{
    stUsers User;
    char Ans = 'N';
    if (IsUserExistByUserName(UserName, User) && MyLib::LowerString(User.Username) != "admin")
    {
        PrintUserCard(User);
        cout << "Are you sure you want to delete this User? [y/n].";
        cin >> Ans;
        if (MyLib::CheckChar(Ans))
        {
            MarkUserDeletedByUserName(UserName, vUsers);
            SaveUsersDataToFile(UsersFileName, vUsers);
            cout << "\n\nClient Deleted Successfully.\n";
            vUsers = LoadUsersDataFromFileToVector(UsersFileName);
            return true;
        }
    }
    else
    {
        cout << "\n\nUser with Account Number [" << UserName << "] is not found! Or This User Is An Admin\n(Admin Can NOT Be Removed.)";
        return false;
    }
}

stClientData ChangeClientInfo(string AccNumber, stClientData& C)
{
    C.AccNumber = AccNumber;

    cout << "\nEnter PinCode: ";
    C.PinCode = MyLib::ReadString();

    cout << "Enter First Name: ";
    C.FirstName = MyLib::ReadString();

    cout << "Enter Last Name: ";
    C.LastName = MyLib::ReadString();

    cout << "Enter Phone Number: ";
    C.PhoneNumber = MyLib::ReadString();

    C.AccBalance = C.AccBalance;

    return C;
}
stUsers ChangeUserInfo(string UserName, stUsers& U)
{
    U.Username = UserName;

    cout << "\nEnter Password: ";
    U.Password = MyLib::ReadString();
    U.Permissions = ReadPermissions();
    return U;
}

bool UpdateClientByAccountNumber(string AccNumber, vector <stClientData>& vClients)
{
    stClientData Client;
    char Ans = 'N';
    if (FindClientByAccNumber(AccNumber, vClients, Client))
    {
        PrintClientCard(Client);
        cout << "\nAre you sure you want to UPDATE this client? [y/n]. ";
        cin >> Ans;
        if (MyLib::CheckChar(Ans))
        {
            for (stClientData& C : vClients)
            {
                if (C.AccNumber == AccNumber)
                {
                    C = ChangeClientInfo(AccNumber, C);
                    SaveClientsDataToFile(ClientsFileName, vClients);
                    cout << "\n\nClient Updated Successfully.\n";
                    break;
                }
            }
        }
        return true;
    }
    else
    {
        cout << "\n\nClient with Account Number [" << AccNumber << "] is not found!\n";
        return false;
    }
}
bool UpdateUserByUserName(string UserName, vector <stUsers>& vUsers)
{
    stUsers User;
    char Ans = 'N';
    if (IsUserExistByUserName(UserName, User))
    {
        PrintUserCard(User);
        cout << "\nAre you sure you want to UPDATE this User? [y/n]. ";
        cin >> Ans;
        if (MyLib::CheckChar(Ans))
        {
            for (stUsers& U : vUsers)
            {
                if (U.Username == UserName)
                {
                    U = ChangeUserInfo(UserName, U);
                    SaveUsersDataToFile(UsersFileName, vUsers);
                    cout << "\nUser Updated Successfully.\n";
                    break;
                }
            }
        }
        return true;
    }
    else
    {
        cout << "\n\nUser with Username [" << UserName << "] is not found!\n";
        return false;
    }
}

void BackToTheClientScreen()
{
    cout << "\n\nPress Any Key To Show The Client's Operation Screen...";
    system("pause>0");
    ShowClientScreen();
}
void BackToTheUserTypeScreen()
{
    cout << "\n\nPress Any Key To Show The Login Screen...";
    system("pause>0");
    ShowUserTypeScreen();
}
void BackToTheMainMenueScreen()
{
    cout << "\n\nPress Any Key To Show The Main Menue Screen...";
    system("pause>0");
    ShowMainMenueScreen();
}
void BackToTheManageUsersScreen()
{
    cout << "\n\nPress Any Key To Show The Manage Users Screen...";
    system("pause>0");
    ShowManageUsersMenueScreen();
}
void BackToTheTransactionsScreen()
{
    cout << "\n\nPress Any Key To Show The Transactions Screen...";
    system("pause>0");
    ShowTransactionsMenueScreen();
}
void BackToTheMoreOperationOnClientScreen()
{
    cout << "\n\nPress Any Key To Show The Operation On Client Screen...";
    system("pause>0");
    ShowMoreOperationOnClientScreen();
}

void ShowWelcomeText()
{
    cout << "\n" << setw(85) << "______________________________________________________\n" << endl;
    cout << setw(31) << "|" << setw(53) << "|";
    cout << "\n" << setw(31) << "|" << setw(40) << " WELCOME TO [ BANK ] SYSTEM " << setw(13) << "|";
    cout << "\n" << setw(31) << "|" << setw(53) << "|" << "\n";
    cout << setw(85) << "______________________________________________________\n" << endl;
    BackToTheUserTypeScreen();
}

void ShowAccessDeniedMessage()
{
    cout << "\n------------------------------------\n";
    cout << "Access Denied, \nYou dont Have Permission To Do this,\nPlease Conact Your Admin.";
    cout << "\n------------------------------------\n";
}

bool CheckAccessPermission(enMainMenuePermissions Permission)
{
    if (CurrentUser.Permissions == enMainMenuePermissions::ePAll)
        return true;

    if ((Permission & CurrentUser.Permissions) == Permission)
        return true;
    else
        return false;
}

void ShowClientsListScreen()
{
    if (!CheckAccessPermission(enMainMenuePermissions::ePShowClientList))
    {
        ShowAccessDeniedMessage();
        return;
    }
    PrintAllClientsData(LoadClientsDataFromFileToVector(ClientsFileName));
}
void ShowUsersListScreen()
{
    PrintAllUsersData(LoadUsersDataFromFileToVector(UsersFileName));
}

void AddNewClientScreen()
{
    if (!CheckAccessPermission(enMainMenuePermissions::ePAddNewClients))
    {
        ShowAccessDeniedMessage();
        return;
    }
    cout << "____________________________________\n\n";
    cout << "\tADD NEW CLIENT SCREEN";
    cout << "\n____________________________________\n";
    AddClients();
}
void AddNewUserScreen()
{
    cout << "____________________________________\n\n";
    cout << "\tADD NEW USER SCREEN";
    cout << "\n____________________________________\n";
    AddUsers();
}

void DeleteClientScreen()
{
    if (!CheckAccessPermission(enMainMenuePermissions::ePDeleteClient))
    {
        ShowAccessDeniedMessage();
        return;
    }

    vector <stClientData> vClients = LoadClientsDataFromFileToVector(ClientsFileName);
    cout << "____________________________________\n\n";
    cout << "\tDELETE CLIENT SCREEN";
    cout << "\n____________________________________\n";
    cout << "\nPlease Enter The Account Number: ";
    string AccNumber = MyLib::ReadString();
    DeleteClientByAccountNumber(AccNumber, vClients);
}
void DeleteAllClientsScreen()
{
    if (!CheckAccessPermission(enMainMenuePermissions::ePDeleteAllClients))
    {
        ShowAccessDeniedMessage();
        return;
    }
    vector <stClientData> vClients = LoadClientsDataFromFileToVector(ClientsFileName);
    vector <stTransactions> vTransactions = LoadTransactionsFromFileToVector(ClinetsTransactionsFile);
    char Ans = 'N';
    cout << "____________________________________\n\n";
    cout << "\tDELETE All CLIENTS SCREEN";
    cout << "\n____________________________________\n";

    cout << "\nAre you sure you want to delete All clients? [y/n]. ";
    cin >> Ans;
    if (MyLib::CheckChar(Ans))
    {
        MarkAllClientsDeleted(vClients);
        SaveClientsDataToFile(ClientsFileName, vClients);
        vClients = LoadClientsDataFromFileToVector(ClientsFileName);

        MarkAllTransactionsDeleted(vTransactions);
        SaveTransactionsAfterDeleteClient(ClinetsTransactionsFile, vTransactions);
        vTransactions = LoadTransactionsFromFileToVector(ClinetsTransactionsFile);

        cout << "\n\nAll Clients Deleted Successfully.\n";
    }
}

void ShowDeleteUserScreen()
{
    vector <stUsers> vUsers = LoadUsersDataFromFileToVector(UsersFileName);
    cout << "____________________________________\n\n";
    cout << "\tDELETE USER SCREEN";
    cout << "\n____________________________________\n";
    cout << "\nPlease Enter The Username: ";
    string UserName = MyLib::ReadString();
    DeleteUserByUserName(UserName, vUsers);
}

void UpdateScreen4ClientAlreadyExist(vector<stClientData>& vClients, stClientData& Client)
{
    system("cls");
    cout << "____________________________________\n\n";
    cout << "\tUPDATE CLIENT SCREEN";
    cout << "\n____________________________________\n";

    ChangeClientInfo(Client.AccNumber, Client);
    SaveClientsDataToFile(ClientsFileName, vClients);
    cout << "\n\nClient Updated Successfully.\n";
}

void UpdateClientScreen()
{
    if (!CheckAccessPermission(enMainMenuePermissions::ePUpdateClientInfo))
    {
        ShowAccessDeniedMessage();
        return;
    }
    vector <stClientData> vClients = LoadClientsDataFromFileToVector(ClientsFileName);
    cout << "____________________________________\n\n";
    cout << "\tUPDATE CLIENT SCREEN";
    cout << "\n____________________________________\n";
    cout << "\nPlease Enter The Account Number: ";
    string AccNumber = MyLib::ReadString();
    UpdateClientByAccountNumber(AccNumber, vClients);
}
void UpdateUserScreen()
{
    vector <stUsers> vUsers = LoadUsersDataFromFileToVector(UsersFileName);
    cout << "____________________________________\n\n";
    cout << "\tUPDATE USER SCREEN";
    cout << "\n____________________________________\n";
    cout << "\nPlease Enter The Username: ";
    string UserName = MyLib::ReadString();
    UpdateUserByUserName(UserName, vUsers);
}

void ShowClientTransactionsScreen(string AccNumber)
{
    vector <stTransactions> vTransaction = LoadTransactionsFromFileToVector(ClinetsTransactionsFile);
    vector <stClientData> vClients = LoadClientsDataFromFileToVector(ClientsFileName);
    PrintAllClientTransactions(AccNumber, vTransaction);
}

void ChangeClientAccountStatus(string AccNumber, vector <stClientData>& vClients, enAccStatus AccStatus = enAccStatus::Active)
{
    for (stClientData& Client : vClients)
    {
        if (Client.AccNumber == AccNumber)
        {
            Client.AccStatus = AccStatus;
            SaveClientsDataToFile(ClientsFileName, vClients);
        }
    }
}

void FindClientScreen()
{
    if (!CheckAccessPermission(enMainMenuePermissions::ePFindClient))
    {
        ShowAccessDeniedMessage();
        BackToTheMainMenueScreen();
        return;
    }
    vector <stClientData> vClients = LoadClientsDataFromFileToVector(ClientsFileName);
    vector <stTransactions> vTransactions = LoadTransactionsFromFileToVector(ClinetsTransactionsFile);
    stClientData Client;
    char Ans = 'N';
    cout << "____________________________________\n\n";
    cout << "\tFIND CLIENT SCREEN";
    cout << "\n____________________________________\n";
    cout << "\nPlease Enter The Account Number: ";
    string AccNumber = MyLib::ReadString();
    if (FindClientByAccNumber(AccNumber, vClients, Client))
    {
        PrintClientCard(Client);
        cout << "\nWould You Like To Perfom More Operations On This Client? [y/n]. ";
        cin >> Ans;
        if (MyLib::CheckChar(Ans))
        {
            ShowMoreOperationOnClientScreen();
            PerformMoreOperationOnClientScreen(ReadAdminChoiceDoToMoreOperationsOnClient(), vClients, Client, vTransactions);
        }
        else
            ShowMainMenueScreen();
    }
    else
    {
        cout << "\n\nClient with Account Number [" << AccNumber << "] is not found!\n";
        BackToTheMainMenueScreen();
    }
}
void FindUserScreen()
{
    vector <stUsers> vUsers = LoadUsersDataFromFileToVector(UsersFileName);
    stUsers User;
    char Ans = 'N';
    cout << "____________________________________\n\n";
    cout << "\tFIND USER SCREEN";
    cout << "\n____________________________________\n";
    cout << "\nPlease Enter The Username: ";
    string UserName = MyLib::ReadString();
    if (IsUserExistByUserName(UserName, User))
        PrintUserCard(User);
    else
        cout << "\n\nUser with Username [" << UserName << "] is not found!\n";
}

void SaveClientTransaction(string FileName, stClientData& Client, double Amount = 0)
{
    fstream MyFile;
    MyFile.open(FileName, ios::out | ios::app);

    if (MyFile.is_open())
    {
        string Line = ConvertClientTransactionIntoLine(Client, Amount);
        MyFile << Line;
    }
    MyFile.close();
}

double DepositScreen4AlreadyExistClient(vector<stClientData>& vClients, stClientData& Client)
{
    cout << "____________________________________\n\n";
    cout << "\tDEPOSIT SCREEN";
    cout << "\n____________________________________\n";
    cout << "\nPlease Enter Deposite Amount: ";
    double DepositAmount = 0;
    DepositAmount = MyLib::ReadPositiveNumber<double>();
    DepositBalanceToClientByAccountNumber(Client.AccNumber, DepositAmount, vClients);

    return DepositAmount;
}

double WithdrawScreen4AlreadyExistClient(vector<stClientData>& vClients, stClientData& Client)
{
    cout << "____________________________________\n\n";
    cout << "\tWITHDRAW SCREEN";
    cout << "\n____________________________________\n";
    cout << "\n\nPlease Enter Withdraw Amount: ";

    double Amount = 0;
    Amount = MyLib::ReadPositiveNumber<double>();
    while (Amount > Client.AccBalance)
    {
        cout << "\nAmount Exceeds the balance, you can withdraw up to: " << Client.AccBalance << endl;
        cout << "\nPlease enter Withdraw Amount? ";
        cin >> Amount;
    }
    DepositBalanceToClientByAccountNumber(Client.AccNumber, Amount * -1, vClients);

    return Amount;
}
void TransferToAnotherClient(string AccNumber1, string AccNumber2, vector <stClientData>& vClients, double Amount)
{
    char Ans = 'N';

    cout << "\n\nAre you sure you want to Perform This Transfer? [y/n]. ";
    cin >> Ans;
    if (MyLib::CheckChar(Ans))
    {
        for (stClientData& C : vClients)
        {
            if (C.AccNumber == AccNumber1)
                C.AccBalance -= Amount;
            if (C.AccNumber == AccNumber2)
                C.AccBalance += Amount;
        }
        SaveClientsDataToFile(ClientsFileName, vClients);
        cout << "\n\nTransfer Done Successfully.\n";
    }
}
void TransferFromExistClient(vector<stClientData>& vClients, stClientData& Client)
{
    stClientData Client2;
    double Amount = 0;
    cout << "____________________________________\n\n";
    cout << "\TRANSFER SCREEN";
    cout << "\n____________________________________\n";
    cout << "\nPlease Enter The Client's Account Number Who You Want To Transfer To: ";
    string AccNumber2 = MyLib::ReadString();

    while (Client.AccNumber == AccNumber2)
    {
        cout << "\nYou Can't Transfer To Yourself!\nPlease Enter Another Account Number: ";
        cin >> AccNumber2;
    }
    if (FindClientByAccNumber(AccNumber2, vClients, Client2))
    {
        cout << "\n\nPlease Enter Transfer Amount: ";
        Amount = MyLib::ReadPositiveNumber<double>();

        while (Amount > Client.AccBalance)
        {
            cout << "\nAmount Exceeds the balance, you can withdraw up to : " << Client.AccBalance << endl;
            cout << "\nPlease enter Withdraw Amount? ";
            cin >> Amount;
        }
        TransferToAnotherClient(Client.AccNumber, AccNumber2, vClients, Amount);
        Client.TransactionType = enTransactionType::eSender;
        Client2.TransactionType = enTransactionType::eReciever;
        Client.TransactionDescription = enTransactionDescription::eOnlineTransfer;
        Client2.TransactionDescription = enTransactionDescription::eOnlineTransfer;
        SaveClientTransaction(ClinetsTransactionsFile, Client, Amount);
        SaveClientTransaction(ClinetsTransactionsFile, Client2, Amount);
    }
    else
    {
        cout << "\n\nClient with Account Number [" << AccNumber2 << "] is not found!\n";
    }
}

void DepositScreen()
{
    stClientData Client;
    double DepositAmount = 0;
    vector <stClientData> vClients = LoadClientsDataFromFileToVector(ClientsFileName);
    cout << "____________________________________\n\n";
    cout << "\tDEPOSIT SCREEN";
    cout << "\n____________________________________\n";
    cout << "\nPlease Enter The Account Number: ";
    string AccNumber = MyLib::ReadString();
    if (FindClientByAccNumber(AccNumber, vClients, Client))
    {
        PrintClientCard(Client);
        cout << "\n\nPlease Enter Deposite Amount: ";
        DepositAmount = MyLib::ReadPositiveNumber<double>();

        DepositBalanceToClientByAccountNumber(AccNumber, DepositAmount, vClients);
        Client.TransactionType = enTransactionType::eDeposit;
        Client.TransactionDescription = enTransactionDescription::eBranch;
        SaveClientTransaction(ClinetsTransactionsFile, Client, DepositAmount);
    }
    else
    {
        cout << "\n\nClient with Account Number [" << AccNumber << "] is not found!\n";
    }

}
void WithdrawScreen()
{
    stClientData Client;
    double Amount = 0;
    vector <stClientData> vClients = LoadClientsDataFromFileToVector(ClientsFileName);
    cout << "____________________________________\n\n";
    cout << "\tWITHDRAW SCREEN";
    cout << "\n____________________________________\n";
    cout << "\nPlease Enter The Account Number: ";
    string AccNumber = MyLib::ReadString();

    if (FindClientByAccNumber(AccNumber, vClients, Client))
    {
        PrintClientCard(Client);
        cout << "\n\nPlease Enter Withdraw Amount: ";
        Amount = MyLib::ReadPositiveNumber<double>();

        while (Amount > Client.AccBalance)
        {
            cout << "\nAmount Exceeds the balance, you can withdraw up to : " << Client.AccBalance << endl;
            cout << "\nPlease enter Withdraw Amount? ";
            cin >> Amount;
        }

        DepositBalanceToClientByAccountNumber(AccNumber, Amount * -1, vClients);
        Client.TransactionType = enTransactionType::eWithdraw;
        Client.TransactionDescription = enTransactionDescription::eBranch;
        SaveClientTransaction(ClinetsTransactionsFile, Client, Amount);
    }
    else
    {
        cout << "\n\nClient with Account Number [" << AccNumber << "] is not found!\n";
    }
}
void TransferScreen()
{
    stClientData Client1, Client2;
    double Amount = 0;
    vector <stClientData> vClients = LoadClientsDataFromFileToVector(ClientsFileName);
    vector <stTransactions> vTransactions = LoadTransactionsFromFileToVector(ClinetsTransactionsFile);
    cout << "____________________________________\n\n";
    cout << "\TRANSFER SCREEN";
    cout << "\n____________________________________\n";
    cout << "\nPlease Enter The Client's Account Number Who Is Sender: ";
    string AccNumber1 = MyLib::ReadString();

    if (FindClientByAccNumber(AccNumber1, vClients, Client1))
    {
        PrintClientCard(Client1);
        cout << "\nPlease Enter The Client's Account Number Who Is Reciever: ";
        string AccNumber2 = MyLib::ReadString();

        while (AccNumber2 == AccNumber1)
        {
            cout << "\nYou Can't Transfer To Yourself!\nPlease Enter Another Account Number: ";
            cin >> AccNumber2;
        }

        if (FindClientByAccNumber(AccNumber2, vClients, Client2))
        {
            PrintClientCard(Client2);
            cout << "\n\nPlease Enter Transfer Amount: ";
            Amount = MyLib::ReadPositiveNumber<double>();

            while (Amount > Client1.AccBalance)
            {
                cout << "\nAmount Exceeds the balance, you can withdraw up to : " << Client1.AccBalance << endl;
                cout << "\nPlease enter Withdraw Amount? ";
                cin >> Amount;
            }
            TransferToAnotherClient(AccNumber1, AccNumber2, vClients, Amount);
            Client1.TransactionType = enTransactionType::eSender;
            Client2.TransactionType = enTransactionType::eReciever;
            Client1.TransactionDescription = enTransactionDescription::eBranchTransfer;
            Client2.TransactionDescription = enTransactionDescription::eBranchTransfer;
            SaveClientTransaction(ClinetsTransactionsFile, Client1, Amount);
            SaveClientTransaction(ClinetsTransactionsFile, Client2, Amount);
        }
        else
        {
            cout << "\n\nClient with Account Number [" << AccNumber2 << "] is not found!\n";
        }
    }
    else
    {
        cout << "\n\nClient with Account Number [" << AccNumber1 << "] is not found!\n";
    }
}

void ShowClientsBalancesList()
{
    PrintAllClientsBalances(LoadClientsDataFromFileToVector(ClientsFileName));
}

void TransactionScreen()
{
    if (!CheckAccessPermission(enMainMenuePermissions::ePTransactions))
    {
        ShowAccessDeniedMessage();
        BackToTheMainMenueScreen();
        return;
    }
    ShowTransactionsMenueScreen();
}

void EndProgramScreen()
{
    system("cls");
    cout << "____________________________________\n\n";
    cout << "\tG O O D  B Y E";
    cout << "\n____________________________________\n";
    cout << "\nPress Any Key To Exit...";
    system("pause>0");
    cout << endl;
}

void PerformMoreOperationOnClientScreen(enAdminMoreOperationOnClient AdminChoice, vector <stClientData>& vClients, stClientData& Client, vector <stTransactions>& vTransactions)
{
    double Amount = 0.0;
    switch (AdminChoice)
    {
    case enAdminMoreOperationOnClient::endeposit:
        system("cls");
        Amount = DepositScreen4AlreadyExistClient(vClients, Client);
        Client.TransactionType = enTransactionType::eDeposit;
        Client.TransactionDescription = enTransactionDescription::eBranch;
        SaveClientTransaction(ClinetsTransactionsFile, Client, Amount);
        BackToTheMoreOperationOnClientScreen();
        PerformMoreOperationOnClientScreen(ReadAdminChoiceDoToMoreOperationsOnClient(), vClients, Client, vTransactions);
        break;
    case enAdminMoreOperationOnClient::enwithdraw:
        system("cls");
        Amount = WithdrawScreen4AlreadyExistClient(vClients, Client);
        Client.TransactionType = enTransactionType::eWithdraw;
        Client.TransactionDescription = enTransactionDescription::eBranch;
        SaveClientTransaction(ClinetsTransactionsFile, Client, Amount);
        BackToTheMoreOperationOnClientScreen();
        PerformMoreOperationOnClientScreen(ReadAdminChoiceDoToMoreOperationsOnClient(), vClients, Client, vTransactions);
        break;
    case enAdminMoreOperationOnClient::enTotalTransactions:
        system("cls");
        ShowClientTransactionsScreen(Client.AccNumber);
        BackToTheMoreOperationOnClientScreen();
        PerformMoreOperationOnClientScreen(ReadAdminChoiceDoToMoreOperationsOnClient(), vClients, Client, vTransactions);
        break;
    case enAdminMoreOperationOnClient::enChangeAccStatus:
        system("cls");
        if (Client.AccStatus == enAccStatus::Blocked)
        {
            ChangeClientAccountStatus(Client.AccNumber, vClients);
            cout << "\nDone Successfully.\n";
        }
        else
        {
            ChangeClientAccountStatus(Client.AccNumber, vClients, enAccStatus::Blocked);
            cout << "\nDone Successfully.\n";
        }
        BackToTheMoreOperationOnClientScreen();
        PerformMoreOperationOnClientScreen(ReadAdminChoiceDoToMoreOperationsOnClient(), vClients, Client, vTransactions);
        break;
    case enAdminMoreOperationOnClient::enBacktoMainMenue:
        system("cls");
        ShowMainMenueScreen();
        break;
    }
}
void PerformClientScreenOptions(enClientMenueOptions ClientScreenOptions, vector <stClientData>& vClients, stClientData& Client)
{
    double Amount = 0.0;
    switch (ClientScreenOptions)
    {
    case enClientMenueOptions::enDeposit2:
        system("cls");
        Amount = DepositScreen4AlreadyExistClient(vClients, Client);
        Client.TransactionType = enTransactionType::eDeposit;
        Client.TransactionDescription = enTransactionDescription::eATM;
        SaveClientTransaction(ClinetsTransactionsFile, Client, Amount);
        BackToTheClientScreen();
        PerformClientScreenOptions(ReadClientChoiceForClientMenue(), vClients, Client);
        break;
    case enClientMenueOptions::enWithdraw2:
        system("cls");
        Amount = WithdrawScreen4AlreadyExistClient(vClients, Client);
        Client.TransactionType = enTransactionType::eWithdraw;
        Client.TransactionDescription = enTransactionDescription::eATM;
        SaveClientTransaction(ClinetsTransactionsFile, Client, Amount);
        BackToTheClientScreen();
        PerformClientScreenOptions(ReadClientChoiceForClientMenue(), vClients, Client);
        break;
    case enClientMenueOptions::enTransfer2:
        system("cls");
        TransferFromExistClient(vClients, Client);
        BackToTheClientScreen();
        PerformClientScreenOptions(ReadClientChoiceForClientMenue(), vClients, Client);
        break;
    case enClientMenueOptions::eUpdateAccount:
        system("cls");
        cout << "____________________________________\n\n";
        cout << "\tUPDATE CLIENT SCREEN";
        cout << "\n____________________________________\n";
        UpdateClientByAccountNumber(Client.AccNumber, vClients);
        BackToTheClientScreen();
        PerformClientScreenOptions(ReadClientChoiceForClientMenue(), vClients, Client);
        break;
    case enClientMenueOptions::eShowAccDetails:
        system("cls");
        PrintClientCard(Client);
        BackToTheClientScreen();
        PerformClientScreenOptions(ReadClientChoiceForClientMenue(), vClients, Client);
        break;
    case enClientMenueOptions::eTotalTransactions:
        system("cls");
        ShowClientTransactionsScreen(Client.AccNumber);
        BackToTheClientScreen();
        PerformClientScreenOptions(ReadClientChoiceForClientMenue(), vClients, Client);
        break;
    case enClientMenueOptions::eLogout2:
        ShowUserTypeScreen();
        break;
    }
}
void PerformLoginScreenOptions(enLoginOptions LoginOption)
{
    switch (LoginOption)
    {
    case enLoginOptions::eUser:
        UserLogin();
        ShowMainMenueScreen();
        break;
    case enLoginOptions::eClient:
        ClientLogin();
        break;
    case enLoginOptions::eExit:
        EndProgramScreen();
        break;
    }
}
void PerformTransactionsScreenOptions(enTransactionsOptions TransactionsOptions)
{
    switch (TransactionsOptions)
    {
    case enTransactionsOptions::enDeposit:
        system("cls");
        DepositScreen();
        BackToTheTransactionsScreen();
        break;
    case enTransactionsOptions::enWithdraw:
        system("cls");
        WithdrawScreen();
        BackToTheTransactionsScreen();
        break;
    case enTransactionsOptions::enTransfer:
        system("cls");
        TransferScreen();
        BackToTheTransactionsScreen();
        break;
    case enTransactionsOptions::eTotalBalance:
        system("cls");
        ShowClientsBalancesList();
        BackToTheTransactionsScreen();
        break;
    case enTransactionsOptions::eMainMenue:
        ShowMainMenueScreen();
        break;
    }
}
void PerformManageUsersScreenOptions(enManageUsersOptions ManageUserOptions)
{
    switch (ManageUserOptions)
    {
    case enManageUsersOptions::enShowUsersList:
        system("cls");
        ShowUsersListScreen();
        BackToTheManageUsersScreen();
        break;
    case enManageUsersOptions::enAddUser:
        system("cls");
        AddNewUserScreen();
        BackToTheManageUsersScreen();
        break;
    case enManageUsersOptions::enDeleteUser:
        system("cls");
        ShowDeleteUserScreen();
        BackToTheManageUsersScreen();
        break;
    case enManageUsersOptions::UpdateUser:
        system("cls");
        UpdateUserScreen();
        BackToTheManageUsersScreen();
        break;
    case enManageUsersOptions::enFindUser:
        system("cls");
        FindUserScreen();
        BackToTheManageUsersScreen();
        break;
    case enManageUsersOptions::enMainMenue:
        system("cls");
        ShowMainMenueScreen();
        break;
    }
}
void PerformMainMenueOptions(enMainMenueChooses MainMenueOption)
{
    switch (MainMenueOption)
    {
    case enMainMenueChooses::eShowClientList:
        system("cls");
        ShowClientsListScreen();
        BackToTheMainMenueScreen();
        break;
    case enMainMenueChooses::eAddNewClients:
        system("cls");
        AddNewClientScreen();
        BackToTheMainMenueScreen();
        break;
    case enMainMenueChooses::eDeleteClient:
        system("cls");
        DeleteClientScreen();
        BackToTheMainMenueScreen();
        break;
    case enMainMenueChooses::eDeleteAllClients:
        system("cls");
        DeleteAllClientsScreen();
        BackToTheMainMenueScreen();
        break;
    case enMainMenueChooses::eUpdateClientInfo:
        system("cls");
        UpdateClientScreen();
        BackToTheMainMenueScreen();
        break;
    case enMainMenueChooses::eFindClient:
        system("cls");
        FindClientScreen();
        break;
    case enMainMenueChooses::eTransactions:
        system("cls");
        TransactionScreen();
        break;
    case enMainMenueChooses::eManageUsers:
        system("cls");
        ShowManageUsersMenueScreen();
        break;
    case enMainMenueChooses::eLogout:
        system("cls");
        ShowUserTypeScreen();
        break;
    }
}

void ShowTransactionsMenueScreen()
{
    system("cls");
    cout << "______________________________________________________\n\n";
    cout << setw(10) << right << "|" << setw(27) << "TRANSACTIONS SCREEN" << setw(8) << right << "|";
    cout << "\n______________________________________________________\n";
    cout << "\n" << setw(10) << "|" << setw(20) << "[1] Deposit." << setw(15) << right << "|";
    cout << "\n" << setw(10) << "|" << setw(21) << "[2] Withdraw." << setw(14) << right << "|";
    cout << "\n" << setw(10) << "|" << setw(21) << "[3] Transfer." << setw(14) << right << "|";
    cout << "\n" << setw(10) << "|" << setw(27) << "[4] Total Balances." << setw(8) << right << "|";
    cout << "\n" << setw(10) << "|" << setw(23) << "[5] Main Menue." << setw(12) << right << "|";
    cout << "\n______________________________________________________\n";
    PerformTransactionsScreenOptions(ReadUserChoiceForTransaction());
}
void ShowManageUsersMenueScreen()
{
    if (!CheckAccessPermission(enMainMenuePermissions::ePManageUsers))
    {
        ShowAccessDeniedMessage();
        BackToTheMainMenueScreen();
        return;
    }
    system("cls");
    cout << "______________________________________________________\n\n";
    cout << setw(10) << right << "|" << setw(27) << "MANAGE USERS SCREEN" << setw(8) << right << "|";
    cout << "\n______________________________________________________\n";
    cout << "\n" << setw(10) << "|" << setw(28) << "[1] Show Users List." << setw(7) << right << "|";
    cout << "\n" << setw(10) << "|" << setw(26) << "[2] Add New Users." << setw(9) << right << "|";
    cout << "\n" << setw(10) << "|" << setw(24) << "[3] Delete User." << setw(11) << right << "|";
    cout << "\n" << setw(10) << "|" << setw(29) << "[4] Update User Info." << setw(6) << right << "|";
    cout << "\n" << setw(10) << "|" << setw(22) << "[5] Find User." << setw(13) << right << "|";
    cout << "\n" << setw(10) << "|" << setw(23) << "[6] Main Menue." << setw(12) << right << "|";
    cout << "\n______________________________________________________\n";
    PerformManageUsersScreenOptions(ReadUserChoiceForManageUserMainMenue());
}
void ShowMainMenueScreen()
{
    system("cls");
    cout << "______________________________________________________\n\n";
    cout << setw(10) << right << "|" << setw(25) << "MAIN MENUE SCREEN" << setw(10) << right << "|";
    cout << "\n______________________________________________________\n";
    cout << "\n" << setw(10) << "|" << setw(30) << "[1] Show Clients List." << setw(5) << right << "|";
    cout << "\n" << setw(10) << "|" << setw(28) << "[2] Add New Clients." << setw(7) << right << "|";
    cout << "\n" << setw(10) << "|" << setw(26) << "[3] Delete Client." << setw(9) << right << "|";
    cout << "\n" << setw(10) << "|" << setw(31) << "[4] Delete All Clients." << setw(4) << right << "|";
    cout << "\n" << setw(10) << "|" << setw(31) << "[5] Update Client Info." << setw(4) << right << "|";
    cout << "\n" << setw(10) << "|" << setw(24) << "[6] Find Client." << setw(11) << right << "|";
    cout << "\n" << setw(10) << "|" << setw(25) << "[7] Transactions." << setw(10) << right << "|";
    cout << "\n" << setw(10) << "|" << setw(25) << "[8] Manage Users." << setw(10) << right << "|";
    cout << "\n" << setw(10) << "|" << setw(19) << "[9] Logout." << setw(16) << right << "|";
    cout << "\n______________________________________________________\n";
    PerformMainMenueOptions(ReadUserChoiceForMainMenu());
}
void ShowUserTypeScreen()
{
    system("cls");
    cout << "______________________________________________________\n\n";
    cout << setw(10) << right << "|" << setw(25) << "USER TYPE SCREEN" << setw(10) << right << "|";
    cout << "\n______________________________________________________\n";
    cout << "\n" << setw(10) << "|" << setw(18) << "[1] User." << setw(17) << right << "|";
    cout << "\n" << setw(10) << "|" << setw(20) << "[2] Client." << setw(15) << right << "|";
    cout << "\n" << setw(10) << "|" << setw(18) << "[3] Exit." << setw(17) << right << "|";
    cout << "\n______________________________________________________\n";
    PerformLoginScreenOptions(ReadUserChoiceForLogin());
}
void ShowClientScreen()
{
    system("cls");
    cout << "______________________________________________________\n\n";
    cout << setw(10) << right << "|" << setw(21) << "CLIENT SCREEN" << setw(14) << right << "|";
    cout << "\n______________________________________________________\n";
    cout << "\n" << setw(10) << "|" << setw(20) << "[1] Deposit." << setw(15) << right << "|";
    cout << "\n" << setw(10) << "|" << setw(21) << "[2] Withdraw." << setw(14) << right << "|";
    cout << "\n" << setw(10) << "|" << setw(21) << "[3] Transfer." << setw(14) << right << "|";
    cout << "\n" << setw(10) << "|" << setw(27) << "[4] Update Account." << setw(8) << right << "|";
    cout << "\n" << setw(10) << "|" << setw(28) << "[5] Account Details." << setw(7) << right << "|";
    cout << "\n" << setw(10) << "|" << setw(31) << "[6] Total Transactions." << setw(4) << right << "|";
    cout << "\n" << setw(10) << "|" << setw(19) << "[7] Logout." << setw(16) << right << "|";
    cout << "\n______________________________________________________\n";
}
void ShowMoreOperationOnClientScreen()
{
    system("cls");
    cout << "______________________________________________________\n\n";
    cout << setw(10) << right << "|" << setw(30) << "OPERATION ON CLIENT SCREEN" << setw(5) << right << "|";
    cout << "\n______________________________________________________\n";
    cout << "\n" << setw(10) << "|" << setw(20) << "[1] Deposit." << setw(15) << right << "|";
    cout << "\n" << setw(10) << "|" << setw(21) << "[2] Withdraw." << setw(14) << right << "|";
    cout << "\n" << setw(10) << "|" << setw(31) << "[3] Total Transactions." << setw(4) << right << "|";
    cout << "\n" << setw(10) << "|" << setw(26) << "[4] Change Status." << setw(9) << right << "|";
    cout << "\n" << setw(10) << "|" << setw(23) << "[5] Main Menue." << setw(12) << right << "|";
    cout << "\n______________________________________________________\n";

}
void ShowClientLoginScreen()
{
    system("cls");
    cout << "____________________________________\n\n";
    cout << "\tCLIENT LOGIN SCREEN";
    cout << "\n____________________________________\n";
    cout << "\nPlease Enter The Account Number: ";
}
void ShowUserLoginScreen()
{
    system("cls");
    cout << "____________________________________\n\n";
    cout << "\tUSER LOGIN SCREEN";
    cout << "\n____________________________________\n";
}

void UserLogin()
{
    string UserName = "", Password = "";
    ShowUserLoginScreen();
    bool LoginFailed;
    do
    {
        LoginFailed = false;
        cout << "\nPlease Enter The Username: ";
        UserName = MyLib::ReadString();
        cout << "Please Enter The Password: ";
        Password = MyLib::ReadString();
        if (!FindUserByUsernameAndPassword(Password, UserName, CurrentUser))
        {
            ShowUserLoginScreen();
            cout << "\nInvalid Username / Password!.\n";
            LoginFailed = true;
        }
    } while (LoginFailed);
}
bool ClientLogin()
{
    ShowClientLoginScreen();
    vector <stClientData> vClients = LoadClientsDataFromFileToVector(ClientsFileName);
    string PinCode = "";
    char Ans = 'N';
    stClientData Client;
    string AccNumber = MyLib::ReadString();

    if (FindClientByAccNumber(AccNumber, vClients, Client))
    {
        if (Client.AccStatus == enAccStatus::Blocked)
        {
            cout << "\nYour card is Blocked, Please call the bank for help\n";
            BackToTheUserTypeScreen();
        }
        else
        {
            cout << "\nPlease Enter The Password: ";
            short FailedCounter = 3;
            do
            {
                FailedCounter--;
                PinCode = MyLib::ReadString();
                for (stClientData& C : vClients)
                {
                    if (C.AccNumber == AccNumber)
                    {
                        if (PinCode == C.PinCode)
                        {
                            ShowClientScreen();
                            PerformClientScreenOptions(ReadClientChoiceForClientMenue(), vClients, C);
                            return true;
                        }
                    }
                }
                cout << "\nWrong PIN, you have " << FailedCounter << " More Tries \n";
            } while (FailedCounter >= 1);

            ChangeClientAccountStatus(AccNumber, vClients, enAccStatus::Blocked);
            cout << "\nYour card is Blocked, Please call the bank for help\n";
            BackToTheUserTypeScreen();
            return false;

        }
    }
    else
    {
        cout << "\n\nClient with Account Number [" << AccNumber << "] is not found!\n";
        BackToTheUserTypeScreen();
    }
}

int main()
{
    ShowWelcomeText();
    return 0;
}