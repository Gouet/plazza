//
// Created by gaspar_q on 4/14/16.
//

#include <algorithm>
#include <iostream>
#include "ResolvedData.hpp"

ResolvedData::ResolvedData()
{

}

ResolvedData::ResolvedData(const ResolvedData &data)
{
    *this = data;
}

ResolvedData::~ResolvedData()
{

}

ResolvedData &ResolvedData::operator=(const ResolvedData &data)
{
    datas = data.datas;
    return *this;
}

std::string         ResolvedData::serialize() const
{
    std::string     serial;

    serial += '[';
    for (std::vector<std::string>::const_iterator it = datas.begin(), end = datas.end(); it != end; ++it)
    {
        if (it != datas.begin())
            serial += ", ";
        serial += "\"" + escape(*it) + "\"";
    }
    serial += ']';
    return serial;
}

void            ResolvedData::unserialize(const std::string &string) throw(SyntaxErrorException)
{
    std::string unescaped;

    clear();
    for (std::string::const_iterator it = string.begin(), end = string.end(); it != end; ++it)
    {
        if (*it == '[')
        {
            while (*it != ']' && it != end)
            {
                if (*it == '\"')
                {
                    ++it;
                    unescaped.clear();
                    while (*it != '\"' && it != end)
                    {
                        if (*it == '\\')
                            ++it;
                        if (it != end)
                        {
                            unescaped += *it;
                            ++it;
                        }
                    }
                    if (it == end)
                        throw SyntaxErrorException("String not ended by \": " + string);
                    if (it != end)
                        datas.push_back(unescaped);
                }
                if (it == end)
                    throw SyntaxErrorException("Array not ended by ]: " + string);
                ++it;
            }
        }
    }
}

std::string     ResolvedData::escape(std::string const &string) const
{
    std::string escape;

    for (std::string::const_iterator it = string.begin(), end = string.end(); it != end; ++it)
    {
        if (*it == '"' || *it == '[' || *it == ']' || *it == '\\')
            escape += '\\';
        escape += *it;
    }
    return escape;
}

std::vector<std::string> const &ResolvedData::getDatas() const
{
    return datas;
}

void ResolvedData::addData(const std::string &string)
{
    datas.push_back(string);
}

void ResolvedData::clear()
{
    datas.clear();
}

std::string ResolvedData::toOutput() const
{
    std::string output;

    for (std::vector<std::string>::const_iterator it = datas.begin(), end = datas.end(); it != end; ++it)
    {
        if (!output.empty())
            output += '\n';
        output += *it;
    }
    return output;
}
