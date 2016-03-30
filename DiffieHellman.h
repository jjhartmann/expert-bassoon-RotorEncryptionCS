//
// Created by Jeremy on 3/28/2016.
//

#ifndef ROTORENCYPTIONDIFFIEHELLMAN_DIFFIEHELLMAN_H
#define ROTORENCYPTIONDIFFIEHELLMAN_DIFFIEHELLMAN_H

using namespace std;

class DiffieHellman
{
public:
    DiffieHellman();
    DiffieHellman(long int g, long int p);

    void gen(long int B);
    long int encrypt(long int schemeId);
    long int decrypt(long int val);
    inline long int getmP() { return mP; }
    inline long int getmG() { return mG; }
    inline long int getmA() { return mA; }
private:
    long int mG;
    long int mP;
    long int mKey;
    long int mA;
    long int mB;
    long int mSharedKey;

    long int sievePrimes(long int N);

};


#endif //ROTORENCYPTIONDIFFIEHELLMAN_DIFFIEHELLMAN_H
