#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include <chrono>
#include <thread>
#include <cmath>

using namespace std;
using namespace std::chrono;

struct ThreadData {
    const vector<vector<float>>& matrizA;
    const vector<vector<float>>& matrizB;
    vector<vector<float>>& matrizC;
    int linhaInicio;
    int linhaFim;
};

vector<vector<float>> lerMatrizDoArquivo(const string& nomeArquivo) {
    ifstream arquivo(nomeArquivo);
    vector<vector<float>> matriz;

    if(arquivo.is_open()){
        int linhas, colunas;
        arquivo>>linhas>>colunas;
        matriz.resize(linhas, vector<float>(colunas));
        // arquivo.ignore(); para ignorar o espaço gerado pela quebra de linha
        arquivo.ignore();
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

void geraArquivoResultado(long long tempoExecucao,string name) {
    ofstream arquivo(name+".txt");
    if (arquivo.is_open()) {
         arquivo<<tempoExecucao;
        arquivo.close();
        cout<<"Arquivo "<<name<<".txt gerado com sucesso"<<endl;
    }else{
        cout<<"Ocorrou um erro ao gerar o arquivo "<<name<<".txt"<<endl;
    }
}
void produtoMatricialParcial(const vector<vector<float>>& matrizA, const vector<vector<float>>& matrizB,int indexInicio,int indexFim, vector<vector<float>>& matrizC) {
    int linhasA=matrizA.size();
    int colunasA=matrizA[0].size();
    int linhasB=matrizB.size();
    int colunasB=matrizB[0].size();
    //verificacao das dimensoes para poder realizar o produto de matrizes
    if (colunasA!=linhasB) {
        cout<<"As dimensões das matrizes não são compatíveis para multiplicação."<<endl;
        return;
    }

    for (int i = indexInicio; i < indexFim; ++i) {
            for (int j = 0; j < colunasB; ++j) {
                for (int k = 0; k < colunasA; ++k) {
                    matrizC[i][j] += matrizA[i][k] * matrizB[k][j];
                    cout<<i<<j<<" "<<matrizC[i][j]<<endl;
                }
            }
        }
   // return matrizC;
}
void imprimirMatriz(const vector<vector<float>>& matriz) {
    for(const auto& linha : matriz){
        for(const auto& elemento : linha){
            cout << elemento << " ";
        }
        cout << endl;
    }
}
void calcularProdutoParcial(const ThreadData& data) {
    vector<vector<float>>matriz(3,vector<float>(3, 0.0));
    for (int i = data.linhaInicio; i < data.linhaFim; ++i) {
        for (int j = 0; j < data.matrizB[0].size(); ++j) {
            for (int k = 0; k < data.matrizA[0].size(); ++k) {
                data.matrizC[i][j] += data.matrizA[i][k] * data.matrizB[k][j];
                matriz[i][j] += data.matrizA[i][k] * data.matrizB[k][j];
            }
        }
    }
}
void criaThreads(const vector<vector<float>>& matrizA, const vector<vector<float>>& matrizB,int P){
    vector<thread> threads;//vetor para armazenar as threads
    int linhasA=matrizA.size();
    int colunasB=matrizB[0].size();
    int elementosPorThreads=ceil((double)(linhasA*colunasB)/P);
    vector<vector<float>>matrizC(linhasA,vector<float>(colunasB, 0.0));
   
    //começa cronometragem
    auto inicio = steady_clock::now();

    int linhasPorThread = linhasA / P;
    int linhaInicio = 0;

    // criação e execução das threads
    for (int i = 0; i < P - 1; ++i) {
        int linhaFim = linhaInicio + linhasPorThread;
        threads.emplace_back(calcularProdutoParcial,ThreadData{matrizA, matrizB, matrizC, linhaInicio, linhaFim});
        linhaInicio = linhaFim;
    }
    threads.emplace_back(calcularProdutoParcial, ThreadData{matrizA, matrizB, matrizC, linhaInicio, linhasA});

    for(auto& t : threads){
        t.join();
    }
    imprimirMatriz(matrizC);
    auto fim = steady_clock::now();
    auto tempoExecucao = duration_cast<milliseconds>(fim - inicio);
    
    for(int i = 0; i < P; ++i){
        geraArquivoResultado(tempoExecucao.count(),"thread"+to_string(i));
    }
    return;
}
int main() {
    // Ler as matrizes dos arquivos
    string nomeArq1,nomeArq2;
    int P;
    // cout<<"Digite o nome dos arquivos para leitura das matrizes:"<<endl;
    // cin>>nomeArq1>>nomeArq2;
    cout<<"Digite o número de threads P:"<<endl;
    cin>>P;
    // vector<vector<float>> matrizA = lerMatrizDoArquivo(nomeArq1+".txt");
    // vector<vector<float>> matrizB = lerMatrizDoArquivo(nomeArq2+".txt");
    vector<vector<float>> matrizA = lerMatrizDoArquivo("matriza.txt");
    vector<vector<float>> matrizB = lerMatrizDoArquivo("matrizb.txt");
    
    int linhas = matrizA.size();
    int colunas = matrizB[0].size(); 
  
    criaThreads(matrizA,matrizB,P);
    return 0;
  
}
