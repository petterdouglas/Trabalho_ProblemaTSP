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

double GrausParaRadianoGeografico(double valor) {
    // Converte o formato DDD.MM para radianos conforme especificado
    int graus = static_cast<int>(valor);  // Parte inteira (graus)
    double minutos = valor - graus;      // Parte decimal (minutos)
    return M_PI * (graus + 5.0 * minutos / 3.0) / 180.0;
}

double distanciaGeografica(double valor1X, double valor1Y, double valor2X, double valor2Y) {
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

int main()
{
    vector<string> arquivos = {"01.ins", "02.ins", "03.ins", "04.ins", "05.ins", "06.ins", "07.ins", "08.ins", "09.ins", "10.ins", "testes.ins"};
    int num = -1;

    while (num != 12)
    {
        cout << "Qual arquivo deseja reproduzir? [digite o numero correspondente]" << endl;
        cout << "1: 01.ins" << endl;
        cout << "2: 02.ins" << endl;
        cout << "3: 03.ins" << endl;
        cout << "4: 04.ins" << endl;
        cout << "5: 05.ins" << endl;
        cout << "6: 06.ins" << endl;
        cout << "7: 07.ins" << endl;
        cout << "8: 08.ins" << endl;
        cout << "9: 09.ins" << endl;
        cout << "10: 10.ins" << endl;
        cout << "11: testes.ins" << endl;
        cout << "12: sair";
        cout << endl
             << endl;
        cin >> num;

        ifstream arquivo(arquivos[num - 1]);

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
                if (i == u) // impede que uma posição do tipo lista_de_adjacencia[i][i] seja acessada
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


        int maior_peso = 0;

        for (const auto &item : sequencia_resultante)
        {
            if (item.first > maior_peso)
            {
                maior_peso = item.first;
            }
        }

        cout << endl;
        cout << "Maior distancia: " << maior_peso << endl;
        arquivo.close();

        char resposta;
        cout << "Deseja realizar outra operacao? (s / n)" << endl;
        cin >> resposta;
        if (resposta == 's')
        {
            continue;
        }
        if (resposta == 'n')
        {
            num = 12;
        }
    }

    return 0;
}