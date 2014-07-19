/* 
 * Universal string base encoding/decoding
 * use an alphabet and encode any block of memory
 * don't encode and decode on different computers
 *
 * writen by ImagicTheCat for Tsux
 * https://github.com/ImagicTheCat/Tsux
 */

#ifndef H_BASE
#define H_BASE

#include <cmath>
#include <iostream>
#include <algorithm>
#include <cstring>

typedef unsigned int BaseWord;

class Base{
  public:
    static std::string encode(const void* data,  int size, const std::string& alpha);
    static void decode(void* data,  int size, const std::string& str, const std::string& alpha);

  private:
    //alphabet functions (char <=> index)
    static int ctoi(char c, const std::string& alpha);
    static char itoc(int i, const std::string& alpha);

    //bit functions
    static std::string wtos(BaseWord word, const std::string& alpha);
    static BaseWord stow(const std::string& string, const std::string& alpha);

    static long unsigned int pow(long unsigned int base, long unsigned int power);

    Base();
};

#endif
