/**
 * @file main.c
 * @author Pedro Seabra Dornellas
 * @brief Código para conferrir a sintaxe de um código em C.
 * @version 1.9
 * @date 2024-07-18
 *
 * Este código foi desenvolvido para a disciplina de Algorítmo e Estrutura de Dados II, do curso de Ciências da Computação, da Universidade Federal de Goiás.
 *
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/**
 * @brief Estrutura para armazenar os erros encontrados.
 */
typedef struct Erro
{
    char mensagem[100];   /**< Mensagem de erro. */
    struct Erro *proximo; /**< Ponteiro para o próximo erro. */
} Erro;

Erro *lista_erros = NULL;

/**
 * @brief Adiciona um erro à lista de erros.
 *
 * @param mensagem A mensagem de erro a ser adicionada.
 */
void adicionar_erro(char *mensagem)
{
    Erro *novo_erro = (Erro *)malloc(sizeof(Erro));
    strcpy(novo_erro->mensagem, mensagem);
    novo_erro->proximo = lista_erros;
    lista_erros = novo_erro;
}

/**
 * @brief Exibe todos os erros armazenados.
 */
void exibir_erros()
{
    Erro *atual = lista_erros;
    while (atual != NULL)
    {
        printf("%s\n", atual->mensagem);
        atual = atual->proximo;
    }
}

/**
 * @brief Limpa todos os erros armazenados.
 */
void limpar_erros()
{
    Erro *atual = lista_erros;
    while (atual != NULL)
    {
        Erro *prox = atual->proximo;
        free(atual);
        atual = prox;
    }
    lista_erros = NULL;
}

char *palavras_reservadas[] = {
    "include", "main", "printf", "scanf", "if", "else", "for", "while", "switch", "case"};

/**
 * @brief Verifica se uma palavra é reservada.
 *
 * @param palavra A palavra a ser verificada.
 * @return int 1 se a palavra é reservada, 0 caso contrário.
 */
int eh_palavra_reservada(char *palavra)
{
    for (int i = 0; i < sizeof(palavras_reservadas) / sizeof(char *); i++)
    {
        if (strcmp(palavra, palavras_reservadas[i]) == 0)
        {
            return 1;
        }
    }
    return 0;
}

typedef struct No
{
    char dado;          /**< Dado armazenado no nó. */
    struct No *proximo; /**< Ponteiro para o próximo nó. */
} No;

typedef struct Pilha
{
    No *topo; /**< Ponteiro para o topo da pilha. */
} Pilha;

/**
 * @brief Cria uma nova pilha.
 *
 * @return Pilha* Ponteiro para a nova pilha criada.
 */
Pilha *criar_pilha()
{
    Pilha *pilha = (Pilha *)malloc(sizeof(Pilha));
    pilha->topo = NULL;
    return pilha;
}

/**
 * @brief Empilha um caractere na pilha.
 *
 * @param pilha Ponteiro para a pilha.
 * @param c Caractere a ser empilhado.
 */
void empilhar(Pilha *pilha, char c)
{
    No *novo = (No *)malloc(sizeof(No));
    novo->dado = c;
    novo->proximo = pilha->topo;
    pilha->topo = novo;
}

/**
 * @brief Desempilha um caractere da pilha.
 *
 * @param pilha Ponteiro para a pilha.
 * @param c Caractere esperado no topo da pilha.
 * @return int 1 se o caractere foi desempilhado, 0 caso contrário.
 */
int desempilhar(Pilha *pilha, char c)
{
    if (pilha->topo == NULL)
        return 0;
    No *topo = pilha->topo;
    pilha->topo = topo->proximo;
    free(topo);
    return 1;
}

/**
 * @brief Verifica se a pilha está vazia.
 *
 * @param pilha Ponteiro para a pilha.
 * @return int 1 se a pilha está vazia, 0 caso contrário.
 */
int pilha_vazia(Pilha *pilha)
{
    return pilha->topo == NULL;
}

/**
 * @brief Destrói a pilha, liberando toda a memória alocada.
 *
 * @param pilha Ponteiro para a pilha.
 */
void destruir_pilha(Pilha *pilha)
{
    while (!pilha_vazia(pilha))
    {
        desempilhar(pilha, pilha->topo->dado);
    }
    free(pilha);
}

