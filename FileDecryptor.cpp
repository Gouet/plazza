//
// Created by gaspar_q on 4/12/16.
//

#include <fstream>
#include <sstream>
#include "FileDecryptor.hpp"
#include "CaesarDecrypt.hpp"
#include "Regex.hpp"
#include "NamedPipe.hpp"
#include "PlazzaCore.hpp"
#include "XORDecrypt.hpp"

/**
 * TODO
 * -    Ajouter class de decrypt XOR et NonCrypted
 * -    Terminer(et commencer) class Caesar
 * -    JSON Serializer modafuka
 * -    Regarder au niveau des paramètres de méthodes appelées par les thread pour remplacer "void *"
 */
FileDecryptor::FileDecryptor(std::string const &filename) :
    filename(filename)
{
    decryptors.push_back(new CaesarDecrypt());
    decryptors.push_back(new XORDecrypt());
//    decryptors.push_back(new NonCrypted);
}

FileDecryptor::FileDecryptor(FileDecryptor const &ref)
{
    *this = ref;
}

FileDecryptor::~FileDecryptor()
{
    for (std::vector<IDecrypt *>::iterator it = decryptors.begin(), end = decryptors.end(); it != end; ++it)
    {
        delete(*it);
    }
}

FileDecryptor &FileDecryptor::operator=(FileDecryptor const &ref)
{
    filename = ref.filename;
    return *this;
}

void                    FileDecryptor::tryDecryption(Regex const &regex)
{
    std::ifstream       fileToDecrypt(filename);
    std::stringstream   streamcontent;
    std::string         content;

    if (!fileToDecrypt.is_open())
        throw std::runtime_error("FileDecryptor: Cannot open " + filename);
    streamcontent << fileToDecrypt.rdbuf();
    fileToDecrypt.close();
    content = streamcontent.str();
    if (regex.doesMatch(content))
    {
        uncryptedFile = content;
        return;
    }
    for (std::vector<IDecrypt *>::iterator it = decryptors.begin(), end = decryptors.end(); it != end; ++it)
    {
        if ((*it)->Detect(content, regex))
        {
            uncryptedFile = (*it)->Decrypt(content);
            break;
        }
    }
}

void FileDecryptor::setFilename(std::string const &filename)
{
    this->filename = filename;
}

std::string const &FileDecryptor::getUncryptedFile() const
{
    return uncryptedFile;
}
