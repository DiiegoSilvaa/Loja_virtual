#include <stdio.h>   // printf, fopen, fgets...
#include <stdlib.h>  // biblioteca padrao
#include <string.h>  // funcoes de texto (strcpy, strcmp...)
#include <ctype.h>   // tolower e isspace

#define MAX 500                  // maximo de produtos na memoria
#define ARQ "produtos.txt"       // arquivo onde ficam os dados
#define TMP "temp.txt"           // arquivo temporario
#define LINHA "---------------------------------------------------------------------------------------------------"

typedef struct {
    int id;                      // codigo do produto
    char nome[50];               // nome
    char categoria[30];          // categoria
    float preco;                 // preco em reais
    int quantidade;              // quantidade no estoque
    char fornecedor[50];         // fornecedor
} Produto;

// copia o texto 'o' para 'd' tudo em minusculo (para comparar sem diferenciar maiuscula)
void minusculo(const char *o, char *d) {
    for (; *o; o++, d++)                     // anda pelo texto com ponteiro
        *d = tolower((unsigned char)*o);     // passa cada letra para minuscula
    *d = '\0';                               // fecha o texto
}

// diz se 'trecho' aparece dentro de 'texto'
int contem(const char *texto, const char *trecho) {
    char a[100], b[100];                     // copias em minusculo
    minusculo(texto, a);                     // texto em minusculo
    minusculo(trecho, b);                    // trecho em minusculo
    return strstr(a, b) != NULL;             // strstr procura um dentro do outro
}

// le um texto com espacos
void lerTexto(const char *msg, char *dest, int tam) {
    char buf[200], *p, *fim;                 // buf guarda o que foi digitado
    printf("%s", msg);                       // mostra a pergunta
    if (!fgets(buf, sizeof buf, stdin))      // le a linha inteira
        buf[0] = '\0';                       // se der erro fica vazio
    for (p = buf; *p; p++)                   // varre o texto
        if (*p == ';' || *p == '\n' || *p == '\r')
            *p = ' ';                        // tira o enter e o ';' (que separa os campos no arquivo)
    fim = buf + strlen(buf);                 // fim aponta pro final do texto
    while (fim > buf && isspace((unsigned char)fim[-1]))
        fim--;                               // volta enquanto for espaco
    *fim = '\0';                             // corta os espacos do final
    for (p = buf; isspace((unsigned char)*p); p++);  // pula os espacos do comeco
    strncpy(dest, p, tam - 1);               // copia sem passar do tamanho do campo
    dest[tam - 1] = '\0';                    // garante o fim do texto
}

// le um numero inteiro (repete ate digitar certo)
int lerInt(const char *msg) {
    char buf[100];
    int v;
    while (1) {                              // repete ate acertar
        printf("%s", msg);
        if (fgets(buf, sizeof buf, stdin) && sscanf(buf, "%d", &v) == 1 && v >= 0)
            return v;                        // numero valido
        puts("Valor invalido!");
    }
}

// le um numero com virgula ou ponto
float lerReal(const char *msg) {
    char buf[100], *p;
    float v;
    while (1) {
        printf("%s", msg);
        if (fgets(buf, sizeof buf, stdin)) {
            for (p = buf; *p; p++)
                if (*p == ',') *p = '.';     // troca virgula por ponto
            if (sscanf(buf, "%f", &v) == 1 && v >= 0)
                return v;                    // numero valido
        }
        puts("Valor invalido!");
    }
}

// grava um produto no arquivo
void gravar(FILE *f, const Produto *p) {
    fprintf(f, "%d;%s;%s;%.2f;%d;%s\n",
            p->id, p->nome, p->categoria, p->preco, p->quantidade, p->fornecedor);
}

