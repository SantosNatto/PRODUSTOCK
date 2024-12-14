#include <stdio.h>
#include <locale.h>
#include <windows.h>


void clear() {
    #ifdef __linux__
        system("clear");
    #else
        system("cls");
    #endif
}

void limparBuffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

// Função para mudar a cor do texto
void mudarCorTexto(WORD cor) {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE); // Pega o handle do console
    SetConsoleTextAttribute(hConsole, cor);  // Altera a cor do texto
}

//----------------------Variaveis Globais
int men = 0;
int contadorProdutos[8] = {0}; // contador para categoria
//---------------------------------------
typedef enum {
    HIGIENE = 1,
    PERFUMARIA,
    MAQUIAGEM,
    CUIDADOS_CAPILARES,
    INFANTIL,
    MASCULINO,
    AROMAS,
    OFERTAS_E_KITS
} Categoria;

//-------Mapeamento de prefixos
const char* getPrefixoCategoria(Categoria categoria){
    switch (categoria){
    case HIGIENE:
        return "HGN";
    case PERFUMARIA:
        return "PRF";
    case MAQUIAGEM:
        return "MAQ";
    case CUIDADOS_CAPILARES:
        return "CCP";
    case INFANTIL:
        return "INF";
    case MASCULINO:
        return "MSC";
    case AROMAS:
        return "ARM";
    case OFERTAS_E_KITS:
        return "OKT";
    default:
        return "DES";
    }
}

typedef struct Data{
    int dia;
    int mes;
    int ano;
} Data;

typedef enum {
    NATURA = 1,
    AVON
}Fornecedor;

const char* getNomeFornecedor(Fornecedor fornecedor) {
    switch (fornecedor)
    {
    case NATURA:
        return "Natura";
    case AVON:
        return "Avon";
    default:
        return "Desconhecido";
    }
}

const char* getNomeCategoria(Categoria categoria) {
    switch (categoria) {
        case HIGIENE:
            return "Higiene";
        case PERFUMARIA:
            return "Perfumaria";
        case MAQUIAGEM:
            return "Maquiagem";
        case CUIDADOS_CAPILARES:
            return "Cuidados capilares";
        case INFANTIL:
            return "Infantil";
        case MASCULINO:
            return "Masculino";
        case AROMAS:
            return "Aromas";
        case OFERTAS_E_KITS:
            return "Ofertas e Kits";
        default:
            return "Desconhecido";
    }
}

typedef struct { // Cadastro de produtos
    char nome[50]; 
    char codProduto[10];
    char descricao[300]; // opcional
    Categoria categoria;
    Fornecedor fornecedor;
    Data Compra; // Data de compra do produto
    Data validade; // Data de validade do produto
    int estoque; // Quantidade que já possui no estoque
}Produtos;

typedef struct Cliente{
    char nome[50];
    char telefone[20];
    char endereco[30];
    char observacoes[100];
}Cliente;

void menu();
void menuProdutos();
void CadastroProduto();
void gerarCodigo(Produtos *produto);
//----------------------Função principal
int main(){
    clear();
    setlocale(LC_ALL, "Portuguese_Brazil"); // Configura a localidade de acordo com o sistema
    
    while (men != 6){
        menu();
        switch (men){
        case 1:
            menuProdutos();
            break;
        
        default:
            break;
        }
    }
    

    return 0;
}

//----------------------Funções
int lerData(char *entrada, Data *data) {
    int dia, mes, ano;

    if ((strlen(entrada) == 10 && sscanf(entrada, "%d/%d/%d", &dia, &mes, &ano) == 3) ||
        (strlen(entrada) == 8 && sscanf(entrada, "%2d%2d%4d", &dia, &mes, &ano) == 3)) {

        if (mes >= 1 && mes <= 12 && ano >=1900){

            if (dia >= 1 && dia <= 31){
                
                if ((mes == 4 || mes == 6 || mes == 9 || mes ==11) && dia > 30){
                    printf("Dia inválido para o mês %d.\n", mes);
                    return 0;
                }
                
                if (mes == 2 && dia > 28){
                    printf("Dia inválido para o mês de fevereiro.\n");
                    return 0;
        
            }
                if (mes == 2 && dia == 29 && (ano % 4 != 0 || ( ano % 100 == 0 && ano % 400 != 0))){
                    printf("Ano %d não é bissexto, 29 de fevereiro é invalido.\n", ano);
                    return 0;
                }

                data->dia = dia;
                data->mes = mes;
                data->ano = ano;
                return 1;
            }

                
        }
    }
           
    data->dia = data->mes = data->ano = -1;
    return 0;
}

