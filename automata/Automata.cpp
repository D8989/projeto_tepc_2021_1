#include "Automata.hpp"
#include <string.h>
#include <iostream>
#include <stdlib.h>

Automata::Automata(unsigned int size, unsigned int qtd) : sizeRoad(size),
                                                          qtdRoad(qtd)
{
    this->automato = new int[size * qtd];
    memset(automato, -0x1, size * qtd * sizeof(int));
}

Automata::~Automata()
{
    delete this->automato;
    std::cout << "VETOR AUTOMATO LIVRE\n";
}

int Automata::pos(int x, int y)
{
    if (x < 0 || x >= qtdRoad || y < 0 || y >= sizeRoad)
    {
        std::cout << "ERROR::OUTBOUND::Posicao (" << x << ", " << y << ") fora do automato" << std::endl;
        this->~Automata();
        exit(EXIT_FAILURE);
    }
    return x * sizeRoad + y;
}

void Automata::print(std::ostream *out)
{
    for (size_t i = 0; i < qtdRoad; i++)
    {
        *out << "faixa(" << i << ")\t";
        for (size_t j = 0; j < sizeRoad; j++)
        {
            int cell = getCell(i, j);
            switch (cell)
            {
            case EMPTY_CELL:
                *out << "-";
                break;

            case BODY_CAR:
                *out << "*";
                break;

            default:
                *out << cell;
                break;
            }
        }
        *out << std::endl;
    }
    *out << std::endl;
}

int Automata::getCell(int i, int j)
{
    return this->automato[pos(i, j)];
}

void Automata::setCar(int i, int j, Veiculo *carro)
{
    this->setValue(i, j, carro->getId());
    int carPartIndex = j;
    for (size_t k = 1; k < carro->getTamanho(); k++)
    {
        carPartIndex--;
        if (carPartIndex < 0)
        {
            carPartIndex = sizeRoad - 1;
        }
        this->setValue(i, carPartIndex, BODY_CAR);
    }
}

bool Automata::isCarFit(int i, int j, Veiculo *carro)
{
    int k = j;
    for (size_t count = 0; count < carro->getTamanho(); count++)
    {
        if (k < 0)
        {
            k = this->sizeRoad - 1;
        }
        if (this->getCell(i, k) != EMPTY_CELL)
        {
            return false;
        }
        k--;
    }
    return true;
}

bool Automata::isCellCar(int i, int j)
{
    int cell = this->getCell(i, j);
    return !(cell == EMPTY_CELL || cell == BODY_CAR);
}

bool Automata::isCellCar(int cell)
{
    return !(cell == EMPTY_CELL || cell == BODY_CAR);
}

bool Automata::isCellBodyCar(int i, int j)
{
    return this->getCell(i, j) == BODY_CAR;
}

bool Automata::isCellBodyCar(int cell)
{
    return cell == BODY_CAR;
}

void Automata::setValue(int i, int j, int value)
{
    this->automato[pos(i, j)] = value;
}

void Automata::cleanAutomato(int value)
{
    memset(automato, value, this->sizeRoad * this->qtdRoad * sizeof(int));
}
