#include "simulacao/Simulacao.hpp"
#include <iostream>
#include <stdlib.h>
#include <ctime>

int main(int argc, const char *argv[])
{
    srand(time(NULL));

    if (argc != 6) {
        std::cout << "Deve-se passar 5 parametros inteiros: qtd_passos tamanho_pista qtd_pista qtd_carros vel_max\n";
        exit(EXIT_FAILURE);
    }
    int qtdPassos = atoi(argv[1]);
    int comprimentoPista = atoi(argv[2]);
    int qtdPistas = atoi(argv[3]);
    int qtdCarros = atoi(argv[4]);
    int velocidadeMax = atoi(argv[5]);

    Simulacao s(comprimentoPista, qtdPistas, qtdCarros, velocidadeMax);

    s.run(qtdPassos);

    return 0;
}