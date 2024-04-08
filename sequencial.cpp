#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include <chrono>

using namespace std;
using namespace std::chrono;

vector<vector<float>> lerMatrizDoArquivo(const string& nomeArquivo) {
    ifstream arquivo(nomeArquivo);
    vector<vector<float>> matriz;

    if(arquivo.is_open()){
        int linhas, colunas;
        arquivo>>linhas>>colunas;
        matriz.resize(linhas, vector<float>(colunas));
        arquivo.ignore();//para ignorar o espaço gerado pela quebra de linha
        for(int i = 0; i < linhas; ++i){
            for(int j = 0; j < colunas; ++j){
                string linha;
                arquivo.ignore();
                //condicional para quando chegar ao fim do arquivo
                if(!getline(arquivo, linha))
                    break;
                istringstream iss(linha);
                string token;
                iss >> token; 
                iss >> matriz[i][j];
            }
        }
        arquivo.close();
    } else {
        cout << "Erro ao abrir o arquivo " << nomeArquivo << endl;
    }

    return matriz;
}

// Função para imprimir a matriz
void imprimirMatriz(const vector<vector<float>>& matriz) {
    for(const auto& linha : matriz){
        for(const auto& elemento : linha){
            cout << elemento << " ";
        }
        cout << endl;
    }
}
void geraArquivoMatriz(vector<vector<float>>& matriz, string name,string tagLine,long long tempoExecucao) {
 ofstream arquivo(name+".txt");
    int linhas = matriz.size();
    int colunas = matriz[0].size();
    if (arquivo.is_open()) {
        arquivo<<linhas<<" "<<colunas<<endl;
        for (int i = 0; i < linhas; ++i) {
                for (int j = 0; j < colunas; ++j) {
                    arquivo<<tagLine<<i+1<<j+1<<" "<<matriz[i][j]<<" "<<endl;
                    if(i+1 == linhas && j+1 == colunas){                
                        arquivo<<tempoExecucao;
                           
                    } 
            }
            arquivo<<endl; 
        }
        arquivo.close();
        cout<<"Arquivo "<<name<<".txt gerado com sucesso"<<endl;
    }else{
        cout<<"Ocorrou um erro ao gerar o arquivo "<<name<<".txt"<<endl;
    }
}
vector<vector<float>> produtoMatricial(const vector<vector<float>>& matrizA, const vector<vector<float>>& matrizB) {
    int linhasA=matrizA.size();
    int colunasA=matrizA[0].size();
    int linhasB=matrizB.size();
    int colunasB=matrizB[0].size();
    //verificacao das dimensoes para poder realizar o produto de matrizes
    if (colunasA!=linhasB) {
        cout<<"As dimensões das matrizes não são compatíveis para multiplicação."<<endl;
        return {};
    }
    vector<vector<float>>matrizC(linhasA,vector<float>(colunasB, 0.0));
    for(int i = 0; i < linhasA; ++i){
        for(int j = 0; j < colunasB; ++j){
            for(int k = 0; k < colunasA; ++k){
                matrizC[i][j] += matrizA[i][k] * matrizB[k][j];
            }
        }
    }

    return matrizC;
}

int main() {
    // Ler as matrizes dos arquivos
    string nomeArq1,nomeArq2;
    //cout<<"Digite o nome dos arquivos para leitura das matrizes\n"<<endl;
    //cin>>nomeArq1>>nomeArq2;
    //vector<vector<float>> matrizA = lerMatrizDoArquivo(nomeArq1+".txt");
    //vector<vector<float>> matrizB = lerMatrizDoArquivo(nomeArq2+".txt");
    vector<vector<float>> matrizA = lerMatrizDoArquivo("matriza.txt");
    vector<vector<float>> matrizB = lerMatrizDoArquivo("matrizb.txt");
    
    int linhas = matrizA.size();
    int colunas = matrizB[0].size(); 

    vector<vector<float>> matrizC(linhas, vector<float>(colunas));

    steady_clock::time_point inicio = steady_clock::now();

    matrizC=produtoMatricial(matrizA,matrizB);

    steady_clock::time_point fim = steady_clock::now();

    auto tempoExecucao = duration_cast<milliseconds>(fim - inicio);

    geraArquivoMatriz(matrizC, "matrizc","c",tempoExecucao.count());

    return 0;
}
