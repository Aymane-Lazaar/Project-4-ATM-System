#include <string>
#include <iostream>
#include <fstream>
#include <vector>
#include <iomanip>

using namespace std;

const string ClientsFileName = "Clients.txt";

struct sClient
{
    string AccountNumber;
    string PinCode;
    string Name;
    string Phone;
    double AccountBalance;
    // bool MarkForUpdate = false;
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

vector<sClient> SaveCleintsDataToFile(string ClientsFileName, vector<sClient> &vClients)
{
    fstream MyFile;
    MyFile.open(ClientsFileName, ios::out); // overwrite

    string DataLine;

    if (MyFile.is_open())
    {
        for (sClient &cl : vClients)
        {
            DataLine = ConvertRecordToLine(cl);

            MyFile << DataLine << endl;
        }
        MyFile.close();
    }
    return vClients;
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

int main()
{

    vector<sClient> vClients = LoadCleintsDataFromFile(ClientsFileName);

    string AccountNumber = ReadClientAccountNumber();

    UpdateClientByAccountNumber(AccountNumber, vClients);

    return 0;

}