void menu(){
    clear(); // Limpa a tela
    printf("-----Menu-----\n\n");
    printf("1. Cadastro de produtos\n");
    printf("2. Fluxo de caixa\n");
    printf("3. Estoque\n");
    printf("4. Clientes\n");
    printf("5. Promoção e Descontos\n");
    printf("6. Sair\n");
    printf("\n-----------------------------\n");
    printf("Escolha uma opção: ");
    scanf("%d", &men);
    limparBuffer();

    if (men == 6){
        printf("Saindo...");
    }

    if (men < 1 || men > 6){
        printf("\nOpção escolhida (%d), não é valida!!", men);
        Sleep(2500);
        clear();
        return;
    }
}

void menuProdutos(){
    int opcao = 0;
    do{
    clear();
    printf("-----Cadastro de produtos-----\n\n");
    printf("1. Cadastrar produto\n");
    printf("2. Editar produto\n");
    printf("3. excluir produto\n");
    printf("4. Listar produtos\n");
    printf("5. Voltar\n");
    printf("\n---------------------------------\n");
    printf("Escolha uma opção: ");
    scanf("%d", &opcao);
    limparBuffer();

    if (opcao == 5){
        return;
    }
    
    if (opcao < 1 || opcao > 5){
        printf("\nOpção selecionada (%d) invalida\n", opcao);
        Sleep(2500);
        clear();  
    }
    
    } while (opcao < 1 || opcao > 5);

    switch (opcao){
    case 1:
        CadastroProduto();
        break;
    
    default:
        break;
    }
}

