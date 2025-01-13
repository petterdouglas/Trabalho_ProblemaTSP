# Problema das Paradas de Ônibus

## Formato das instâncias

   O formato das instâncias é o mesmo das instância do problema do caixeiro viajante.
   
   Na página da TSPLIB encontra-se uma [documentação](http://comopt.ifi.uni-heidelberg.de/software/TSPLIB95/tsp95.pdf) detalhada.

// verificação da lista de adjacência

    for (int i = 0; i < tamanho - 1; i++)
    {
        for (int j = 0; j < tamanho - 1; j++)
        {
            cout << "distância de " << i << " para " << lista_de_adjacencia[i][j].first << " = " << lista_de_adjacencia[i][j].second << endl;
        }
    }


   int u = 0;

    for (int i = 0; i < tamanho * tamanho; i++)
    {
        double number = lista_de_adjacencia[u][i].first;
        double distancia = lista_de_adjacencia[u][i].second.first;
        cout << "vértice: " << number << " distância: " << distancia << endl;
    }