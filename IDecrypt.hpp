//
// Created by gaspar_q on 4/12/16.
//

#ifndef CPP_PLAZZA_IDECRYPT_HPP
#define CPP_PLAZZA_IDECRYPT_HPP

#include <string>
#include "Regex.hpp"

/**
 *             .:'
 *         __ :'__
 *      .'`  `-'  ``.
 *     :          .-'
 *     :         :
 *      :         `-;
 *       `.__.-.__.'
 */
class IDecrypt
{
public:
    virtual ~IDecrypt(){}

public:
    virtual bool            Detect(std::string const &toDecrypt, Regex const &regex) = 0;
    virtual std::string     Decrypt(std::string const &toDecrypt) const = 0;
};

#endif //CPP_PLAZZA_IDECRYPT_HPP
