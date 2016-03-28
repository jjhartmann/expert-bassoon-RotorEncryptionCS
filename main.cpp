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

using namespace std;

#define MCHARS 94

// Forward Declaration
void buildEncryptionSchemeFlatFile(int rotorcount, int schemeCount);


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
void permuteASCIIMap(string &map)
{
    // Create random permutation of string
    srand(time(NULL));
    int len = map.length();
    for (int i = len; i > 1; --i)
    {
        int offset = rand() % len;
        swap(map[offset], map[i - 1]);
    }
}

void buildEncryptionSchemeFlatFile(int rotorcount, int schemeCount)
{
    // Seed RNG
    srand(time(NULL));
    string asciimap = " !\"#$%&\'()*+,-./0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[]^_`abcdefghijklmnopqrstuvwxyz{|}~";
    int len = asciimap.length();
    // Open file to write to
    ofstream file("encyptionscheme.txt");
    for (int i = 0; i < schemeCount; ++i)
    {
        // identify the scheme number
        file << i << "\n";
        // For the number of rotors
        for (int j = 0; j < rotorcount; ++j)
        {
            // Permute map
            permuteASCIIMap(asciimap);

            // For each character in map
            for (int k = 0; k < len - 1; ++k)
            {
                file <<  asciimap[k] << "\t";
            }
            // Last char
            file <<  asciimap[len - 1] << "\n";
        }

        file << "\n";
    }
    // Close file handle
    file.close();
}
