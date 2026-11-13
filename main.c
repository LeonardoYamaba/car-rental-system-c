#include <stdio.h>
#include <string.h>

#define MAX_CARROS 50
#define TAM_TEXTO 40
#define ARQUIVO "carros.dat"
#define DESCONTO_SEMANA 0.10f   // 10% de desconto a partir de 7 dias
#define MULTA_ATRASO 0.50f      // +50% da diaria por dia de atraso

// Filtros usados na listagem
#define TODOS 0
#define DISPONIVEIS 1
#define ALUGADOS 2

// Agrupa os dados de um carro
typedef struct {
    char modelo[TAM_TEXTO];
    float precoDia;
    int disponivel;          // 1 = livre, 0 = alugado
    char cliente[TAM_TEXTO];
    int dias;                // dias contratados
} Carro;

/* ---------- Entrada de dados ---------- */

// Le um inteiro e guarda no endereco recebido (*destino)
void lerInteiro(const char *mensagem, int *destino) {
    int lidos;

    printf("%s", mensagem);
    while ((lidos = scanf("%d", destino)) != 1) {
        if (lidos == EOF) {          // fim da entrada: devolve 0 (sair)
            *destino = 0;
            return;
        }
        while (getchar() != '\n');   // descarta o que foi digitado errado
        printf("Valor invalido. Tente de novo: ");
    }
    while (getchar() != '\n');       // limpa o Enter que sobrou
}

// Mesma ideia, mas para numeros com virgula
void lerFloat(const char *mensagem, float *destino) {
    int lidos;

    printf("%s", mensagem);
    while ((lidos = scanf("%f", destino)) != 1) {
        if (lidos == EOF) {
            *destino = 0;
            return;
        }
        while (getchar() != '\n');
        printf("Valor invalido. Tente de novo: ");
    }
    while (getchar() != '\n');
}

// Le um texto com espacos e escreve direto no vetor recebido
void lerTexto(const char *mensagem, char *destino, int tamanho) {
    printf("%s", mensagem);
    fgets(destino, tamanho, stdin);
    destino[strcspn(destino, "\n")] = '\0';   // remove o \n do final
}

// Procedimento: so espera o usuario apertar Enter
void pausar(void) {
    printf("\nPressione Enter para continuar...");
    getchar();
}

/* ---------- Arquivo ---------- */

// Salva a quantidade e depois todos os carros no arquivo binario
void salvarDados(Carro carros[], int total) {
    FILE *arq = fopen(ARQUIVO, "wb");

    if (arq == NULL) {
        printf("\nErro ao salvar os dados.\n");
        return;
    }
    fwrite(&total, sizeof(int), 1, arq);
    fwrite(carros, sizeof(Carro), total, arq);
    fclose(arq);
}

// Le o arquivo. Retorna 1 se carregou, 0 se nao existe (ou esta invalido)
int carregarDados(Carro carros[], int *total) {
    FILE *arq = fopen(ARQUIVO, "rb");
    int lidos;

    if (arq == NULL) return 0;   // primeira vez: ainda nao ha arquivo

    if (fread(total, sizeof(int), 1, arq) != 1 || *total < 0 || *total > MAX_CARROS) {
        fclose(arq);
        return 0;
    }
    lidos = (int) fread(carros, sizeof(Carro), *total, arq);
    fclose(arq);
    return lidos == *total;
}

/* ---------- Cadastro de carros ---------- */

// Adiciona um carro no fim do vetor e aumenta o total (*total)
int adicionarCarro(Carro carros[], int *total, const char *modelo, float preco) {
    Carro *novo;

    if (*total >= MAX_CARROS) return 0;   // vetor cheio

    novo = &carros[*total];               // ponteiro para a proxima posicao livre
    strcpy(novo->modelo, modelo);
    novo->precoDia = preco;
    novo->disponivel = 1;
    novo->cliente[0] = '\0';
    novo->dias = 0;

    (*total)++;
    return 1;
}

