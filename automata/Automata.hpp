#ifndef AUTOMATA_HPP
#define AUTOMATA_HPP

#define OUT_BOUND -2
#define EMPTY_CELL -1

class Automata
{
private:
    int *automato;
    unsigned int sizeRoad; // Tamanho linear de uma pista
    unsigned int qtdRoad;  // quantidade de pistas
    double velocityMax;
    bool isClosedSystem;

    int pos(int, int);

public:
    Automata(unsigned int size, unsigned int qtd, double vMax, bool isClosed);
    ~Automata();

    void print();
    int getCell(int, int);
    void setCell(int, int, int);
};

#endif