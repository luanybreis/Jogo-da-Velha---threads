#include <stdio.h>
#include <pthread.h>

#define TAM 3

char tabuleiro[TAM][TAM] = {
    {'x', 'o', 'o'}, // o tabuleiro pode ser alterado para testes
    {'x', 'x', 'x'}, 
    {'o', 'x', 'o'}
};
int vencedor = 0; // 0 é empate, 1 para jogador 1(x), 2 para jogador 2(o)
pthread_mutex_t mutex_vencedor;

void* verificar_linha(void* arg) {
    for (int i = 0; i < TAM; i++) {
        if (tabuleiro[i][0] == tabuleiro[i][1] && tabuleiro[i][1] == tabuleiro[i][2]) {
            pthread_mutex_lock(&mutex_vencedor);
            if (tabuleiro[i][0] == 'x') {
                vencedor = 1;
            } else {
                vencedor = 2;
            }
            pthread_mutex_unlock(&mutex_vencedor);
        }
    }
    pthread_exit(NULL); //finalizar a thread
}

void* verificar_coluna(void* arg) {
    for (int i = 0; i < TAM; i++) {
        if (tabuleiro[0][i] == tabuleiro[1][i] && tabuleiro[1][i] == tabuleiro[2][i]) {
            pthread_mutex_lock(&mutex_vencedor); // Bloqueia o mutex
            if (tabuleiro[0][i] == 'x') {
                vencedor = 1;
            } else {
                vencedor = 2;
            }
            pthread_mutex_unlock(&mutex_vencedor);  // Desbloqueia o mutex
        }
    }
    pthread_exit(NULL);//finaliza a thread
}

void* verificar_diagonal(void* arg) {
    // Diagonal principal
    if(tabuleiro[0][0] == tabuleiro[1][1] && tabuleiro[1][1] == tabuleiro[2][2]) {
        pthread_mutex_lock(&mutex_vencedor); // Bloqueia o mutex
        if(tabuleiro[0][0] == 'x') {
            vencedor = 1;
        } else {
            vencedor = 2;
        }
        pthread_mutex_unlock(&mutex_vencedor); // Desbloqueia o mutex
    }
     
    // Diagonal secundária
    if(tabuleiro[0][2] == tabuleiro[1][1] && tabuleiro[1][1] == tabuleiro[2][0]) {
        pthread_mutex_lock(&mutex_vencedor); // Bloqueia o mutex
        if(tabuleiro[0][2] == 'x') {
            vencedor = 1;
        } else {
            vencedor = 2;
        }
        pthread_mutex_unlock(&mutex_vencedor);  // Desbloqueia o mutex
    }
    pthread_exit(NULL); //finaliza a thread
}

int main() {
    pthread_t threads[3]; // Declaração de 3 threads

    pthread_mutex_init(&mutex_vencedor, NULL); // Inicializa o mutex

    // Criar as threads para verificar linhas, colunas e diagonais
    pthread_create(&threads[0], NULL, verificar_linha, NULL);
    pthread_create(&threads[1], NULL, verificar_coluna, NULL);
    pthread_create(&threads[2], NULL, verificar_diagonal, NULL);

    // Aguardar a conclusão das três threads
    for (int i = 0; i < 3; i++) {
        pthread_join(threads[i], NULL);
    }

    // Verificar o resultado do jogo
    if (vencedor == 1) {
        printf("O jogador 1 venceu!\n");
    } else if (vencedor == 2) {
        printf("O jogador 2 venceu!\n");
    } else {
        printf("Deu velha!\n");
    }

    pthread_mutex_destroy(&mutex_vencedor); // Destrói o mutex

    return 0;
}
