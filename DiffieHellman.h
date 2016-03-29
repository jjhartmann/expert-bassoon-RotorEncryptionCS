//
// Created by Jeremy on 3/28/2016.
//

#ifndef ROTORENCYPTIONDIFFIEHELLMAN_DIFFIEHELLMAN_H
#define ROTORENCYPTIONDIFFIEHELLMAN_DIFFIEHELLMAN_H


class DiffieHellman
{
public:
    DiffieHellman();
    DiffieHellman(long long int g, long long int p);

    void gen(long long int B);
    long long int encrypt(long long int schemeId);

private:
    long long int mG;
    long long int mP;
    long long int mKey;
    long long int mA;
    long long int mB;
    long long int mSharedKey;
};


#endif //ROTORENCYPTIONDIFFIEHELLMAN_DIFFIEHELLMAN_H
