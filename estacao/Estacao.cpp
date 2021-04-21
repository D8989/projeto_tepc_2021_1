#include <iostream>
#include "Estacao.hpp"

Estacao::Estacao(int id, int posRoad, int size, int laneSize, int maxTime) : ID(id), posRoad(posRoad), size(size), stopLaneSize(laneSize), MaxTime(maxTime)
{
    this->hasCar = false;
    this->countTime = 0;
}

Estacao::~Estacao()
{
}

int Estacao::getId()
{
    return this->ID;
}

int Estacao::getTamanhoTotal()
{
    return this->stopLaneSize * 2 + this->size;
}

int Estacao::getPosRoad()
{
    return this->posRoad;
}

int Estacao::getSize()
{
    return this->size;
}

int Estacao::getStopLaneSize()
{
    return this->stopLaneSize;
}

int Estacao::getBeginStation(int limite)
{
    int count = 0;
    int beginStation = posRoad;
    while (count < size + stopLaneSize)
    {
        beginStation--;
        if (beginStation < 0)
        {
            beginStation = limite - 1;
        }
        count++;
    }
    return beginStation;
}

int Estacao::getStartStation(int limite)
{
    int count = 0;
    int startStation = posRoad;
    while (count < size)
    {
        startStation--;
        if (startStation < 0)
        {
            startStation = limite - 1;
        }
        count++;
    }
    return startStation;
}

void Estacao::setCar()
{
    this->hasCar = true;
}

void Estacao::removeCar()
{
    this->hasCar = false;
}

bool Estacao::isCarInStation()
{
    return this->hasCar;
}

bool Estacao::isTimeOut()
{
    return this->countTime >= this->MaxTime;
}

void Estacao::addTime()
{
    this->countTime = this->countTime + 1;
}

void Estacao::resetTime()
{
    this->countTime = 0;
}

void Estacao::print()
{
    std::cout << "ID: " << ID << "PosRoad: " << posRoad << std::endl;
}

bool Estacao::isCarInStopLineIn(int posCar, int limite)
{
    return posCar > getBeginStation(limite) && posCar < getStartStation(limite);
}

bool Estacao::isCarInStopLineOut(int posCar, int limite)
{
    return posCar > this->posRoad && posCar < this->posRoad + this->stopLaneSize;
}
