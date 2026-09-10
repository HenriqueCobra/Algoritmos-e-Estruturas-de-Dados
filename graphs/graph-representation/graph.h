#ifndef GRAPH_H
#define GRAPH_H

/* Grafo simples (sem arestas paralelas nem laços) com vértices numerados de
   0 a n-1, guardado como lista de adjacência: para cada vértice, uma lista
   encadeada dos seus vizinhos.

   Complexidade da lista de adjacência, com V vértices e E arestas:
     - espaço            O(V + E)
     - adicionar aresta  O(grau) para checar duplicata
     - "existe u-v?"     O(grau(u))
     - percorrer vizinhos de u   O(grau(u))

   A alternativa é a matriz de adjacência (V x V): "existe u-v?" vira O(1), mas o
   espaço é O(V^2) e percorrer vizinhos é sempre O(V). Vale a pena só para grafos
   densos. Veja o README. */
typedef struct grafo Grafo;

/* `dirigido` != 0 cria um dígrafo; caso contrário cada aresta vale nos dois
   sentidos. Devolve NULL se n <= 0 ou faltar memória. */
Grafo *grafo_criar(int n, int dirigido);
void   grafo_destruir(Grafo *g);

int grafo_num_vertices(const Grafo *g);
int grafo_num_arestas(const Grafo *g);
int grafo_dirigido(const Grafo *g);

/* Adiciona a aresta u->v (e v->u se não dirigido).
   0 em sucesso; 1 se a aresta já existia; -1 se um índice é inválido, u == v,
   ou faltou memória. */
int grafo_adicionar_aresta(Grafo *g, int u, int v);
/* Remove a aresta. 0 em sucesso, -1 se ela não existe ou índice inválido. */
int grafo_remover_aresta(Grafo *g, int u, int v);

int grafo_tem_aresta(const Grafo *g, int u, int v);
int grafo_grau_saida(const Grafo *g, int u);
int grafo_grau_entrada(const Grafo *g, int u);

/* Chama `visita(vizinho, ctx)` para cada vizinho de saída de u, em ordem
   crescente de índice. */
void grafo_para_cada_vizinho(const Grafo *g, int u,
                             void (*visita)(int vizinho, void *ctx), void *ctx);

#endif
