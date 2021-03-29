#include "simulacao/Simulacao.hpp"
#include "arquivo/Arquivo.hpp"
#include <iostream>
#include <stdlib.h>
#include <ctime>
#include <string>

int main(int argc, const char *argv[])
{
    srand(time(NULL));

    if (argc != 7)
    {
        std::cout << "Deve-se passar 6 parametros inteiros: qtd_passos tamanho_pista qtd_pista qtd_carros vel_max salvar_Arquivo\n";
        exit(EXIT_FAILURE);
    }
    int qtdPassos = atoi(argv[1]);
    int comprimentoPista = atoi(argv[2]);
    int qtdPistas = atoi(argv[3]);
    int qtdCarros = atoi(argv[4]);
    int velocidadeMax = atoi(argv[5]);
    int salvarEmArquivo = atoi(argv[6]);

    Simulacao s(comprimentoPista, qtdPistas, qtdCarros, velocidadeMax);
    Arquivo *arq = new Arquivo();

    if (salvarEmArquivo)
    {
        std::string str("resultado_simples_");
        str.append(argv[1]);
        str.append("_");
        str.append(argv[2]);
        str.append("_");
        str.append(argv[4]);
        str.append("_");
        str.append(argv[5]);
        str.append(".txt");

        arq->open(str.c_str());
        s.setFile(arq);
    }

    s.run(qtdPassos);

    return 0;
}