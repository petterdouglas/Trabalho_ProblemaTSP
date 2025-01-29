#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <utility>
#include <queue>
#include <stack>
#include <cmath>
#include <chrono>

using namespace std;
using namespace chrono;

typedef pair<int, int> ii;
typedef pair<double, int> di;
typedef pair<double, double> dd;
typedef vector<int> vi;
typedef vector<double> vd;
typedef vector<pair<double, ii>> vdii;
typedef vector<pair<int, pair<double, bool>>> vidb;
typedef vector<vidb> vv;
typedef vector<pair<int, dd>> vidd;

double GrausParaRadianoGeografico(double valor)
{
    // Converte o formato DDD.MM para radianos conforme especificado
    int graus = static_cast<int>(valor); // Parte inteira (graus)
    double minutos = valor - graus;      // Parte decimal (minutos)
    return M_PI * (graus + 5.0 * minutos / 3.0) / 180.0;
}

double distanciaGeografica(double valor1X, double valor1Y, double valor2X, double valor2Y)
{
    const double RRR = 6378.388; // Raio idealizado da Terra em km

    // Converte os valores para latitude e longitude em radianos
    double latitude1 = GrausParaRadianoGeografico(valor1X);
    double longitude1 = GrausParaRadianoGeografico(valor1Y);
    double latitude2 = GrausParaRadianoGeografico(valor2X);
    double longitude2 = GrausParaRadianoGeografico(valor2Y);

    // Calcula os componentes necessários
    double q1 = cos(longitude1 - longitude2);
    double q2 = cos(latitude1 - latitude2);
    double q3 = cos(latitude1 + latitude2);

    // Calcula a distância geográfica conforme a fórmula
    double dij = RRR * acos(0.5 * ((1.0 + q1) * q2 - (1.0 - q1) * q3)) + 1.0;

    // Retorna a distância arredondada para o inteiro mais próximo
    return static_cast<int>(dij);
}

int calcularDistanciaEUC_2D(double valor1X, double valor1Y, double valor2X, double valor2Y)
{
    double xd = valor2X - valor1X;
    double yd = valor2Y - valor1Y;
    double distancia = sqrt(xd * xd + yd * yd);
    return static_cast<int>(distancia + 0.5); // Arredonda para o inteiro mais próximo
}

void Menor1Menor2(int &marcador1, int &marcador2, vv &lista_de_adjacencia, int u)
{
    for (int i = 0; i < (int)lista_de_adjacencia[u].size(); i++)
    {
        double pesoAtual = lista_de_adjacencia[u][i].second.first;
        double pesoMarc1;
        double pesoMarc2;

        if (lista_de_adjacencia[u][i].second.second == true)
        {

            if (marcador1 == -1)
            {
                marcador1 = i;
            }
            else
            {
                pesoMarc1 = lista_de_adjacencia[u][marcador1].second.first;
                if (pesoAtual < pesoMarc1)
                {
                    marcador2 = marcador1;
                    marcador1 = i;
                }
                else
                {
                    if (marcador2 == -1)
                    {
                        marcador2 = i;
                    }
                    else
                    {
                        pesoMarc2 = lista_de_adjacencia[u][marcador2].second.first;
                        if (pesoAtual < pesoMarc2)
                        {
                            marcador2 = i;
                        }
                    }
                }
            }
            lista_de_adjacencia[u][i].second.second = false;
            lista_de_adjacencia[i][u].second.second = false;
        }
    }
}