// le o arquivo e guarda tudo no vetor. Devolve quantos produtos leu (-1 se nao existe)
int carregar(Produto *l) {
    FILE *f = fopen(ARQ, "r");               // abre pra leitura
    char linha[300];
    int n = 0;                               // contador de produtos
    if (!f) return -1;                       // arquivo nao existe
    while (n < MAX && fgets(linha, sizeof linha, f))   // le linha por linha
        if (sscanf(linha, "%d;%49[^;];%29[^;];%f;%d;%49[^\n\r]",
                   &l[n].id, l[n].nome, l[n].categoria,
                   &l[n].preco, &l[n].quantidade, l[n].fornecedor) == 6)
            n++;                             // so conta se leu os 6 campos
    fclose(f);                               // fecha o arquivo
    return n;
}

// carrega os produtos e avisa se o arquivo ainda nao foi criado
int abrir(Produto *l) {
    int n = carregar(l);
    if (n < 0) puts("Arquivo nao encontrado! Use a opcao 1.");
    return n;
}

// procura um produto pelo id e devolve o endereco dele (ou NULL)
Produto *buscarId(Produto *l, int n, int id) {
    Produto *p;
    for (p = l; p < l + n; p++)              // ponteiro vai de l ate l+n
        if (p->id == id) return p;           // achou
    return NULL;                             // nao achou
}

// verifica se ja existe um produto com esse nome
int nomeExiste(Produto *l, int n, const char *nome, int ignorar) {
    Produto *p;
    char a[50], b[50];
    minusculo(nome, a);                      // nome digitado em minusculo
    for (p = l; p < l + n; p++) {
        minusculo(p->nome, b);               // nome do cadastro em minusculo
        if (p->id != ignorar && strcmp(a, b) == 0)
            return 1;                        // nomes iguais
    }
    return 0;
}

// mostra os produtos em forma de tabela
void tabela(const Produto *l, int n) {
    const Produto *p;
    puts(LINHA);
    printf("| %-3s | %-28s | %-16s | %10s | %5s | %-18s |\n",
           "ID", "Produto", "Categoria", "Preco (R$)", "Qtd", "Fornecedor");
    puts(LINHA);
    for (p = l; p < l + n; p++)              // uma linha pra cada produto
        printf("| %-3d | %-28.28s | %-16.16s | %10.2f | %5d | %-18.18s |\n",
               p->id, p->nome, p->categoria, p->preco, p->quantidade, p->fornecedor);
    puts(LINHA);
    printf("Total de produtos: %d\n", n);
}

// 1 - cria o arquivo ja com os 4 produtos da tabela do enunciado
void criar(void) {
    Produto ini[4] = {
        {1, "Camiseta Basica",  "Vestuario",       49.90f, 30, "Moda Brasil"},
        {2, "Mouse Optico USB", "Informatica",     59.99f, 15, "TecnoShop"},
        {3, "Shampoo 300ml",    "Higiene Pessoal", 22.50f, 40, "Beleza Pura"},
        {4, "Cafe 500g",        "Alimentos",       18.00f, 25, "Graos do Sul"}};
    char r[10];
    FILE *f = fopen(ARQ, "r");               // testa se o arquivo ja existe
    int i;
    if (f) {                                 // se existe, pergunta antes de apagar
        fclose(f);
        lerTexto("O arquivo ja existe. Apagar e criar de novo? (s/n): ", r, 10);
        if (tolower(r[0]) != 's') return;    // nao confirmou
    }
    f = fopen(ARQ, "w");                     // "w" cria o arquivo do zero
    if (!f) { puts("Erro ao criar o arquivo!"); return; }
    for (i = 0; i < 4; i++)
        gravar(f, &ini[i]);                  // grava os 4 produtos
    fclose(f);
    puts("Arquivo criado com 4 produtos.");
}

// 2 - lista todos os produtos
void listar(Produto *l) {
    int n = abrir(l);                        // carrega o arquivo
    if (n == 0) puts("Nenhum produto cadastrado.");
    else if (n > 0) tabela(l, n);            // mostra a tabela
}

