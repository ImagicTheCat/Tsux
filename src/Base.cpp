/* 
 * Universal string base encoding/decoding
 * use an alphabet and encode any block of memory
 * don't encode and decode on different computers
 *
 * writen by ImagicTheCat for Tsux
 * https://github.com/ImagicTheCat/Tsux
 */


#include "Base.hpp"

long unsigned int Base::pow(long unsigned int base, long unsigned int power){
  long unsigned int r = 1;
  for(long unsigned int i = 0; i < power; i++)
    r *= base;

  return r;
}

char Base::itoc(int i, const std::string& a){
  if(i >= 0 && i < a.size())
    return a[i];
  else
    return '\0';
}

int Base::ctoi(char c, const std::string& a){
  for(int i = 0; i < a.size(); i++){
    if(c == a[i])
      return i;
  }

  return -1;
}

std::string Base::wtos(BaseWord word, const std::string& a){
  std::string r;
  BaseWord wsize = sizeof(BaseWord);
  BaseWord base = a.size();

  if(base > 1){
    //compute amount of minimum bits per word
    BaseWord min_bits = ceil(log(pow(2,wsize*8))/log(base));
    BaseWord w = 0;

    //compute word into base
    BaseWord res = 0; 
    BaseWord div = word;
    for(BaseWord i = 0; i < min_bits; i++){
      res = div%base;
      div = div/base;
      r += itoc(res, a);
      w += pow(base, i)*res;
    }

    std::reverse(r.begin(), r.end());
  }

  return r;
}

BaseWord Base::stow(const std::string& str, const std::string& a){
  BaseWord r = 0;
  int wsize = sizeof(BaseWord);
  BaseWord base = a.size();

  if(base > 1){
    for(int i = 0; i < str.size(); i++){
      int p = ctoi(str[i], a);
      if(p != -1)
        r += pow(base, str.size()-i-1)*p;
    }
  }

  return r;
}

std::string Base::encode(const void* data, int size, const std::string& a){
  std::string r;
  int wsize = sizeof(BaseWord);
  int words = ceil(size/(float)wsize);
  int done = 0;

  for(int i = 0; i < words; i++){
    BaseWord word = 0;
    //fill the word with data
    memcpy(&word, (char*)data+wsize*i, std::min(size-done,wsize));
    
    r += wtos(word, a);

    done += wsize;
  }

  //cut first 0
  char z = itoc(0, a);
  int i = 0;
  bool ignore = true;
  while(i < r.size() && ignore){
    ignore = (r[i] == z);
    if(!ignore)
      r = r.substr(i);
    i++;
  }

  if(ignore)
    r = z;


  return r;
}

void Base::decode(void* data, int size, const std::string& str, const std::string& a){
  int wsize = sizeof(BaseWord);
  int base = a.size();
  int done = 0;
  if(base > 1){
    int min_bits = ceil(log(pow(2,wsize*8))/log(base));
    int words = ceil(str.size()/(float)min_bits);

    int diff = min_bits*words-str.size();

    int shift = 0;

    for(int i = 0; i < words; i++){
      std::string sword;

      //load string word
      for(int j = 0; j < min_bits; j++){
        //add zero to format data
        if(diff > 0){
          sword += a[0];
          diff--;
        }
        else{
          sword += str[shift];
          shift++;
        }
      }

      //load memory word
      BaseWord word = stow(sword, a);

      //write at the right position
      memcpy((char*)data+wsize*i, &word, std::min(size-done,wsize));

      done += wsize;
    }
  }
}

