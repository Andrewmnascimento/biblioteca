#include "biblioteca.h"

int main(int argc, char* argv[]) {
    Biblioteca* biblioteca;
    
    if (argc == 2) {
        biblioteca = carregar_arquivo(argv[1]);
        if (biblioteca == NULL) {
            printf("Arquivo não encontrado. Criando nova biblioteca.\n");
            biblioteca = criar_biblioteca();
        } else {
            biblioteca->nome_arquivo = malloc(strlen(argv[1]) + 1);
            strcpy(biblioteca->nome_arquivo, argv[1]);
        }
    } else if (argc == 1) {
        biblioteca = criar_biblioteca();
        printf("Nova biblioteca criada. Use 8 para guardar.\n");
    } else {
        printf("Uso: %s [arquivo.csv]\n", argv[0]);
        return 1;
    }
    
    executar_menu(biblioteca);
    destruir_biblioteca(biblioteca);
    
    return 0;
}
