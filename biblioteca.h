#ifndef BIBLIOTECA_H
#define BIBLIOTECA_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_STRING 100
#define CAPACIDADE_INICIAL 10

typedef struct {
    char *titulo;
    char *autor;
    int ano_publicacao;
    int lido;
    int id;
} Livro;

typedef struct {
    Livro **livros;
    int quantidade;
    int capacidade;
    int proximo_id;
    char *nome_arquivo;
} Biblioteca;

// Protótipos de funções
Biblioteca* criar_biblioteca();
void adicionar_livro(Biblioteca *b);
void listar_livros(Biblioteca *b);
void salvar_biblioteca(Biblioteca *b);
Biblioteca* carregar_arquivo(const char* arquivo);
Livro* buscar_livros(Biblioteca *b, int id);
void destruir_biblioteca(Biblioteca* b);
int remover_livro(Biblioteca* b, int id);
void executar_menu(Biblioteca* b);
void marcar_lido(Biblioteca* b);
Livro* busca_por_autor(Biblioteca* b);


Biblioteca* criar_biblioteca() {
    Biblioteca *b = malloc(sizeof(Biblioteca));
    if (b == NULL) {
        return NULL;
    }

    b->quantidade = 0;
    b->capacidade = CAPACIDADE_INICIAL;
    b->livros = malloc(CAPACIDADE_INICIAL * sizeof(Livro*));
    b->proximo_id = 1;
    b->nome_arquivo = NULL;
    
    return b;
}

void adicionar_livro(Biblioteca *b) {
    if(b->quantidade == b->capacidade) {
        int nova_capacidade = b->capacidade * 2;
        Livro** novoarray = realloc(b->livros, (nova_capacidade * sizeof(Livro*)));
        
        if(novoarray == NULL) {
            printf("Erro ao expandir memoria!!\n");
            return;
        }
        
        b->livros = novoarray;
        b->capacidade = nova_capacidade;
    }
    
    Livro *novo = malloc(sizeof(Livro));
    novo->id = b->proximo_id;
    b->proximo_id++;
    
    printf("Digite o Nome do Livro: ");
    while (getchar() != '\n');
    
    char buffer_title[MAX_STRING];
    fgets(buffer_title, MAX_STRING, stdin);
    buffer_title[strcspn(buffer_title, "\n")] = '\0';
    novo->titulo = malloc(strlen(buffer_title) + 1);
    strcpy(novo->titulo, buffer_title);
    
    printf("Digite o Nome do Autor: ");
    char buffer_autor[MAX_STRING];
    fgets(buffer_autor, MAX_STRING, stdin);
    buffer_autor[strcspn(buffer_autor, "\n")] = '\0';
    novo->autor = malloc(strlen(buffer_autor) + 1);
    strcpy(novo->autor, buffer_autor);
    
    printf("Digite o Ano de Publicação: ");
    scanf("%d", &(novo->ano_publicacao));
    while (getchar() != '\n');
    
    novo->lido = 0;
    b->livros[b->quantidade] = novo;
    b->quantidade++;
}

void listar_livros(Biblioteca *b) {
    if (b->quantidade == 0) {
        printf("Biblioteca vazia!\n");
        return;
    }
    
    for(int i = 0; i < b->quantidade; i++) {
        printf("ID: %d\n", b->livros[i]->id);
        printf("Título: %s\n", b->livros[i]->titulo);
        printf("Autor: %s\n", b->livros[i]->autor);
        printf("Ano de Publicação: %d\n", b->livros[i]->ano_publicacao);
        printf("Lido: %s\n", b->livros[i]->lido ? "Sim" : "Não");
        printf("\n");
    }
}

