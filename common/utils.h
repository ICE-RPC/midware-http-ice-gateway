
#pragma once

#include <string>

static std::string RQSTID(int n=20) {
    //static char CHARS[] = "ABCDEFGHIJKLMNOPQRSTUVWXWZ1234567890";
    static char CHARS[] = "abcdefghijklmnopqrstuvwxwz1234567890";
    std::string uuid(n, '0');
    for(size_t i=0; i<uuid.size(); i++)
        uuid[i] = CHARS[rand()%(sizeof(CHARS)-1)];
    return uuid;
}

