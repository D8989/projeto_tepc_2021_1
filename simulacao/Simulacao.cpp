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

Veiculo *Simulacao::getPreviousCar(Veiculo *car, int road) const
{
    int x = road;
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

int Simulacao::distancePreviousCar(Veiculo *v, int road) const
{
    int x = road;
    int y = v->getPosRoad();
    bool nextCarFounded = false;
    int count = 0;

    while (!(count == this->velocityMax + 1) && !nextCarFounded)
    {
        y--;
        count++;
        if (y < 0)
        { // road circular
            y = sizeRoad - 1;
        }
        int cell = estadoAnterior->getCell(x, y);
        if (cell != EMPTY_CELL)
        {
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
            print(file->getStream());
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
        checkQtdVeiculos(); // Check para ver se algum veículo se perdeu
        printPasso(out);
        changeRoad();
        copyAtualToAnterior();
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

void Simulacao::print(std::ostream *out) const
{
    *out << "ATUAL:" << std::endl;
    estadoAtual->print(out);
    *out << std::endl;
    *out << "ANTERIOR:" << std::endl;
    estadoAnterior->print(out);
    *out << std::endl;

    *out << "VEICULOS:\n";
    for (size_t i = 0; i < qtdVeiculos; i++)
    {
        veiculos[i]->print(out);
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
                // *out << "[" << veiculos[veicId]->getId() << "]" << "(" << veiculos[veicId]->getVelocidade() << ")";
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

void Simulacao::changeRoad()
{
    estadoAtual->cleanAutomato();
    for (size_t i = 0; i < qtdVeiculos; i++)
    {
        if (regraModifacaoRL(veiculos[i]) && regraSegurancaRL(veiculos[i]))
        {
            veiculos[i]->setPos(veiculos[i]->getRoad() - 1, veiculos[i]->getPosRoad());
        }
        else if (regraModifacaoLR(veiculos[i]) && regraSegurancaLR(veiculos[i]))
        {
            veiculos[i]->setPos(veiculos[i]->getRoad() + 1, veiculos[i]->getPosRoad());
        }
        estadoAtual->setCell(veiculos[i]->getRoad(), veiculos[i]->getPosRoad(), veiculos[i]->getId());
    }
}

int Simulacao::getVeiculoSideRoad(int roadAtual, Direcao dir)
{
    int sideRoad;
    if (dir == Direcao::left_to_rigth)
    {
        sideRoad = roadAtual + 1;
        return sideRoad < qtdRoad ? sideRoad : OUT_BOUND;
    }
    else if (dir == Direcao::rigth_to_left)
    {
        sideRoad = roadAtual - 1;
        return sideRoad >= 0 ? sideRoad : OUT_BOUND;
    }
    else
    {
        std::cout << "ERROR::SIMULACAO::getVeiculoSideRoad::Direcao desconhecida" << std::endl;
        this->~Simulacao();
        exit(EXIT_FAILURE);
    }
}

bool Simulacao::regraModifacao(int veicId, Direcao dir)
{
    if (dir == Direcao::left_to_rigth)
    {
        return this->regraModifacaoLR(this->veiculos[veicId]);
    }
    else if (dir == Direcao::rigth_to_left)
    {
        return this->regraModifacaoRL(this->veiculos[veicId]);
    }
    else
    {
        std::cout << "ERROR::SIMULACAO::regraModifacao::Direcao desconhecida" << std::endl;
        this->~Simulacao();
        exit(EXIT_FAILURE);
    }
}

bool Simulacao::regraModifacaoLR(Veiculo *veiculo) // pista rapida para pista normal
{
    bool rulePrevious;
    int rigthRoad = this->getVeiculoSideRoad(veiculo->getRoad(), Direcao::left_to_rigth);
    if (rigthRoad == OUT_BOUND || estadoAnterior->getCell(rigthRoad, veiculo->getPosRoad()) != EMPTY_CELL)
    {
        return false;
    }

    Veiculo *previousCar = getPreviousCar(veiculo, veiculo->getRoad());
    if (!previousCar)
    {
        rulePrevious = false;
    }
    else
    {
        int distancePrevious = distanceNextCar(previousCar, previousCar->getRoad());
        rulePrevious = previousCar->getVelocidade() > veiculo->getVelocidade() && distancePrevious < previousCar->getVelocidade();
    }

    int distanceNext = distanceNextCar(veiculo, veiculo->getRoad());

    return rulePrevious || veiculo->getVelocidade() > distanceNext;
}

bool Simulacao::regraModifacaoRL(Veiculo *veiculo) // pista normal para pista rapida
{
    int leftRoad = this->getVeiculoSideRoad(veiculo->getRoad(), Direcao::rigth_to_left);
    if (leftRoad == OUT_BOUND || estadoAnterior->getCell(leftRoad, veiculo->getPosRoad()) != EMPTY_CELL)
    {
        return false;
    }

    int distanceRoadCar = distanceNextCar(veiculo, veiculo->getRoad());
    int distanceLeftRoadCar = distanceNextCar(veiculo, leftRoad);

    return veiculo->getVelocidade() > distanceRoadCar && veiculo->getVelocidade() < distanceLeftRoadCar;
}

bool Simulacao::regraSegurancaLR(Veiculo *veiculo)
{
    bool regraBackCar = false;
    int rigthRoad = this->getVeiculoSideRoad(veiculo->getRoad(), Direcao::left_to_rigth);
    if (rigthRoad == OUT_BOUND || estadoAnterior->getCell(rigthRoad, veiculo->getPosRoad()) != EMPTY_CELL)
    {
        return false;
    }

    Veiculo *previousCar = getPreviousCar(veiculo, rigthRoad);
    if (!previousCar)
    {
        regraBackCar = true;
    }
    else
    {
        int distBackCar = distancePreviousCar(veiculo, rigthRoad);
        regraBackCar = distBackCar > previousCar->getVelocidade();
    }

    int distNextCar = distanceNextCar(veiculo, rigthRoad);

    return regraBackCar && distNextCar > veiculo->getVelocidade();
}

bool Simulacao::regraSegurancaRL(Veiculo *veiculo)
{
    bool regraBackCar = false;
    int leftRoad = this->getVeiculoSideRoad(veiculo->getRoad(), Direcao::rigth_to_left);
    if (leftRoad == OUT_BOUND || estadoAnterior->getCell(leftRoad, veiculo->getPosRoad()) != EMPTY_CELL)
    {
        return false;
    }

    Veiculo *previousCar = getPreviousCar(veiculo, leftRoad);
    if (!previousCar)
    {
        regraBackCar = true;
    }
    else
    {
        int distBackCar = distancePreviousCar(veiculo, leftRoad);
        regraBackCar = distBackCar > previousCar->getVelocidade();
    }

    int distNextCar = distanceNextCar(veiculo, leftRoad);

    return regraBackCar && distNextCar > veiculo->getVelocidade();
}

void Simulacao::checkQtdVeiculos() const
{
    int count = 0;
    for (size_t i = 0; i < qtdRoad; i++)
    {
        for (size_t j = 0; j < sizeRoad; j++)
        {
            int cell = estadoAnterior->getCell(i, j);
            if (cell != EMPTY_CELL)
            {
                count++;
            }
        }
    }

    if (count != qtdVeiculos)
    {
        std::cout << "ERRO::SIMULACAL::checkQtdVeiculos::Algum veículo se perdeu";
        file->write("ERRO::SIMULACAL::checkQtdVeiculos::Algum veículo se perdeu");
        this->print(file->getStream());
        this->~Simulacao();
    }
}