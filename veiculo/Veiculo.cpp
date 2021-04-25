#include "Veiculo.hpp"
#include <iostream>

Veiculo::Veiculo(int id, int vel, int tam, int qtdEstacoes, int *estacoesIds, int max) : ID(id), velocidade(vel), tamanho(tam), MaxTime(max)
{
    this->countTime = 0;
    this->stoped = false;
    this->willMove = true;
    if (qtdEstacoes > 0)
    {
        this->qtdEstacoes = qtdEstacoes;
        this->estacoesId = new int[qtdEstacoes];
        this->estacaoAtualId = 0;

        for (int i = 0; i < qtdEstacoes; i++)
        {
            this->estacoesId[i] = estacoesIds[i];
        }
    }
}

Veiculo::~Veiculo()
{
    if (this->estacoesId)
    {
        delete this->estacoesId;
    }
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
    *out << "Veiculo " << this->ID << "; V = " << this->velocidade << "; P(" << road << ", " << posRoad << "); Stop = " << stoped << "; count = " << countTime;

    if (qtdEstacoes > 0)
    {
        *out << ": stations " << qtdEstacoes << " [";
        for (size_t i = 0; i < qtdEstacoes; i++)
        {
            *out << this->estacoesId[i];
            if (i == qtdEstacoes - 1)
            {
                *out << "]";
            }
            else
            {
                *out << ", ";
            }
        }
    }

    *out << std::endl;
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

int Veiculo::getTamanho()
{
    return this->tamanho;
}

int Veiculo::getNextStationID()
{
    return estacoesId[estacaoAtualId];
}

void Veiculo::carStoped()
{
    this->stoped = true;
    this->velocidade = 0;
}

bool Veiculo::isCarStoped()
{
    return this->stoped;
}

void Veiculo::checkTime()
{
    this->countTime++;
    if (this->countTime > this->MaxTime)
    {
        this->stoped = false;
        this->willMove = true;
        this->countTime = 0;
    }
}

void Veiculo::setNextStationID()
{
    this->estacaoAtualId++;
    if (this->estacaoAtualId == this->qtdEstacoes)
    {
        this->estacaoAtualId = 0;
    }
}

void Veiculo::carWillStop()
{
    this->willMove = false;
}

bool Veiculo::isCarMove()
{
    return this->willMove;
}
