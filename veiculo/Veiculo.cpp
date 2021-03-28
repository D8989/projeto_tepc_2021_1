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
    this->velocidade = vel;
}

void Veiculo::print()
{
    std::cout << "Veiculo " << this->ID << "; V = " << this->velocidade << std::endl;
}
