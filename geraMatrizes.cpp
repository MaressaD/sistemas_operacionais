#include <iostream>
#include <vector>
#include <random>
#include <fstream>

using namespace std;

void preencherMatrizAleatoriamente(vector<vector<float>>& matriz, int linhas, int colunas) {
    random_device rd;
    mt19937 gen(rd());
    uniform_real_distribution<float> dis(1.0f, 100.0f); 
    for (int i = 0; i < linhas; ++i) {
        for (int j = 0; j < colunas; ++j) {
            matriz[i][j] = dis(gen);
        }
    }
}
void geraArquivoMatriz(vector<vector<float>>& matriz, string name,string tagLine) {
 ofstream arquivo(name+".txt");
    int linhas = matriz.size();
    int colunas = matriz[0].size();
    if (arquivo.is_open()) {
        arquivo<<linhas<<" "<<colunas<<endl;
        for (int i = 0; i < linhas; ++i) {
                for (int j = 0; j < colunas; ++j) {
                     arquivo<<tagLine<<i+1<<j+1<<" "<<matriz[i][j]<<" "<<endl;
            }
            arquivo<<endl;
        }
        arquivo.close();
        cout<<"Arquivo "<<name<<".txt gerado com sucesso"<<endl;
    }else{
        cout<<"Ocorrou um erro ao gerar o arquivo "<<name<<".txt"<<endl;
    }
}
int main() {
    int n1,m1,n2,m2;
    cout<<"Digite as dimensões das duas matrizers, sendo n1 m1 n2 e m2 respectivamente\n"<<endl;
    cin>>n1>>m1>>n2>>m2;
    
    vector<vector<float>> matrizA(m1, vector<float>(m2));
    vector<vector<float>> matrizB(n1, vector<float>(n2));

    preencherMatrizAleatoriamente(matrizA, m1, m2);
    preencherMatrizAleatoriamente(matrizB, n1, n2);

    geraArquivoMatriz(matrizA,"matriza","a");
    geraArquivoMatriz(matrizB, "matrizb","b");

    return 0;
}
