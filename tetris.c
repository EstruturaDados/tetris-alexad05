#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// --- DEFINIÇÕES GLOBAIS E ESTRUTURAS ---

// Define a capacidade máxima da fila e da pilha, conforme o desafio.
#define FILA_MAX 5
#define PILHA_MAX 3

/**
 * @brief Estrutura que representa uma peça do jogo.
 *
 * Cada peça possui um 'nome' (tipo) e um 'id' único para rastreamento.
 */
typedef struct {
    char nome; // Tipo da peça: 'I', 'O', 'T', 'L', 'S', 'Z', 'J'
    int id;
} Peca;

/**
 * @brief Estrutura de Fila Circular.
 *
 * Utiliza um array estático para armazenar as peças.
 * 'inicio', 'fim' e 'total' controlam o estado da fila.
 */
typedef struct {
    Peca itens[FILA_MAX];
    int inicio;
    int fim;
    int total;
} Fila;

/**
 * @brief Estrutura de Pilha.
 *
 * Utiliza um array estático. 'topo' indica a posição do último elemento.
 * O topo é -1 quando a pilha está vazia.
 */
typedef struct {
    Peca itens[PILHA_MAX];
    int topo;
} Pilha;

// --- FUNÇÕES DA FILA ---

void inicializarFila(Fila *f) {
    f->inicio = 0;
    f->fim = 0;
    f->total = 0;
}

int filaVazia(Fila *f) {
    return f->total == 0;
}

int filaCheia(Fila *f) {
    return f->total == FILA_MAX;
}

// Adiciona uma peça ao final da fila (enqueue)
void inserirFila(Fila *f, Peca p) {
    if (filaCheia(f)) {
        return; // Não deveria acontecer na lógica do jogo, mas é uma proteção.
    }
    f->itens[f->fim] = p;
    f->fim = (f->fim + 1) % FILA_MAX; // Lógica circular
    f->total++;
}

// Remove uma peça do início da fila (dequeue)
Peca removerFila(Fila *f) {
    Peca p = {{-1}, -1}; // Peça inválida para retorno em caso de erro
    if (filaVazia(f)) {
        return p;
    }
    p = f->itens[f->inicio];
    f->inicio = (f->inicio + 1) % FILA_MAX; // Lógica circular
    f->total--;
    return p;
}

// --- FUNÇÕES DA PILHA ---

void inicializarPilha(Pilha *p) {
    p->topo = -1; // -1 indica que a pilha está vazia
}

int pilhaVazia(Pilha *p) {
    return p->topo == -1;
}

int pilhaCheia(Pilha *p) {
    return p->topo == PILHA_MAX - 1;
}

// Adiciona uma peça ao topo da pilha (push)
void pushPilha(Pilha *p, Peca peca) {
    if (pilhaCheia(p)) {
        return;
    }
    p->topo++;
    p->itens[p->topo] = peca;
}

// Remove uma peça do topo da pilha (pop)
Peca popPilha(Pilha *p) {
    Peca peca = {{-1}, -1};
    if (pilhaVazia(p)) {
        return peca;
    }
    peca = p->itens[p->topo];
    p->topo--;
    return peca;
}


// --- FUNÇÕES DO JOGO ---

/**
 * @brief Gera uma nova peça com um tipo aleatório e um ID sequencial.
 *
 * @return A peça gerada.
 */
Peca gerarPeca() {
    static int id_contador = 0; // 'static' mantém o valor entre chamadas
    Peca p;
    char tipos[] = "IOTLSZJ";
    p.nome = tipos[rand() % 7];
    p.id = id_contador++;
    return p;
}

/**
 * @brief Exibe o estado atual do jogo, mostrando a fila e a pilha.
 */
void exibirEstado(Fila *f, Pilha *p) {
    printf("\n--- ESTADO ATUAL DO JOGO ---\n");
    // Mostra a Fila
    printf("Fila de pecas: ");
    if (filaVazia(f)) {
        printf("(vazia)");
    } else {
        int idx = f->inicio;
        for (int i = 0; i < f->total; i++) {
            printf("[%c%d] ", f->itens[idx].nome, f->itens[idx].id);
            idx = (idx + 1) % FILA_MAX;
        }
    }
    printf("\n");

    // Mostra a Pilha
    printf("Pilha de reserva (Topo -> Base): ");
    if (pilhaVazia(p)) {
        printf("(vazia)");
    } else {
        for (int i = p->topo; i >= 0; i--) {
            printf("[%c%d] ", p->itens[i].nome, p->itens[i].id);
        }
    }
    printf("\n-----------------------------\n");
}

/**
 * @brief Exibe o menu de opções para o jogador.
 */
