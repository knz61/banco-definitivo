#include "biblioteca.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>

void exibirConta(conta *conta) {
    printf("\n=== Detalhes da Conta ===\n");
    printf("Número da Conta: %d\n", conta->numero_conta);
    printf("Tipo de Conta: %s\n", conta->tipo_conta);
    printf("Nome: %s\n", conta->nome);
    printf("CPF: %s\n", conta->cpf);
    printf("Saldo: %.2f\n", conta->saldo);
    printf("Limite: %.2f\n", conta->limite);
    printf("Data de Vencimento: %s\n", conta->data_vencimento);
}

void encerrarConta(conta *conta, FILE *Arquivo) {
    if (conta->ativa) {
        conta->ativa = 0;
        conta->saldo = 0.0;
        printf("Conta %d encerrada com sucesso.\n", conta->numero_conta);
    } else {
        printf("Conta %d já está encerrada.\n", conta->numero_conta);
    }
    fseek(Arquivo, -sizeof(conta), SEEK_CUR);
    fwrite(conta, sizeof(conta), 1, Arquivo);
    fflush(Arquivo);
}

void buscarConta(FILE *Arquivo, int numero_conta, conta *conta) {
    rewind(Arquivo);
    while (fread(conta, sizeof(conta), 1, Arquivo)) {
        if (conta->numero_conta == numero_conta) {
            return;
        }
    }
    conta->numero_conta = -1; // Indica que a conta não foi encontrada
}

void alterarDadosConta(conta *conta, FILE *Arquivo, const char *novoNome, double novoSaldo,
                       const char *novoTelefone, const char *novoLocal, int novoNumeroCasa,
                       const char *novoCep, const char *novoBairro, const char *novaCidade,
                       const char *novoEstado) {
    if (conta->ativa) {
        strncpy(conta->nome_cliente, novoNome, sizeof(conta->nome_cliente) - 1);
        conta->nome_cliente[sizeof(conta->nome_cliente) - 1] = '\0'; // Garantir terminação da string
        conta->saldo = novoSaldo;
        strncpy(conta->telefone, novoTelefone, sizeof(conta->telefone) - 1);
        conta->telefone[sizeof(conta->telefone) - 1] = '\0';
        strncpy(conta->endereco_local, novoLocal, sizeof(conta->endereco_local) - 1);
        conta->endereco_local[sizeof(conta->endereco_local) - 1] = '\0';
        conta->numero_casa = novoNumeroCasa;
        strncpy(conta->cep, novoCep, sizeof(conta->cep) - 1);
        conta->cep[sizeof(conta->cep) - 1] = '\0';
        strncpy(conta->bairro, novoBairro, sizeof(conta->bairro) - 1);
        conta->bairro[sizeof(conta->bairro) - 1] = '\0';
        strncpy(conta->cidade, novaCidade, sizeof(conta->cidade) - 1);
        conta->cidade[sizeof(conta->cidade) - 1] = '\0';
        strncpy(conta->estado, novoEstado, sizeof(conta->estado) - 1);
        conta->estado[sizeof(conta->estado) - 1] = '\0';

        printf("Dados da conta %d alterados com sucesso.\n", conta->numero_conta);
    } else {
        printf("Não é possível alterar os dados de uma conta encerrada.\n");
    }
    
    // Posicionar o arquivo na posição correta para escrita
    fseek(Arquivo, -sizeof(conta), SEEK_CUR);
    
    // Escrever a conta atualizada de volta no arquivo
    fwrite(conta, sizeof(conta), 1, Arquivo);
    fflush(Arquivo);
}

