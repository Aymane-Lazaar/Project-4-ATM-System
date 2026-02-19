#include <string>
#include <iostream>
#include <fstream>
#include <vector>
#include <iomanip>
#include <limits>

using namespace std;

const string ClientsFileName = "Clients.txt";

struct sClient
{
    string AccountNumber;
    string PinCode;
    string Name;
    string Phone;
    double AccountBalance;
    bool MarkForDelete = false;
};

enum enChoise
{
    showClient = 1,
    AddClient = 2,
    DeleteClient = 3,
    UpdateClient = 4,
    FindClient = 5,
    Exit = 6
};

string ReadClientAccountNumber()
{
    string AccountNumber = "";
    cout << "\nPlease enter AccountNumber? ";
    cin >> AccountNumber;

    return AccountNumber;
}

vector<string> SplitString(string S1, string Delim)
{
    vector<string> vString;

    short pos = 0;
    string sWord;

    while ((pos = S1.find(Delim)) != std::string::npos)
    {
        sWord = S1.substr(0, pos); // substr(start, length)
        if (sWord != "")
        {
            vString.push_back(sWord);
        }

        S1.erase(0, pos + Delim.length()); // erase(start, length)
    }

    if (S1 != "")
    {
        vString.push_back(S1);
    }

    return vString;
}

sClient ConvertLinetoRecord(string Line, string Seperator = "#//#")
{
    sClient Client;
    vector<string> vClientData;

    vClientData = SplitString(Line, Seperator);

    Client.AccountNumber = vClientData[0];
    Client.PinCode = vClientData[1];
    Client.Name = vClientData[2];
    Client.Phone = vClientData[3];
    Client.AccountBalance = stod(vClientData[4]); // cast string to double

    return Client;
}

string ConvertRecordToLine(sClient Client, string Seperator = "#//#")
{
    string stClientRecord = "";

    stClientRecord += Client.AccountNumber + Seperator;
    stClientRecord += Client.PinCode + Seperator;
    stClientRecord += Client.Name + Seperator;
    stClientRecord += Client.Phone + Seperator;
    stClientRecord += to_string(Client.AccountBalance);

    return stClientRecord;
}

vector<sClient> LoadCleintsDataFromFile(string FileName)
{
    /*
        Reading → ios::in

        Writing → ios::out

        Appending → ios::app
    */

    fstream file;

    file.open(ClientsFileName, ios::in); // Read Mode
    string Line;

    vector<sClient> vClient;
    sClient Client;

    if (file.is_open())
    {
        while (getline(file, Line))
        {
            Client = ConvertLinetoRecord(Line, "#//#");
            vClient.push_back(Client);
        }
    }
    return vClient;
}

bool FindClientByAccountNumber(string AccountNumber, vector<sClient> vClients, sClient &Client)
{
    // vClients = LoadCleintsDataFromFile(ClientsFileName);

    for (sClient &cl : vClients)
    {
        if (cl.AccountNumber == AccountNumber)
        {
            /*
                When this runs :

                Client = cl;

            C++ performs a member - wise copy :

                Equivalent to :

                Client.AccountNumber = cl.AccountNumber;
                Client.PinCode = cl.PinCode;
                Client.Name = cl.Name;
                Client.Phone = cl.Phone;
                Client.AccountBalance = cl.AccountBalance;
            */

            Client = cl;
            return true;
        }
    }

    return false;
}

void PrintClientCard(sClient Client)
{
    cout << "\nThe following are the client details:\n";
    cout << "\nAccout Number: " << Client.AccountNumber;
    cout << "\nPin Code     : " << Client.PinCode;
    cout << "\nName         : " << Client.Name;
    cout << "\nPhone        : " << Client.Phone;
    cout << "\nAccount Balance: " << Client.AccountBalance;
}

bool MarkClientForDeleteByAccountNumber(string AccountNumber, vector<sClient> &vClients)
{
    for (sClient &cl : vClients)
    {
        if (cl.AccountNumber == AccountNumber)
        {
            cl.MarkForDelete = true;
            return true;
        }
    }
    return false;
}

