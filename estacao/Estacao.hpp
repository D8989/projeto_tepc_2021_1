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
    bool hasCar;
    int countTime;
    const int MaxTime;

    void addTime();
    void resetTime();

public:
    Estacao(int id, int posRoad, int size, int laneSize, int maxTime);
    ~Estacao();

    int getId();
    int getTamanhoTotal();
    int getSize();
    int getStopLaneSize();
    int getPosRoad();
    int getBeginStation(int limite); // size + stop lane
    int getStartStation(int limite); // size
    void setCar();
    void removeCar();
    bool isCarInStation();
    bool isTimeOut();

    bool isCarInStopLineIn(int posCar, int limite);
    bool isCarInStopLineOut(int posCar, int limite);

    void print();
};

#endif