void cadastrarFuncionario(FILE *arquivoFuncionarios) {
    funcionario func;

    printf("Digite os dados do funcionario:\n");

    printf("Agencia: ");
    scanf("%d", &func.agencia);
    while (getchar() != '\n');

    printf("Numero da conta: ");
    scanf("%d", &func.numero_conta);
    while (getchar() != '\n');

    printf("Tipo (C para corrente, P para poupanca): ");
    scanf(" %c", &func.tipo);
    while (getchar() != '\n');

    printf("Nome: ");
    fgets(func.nome, sizeof(func.nome), stdin);
    func.nome[strcspn(func.nome, "\n")] = '\0';

    printf("Saldo: ");
    scanf("%f", &func.saldo);
    while (getchar() != '\n');

    printf("CPF: ");
    scanf("%d", &func.cpf);
    while (getchar() != '\n');

    printf("Data de nascimento (dd mm aaaa): ");
    printf("DIGITE O DIA ");
    fgets(func.data_nascimento.dia, sizeof(func.data_nascimento.dia), stdin);
    fflush(stdin);
    printf("DIGITE O MES ");
    fgets(func.data_nascimento.mes, sizeof(func.data_nascimento.mes), stdin);
    fflush(stdin);
    printf("DIGITE O ANO ");
    fgets(func.data_nascimento.ano, sizeof(func.data_nascimento.ano), stdin);
    fflush(stdin);
    

    printf("Telefone de contato: ");
    scanf("%d", &func.tel_ctt);
    while (getchar() != '\n');

    fseek(arquivoFuncionarios, 0, SEEK_END);
    fwrite(&func, sizeof(funcionario), 1, arquivoFuncionarios);
    fflush(arquivoFuncionarios);

    printf("Funcionario cadastrado com sucesso!\n");
    getch();
}


void gerarRelatorios(FILE *Arquivo) {
    conta conta;
    rewind(Arquivo);
    printf("$$$$ RELATORIO DE CONTAS $$$$\n\n");
    while (fread(&conta, sizeof(conta), 1, Arquivo)) {
        exibirConta(&conta);
        printf("\n-----------------------------\n");
    }
    getch();
}


void inserirSaldo(conta *conta, FILE *Arquivo, double valor) {
    conta->saldo += valor;
    // Atualizar o arquivo com os novos dados da conta, se necessário.
    // Exemplo de atualização do arquivo:
    fseek(Arquivo, -sizeof(conta), SEEK_CUR); // Voltar para a posição correta no arquivo
    fwrite(conta, sizeof(conta), 1, Arquivo);
    fflush(Arquivo); // Garantir que os dados são escritos no arquivo imediatamente
}



