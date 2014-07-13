/* URI encode/decode functions wrapper
 * source: http://www.codeguru.com/cpp/cpp/algorithms/strings/article.php/c12759/URI-Encoding-and-Decoding.htm
 *
 * Uri encode and decode.
 * RFC1630, RFC1738, RFC2396
 */


#ifndef H_URI
#define H_URI

#include <iostream>

class URI{
  public:
    static std::string encode(const std::string& str);
    static std::string decode(const std::string& str);

    static const char HEX2DEC[256];
    static const char SAFE[256];

  private:
    URI();
};

#endif
