#ifndef SIMULACAO_HPP
#define SIMULACAO_HPP

#include "../automata/Automata.hpp"
#include "../veiculo/Veiculo.hpp"
#include "../arquivo/Arquivo.hpp"
#include <ostream>

#define INVALID_INDEX -3

enum Direcao
{
    left_to_rigth,
    rigth_to_left,
};

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
    Veiculo *getPreviousCar(Veiculo *, int) const;
    int distanceNextCar(Veiculo *, int) const;
    int distancePreviousCar(Veiculo *, int) const;
    void runWithPrint(int qtdPassos, std::ostream *out);
    void runWithoutPrint(int qtdPassos);
    int getVeiculoSideRoad(int roadAtual, Direcao dir);
    bool regraModifacaoLR(Veiculo *veiculo);
    bool regraModifacaoRL(Veiculo *veiculo);
    bool regraModifacao(int veicId, Direcao dir);
    bool regraSegurancaLR(Veiculo *veiculo);
    bool regraSegurancaRL(Veiculo *veiculo);

    void checkQtdVeiculos() const;

public:
    Simulacao(int sizeRoad, int qtdRoads, int qtdVeiculos, int vMax, int sizeVeiculo);
    ~Simulacao();

    void passoVelocidade();
    void passoPosicao();
    void run(int qtdPassos);

    void print(std::ostream *out) const;
    void printPasso(std::ostream *out);
    void printDados(std::ostream *out) const;
    void setFile(Arquivo *file);

    void changeRoad();
};

#endif