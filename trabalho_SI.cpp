#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <utility>
#include <queue>
#include <functional>
#include <cmath>
using namespace std;

typedef pair<double, int> di;
typedef pair<double, double> dd;
typedef vector<int> vi;
typedef vector<double> vd;
typedef vector<pair<int, pair<double, bool>>> vid;
typedef vector<vid> vv;
typedef vector<pair<int, dd>> vidd;

double GrausParaRadiano(double valor)
{
    return valor * M_PI;
}

double haversine(double valor1X, double valor1Y, double valor2X, double valor2Y)
{
    if (valor1X == valor2X and valor1Y == valor2Y)
    {
        return 0;
    }
    else
    {
        const double Raio = 6371.0; // Raio da Terra em km
        valor1X = GrausParaRadiano(valor1X);
        valor1Y = GrausParaRadiano(valor1Y);
        valor2X = GrausParaRadiano(valor2X);
        valor2Y = GrausParaRadiano(valor2Y);

        double dlatitude = valor2X - valor1X;
        double dlongitude = valor2Y - valor1Y;

        double a = sin(dlatitude / 2) * sin(dlatitude / 2) +
                   cos(valor1X) * cos(valor2X) * sin(dlongitude / 2) * sin(dlongitude / 2);

        double c = 2 * atan2(sqrt(a), sqrt(1 - a));

        return Raio * c;
    }
}

double calcularDistanciaEUC_2D(double valor1X, double valor1Y, double valor2X, double valor2Y)
{
    return sqrt(pow(valor2X - valor1X, 2) + pow(valor2Y - valor1Y, 2));
}

int main()
{
    ifstream arquivo("03.ins");

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
    cout << "Qtd. vértices: " << tamanho << endl;
    cout << "Tipo de cálculo para distância das arestas: " << tipoCalc << endl;

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
        double valor2X, valor2Y;
        for (int j = 0; j < tamanho; j++)
        {
            valor2X = lista_de_vertices[j].second.first;
            valor2Y = lista_de_vertices[j].second.second;
            if (tipoCalc == "GEO") // avalia o tipo do cálculo do peso da distância entre os vertices(arestas)
            {
                double peso = haversine(valor1X, valor1Y, valor2X, valor2Y);
                lista_de_adjacencia[i].emplace_back(make_pair(j, make_pair(peso, false)));
            }
            if (tipoCalc == "EUC_2D")
            {
                double peso = calcularDistanciaEUC_2D(valor1X, valor1Y, valor2X, valor2Y);
                lista_de_adjacencia[i].emplace_back(make_pair(j, make_pair(peso, false)));
            }
        }
    }

    // Escolha do ponto inicial será estrategicamente escolhida pelo vértice que compõe as duas arestas com menores pesos do grafo
    int inicial = 0;
    int u = inicial;
    vi sequencia_resultante(tamanho);
    vi vetor_graus(tamanho);

    for (int i = 0; i < tamanho; i++)
    {
        vetor_graus.push_back(tamanho - 1); // inicializa o grau de todos os vértices com (tamanho - 1), pois o grafo é completo
    }
    priority_queue<di, vector<di>, std::greater<di>> minHeap;

    for (int count = 0; count < tamanho; count++)
    {
        for (int i = 0; i < tamanho; i++)
        {
            if (i == u or lista_de_adjacencia[u][i].second.second)
            {
                continue;
            }
            else
            {
                if (u == inicial)
                {
                    double vertice = lista_de_adjacencia[u][i].first;
                    double distancia = lista_de_adjacencia[u][i].second.first;
                    minHeap.push(make_pair(distancia, vertice));
                }
                else
                {
                    int vertice1 = lista_de_adjacencia[u][i].first;
                    int vertice2 = lista_de_adjacencia[u][i + 1].first;
                }
            }
        }
        if (u == inicial)
        {
            di menor1 = minHeap.top();
            minHeap.pop();
            di menor2 = minHeap.top();
            minHeap.pop();
            vetor_graus[u] = 1;

            while (!minHeap.empty())
            {
                for (int i = 0; i < tamanho; i++)
                {
                    if (i == menor2.second)
                    {
                        continue;
                    }
                    lista_de_adjacencia[u][i].second.second = true;
                    lista_de_adjacencia[i][u].second.second = true;
                    vetor_graus[i]--;
                }
                minHeap.pop();
            }
            u = menor1.second;
        }
        else
        {
            
        }
        sequencia_resultante[count] = u;
    }

    arquivo.close();

    return 0;
}