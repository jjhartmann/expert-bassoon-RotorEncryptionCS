//
// Created by Jeremy on 3/27/2016.
//

#include "RotorEncryption.h"
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <ctime>
#include <algorithm>

using namespace std;

// Static Methods
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Build Encryption Scheme
string RotorEncryption::asciimap = " !\"#$%&\'()*+,-./0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[]^_`abcdefghijklmnopqrstuvwxyz{|}~";
void RotorEncryption::buildEncryptionSchemeFlatFile(int rotorcount, int schemeCount)
{
    // Seed RNG
    srand(time(NULL));
    int len = RotorEncryption::asciimap.length();
    // Open file to write to
    ofstream file("encyptionscheme.txt");

    // Set header
    file << schemeCount << "\n" << rotorcount << "\n" << len << "\n\n";
    for (int i = 0; i < schemeCount; ++i)
    {
        // identify the scheme number
        file << i << "\n";
        // For the number of rotors
        for (int j = 0; j < rotorcount; ++j)
        {
            // Permute map
            permuteASCIIMap(RotorEncryption::asciimap);

            // For each character in map
            for (int k = 0; k < len - 1; ++k)
            {
                file <<  RotorEncryption::asciimap[k];
            }
            // Last char
            file <<  RotorEncryption::asciimap[len - 1] << "\n";
        }

        file << "\n";
    }
    // Close file handle
    file.close();
}

void RotorEncryption::permuteASCIIMap(string &map)
{
    // Create random permutation of string
    srand(time(NULL));
    int len = map.length();
    for (int i = len - 1; i >= 0; --i)
    {
        int offset = rand() % len;
        swap(map[offset], map[i]);
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Rotor Cipher Encryption/Decryption

// Deconstructor
RotorEncryption::~RotorEncryption()
{
    for (int i = 0; i < mSchemes.size(); ++i)
    {
        delete mSchemes[i];
    }
}


// Generate the Encryption Vector from the flat files
void RotorEncryption::generateEncryptionSchemeArray()
{
    int maplen = asciimap.length();

    ifstream file("encyptionscheme.txt");
    if (!file.is_open())
    {
        cout << "Failded to open encyptionscheme.txt. Make sure it is in working dir." << endl;
        return;
    }
    string line;
    // Retrieve Header
    getline(file, line);
    int schemeCount = atoi(line.c_str());
    getline(file, line);
    int rotorCount = atoi(line.c_str());
    getline(file, line);
    int mlen = atoi(line.c_str());

    // Retrieve schemes
    while(getline(file, line))
    {
        if(line.length() == 0) continue; // Nothing to parse

        // Get scheme id and create Scheme
        if (line.length() < 10)
        {
            int schemeId = atoi(line.c_str());
            EScheme *scheme = new EScheme(schemeId, rotorCount, mlen);
            mSchemes.push_back(scheme);
            continue;
        }

        // Create ioMap for each rotor in scheme
        for (int i = 0; i < rotorCount; ++i)
        {
            // For each char on line
            for (int j = 0; j < mlen; ++j)
            {
                mSchemes.back()->ioMap[(i * mlen) + j] = line[j];
            }

            getline(file, line);
        }
    }

    file.close();
}