void exibirMenu() {
    printf("\nOpcoes disponiveis:\n");
    printf("1 - Jogar peca da frente da fila\n");
    printf("2 - Enviar peca da fila para a pilha de reserva\n");
    printf("3 - Usar peca da pilha de reserva\n");
    printf("4 - Trocar peca da frente da fila com o topo da pilha\n");
    printf("5 - Trocar os 3 primeiros da fila com as 3 pecas da pilha\n");
    printf("0 - Sair\n");
    printf("Opcao escolhida: ");
}

// --- LÓGICA PRINCIPAL ---

int main() {
    // Inicializa o gerador de números aleatórios
    srand(time(NULL));

    // Declara e inicializa as estruturas do jogo
    Fila filaDePecas;
    Pilha pilhaDeReserva;
    inicializarFila(&filaDePecas);
    inicializarPilha(&pilhaDeReserva);

    // Preenche a fila inicial com 5 peças
    for (int i = 0; i < FILA_MAX; i++) {
        inserirFila(&filaDePecas, gerarPeca());
    }

    int opcao;
    do {
        exibirEstado(&filaDePecas, &pilhaDeReserva);
        exibirMenu();
        scanf("%d", &opcao);

        switch (opcao) {
            case 1: // Jogar peça
                if (!filaVazia(&filaDePecas)) {
                    Peca jogada = removerFila(&filaDePecas);
                    printf("\nAcao: Peca [%c%d] jogada.\n", jogada.nome, jogada.id);
                    // Adiciona uma nova peça para manter a fila cheia
                    inserirFila(&filaDePecas, gerarPeca());
                } else {
                    printf("\nAcao: Fila vazia, impossivel jogar.\n");
                }
                break;

            case 2: // Reservar peça
                if (pilhaCheia(&pilhaDeReserva)) {
                    printf("\nAcao: Pilha de reserva cheia! Impossivel reservar.\n");
                } else if (filaVazia(&filaDePecas)) {
                    printf("\nAcao: Fila vazia, impossivel reservar.\n");
                } else {
                    Peca reservada = removerFila(&filaDePecas);
                    pushPilha(&pilhaDeReserva, reservada);
                    printf("\nAcao: Peca [%c%d] movida para a reserva.\n", reservada.nome, reservada.id);
                    inserirFila(&filaDePecas, gerarPeca());
                }
                break;

            case 3: // Usar peça reservada
                if (!pilhaVazia(&pilhaDeReserva)) {
                    Peca usada = popPilha(&pilhaDeReserva);
                    printf("\nAcao: Peca [%c%d] da reserva foi usada.\n", usada.nome, usada.id);
                } else {
                    printf("\nAcao: Pilha de reserva vazia!\n");
                }
                break;

            case 4: // Trocar peça atual com topo da pilha
                if (filaVazia(&filaDePecas) || pilhaVazia(&pilhaDeReserva)) {
                    printf("\nAcao: E preciso ter pecas na fila E na pilha para trocar.\n");
                } else {
                    // Realiza a troca direta nos arrays
                    Peca temp = filaDePecas.itens[filaDePecas.inicio];
                    filaDePecas.itens[filaDePecas.inicio] = pilhaDeReserva.itens[pilhaDeReserva.topo];
                    pilhaDeReserva.itens[pilhaDeReserva.topo] = temp;
                    printf("\nAcao: Troca realizada entre a frente da fila e o topo da pilha.\n");
                }
                break;

            case 5: // Troca múltipla
                // Verifica se ambas as estruturas têm pelo menos 3 peças
                if (filaDePecas.total < 3 || pilhaDeReserva.topo < 2) {
                    printf("\nAcao: E preciso ter 3 pecas na fila E 3 na pilha para a troca multipla.\n");
                } else {
                    for (int i = 0; i < 3; i++) {
                        // Calcula o índice na fila circular
                        int idx_fila = (filaDePecas.inicio + i) % FILA_MAX;
                        // O índice na pilha é mais simples
                        int idx_pilha = pilhaDeReserva.topo - i;

                        // Troca
                        Peca temp = filaDePecas.itens[idx_fila];
                        filaDePecas.itens[idx_fila] = pilhaDeReserva.itens[idx_pilha];
                        pilhaDeReserva.itens[idx_pilha] = temp;
                    }
                    printf("\nAcao: Troca realizada entre os 3 primeiros da fila e os 3 da pilha.\n");
                }
                break;

            case 0: // Sair
                printf("\nEncerrando o jogo Tetris Stack. Ate a proxima!\n");
                break;

            default:
                printf("\nOpcao invalida. Tente novamente.\n");
                break;
        }

    } while (opcao != 0);

    return 0;
}