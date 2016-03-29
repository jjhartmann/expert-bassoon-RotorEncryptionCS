//
// Created by Jeremy on 3/28/2016.
//
#include <vector>
#include <cmath>
#include <ctime>
#include <cstdlib>

#include "DiffieHellman.h"

DiffieHellman::DiffieHellman() :
    mA(0),
    mB(0),
    mSharedKey(0)
{
    sievePrimes(100000);
    srand(time(0));

    mKey = (rand() % 10000) + 1000;

}

DiffieHellman::DiffieHellman(long int g, long int p) :
    mP(p),
    mG(g),
    mA(0),
    mB(0),
    mSharedKey(0)
{
    mKey = (rand() % 10000) + 1000;
}

long int DiffieHellman::encrypt(long int schemeId)
{
    return 0;
}

void DiffieHellman::gen(long int B)
{

}

// Sieve and generate prime numbers
long int DiffieHellman::sievePrimes( long int N)
{
    // Genereate primes with the sieve of eratosthenes
    vector<bool> buffer(N, true);
    for (long int i = 2; i < sqrt(N); ++i)
    {
        if (buffer[i])
        {
            for (long int j = i*i; j < N; j += i)
            {
                buffer[j] = false;
            }
        }
    }

    // Get two random primes.
    srand(time(0));

    long int index = (rand() % (long)(N * 0.1)) + (N * 0.9);
    while (!buffer[index]) { index++; }
    mP = index;

    index = (rand() % (3*N)/10) + (2*N)/10;
    while (!buffer[index]){ index++; }
    mG = index;
}





