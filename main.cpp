#include "simulacao/Simulacao.hpp"
#include "arquivo/Arquivo.hpp"
#include <iostream>
#include <stdlib.h>
#include <ctime>
#include <string>

int main(int argc, const char *argv[])
{
    srand(time(NULL));
    int qtdThreads = 0;
    if (argc < 8)
    {
        std::cout << "Deve-se passar 6 parametros inteiros: qtd_passos tamanho_pista qtd_estacoes qtd_carros vel_max tamanho_veiculos salvar_Arquivo\n";
        exit(EXIT_FAILURE);
    }

    if (argc == 9)
    {
        qtdThreads = atoi(argv[8]);
        if (qtdThreads < 0)
        {
            std::cerr << "Deve-se passar algum valor positivo para as trheads." << std::endl;
            exit(EXIT_FAILURE);
        }
    }
    int qtdPassos = atoi(argv[1]);
    int comprimentoPista = atoi(argv[2]);
    int qtdEstacoes = atoi(argv[3]);
    int qtdCarros = atoi(argv[4]);
    int velocidadeMax = atoi(argv[5]);
    int tamanhoVeiculo = atoi(argv[6]);
    int salvarEmArquivo = atoi(argv[7]);

    Simulacao s(comprimentoPista, 2, qtdCarros, velocidadeMax, tamanhoVeiculo, qtdEstacoes);
    s.printDados(&std::cout);
    Arquivo *arq = new Arquivo();
    Arquivo *arqStatic = new Arquivo();

    const char *paramStatic[] = {"result_static", argv[1], argv[2], argv[4], argv[5]};
    std::string nomeArqStatic = arqStatic->montarNome(paramStatic, 5);
    arqStatic->open(nomeArqStatic.c_str());
    s.setFileStatic(arqStatic);

    if (salvarEmArquivo)
    {
        const char *param[] = {"ressult", argv[1], argv[2], argv[4], argv[5]};
        std::string str = arq->montarNome(param, 5);
        arq->open(str.c_str());
        s.setFile(arq);
    }

#pragma omp parallel num_threads(qtdThreads)
    {
        s.run(qtdPassos);
    }

    return 0;
}