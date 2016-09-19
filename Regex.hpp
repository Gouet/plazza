//
// Created by gaspar_q on 4/8/16.
//

#ifndef CPP_PLAZZA_REGEX_HPP
#define CPP_PLAZZA_REGEX_HPP

#include <string>
#include <regex.h>
#include <stdexcept>
#include <vector>

/**
 * \brief The regex class for an overload of POSIX regexes
 */
class Regex
{
public:
    class RegexError : public std::runtime_error
    {
    public:
        explicit RegexError(std::string const &);
        virtual ~RegexError() throw();
    };

public:
    static const size_t matches_nb;

public:
    Regex(const std::string &pat = "", const int regflag = REG_EXTENDED) throw(Regex::RegexError);
    Regex(Regex const &);
    Regex   &operator=(Regex const&);
    ~Regex();

public:
    void                        setPattern(std::string const &) throw(RegexError);
    std::vector<std::string>    search(std::string, int matchFlags = 0, size_t findEach = 1, size_t maxMatch = 0) const;
    bool                        doesMatch(std::string const &, int regFlags = 0) const;

private:
    const int                   regflag;
    std::string                 pattern;
    regex_t                     reg;
};

#endif //CPP_PLAZZA_REGEX_HPP
