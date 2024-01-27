/* -------------------------------------------------------------------------------------
CalCoin

FILENAME:    CVector.cpp 
COURSE:      Senior Project
DATE:        4-24-2022

DESCRIPTION:
    File includes a custom class to create a vector like data structure to include 
    abstract data types. This structure is used to hold node's wallet addresses, node's
    IP addresses, and node's stake amounts.
------------------------------------------------------------------------------------- */

#ifndef VECTOR_STRUCTURE_H
#define VECTOR_STRUCTURE_H

#include <vector>
#include <iostream>

using namespace std;

//Custom class for the Vector Structure
template <typename T> class Vector_Structure {
    T* arr;
    int capacity;
    int current;

    public:
        Vector_Structure() {
            arr = new T[1];
            capacity = 1;
            current = 0;
        }

        //Public Class Functions
        //Add data to structure
        void push(T data) {
            if (current == capacity) {
                T* temp = new T[2 * capacity];

                for (int i = 0; i < capacity; i++) {
                    temp[i] = arr[i];
                }

                delete[] arr;
                capacity *= 2;
                arr = temp;
            }

            arr[current] = data;
            current++;
        }

        //Add data to structure by index
        void push(T data, int index) {
            if (index == capacity) {
                push(data);
            }
            else {
                arr[index] = data;
            }
        }

        //Return the index of the structure
        T get(int index) {
            if (index < current) {
                return arr[index];
            }
        }

        //Remove the top of the structure
        void pop() {
            current--;
        }

        //Get the size of the vectore structure
        int size() {
            return current;
        }

        //Get the capacity of the vector structure
        int getcapacity(){
            return capacity;
        }
};

#endif