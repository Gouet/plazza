//
// Created by gaspar_q on 4/12/16.
//

#ifndef CPP_PLAZZA_CAESARDECRYPT_HPP
#define CPP_PLAZZA_CAESARDECRYPT_HPP

#include <string>
#include "IDecrypt.hpp"
#include "Regex.hpp"

class CaesarDecrypt : public IDecrypt
{
public:
    CaesarDecrypt();
    virtual ~CaesarDecrypt();

public:
    bool            Detect(std::string const &toDecrypt, Regex const &regex); //const;
    std::string     Decrypt(std::string const &toDecrypt = "") const;

private:
    void Rotate(int rot);

private:
    std::string m_toDecrypt;
    std::string m_decrypted;
};

#endif //CPP_PLAZZA_CAESARDECRYPT_HPP