// Carros iniciais (usados so na primeira execucao)
void iniciarCarros(Carro carros[], int *total) {
    adicionarCarro(carros, total, "Fiat Mobi", 90.0f);
    adicionarCarro(carros, total, "Chevrolet Onix", 110.0f);
    adicionarCarro(carros, total, "Hyundai HB20", 115.0f);
    adicionarCarro(carros, total, "VW Polo", 130.0f);
    adicionarCarro(carros, total, "Renault Kwid", 85.0f);
    adicionarCarro(carros, total, "Toyota Corolla", 200.0f);
    adicionarCarro(carros, total, "Jeep Renegade", 220.0f);
    adicionarCarro(carros, total, "Honda Civic", 210.0f);
}

/* ---------- Telas e listagens ---------- */

void exibirMenu(void) {
    printf("\n=============================\n");
    printf("      LOCADORA DE CARROS\n");
    printf("=============================\n");
    printf(" 1 - Ver carros disponiveis\n");
    printf(" 2 - Ver todos os carros\n");
    printf(" 3 - Alugar um carro\n");
    printf(" 4 - Devolver um carro\n");
    printf(" 5 - Cadastrar novo carro\n");
    printf(" 0 - Sair\n");
    printf("-----------------------------\n");
}

// Mostra a tabela de carros; o filtro decide quais linhas aparecem
void listarCarros(Carro carros[], int total, int filtro) {
    printf("\n %-4s %-18s %-11s %s\n", "ID", "Modelo", "Valor/dia", "Status");

    for (int i = 0; i < total; i++) {
        if (filtro == DISPONIVEIS && !carros[i].disponivel) continue;   // pula alugados
        if (filtro == ALUGADOS && carros[i].disponivel) continue;       // pula livres

        printf(" %-4d %-18s R$ %-8.2f ", i + 1, carros[i].modelo, carros[i].precoDia);

        if (carros[i].disponivel)
            printf("Disponivel\n");
        else
            printf("Alugado (%s, %d dias)\n", carros[i].cliente, carros[i].dias);
    }
}

/* ---------- Regras de negocio ---------- */

// Conta quantos carros estao livres (retorna o valor)
int contarDisponiveis(Carro carros[], int total) {
    int livres = 0;

    for (int i = 0; i < total; i++) {
        if (carros[i].disponivel) livres++;
    }
    return livres;
}

// Valida o ID digitado e converte para indice do vetor
// Retorna -1 se o ID nao existir
int obterIndice(int id, int total) {
    if (id < 1 || id > total) return -1;
    return id - 1;   // vetor comeca em 0, o menu comeca em 1
}

// Calcula o total e a multa, devolvendo os dois por ponteiro
void calcularValores(float preco, int diasContratados, int diasUsados,
                     float *total, float *multa) {
    *total = preco * diasContratados;
    if (diasContratados >= 7) {
        *total -= *total * DESCONTO_SEMANA;   // aplica o desconto
    }

    *multa = 0;
    if (diasUsados > diasContratados) {       // devolveu atrasado
        *multa = (diasUsados - diasContratados) * preco * (1 + MULTA_ATRASO);
    }
    *total += *multa;
}

// Retorna 1 se alugou (para o main salvar), 0 se cancelou
int alugarCarro(Carro carros[], int total) {
    int id, dias, pos;
    float valor, multa;
    char nome[TAM_TEXTO];
    Carro *c;

    if (contarDisponiveis(carros, total) == 0) {
        printf("\nNenhum carro disponivel no momento.\n");
        return 0;
    }

    listarCarros(carros, total, DISPONIVEIS);
    lerInteiro("\nDigite o ID do carro: ", &id);   // &id = passa o endereco

    pos = obterIndice(id, total);
    if (pos == -1) {
        printf("\nID invalido.\n");
        return 0;
    }
    if (!carros[pos].disponivel) {
        printf("\nEsse carro ja esta alugado.\n");
        return 0;
    }

    lerTexto("Nome do cliente: ", nome, TAM_TEXTO);
    if (strlen(nome) == 0) {
        printf("\nO nome nao pode ficar vazio.\n");
        return 0;
    }

    lerInteiro("Quantos dias de aluguel? ", &dias);
    if (dias <= 0) {
        printf("\nQuantidade de dias invalida.\n");
        return 0;
    }

    c = &carros[pos];   // atalho: c aponta para o carro escolhido
    calcularValores(c->precoDia, dias, dias, &valor, &multa);

    // Tudo certo: registra o aluguel
    strcpy(c->cliente, nome);
    c->dias = dias;
    c->disponivel = 0;

    printf("\nAluguel confirmado!\n");
    printf("Carro: %s | Cliente: %s | Dias: %d\n", c->modelo, nome, dias);
    printf("Total a pagar: R$ %.2f\n", valor);
    if (dias >= 7) printf("(desconto de 10%% aplicado)\n");
    return 1;
}

