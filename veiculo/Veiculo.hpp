#ifndef VEICULO_HPP
#define VEICULO_HPP

#include <ostream>

#define BODY_CAR -4

class Veiculo
{
private:
    unsigned int ID;
    int velocidade;
    int road, posRoad; // posicao do ve'iculo na malha
    const int tamanho;
    int *estacoesId = NULL;
    int qtdEstacoes;
    int estacaoAtualId;

public:
    Veiculo(int id, int vel, int tam = 1, int qtdEstacoes = 0, int *estacoesIds = NULL);
    ~Veiculo();

    unsigned int getId();
    int getVelocidade();
    void setVelocidade(int);
    void print(std::ostream *out);
    int getRoad();
    int getPosRoad();
    int getTamanho();
    void setPos(int road, int posRoad);

    int getNextStationID();
};

#endif