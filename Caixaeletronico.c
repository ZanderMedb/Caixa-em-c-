#include <ctype.h>
#include <stdio.h>
#include <string.h>
#include <locale.h>
#include <unistd.h>
#include <stdlib.h>

typedef struct _cliente {
    char nome[50];
    float saldo;
    struct _cliente *prox;
} Cliente;

Cliente *criarCliente(char *nome, float saldo) {
    Cliente *novoCliente = calloc(1, sizeof(Cliente));
    strcpy(novoCliente->nome, nome);
    novoCliente->saldo = saldo;
    novoCliente->prox = NULL;
    return novoCliente;
}

void displayCliente(Cliente *cliente) {
    printf("Nome: %s\n", cliente->nome);
    printf("Saldo: %.2f\n", cliente->saldo);
}

void sacar(Cliente *cliente, float valor) {
    if (cliente->saldo >= valor) {
        cliente->saldo -= valor;
    }
}

void depositar(Cliente *cliente, float valorDep) {
    cliente->saldo += valorDep;
}

void transferir(Cliente *cliente, Cliente *remetente, Cliente *destinatario, float valorTransf) {
    if (remetente->saldo >= valorTransf) {
        remetente->saldo -= valorTransf;
        destinatario->saldo += valorTransf;
    }
}

void limparMemoriaPorNome(Cliente **head, char *nome) {
    Cliente *atual = *head;
    Cliente *anterior = NULL;

    while (atual != NULL) {
        printf("\nVerificando cliente: %s\n", atual->nome);

        if (strcmp(atual->nome, nome) == 0) {
            if (anterior == NULL) {
                *head = atual->prox;
            } else {
                anterior->prox = atual->prox;
            }
            printf("Saldo final de %s: %.2f\n", atual->nome, atual->saldo);
            free(atual); 
            return;
        }
        anterior = atual;
        atual = atual->prox;
    }

    printf("Cliente com nome '%s' nao encontrado.\n", nome);
}

int main(void) {
    setlocale(LC_ALL, "Portuguese");

    int continuar = 1;

    
    char nomeCliente[50];
    float saldoCliente, valorOperacao;
    int opcao;

    
    int promptsMostrados = 0;

    while (continuar) {
        Cliente *cliente = NULL, *cliente1 = NULL, *cliente2 = NULL;

        float saldoIni;

        if (!promptsMostrados) {
            printf("Informe o nome do cliente: ");
            scanf("%s", nomeCliente);

            printf("\nInforme o saldo do cliente: ");
            scanf("%f", &saldoCliente);

            promptsMostrados = 1;
        }

        printf("Escolha [1] para saque, [2] para deposito e [3] para transferencia: ");
        scanf("%d", &opcao);
        printf("\n--------------------------------------------------------------------");

        switch (opcao) {
            case 1:
                cliente = criarCliente(nomeCliente, saldoCliente);

                printf("\nInforme o valor a ser sacado: ");
                scanf("%f", &valorOperacao);

                sacar(cliente, valorOperacao);

                if (valorOperacao > cliente->saldo) {
                    printf("\n------------------------------");
                    printf("\nValor a ser sacado invalido");
                    printf("\n------------------------------");
                } else if (cliente->saldo > valorOperacao && valorOperacao <= 0) {
                    printf("\n------------------------------");
                    printf("\nValor a ser sacado invalido");
                    printf("\n------------------------------");
                } else {
                    saldoIni = cliente->saldo + valorOperacao;

                    printf("-----------------------------\n");
                    printf("Saque realizado com sucesso.\n");
                    printf("-----------------------------\n");

                    printf("Nome cliente: %s", cliente->nome);
                    printf("\nSaldo inicial: %.2f", saldoIni);
                    printf("\nSaldo final: %.2f", cliente->saldo);
                    printf("\nValor sacado: %.2f", valorOperacao);
                }

                limparMemoriaPorNome(&cliente, nomeCliente);

                cliente = NULL;
                saldoIni = 0;
                valorOperacao = 0;

                break;

            case 2:
                cliente = criarCliente(nomeCliente, saldoCliente);

                printf("\nInforme o valor a ser depositado: ");
                scanf("%f", &valorOperacao);

                depositar(cliente, valorOperacao);

                if (valorOperacao <= 0) {
                    printf("\n------------------------------");
                    printf("\nValor a ser depositado inválido");
                    printf("\n------------------------------");
                } else {
                    saldoIni = cliente->saldo - valorOperacao;

                    printf("-----------------------------\n");
                    printf("Deposito realizado com sucesso.\n");
                    printf("-----------------------------\n");

                    printf("Nome cliente: %s", cliente->nome);
                    printf("\nSaldo inicial: %.2f", saldoIni);
                    printf("\nSaldo final: %.2f", cliente->saldo);
                    printf("\nValor depositado: %.2f", valorOperacao);
                }

                limparMemoriaPorNome(&cliente, nomeCliente);

                cliente = NULL;
                saldoIni = 0;
                valorOperacao = 0;

                break;

            case 3:
                cliente1 = criarCliente(nomeCliente, saldoCliente);

                printf("\nInforme o valor a ser transferido: ");
                scanf("%f", &valorOperacao);

                if (valorOperacao > cliente1->saldo) {
                    printf("---------------------------------------");
                    printf("\nSaldo insuficiente para transferencia");
                    printf("\n---------------------------------------");
                } else if (valorOperacao <= 0) {
                    printf("-------------------------------");
                    printf("\nValor para transferencia invalido");
                    printf("\n-------------------------------");
                } else {
                    cliente2 = criarCliente("", 0);

                    printf("\nInforme o nome do cliente (Destinatário): ");
                    scanf("%s", cliente2->nome);

                    printf("\nInforme seu saldo: ");
                    scanf("%f", &cliente2->saldo);

                    transferir(cliente, cliente1, cliente2, valorOperacao);

                    saldoIni = cliente1->saldo + valorOperacao;

                    printf("\n------------------------------------------------");
                    printf("\nNome (Remetente): %s", cliente1->nome);
                    printf("\nSaldo inicial: %.2f", saldoIni);
                    printf("\nSaldo final: %.2f", cliente1->saldo);
                    printf("\nValor da transferencia: %.2f", valorOperacao);

                    saldoIni = cliente2->saldo - valorOperacao;

                    printf("\n------------------------------------------------");
                    printf("\nNome (Destinatário): %s", cliente2->nome);
                    printf("\nSaldo inicial: %.2f", saldoIni);
                    printf("\nSaldo final: %.2f", cliente2->saldo);
                    printf("\nValor recebido: %.2f", valorOperacao);
                }

                limparMemoriaPorNome(&cliente1, nomeCliente);
                limparMemoriaPorNome(&cliente2, cliente2->nome);

                cliente1 = NULL;
                cliente2 = NULL;
                saldoIni = 0;
                valorOperacao = 0;

                break;

            default:
                printf("\nOpção inválida");
        }
        
        printf("\nDeseja realizar outra operacao? (1 - Sim / 0 - Não): ");
        scanf("%d", &continuar);
        system("cls");
    }

    printf("\nPrograma encerrado. Obrigado!\n");

    return 0;
}
