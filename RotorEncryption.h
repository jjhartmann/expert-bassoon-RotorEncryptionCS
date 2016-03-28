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
            rotorCount(c),
            ioMap(new char[len * c])
    {}

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

    void generateEncryptionSchemeArray();
private:
    // Vars
    vector<EScheme> mSchemes; // Hold all shemes

    // Static Methods
    static void permuteASCIIMap(string &map);

};


#endif //ROTORENCYPTIONDIFFIEHELLMAN_ROTORENCRYPTION_H