// Retorna 1 se devolveu (para o main salvar), 0 se cancelou
int devolverCarro(Carro carros[], int total) {
    int id, pos, diasUsados;
    float valor, multa;
    Carro *c;

    if (contarDisponiveis(carros, total) == total) {
        printf("\nNao ha carros alugados para devolver.\n");
        return 0;
    }

    listarCarros(carros, total, ALUGADOS);
    lerInteiro("\nDigite o ID do carro a devolver: ", &id);

    pos = obterIndice(id, total);
    if (pos == -1) {
        printf("\nID invalido.\n");
        return 0;
    }
    if (carros[pos].disponivel) {
        printf("\nEsse carro nao esta alugado.\n");
        return 0;
    }

    c = &carros[pos];
    printf("Dias contratados: %d\n", c->dias);
    lerInteiro("Quantos dias o cliente ficou com o carro? ", &diasUsados);
    if (diasUsados <= 0) {
        printf("\nQuantidade de dias invalida.\n");
        return 0;
    }

    calcularValores(c->precoDia, c->dias, diasUsados, &valor, &multa);

    printf("\nDevolucao registrada!\n");
    printf("Carro: %s | Cliente: %s\n", c->modelo, c->cliente);
    if (multa > 0) printf("Multa por atraso: R$ %.2f\n", multa);
    printf("Total pago: R$ %.2f\n", valor);

    // Libera o carro e limpa os dados do aluguel
    c->disponivel = 1;
    c->cliente[0] = '\0';
    c->dias = 0;
    return 1;
}

// Retorna 1 se cadastrou. O total vai por ponteiro porque aumenta
int cadastrarCarro(Carro carros[], int *total) {
    char modelo[TAM_TEXTO];
    float preco;

    if (*total >= MAX_CARROS) {
        printf("\nLimite de %d carros atingido.\n", MAX_CARROS);
        return 0;
    }

    lerTexto("\nModelo do carro: ", modelo, TAM_TEXTO);
    if (strlen(modelo) == 0) {
        printf("\nO modelo nao pode ficar vazio.\n");
        return 0;
    }

    lerFloat("Valor da diaria (R$): ", &preco);
    if (preco <= 0) {
        printf("\nValor invalido.\n");
        return 0;
    }

    adicionarCarro(carros, total, modelo, preco);
    printf("\nCarro cadastrado com o ID %d!\n", *total);
    return 1;
}

/* ---------- Programa principal ---------- */

int main(void) {
    Carro carros[MAX_CARROS];
    int total = 0;
    int opcao;

    // Tenta ler o arquivo; se nao existir, cria os carros iniciais
    if (!carregarDados(carros, &total)) {
        total = 0;
        iniciarCarros(carros, &total);
        salvarDados(carros, total);
    }

    do {
        exibirMenu();
        lerInteiro("Escolha uma opcao: ", &opcao);

        switch (opcao) {
            case 1:
                if (contarDisponiveis(carros, total) == 0)
                    printf("\nNenhum carro disponivel no momento.\n");
                else
                    listarCarros(carros, total, DISPONIVEIS);
                pausar();
                break;
            case 2:
                listarCarros(carros, total, TODOS);
                pausar();
                break;
            case 3:
                if (alugarCarro(carros, total)) salvarDados(carros, total);
                pausar();
                break;
            case 4:
                if (devolverCarro(carros, total)) salvarDados(carros, total);
                pausar();
                break;
            case 5:
                if (cadastrarCarro(carros, &total)) salvarDados(carros, total);
                pausar();
                break;
            case 0:
                printf("\nAte logo!\n");
                break;
            default:
                printf("\nOpcao invalida.\n");
                pausar();
        }
    } while (opcao != 0);

    return 0;
}
