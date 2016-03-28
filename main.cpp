////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Client/Server Rotor Encryption Simulation
//
// Jeremy J. Hartmann
// 2016
#include <iostream>
#include <cstdlib>

using namespace std;

// Forward Declaration
void buildEncryptionSchemeFlatFile(int rotorcount, int N);


// Struct definition for Encruyption scheme
struct EScheme {
    int id;
    int rotorCount;
    int *ioMap;
};

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
        buildEncryptionSchemeFlatFile(rotorCount, schemeCount);

    }
    if (choiceVal == 1)
    {
        // Set up the Client
    }
    if (choiceVal == 2)
    {
        // Set up the Server.
    }

    return 0;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Build Encryption Scheme
void buildEncryptionSchemeFlatFile(int rotorcount, int N)
{






}
