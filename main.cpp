#include "simulacao/Simulacao.hpp"
#include <stdlib.h>
#include <ctime>

int main(int argc, const char *argv[])
{
    srand(time(NULL));

    /**
     * Primeiro valor é o tamanho da faixa
     * segundo é a quantidade de faixas
     * terceiro é a quantidade de carros
     * quarto é a velocidade máxima
     **/
    Simulacao s(70, 1, 10, 6);

    // Quantidade de passos do automato
    s.run(40);

    return 0;
}