vector<sClient> SaveCleintsDataToFile(string ClientsFileName, vector<sClient> vClients)
{
    fstream MyFile;
    MyFile.open(ClientsFileName, ios::out); // overwrite

    string DataLine;

    if (MyFile.is_open())
    {
        for (sClient &cl : vClients)
        {
            if (cl.MarkForDelete == false)
            {
                // we only write records that are not marked for delete.
                DataLine = ConvertRecordToLine(cl);
                MyFile << DataLine << endl;
            }
        }
        MyFile.close();
    }
    return vClients;
}

bool DeleteClientByAccountNumber(string AccountNumber, vector<sClient> &vClients)
{
    sClient Client;
    char Answer = 'n';

    if (FindClientByAccountNumber(AccountNumber, vClients, Client))
    {
        PrintClientCard(Client);

        cout << "\nAre you sure you want delete this client? y/n ?";
        cin >> Answer;

        if (Answer == 'y' || Answer == 'Y')
        {
            MarkClientForDeleteByAccountNumber(AccountNumber, vClients);
            SaveCleintsDataToFile(ClientsFileName, vClients);

            // Refresh Clients
            // vClients = LoadCleintsDataFromFile(ClientsFileName);

            cout << "\n\nClient Deleted Successfully.";
            return true;
        }
    }
    else
    {
        cout << "\nClient with Account Number (" << AccountNumber << ") is Not Found!";
        return false;
    }
    return false;
}

void MenuScreen()
{
    cout << "==================================\n";
    cout << "             Main Menu  Screen    \n";
    cout << "==================================\n";

    cout << "         [1] Show Client List.\n";
    cout << "         [2] Add New Client.\n";
    cout << "         [3] Delete Client.\n";
    cout << "         [4] Update Client.\n";
    cout << "         [5] Find Client.\n";
    cout << "         [6] Exist.\n";
}

void PrintClientRecord(sClient Client)
{
    cout << "| " << setw(15) << left << Client.AccountNumber;
    cout << "| " << setw(10) << left << Client.PinCode;
    cout << "| " << setw(40) << left << Client.Name;
    cout << "| " << setw(12) << left << Client.Phone;
    cout << "| " << setw(12) << left << Client.AccountBalance;
}

void PrintAllClientsData(vector<sClient> &vClients)
{
    cout << "\n\t\t\t\t\tClient List (" << vClients.size() << ")Client(s).";
    cout
        << "\n_______________________________________________________";
    cout << "_________________________________________\n"
         << endl;
    cout << "| " << left << setw(15) << "Accout Number";
    cout << "| " << left << setw(10) << "Pin Code";
    cout << "| " << left << setw(40) << "Client Name";
    cout << "| " << left << setw(12) << "Phone";
    cout << "| " << left << setw(12) << "Balance";
    cout << "\n_______________________________________________________";
    cout << "_________________________________________\n"
         << endl;

    // Method 1
    for (sClient &client : vClients)
    {
        PrintClientRecord(client);
        cout << endl;
    }

    cout << "\n_______________________________________________________";
    cout << "_________________________________________\n"
         << endl;
}

void AddDataLineToFile(string FileName, string stDataLine)
{

    fstream MyFile;

    MyFile.open(FileName, ios::out | ios::app);
    if (MyFile.is_open())
    {
        MyFile << stDataLine << endl;
        MyFile.close();
    }
}

sClient ReadNewClient(vector<sClient> vClients)
{
    sClient Client;

    cout << "Enter Account Number? ";

    // getline() only works with strings
    // Usage of std::ws will extract allthe whitespace character (" " and \n and \t)
    getline(cin >> ws, Client.AccountNumber);

    for (short i = 0; i < vClients.size(); i++)
    {
        while (Client.AccountNumber == vClients[i].AccountNumber)
        {
            cout << "Client with [" << Client.AccountNumber << "] aleready exists, ";
            cout << "Enter another Account Number?";
            getline(cin >> ws, Client.AccountNumber);
            i = 0;
        }
        vClients = LoadCleintsDataFromFile(ClientsFileName);
    }

    cout << "Enter PinCode? ";
    getline(cin, Client.PinCode);

    cout << "Enter Name? ";
    getline(cin, Client.Name);

    cout << "Enter Phone? ";
    getline(cin, Client.Phone);

    cout << "Enter AccountBalance? ";
    cin >> Client.AccountBalance;

    return Client;
}

