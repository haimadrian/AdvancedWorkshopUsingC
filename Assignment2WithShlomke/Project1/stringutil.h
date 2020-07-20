//
// Created by Haim Adrian
//

#ifndef ASSIGNMENT2_STRINGUTIL_H
#define ASSIGNMENT2_STRINGUTIL_H

typedef char* string;

string readString();
void trim(string* str);
int startWith(string a, string declaration);
void delete(string* str, int count);

#endif //ASSIGNMENT2_STRINGUTIL_H
