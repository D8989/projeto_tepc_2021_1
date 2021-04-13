#include "Veiculo.hpp"
#include <iostream>

Veiculo::Veiculo(int id, int vel) : ID(id), velocidade(vel)
{
}

Veiculo::~Veiculo()
{
}

unsigned int Veiculo::getId()
{
    return this->ID;
}

int Veiculo::getVelocidade()
{
    return this->velocidade;
}

void Veiculo::setVelocidade(int vel)
{
    this->velocidade = vel < 0 ? 0 : vel;
}

void Veiculo::print(std::ostream *out)
{
    *out << "Veiculo " << this->ID << "; V = " << this->velocidade << "; P(" << road << ", " << posRoad << ")" << std::endl;
}

int Veiculo::getRoad()
{
    return road;
}

int Veiculo::getPosRoad()
{
    return posRoad;
}

void Veiculo::setPos(int road, int posRoad)
{
    this->road = road;
    this->posRoad = posRoad;
}
