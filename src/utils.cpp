#include "all.h"

#ifdef VITASDK
#include <psp2/io/dirent.h> 
#endif

std::string TrimSpacesInString(std::string target) {
    while (!target.empty()) {
        if (target.at(0) == ' '){
            target = target.substr(1);
        } else if (target.at(target.size()-1) == ' '){
            target = target.substr(0,target.size()-1);
        } else {
            break;
        }
    }
    return target;
}

bool DirectoryExists(std::string path) {
#ifdef VITASDK
    SceUID returnCode = sceIoDopen(path.c_str());
    if(returnCode >= 0) {
        sceIoDclose(returnCode);
        return true;
    } else {
        return false;
    }
#else
    return fs::exists(path);
#endif

}

float XM1PW3P1(float a) {
    return (a - 1) * (a - 1) * (a - 1) + 1;
}

bool ParseUTF8(unsigned char ch, int* nextUTFBytes, uint32_t& out) {
    if ((ch >> 5) == 0b110) {
        *nextUTFBytes = 1;
        out = ((uint32_t)ch & 0b11111) << 6;
        return false;
    }
    else if ((ch >> 4) == 0b1110) {
        *nextUTFBytes = 2;
        out = ((uint32_t)ch & 0b1111) << 12;
        return false;
    }
    else if ((ch >> 3) == 0b11110) {
        *nextUTFBytes = 3;
        out = ((uint32_t)ch & 0b111) << 18;
        return false;
    }
    else if ((ch >> 2) == 0b111110) {
        *nextUTFBytes = 4;
        out = ((uint32_t)ch & 0b11) << 24;
        return false;
    }
    else if ((ch >> 1) == 0b1111110) {
        *nextUTFBytes = 5;
        out = ((uint32_t)ch & 0b1) << 30;
        return false;
    }
    else if ((ch >> 6) == 0b10) {
        if ((*nextUTFBytes)-- > 0) {
            out |= ((uint32_t)ch & 0b111111) << (*nextUTFBytes * 6);
            return *nextUTFBytes == 0;
        }
        else {
            //invalid utf8;
            out = 0;
            return true;
        }
    }
    else {
        *nextUTFBytes = 0;
        out = (uint32_t)ch;
        return true;
    }
        
}