void salvar_biblioteca(Biblioteca *b) {
    char nome_arquivo[256];
    
    if(b->nome_arquivo == NULL) {
        printf("Nome do Arquivo para salvar (termina com .csv): ");
        while(getchar() != '\n');
        fgets(nome_arquivo, sizeof(nome_arquivo), stdin);
        nome_arquivo[strcspn(nome_arquivo, "\n")] = '\0';
        b->nome_arquivo = malloc(strlen(nome_arquivo) + 1);
        strcpy(b->nome_arquivo, nome_arquivo);
    } else {
        strcpy(nome_arquivo, b->nome_arquivo);
    }

    FILE *arquivo = fopen(nome_arquivo, "w");
    if (arquivo == NULL) {
        printf("Erro ao abrir arquivo para escrita!\n");
        return;
    }
    
    fprintf(arquivo, "%d,%d\n", b->proximo_id, b->quantidade);
    
    for(int i = 0; i < b->quantidade; i++) {
        fprintf(arquivo, "%d,%s,%s,%d,%d\n", 
                b->livros[i]->id, 
                b->livros[i]->titulo, 
                b->livros[i]->autor, 
                b->livros[i]->ano_publicacao, 
                b->livros[i]->lido);
    }
    
    fclose(arquivo);
    printf("Biblioteca salva em %s\n", nome_arquivo);
}

Livro* buscar_livros(Biblioteca *b, int id) {
    for(int i = 0; i < b->quantidade; i++) {
        if(b->livros[i]->id == id) {
            return b->livros[i];
        }
    }
    return NULL;
}

Livro* busca_por_autor(Biblioteca* b) {
    char autor[MAX_STRING];
    
    printf("Qual o Autor: ");
    while (getchar() != '\n');
    fgets(autor, MAX_STRING, stdin);
    autor[strcspn(autor, "\n")] = '\0';
    
    for(int i = 0; i < b->quantidade; i++) {
        if(strcmp(b->livros[i]->autor, autor) == 0) {
            return b->livros[i];
        }
    }
    
    return NULL;
}

Biblioteca* carregar_arquivo(const char* arquivo) {
    FILE *biblioteca_file = fopen(arquivo, "r");
    if (biblioteca_file == NULL) {
        return NULL;
    }
    
    Biblioteca *b = criar_biblioteca();
    if(b == NULL) {
        fclose(biblioteca_file);
        return NULL;
    }

    int proximo_id, quantidade;
    if(fscanf(biblioteca_file, "%d,%d\n", &proximo_id, &quantidade) != 2) {
        fclose(biblioteca_file);
        destruir_biblioteca(b);
        return NULL;
    }

    b->proximo_id = proximo_id;

    char linha[500];
    for(int i = 0; i < quantidade; i++) {
        if(fgets(linha, sizeof(linha), biblioteca_file) == NULL) {
            break;
        }

        Livro *livro = malloc(sizeof(Livro));
        char* token;

        token = strtok(linha, ",");
        if (token == NULL) {
            free(livro);
            continue;
        }
        livro->id = atoi(token);

        token = strtok(NULL, ",");
        if (token == NULL) {
            free(livro);
            continue;
        }
        livro->titulo = malloc(strlen(token) + 1);
        strcpy(livro->titulo, token);

        token = strtok(NULL, ",");
        if (token == NULL) {
            free(livro->titulo);
            free(livro);
            continue;
        }
        livro->autor = malloc(strlen(token) + 1);
        strcpy(livro->autor, token);

        token = strtok(NULL, ",");
        if (token == NULL) {
            free(livro->titulo);
            free(livro->autor);
            free(livro);
            continue;
        }
        livro->ano_publicacao = atoi(token);

        token = strtok(NULL, ",");
        if (token == NULL) {
            free(livro->titulo);
            free(livro->autor);
            free(livro);
            continue;
        }
        livro->lido = atoi(token);
        
        if (b->quantidade == b->capacidade) {
            int nova_capacidade = b->capacidade * 2;
            Livro** novoarray = realloc(b->livros, (nova_capacidade * sizeof(Livro*)));
            
            if(novoarray == NULL) {
                printf("Erro ao expandir memoria!!\n");
                free(livro->titulo);
                free(livro->autor);
                free(livro);
                fclose(biblioteca_file);
                return b;
            }
            
            b->livros = novoarray;
            b->capacidade = nova_capacidade;
        }
        
        b->livros[b->quantidade] = livro;
        b->quantidade++;
    }
    
    fclose(biblioteca_file);
    return b;
}

void destruir_biblioteca(Biblioteca* b) {
    if (b == NULL) return;
    
    for (int i = 0; i < b->quantidade; i++) {
        if (b->livros[i] != NULL) {
            free(b->livros[i]->titulo);
            free(b->livros[i]->autor);
            free(b->livros[i]);
        }
    }
    
    free(b->livros);
    free(b->nome_arquivo);
    free(b);
}

