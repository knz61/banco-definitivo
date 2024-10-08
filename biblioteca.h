typedef struct {
    int agencia;
    int numero_conta;
    char tipo; // 'C' para corrente, 'P' para poupança
    char nome[50];
    double saldo;
    int cpf;
    int dia_nascimento;
    int mes_nascimento;
    int ano_nascimento;
    int tel_ctt;
    char telefone[20];
    char cep[9];
    int num_casa;
    char bairro[50];
    char cidade[50];
    char estado[50];
    int senha;
    int limite;
    int dia_vencimento;
    int mes_vencimento;
    int ano_vencimento;
    int ativa;
    int tipo_conta;
    int data_vencimento;
    char endereco_local[50];
    int numero_casa;
    char nome_cliente[50];
} conta;

typedef struct {
      char dia[2];
      char mes[2];
      char ano[4];
}data;

typedef struct {
      int cep [9];
      int num_casa;
      char bairro[100];
      char cidade[100];
      char estado[100];
}endereco; 

typedef struct {
	int agencia;
    int numero_conta;
    char tipo; // 'C' para corrente, 'P' para poupança
    char nome[50];
    double saldo;
    int cpf;
    data data_nascimento;
    endereco Endereco;
    int tel_ctt;
    
}funcionario;


 void MenuPrincipal();
 int Insere_conta_cc;
 int Insere_conta_cp;
