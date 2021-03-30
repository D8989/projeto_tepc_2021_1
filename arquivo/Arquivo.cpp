#include "Arquivo.hpp"
#include <iostream>
#include <ctime>
#include <string>

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

std::string Arquivo::montarNome(const char **nomes, int size)
{
    time_t rawTime;
    struct tm *timeInfo;
    time(&rawTime);
    timeInfo = localtime(&rawTime);

    std::string nomeFinal;
    for (size_t i = 0; i < size; i++)
    {
        nomeFinal.append(nomes[i]);
        nomeFinal.append("_");
    }

    const char *nomesAuxiliares[] = {
        "DATA",
        std::to_string(timeInfo->tm_year + 1900).c_str(), // +1900 por causa do C (provavelmente)
        std::to_string(timeInfo->tm_mon + 1).c_str(),     // +1 pois Janeiro comeca com 0
        std::to_string(timeInfo->tm_mday).c_str(),
        std::to_string(timeInfo->tm_hour).c_str(),
        std::to_string(timeInfo->tm_min).c_str(),
        std::to_string(timeInfo->tm_sec).c_str(),
    };
    for (size_t i = 0; i < 7; i++)
    {
        nomeFinal.append(nomesAuxiliares[i]);
        if (i == 6)
        {
            nomeFinal.append(".txt");
        }
        else
        {
            nomeFinal.append("_");
        }
    }

    return nomeFinal;
}