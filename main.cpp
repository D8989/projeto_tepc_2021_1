#include "simulacao/Simulacao.hpp"
#include "arquivo/Arquivo.hpp"
#include <iostream>
#include <stdlib.h>
#include <ctime>
#include <string>

int main(int argc, const char *argv[])
{
    srand(time(NULL));

    if (argc != 8)
    {
        std::cout << "Deve-se passar 6 parametros inteiros: qtd_passos tamanho_pista qtd_pista qtd_carros vel_max tamanho_veiculos salvar_Arquivo\n";
        exit(EXIT_FAILURE);
    }
    int qtdPassos = atoi(argv[1]);
    int comprimentoPista = atoi(argv[2]);
    int qtdPistas = atoi(argv[3]);
    int qtdCarros = atoi(argv[4]);
    int velocidadeMax = atoi(argv[5]);
    int tamanhoVeiculo = atoi(argv[6]);
    int salvarEmArquivo = atoi(argv[7]);

    Simulacao s(comprimentoPista, qtdPistas, qtdCarros, velocidadeMax, tamanhoVeiculo);
    s.printDados(&std::cout);
    Arquivo *arq = new Arquivo();

    if (salvarEmArquivo)
    {
        const char *param[] = {"ressult", argv[1], argv[2], argv[4], argv[5]};
        std::string str = arq->montarNome(param, 5);
        arq->open(str.c_str());
        s.setFile(arq);
    }

    s.run(qtdPassos);

    return 0;
}