// 3 - cadastra um produto novo
void inserir(Produto *l) {
    int n = abrir(l), i;
    Produto p;                               // produto novo
    FILE *f;
    if (n < 0) return;
    lerTexto("Nome: ", p.nome, 50);          // le o nome
    if (!*p.nome || nomeExiste(l, n, p.nome, 0)) {   // vazio ou repetido
        puts("Nome vazio ou produto ja cadastrado!");
        return;
    }
    lerTexto("Categoria: ", p.categoria, 30);        // le os outros campos
    p.preco = lerReal("Preco: ");
    p.quantidade = lerInt("Quantidade: ");
    lerTexto("Fornecedor: ", p.fornecedor, 50);
    p.id = 1;                                // id automatico: maior id + 1
    for (i = 0; i < n; i++)
        if (l[i].id >= p.id) p.id = l[i].id + 1;
    while (buscarId(l, n, p.id)) p.id++;     // garante que o id nao repete
    f = fopen(ARQ, "a");                     // "a" adiciona no final do arquivo
    if (!f) { puts("Erro ao abrir o arquivo!"); return; }
    gravar(f, &p);                           // grava o produto
    fclose(f);
    printf("Produto cadastrado! ID gerado: %d\n", p.id);
}

// grava tudo em temp.txt (sem o produto 'id' ou com ele alterado) e troca pelo original
void regravar(Produto *l, int n, int id, const Produto *novo) {
    FILE *f = fopen(TMP, "w");               // cria o arquivo temporario
    Produto *p;
    if (!f) { puts("Erro ao criar o arquivo temporario!"); return; }
    for (p = l; p < l + n; p++) {
        if (p->id != id) gravar(f, p);       // os outros produtos ficam iguais
        else if (novo) gravar(f, novo);      // o escolhido vai alterado (na exclusao nao grava)
    }
    fclose(f);
    remove(ARQ);                             // apaga o arquivo antigo
    rename(TMP, ARQ);                        // o temporario vira o arquivo principal
}

// 4 - altera um produto pelo id
void alterar(Produto *l) {
    int n = abrir(l), id, q;
    Produto *a, e;                           // a = produto achado, e = copia editada
    char t[50], *p;
    float v;
    if (n < 0) return;
    id = lerInt("ID do produto: ");
    a = buscarId(l, n, id);                  // procura o produto
    if (!a) { puts("Produto nao encontrado!"); return; }
    e = *a;                                  // copia os dados atuais
    puts("Aperte ENTER para manter o valor atual.");
    lerTexto("Novo nome: ", t, 50);
    if (*t) {                                // so muda se digitou algo
        if (nomeExiste(l, n, t, id)) { puts("Ja existe produto com esse nome!"); return; }
        strcpy(e.nome, t);
    }
    lerTexto("Nova categoria: ", t, 30);
    if (*t) strcpy(e.categoria, t);
    lerTexto("Novo preco: ", t, 50);
    for (p = t; *p; p++)
        if (*p == ',') *p = '.';             // aceita virgula no preco
    if (*t && sscanf(t, "%f", &v) == 1 && v >= 0) e.preco = v;
    lerTexto("Nova quantidade: ", t, 50);
    if (*t && sscanf(t, "%d", &q) == 1 && q >= 0) e.quantidade = q;
    lerTexto("Novo fornecedor: ", t, 50);
    if (*t) strcpy(e.fornecedor, t);
    regravar(l, n, id, &e);                  // salva usando o arquivo temporario
    puts("Produto alterado!");
}

// 5 - exclui um produto pelo id
void excluir(Produto *l) {
    int n = abrir(l), id;
    char r[10];
    Produto *a;
    if (n < 0) return;
    id = lerInt("ID do produto: ");
    a = buscarId(l, n, id);                  // procura o produto
    if (!a) { puts("Produto nao encontrado!"); return; }
    printf("Produto: %s\n", a->nome);
    lerTexto("Confirma a exclusao? (s/n): ", r, 10);
    if (tolower(r[0]) != 's') return;        // cancelou
    regravar(l, n, id, NULL);                // NULL = nao grava esse produto
    puts("Produto excluido!");
}

