#ifndef VEICULO_HPP
#define VEICULO_HPP

class Veiculo
{
private:
    unsigned int ID;
    int velocidade;

public:
    Veiculo(int, int);
    ~Veiculo();

    unsigned int getId();
    int getVelocidade();
    void setVelocidade(int);
    void print();
};

#endif