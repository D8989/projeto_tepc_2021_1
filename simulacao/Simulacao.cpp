#include "Simulacao.hpp"
#include <stdlib.h>
#include <iostream>
#include <cmath>

Simulacao::Simulacao(int sizeRoad, int qtdRoads, int qtdVeiculos, int vMax, int sizeVeiculo, int qtdEstacoes) : qtdRoad(qtdRoads), sizeRoad(sizeRoad), qtdVeiculos(qtdVeiculos), velocityMax(vMax)
{
    this->file = NULL;
    this->estadoAtual = new Automata(sizeRoad, qtdRoads);

    this->totalEstacoes = qtdEstacoes;
    int *estacoesIDs = NULL;
    if (qtdEstacoes > 0)
    {
        this->estadoAtual->cleanAutomato(STOP_LANE, OUT_BOUND);
        estacoesIDs = new int[qtdEstacoes];
        int sizeEstacao = sizeVeiculo;
        int laneSize = sizeVeiculo;
        // TODO (daniel) Melhorar essa conta
        int sizeTotalEstacao = sizeEstacao + laneSize * 2;
        int espaco = (int)floor((double)sizeRoad / qtdEstacoes);
        if (espaco <= sizeTotalEstacao)
        {
            std::cerr << "ERROR::SIMULACAO::Simulacao::A pista é pequena para essa quantidade de estação\n";
            exit(EXIT_FAILURE);
        }

        this->estacoes = new Estacao *[qtdEstacoes];
        for (int i = 0; i < qtdEstacoes; i++)
        {
            int posRoadEstacao = (i * espaco) + laneSize + sizeEstacao;
            std::cout << "ESTACAO " << i << " está em " << posRoadEstacao << std::endl;
            this->estacoes[i] = new Estacao(i, posRoadEstacao, sizeEstacao, laneSize);

            int count = 0;
            int stationBody = this->estacoes[i]->getBeginStation(sizeRoad);

            while (count < this->estacoes[i]->getTamanhoTotal())
            {
                this->estadoAtual->setValue(STOP_LANE, stationBody, EMPTY_CELL);
                stationBody++;
                if (stationBody >= sizeRoad)
                {
                    stationBody = 0;
                }
                count++;
            }
        }
    }

    this->veiculos = (Veiculo **)malloc(sizeof(Veiculo *) * qtdVeiculos);
    for (size_t i = 0; i < qtdVeiculos; i++)
    {
        int velRand = myRandom(1, vMax + 1);
        int qtdEstAux = 0;
        if (estacoesIDs != NULL)
        {
            if (i % 2 == 0)
            {
                for (int j = 0; j < qtdEstacoes; j++)
                {
                    qtdEstAux++;
                    estacoesIDs[j] = j;
                }
            }
            else
            {
                int e = 0;
                int j = 0;
                while (e < qtdEstacoes)
                {
                    if (e % 2 != 0)
                    {
                        qtdEstAux++;
                        estacoesIDs[j] = e;
                        j++;
                    }
                    e++;
                }
            }
            this->veiculos[i] = new Veiculo(i, velRand, sizeVeiculo, qtdEstAux, estacoesIDs, 5);
        }
        else
        {
            this->veiculos[i] = new Veiculo(i, velRand, sizeVeiculo);
        }

        bool veicSet = false;
        while (!veicSet)
        {
            int road = qtdRoads - 1;
            int posRoad = myRandom(0, sizeRoad);
            if (estadoAtual->isCarFit(road, posRoad, veiculos[i]))
            {
                estadoAtual->setCar(road, posRoad, veiculos[i]);
                veiculos[i]->setPos(road, posRoad);
                veicSet = true;
            }
        }
    }

    if (estacoesIDs != NULL)
    {
        delete estacoesIDs;
    }

    this->estadoAnterior = new Automata(sizeRoad, qtdRoads);
    copyAtualToAnterior();
}

