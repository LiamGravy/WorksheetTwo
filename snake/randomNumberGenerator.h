#ifndef RANDOM_NUMBER_GENERATOR_H
#define RANDOM_NUMBER_GENERATOR_H

void seedRandom();
int LehmerRNG(unsigned int seed);
int getRandomNumber(int min, int max);

#endif