int main()
{
    auto start = high_resolution_clock::now();

    string arq;

    cin >> arq;

    ifstream arquivo(arq);

    if (!arquivo.is_open())
    {
        cerr << "Erro ao abrir o arquivo!" << endl;
        return 1;
    }

    int tamanho;
    string linha;
    string tipoCalc;
    string lixo;

    while (getline(arquivo, linha))
    {
        if (linha.find("DIMENSION") != string::npos)
        {
            istringstream tamlinha(linha);
            tamlinha >> lixo >> lixo >> tamanho;
        }
        if (linha.find("EDGE_WEIGHT_TYPE") != string::npos)
        {
            istringstream tipoCalcAresta(linha);
            tipoCalcAresta >> lixo >> lixo >> tipoCalc;
        }
        if (linha.find("NODE_COORD_SECTION") != string::npos)
            break;
    }
    cout << "Qtd. vertices: " << tamanho << endl;
    cout << "Tipo de calculo para distancia das arestas: " << tipoCalc << endl;

    vidd lista_de_vertices;
    int id;
    double coord_x, coord_y;

    while (getline(arquivo, linha))
    {

        if (linha.find("EOF") != string::npos)
        {
            break;
        }
        else
        {
            istringstream coordenadas(linha);
            coordenadas >> id >> coord_x >> coord_y;
            lista_de_vertices.emplace_back(make_pair(id, make_pair(coord_x, coord_y))); // armazena os vertices com seu ID e suas coordenadas x e y
        }
    }

    vv lista_de_adjacencia(tamanho); // vetor para armazenamento das arestas dos vertices "i" e "j" e seu respectivo peso
    for (int i = 0; i < tamanho; i++)
    {
        double valor1X = lista_de_vertices[i].second.first;
        double valor1Y = lista_de_vertices[i].second.second;
        double valor2X, valor2Y, peso;
        for (int j = 0; j < tamanho; j++)
        {
            valor2X = lista_de_vertices[j].second.first;
            valor2Y = lista_de_vertices[j].second.second;
            if (i == j)
            {
                peso = 0;
                lista_de_adjacencia[i].emplace_back(make_pair(j, make_pair(peso, false)));
            }
            else
            {
                if (tipoCalc == "GEO") // avalia o tipo do cálculo do peso da distância entre os vertices(arestas)
                {
                    peso = distanciaGeografica(valor1X, valor1Y, valor2X, valor2Y);
                    lista_de_adjacencia[i].emplace_back(make_pair(j, make_pair(peso, true)));
                }
                if (tipoCalc == "EUC_2D")
                {
                    peso = calcularDistanciaEUC_2D(valor1X, valor1Y, valor2X, valor2Y);
                    lista_de_adjacencia[i].emplace_back(make_pair(j, make_pair(peso, true)));
                }
            }
        }
    }

    // INICIO DA LÓGICA DO ALGORITMO

    // Escolha do ponto inicio será estrategicamente escolhida pelo vértice que compõe as duas arestas com menores pesos do grafo

    double melhorMaiorDistancia = numeric_limits<double>::max();
    vd melhor_sequencia;

    for (int inicio = 0; inicio < tamanho; inicio++)
    {
        vd sequencia_resultante;
        sequencia_resultante.push_back(inicio);
        lista_de_adjacencia[inicio][inicio].second.second = false;
        lista_de_adjacencia[inicio][inicio].second.second = false;

        int marcador1 = -1;
        int marcador2 = -1;

        // descobrir dois marcadores de saída
        Menor1Menor2(marcador1, marcador2, lista_de_adjacencia, inicio);

        // pilha para caminho de marcador2
        stack<int> caminho2;
        caminho2.push(marcador2);
        // fila para caminho de marcador1
        queue<int> caminho1;
        caminho1.push(marcador1);

        // controle de percurso
        bool fechouCiclo = false;

        // Lógica de escolha do caminho
        while (!fechouCiclo)
        {
            lista_de_adjacencia[marcador1][marcador2].second.second = false;
            lista_de_adjacencia[marcador2][marcador1].second.second = false;

            int menor1Marc1 = -1;
            int menor2Marc1 = -1;
            Menor1Menor2(menor1Marc1, menor2Marc1, lista_de_adjacencia, marcador1);

            int menor1Marc2 = -1;
            int menor2Marc2 = -1;
            Menor1Menor2(menor1Marc2, menor2Marc2, lista_de_adjacencia, marcador2);

            if (menor1Marc1 == -1 or menor2Marc1 == -1 or menor1Marc2 == -1 or menor2Marc2 == -1)
            {
                if (menor1Marc1 > -1)
                {
                    caminho1.push(menor1Marc1);
                }
                else if (menor2Marc1 > -1)
                {
                    caminho1.push(menor2Marc1);
                }
                else if (menor1Marc2 > -1)
                {
                    caminho2.push(menor1Marc2);
                }
                else if (menor2Marc2 > -1)
                {
                    caminho2.push(menor2Marc2);
                }

                fechouCiclo = true;
            }
            else
            {
                if (menor1Marc1 == menor1Marc2)
                {
                    double pesoMenor1Marc1 = lista_de_adjacencia[marcador1][menor1Marc1].second.first;
                    double pesoMenor2Marc1 = lista_de_adjacencia[marcador1][menor2Marc1].second.first;
                    double pesoMenor1Marc2 = lista_de_adjacencia[marcador2][menor1Marc2].second.first;
                    double pesoMenor2Marc2 = lista_de_adjacencia[marcador2][menor2Marc2].second.first;
                    double gastoEscolha1 = pesoMenor1Marc1 + pesoMenor2Marc2; // menor1Marc1 é escolhido
                    double gastoEscolha2 = pesoMenor2Marc1 + pesoMenor1Marc2; // menor1Marc2 é escolhido
                    if (gastoEscolha1 < gastoEscolha2)
                    {
                        marcador1 = menor1Marc1;
                        marcador2 = menor2Marc2;
                    }
                    else if (gastoEscolha1 > gastoEscolha2)
                    {
                        marcador1 = menor2Marc1;
                        marcador2 = menor1Marc2;
                    }
                    else
                    {
                        if (pesoMenor1Marc1 <= pesoMenor1Marc2)
                        {
                            marcador1 = menor1Marc1;
                            marcador2 = menor2Marc2;
                        }
                        else
                        {
                            marcador1 = menor2Marc1;
                            marcador2 = menor1Marc2;
                        }
                    }
                    caminho1.push(marcador1);
                    caminho2.push(marcador2);
                }
                else
                {
                    marcador1 = menor1Marc1;
                    marcador2 = menor1Marc2;
                    caminho1.push(marcador1);
                    caminho2.push(marcador2);
                }
            }
        }

        // construir sequencia baseado nos caminhos de marcador1 e marcador2
        while (!caminho1.empty())
        {
            int vertice = caminho1.front();
            // cout << vertice << " ";
            sequencia_resultante.push_back(vertice);
            caminho1.pop();
            if (vertice == -1)
            {
                break;
            }
        }

        while (!caminho2.empty())
        {
            int vertice = caminho2.top();
            sequencia_resultante.push_back(vertice);
            caminho2.pop();
        }

        double maiorDistanciaAtual = 0;
        int vertice1, vertice2;
        int tamSequencia = (int)sequencia_resultante.size();

        for (int i = 1; i < tamSequencia; i++)
        {
            vertice1 = sequencia_resultante[i - 1];
            vertice2 = sequencia_resultante[i];
            double pesoAtual = lista_de_adjacencia[vertice1][vertice2].second.first;
            if (pesoAtual > maiorDistanciaAtual)
            {
                maiorDistanciaAtual = pesoAtual;
            }
        }

        if (maiorDistanciaAtual < melhorMaiorDistancia and tamSequencia == tamanho)
        {
            melhorMaiorDistancia = maiorDistanciaAtual;
            melhor_sequencia = sequencia_resultante;
        }

        for (int x = 0; x < tamanho; x++)
        {
            for (auto &aresta : lista_de_adjacencia[x])
            {
                if (aresta.first == x)
                {
                    continue;
                }
                aresta.second.second = true;
            }
        }
    }

    auto end = high_resolution_clock::now();

    auto duration = duration_cast<milliseconds>(end - start);

    cout << endl << "Tempo de execucao: " << duration.count() << " ms" << endl;
    cout << endl << "Maior distancia: " << melhorMaiorDistancia << endl;

    ofstream resultado("saida.txt");

    for (const auto &vertice : melhor_sequencia)
    {
        resultado << vertice << " ";
    }

    arquivo.close();

    return 0;
}