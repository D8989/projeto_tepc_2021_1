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
    return x * qtdRoad + y;
}

void Automata::print()
{
    std::cout << "=> ";
    for (size_t i = 0; i < qtdRoad; i++)
    {
        for (size_t j = 0; j < sizeRoad; j++)
        {
            int cell = getCell(i, j);
            if (cell == EMPTY_CELL)
            {
                std::cout << "-";
            }
            else
            {
                std::cout << cell;
            }
        }
        std::cout << std::endl;
    }
}

int Automata::getCell(int i, int j)
{
    return this->automato[pos(i, j)];
}

void Automata::setCell(int i, int j, int carIndex)
{
    this->automato[pos(i, j)] = carIndex;
}
