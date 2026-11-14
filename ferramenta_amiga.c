#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <errno.h>
#include <limits.h>

#define CAPACIDADE 100
#define TEXTO 81
#define CODIGO 17

typedef struct {
    char codigo[CODIGO];
    char nome[TEXTO];
    char categoria[TEXTO];
    int total;
    int emprestadas;
} Ferramenta;

static int lerLinha(const char *mensagem, char *destino, size_t tamanho) {
    size_t n, inicio, fim;
    int ch;
    for (;;) {
        printf("%s", mensagem);
        if (!fgets(destino, (int)tamanho, stdin)) return 0;
        n = strlen(destino);
        if (n > 0 && destino[n - 1] == '\n') destino[--n] = '\0';
        else {
            ch = getchar();
            if (ch != '\n' && ch != EOF) {
                while ((ch = getchar()) != '\n' && ch != EOF) { }
                printf("Entrada longa demais. Maximo: %zu caracteres.\n", tamanho - 1);
                continue;
            }
        }
        inicio = 0;
        while (destino[inicio] && isspace((unsigned char)destino[inicio])) inicio++;
        fim = strlen(destino);
        while (fim > inicio && isspace((unsigned char)destino[fim - 1])) fim--;
        memmove(destino, destino + inicio, fim - inicio);
        destino[fim - inicio] = '\0';
        if (*destino) return 1;
        puts("Campo obrigatorio. Tente novamente.");
    }
}

static int lerInteiro(const char *mensagem, int minimo, int maximo, int *valor) {
    char linha[128], *fim;
    long numero;
    for (;;) {
        if (!lerLinha(mensagem, linha, sizeof linha)) return 0;
        errno = 0;
        numero = strtol(linha, &fim, 10);
        if (errno == 0 && fim != linha && *fim == '\0' &&
            numero >= minimo && numero <= maximo) {
            *valor = (int)numero;
            return 1;
        }
        printf("Informe um inteiro entre %d e %d.\n", minimo, maximo);
    }
}

static int iguaisSemCaixa(const char *a, const char *b) {
    while (*a && *b) {
        if (tolower((unsigned char)*a) != tolower((unsigned char)*b)) return 0;
        a++; b++;
    }
    return *a == *b;
}

static int contemTexto(const char *texto, const char *termo) {
    size_t i, j;
    for (i = 0; texto[i]; i++) {
        for (j = 0; termo[j] && texto[i + j]; j++) {
            if (tolower((unsigned char)texto[i + j]) !=
                tolower((unsigned char)termo[j])) break;
        }
        if (!termo[j]) return 1;
    }
    return 0;
}

static int buscarCodigo(const Ferramenta itens[], int quantidade, const char *codigo) {
    int i;
    for (i = 0; i < quantidade; i++)
        if (iguaisSemCaixa(itens[i].codigo, codigo)) return i;
    return -1;
}

static void exibir(const Ferramenta *item) {
    printf("Codigo: %s\nNome: %s\nCategoria: %s\n", item->codigo, item->nome, item->categoria);
    printf("Total: %d | Emprestadas: %d | Disponiveis: %d\n",
           item->total, item->emprestadas, item->total - item->emprestadas);
}

static int cadastrar(Ferramenta itens[], int *quantidade) {
    Ferramenta novo;
    if (*quantidade == CAPACIDADE) {
        puts("Limite de 100 registros atingido."); return 1;
    }
    for (;;) {
        if (!lerLinha("Codigo (ate 16 caracteres): ", novo.codigo, sizeof novo.codigo)) return 0;
        if (buscarCodigo(itens, *quantidade, novo.codigo) < 0) break;
        puts("Codigo duplicado. Informe outro codigo.");
    }
    if (!lerLinha("Nome: ", novo.nome, sizeof novo.nome) ||
        !lerLinha("Categoria: ", novo.categoria, sizeof novo.categoria) ||
        !lerInteiro("Quantidade total (1 a 10000): ", 1, 10000, &novo.total)) return 0;
    novo.emprestadas = 0;
    itens[*quantidade] = novo;
    (*quantidade)++;
    puts("Ferramenta cadastrada.");
    return 1;
}

