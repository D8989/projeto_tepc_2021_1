all:
	echo usar 'make buid_seq' ou 'make build_paralel'	

build_seq:
	g++ main.cpp simulacao/Simulacao.cpp automata/Automata.cpp veiculo/Veiculo.cpp arquivo/Arquivo.cpp estacao/Estacao.cpp -o a.out

build_paralel:
	g++ main.cpp simulacao/Simulacao.cpp automata/Automata.cpp veiculo/Veiculo.cpp arquivo/Arquivo.cpp estacao/Estacao.cpp -fopenmp -o par.out