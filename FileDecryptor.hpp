//
// Created by gaspar_q on 4/12/16.
//

#ifndef CPP_PLAZZA_FILEDECRYPTOR_HPP
#define CPP_PLAZZA_FILEDECRYPTOR_HPP

#include <string>
#include <vector>
#include "IDecrypt.hpp"
#include "Thread.hpp"
#include "Regex.hpp"

class FileDecryptor
{
public:
    FileDecryptor(std::string const &filename = "");
    FileDecryptor(FileDecryptor const &ref);
    ~FileDecryptor();
    FileDecryptor   &operator=(FileDecryptor const &ref);

public:
    void                        tryDecryption(Regex const &regex);

public:
    void                        setFilename(std::string const &filename);
    std::string const           &getUncryptedFile() const;

private:
    std::string                 filename;
    std::vector<IDecrypt *>     decryptors;
    std::string                 uncryptedFile;
};

#endif //CPP_PLAZZA_FILEDECRYPTOR_HPP