int remover_livro(Biblioteca* b, int id) {
    if (b == NULL || b->quantidade == 0) {
        return -1;
    }
    
    int indice = -1;
    for (int i = 0; i < b->quantidade; i++) {
        if (b->livros[i] != NULL && b->livros[i]->id == id) {
            indice = i;
            break;
        }
    }
    
    if (indice == -1) {
        return -2;
    }

    if (b->livros[indice] != NULL) {
        free(b->livros[indice]->titulo);
        free(b->livros[indice]->autor);
        free(b->livros[indice]);
    }

    for (int i = indice; i < b->quantidade - 1; i++) {
        b->livros[i] = b->livros[i + 1];
    }

    b->quantidade--;
    return 0;
}

void executar_menu(Biblioteca* b) {
    int opcao;
    
    do {
        printf("\n=== BIBLIOTECA PESSOAL ===\n");
        printf("1. Adicionar livro\n");
        printf("2. Listar livros\n");
        printf("3. Marcar como lido\n");
        printf("4. Buscar por autor\n");
        printf("5. Buscar por ID\n");
        printf("6. Remover livro\n");
        printf("7. Estatisticas\n");
        printf("8. Salvar e Sair\n");
        printf("Escolha: ");
        scanf("%d", &opcao);
        printf("\n");
        while(opcao < 1 || opcao > 8) {
            printf("Escolha Não Valida, Tente novamente: ");
            scanf("%d", &opcao);
        }
        
        switch (opcao) {
            case 1:
                adicionar_livro(b);
                break;
                
            case 2:
                listar_livros(b);
                break;
                
            case 3:
                marcar_lido(b);
                break;
                
            case 4: {
                Livro* resposta = busca_por_autor(b);
                if(resposta != NULL) {
                    printf("Livro encontrado:\n");
                    printf("ID: %d\n", resposta->id);
                    printf("Título: %s\n", resposta->titulo);
                    printf("Lido: %s\n", resposta->lido ? "Sim" : "Não");
                } else {
                    printf("Não há nenhum livro deste autor\n");
                }
                break;
            }
                
            case 5: {
                int id;
                printf("Insira o ID do livro: ");
                scanf("%d", &id);
                Livro* resposta = buscar_livros(b, id);
                if(resposta != NULL) {
                    printf("Livro encontrado:\n");
                    printf("Título: %s\n", resposta->titulo);
                    printf("Autor: %s\n", resposta->autor);
                    printf("Lido: %s\n", resposta->lido ? "Sim" : "Não");
                } else {
                    printf("Não há nenhum livro com este id\n");
                }
                break;
            }
                
            case 6: {
                int id;
                printf("Insira o ID do livro: ");
                scanf("%d", &id);
                int resultado = remover_livro(b, id);
                if(resultado == 0) {
                    printf("Livro removido com sucesso!!\n");
                } else if (resultado == -2) {
                    printf("Livro não encontrado!\n");
                } else {
                    printf("Não foi possível remover esse livro\n");
                }
                break;
            }
                
            case 7:
                printf("Quantidade de Livros: %d\n", b->quantidade);
                printf("Capacidade atual: %d\n", b->capacidade);
                if(b->nome_arquivo != NULL) {
                    printf("Nome do arquivo: %s\n", b->nome_arquivo);
                } else {
                    printf("Arquivo: Não salvo ainda\n");
                }
                break;
                
            case 8:
                salvar_biblioteca(b);
                break;
        }
        
        if (opcao != 8) {
            printf("\nPressione Enter para continuar...");
            while (getchar() != '\n');
            getchar();
        }
        
    } while (opcao != 8);
}

void marcar_lido(Biblioteca* b) {
    int id;
    printf("Qual o ID do livro? ");
    scanf("%d", &id);
    
    Livro* livro = buscar_livros(b, id);
    if (livro != NULL) {
        livro->lido = 1;
        printf("Livro '%s' marcado como lido!\n", livro->titulo);
    } else {
        printf("Livro com ID %d não encontrado!\n", id);
    }
}
#endif