/**
 * @brief Verifica a correção dos blocos no código.
 *
 * @param codigo O código a ser verificado.
 */
void verificar_blocos(char *codigo)
{
    Pilha *pilha = criar_pilha();
    for (int i = 0; i < strlen(codigo); i++)
    {
        char c = codigo[i];
        if (c == '{' || c == '[' || c == '(' || c == '\"' || c == '\'')
        {
            empilhar(pilha, c);
        }
        else if (c == '}' || c == ']' || c == ')')
        {
            if (!desempilhar(pilha, c))
            {
                adicionar_erro("Erro: Bloco nao balanceado");
            }
        }
    }
    if (!pilha_vazia(pilha))
    {
        adicionar_erro("Erro: Bloco nao balanceado");
    }
    destruir_pilha(pilha);
}

/**
 * @brief Verifica a sintaxe dos comandos no código.
 *
 * @param codigo O código a ser verificado.
 */
void verificar_sintaxe(char *codigo)
{
    if (strstr(codigo, "#include") == NULL)
    {
        adicionar_erro("Erro: Faltando diretiva #include");
    }
    if (strstr(codigo, "int main") == NULL)
    {
        adicionar_erro("Erro: Faltando funcao main");
    }
}

/**
 * @brief Verifica a presença de expressões no código.
 *
 * @param codigo O código a ser verificado.
 */
void verificar_expressoes(char *codigo)
{
    if (strstr(codigo, "printf") == NULL)
    {
        adicionar_erro("Erro: Faltando comando printf");
    }
    if (strstr(codigo, "scanf") == NULL)
    {
        adicionar_erro("Erro: Faltando comando scanf");
    }
}

/**
 * @brief Lê o conteúdo de um arquivo e retorna como uma string.
 *
 * @param nome_arquivo O nome do arquivo a ser lido.
 * @return char* O conteúdo do arquivo como uma string.
 */
char *ler_arquivo(const char *nome_arquivo)
{
    FILE *arquivo = fopen(nome_arquivo, "r");
    if (arquivo == NULL)
    {
        adicionar_erro("Erro: Nao foi possivel abrir o arquivo");
        return NULL;
    }
    fseek(arquivo, 0, SEEK_END);
    long tamanho = ftell(arquivo);
    fseek(arquivo, 0, SEEK_SET);

    char *conteudo = (char *)malloc(tamanho + 1);
    fread(conteudo, 1, tamanho, arquivo);
    conteudo[tamanho] = '\0';

    fclose(arquivo);
    return conteudo;
}

/**
 * @brief Exibe o menu de opções para o usuário.
 */
void exibir_menu()
{
    int opcao;
    do
    {
        printf("\nMenu:\n");
        printf("1. Verificar Blocos\n");
        printf("2. Verificar Sintaxe\n");
        printf("3. Verificar Expressoes\n");
        printf("4. Exibir Erros\n");
        printf("5. Sair\n");
        printf("Escolha uma opcao: ");
        scanf("%d", &opcao);

        char nome_arquivo[100];
        char *codigo;
        switch (opcao)
        {
        case 1:
            printf("Digite o nome do arquivo a ser verificado:\n");
            scanf("%s", nome_arquivo);
            codigo = ler_arquivo(nome_arquivo);
            if (codigo != NULL)
            {
                verificar_blocos(codigo);
                free(codigo);
            }
            break;
        case 2:
            printf("Digite o nome do arquivo a ser verificado:\n");
            scanf("%s", nome_arquivo);
            codigo = ler_arquivo(nome_arquivo);
            if (codigo != NULL)
            {
                verificar_sintaxe(codigo);
                free(codigo);
            }
            break;
        case 3:
            printf("Digite o nome do arquivo a ser verificado:\n");
            scanf("%s", nome_arquivo);
            codigo = ler_arquivo(nome_arquivo);
            if (codigo != NULL)
            {
                verificar_expressoes(codigo);
                free(codigo);
            }
            break;
        case 4:
            exibir_erros();
            break;
        case 5:
            printf("Saindo...\n");
            break;
        default:
            printf("Opcao invalida!\n");
        }
    } while (opcao != 5);
}

/**
 * @brief Função principal do programa.
 *
 * @return int Código de retorno.
 */
int main()
{
    printf("Bem-vindo ao Editor/Interpretador\n");
    exibir_menu();
    return 0;
}
