#ifndef AUTOMATA_HPP
#define AUTOMATA_HPP

#include <ostream>

#define OUT_BOUND -2
#define EMPTY_CELL -1

class Automata
{
private:
    int *automato;
    unsigned int sizeRoad; // Tamanho linear de uma pista
    unsigned int qtdRoad;  // quantidade de pistas

    int pos(int, int);

public:
    Automata(unsigned int size, unsigned int qtd);
    ~Automata();

    void print(std::ostream *out);
    int getCell(int, int);
    void setCell(int, int, int);
    void cleanAutomato();
};

#endif