void MenuPrincipal(int x) {
    FILE *Arquivo;
    FILE *arquivoFuncionarios;
    conta conta;
    char opcao, alo;
    int senha, numero_conta;
    int senha_admin = 1234;

    Arquivo = fopen("banco.txt", "r+b");
    if (Arquivo == NULL) {
        Arquivo = fopen("banco.txt", "w+b");
    }
 
    if (Arquivo != NULL) {
        do {
            system("cls");
            printf("\n888b     d888  .d8888b.     888    d8P      888888b.          d8888 888b    888 888    d8P ");
				printf("\n8888b   d8888 d88P   88b    888   d8P       888   88b        d88888 8888b   888 888   d8P    ");
				printf("\n88888b.d88888 Y88b. d88P    888  d8P        888  .88P       d88P888 88888b  888 888  d8P     ");
				printf("\n888Y88888P888   Y8888P      888d88K         8888888K.      d88P 888 888Y88b 888 888d88K    ");  
				printf("\n888 Y888P 888 .d88P88K.d88P 8888888b        888   Y88b    d88P  888 888 Y88b888 8888888b    "); 
				printf("\n888  Y8P  888 888   Y888P   888  Y88b       888    888   d88P   888 888  Y88888 888  Y88b   "); 
				printf("\n888       888 Y88b .d8888b  888   Y88b      888   d88P  d8888888888 888   Y8888 888   Y88b   ");
				printf("\n888       888   Y8888P  Y88b888    Y88b     8888888P   d88P     888 888    Y888 888    Y88b \n ");
				
            	
            printf("$$$$MENU INICIAL$$$$\n");
            printf("\nFUNCIONARIO [1]\n");
            printf("\nCLIENTE [2]\n");
            printf("\nSAIR [3]\n"); 
            opcao = getch();
            switch (opcao) {
                case '1':
                    system("cls");
                    printf("\nDigite a senha do Administrador: ");
                    scanf("%d", &senha);
                    while (getchar() != '\n');
                    if (senha == senha_admin) {
                        do {
                            system("cls");
                            printf("$$$$MENU FUNCIONARIO$$$$\n");
                            printf("\nABERTURA DE CONTA [1]\n");
                            printf("\nENCERRAMENTO DE CONTA [2]\n");
                            printf("\nALTERAR DADOS [3]\n");
                            printf("\nCONSULTAR CONTA [4]\n");
                            printf("\nCADASTRAMENTO DE FUNCIONARIOS [5]\n");
                            printf("\nGERAR RELATORIOS [6]\n");
                            printf("\nSAIR [7]\n");
                            
                            alo = getch();
                            switch (alo) {
                                case '1':
                                    system("cls");
                                    printf("\n");
                                    printf("CONTA POUPANCA - CP [1]");
                                    printf("\n\nCONTA CORRENTE - CC [2]");
                                    printf("\n\n VOLTAR [3]");
                                    
                                    do {
                                        alo = getch();
                                        switch (alo) {
                                            case '1':
                                                system("cls");
                                                printf("\n$$$$ ABERTURA CONTA POUPANCA $$$$");
                                                
                                                printf("\n\nAGENCIA:  ");
                                                scanf("%d", &conta.agencia);
                                                while (getchar() != '\n');
                                                
                                                printf("NUMERO DA CONTA: ");
                                                scanf("%d", &conta.numero_conta);
                                                while (getchar() != '\n');
                                                
                                                printf("CPF: ");
                                                scanf("%d", &conta.cpf);
                                                while (getchar() != '\n');
                                                
                                                printf("DATA DE NASCIMENTO: ");
                                                
                                                while (getchar() != '\n');
                                                
                                                printf("ESTADO: ");
                                                scanf("%d", &conta.estado);
                                                while (getchar() != '\n');
                                                
                                                printf("CIDADE: ");
                                                scanf("%d", &conta.cidade);
                                                while (getchar() != '\n');
                                                
                                                printf("BAIRRO: ");
                                                scanf("%d", &conta.bairro);
                                                while (getchar() != '\n');
                                                
                                                printf("CEP: ");
                                                scanf("%d", &conta.cep);
                                                while (getchar() != '\n');
                                                
                                                printf("TELEFONE DE CONTATO: ");
                                                scanf("%d", &conta.tel_ctt);
                                                while (getchar() != '\n');
                                                
                                                
                                                printf("SENHA: ");
                                                scanf("%d", &conta.senha);
                                                while (getchar() != '\n');
                                                
                                                printf("NOME: ");
                                                fgets(conta.nome, sizeof(conta.nome), stdin);
                                                conta.nome[strcspn(conta.nome, "\n")] = 0; // Remove newline
                                                
                                                
                                                
                                                conta.tipo = 'P';
                                                conta.saldo = 0.0;
                                                
                                                fseek(Arquivo, 0, SEEK_END);
                                                fwrite(&conta, sizeof(conta), 1, Arquivo);
                                                fflush(Arquivo);
                                                
                                                printf("\nConta poupança criada com sucesso!");
                                                getch();
                                                break;

                                            case '2':
                                                system("cls");
                                                printf("\n$$$$ ABERTURA CONTA CORRENTE $$$$");
                                                
                                                printf("\n\nAGENCIA :  ");
                                                scanf("%d", &conta.agencia);
                                                while (getchar() != '\n');
                                                
                                                printf("NUMERO DA CONTA : ");
                                                scanf("%d", &conta.numero_conta);
                                                while (getchar() != '\n');
                                                
                                                printf("LIMITE DA CONTA : ");
                                                scanf("%d", &conta.limite);
                                                while (getchar() != '\n');
                                                
                                                printf("DATA DE VENCIMENTO : ");
                                                scanf("%d", &conta.ano_nascimento);
                                                while (getchar() != '\n');
                                                
                                                printf("CPF: ");
                                                scanf("%d", &conta.cpf);
                                                while (getchar() != '\n');
                                                
                                                printf("DATA DE NASCIMENTO : ");
                                                scanf("%d", &conta.ano_nascimento);
                                                while (getchar() != '\n');
                                                
                                                printf("TELEFONE DE CONTATO : ");
                                                scanf("%d", &conta.tel_ctt);
                                                while (getchar() != '\n');
                                                
                                                printf("CEP: ");
                                                scanf("%d", &conta.cep);
                                                while (getchar() != '\n');
                                                
                                                printf("NUMERO DA CASA : ");
                                                scanf("%d", &conta.num_casa);
                                                while (getchar() != '\n');
                                                
                                                printf("BAIRRO : ");
                                                scanf("%d", &conta.bairro);
                                                while (getchar() != '\n');
                                                
                                                printf("CIDADE : ");
                                                scanf("%d", &conta.cidade);
                                                while (getchar() != '\n');
                                                
                                                printf("ESTADO : ");
                                                scanf("%d", &conta.estado);
                                                while (getchar() != '\n');
                                                
                                                printf("SENHA : ");
                                                scanf("%d", &conta.senha);
                                                while (getchar() != '\n');

                                                printf("NOME: ");
                                                fgets(conta.nome, sizeof(conta.nome), stdin);
                                                conta.nome[strcspn(conta.nome, "\n")] = 0; // Remove newline
                                                
                                                conta.tipo = 'C';
                                                conta.saldo = 0.0;
                                                
                                                fseek(Arquivo, 0, SEEK_END);
                                                fwrite(&conta, sizeof(conta), 1, Arquivo);
                                                fflush(Arquivo);
                                                
                                                printf("\nConta corrente criada com sucesso!");
                                                getch();
                                                break;

                                            case '3':
                                                break;

                                            default:
                                                printf("\nOpcao invalida!");
                                                getch();
                                                break;
                                        }
                                    } while (alo != '3');
                                    break;

                                case '2':
                                  
									// Encerramento de conta
                                    system("cls");
                                    printf("\n$$$$ ENCERRAMENTO DE CONTA $$$$\n");
                                    printf("NUMERO DA CONTA: ");
                                    scanf("%d", &numero_conta);
                                    while (getchar() != '\n');

                                    buscarConta(Arquivo, numero_conta, &conta);
                                    if (conta.numero_conta != -1) {
                                        encerrarConta(&conta, Arquivo);
                                    } else {
                                        printf("Conta nao encontrada.\n");
                                    }
                                    getchar();
                                    break;
                                    
                                    
									
									// Implementar lógica de encerramento de conta
                                    

                                case '3':
                                system("cls");
                                 printf("\n$$$$ ALTERAR DADOS DA CONTA $$$$\n");
                                  printf("NUMERO DA CONTA: ");
                                    scanf("%d", &numero_conta);
                                   while (getchar() != '\n'); // Limpar o buffer de entrada

                                    buscarConta(Arquivo, numero_conta, &conta);
   
                     if (conta.numero_conta != -1) {
        char novoNome[50], novoTelefone[20], novoLocal[50], novoCep[10], novoBairro[30], novaCidade[30], novoEstado[3];
        double novoSaldo;
        int novoNumeroCasa;

        printf("NOVO NOME DO CLIENTE: ");
        fgets(novoNome, sizeof(novoNome), stdin);
        novoNome[strcspn(novoNome, "\n")] = 0;

        printf("NOVO TELEFONE: ");
        fgets(novoTelefone, sizeof(novoTelefone), stdin);
        novoTelefone[strcspn(novoTelefone, "\n")] = 0;

        printf("NOVO LOCAL: ");
        fgets(novoLocal, sizeof(novoLocal), stdin);
        novoLocal[strcspn(novoLocal, "\n")] = 0;

        printf("NOVO NUMERO DA CASA: ");
        scanf("%d", &novoNumeroCasa);
        while (getchar() != '\n');

        printf("NOVO CEP: ");
        fgets(novoCep, sizeof(novoCep), stdin);
        novoCep[strcspn(novoCep, "\n")] = 0;

        printf("NOVO BAIRRO: ");
        fgets(novoBairro, sizeof(novoBairro), stdin);
        novoBairro[strcspn(novoBairro, "\n")] = 0;

        printf("NOVA CIDADE: ");
        fgets(novaCidade, sizeof(novaCidade), stdin);
        novaCidade[strcspn(novaCidade, "\n")] = 0;

        printf("NOVO ESTADO: ");
        fgets(novoEstado, sizeof(novoEstado), stdin);
        novoEstado[strcspn(novoEstado, "\n")] = 0;

        printf("NOVO SALDO: ");
        scanf("%lf", &novoSaldo);
        while (getchar() != '\n');

        alterarDadosConta(&conta, Arquivo, novoNome, novoSaldo, novoTelefone, novoLocal,
                          novoNumeroCasa, novoCep, novoBairro, novaCidade, novoEstado);
    } else {
        printf("Conta não encontrada.\n");
    }

                    getchar();
                       break;


                                case '4':
                                    // Consultar conta
                                    
                                    system("cls");
                                    printf("\n$$$$ CONSULTAR CONTA $$$$\n");
                                    printf("NÚMERO DA CONTA: ");
                                    scanf("%d", &numero_conta);
                                    while (getchar() != '\n');

                                    buscarConta(Arquivo, numero_conta, &conta);
                                    if (conta.numero_conta != -1) {
                                        exibirConta(&conta);
                                    } else {
                                        printf("Conta não encontrada.\n");
                                    }
                                    getchar();
                                    
                                    
                                    
                                    break;

                                case '5':
                                    // Cadastramento de funcionários
                                    cadastrarFuncionario(arquivoFuncionarios);
                                    // Implementar lógica de cadastramento de funcionários
                                    break;

                                case '6':{
								
                                    fclose(Arquivo);
                                    // Gerar relatóriosq
                                    system("dados.xlsx");
                                    gerarRelatorios(Arquivo);
                                    Arquivo = fopen("banco.txt", "r+b");
                                    // Implementar lógica de geração de relatórios
                                    break;
							}
                                case '7':
                                	system("cls");
                                       printf("Saindo do menu do funcionario...\n");
                                    getch();
                                    break;

                                default:
                                    printf("\nOpcao invalida!");
                                    getch();
                                    break;
                            }
                        } while (alo != '7');
                    } else {
                        printf("\nSenha invalida!");
                        getch();
                    }
                    break;

                case '2':
                    // Lógica para o menu do cliente
                    system("cls");
                    printf("\nDigite o numero da sua conta: ");
                    scanf("%d", &numero_conta);
                    buscarConta(Arquivo, numero_conta, &conta);
                    if (conta.numero_conta != -1 && conta.ativa) {
                        char opcao_cliente;
                        do {
                            system("cls");
                            printf("$$$$MENU CLIENTE$$$$\n");
                            printf("\nVER SALDO [1]\n");
                            printf("\nDEPOSITO[2]\n");
                            printf("\nSAQUE[3]\n");
                            printf("\nEXTRATO[4]\n");
                            printf("\nCONSULTAR LIMITE [5]\n");
                            printf("\nSAIR [6]\n");

                            opcao_cliente = getch();
                            switch (opcao_cliente) {
                                case '1':
                                	
                                	 system("cls");
                    				printf("Saldo atual: %.2f\n", conta.saldo);
                    				getch();
                    				break;
                                    
                                    case '2': 
                                    system("cls");
    								double valor_deposito;
								    printf("\nDigite o valor a ser depositado: ");
								    scanf("%lf", &valor_deposito);
								    inserirSaldo(&conta, Arquivo, valor_deposito);
								    printf("\nDepósito realizado com sucesso!\n");
								    getch();
								    break;    
								    
								    case '3':
								    system("cls");
								    double valor_saque;
								    printf("\nDigite o valor a ser sacado: ");
								    scanf("%lf", &valor_saque);
								    getch();
								    
								    if (valor_saque <= 0) {
								        printf("\nValor de saque inválido.\n");
								        getch();
								        break;
								    }
								    
								    if (valor_saque > conta.saldo) {
								        printf("\nSaldo insuficiente para realizar o saque.\n");
								    } else {
								        conta.saldo -= valor_saque;
								        printf("\nSaque de %.2f realizado com sucesso!\n", valor_saque);
								        // Atualizar o arquivo com os novos dados da conta
								        fseek(Arquivo, -sizeof(conta), SEEK_CUR);
								        fwrite(&conta, sizeof(conta), 1, Arquivo);
								        fflush(Arquivo);
								    }
								    getch();
								    break;
								   
								   case '4':
                    
                                     // Lógica para exibir o extrato
                    
                                    break;
								   
								   
								    case '5':
                                      system("cls");
                    // Lógica para consultar o limite
                                       printf("Limite atual: %.2f\n", conta.limite);
                                    getch();
                                    break;

                                    case '6':
                    // Sair do menu cliente
                                      system("cls");
                                       printf("Saindo do menu do cliente...\n");
                                    getch();
                                    break;

                                    default:
                                    system("cls");
                                    printf("Opcao invalida! Tente novamente.\n");
                                    getch();
                                    break;
        }
        
        
    } while (opcao_cliente != '6');


} else {
	
    printf("Conta não encontrada ou inativa.\n");
    getch();
}

                    break;

                case '3':
                    fclose(Arquivo);
                    return;

                default:
                    printf("\nOpcao invalida!");
                    getch();
                    break;
            }
        } while (opcao != '3');
    } else {
        printf("\nErro ao abrir o arquivo!");
    }
}

int main() {
	
    MenuPrincipal(0);
    setlocale (LC_ALL, "");
    return 0;
}
