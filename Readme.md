# Problema das Paradas de Ônibus

## Formato das instâncias

O formato das instâncias é o mesmo das instância do problema do caixeiro viajante.

Na página da TSPLIB encontra-se uma [documentação](http://comopt.ifi.uni-heidelberg.de/software/TSPLIB95/tsp95.pdf) detalhada.

## Funcionalidade do código

> Leitura de Arquivo de Entrada:

_O programa lê um arquivo contendo:_
A dimensão do grafo (número de vértices).
O tipo de cálculo de peso das arestas (GEO ou EUC_2D).
As coordenadas dos vértices (latitude e longitude ou coordenadas cartesianas).

> Cálculo de Distâncias:

- **Distância Geográfica (GEO):**
  Utiliza a fórmula da distância esférica com base na latitude e longitude dos vértices.

- **Distância Euclidiana 2D (EUC_2D):**
  Aplica a fórmula padrão de distância euclidiana em um plano cartesiano.

> Representação do Grafo:

O grafo é representado por uma lista de adjacência, onde cada vértice possui uma lista de arestas com seus pesos e estados (visitadas ou não).

> Construção de Caminhos:

Identifica os dois caminhos com menores pesos a partir de um vértice inicial.
Explora uma abordagem iterativa utilizando pilhas e filas para construir os caminhos, fechando ciclos quando necessário.

> Estratégia de Escolha:

O vértice inicial é estrategicamente escolhido com base nas menores arestas conectadas.
Utiliza dois marcadores (marcador1 e marcador2) para guiar a construção dos caminhos.
Utiliza da estratégia do vizinho mais próximo para a escolha dos marcadores seguintes, que sejam os com menores distâncias possíveis do marcador atual.

## Uso das bibliotecas

- **iostream:** Manipulação de entrada e saída de dados.
- **fstream:** Leitura e manipulação de arquivos.
- **sstream:** Manipulação de strings como streams, permitindo extrair ou formatar dados em strings. Uso do 'istringstream'.
- **vector:** Estrutura de dados dinâmica para armazenar e manipular listas de elementos.
- **utility:** Estruturas auxiliares, como pares (pair), para agrupar dois valores relacionados.
- **queue:** Estrutura de dados FIFO (First In, First Out) para gerenciar filas.
- **stack:** Estrutura de dados LIFO (Last In, First Out) para gerenciar pilhas.
- **cmath:** Operações matemáticas avançadas.
- **chrono:** Marcação de tempo de execução do código.

## Especificação de funções e métodos utilizados

> GrausParaRadianoGeografico:

Função auxiliar para a função distanciaGeografica responsável por ler os valores vindos direto do arquivo .ins, que está no padrão de graus e minutos, e convertê-los para radianos. Logo após, os valores convertidos são retornados para a função distanciaGeografica.

> distanciaGeografica:

Função responsável por realizar o cálculo da distância esférica entre dois pontos com base em suas coordenadas geográficas.

> calcularDistanciaEUC_2D:

Função responsável por calcular a distância euclidiana entre dois pontos em um plano cartesiano.

> Menor1Menor2:

Função que identifica as duas menores arestas conectadas a um vértice, atualizando os estados das arestas. Retornando os dois menores valores em sequência.
