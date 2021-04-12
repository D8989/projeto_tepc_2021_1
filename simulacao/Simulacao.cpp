#include "Simulacao.hpp"
#include <stdlib.h>
#include <iostream>

Simulacao::Simulacao(int sizeRoad, int qtdRoads, int qtdVeiculos, int vMax) : qtdRoad(qtdRoads), sizeRoad(sizeRoad), qtdVeiculos(qtdVeiculos), velocityMax(vMax)
{
    this->file = NULL;
    this->estadoAtual = new Automata(sizeRoad, qtdRoads);
    this->veiculos = (Veiculo **)malloc(sizeof(Veiculo *) * qtdVeiculos);
    for (size_t i = 0; i < qtdVeiculos; i++)
    {
        int velRand = myRandom(1, vMax + 1);
        this->veiculos[i] = new Veiculo(i, velRand);

        bool veicSet = false;
        while (!veicSet)
        {
            int road = qtdRoads - 1;
            int posRoad = myRandom(0, sizeRoad);
            if (estadoAtual->getCell(road, posRoad) == EMPTY_CELL)
            {
                estadoAtual->setCell(road, posRoad, veiculos[i]->getId());
                veiculos[i]->setPos(road, posRoad);
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
    }

    free(this->veiculos);
}

/**
 * O valor retornado é entre [min, max)
**/
int Simulacao::myRandom(int min, int max)
{
    return rand() % (max - min) + min;
}

double Simulacao::myRandom()
{
    return ((double)rand() / (RAND_MAX));
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

Veiculo *Simulacao::getNextCar(Veiculo *v) const
{
    int x = v->getRoad();
    int y = v->getPosRoad();
    bool foundNextCar = false;
    int count = 0;
    int nextCarId = 0;

    while (!foundNextCar)
    {
        y++;
        count++;
        if (y == sizeRoad)
        { // road circular
            y = 0;
        }
        int veiculoIndex = estadoAnterior->getCell(x, y);
        if (veiculoIndex != EMPTY_CELL)
        {
            if (v->getId() == veiculos[veiculoIndex]->getId())
            {
                std::cout << "ERROR::SIMULACAO::NEXTCAR::ACHOU O MESMO CARRO\n";
                this->~Simulacao();
                exit(EXIT_FAILURE);
            }
            nextCarId = veiculoIndex;
            foundNextCar = true;
        }
        if (count > sizeRoad)
        {
            std::cout << "ERROR::SIMULACAO::NEXTCAR::NAO_ACHOU NENHUM CARRO\n";
            this->~Simulacao();
            exit(EXIT_FAILURE);
        }
    }
    return veiculos[nextCarId];
}

Veiculo *Simulacao::getPreviousCar(Veiculo *car) const
{
    int x = car->getRoad();
    int y = car->getPosRoad();
    bool foundPreviousCar = false;
    int count = 0;
    int previousCarId = INVALID_INDEX;

    while (!(count == this->velocityMax + 1) && !foundPreviousCar)
    {
        y--;
        count++;
        if (y < 0)
        { // road circular
            y = sizeRoad - 1;
        }

        int veiculoIndex = estadoAnterior->getCell(x, y);
        if (veiculoIndex != EMPTY_CELL)
        {
            previousCarId = veiculoIndex;
            foundPreviousCar = true;
        }
    }
    return previousCarId == INVALID_INDEX ? nullptr : veiculos[previousCarId];
}

int Simulacao::distanceNextCar(Veiculo *v, int road) const
{
    int x = road;
    int y = v->getPosRoad();
    bool nextCarFounded = false;
    int count = 0;

    while (!(count == this->velocityMax + 1) && !nextCarFounded)
    {
        y++;
        count++;
        if (y == sizeRoad)
        { // road circular
            y = 0;
        }
        int cell = estadoAnterior->getCell(x, y);
        if (cell != EMPTY_CELL)
        {
            if (count == sizeRoad)
            {
                std::cout << "ERROR::SIMULACAO::distanceNextCar::ENCONTROU ELE MESMO\n";
                this->~Simulacao();
                exit(EXIT_FAILURE);
            }
            nextCarFounded = true;
        }
        if (count >= sizeRoad)
        {
            std::cout << "ERROR::SIMULACAO::distanceNextCar::NAO_ACHOU NENHUM CARRO\n";
            this->~Simulacao();
            exit(EXIT_FAILURE);
        }
    }

    return count;
}

void Simulacao::passoVelocidade()
{
    for (size_t i = 0; i < qtdVeiculos; i++)
    {
        int carVelocidade = veiculos[i]->getVelocidade();
        int distNextCar = distanceNextCar(veiculos[i], veiculos[i]->getRoad());
        int novaVelocidade = carVelocidade;

        if (carVelocidade < velocityMax && distNextCar > carVelocidade + 1)
        {
            novaVelocidade = carVelocidade + 1;
        }
        else if (carVelocidade >= distNextCar)
        {
            novaVelocidade = distNextCar - 1;
        }

        if (myRandom() < 0.25)
        {
            if (novaVelocidade > 0)
            {
                novaVelocidade--;
            }
        }

        veiculos[i]->setVelocidade(novaVelocidade);
    }
}

void Simulacao::passoPosicao()
{
    estadoAtual->cleanAutomato();
    for (size_t i = 0; i < qtdVeiculos; i++)
    {
        int road = veiculos[i]->getRoad();
        int posRoad = veiculos[i]->getPosRoad();
        int velocidade = veiculos[i]->getVelocidade();

        int novaPosRoad = 0;
        if (posRoad + velocidade >= sizeRoad)
        {
            novaPosRoad = velocidade - (sizeRoad - posRoad);
        }
        else
        {
            novaPosRoad = posRoad + velocidade;
        }

        if (estadoAtual->getCell(road, novaPosRoad) != EMPTY_CELL)
        {
            std::cout << "ERROR::SIMULACAO::passoPosicao::Celula (" << road << ", " << novaPosRoad << ") está ocupada com o carro " << estadoAtual->getCell(road, novaPosRoad) << "; não está livre para colocar o carro " << veiculos[i]->getId() << std::endl;
            this->~Simulacao();
            exit(EXIT_FAILURE);
        }
        estadoAtual->setCell(road, novaPosRoad, veiculos[i]->getId());
        veiculos[i]->setPos(road, novaPosRoad);
    }
}

void Simulacao::run(int qtdPassos)
{
    if (file == NULL)
    {
        runWithPrint(qtdPassos, &std::cout);
    }
    else
    {
        runWithPrint(qtdPassos, file->getStream());
    }
}

void Simulacao::runWithPrint(int qtdPassos, std::ostream *out)
{
    for (size_t i = 0; i < qtdPassos; i++)
    {
        printPasso(out);
        passoVelocidade();
        passoPosicao();
        copyAtualToAnterior();
    }
}

void Simulacao::runWithoutPrint(int qtdPassos)
{
    for (size_t i = 0; i < qtdPassos; i++)
    {
        passoVelocidade();
        passoPosicao();
        copyAtualToAnterior();
    }
}

void Simulacao::print()
{
    std::cout << "ATUAL:" << std::endl;
    estadoAtual->print();
    std::cout << std::endl;
    std::cout << "ANTERIOR:" << std::endl;
    estadoAnterior->print();
    std::cout << std::endl;

    std::cout << "VEICULOS:\n";
    for (size_t i = 0; i < qtdVeiculos; i++)
    {
        veiculos[i]->print();
    }
}

void Simulacao::printPasso(std::ostream *out)
{
    for (size_t i = 0; i < qtdRoad; i++)
    {
        *out << "[" << i << "]\t";
        for (size_t j = 0; j < sizeRoad; j++)
        {
            int veicId = estadoAtual->getCell(i, j);
            if (veicId == EMPTY_CELL)
            {
                *out << "-";
            }
            else
            {
                *out << veiculos[veicId]->getVelocidade();
            }
        }
        *out << std::endl;
    }
    *out << std::endl;
}

void Simulacao::setFile(Arquivo *file)
{
    this->file = file;
}
