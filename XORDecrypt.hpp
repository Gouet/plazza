//
// XORDecrypt.hpp for DECRYPT in /home/gouet_v/Rendu/semester4/CPP/cpp_plazza/XOR
// 
// Made by Victor Gouet
// Login   <gouet_v@epitech.net>
// 
// Started on  Wed Apr 13 16:47:41 2016 Victor Gouet
// Last update Wed Apr 13 17:24:52 2016 Victor Gouet
//

#ifndef XORDECRYPT_HPP_
# define XORDECRYPT_HPP_

# include <iostream>
# include <climits>
# include "Regex.hpp"
#include "IDecrypt.hpp"

class XORDecrypt : public IDecrypt
{
public:
    XORDecrypt();

    ~XORDecrypt();

public:
    bool            Detect(std::string const &toDecrypt, Regex const &regex);
    std::string     Decrypt(std::string const &toDecrypt) const;

private:
    bool    tryKey(unsigned short *tab, unsigned short key, size_t len, bool isPadded, size_t strlength, Regex const  &regex);

private:
    std::string     decrypted;
};

#endif
