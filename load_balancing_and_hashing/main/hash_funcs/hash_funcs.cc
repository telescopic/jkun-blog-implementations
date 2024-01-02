#include <iostream>
#include "hash_funcs.h"

using namespace std;

int polynomialHash(int a, int b, int p, int x) {
    if(p <= 0) {
        cout<< "Invalid value for p, should be prime";
        return -1; 
    }
    return (a * x + b) % p;
}

// O(root(N))
bool isValidPrime(int numForPrimeChecking) {
    if (numForPrimeChecking <= 0) return false;

    for(int i=2; i*i <= numForPrimeChecking; i++) {
        if( numForPrimeChecking % i == 0) return false;
    }
    return true;
}