void CadastroProduto(){
    clear();
    Produtos produto;
    int campo = 1;
    int opcaoCategoria = 0;
    int opcaoFornecedor = 0;
    char dataEntrada[11];
    char dataValidade[11];

    while (campo <= 7){
        switch (campo)
        {
        case 1: // Nome do produto
            clear();
            printf("-----Cadastrar novo produto-----\n\n");
            printf("Nome do produto: ");
            fgets(produto.nome, sizeof(produto.nome), stdin);
            produto.nome[strcspn(produto.nome, "\n")] = '\0';
            campo++;
            break;

        case 2: // Categoria
            clear();
            printf("-----Cadastrar novo produto-----\n\n");
            printf("Nome: %s\n", produto.nome);
            printf("\n---------------------------------\n");
            printf("Categoria:\n\n");
            printf("1. Higiene e Cuidados\n");
            printf("2. Perfumaria\n");
            printf("3. Maquiagem\n");
            printf("4. Cuidados capilares\n");
            printf("5. Infantil\n");
            printf("6. Produtos masculino\n");
            printf("7. Aromas\n");
            printf("8. Ofertas e Kits\n");
            do{
                printf("Escolha a categoria (1-8): ");
                scanf("%d", &opcaoCategoria);
                limparBuffer();
                if (opcaoCategoria < 1 || opcaoCategoria > 8){
                    printf("Categoria inválida! Escolha entre 1 e 8.\n");
                }
            
            } while (opcaoCategoria < 1 || opcaoCategoria > 8);
            produto.categoria = opcaoCategoria;
            campo++;
            break;

        case 3: //Quantidade em estoque
            clear();
            printf("-----Cadastrar novo produto-----\n\n");
            printf("Nome: %s\n", produto.nome);
            printf("Categoria: %s\n", getNomeCategoria(produto.categoria));
            printf("\n---------------------------------\n");
            printf("Quantidade em estoque: ");
            scanf("%d", &produto.estoque);
            limparBuffer();
            campo++;
            break;

        case 4: // Descrição do produto
            clear();
            printf("-----Cadastrar novo produto-----\n\n");
            printf("Nome: %s\n", produto.nome);
            printf("Categoria: %s\n", getNomeCategoria(produto.categoria));
            printf("Quantidade em estoque: %d\n", produto.estoque);
            printf("\n---------------------------------\n");
            printf("Descrição do produto\n");
            fgets(produto.descricao, sizeof(produto.descricao), stdin);
            produto.descricao[strcspn(produto.descricao, "\n")] = '\0';
            campo++;
            break;

        case 5: // Marca do produto
            clear();
            printf("-----Cadastrar novo produto-----\n\n");
            printf("Nome: %s\n", produto.nome);
            printf("Categoria: %s\n", getNomeCategoria(produto.categoria));
            printf("Quantidade em estoque: %d\n", produto.estoque);
            printf("Descrição do produto\n");
            printf("%s\n", produto.descricao);
            printf("\n---------------------------------\n");
            printf("Marca:\n\n");
            printf("1. Natura\n");
            printf("2. Avon\n");
            do{
                printf("Escolha a marca deste produto (1 ou 2): ");
                scanf("%d", &opcaoFornecedor);
                limparBuffer();
            } while (opcaoFornecedor < 1 || opcaoFornecedor > 2);
            produto.fornecedor = opcaoFornecedor;
            campo++;
            break;

        case 6: // Data de compra
            clear();
            printf("-----Cadastrar novo produto-----\n\n");
            printf("Nome: %s\n", produto.nome);
            printf("Categoria: %s\n", getNomeCategoria(produto.categoria));
            printf("Quantidade em estoque: %d\n", produto.estoque);
            printf("Descrição do produto\n");
            printf("%s\n", produto.descricao);
            printf("Fornecedor: %s\n", getNomeFornecedor(produto.fornecedor));
            printf("\n---------------------------------\n");
            printf("Data de compra (ou pressione Enter para continuar): ");

            fgets(dataEntrada, sizeof(dataEntrada), stdin);
            dataEntrada[strcspn(dataEntrada, "\n")] = '\0';

            if (strlen(dataEntrada) == 0){
                produto.Compra.dia = produto.Compra.mes = produto.Compra.ano = 0; // Pular data
            }else if (!lerData(dataEntrada, &produto.Compra)){ 
                    printf("Data inválida. Tente novamnete.\n");
                    continue;
            }    
            campo++;
            break;
            
        case 7: //Data de validade
            clear();
            printf("-----Cadastrar novo produto-----\n\n");
            printf("Nome: %s\n", produto.nome);
            printf("Categoria: %s\n", getNomeCategoria(produto.categoria));
            printf("Quantidade em estoque: %d\n", produto.estoque);
            printf("Descrição do produto: %s\n", produto.descricao);
            printf("Fornecedor: %s\n", getNomeFornecedor(produto.fornecedor));
    
            if (produto.Compra.dia != 0) {
                printf("Data de compra registrada: %02d/%02d/%04d\n", produto.Compra.dia, produto.Compra.mes, produto.Compra.ano);
            } else {
                printf("Data de compra: Pulada\n");
            }
            printf("\n---------------------------------\n");
            printf("Data de validade (ou pressione Enter para continuar): ");

            fgets(dataValidade, sizeof(dataValidade), stdin);
            dataValidade[strcspn(dataValidade, "\n")] = '\0';

            if (strlen(dataValidade) == 0) {
                // Pular data de validade
                produto.validade.dia = produto.validade.mes = produto.validade.ano = 0;
                campo++;
            } else if (lerData(dataValidade, &produto.validade)) {
                campo++;
            } else {
                printf("Data inválida. Tente novamente.\n");
                printf("Pressione Enter para continuar.\n");
                getchar();
            }
            break;
        default:
            break;
        }
    }
    clear();
    gerarCodigo(&produto);
    printf("\nProduto cadastrado com sucesso!\n\n");
    printf("Código do produto: %s\n", produto.codProduto);
    printf("Nome: %s\n", produto.nome);
    printf("Categoria: %s\n", getNomeCategoria(produto.categoria));
    printf("Fornecedor: %s\n", getNomeFornecedor(produto.fornecedor));
    if (produto.Compra.dia > 0)
        printf("Data de compra: %02d/%02d/%04d\n", produto.Compra.dia, produto.Compra.mes, produto.Compra.ano);
    if (produto.validade.dia > 0)
        printf("Data de validade: %02d/%02d/%04d\n", produto.validade.dia, produto.validade.mes, produto.validade.ano);
    printf("Quantidade em estoque: %d\n", produto.estoque);
    printf("Descrição: %s\n\n", produto.descricao);
    printf("Pressione Enter para continuar\n");
    getchar();

} 

void gerarCodigo(Produtos *produto) {
    const char *prefixo = getPrefixoCategoria(produto->categoria);
    contadorProdutos[produto->categoria - 1]++;
    sprintf(produto->codProduto, "%s%03d", prefixo, contadorProdutos[produto->categoria - 1]);
}