Simulacao::~Simulacao()
{
    delete estadoAtual;
    delete estadoAnterior;

    if (this->estacoes != NULL)
    {
        for (size_t i = 0; i < totalEstacoes; i++)
        {
            delete this->estacoes[i];
        }
        delete this->estacoes;
    }

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
            estadoAnterior->setValue(i, j, estadoAtual->getCell(i, j));
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
        if (estadoAnterior->isCellCar(veiculoIndex))
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

    while (!(count == this->velocityMax) && !foundPreviousCar)
    {
        y--;
        count++;
        if (y < 0)
        { // road circular
            y = sizeRoad - 1;
        }

        int veiculoIndex = estadoAnterior->getCell(x, y);
        if (estadoAnterior->isCellCar(veiculoIndex))
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

    while (!(count == this->velocityMax) && !nextCarFounded)
    {
        y++;
        count++;
        if (y == sizeRoad)
        { // road circular
            y = 0;
        }
        if (estadoAnterior->isCellCar(x, y))
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

    // Caso do count parar "no meio" do veículo
    if (estadoAnterior->isCellBodyCar(x, y))
    {
        while (estadoAnterior->isCellBodyCar(x, y))
        {
            y++;
            if (y == sizeRoad)
            { // road circular
                y = 0;
            }
            count++;
        }
        nextCarFounded = true;
    }

    return nextCarFounded ? count - v->getTamanho() : count;
}

int Simulacao::distanceNextStation(Veiculo *car) const
{
    if (car->getRoad() == STOP_LANE)
    {
        return this->velocityMax;
    }
    int eId = car->getNextStationID();
    int stationGhostWall = estacoes[eId]->getStartStation(sizeRoad);
    int y = car->getPosRoad();
    int count = 0;
    bool stationFounded = false;
    while (!(count == this->velocityMax) && !stationFounded)
    {
        y++;
        count++;
        if (y == sizeRoad)
        {
            y = 0;
        }

        if (y == stationGhostWall)
        {
            stationFounded = true;
        }
    }
    return count - 1;
}

int Simulacao::distanceNextObstacle(Veiculo *car) const
{
    int eId = car->getNextStationID();
    int stationGhostWall = estacoes[eId]->getStartStation(sizeRoad);
    int stationDoor = estacoes[eId]->getPosRoad();
    int x = car->getRoad();
    int y = car->getPosRoad();
    int count = 0;
    bool obstacleFound = false;
    while (!(count == this->velocityMax) && !obstacleFound)
    {
        y++;
        count++;
        if (y == sizeRoad)
        {
            y = 0;
        }
        if (estadoAnterior->isCellCar(x, y) || estadoAnterior->isCellOutBound(x, y) || (y == stationGhostWall && x == MAIN_LANE) || (y == stationDoor && x == STOP_LANE && !car->isCarStoped()))
        {
            obstacleFound = true;
        }
    }

    // Caso do count parar "no meio" do veículo
    if (estadoAnterior->isCellBodyCar(x, y))
    {
        while (estadoAnterior->isCellBodyCar(x, y))
        {
            y++;
            if (y == sizeRoad)
            { // road circular
                y = 0;
            }
            count++;
        }
        obstacleFound = true;
    }

    if (obstacleFound)
    {
        return estadoAnterior->isCellCar(x, y) ? count - car->getTamanho() : count - 1;
    }
    return count;
}

int Simulacao::distancePreviousCar(Veiculo *v, int road) const
{
    int x = road;
    int y = v->getPosRoad();
    bool nextCarFounded = false;
    int count = 0;

    while (!(count == this->velocityMax) && !nextCarFounded)
    {
        y--;
        count++;
        if (y < 0)
        { // road circular
            y = sizeRoad - 1;
        }
        if (estadoAnterior->isCellCar(x, y))
        {
            nextCarFounded = true;
        }
        if (count >= sizeRoad)
        {
            std::cout << "ERROR::SIMULACAO::distancePreviousCar::NAO_ACHOU NENHUM CARRO\n";
            this->~Simulacao();
            exit(EXIT_FAILURE);
        }
    }

    return nextCarFounded ? count - v->getTamanho() : count;
}

void Simulacao::passoVelocidade()
{
    for (size_t i = 0; i < qtdVeiculos; i++)
    {
        if (veiculos[i]->isCarStoped())
        {
            veiculos[i]->checkTime();
        }
        else
        {
            int carVelocidade = veiculos[i]->getVelocidade();
            int distance = distanceNextObstacle(veiculos[i]);
            int novaVelocidade = carVelocidade;

            if (carVelocidade < velocityMax && distance >= carVelocidade + 1)
            {
                novaVelocidade = carVelocidade + 1;
            }
            else if (carVelocidade > distance)
            {
                novaVelocidade = distance;
            }

            if (veiculos[i]->getRoad() == MAIN_LANE && myRandom() < 0.25)
            {
                if (novaVelocidade > 0)
                {
                    novaVelocidade--;
                }
            }

            veiculos[i]->setVelocidade(novaVelocidade);
        }
    }
}

void Simulacao::passoPosicao()
{
    estadoAtual->cleanCars();
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

        if (!estadoAtual->isCarFit(road, novaPosRoad, veiculos[i]))
        {
            std::cout << "ERROR::SIMULACAO::passoPosicao::Celula (" << road << ", " << novaPosRoad << ") está ocupada com o carro " << estadoAtual->getCell(road, novaPosRoad) << "; não está livre para colocar o carro " << veiculos[i]->getId() << std::endl;
            print(file->getStream());
            this->~Simulacao();
            exit(EXIT_FAILURE);
        }
        estadoAtual->setCar(road, novaPosRoad, veiculos[i]);
        veiculos[i]->setPos(road, novaPosRoad);
        if (estacoes[veiculos[i]->getNextStationID()]->isCarInStation(veiculos[i]->getPosRoad()))
        {
            veiculos[i]->carStoped();
        }
    }
}

void Simulacao::run(int qtdPassos)
{
    if (file == NULL)
    {
        runWithoutPrint(qtdPassos);
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
        changeRoad();
        copyAtualToAnterior();
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
    printEstacoesPasso(out);
    for (size_t i = 0; i < qtdRoad; i++)
    {
        *out << "[" << i << "]\t";
        for (size_t j = 0; j < sizeRoad; j++)
        {
            int cell = estadoAtual->getCell(i, j);
            switch (cell)
            {
            case EMPTY_CELL:
                *out << "-";
                break;

            case BODY_CAR:
                *out << "*";
                break;

            case OUT_BOUND:
                *out << " ";
                break;

            default:
                *out << veiculos[cell]->getVelocidade();
                break;
            }
        }
        *out << std::endl;
    }
    *out << std::endl;
}

void Simulacao::printEstacoesPasso(std::ostream *out) const
{
    *out << "[s]\t";
    int count = 0;
    int e = 0;
    while (count < sizeRoad)
    {
        if (e < totalEstacoes && estacoes[e]->getStartStation(sizeRoad) == count)
        {
            int i = count;
            int j = 0;
            while (j < estacoes[e]->getSize())
            {
                *out << estacoes[e]->getId();
                i++;
                j++;
            }
            e++;
            count = i - 1;
        }
        else
        {
            *out << " ";
        }
        count++;
    }
    *out << std::endl;
}

void Simulacao::setFile(Arquivo *file)
{
    this->file = file;
}

void Simulacao::changeRoad()
{
    estadoAtual->cleanCars();
    for (size_t i = 0; i < qtdVeiculos; i++)
    {
        if (regraModifacaoRL(veiculos[i]) && regraSegurancaRL(veiculos[i])) // Go to STOP_LANE
        {
            veiculos[i]->setPos(veiculos[i]->getRoad() - 1, veiculos[i]->getPosRoad());
        }
        else if (regraModifacaoLR(veiculos[i]) && regraSegurancaLR(veiculos[i])) // Go to MAIN_LANE
        {
            veiculos[i]->setPos(veiculos[i]->getRoad() + 1, veiculos[i]->getPosRoad());
            veiculos[i]->setNextStationID();
        }
        estadoAtual->setCar(veiculos[i]->getRoad(), veiculos[i]->getPosRoad(), veiculos[i]);
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

bool Simulacao::regraModifacaoLR(Veiculo *veiculo) // STOP_LANE to MAIN_LANE
{
    int eId = veiculo->getNextStationID();
    return estacoes[eId]->isCarInStopLineOut(veiculo->getPosRoad(), sizeRoad);
}

bool Simulacao::regraModifacaoRL(Veiculo *veiculo) // MAIN_LANE to STOP_LANE
{
    int eId = veiculo->getNextStationID();
    return estacoes[eId]->isCarInStopLineIn(veiculo->getPosRoad(), sizeRoad);
}

bool Simulacao::regraSegurancaLR(Veiculo *veiculo) // STOP_LANE to MAIN_LANE
{
    bool regraBackCar = false;
    int rigthRoad = this->getVeiculoSideRoad(veiculo->getRoad(), Direcao::left_to_rigth);
    if (rigthRoad == OUT_BOUND || !estadoAnterior->isCarFit(rigthRoad, veiculo->getPosRoad(), veiculo))
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
        regraBackCar = distBackCar >= previousCar->getVelocidade();
    }

    int distNextCar = distanceNextCar(veiculo, rigthRoad);

    return regraBackCar && distNextCar >= veiculo->getVelocidade();
}

bool Simulacao::regraSegurancaRL(Veiculo *veiculo)
{
    bool regraBackCar = false;
    int leftRoad = this->getVeiculoSideRoad(veiculo->getRoad(), Direcao::rigth_to_left);
    if (leftRoad == OUT_BOUND || !estadoAnterior->isCarFit(leftRoad, veiculo->getPosRoad(), veiculo))
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
        regraBackCar = distBackCar >= previousCar->getVelocidade();
    }

    int distNextCar = distanceNextCar(veiculo, leftRoad);

    return regraBackCar && distNextCar >= veiculo->getVelocidade();
}

void Simulacao::checkQtdVeiculos() const
{
    int count = 0;
    for (size_t i = 0; i < qtdRoad; i++)
    {
        for (size_t j = 0; j < sizeRoad; j++)
        {
            if (estadoAnterior->isCellCar(i, j))
            {
                count++;
            }
        }
    }

    if (count != qtdVeiculos)
    {
        std::cout << "ERRO::SIMULACAL::checkQtdVeiculos::Algum veículo se perdeu\n";
        file->write("ERRO::SIMULACAL::checkQtdVeiculos::Algum veículo se perdeu\n");
        this->print(file->getStream());
        this->~Simulacao();
    }
}

void Simulacao::printDados(std::ostream *out) const
{
    *out << "Quantidade de pistas: " << qtdRoad << std::endl;
    *out << "comprimento das pistas: " << sizeRoad << std::endl;
    *out << "Quantidade de veículos: " << qtdVeiculos << std::endl;
    *out << "Velocidade Máxima: " << velocityMax << std::endl;
}