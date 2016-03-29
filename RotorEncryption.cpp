//
// Created by Jeremy on 3/27/2016.
//

#include "RotorEncryption.h"
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <ctime>
#include <algorithm>
#include <cstring>

using namespace std;

// Static Methods
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Build Encryption Scheme
string RotorEncryption::asciimap = " !\"#$%&\'()*+,-./0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\\]^_`abcdefghijklmnopqrstuvwxyz{|}~";
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
// Constructor
RotorEncryption::RotorEncryption() :
    mCurrentSchemeId(0)
{
    generateEncryptionSchemeArray();
}

// Deconstructor
RotorEncryption::~RotorEncryption()
{
    for (int i = 0; i < mSchemes.size(); ++i)
    {
        EScheme *tmp =  mSchemes[i];
        delete tmp;
    }

    if (offsetMap)
    {
        delete[] offsetMap;
    }
}

// Encypt message based on schemeId
string RotorEncryption::encrypt(string message)
{
    memset(offsetMap, 0, sizeof(int) * mRotorCount);
    string encryptMessage;
    for (int i = 0; i < message.length(); ++i)
    {
        encryptMessage += encryptchar(message[i]);
    }

    return encryptMessage;
}

// Decrypt message based on schemeId
string RotorEncryption::decrypt(string message)
{
    memset(offsetMap, 0, sizeof(int) * mRotorCount);
    string decryptMessage;
    for (int i = 0; i < message.length(); ++i)
    {
        decryptMessage += decryptchar(message[i]);
    }

    return decryptMessage;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Private Methods
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
    mSchemeCount = atoi(line.c_str());
    getline(file, line);
    mRotorCount = atoi(line.c_str());
    getline(file, line);
    mMLength = atoi(line.c_str());

    // Retrieve schemes
    while(getline(file, line))
    {
        if(line.length() == 0) continue; // Nothing to parse

        // Get scheme id and create Scheme
        if (line.length() < 10)
        {
            int schemeId = atoi(line.c_str());
            EScheme *scheme = new EScheme(schemeId, mRotorCount, mMLength);
            mSchemes.push_back(scheme);
            continue;
        }

        // Create ioMap for each rotor in scheme
        for (int i = 0; i < mRotorCount; ++i)
        {
            // For each char on line
            for (int j = 0; j < mMLength; ++j)
            {
//                cout << "<" <<line[j] << " " << (char)((j) + ' ') << ">";
                mSchemes.back()->ioMapEn[(i * mMLength) + j] = line[j];
                mSchemes.back()->ioMapDe[(i * mMLength) + ((int)line[j] -  (int)' ')] = ((char)(j) + ' ');
            }
//            cout << endl;

            getline(file, line);
        }
    }

    // Create offset map
    offsetMap = new int[mRotorCount];
    memset(offsetMap, 0, sizeof(int) * mRotorCount);

    file.close();
}

// Encrypt a single char and increment offset counters.
char RotorEncryption::encryptchar(char c)
{
    EScheme *currentScheme = mSchemes[mCurrentSchemeId];
    char *currentMap = currentScheme->ioMapEn;
    int n_c = c - ' ';
    for (int i = 0; i < mRotorCount; ++i)
    {
        n_c = (currentMap[(i * mMLength) + ((n_c + offsetMap[i]) % mMLength)]) - (int)(' ');
    }

    // Set current char
    c = (char) n_c + ' ';

    // Increment offsets
    incrementOffset();

    return c;
}

// Encrypt a single char and increment offset counters.
char RotorEncryption::decryptchar(char c)
{
    EScheme *currentScheme = mSchemes[mCurrentSchemeId];
    char *currentMap = currentScheme->ioMapDe;
    int n_c = c - ' ';
//    cout << "Decrypt: " << c << endl;
    for (int i = mRotorCount - 1; i >= 0; --i)
    {
        n_c = (int)(currentMap[(i * mMLength) + n_c] - ' ') - offsetMap[i];
        if (n_c < 0)
        {
            n_c = mMLength + n_c;
        }
//        cout << (char)(n_c + ' ');
    }
//    cout << endl;
    // Set current char
    c = (char) n_c + ' ';

    // Increment offsets
    incrementOffset();

    return c;
}

void RotorEncryption::incrementOffset()
{
    // Increment offsets
    int offset = offsetMap[0];
    offset = (offset + 1) % mMLength;
    offsetMap[0] = offset;

    int index = 0;
    while (offsetMap[index] == 0 && index < mRotorCount - 1)
    {
        index++;
        offset = offsetMap[index];
        offset = (offset + 1) % mMLength;
        offsetMap[index] = offset;
    }
}









