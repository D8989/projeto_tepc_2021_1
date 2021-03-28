#ifndef VEICULO_HPP
#define VEICULO_HPP

class Veiculo
{
private:
    unsigned int ID;
    int velocidade;
    int road, posRoad; // posicao do ve'iculo na malha

public:
    Veiculo(int, int);
    ~Veiculo();

    unsigned int getId();
    int getVelocidade();
    void setVelocidade(int);
    void print();
    int getRoad();
    int getPosRoad();
    void setPos(int road, int posRoad);
};

#endif