#ifndef SIMULACAO_HPP
#define SIMULACAO_HPP

#include "../automata/Automata.hpp"
#include "../veiculo/Veiculo.hpp"
#include "../arquivo/Arquivo.hpp"
#include "../estacao/Estacao.hpp"
#include <ostream>

#define INVALID_INDEX -3
#define MAIN_LANE 1
#define STOP_LANE 0

enum Direcao
{
    left_to_rigth,
    rigth_to_left,
};

class Simulacao
{
private:
    Automata *estadoAtual = NULL;
    Automata *estadoAnterior = NULL;
    Veiculo **veiculos = NULL;
    Estacao **estacoes = NULL;
    Arquivo *file = NULL;
    int velocityMax;
    int qtdRoad;
    int sizeRoad;
    int qtdVeiculos;
    int totalEstacoes;

    int myRandom(int, int);
    double myRandom();
    void copyAtualToAnterior();
    Veiculo *getNextCar(Veiculo *) const;
    Veiculo *getPreviousCar(Veiculo *, int) const;
    int distanceNextCar(Veiculo *, int) const;
    int distancePreviousCar(Veiculo *, int) const;
    int distanceNextStation(Veiculo *car) const;
    int distanceNextObstacle(Veiculo *car) const;
    void runWithPrint(int qtdPassos, std::ostream *out);
    void runWithoutPrint(int qtdPassos);
    int getVeiculoSideRoad(int roadAtual, Direcao dir);
    bool regraModifacaoLR(Veiculo *veiculo);
    bool regraModifacaoRL(Veiculo *veiculo);
    bool regraModifacao(int veicId, Direcao dir);
    bool regraSegurancaLR(Veiculo *veiculo);
    bool regraSegurancaRL(Veiculo *veiculo);

    void checkQtdVeiculos() const;
    void printEstacoesPasso(std::ostream *out) const;

    int getNextStationID(Veiculo *veiculo) const;

public:
    Simulacao(int sizeRoad, int qtdRoads, int qtdVeiculos, int vMax, int sizeVeiculo, int qtdEstacoes = 0);
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