//
// Created by gaspar_q on 4/8/16.
//

#include <string.h>
#include <regex.h>
#include <errno.h>
#include <iostream>
#include "Regex.hpp"

const size_t Regex::matches_nb = 42;

/**
 * \brief The constructor of the regex in which you can send the pattern and the flags. Throws RegexError
 *
 * \param string pattern of the regex. Default value is empty string
 * \param regflag flags of the regex. Default value is REG_EXTENDED
 */
Regex::Regex(const std::string &string, const int regflag) throw(Regex::RegexError) :
        regflag(regflag)
{
    setPattern(string);
}

/**
 * \brief Copy constructor of the regex.
 *
 * \param regex The regex to copy
 */
Regex::Regex(const Regex &regex) :
    regflag(regex.regflag)
{
    *this = regex;
}

/**
 * \brief Operator for copy of regexes
 *
 * \param regex The regex to copy
 * \return A reference to *this
 */
Regex &Regex::operator=(const Regex &regex)
{
    setPattern(regex.pattern);
    return *this;
}

/**
 * \brief The destructor of the regex that will free the regex_t
 */
Regex::~Regex()
{
    if (!pattern.empty())
        regfree(&reg);
}

/**
 * \brief Set the pattern of the regex, will also update the regex_t object. Throws RegexError if regcomp fails
 *
 * \param string The new pattern of the regex
 */
void Regex::setPattern(const std::string &string) throw(Regex::RegexError)
{
    if (!pattern.empty())
        regfree(&reg);
    pattern = string;
    if (regcomp(&reg, pattern.c_str(), regflag) != 0)
        throw Regex::RegexError(strerror(errno));
}

/**
 * \brief Method for searching strings that respect the pattern of the regex in a text
 *
 * \param text The text in which search
 * \param matchFlags The flags for matching result. Default value is 0
 * \param maxMatch Define the maximum number of match that will be returned, set to 0 if you don't want any limit
 * \return A vector
 */
std::vector<std::string>        Regex::search(std::string text, int matchFlags, size_t findEach, size_t maxMatch) const
{
    std::vector<std::string>    matches;
    size_t                      matchNb = (maxMatch == 0 ? Regex::matches_nb : maxMatch);
    regmatch_t                  matchTab[matchNb];
    size_t                      lastPos;
    size_t                      cursor = findEach;

    while (regexec(&reg, text.c_str(), matchNb, matchTab, matchFlags) != REG_NOMATCH)
    {
        lastPos = 0;
        for (size_t i = 0; i < matchNb; ++i)
        {
            if (matchTab[i].rm_so == -1 || matchTab[i].rm_eo == -1)
                break;
            if (cursor == findEach)
            {
                matches.push_back(text.substr(static_cast<size_t>(matchTab[i].rm_so), static_cast<size_t>(matchTab[i].rm_eo - matchTab[i].rm_so)));
                cursor = 1;
            }
            else
                ++cursor;
            if (static_cast<size_t>(matchTab[i].rm_eo) > lastPos)
                lastPos = static_cast<size_t>(matchTab[i].rm_eo);
        }
        if (maxMatch != 0)
            break;
        text = text.substr(lastPos, text.length() - lastPos);
    }
    return matches;
}

/**
 * \brief Check if the regex match on the text in parameters
 *
 * \param string The text in which try the match
 * \param regFlags The flags of the match
 * \return If the string match with the regex
 */
bool            Regex::doesMatch(const std::string &string, int regFlags) const
{
    regmatch_t  match;

    return regexec(&reg, string.c_str(), 1, &match, regFlags) != REG_NOMATCH;
}

/**
 * \brief Regex Exception constructor that call std::runtime_error constructors
 */
Regex::RegexError::RegexError(const std::string &string) :
        std::runtime_error("Regex: " + string)
{

}

/**
 * \brief Regex Exception destructor that does absolutely nothing
 */
Regex::RegexError::~RegexError() throw()
{

}