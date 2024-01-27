#ifndef SERIALIZATION_H
#define SERIALIZATION_H

#include <vector>
#include <iostream>
#include <string>

using namespace std;

string* serialize(vector<string>&, unsigned int*);
void deserialize(vector<string>&, char*, int);

#endif