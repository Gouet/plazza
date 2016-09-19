//
// Created by gaspar_q on 4/12/16.
//

#include <ostream>
#include <iostream>
#include <limits.h>
#include "CaesarDecrypt.hpp"
#include "Regex.hpp"

CaesarDecrypt::CaesarDecrypt() :
        m_decrypted("")
{

}

CaesarDecrypt::~CaesarDecrypt()
{

}

bool CaesarDecrypt::Detect(std::string const &toDecrypt, Regex const &regex) //const
{
    int rot = 256;
    m_toDecrypt = toDecrypt;

    do
    {
        --rot;
        m_toDecrypt = toDecrypt;
        Rotate(rot);
        if (regex.doesMatch(m_toDecrypt))
        {
            m_decrypted = m_toDecrypt;
            return true;
        }
    } while (rot > 0);

    return false;
}

std::string CaesarDecrypt::Decrypt(std::string const &) const
{
    return m_decrypted;
}

void CaesarDecrypt::Rotate(int rot)
{
    for (unsigned i = 0; i < m_toDecrypt.length(); i++)
    {
        m_toDecrypt[i] = (m_toDecrypt[i] + rot) % (UCHAR_MAX + 1);
    }
}
