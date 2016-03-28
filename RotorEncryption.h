//
// Created by Jeremy on 3/27/2016.
//

#ifndef ROTORENCYPTIONDIFFIEHELLMAN_ROTORENCRYPTION_H
#define ROTORENCYPTIONDIFFIEHELLMAN_ROTORENCRYPTION_H

#include <string>

// Struct definition for Encruyption scheme
struct EScheme {
    int id;
    int rotorCount;
    int *ioMap;
};

// Main class
class RotorEncryption
{
public:
    static void buildEncryptionSchemeFlatFile(int rotorcount, int schemeCount);
    void generateEncryptionSchemeArray();

private:
    static void permuteASCIIMap(std::string &map);

};


#endif //ROTORENCYPTIONDIFFIEHELLMAN_ROTORENCRYPTION_H
