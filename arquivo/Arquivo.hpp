#ifndef ARQUIVO_HPP
#define ARQUIVO_HPP

#include <fstream>

class Arquivo
{
private:
    std::fstream file;
    std::ofstream ofFile;

public:
    Arquivo();
    ~Arquivo();

    void open(const char *pathFileName);
    void openBin(const char *pathFileName);
    void write(const char *line);
    void writeBin(char *, int);
    std::ofstream *getStream();
    std::string montarNome(const char **nomes, int size);
};

#endif