void AddNewClient(vector<sClient> vClients)
{
    sClient Client;
    Client = ReadNewClient(vClients);
    AddDataLineToFile(ClientsFileName, ConvertRecordToLine(Client));
}

void AddClients(vector<sClient> vClients)
{
    char AddMore = 'Y';
    do
    {
        system("cls");
        cout << "Adding New Client:\n\n";

        AddNewClient(vClients);
        cout << "\nClient Added Successfully, do you want to add more clients? Y / N ? ";
        cin >> AddMore;

    } while (toupper(AddMore) == 'Y');
}

sClient ChangeClientRecord(string AccountNumber)
{
    sClient Client;

    Client.AccountNumber = AccountNumber;

    cout << "\n\nEnter PinCode? ";
    getline(cin >> ws, Client.PinCode);

    cout << "Enter Name? ";
    getline(cin, Client.Name);

    cout << "Enter Phone? ";
    getline(cin, Client.Phone);

    cout << "Enter AccountBalance? ";
    cin >> Client.AccountBalance;

    return Client;
}

bool UpdateClientByAccountNumber(string AccountNumber, vector<sClient> &vClients)
{
    sClient Client;
    char Answer = 'n';

    if (FindClientByAccountNumber(AccountNumber, vClients, Client))
    {
        PrintClientCard(Client);

        cout << "\nAre you sure you want Update this client? y/n ?";
        cin >> Answer;

        if (Answer == 'y' || Answer == 'Y')
        {
            for (sClient &cl : vClients)
            {
                if (cl.AccountNumber == AccountNumber)
                {
                    cl = ChangeClientRecord(AccountNumber);
                    break;
                }
            }

            SaveCleintsDataToFile(ClientsFileName, vClients);

            // Refresh Clients
            // vClients = LoadCleintsDataFromFile(ClientsFileName);

            cout << "\n\nClient Update Successfully.";
            return true;
        }
        cout << "\n\nOperation Cancelled. Client data remains unchanged.\n";

        return false;
    }
    else
    {
        cout << "\nClient with Account Number (" << AccountNumber << ") is Not Found!";
        return false;
    }
    return false;
}

void Start(short Choice)
{
    vector<sClient> vClients = LoadCleintsDataFromFile(ClientsFileName);
    sClient Client;

    string AccountNumber;

    switch (Choice)
    {
    case enChoise::showClient:
        PrintAllClientsData(vClients);
        break;
    case enChoise::AddClient:
        AddClients(vClients);
        break;
    case enChoise::DeleteClient:
        DeleteClientByAccountNumber(ReadClientAccountNumber(), vClients);
        break;
    case enChoise::UpdateClient:
        UpdateClientByAccountNumber(ReadClientAccountNumber(), vClients);
        break;
    case enChoise::FindClient:
        AccountNumber = ReadClientAccountNumber();

        if (FindClientByAccountNumber(AccountNumber, vClients, Client))
            PrintClientCard(Client);
        else
            cout << "\nClient With Account Number (" << AccountNumber << ") is Not Found!";
        break;

    case enChoise::Exit:
        return;

    default:
        break;
    }
}

void Pause()
{
    cout << "\nPress any key to continue . . .";
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    cin.get();
}

int main()
{

    while (true)
    {
        // system("cls");
        MenuScreen();

        short Choice;

        cout << "Choose what do you want to do? [1 to 6]?";
        cin >> Choice;

        if (Choice == enChoise::Exit)
            break;

        Start(Choice);

        Pause();
    }

    return 0;
}