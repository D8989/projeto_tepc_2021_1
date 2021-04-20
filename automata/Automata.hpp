#ifndef AUTOMATA_HPP
#define AUTOMATA_HPP

#include "../veiculo/Veiculo.hpp"
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
    void setCar(int, int, Veiculo *carro);
    void setValue(int, int, int);
    bool isCarFit(int, int, Veiculo *carro);
    bool isCellCar(int, int);     // coordenada da malha
    bool isCellCar(int);          // valor da celula
    bool isCellBodyCar(int, int); // coordenada da malha
    bool isCellBodyCar(int);      // valor da celula
    void cleanAutomato(int value);
};

#endif