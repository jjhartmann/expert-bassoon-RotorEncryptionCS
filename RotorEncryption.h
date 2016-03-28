//
// Created by Jeremy on 3/27/2016.
//

#ifndef ROTORENCYPTIONDIFFIEHELLMAN_ROTORENCRYPTION_H
#define ROTORENCYPTIONDIFFIEHELLMAN_ROTORENCRYPTION_H

#include <string>
#include <vector>

using namespace std;

// Struct definition for Encruyption scheme
struct EScheme {
    EScheme(int i, int c, int len) :
            schemeId(i),
            rotorCount(c)
    {
        ioMap = new char[len * c];
    }

    ~EScheme(){
        if (ioMap)
        {
            delete[] ioMap;
        }
    }

    int schemeId;
    int rotorCount;
    char *ioMap;
};

// Main class
class RotorEncryption
{
public:
    static void buildEncryptionSchemeFlatFile(int rotorcount, int schemeCount);
    static string asciimap;

    // De/Constructor
    RotorEncryption();
    ~RotorEncryption();

    // Set current schemeID
    inline void setSchemeId(int id) { mCurrentSchemeId = id; }

    // Encrypt/Decrypt
    string encrypt(string message);
    string decrypt(string message);
private:
    // Vars
    vector<EScheme*> mSchemes; // Hold all shemes
    int *offsetMap;
    int mRotorCount;
    int mSchemeCount;
    int mMLength;
    int mCurrentSchemeId;

    // Private Methods
    void generateEncryptionSchemeArray();
    char encryptchar(char c);

    // Static Methods
    static void permuteASCIIMap(string &map);

};


#endif //ROTORENCYPTIONDIFFIEHELLMAN_ROTORENCRYPTION_H
