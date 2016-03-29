////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Client/Server Rotor Encryption Simulation
//
// Jeremy J. Hartmann
// 2016
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <ctime>
#include <algorithm>

#include "RotorEncryption.h"

using namespace std;

#define MCHARS 94

// Forward Declaration
void buildEncryptionSchemeFlatFile(int rotorcount, int schemeCount);

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Main
int main()
{
    cout << "Rotor Encryption With Diffie-Hellman" << endl;

    cout << "Choose Option:\n\tBuild Encryption Schemes: 0\n\tSet Up Client: 1\n\tSet Up Server: 2\n\n Answer: ";
    string choice;
    cin >> choice;
    int choiceVal = atoi(choice.c_str());

    if (choiceVal > 2 || choiceVal < 0)
    {
        cout << "Invalid Choice." << endl;
        return 1;
    }

    if (choiceVal == 0)
    {
        // Build the encryption scheme to be shared between the server and client.
        // Output is a flatfile in .txt format
        string scheme;
        string rotor;

        cout << "Number of encryption schemes: ";
        cin >> scheme;
        cout << "\nNumber of rotors per scheme: ";
        cin >> rotor;
        int schemeCount = atoi(scheme.c_str());
        int rotorCount = atoi(rotor.c_str());
        RotorEncryption::buildEncryptionSchemeFlatFile(rotorCount, schemeCount);

    }
    if (choiceVal == 1)
    {
        // Set up the Client
        RotorEncryption device;
        device.setSchemeId(0);
        string message = "Hello this is a string and a test to see how much this will work. I am writing to you today to warn you about the never ending waterfall in the distance horizon. I know this may come to you as a skock but this needs to be addressed in oder to prevent a grave catatastraphy.";
        string response = device.encrypt(message);
        cout << response << endl;
        string decrypted = device.decrypt(response);
        cout << decrypted << endl;
    }
    if (choiceVal == 2)
    {
        // Set up the Server.
    }

    return 0;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Server



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Client