// 6 e 7 - busca por nome (porNome = 1) ou por categoria (porNome = 0)
void buscar(Produto *l, int porNome) {
    Produto achados[MAX], *p;                // achados guarda o resultado
    char termo[50];
    int n = abrir(l), k = 0;                 // k = quantos achou
    if (n < 0) return;
    lerTexto(porNome ? "Nome: " : "Categoria: ", termo, 50);
    for (p = l; p < l + n; p++)
        if (contem(porNome ? p->nome : p->categoria, termo))
            achados[k++] = *p;               // guarda se contem o texto
    if (k) tabela(achados, k);
    else puts("Nenhum produto encontrado.");
}

// troca dois produtos de lugar usando ponteiros
void trocar(Produto *a, Produto *b) {
    Produto t = *a;                          // guarda o a
    *a = *b;                                 // a recebe o b
    *b = t;                                  // b recebe o antigo a
}

// diz se 'a' e 'b' estao na ordem errada. c: 1 nome A-Z, 2 nome Z-A, 3 preco menor, 4 preco maior
int deveTrocar(const Produto *a, const Produto *b, int c) {
    char x[50], y[50];
    minusculo(a->nome, x);                   // nomes em minusculo
    minusculo(b->nome, y);
    switch (c) {
        case 1: return strcmp(x, y) > 0;     // A-Z
        case 2: return strcmp(x, y) < 0;     // Z-A
        case 3: return a->preco > b->preco;  // menor -> maior
        default: return a->preco < b->preco; // maior -> menor
    }
}

// 8 a 11 - ordena na memoria com Bubble Sort e mostra (o arquivo nao muda)
void ordenar(Produto *l, int c) {
    int n = abrir(l), i, j;
    if (n <= 0) return;
    for (i = 0; i < n - 1; i++)              // varias passadas
        for (j = 0; j < n - i - 1; j++)      // compara os vizinhos
            if (deveTrocar(l + j, l + j + 1, c))
                trocar(l + j, l + j + 1);    // troca se estiver fora de ordem
    tabela(l, n);
}

// 12 - exporta para CSV (separado por virgula)
void exportar(Produto *l) {
    int n = abrir(l);
    Produto *p;
    FILE *f;
    if (n < 0) return;
    f = fopen("produtos.csv", "w");          // cria o csv
    if (!f) { puts("Erro ao criar o CSV!"); return; }
    fprintf(f, "ID,Produto,Categoria,Preco,Quantidade,Fornecedor\n");   // cabecalho
    for (p = l; p < l + n; p++)              // uma linha por produto
        fprintf(f, "%d,\"%s\",\"%s\",%.2f,%d,\"%s\"\n",
                p->id, p->nome, p->categoria, p->preco, p->quantidade, p->fornecedor);
    fclose(f);
    puts("Dados exportados para produtos.csv");
}

int main(void) {
    Produto lista[MAX];                      // vetor de produtos
    int op;                                  // opcao do menu
    do {
        printf("\n===== CADASTRO DE LOJA =====\n"
               " 1 - Criar arquivo\n 2 - Listar\n 3 - Inserir\n 4 - Alterar\n 5 - Excluir\n"
               " 6 - Buscar por nome\n 7 - Buscar por categoria\n"
               " 8 - Ordenar por nome (A-Z)\n 9 - Ordenar por nome (Z-A)\n"
               "10 - Ordenar por preco (menor -> maior)\n"
               "11 - Ordenar por preco (maior -> menor)\n"
               "12 - Exportar para CSV\n 0 - Sair\n");
        op = lerInt("Opcao: ");              // le a opcao
        switch (op) {                        // chama a funcao da opcao
            case 1:  criar(); break;
            case 2:  listar(lista); break;
            case 3:  inserir(lista); break;
            case 4:  alterar(lista); break;
            case 5:  excluir(lista); break;
            case 6:  buscar(lista, 1); break;
            case 7:  buscar(lista, 0); break;
            case 8: case 9: case 10: case 11:
                ordenar(lista, op - 7); break;   // op-7 vira o criterio 1 a 4
            case 12: exportar(lista); break;
            case 0:  puts("Ate logo!"); break;
            default: puts("Opcao invalida!");
        }
    } while (op != 0);                       // repete ate escolher 0
    return 0;
}
