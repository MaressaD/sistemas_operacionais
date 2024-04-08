#include <iostream>
#include <fstream>
#include <vector>
#include <cmath>
#include <unistd.h>
#include <sys/wait.h>

using namespace std;

// Função para ler uma matriz de um arquivo
vector<vector<float>> lerMatrizDoArquivo(const string& nomeArquivo) {
    ifstream arquivo(nomeArquivo);
    int linhas, colunas;
    arquivo >> linhas >> colunas;
    vector<vector<float>> matriz(linhas, vector<float>(colunas));
    for (int i = 0; i < linhas; ++i) {
        for (int j = 0; j < colunas; ++j) {
            arquivo >> matriz[i][j];
        }
    }
    arquivo.close();
    return matriz;
}

// Função para calcular a multiplicação de uma parte da matriz
void calcularParteMatriz(const vector<vector<float>>& matrizA, const vector<vector<float>>& matrizB,
                         int linhaInicio, int linhaFim, const string& nomeArquivoResultado) {
    int linhasA = matrizA.size();
    int colunasB = matrizB[0].size();
    vector<vector<float>> resultado(linhaFim - linhaInicio + 1, vector<float>(colunasB, 0));
    for (int i = linhaInicio; i <= linhaFim; ++i) {
        for (int j = 0; j < colunasB; ++j) {
            for (int k = 0; k < linhasA; ++k) {
                resultado[i - linhaInicio][j] += matrizA[i][k] * matrizB[k][j];
            }
        }
    }
    ofstream arquivoResultado(nomeArquivoResultado);
    arquivoResultado << resultado.size() << " " << resultado[0].size() << endl;
    for (int i = 0; i < resultado.size(); ++i) {
        for (int j = 0; j < resultado[i].size(); ++j) {
            arquivoResultado << resultado[i][j] << " ";
        }
        arquivoResultado << endl;
    }
    arquivoResultado.close();
}

int main(int argc, char *argv[]) {
    if (argc < 4) {
        cerr << "Uso: " << argv[0] << " <arquivo_matriz_A> <arquivo_matriz_B> <numero_processos>" << endl;
        return 1;
    }

    string nomeArquivoMatrizA = argv[1];
    string nomeArquivoMatrizB = argv[2];
    int numeroProcessos = atoi(argv[3]);

    vector<vector<float>> matrizA = lerMatrizDoArquivo(nomeArquivoMatrizA);
    vector<vector<float>> matrizB = lerMatrizDoArquivo(nomeArquivoMatrizB);
    int linhasA = matrizA.size();
    int colunasA = matrizA[0].size();
    int linhasB = matrizB.size();
    int colunasB = matrizB[0].size();

    // Verifica se é possível multiplicar as matrizes
    if (colunasA != linhasB) {
        cerr << "Erro: O número de colunas da matriz A não é igual ao número de linhas da matriz B." << endl;
        return 1;
    }

    int elementosPorProcesso = ceil((float)(linhasA * colunasB) / numeroProcessos);
    int linhaInicio = 0;
    int linhaFim = -1;

    for (int i = 0; i < numeroProcessos; ++i) {
        int pid = fork();
        if (pid == 0) { // Processo filho
            linhaInicio = linhaFim + 1;
            linhaFim = min(linhaInicio + elementosPorProcesso - 1, linhasA - 1);
            string nomeArquivoResultado = "resultado_" + to_string(i) + ".txt";
            calcularParteMatriz(matrizA, matrizB, linhaInicio, linhaFim, nomeArquivoResultado);
            return 0;
        } else if (pid < 0) { // Erro ao criar o processo
            cerr << "Erro ao criar o processo." << endl;
            return 1;
        }
    }

    // Processo pai
    // Espera pelos processos filhos terminarem
    for (int i = 0; i < numeroProcessos; ++i) {
        wait(NULL);
    }

    cout << "Multiplicação de matrizes concluída." << endl;

    return 0;
}
