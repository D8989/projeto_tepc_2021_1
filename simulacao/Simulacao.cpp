#include "Simulacao.hpp"
#include <stdlib.h>
// #include <cstdlib>
#include <ctime>
#include <iostream>

Simulacao::Simulacao(int sizeRoad, int qtdRoads, int qtdVeiculos, int vMax) : qtdRoad(qtdRoads), sizeRoad(sizeRoad), qtdVeiculos(qtdVeiculos), velocityMax(vMax)
{
    this->estadoAtual = new Automata(sizeRoad, qtdRoads);
    this->veiculos = (Veiculo **)malloc(sizeof(Veiculo *) * qtdVeiculos);
    for (size_t i = 0; i < qtdVeiculos; i++)
    {
        int velRand = myRandom(1, vMax + 1);
        this->veiculos[i] = new Veiculo(i, velRand);

        bool veicSet = false;
        while (!veicSet)
        {
            int randomRoad = myRandom(0, qtdRoads);
            int posRoad = myRandom(0, sizeRoad);
            if (estadoAtual->getCell(randomRoad, posRoad) == EMPTY_CELL)
            {
                estadoAtual->setCell(randomRoad, posRoad, veiculos[i]->getId());
                veicSet = true;
            }
        }
    }

    this->estadoAnterior = new Automata(sizeRoad, qtdRoads);
    copyAtualToAnterior();
}

Simulacao::~Simulacao()
{
    delete estadoAtual;
    delete estadoAnterior;

    for (size_t i = 0; i < qtdVeiculos; i++)
    {
        delete this->veiculos[i];
        std::cout << "VEICULO " << i << " FREE\n";
    }

    free(this->veiculos);
    std::cout << "VETOR VEICULOS FREE\n";
}

int Simulacao::myRandom(int min, int max)
{
    srand(time(NULL)); // Seed the time
    return rand() % (max - min) + min;
}

void Simulacao::copyAtualToAnterior()
{
    for (size_t i = 0; i < qtdRoad; i++)
    {
        for (size_t j = 0; j < sizeRoad; j++)
        {
            estadoAnterior->setCell(i, j, estadoAtual->getCell(i, j));
        }
    }
}

void Simulacao::print()
{
    std::cout << "ATUAL:";
    estadoAtual->print();
    std::cout << std::endl;
    std::cout << "ANTERIOR:";
    estadoAnterior->print();
    std::cout << std::endl;

    std::cout << "VEICULOS:\n";
    for (size_t i = 0; i < qtdVeiculos; i++)
    {
        veiculos[i]->print();
    }
}

void Simulacao::printPasso()
{
    std::cout << "\t";
    for (size_t i = 0; i < qtdRoad; i++)
    {
        for (size_t j = 0; j < sizeRoad; j++)
        {
            int veicId = estadoAtual->getCell(i, j);
            if (veicId == EMPTY_CELL)
            {
                std::cout << "-";
            }
            else
            {
                std::cout << veiculos[veicId]->getVelocidade();
            }
        }
        std::cout << std::endl;
    }
}
