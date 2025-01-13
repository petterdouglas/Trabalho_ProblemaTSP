#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <utility>
#include <queue>
#include <functional>
#include <cmath>
using namespace std;

typedef pair<int, int> ii;
typedef pair<double, int> di;
typedef pair<double, double> dd;
typedef vector<int> vi;
typedef vector<double> vd;
typedef vector<pair<double, ii>> vdii;
typedef vector<pair<int, pair<double, bool>>> vidb;
typedef vector<vidb> vv;
typedef vector<pair<int, dd>> vidd;

void exportarParaMinheap(priority_queue<di, vector<di>, std::greater<di>> &minHeap, vv &lista_de_adjacencia, int u, int i)
{
    double valor1 = lista_de_adjacencia[u][i].second.first;
    double valor2 = i;
    minHeap.push(make_pair(valor1, valor2));
}

double GrausParaRadiano(double valor)
{
    return valor * M_PI / 180.0;
}

double haversine(double &valor1X, double &valor1Y, double &valor2X, double &valor2Y)
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

double calcularDistanciaEUC_2D(double &valor1X, double &valor1Y, double &valor2X, double &valor2Y)
{
    return sqrt(pow(valor2X - valor1X, 2) + pow(valor2Y - valor1Y, 2));
}

int main()
{
    ifstream arquivo("02.ins");

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
        double valor2X, valor2Y, peso;
        for (int j = 0; j < tamanho; j++)
        {
            valor2X = lista_de_vertices[j].second.first;
            valor2Y = lista_de_vertices[j].second.second;
            if (i == j)
            {
                peso = 0;
                lista_de_adjacencia[i].emplace_back(make_pair(j, make_pair(peso, true)));
            }
            else
            {
                if (tipoCalc == "GEO") // avalia o tipo do cálculo do peso da distância entre os vertices(arestas)
                {
                    peso = haversine(valor1X, valor1Y, valor2X, valor2Y);
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

    // Escolha do ponto inicial será estrategicamente escolhida pelo vértice que compõe as duas arestas com menores pesos do grafo
    int inicial = 0;
    int u = inicial;
    vdii sequencia_resultante;
    vi vetor_graus(tamanho, tamanho - 1); // inicializa o grau de todos os vértices com (tamanho - 1), pois o grafo é completo
    priority_queue<di, vector<di>, std::greater<di>> minHeap;

    di menor2 = {0, -1};
    queue<int> opcoes_vertices;

    for (int count = 0; count < tamanho; count++)
    {
        di menor1;
        di vertice_escolhido;

        for (int i = 0; i < tamanho; i++)
        {
            if (i == u)
            {
                continue;
            }
            else if (vetor_graus[i] == 0)
            {
                continue;
            }
            else
            {
                if (u == inicial)
                {
                    exportarParaMinheap(minHeap, lista_de_adjacencia, u, i);
                }
                else
                {
                    if (i == inicial and count < tamanho - 1)
                    {
                        continue;
                    }
                    else
                    {
                        int vertice_atual = i;

                        if (!opcoes_vertices.empty())
                        {
                            int menor = opcoes_vertices.front();
                            if (vetor_graus[vertice_atual] < vetor_graus[menor])
                            {
                                opcoes_vertices.push(vertice_atual);
                            }
                            if (vetor_graus[vertice_atual] == vetor_graus[menor])
                            {
                                opcoes_vertices.push(vertice_atual);
                            }
                        }
                        else
                        {
                            opcoes_vertices.push(vertice_atual);
                        }
                    }
                }
            }
        }

        if (u == inicial)
        {
            menor1 = minHeap.top();
            minHeap.pop();
            menor2 = minHeap.top();
            minHeap.pop();
            vetor_graus[u] = 1;
        }
        else
        {
            int vertice;
            double distancia;
            if (opcoes_vertices.size() > 1)
            {
                while (!opcoes_vertices.empty())
                {
                    int frente = opcoes_vertices.front();
                    exportarParaMinheap(minHeap, lista_de_adjacencia, u, frente);
                    opcoes_vertices.pop();
                }
                vertice = minHeap.top().second;
            }
            else if (opcoes_vertices.size() == 1)
            {
                vertice = opcoes_vertices.front();
                opcoes_vertices.pop();
            }
            else
            {
                cerr << "Houve algum problema!" << endl;
            }
            distancia = lista_de_adjacencia[u][vertice].second.first;
            menor1 = {distancia, vertice};
            vetor_graus[u] = 0;
        }

        while (minHeap.size() != 0)
        {
            minHeap.pop();
        }

        for (int i = 0; i < tamanho; i++)
        {
            if (u == inicial and i == menor2.second)
            {
                continue;
            }
            lista_de_adjacencia[u][i].second.second = false;
            lista_de_adjacencia[i][u].second.second = false;
            if (vetor_graus[i] > 0 and i != inicial)
            {
                vetor_graus[i]--;
            }
        }

        vertice_escolhido = menor1;

        sequencia_resultante.push_back(make_pair(vertice_escolhido.first, make_pair(u, vertice_escolhido.second)));
        u = vertice_escolhido.second;
    }

    double somatoria_resultado = 0;

    for (const auto &item : sequencia_resultante)
    {
        cout << item.second.first << " para " << item.second.second << " com peso " << item.first << endl;
        somatoria_resultado += item.first;
    }

    cout << "Resultado: " << somatoria_resultado << endl;

    arquivo.close();

    return 0;
}