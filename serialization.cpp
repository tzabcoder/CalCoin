#include "serialization.h"

string* serialize(vector<string> &v, unsigned int *count) {
    string* buffer = new string[50];

    for (int i = 0; i < v.size(); i++) {
        buffer[i] = v[i];
    }

    return buffer;
}

void deserialize(vector<string> &restore,  char* buffer, int total_count) {
    for(int i = 0; i < total_count; i ++ ) {
        const char *begin = &buffer[i];
        int size = 0;
        while(buffer[i++]) {
            size += 1;
        }
        restore.push_back(string(begin, size));
    }
}