static void listar(const Ferramenta itens[], int quantidade) {
    int i;
    if (!quantidade) { puts("Nenhuma ferramenta cadastrada."); return; }
    for (i = 0; i < quantidade; i++) {
        printf("\n--- Registro %d ---\n", i + 1); exibir(&itens[i]);
    }
}

static int pesquisar(const Ferramenta itens[], int quantidade) {
    char termo[TEXTO];
    int tipo, i, encontrados = 0;
    if (!quantidade) { puts("Nenhuma ferramenta cadastrada."); return 1; }
    if (!lerInteiro("Buscar por 1-Codigo ou 2-Nome: ", 1, 2, &tipo) ||
        !lerLinha("Termo: ", termo, sizeof termo)) return 0;
    for (i = 0; i < quantidade; i++) {
        if ((tipo == 1 && iguaisSemCaixa(itens[i].codigo, termo)) ||
            (tipo == 2 && contemTexto(itens[i].nome, termo))) {
            exibir(&itens[i]); encontrados++;
        }
    }
    printf("Resultados encontrados: %d\n", encontrados);
    return 1;
}

static int movimentar(Ferramenta itens[], int quantidade, int devolucao) {
    char codigo[CODIGO];
    int indice, limite, unidades;
    if (!quantidade) { puts("Nenhuma ferramenta cadastrada."); return 1; }
    if (!lerLinha("Codigo da ferramenta: ", codigo, sizeof codigo)) return 0;
    indice = buscarCodigo(itens, quantidade, codigo);
    if (indice < 0) { puts("Registro inexistente."); return 1; }
    limite = devolucao ? itens[indice].emprestadas : itens[indice].total - itens[indice].emprestadas;
    if (limite == 0) {
        puts(devolucao ? "Nao ha unidades emprestadas para devolver." : "Nao ha unidades disponiveis.");
        return 1;
    }
    printf("Quantidade maxima nesta operacao: %d\n", limite);
    if (!lerInteiro("Quantidade: ", 1, limite, &unidades)) return 0;
    if (devolucao) itens[indice].emprestadas -= unidades;
    else itens[indice].emprestadas += unidades;
    puts(devolucao ? "Devolucao registrada." : "Emprestimo registrado.");
    exibir(&itens[indice]);
    return 1;
}

static void resumo(const Ferramenta itens[], int quantidade) {
    int i, total = 0, emprestadas = 0;
    for (i = 0; i < quantidade; i++) {
        total += itens[i].total; emprestadas += itens[i].emprestadas;
    }
    printf("Tipos cadastrados: %d\nUnidades totais: %d\n", quantidade, total);
    printf("Emprestadas: %d\nDisponiveis: %d\n", emprestadas, total - emprestadas);
    if (total) printf("Percentual emprestado: %.1f%%\n", 100.0 * emprestadas / total);
    else puts("Percentual emprestado: nao calculado (acervo vazio).");
}

int main(void) {
    Ferramenta itens[CAPACIDADE];
    int quantidade = 0, opcao, continuar = 1;
    puts("FERRAMENTA AMIGA - demonstracao academica com dados ficticios.");
    puts("Dados apenas em memoria. Nada e salvo ao encerrar.");
    while (continuar) {
        puts("\n1-Cadastrar ferramenta\n2-Listar acervo\n3-Pesquisar ferramenta");
        puts("4-Registrar emprestimo\n5-Registrar devolucao\n6-Exibir resumo\n0-Sair");
        if (!lerInteiro("Opcao: ", 0, 6, &opcao)) break;
        switch (opcao) {
            case 0: continuar = 0; break;
            case 1: continuar = cadastrar(itens, &quantidade); break;
            case 2: listar(itens, quantidade); break;
            case 3: continuar = pesquisar(itens, quantidade); break;
            case 4: continuar = movimentar(itens, quantidade, 0); break;
            case 5: continuar = movimentar(itens, quantidade, 1); break;
            case 6: resumo(itens, quantidade); break;
        }
    }
    puts("Programa encerrado. Os dados em memoria foram descartados.");
    return 0;
}
