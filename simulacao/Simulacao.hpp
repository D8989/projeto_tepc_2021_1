#ifndef SIMULACAO_HPP
#define SIMULACAO_HPP

#include "../automata/Automata.hpp"
#include "../veiculo/Veiculo.hpp"
#include "../arquivo/Arquivo.hpp"
#include <ostream>

class Simulacao
{
private:
    Automata *estadoAtual;
    Automata *estadoAnterior;
    Veiculo **veiculos;
    Arquivo *file;
    int velocityMax;
    int qtdRoad;
    int sizeRoad;
    int qtdVeiculos;

    int myRandom(int, int);
    double myRandom();
    void copyAtualToAnterior();
    Veiculo *getNextCar(Veiculo *) const;
    int distanceNextCar(Veiculo *) const;
    void runWithPrint(int qtdPassos, std::ostream *out);
    void runWithoutPrint(int qtdPassos);

public:
    Simulacao(int sizeRoad, int qtdRoads, int qtdVeiculos, int vMax);
    ~Simulacao();

    void passoVelocidade();
    void passoPosicao();
    void run(int qtdPassos);

    void print();
    void printPasso(std::ostream *out);
    void setFile(Arquivo *file);
};

#endif