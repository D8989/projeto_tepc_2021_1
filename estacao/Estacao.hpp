#ifndef ESTACAO_HPP
#define ESTACAO_HPP

#include "../veiculo/Veiculo.hpp"

class Estacao
{
private:
    const int ID;
    int size;
    int posRoad;
    int stopLaneSize; // aprox zone

    void addTime();
    void resetTime();

public:
    Estacao(int id, int posRoad, int size, int laneSize);
    ~Estacao();

    int getId();
    int getTamanhoTotal();
    int getSize();
    int getStopLaneSize();
    int getPosRoad();
    int getBeginStation(int limite); // size + stop lane
    int getStartStation(int limite); // size
    bool isTimeOut();

    bool isCarInStopLineIn(int posCar, int limite);
    bool isCarInStopLineOut(int posCar, int limite);
    bool isCarInStation(int posCar);

    void print();
};

#endif