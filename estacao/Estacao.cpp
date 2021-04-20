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