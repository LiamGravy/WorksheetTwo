#include "../drivers/timerInterrupt.h"

unsigned int seed = 0;

void seedRandom()
{
    seed = get_cpu_clock();
}

int LehmerRNG(unsigned int seed)
{
    const unsigned int a = 16807;
    const unsigned int m = 2147483647; 
    unsigned int randomNumber = (a * seed) % m;
    return randomNumber;
}

int getRandomNumber(int min, int max)
{
    seedRandom();
    unsigned int number = LehmerRNG(seed);
    int randomNumber = (number % (max - min + 1)) + min;
    return randomNumber;
}