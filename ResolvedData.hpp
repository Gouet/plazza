//
// Created by gaspar_q on 4/14/16.
//

#ifndef CPP_PLAZZA_RESOLVEDDATA_HPP
#define CPP_PLAZZA_RESOLVEDDATA_HPP

#include <string>
#include <vector>
#include <stdexcept>

class ResolvedData
{
public:
    class SyntaxErrorException : public std::runtime_error
    {
    public:
        SyntaxErrorException(std::string const &err) throw() :
                std::runtime_error("Syntax Error: " + err)
        {

        }
        virtual ~SyntaxErrorException() throw()
        {

        }
    };

public:
    ResolvedData();
    ResolvedData(ResolvedData const &);
    ~ResolvedData();
    ResolvedData    &operator=(ResolvedData const &);

public:
    std::string                     serialize() const;
    void                            unserialize(std::string const &) throw(SyntaxErrorException);
    std::vector<std::string> const  &getDatas() const;
    void                            addData(std::string const &);
    void                            clear();
    std::string                     toOutput() const;

private:
    std::string                     escape(std::string const &) const;

private:
    std::vector<std::string>        datas;
};

#endif //CPP_PLAZZA_RESOLVEDDATA_HPP
