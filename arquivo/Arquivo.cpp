#include "Arquivo.hpp"
#include <iostream>

Arquivo::Arquivo()
{
}

Arquivo::~Arquivo()
{
    if (file.is_open())
    {
        file.close();
    }
    if (ofFile.is_open())
    {
        ofFile.close();
    }
}

void Arquivo::open(const char *pathFileName)
{
    ofFile.open(pathFileName);
    if (!ofFile.is_open())
    {
        std::cout << "ERROR::ARQUIVO_CPP::open::Arquivo \"" << pathFileName << "\" não foi aberto\n";
        exit(EXIT_FAILURE);
    }
}

void Arquivo::openBin(const char *pathFileName)
{
}

void Arquivo::write(const char *line)
{
    ofFile << line << std::endl;
}

void Arquivo::writeBin(char *pointer, int size)
{
}

std::ofstream *Arquivo::getStream()
{
    return &this->ofFile;
}