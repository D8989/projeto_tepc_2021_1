#ifndef SIMULACAO_HPP
#define SIMULACAO_HPP

#include "../automata/Automata.hpp"
#include "../veiculo/Veiculo.hpp"

class Simulacao
{
private:
    Automata *estadoAtual;
    Automata *estadoAnterior;
    Veiculo **veiculos;
    int velocityMax;
    int qtdRoad;
    int sizeRoad;
    int qtdVeiculos;

    int myRandom(int, int);
    void copyAtualToAnterior();

public:
    Simulacao(int sizeRoad, int qtdRoads, int qtdVeiculos, int vMax);
    ~Simulacao();

    void print();
    void printPasso();
};

#endif