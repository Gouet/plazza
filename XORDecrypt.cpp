//
// XORDecrypt.cpp for DECRYPT in /home/gouet_v/Rendu/semester4/CPP/cpp_plazza/XOR
//
// Made by Victor Gouet
// Login   <gouet_v@epitech.net>
//
// Started on  Wed Apr 13 16:48:18 2016 Victor Gouet
// Last update Tue Apr 19 21:55:22 2016 Quentin Gasparotto
//

#include <string.h>
#include "XORDecrypt.hpp"

XORDecrypt::XORDecrypt()
{

}

XORDecrypt::~XORDecrypt()
{

}

bool                XORDecrypt::Detect(std::string const &toDecrypt, Regex const &regex)
{
    size_t          len = (toDecrypt.length() + 1) / 2;
    unsigned short  tab[len + 1];

    unsigned short  key = USHRT_MAX;
    bool            isPadded = (toDecrypt.length() % 2 != 0);

    bzero(tab, len * sizeof(unsigned short));
    memcpy(tab, toDecrypt.c_str(), toDecrypt.length());
    do
    {
        if (tryKey(tab, key, len, isPadded, toDecrypt.size(), regex))
            return true;
        --key;
    } while (key > 0);
    return tryKey(tab, key, len, isPadded, toDecrypt.size(), regex);
}

std::string XORDecrypt::Decrypt(std::string const &) const
{
    return decrypted;
}

bool XORDecrypt::tryKey(unsigned short *tab, unsigned short key, size_t len, bool isPadded, size_t strlength, Regex const &regex)
{
    unsigned short  tabDecrypted[len + 1];

    for (size_t i = 0; i < len; ++i)
    {
        tabDecrypted[i] = tab[i] ^ key;
    }
    tabDecrypted[len] = 0;
    if (isPadded)
        ((char *) tabDecrypted)[strlength] = 0;
    decrypted = (char *)&tabDecrypted[0];
    return regex.doesMatch(decrypted);
}
