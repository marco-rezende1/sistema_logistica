#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h>

typedef struct {
    char rua[100];
    char bairro[100];
    char cidade[50];
    char numero_endereco[10];
} Endereco;

typedef struct {
    int  id_cliente;
    char nome_completo[200];
    char cpf_cnpj[20];
    char telefone[17];
    char email[100];
    char endereco[200];
} Cliente;

typedef struct {
    int   id_carga;
    char  descricao[200];
    float peso;
    char  dimensoes[50];
    char  tipo_material[100];
    float valor_declarado;
    char  origem[80];
    char  destino[78];
    char  observacoes[80];
} Carga;

typedef struct {
    int   id_entrega;
    int   id_cliente;
    char  nome_completo[100];
    int   id_carga;
    char  descricao[70];
    char  data_coleta[80];
    char  data_entrega_prevista[80];
    char  data_entrega_realizada[80];
    char  status_entrega[80];
    char  localizacao_atual[80];
    char  motorista_responsavel[80];
    char  veiculo_utilizado[80];
    float valor_frete;
} Entrega;

void mostrar_menu_inicial(void);
void mostrar_menu_clientes(void);
void mostrar_menu_cargas(void);
void mostrar_menu_entregas(void);
void handle_escolhas_menu(int);

void carregar_clientes(void);
void salvar_clientes(void);
void carregar_cargas(void);
void salvar_cargas(void);
void carregar_entregas(void);
void salvar_entregas(void);
void pre_carregar_dados(void);
void sair_programa(void);

static void limpa_stdin(void);

void cadastrar_cliente(Cliente **clientes, int *qtdClientes);
int  buscar_cliente(void);
int  buscar_cliente_porID(int);
void mostrar_cliente(int index_cliente);
void exibir_cliente(int index_cliente);
void listar_todos_clientes(void);
void editar_cliente(void);
void deletar_cliente(void);

void cadastrar_carga(Carga **pcargas, int *pqtdCargas);
int  buscar_carga(void);
void mostrar_carga(int index_carga);
void listar_todas_cargas(void);
void editar_carga(void);
void deletar_carga(void);

void cadastrar_entrega(Entrega **pentregas, int *pqtdEntregas);
int  buscar_entrega(void);
void mostrar_entrega(int index_entrega);
void listar_todas_entregas(void);
void editar_entrega(void);
void deletar_entrega(void);

void menu_relatorios(void);
void relatorio_por_periodo(void);
void relatorio_por_status(void);
void relatorio_por_cliente(void);
void relatorio_por_motorista(void);

Cliente *clientes = NULL;
Carga *cargas = NULL;
Entrega *entregas = NULL;
int qtdClientes = 0;
int qtdCargas = 0;
int qtdEntregas = 0;

void carregar_clientes(void) {
    FILE *arquivo = fopen("clientes.bin", "rb");
    if (arquivo == NULL) {

        qtdClientes = 0;
        clientes = NULL;
        return;
    }


    if (fread(&qtdClientes, sizeof(int), 1, arquivo) != 1) {
        qtdClientes = 0;
        clientes = NULL;
        fclose(arquivo);
        return;
    }

    if (qtdClientes > 0) {

        clientes = malloc((size_t)qtdClientes * sizeof(Cliente));
        if (clientes == NULL) {
            fprintf(stderr, "Erro de memoria ao carregar clientes!\n");
            qtdClientes = 0;
            fclose(arquivo);
            return;
        }


        if (fread(clientes, sizeof(Cliente), (size_t)qtdClientes, arquivo) != (size_t)qtdClientes) {
            fprintf(stderr, "Erro ao ler dados dos clientes!\n");
            free(clientes);
            clientes = NULL;
            qtdClientes = 0;
        }
    } else {
        clientes = NULL;
    }

    fclose(arquivo);
}

void salvar_clientes(void) {
    FILE *arquivo = fopen("clientes.bin", "wb");
    if (arquivo == NULL) {
        fprintf(stderr, "Erro ao criar arquivo de clientes!\n");
        return;
    }


    if (fwrite(&qtdClientes, sizeof(int), 1, arquivo) != 1) {
        fprintf(stderr, "Erro ao escrever quantidade de clientes!\n");
        fclose(arquivo);
        return;
    }


    if (qtdClientes > 0 && clientes != NULL) {
        if (fwrite(clientes, sizeof(Cliente), (size_t)qtdClientes, arquivo) != (size_t)qtdClientes) {
            fprintf(stderr, "Erro ao escrever dados dos clientes!\n");
        }
    }

    fclose(arquivo);
}

void carregar_cargas(void) {
    FILE *arquivo = fopen("cargas.bin", "rb");
    if (arquivo == NULL) {

        qtdCargas = 0;
        cargas = NULL;
        return;
    }


    if (fread(&qtdCargas, sizeof(int), 1, arquivo) != 1) {
        qtdCargas = 0;
        cargas = NULL;
        fclose(arquivo);
        return;
    }

    if (qtdCargas > 0) {

        cargas = malloc((size_t)qtdCargas * sizeof(Carga));
        if (cargas == NULL) {
            fprintf(stderr, "Erro de memoria ao carregar cargas!\n");
            qtdCargas = 0;
            fclose(arquivo);
            return;
        }


        if (fread(cargas, sizeof(Carga), (size_t)qtdCargas, arquivo) != (size_t)qtdCargas) {
            fprintf(stderr, "Erro ao ler dados das cargas!\n");
            free(cargas);
            cargas = NULL;
            qtdCargas = 0;
        }
    } else {
        cargas = NULL;
    }

    fclose(arquivo);
}

void salvar_cargas(void) {
    FILE *arquivo = fopen("cargas.bin", "wb");
    if (arquivo == NULL) {
        fprintf(stderr, "Erro ao criar arquivo de cargas!\n");
        return;
    }


    if (fwrite(&qtdCargas, sizeof(int), 1, arquivo) != 1) {
        fprintf(stderr, "Erro ao escrever quantidade de cargas!\n");
        fclose(arquivo);
        return;
    }


    if (qtdCargas > 0 && cargas != NULL) {
        if (fwrite(cargas, sizeof(Carga), (size_t)qtdCargas, arquivo) != (size_t)qtdCargas) {
            fprintf(stderr, "Erro ao escrever dados das cargas!\n");
        }
    }

    fclose(arquivo);
}

void carregar_entregas(void) {
    FILE *arquivo = fopen("entregas.bin", "rb");
    if (arquivo == NULL) {

        qtdEntregas = 0;
        entregas = NULL;
        return;
    }


    if (fread(&qtdEntregas, sizeof(int), 1, arquivo) != 1) {
        qtdEntregas = 0;
        entregas = NULL;
        fclose(arquivo);
        return;
    }

    if (qtdEntregas > 0) {

        entregas = malloc((size_t)qtdEntregas * sizeof(Entrega));
        if (entregas == NULL) {
            fprintf(stderr, "Erro de memoria ao carregar entregas!\n");
            qtdEntregas = 0;
            fclose(arquivo);
            return;
        }


        if (fread(entregas, sizeof(Entrega), (size_t)qtdEntregas, arquivo) != (size_t)qtdEntregas) {
            fprintf(stderr, "Erro ao ler dados das entregas!\n");
            free(entregas);
            entregas = NULL;
            qtdEntregas = 0;
        }
    } else {
        entregas = NULL;
    }

    fclose(arquivo);
}

void salvar_entregas(void) {
    FILE *arquivo = fopen("entregas.bin", "wb");
    if (arquivo == NULL) {
        fprintf(stderr, "Erro ao criar arquivo de entregas!\n");
        return;
    }


    if (fwrite(&qtdEntregas, sizeof(int), 1, arquivo) != 1) {
        fprintf(stderr, "Erro ao escrever quantidade de entregas!\n");
        fclose(arquivo);
        return;
    }


    if (qtdEntregas > 0 && entregas != NULL) {
        if (fwrite(entregas, sizeof(Entrega), (size_t)qtdEntregas, arquivo) != (size_t)qtdEntregas) {
            fprintf(stderr, "Erro ao escrever dados das entregas!\n");
        }
    }

    fclose(arquivo);
}

void sair_programa(void) {
    salvar_clientes();
    salvar_cargas();
    salvar_entregas();
    free(clientes);
    free(cargas);
    free(entregas);
    printf("Dados salvos com sucesso! Encerrando programa...\n");
    exit(0);
}

static void limpa_stdin(void) {
    int ch;
    while ((ch = getchar()) != '\n' && ch != EOF) {}
}

static int proximo_id_cliente_disponivel(void) {
    int max_id = 0;
    for (int i = 0; i < qtdClientes; i++) {
        if (clientes[i].id_cliente > max_id) {
            max_id = clientes[i].id_cliente;
        }
    }
    return max_id + 1;
}

static int proximo_id_carga_disponivel(void) {
    int max_id = 0;
    for (int i = 0; i < qtdCargas; i++) {
        if (cargas[i].id_carga > max_id) {
            max_id = cargas[i].id_carga;
        }
    }
    return max_id + 1;
}

static int proximo_id_entrega_disponivel(void) {
    int max_id = 0;
    for (int i = 0; i < qtdEntregas; i++) {
        if (entregas[i].id_entrega > max_id) {
            max_id = entregas[i].id_entrega;
        }
    }
    return max_id + 1;
}

static int cpf_cnpj_ja_existe(const char *cpf_cnpj) {
    for (int i = 0; i < qtdClientes; i++) {
        if (strcmp(clientes[i].cpf_cnpj, cpf_cnpj) == 0) {
            return 1;
        }
    }
    return 0;
}

static int id_cliente_ja_existe(int id) {
    for (int i = 0; i < qtdClientes; i++) {
        if (clientes[i].id_cliente == id) {
            return 1;
        }
    }
    return 0;
}

static int id_carga_ja_existe(int id) {
    for (int i = 0; i < qtdCargas; i++) {
        if (cargas[i].id_carga == id) {
            return 1;
        }
    }
    return 0;
}

static int id_entrega_ja_existe(int id) {
    for (int i = 0; i < qtdEntregas; i++) {
        if (entregas[i].id_entrega == id) {
            return 1;
        }
    }
    return 0;
}

static int cliente_tem_entregas(int id_cliente) {
    for (int i = 0; i < qtdEntregas; i++) {
        if (entregas[i].id_cliente == id_cliente) {
            return 1;
        }
    }
    return 0;
}

static int carga_tem_entregas(int id_carga) {
    for (int i = 0; i < qtdEntregas; i++) {
        if (entregas[i].id_carga == id_carga) {
            return 1;
        }
    }
    return 0;
}

void pre_carregar_dados(void) {
    int escolha = 0;

#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif

    printf("Pre-carregar Dados de Exemplo\n");
    printf("========================================\n\n");

    if (qtdClientes > 0 || qtdCargas > 0 || qtdEntregas > 0) {
        printf("ATENCAO: Ja existem dados cadastrados!\n");
        printf("Esta operacao ira adicionar dados de exemplo aos existentes.\n\n");
        printf("Deseja continuar?\n");
        printf("\t[1] Sim, adicionar dados de exemplo\n");
        printf("\t[2] Nao, cancelar operacao\n");

        for (;;) {
            if (scanf("%d", &escolha) != 1) { limpa_stdin(); continue; }
            limpa_stdin();
            if (escolha == 1 || escolha == 2) break;
            printf("Escolha invalida!\n");
        }

        if (escolha == 2) {
            mostrar_menu_inicial();
            return;
        }
    }

    printf("Pre-carregando dados de exemplo...\n\n");

    Cliente clientes_exemplo[10];

    // Inicializar todos os arrays de caracteres com zeros
    for (int i = 0; i < 10; i++) {
        memset(clientes_exemplo[i].nome_completo, 0, sizeof(clientes_exemplo[i].nome_completo));
        memset(clientes_exemplo[i].cpf_cnpj, 0, sizeof(clientes_exemplo[i].cpf_cnpj));
        memset(clientes_exemplo[i].telefone, 0, sizeof(clientes_exemplo[i].telefone));
        memset(clientes_exemplo[i].email, 0, sizeof(clientes_exemplo[i].email));
        memset(clientes_exemplo[i].endereco, 0, sizeof(clientes_exemplo[i].endereco));
    }

    // Inicializar clientes exemplo
    strcpy(clientes_exemplo[0].nome_completo, "Joao Silva Santos");
    strcpy(clientes_exemplo[0].cpf_cnpj, "123.456.789-00");
    strcpy(clientes_exemplo[0].telefone, "(11) 99999-1111");
    strcpy(clientes_exemplo[0].email, "joao@email.com");
    strcpy(clientes_exemplo[0].endereco, "Rua das Flores, 123, Centro, Sao Paulo");

    strcpy(clientes_exemplo[1].nome_completo, "Maria Oliveira Costa");
    strcpy(clientes_exemplo[1].cpf_cnpj, "987.654.321-00");
    strcpy(clientes_exemplo[1].telefone, "(11) 99999-2222");
    strcpy(clientes_exemplo[1].email, "maria@email.com");
    strcpy(clientes_exemplo[1].endereco, "Av. Paulista, 456, Bela Vista, Sao Paulo");

    strcpy(clientes_exemplo[2].nome_completo, "Pedro Santos Lima");
    strcpy(clientes_exemplo[2].cpf_cnpj, "456.789.123-00");
    strcpy(clientes_exemplo[2].telefone, "(11) 99999-3333");
    strcpy(clientes_exemplo[2].email, "pedro@email.com");
    strcpy(clientes_exemplo[2].endereco, "Rua Augusta, 789, Consolacao, Sao Paulo");

    strcpy(clientes_exemplo[3].nome_completo, "Ana Paula Ferreira");
    strcpy(clientes_exemplo[3].cpf_cnpj, "789.123.456-00");
    strcpy(clientes_exemplo[3].telefone, "(11) 99999-4444");
    strcpy(clientes_exemplo[3].email, "ana@email.com");
    strcpy(clientes_exemplo[3].endereco, "Rua Oscar Freire, 321, Jardins, Sao Paulo");

    strcpy(clientes_exemplo[4].nome_completo, "Carlos Eduardo Souza");
    strcpy(clientes_exemplo[4].cpf_cnpj, "321.654.987-00");
    strcpy(clientes_exemplo[4].telefone, "(11) 99999-5555");
    strcpy(clientes_exemplo[4].email, "carlos@email.com");
    strcpy(clientes_exemplo[4].endereco, "Av. Faria Lima, 654, Itaim Bibi, Sao Paulo");

    strcpy(clientes_exemplo[5].nome_completo, "Lucia Mendes Alves");
    strcpy(clientes_exemplo[5].cpf_cnpj, "654.321.789-00");
    strcpy(clientes_exemplo[5].telefone, "(11) 99999-6666");
    strcpy(clientes_exemplo[5].email, "lucia@email.com");
    strcpy(clientes_exemplo[5].endereco, "Rua Haddock Lobo, 987, Cerqueira Cesar, Sao Paulo");

    strcpy(clientes_exemplo[6].nome_completo, "Roberto Carlos Silva");
    strcpy(clientes_exemplo[6].cpf_cnpj, "147.258.369-00");
    strcpy(clientes_exemplo[6].telefone, "(11) 99999-7777");
    strcpy(clientes_exemplo[6].email, "roberto@email.com");
    strcpy(clientes_exemplo[6].endereco, "Av. Reboucas, 147, Pinheiros, Sao Paulo");

    strcpy(clientes_exemplo[7].nome_completo, "Fernanda Costa Lima");
    strcpy(clientes_exemplo[7].cpf_cnpj, "258.369.147-00");
    strcpy(clientes_exemplo[7].telefone, "(11) 99999-8888");
    strcpy(clientes_exemplo[7].email, "fernanda@email.com");
    strcpy(clientes_exemplo[7].endereco, "Rua Teodoro Sampaio, 258, Pinheiros, Sao Paulo");

    strcpy(clientes_exemplo[8].nome_completo, "Marcos Antonio Santos");
    strcpy(clientes_exemplo[8].cpf_cnpj, "369.147.258-00");
    strcpy(clientes_exemplo[8].telefone, "(11) 99999-9999");
    strcpy(clientes_exemplo[8].email, "marcos@email.com");
    strcpy(clientes_exemplo[8].endereco, "Av. Brigadeiro Luiz Antonio, 369, Bela Vista, Sao Paulo");

    strcpy(clientes_exemplo[9].nome_completo, "Juliana Pereira Rocha");
    strcpy(clientes_exemplo[9].cpf_cnpj, "741.852.963-00");
    strcpy(clientes_exemplo[9].telefone, "(11) 99999-0000");
    strcpy(clientes_exemplo[9].email, "juliana@email.com");
    strcpy(clientes_exemplo[9].endereco, "Rua da Consolacao, 741, Consolacao, Sao Paulo");

    Carga cargas_exemplo[10];

    // Inicializar todos os arrays de caracteres com zeros
    for (int i = 0; i < 10; i++) {
        memset(cargas_exemplo[i].descricao, 0, sizeof(cargas_exemplo[i].descricao));
        memset(cargas_exemplo[i].dimensoes, 0, sizeof(cargas_exemplo[i].dimensoes));
        memset(cargas_exemplo[i].tipo_material, 0, sizeof(cargas_exemplo[i].tipo_material));
        memset(cargas_exemplo[i].origem, 0, sizeof(cargas_exemplo[i].origem));
        memset(cargas_exemplo[i].destino, 0, sizeof(cargas_exemplo[i].destino));
        memset(cargas_exemplo[i].observacoes, 0, sizeof(cargas_exemplo[i].observacoes));
    }

    // Inicializar cargas exemplo
    strcpy(cargas_exemplo[0].descricao, "Eletrodomesticos - Geladeira");
    cargas_exemplo[0].peso = 45.5;
    strcpy(cargas_exemplo[0].dimensoes, "60x60x180 cm");
    strcpy(cargas_exemplo[0].tipo_material, "Eletrodomesticos");
    cargas_exemplo[0].valor_declarado = 2500.00;
    strcpy(cargas_exemplo[0].origem, "Sao Paulo");
    strcpy(cargas_exemplo[0].destino, "Rio de Janeiro");
    strcpy(cargas_exemplo[0].observacoes, "Fragil - Manter em pe");

    strcpy(cargas_exemplo[1].descricao, "Moveis - Mesa de Jantar");
    cargas_exemplo[1].peso = 25.0;
    strcpy(cargas_exemplo[1].dimensoes, "120x80x75 cm");
    strcpy(cargas_exemplo[1].tipo_material, "Moveis");
    cargas_exemplo[1].valor_declarado = 800.00;
    strcpy(cargas_exemplo[1].origem, "Sao Paulo");
    strcpy(cargas_exemplo[1].destino, "Belo Horizonte");
    strcpy(cargas_exemplo[1].observacoes, "Desmontada");

    strcpy(cargas_exemplo[2].descricao, "Roupas - Lote Variado");
    cargas_exemplo[2].peso = 15.0;
    strcpy(cargas_exemplo[2].dimensoes, "50x40x30 cm");
    strcpy(cargas_exemplo[2].tipo_material, "Têxtil");
    cargas_exemplo[2].valor_declarado = 500.00;
    strcpy(cargas_exemplo[2].origem, "Sao Paulo");
    strcpy(cargas_exemplo[2].destino, "Salvador");
    strcpy(cargas_exemplo[2].observacoes, "Roupas de inverno");

    strcpy(cargas_exemplo[3].descricao, "Livros - Colecao Academica");
    cargas_exemplo[3].peso = 8.5;
    strcpy(cargas_exemplo[3].dimensoes, "30x20x15 cm");
    strcpy(cargas_exemplo[3].tipo_material, "Papel");
    cargas_exemplo[3].valor_declarado = 300.00;
    strcpy(cargas_exemplo[3].origem, "Sao Paulo");
    strcpy(cargas_exemplo[3].destino, "Brasilia");
    strcpy(cargas_exemplo[3].observacoes, "Livros didaticos");

    strcpy(cargas_exemplo[4].descricao, "Ferramentas - Kit Completo");
    cargas_exemplo[4].peso = 12.0;
    strcpy(cargas_exemplo[4].dimensoes, "40x30x20 cm");
    strcpy(cargas_exemplo[4].tipo_material, "Metal");
    cargas_exemplo[4].valor_declarado = 450.00;
    strcpy(cargas_exemplo[4].origem, "Sao Paulo");
    strcpy(cargas_exemplo[4].destino, "Recife");
    strcpy(cargas_exemplo[4].observacoes, "Ferramentas profissionais");

    strcpy(cargas_exemplo[5].descricao, "Cosmeticos - Lote Variado");
    cargas_exemplo[5].peso = 5.0;
    strcpy(cargas_exemplo[5].dimensoes, "25x20x10 cm");
    strcpy(cargas_exemplo[5].tipo_material, "Cosmeticos");
    cargas_exemplo[5].valor_declarado = 200.00;
    strcpy(cargas_exemplo[5].origem, "Sao Paulo");
    strcpy(cargas_exemplo[5].destino, "Fortaleza");
    strcpy(cargas_exemplo[5].observacoes, "Produtos de beleza");

    strcpy(cargas_exemplo[6].descricao, "Eletronicos - Notebook");
    cargas_exemplo[6].peso = 2.5;
    strcpy(cargas_exemplo[6].dimensoes, "35x25x5 cm");
    strcpy(cargas_exemplo[6].tipo_material, "Eletronicos");
    cargas_exemplo[6].valor_declarado = 3500.00;
    strcpy(cargas_exemplo[6].origem, "Sao Paulo");
    strcpy(cargas_exemplo[6].destino, "Manaus");
    strcpy(cargas_exemplo[6].observacoes, "Equipamento sensivel");

    strcpy(cargas_exemplo[7].descricao, "Alimentos - Produtos Organicos");
    cargas_exemplo[7].peso = 20.0;
    strcpy(cargas_exemplo[7].dimensoes, "60x40x25 cm");
    strcpy(cargas_exemplo[7].tipo_material, "Alimentos");
    cargas_exemplo[7].valor_declarado = 150.00;
    strcpy(cargas_exemplo[7].origem, "Sao Paulo");
    strcpy(cargas_exemplo[7].destino, "Curitiba");
    strcpy(cargas_exemplo[7].observacoes, "Refrigeracao necessaria");

    strcpy(cargas_exemplo[8].descricao, "Esportes - Equipamentos");
    cargas_exemplo[8].peso = 18.0;
    strcpy(cargas_exemplo[8].dimensoes, "80x40x30 cm");
    strcpy(cargas_exemplo[8].tipo_material, "Esportes");
    cargas_exemplo[8].valor_declarado = 600.00;
    strcpy(cargas_exemplo[8].origem, "Sao Paulo");
    strcpy(cargas_exemplo[8].destino, "Porto Alegre");
    strcpy(cargas_exemplo[8].observacoes, "Equipamentos de academia");

    strcpy(cargas_exemplo[9].descricao, "Decoracao - Quadros e Objetos");
    cargas_exemplo[9].peso = 7.0;
    strcpy(cargas_exemplo[9].dimensoes, "50x40x20 cm");
    strcpy(cargas_exemplo[9].tipo_material, "Decoracao");
    cargas_exemplo[9].valor_declarado = 350.00;
    strcpy(cargas_exemplo[9].origem, "Sao Paulo");
    strcpy(cargas_exemplo[9].destino, "Goiania");
    strcpy(cargas_exemplo[9].observacoes, "Objetos decorativos");

    Entrega entregas_exemplo[10];

    // Inicializar todos os arrays de caracteres com zeros
    for (int i = 0; i < 10; i++) {
        memset(entregas_exemplo[i].nome_completo, 0, sizeof(entregas_exemplo[i].nome_completo));
        memset(entregas_exemplo[i].descricao, 0, sizeof(entregas_exemplo[i].descricao));
        memset(entregas_exemplo[i].data_coleta, 0, sizeof(entregas_exemplo[i].data_coleta));
        memset(entregas_exemplo[i].data_entrega_prevista, 0, sizeof(entregas_exemplo[i].data_entrega_prevista));
        memset(entregas_exemplo[i].data_entrega_realizada, 0, sizeof(entregas_exemplo[i].data_entrega_realizada));
        memset(entregas_exemplo[i].status_entrega, 0, sizeof(entregas_exemplo[i].status_entrega));
        memset(entregas_exemplo[i].localizacao_atual, 0, sizeof(entregas_exemplo[i].localizacao_atual));
        memset(entregas_exemplo[i].motorista_responsavel, 0, sizeof(entregas_exemplo[i].motorista_responsavel));
        memset(entregas_exemplo[i].veiculo_utilizado, 0, sizeof(entregas_exemplo[i].veiculo_utilizado));
    }

    // Inicializar entregas exemplo
    strcpy(entregas_exemplo[0].nome_completo, "Joao Silva Santos");
    strcpy(entregas_exemplo[0].descricao, "Eletrodomesticos - Geladeira");
    strcpy(entregas_exemplo[0].data_coleta, "15/01/24");
    strcpy(entregas_exemplo[0].data_entrega_prevista, "20/01/24");
    strcpy(entregas_exemplo[0].data_entrega_realizada, "18/01/24");
    strcpy(entregas_exemplo[0].status_entrega, "Entregue");
    strcpy(entregas_exemplo[0].localizacao_atual, "Rio de Janeiro");
    strcpy(entregas_exemplo[0].motorista_responsavel, "Carlos Motorista");
    strcpy(entregas_exemplo[0].veiculo_utilizado, "Van Grande");
    entregas_exemplo[0].valor_frete = 150.00;

    strcpy(entregas_exemplo[1].nome_completo, "Maria Oliveira Costa");
    strcpy(entregas_exemplo[1].descricao, "Moveis - Mesa de Jantar");
    strcpy(entregas_exemplo[1].data_coleta, "16/01/24");
    strcpy(entregas_exemplo[1].data_entrega_prevista, "22/01/24");
    strcpy(entregas_exemplo[1].data_entrega_realizada, "21/01/24");
    strcpy(entregas_exemplo[1].status_entrega, "Entregue");
    strcpy(entregas_exemplo[1].localizacao_atual, "Belo Horizonte");
    strcpy(entregas_exemplo[1].motorista_responsavel, "Ana Condutora");
    strcpy(entregas_exemplo[1].veiculo_utilizado, "Caminhao Medio");
    entregas_exemplo[1].valor_frete = 200.00;

    strcpy(entregas_exemplo[2].nome_completo, "Pedro Santos Lima");
    strcpy(entregas_exemplo[2].descricao, "Roupas - Lote Variado");
    strcpy(entregas_exemplo[2].data_coleta, "17/01/24");
    strcpy(entregas_exemplo[2].data_entrega_prevista, "25/01/24");
    strcpy(entregas_exemplo[2].data_entrega_realizada, "");
    strcpy(entregas_exemplo[2].status_entrega, "Em Transito");
    strcpy(entregas_exemplo[2].localizacao_atual, "Salvador");
    strcpy(entregas_exemplo[2].motorista_responsavel, "Roberto Entregador");
    strcpy(entregas_exemplo[2].veiculo_utilizado, "Van Pequena");
    entregas_exemplo[2].valor_frete = 120.00;

    strcpy(entregas_exemplo[3].nome_completo, "Ana Paula Ferreira");
    strcpy(entregas_exemplo[3].descricao, "Livros - Colecao Academica");
    strcpy(entregas_exemplo[3].data_coleta, "18/01/24");
    strcpy(entregas_exemplo[3].data_entrega_prevista, "26/01/24");
    strcpy(entregas_exemplo[3].data_entrega_realizada, "");
    strcpy(entregas_exemplo[3].status_entrega, "Em Transito");
    strcpy(entregas_exemplo[3].localizacao_atual, "Brasilia");
    strcpy(entregas_exemplo[3].motorista_responsavel, "Lucia Motorista");
    strcpy(entregas_exemplo[3].veiculo_utilizado, "Van Pequena");
    entregas_exemplo[3].valor_frete = 100.00;

    strcpy(entregas_exemplo[4].nome_completo, "Carlos Eduardo Souza");
    strcpy(entregas_exemplo[4].descricao, "Ferramentas - Kit Completo");
    strcpy(entregas_exemplo[4].data_coleta, "19/01/24");
    strcpy(entregas_exemplo[4].data_entrega_prevista, "27/01/24");
    strcpy(entregas_exemplo[4].data_entrega_realizada, "");
    strcpy(entregas_exemplo[4].status_entrega, "Coletado");
    strcpy(entregas_exemplo[4].localizacao_atual, "Recife");
    strcpy(entregas_exemplo[4].motorista_responsavel, "Marcos Condutor");
    strcpy(entregas_exemplo[4].veiculo_utilizado, "Van Media");
    entregas_exemplo[4].valor_frete = 180.00;

    strcpy(entregas_exemplo[5].nome_completo, "Lucia Mendes Alves");
    strcpy(entregas_exemplo[5].descricao, "Cosmeticos - Lote Variado");
    strcpy(entregas_exemplo[5].data_coleta, "20/01/24");
    strcpy(entregas_exemplo[5].data_entrega_prevista, "28/01/24");
    strcpy(entregas_exemplo[5].data_entrega_realizada, "");
    strcpy(entregas_exemplo[5].status_entrega, "Coletado");
    strcpy(entregas_exemplo[5].localizacao_atual, "Fortaleza");
    strcpy(entregas_exemplo[5].motorista_responsavel, "Fernanda Entregadora");
    strcpy(entregas_exemplo[5].veiculo_utilizado, "Van Pequena");
    entregas_exemplo[5].valor_frete = 80.00;

    strcpy(entregas_exemplo[6].nome_completo, "Roberto Carlos Silva");
    strcpy(entregas_exemplo[6].descricao, "Eletronicos - Notebook");
    strcpy(entregas_exemplo[6].data_coleta, "21/01/24");
    strcpy(entregas_exemplo[6].data_entrega_prevista, "29/01/24");
    strcpy(entregas_exemplo[6].data_entrega_realizada, "");
    strcpy(entregas_exemplo[6].status_entrega, "Coletado");
    strcpy(entregas_exemplo[6].localizacao_atual, "Manaus");
    strcpy(entregas_exemplo[6].motorista_responsavel, "Juliana Motorista");
    strcpy(entregas_exemplo[6].veiculo_utilizado, "Van Segura");
    entregas_exemplo[6].valor_frete = 300.00;

    strcpy(entregas_exemplo[7].nome_completo, "Fernanda Costa Lima");
    strcpy(entregas_exemplo[7].descricao, "Alimentos - Produtos Organicos");
    strcpy(entregas_exemplo[7].data_coleta, "22/01/24");
    strcpy(entregas_exemplo[7].data_entrega_prevista, "30/01/24");
    strcpy(entregas_exemplo[7].data_entrega_realizada, "");
    strcpy(entregas_exemplo[7].status_entrega, "Coletado");
    strcpy(entregas_exemplo[7].localizacao_atual, "Curitiba");
    strcpy(entregas_exemplo[7].motorista_responsavel, "Pedro Condutor");
    strcpy(entregas_exemplo[7].veiculo_utilizado, "Van Refrigerada");
    entregas_exemplo[7].valor_frete = 160.00;

    strcpy(entregas_exemplo[8].nome_completo, "Marcos Antonio Santos");
    strcpy(entregas_exemplo[8].descricao, "Esportes - Equipamentos");
    strcpy(entregas_exemplo[8].data_coleta, "23/01/24");
    strcpy(entregas_exemplo[8].data_entrega_prevista, "31/01/24");
    strcpy(entregas_exemplo[8].data_entrega_realizada, "");
    strcpy(entregas_exemplo[8].status_entrega, "Coletado");
    strcpy(entregas_exemplo[8].localizacao_atual, "Porto Alegre");
    strcpy(entregas_exemplo[8].motorista_responsavel, "Ana Motorista");
    strcpy(entregas_exemplo[8].veiculo_utilizado, "Van Grande");
    entregas_exemplo[8].valor_frete = 220.00;

    strcpy(entregas_exemplo[9].nome_completo, "Juliana Pereira Rocha");
    strcpy(entregas_exemplo[9].descricao, "Decoracao - Quadros e Objetos");
    strcpy(entregas_exemplo[9].data_coleta, "24/01/24");
    strcpy(entregas_exemplo[9].data_entrega_prevista, "01/02/24");
    strcpy(entregas_exemplo[9].data_entrega_realizada, "");
    strcpy(entregas_exemplo[9].status_entrega, "Coletado");
    strcpy(entregas_exemplo[9].localizacao_atual, "Goiania");
    strcpy(entregas_exemplo[9].motorista_responsavel, "Carlos Entregador");
    strcpy(entregas_exemplo[9].veiculo_utilizado, "Van Pequena");
    entregas_exemplo[9].valor_frete = 140.00;

    for (int i = 0; i < 10; i++) {
        Cliente *tmp_clientes = realloc(clientes, (size_t)(qtdClientes + 1) * sizeof(Cliente));
        if (tmp_clientes != NULL) {
            clientes = tmp_clientes;
            clientes[qtdClientes].id_cliente = proximo_id_cliente_disponivel();
            strcpy(clientes[qtdClientes].nome_completo, clientes_exemplo[i].nome_completo);
            strcpy(clientes[qtdClientes].cpf_cnpj, clientes_exemplo[i].cpf_cnpj);
            strcpy(clientes[qtdClientes].telefone, clientes_exemplo[i].telefone);
            strcpy(clientes[qtdClientes].email, clientes_exemplo[i].email);
            strcpy(clientes[qtdClientes].endereco, clientes_exemplo[i].endereco);
            qtdClientes++;
        }

        Carga *tmp_cargas = realloc(cargas, (size_t)(qtdCargas + 1) * sizeof(Carga));
        if (tmp_cargas != NULL) {
            cargas = tmp_cargas;
            cargas[qtdCargas].id_carga = proximo_id_carga_disponivel();
            strcpy(cargas[qtdCargas].descricao, cargas_exemplo[i].descricao);
            cargas[qtdCargas].peso = cargas_exemplo[i].peso;
            strcpy(cargas[qtdCargas].dimensoes, cargas_exemplo[i].dimensoes);
            strcpy(cargas[qtdCargas].tipo_material, cargas_exemplo[i].tipo_material);
            cargas[qtdCargas].valor_declarado = cargas_exemplo[i].valor_declarado;
            strcpy(cargas[qtdCargas].origem, cargas_exemplo[i].origem);
            strcpy(cargas[qtdCargas].destino, cargas_exemplo[i].destino);
            strcpy(cargas[qtdCargas].observacoes, cargas_exemplo[i].observacoes);
            qtdCargas++;
        }

        Entrega *tmp_entregas = realloc(entregas, (size_t)(qtdEntregas + 1) * sizeof(Entrega));
        if (tmp_entregas != NULL) {
            entregas = tmp_entregas;
            entregas[qtdEntregas].id_entrega = proximo_id_entrega_disponivel();
            entregas[qtdEntregas].id_cliente = clientes[qtdClientes - 1].id_cliente;
            entregas[qtdEntregas].id_carga = cargas[qtdCargas - 1].id_carga;
            strcpy(entregas[qtdEntregas].nome_completo, clientes[qtdClientes - 1].nome_completo);
            strcpy(entregas[qtdEntregas].descricao, cargas[qtdCargas - 1].descricao);
            strcpy(entregas[qtdEntregas].data_coleta, entregas_exemplo[i].data_coleta);
            strcpy(entregas[qtdEntregas].data_entrega_prevista, entregas_exemplo[i].data_entrega_prevista);
            strcpy(entregas[qtdEntregas].data_entrega_realizada, entregas_exemplo[i].data_entrega_realizada);
            strcpy(entregas[qtdEntregas].status_entrega, entregas_exemplo[i].status_entrega);
            strcpy(entregas[qtdEntregas].localizacao_atual, entregas_exemplo[i].localizacao_atual);
            strcpy(entregas[qtdEntregas].motorista_responsavel, entregas_exemplo[i].motorista_responsavel);
            strcpy(entregas[qtdEntregas].veiculo_utilizado, entregas_exemplo[i].veiculo_utilizado);
            entregas[qtdEntregas].valor_frete = entregas_exemplo[i].valor_frete;
            qtdEntregas++;
        }
    }

    printf("Dados pre-carregados com sucesso!\n");
    printf("- %d clientes adicionados\n", 10);
    printf("- %d cargas adicionadas\n", 10);
    printf("- %d entregas adicionadas\n", 10);
    printf("\n\t[0] Voltar\n");

    for (;;) {
        if (scanf("%d", &escolha) != 1) { limpa_stdin(); continue; }
        if (escolha == 0) { limpa_stdin(); mostrar_menu_inicial(); break; }
        printf("Essa nao e uma escolha valida!\n");
        limpa_stdin();
    }
}

static int ler_inteiro(const char *prompt) {
    int v;
    for (;;) {
        if (prompt && *prompt) printf("%s", prompt);
        if (scanf("%d", &v) == 1) { limpa_stdin(); return v; }
        printf("Entrada invalida. Digite um numero.\n");
        limpa_stdin();
    }
}

static float ler_float(const char *prompt) {
    float v;
    for (;;) {
        if (prompt && *prompt) printf("%s", prompt);
        if (scanf("%f", &v) == 1) { limpa_stdin(); return v; }
        printf("Entrada invalida. Digite um numero.\n");
        limpa_stdin();
    }
}

static void ler_linha(const char *prompt, char *buf, size_t tam) {
    for (;;) {
        if (prompt && *prompt) printf("%s", prompt);
        if (fgets(buf, tam, stdin) != NULL) {
            buf[strcspn(buf, "\n")] = '\0';
            return;
        }
        clearerr(stdin);
    }
}

static int validar_formato_data(const char *data) {
    if (strlen(data) != 8) return 0;

    for (int i = 0; i < 8; i++) {
        if (i == 2 || i == 5) {
            if (data[i] != '/') return 0;
        } else {
            if (!isdigit(data[i])) return 0;
        }
    }
    return 1;
}

static int converter_data_para_tm(const char *data_str, struct tm *data_tm) {
    if (!validar_formato_data(data_str)) return 0;

    int dia, mes, ano;
    if (sscanf(data_str, "%d/%d/%d", &dia, &mes, &ano) != 3) return 0;

    if (dia < 1 || dia > 31 || mes < 1 || mes > 12) return 0;

    data_tm->tm_mday = dia;
    data_tm->tm_mon = mes - 1;
    data_tm->tm_year = ano + 100;
    data_tm->tm_hour = 0;
    data_tm->tm_min = 0;
    data_tm->tm_sec = 0;
    data_tm->tm_isdst = -1;

    return 1;
}

static int comparar_datas(const char *data1, const char *data2) {
    struct tm tm1, tm2;

    if (!converter_data_para_tm(data1, &tm1) || !converter_data_para_tm(data2, &tm2)) {
        return -2;
    }

    time_t time1 = mktime(&tm1);
    time_t time2 = mktime(&tm2);

    if (time1 == -1 || time2 == -1) return -2;

    if (time1 < time2) return -1;
    if (time1 > time2) return 1;
    return 0;
}

static void ler_data_com_validacao(const char *prompt, char *buf, size_t tam) {
    for (;;) {
        printf("%s (formato DD/MM/AA): ", prompt);
        if (fgets(buf, tam, stdin) != NULL) {
            buf[strcspn(buf, "\n")] = '\0';
            if (validar_formato_data(buf)) {
                return;
            }
        }
        printf("Formato invalido! Use DD/MM/AA (exemplo: 25/12/24)\n");
        clearerr(stdin);
    }
}

static void editar_data_com_validacao(const char *prompt, char *buf, size_t tam, const char *data_referencia, const char *tipo_validacao) {
    for (;;) {
        ler_data_com_validacao(prompt, buf, tam);

        if (strcmp(tipo_validacao, "prevista") == 0) {
            if (comparar_datas(buf, data_referencia) < 0) {
                printf("Erro: A data de entrega prevista nao pode ser anterior a data de coleta!\n");
                continue;
            }
        } else if (strcmp(tipo_validacao, "realizada") == 0) {
            if (comparar_datas(buf, data_referencia) < 0) {
                printf("Erro: A data de entrega realizada nao pode ser anterior a data de coleta!\n");
                continue;
            }
        }
        break;
    }
}

void exibir_cliente(int index_cliente) {
    if (index_cliente < 0 || index_cliente >= qtdClientes) {
        printf("Indice de cliente invalido.\n");
        return;
    }
    printf("ID: %d | Nome: %s | CPF/CNPJ: %s | Tel: %s | Email: %s | Endereco: %s\n",
           clientes[index_cliente].id_cliente,
           clientes[index_cliente].nome_completo,
           clientes[index_cliente].cpf_cnpj,
           clientes[index_cliente].telefone,
           clientes[index_cliente].email,
           clientes[index_cliente].endereco);
}

void listar_todos_clientes(void) {
    int escolha = 0;

#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif

    printf("Lista de Todos os Clientes\n");
    printf("========================================\n\n");

    if (qtdClientes == 0) {
        printf("Nenhum cliente cadastrado.\n");
    } else {
        printf("Total de clientes: %d\n\n", qtdClientes);
        for (int i = 0; i < qtdClientes; i++) {
            printf("[%d] ID: %d | Nome: %s | CPF/CNPJ: %s | Tel: %s | Email: %s | Endereco: %s\n",
                   i + 1,
                   clientes[i].id_cliente,
                   clientes[i].nome_completo,
                   clientes[i].cpf_cnpj,
                   clientes[i].telefone,
                   clientes[i].email,
                   clientes[i].endereco);
        }
    }

    printf("\n\t[0] Voltar\n");
    for (;;) {
        if (scanf("%d", &escolha) != 1) { limpa_stdin(); continue; }
        if (escolha == 0) { limpa_stdin(); mostrar_menu_clientes(); break; }
        printf("Essa nao e uma escolha valida!\n");
        limpa_stdin();
    }
}

void cadastrar_carga(Carga **pcargas, int *pqtdCargas) {
    int escolha = 0;
    Carga *tmp = realloc(*pcargas, (size_t)(*pqtdCargas + 1) * sizeof *tmp);
    if (tmp == NULL) {
        fprintf(stderr, "Erro de memoria!\n");
        free(*pcargas);
        exit(EXIT_FAILURE);
    }
    *pcargas = tmp;

    Carga c;

    do {
        c.id_carga = ler_inteiro("ID da carga: ");
        if (id_carga_ja_existe(c.id_carga)) {
            printf("Erro: Ja existe uma carga com este ID! Tente outro ID.\n");
        }
    } while (id_carga_ja_existe(c.id_carga));
    ler_linha("Descricao: ", c.descricao, sizeof c.descricao);
    c.peso = ler_float("Peso (kg): ");
    ler_linha("Dimensoes: ", c.dimensoes, sizeof c.dimensoes);
    ler_linha("Tipo de material: ", c.tipo_material, sizeof c.tipo_material);
    c.valor_declarado = ler_float("Valor declarado (R$): ");
    ler_linha("Origem: ", c.origem, sizeof c.origem);
    ler_linha("Destino: ", c.destino, sizeof c.destino);
    ler_linha("Observacoes: ", c.observacoes, sizeof c.observacoes);

    (*pcargas)[*pqtdCargas] = c;
    (*pqtdCargas)++;

    printf("\nCarga cadastrada com sucesso!\n");
    printf("\n\t[0] Voltar\n");
    for (;;) {
        if (scanf("%d", &escolha) != 1) { limpa_stdin(); continue; }
        if (escolha == 0) { limpa_stdin(); mostrar_menu_cargas(); break; }
        printf("Essa nao e uma escolha valida!\n");
        limpa_stdin();
    }
}

int buscar_cliente_porID(int id_busca) {
    for (int i = 0; i < qtdClientes; i++) {
        if (clientes[i].id_cliente == id_busca) return i;
    }
    return -1;
}

void editar_cliente(void){
    int index_cliente = -1;
    int escolha = -1;

#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif

    index_cliente = buscar_cliente();
    if (index_cliente < 0) {
        mostrar_menu_clientes();
        return;
    }

    for (;;) {
#ifdef _WIN32
        system("cls");
#else
        system("clear");
#endif
        printf("Editar Cliente\n");
        printf("----------------------------------------\n");
        exibir_cliente(index_cliente);
        printf("----------------------------------------\n");
        printf("\t [1] Nome\n");
        printf("\t [2] CPF/CNPJ\n");
        printf("\t [3] Telefone\n");
        printf("\t [4] Email\n");
        printf("\t [5] Endereco\n");
        printf("\n\t [0] Voltar\n");
        if (scanf("%d", &escolha) != 1) { limpa_stdin(); continue; }
        limpa_stdin();

        if (escolha == 0) { mostrar_menu_clientes(); break; }
        else if (escolha == 1) { ler_linha("Novo nome: ", clientes[index_cliente].nome_completo, sizeof clientes[index_cliente].nome_completo); }
        else if (escolha == 2) { ler_linha("Novo CPF/CNPJ: ", clientes[index_cliente].cpf_cnpj, sizeof clientes[index_cliente].cpf_cnpj); }
        else if (escolha == 3) { ler_linha("Novo telefone: ", clientes[index_cliente].telefone, sizeof clientes[index_cliente].telefone); }
        else if (escolha == 4) { ler_linha("Novo email: ", clientes[index_cliente].email, sizeof clientes[index_cliente].email); }
        else if (escolha == 5) { ler_linha("Novo endereco: ", clientes[index_cliente].endereco, sizeof clientes[index_cliente].endereco); }
        else { printf("Escolha invalida!\n"); continue; }

        printf("\nAtualizado com sucesso!\n");
        printf("\n\t[0] Voltar\n");
        for (;;) {
            if (scanf("%d", &escolha) != 1) { limpa_stdin(); continue; }
            if (escolha == 0) { mostrar_menu_clientes(); return; }
            limpa_stdin();
        }
    }
}

void deletar_cliente(void){
    int index_cliente = -1;
    int escolha = -1;

#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif

    index_cliente = buscar_cliente();
    if (index_cliente < 0) {
        mostrar_menu_clientes();
        return;
    }

    if (cliente_tem_entregas(clientes[index_cliente].id_cliente)) {
        printf("ERRO: Nao e possivel deletar este cliente!\n");
        printf("O cliente possui entregas cadastradas e nao pode ser removido.\n");
        exibir_cliente(index_cliente);
        printf("\n\t[0] Voltar\n");
        for (;;) {
            if (scanf("%d", &escolha) != 1) { limpa_stdin(); continue; }
            if (escolha == 0) { limpa_stdin(); mostrar_menu_clientes(); break; }
            printf("Essa nao e uma escolha valida!\n");
            limpa_stdin();
        }
        return;
    }

    printf("Voce deseja deletar este cliente?\n");
    exibir_cliente(index_cliente);
    printf("----------------------------------------\n");
    printf("\t [1] Sim\n");
    printf("\t [2] Nao\n");
    for (;;) {
        if (scanf("%d", &escolha) != 1) { limpa_stdin(); continue; }
        limpa_stdin();
        if (escolha == 1 || escolha == 2) break;
        printf("Escolha invalida!\n");
    }

    if (escolha == 1) {
        for (int i = index_cliente; i < qtdClientes - 1; i++) clientes[i] = clientes[i + 1];
        qtdClientes--;
        if (qtdClientes == 0) { free(clientes); clientes = NULL; }
        else {
            Cliente *tmp = realloc(clientes, (size_t)qtdClientes * sizeof *tmp);
            if (tmp != NULL) clientes = tmp;
        }
        printf("\nCliente deletado com sucesso!\n");
    } else {
        printf("\nOperacao cancelada.\n");
    }

    printf("\n\t[0] Voltar\n");
    for (;;) {
        if (scanf("%d", &escolha) != 1) { limpa_stdin(); continue; }
        if (escolha == 0) { mostrar_menu_clientes(); break; }
        limpa_stdin();
    }
}

void cadastrar_cliente(Cliente **pclientes, int *pqtdClientes){
    int escolha = 0;
    Cliente *tmp = realloc(*pclientes, (size_t)(*pqtdClientes + 1) * sizeof *tmp);
    if (tmp == NULL) {
        fprintf(stderr, "Erro de memoria!\n");
        free(*pclientes);
        exit(EXIT_FAILURE);
    }
    *pclientes = tmp;

    Cliente c;

    do {
        c.id_cliente = ler_inteiro("ID do cliente: ");
        if (id_cliente_ja_existe(c.id_cliente)) {
            printf("Erro: Ja existe um cliente com este ID! Tente outro ID.\n");
        }
    } while (id_cliente_ja_existe(c.id_cliente));

    ler_linha("Nome completo: ", c.nome_completo, sizeof c.nome_completo);

    do {
        ler_linha("CPF/CNPJ: ", c.cpf_cnpj, sizeof c.cpf_cnpj);
        if (cpf_cnpj_ja_existe(c.cpf_cnpj)) {
            printf("Erro: Ja existe um cliente com este CPF/CNPJ! Tente outro.\n");
        }
    } while (cpf_cnpj_ja_existe(c.cpf_cnpj));

    ler_linha("Telefone: ", c.telefone, sizeof c.telefone);
    ler_linha("Email: ", c.email, sizeof c.email);
    ler_linha("Endereco: ", c.endereco, sizeof c.endereco);

    (*pclientes)[*pqtdClientes] = c;
    (*pqtdClientes)++;

    printf("\nCliente cadastrado com sucesso!\n");
    printf("\n\t[0] Voltar\n");
    for (;;) {
        if (scanf("%d", &escolha) != 1) { limpa_stdin(); continue; }
        if (escolha == 0) { mostrar_menu_clientes(); break; }
        printf("Essa nao e uma escolha valida!\n");
        limpa_stdin();
    }
}

int buscar_cliente(void) {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
    int id_busca = ler_inteiro("Para consultar um cliente, digite seu respectivo ID cadastrado:\n");

    for (int i = 0; i < qtdClientes; i++) {
        if (clientes[i].id_cliente == id_busca) return i;
    }

    printf("Cliente com ID %d nao encontrado.\n", id_busca);
    return -1;
}

void mostrar_cliente(int index_cliente) {
    int escolha = 0;
    if (index_cliente < 0 || index_cliente >= qtdClientes) {
        printf("Indice de cliente invalido.\n");
        return;
    }
    printf("ID: %d | Nome: %s | CPF/CNPJ: %s | Tel: %s | Email: %s | Endereco: %s\n",
           clientes[index_cliente].id_cliente,
           clientes[index_cliente].nome_completo,
           clientes[index_cliente].cpf_cnpj,
           clientes[index_cliente].telefone,
           clientes[index_cliente].email,
           clientes[index_cliente].endereco);

    printf("\n\t[0] Voltar\n");
    for (;;) {
        if (scanf("%d", &escolha) != 1) { limpa_stdin(); continue; }
        if (escolha == 0) { mostrar_menu_clientes(); break; }
        printf("Essa nao e uma escolha valida!\n");
        limpa_stdin();
    }
}

int buscar_carga(void) {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
    int id_busca = ler_inteiro("Para consultar uma carga, digite seu respectivo ID cadastrado:\n");

    for (int i = 0; i < qtdCargas; i++) {
        if (cargas[i].id_carga == id_busca) return i;
    }

    printf("Carga com ID %d nao encontrada.\n", id_busca);
    return -1;
}

void mostrar_carga(int index_carga) {
    int escolha = 0;
    if (index_carga < 0 || index_carga >= qtdCargas) {
        printf("Indice da Carga invalido.\n");
        return;
    }
    printf("ID: %d | Descricao: %s | Peso: %f | Dimensoes: %s | Tipo do material: %s | Valor declarado: %f | Origem: %s | Destino: %s | Observacoes: %s\n",
           cargas[index_carga].id_carga,
           cargas[index_carga].descricao,
           cargas[index_carga].peso,
           cargas[index_carga].dimensoes,
           cargas[index_carga].tipo_material,
           cargas[index_carga].valor_declarado,
           cargas[index_carga].origem,
           cargas[index_carga].destino,
           cargas[index_carga].observacoes);

    printf("\n\t[0] Voltar\n");
    for (;;) {
        if (scanf("%d", &escolha) != 1) { limpa_stdin(); continue; }
        if (escolha == 0) { mostrar_menu_cargas(); break; }
        printf("Essa nao e uma escolha valida!\n");
        limpa_stdin();
    }
}

void listar_todas_cargas(void) {
    int escolha = 0;

#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif

    printf("Lista de Todas as Cargas\n");
    printf("========================================\n\n");

    if (qtdCargas == 0) {
        printf("Nenhuma carga cadastrada.\n");
    } else {
        printf("Total de cargas: %d\n\n", qtdCargas);
        for (int i = 0; i < qtdCargas; i++) {
            printf("[%d] ID: %d | Descricao: %s | Peso: %.2f kg | Dimensoes: %s | Tipo: %s | Valor: R$ %.2f | Origem: %s | Destino: %s\n",
                   i + 1,
                   cargas[i].id_carga,
                   cargas[i].descricao,
                   cargas[i].peso,
                   cargas[i].dimensoes,
                   cargas[i].tipo_material,
                   cargas[i].valor_declarado,
                   cargas[i].origem,
                   cargas[i].destino);
        }
    }

    printf("\n\t[0] Voltar\n");
    for (;;) {
        if (scanf("%d", &escolha) != 1) { limpa_stdin(); continue; }
        if (escolha == 0) { limpa_stdin(); mostrar_menu_cargas(); break; }
        printf("Essa nao e uma escolha valida!\n");
        limpa_stdin();
    }
}

void editar_carga(void){
    int index_carga = -1;
    int escolha = -1;

    index_carga = buscar_carga();
    if (index_carga < 0) {
        mostrar_menu_cargas();
        return;
    }

    for (;;) {
#ifdef _WIN32
        system("cls");
#else
        system("clear");
#endif
        printf("Editar Carga\n");
        printf("----------------------------------------\n");
        printf("ID: %d | Descricao: %s | Peso: %f | Dimensoes: %s | Tipo do material: %s | Valor declarado: %f | Origem: %s | Destino: %s | Observacoes: %s\n",
               cargas[index_carga].id_carga,
               cargas[index_carga].descricao,
               cargas[index_carga].peso,
               cargas[index_carga].dimensoes,
               cargas[index_carga].tipo_material,
               cargas[index_carga].valor_declarado,
               cargas[index_carga].origem,
               cargas[index_carga].destino,
               cargas[index_carga].observacoes);
        printf("----------------------------------------\n");
        printf("\t [1] Descricao\n");
        printf("\t [2] Peso\n");
        printf("\t [3] Dimensoes\n");
        printf("\t [4] Tipo de material\n");
        printf("\t [5] Valor declarado\n");
        printf("\t [6] Origem\n");
        printf("\t [7] Destino\n");
        printf("\t [8] Observacoes\n");
        printf("\n\t [0] Voltar\n");

        if (scanf("%d", &escolha) != 1) { limpa_stdin(); continue; }
        limpa_stdin();

        if (escolha == 0) { mostrar_menu_cargas(); break; }
        else if (escolha == 1) { ler_linha("Nova descricao: ", cargas[index_carga].descricao, sizeof cargas[index_carga].descricao); }
        else if (escolha == 2) { cargas[index_carga].peso = ler_float("Novo peso: "); }
        else if (escolha == 3) { ler_linha("Novas dimensoes: ", cargas[index_carga].dimensoes, sizeof cargas[index_carga].dimensoes); }
        else if (escolha == 4) { ler_linha("Novo tipo de material: ", cargas[index_carga].tipo_material, sizeof cargas[index_carga].tipo_material); }
        else if (escolha == 5) { cargas[index_carga].valor_declarado = ler_float("Novo valor declarado: "); }
        else if (escolha == 6) { ler_linha("Nova origem: ", cargas[index_carga].origem, sizeof cargas[index_carga].origem); }
        else if (escolha == 7) { ler_linha("Novo destino: ", cargas[index_carga].destino, sizeof cargas[index_carga].destino); }
        else if (escolha == 8) { ler_linha("Novas observacoes: ", cargas[index_carga].observacoes, sizeof cargas[index_carga].observacoes); }
        else { printf("Escolha invalida!\n"); continue; }

        printf("\nAtualizado com sucesso!\n");
        printf("\n\t[0] Voltar\n");
        for (;;) {
            if (scanf("%d", &escolha) != 1) { limpa_stdin(); continue; }
            if (escolha == 0) { mostrar_menu_cargas(); return; }
            limpa_stdin();
        }
    }
}

void deletar_carga(void){
    int index_carga = -1;
    int escolha = -1;

    index_carga = buscar_carga();
    if (index_carga < 0) {
        mostrar_menu_cargas();
        return;
    }

    if (carga_tem_entregas(cargas[index_carga].id_carga)) {
        printf("ERRO: Nao e possivel deletar esta carga!\n");
        printf("A carga possui entregas cadastradas e nao pode ser removida.\n");
        printf("ID: %d | Descricao: %s\n", cargas[index_carga].id_carga, cargas[index_carga].descricao);
        printf("\n\t[0] Voltar\n");
        for (;;) {
            if (scanf("%d", &escolha) != 1) { limpa_stdin(); continue; }
            if (escolha == 0) { limpa_stdin(); mostrar_menu_cargas(); break; }
            printf("Essa nao e uma escolha valida!\n");
            limpa_stdin();
        }
        return;
    }

    printf("Voce deseja deletar esta carga?\n");
    printf("ID: %d | Descricao: %s\n", cargas[index_carga].id_carga, cargas[index_carga].descricao);
    printf("----------------------------------------\n");
    printf("\t [1] Sim\n");
    printf("\t [2] Nao\n");
    for (;;) {
        if (scanf("%d", &escolha) != 1) { limpa_stdin(); continue; }
        limpa_stdin();
        if (escolha == 1 || escolha == 2) break;
        printf("Escolha invalida!\n");
    }

    if (escolha == 1) {
        for (int i = index_carga; i < qtdCargas - 1; i++) cargas[i] = cargas[i + 1];
        qtdCargas--;
        if (qtdCargas == 0) { free(cargas); cargas = NULL; }
        else {
            Carga *tmp = realloc(cargas, (size_t)qtdCargas * sizeof *tmp);
            if (tmp != NULL) cargas = tmp;
        }
        printf("\nCarga deletada com sucesso!\n");
    } else {
        printf("\nOperacao cancelada.\n");
    }

    printf("\n\t[0] Voltar\n");
    for (;;) {
        if (scanf("%d", &escolha) != 1) { limpa_stdin(); continue; }
        if (escolha == 0) { mostrar_menu_cargas(); break; }
        limpa_stdin();
    }
}

void cadastrar_entrega(Entrega **pentregas, int *pqtdEntregas) {
    int escolha = 0;
    int id_cliente, id_carga;
    int index_cliente = -1, index_carga = -1;


    if (qtdClientes == 0) {
        printf("Nao e possivel cadastrar entrega: nenhum cliente cadastrado!\n");
        printf("\n\t[0] Voltar\n");
        for (;;) {
            if (scanf("%d", &escolha) != 1) { limpa_stdin(); continue; }
            if (escolha == 0) { limpa_stdin(); mostrar_menu_inicial(); break; }
            printf("Essa nao e uma escolha valida!\n");
            limpa_stdin();
        }
        return;
    }


    if (qtdCargas == 0) {
        printf("Nao e possivel cadastrar entrega: nenhuma carga cadastrada!\n");
        printf("\n\t[0] Voltar\n");
        for (;;) {
            if (scanf("%d", &escolha) != 1) { limpa_stdin(); continue; }
            if (escolha == 0) { limpa_stdin(); mostrar_menu_inicial(); break; }
            printf("Essa nao e uma escolha valida!\n");
            limpa_stdin();
        }
        return;
    }

    Entrega *tmp = realloc(*pentregas, (size_t)(*pqtdEntregas + 1) * sizeof *tmp);
    if (tmp == NULL) {
        fprintf(stderr, "Erro de memoria!\n");
        free(*pentregas);
        exit(EXIT_FAILURE);
    }
    *pentregas = tmp;

    Entrega e;

    do {
        e.id_entrega = ler_inteiro("ID da entrega: ");
        if (id_entrega_ja_existe(e.id_entrega)) {
            printf("Erro: Ja existe uma entrega com este ID! Tente outro ID.\n");
        }
    } while (id_entrega_ja_existe(e.id_entrega));


    do {
        id_cliente = ler_inteiro("ID do cliente: ");
        index_cliente = buscar_cliente_porID(id_cliente);
        if (index_cliente == -1) {
            printf("Cliente com ID %d nao encontrado. Tente novamente.\n", id_cliente);
        }
    } while (index_cliente == -1);


    do {
        id_carga = ler_inteiro("ID da carga: ");
        for (int i = 0; i < qtdCargas; i++) {
            if (cargas[i].id_carga == id_carga) {
                index_carga = i;
                break;
            }
        }
        if (index_carga == -1) {
            printf("Carga com ID %d nao encontrada. Tente novamente.\n", id_carga);
        }
    } while (index_carga == -1);

    e.id_cliente = id_cliente;
    strcpy(e.nome_completo, clientes[index_cliente].nome_completo);
    e.id_carga = id_carga;
    strcpy(e.descricao, cargas[index_carga].descricao);

    ler_data_com_validacao("Data de coleta", e.data_coleta, sizeof e.data_coleta);

    do {
        ler_data_com_validacao("Data de entrega prevista", e.data_entrega_prevista, sizeof e.data_entrega_prevista);
        if (comparar_datas(e.data_entrega_prevista, e.data_coleta) < 0) {
            printf("Erro: A data de entrega prevista nao pode ser anterior a data de coleta!\n");
        }
    } while (comparar_datas(e.data_entrega_prevista, e.data_coleta) < 0);

    do {
        ler_data_com_validacao("Data de entrega realizada", e.data_entrega_realizada, sizeof e.data_entrega_realizada);
        if (comparar_datas(e.data_entrega_realizada, e.data_coleta) < 0) {
            printf("Erro: A data de entrega realizada nao pode ser anterior a data de coleta!\n");
        }
    } while (comparar_datas(e.data_entrega_realizada, e.data_coleta) < 0);
    ler_linha("Status da entrega: ", e.status_entrega, sizeof e.status_entrega);
    ler_linha("Localizacao atual: ", e.localizacao_atual, sizeof e.localizacao_atual);
    ler_linha("Motorista responsavel: ", e.motorista_responsavel, sizeof e.motorista_responsavel);
    ler_linha("Veiculo utilizado: ", e.veiculo_utilizado, sizeof e.veiculo_utilizado);
    e.valor_frete = ler_float("Valor do frete (R$): ");

    (*pentregas)[*pqtdEntregas] = e;
    (*pqtdEntregas)++;

    printf("\nEntrega cadastrada com sucesso!\n");
    printf("\n\t[0] Voltar\n");
    for (;;) {
        if (scanf("%d", &escolha) != 1) { limpa_stdin(); continue; }
        if (escolha == 0) { limpa_stdin(); mostrar_menu_inicial(); break; }
        printf("Essa nao e uma escolha valida!\n");
        limpa_stdin();
    }
}

int buscar_entrega(void) {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
    int id_busca = ler_inteiro("Para consultar uma entrega, digite seu respectivo ID cadastrado:\n");

    for (int i = 0; i < qtdEntregas; i++) {
        if (entregas[i].id_entrega == id_busca) return i;
    }

    printf("Entrega com ID %d nao encontrada.\n", id_busca);
    return -1;
}

void mostrar_entrega(int index_entrega) {
    int escolha = 0;
    if (index_entrega < 0 || index_entrega >= qtdEntregas) {
        printf("Indice da entrega invalido.\n");
        return;
    }
    printf("ID: %d | Cliente: %s (ID: %d) | Carga: %s (ID: %d) | Data Coleta: %s | Data Prevista: %s | Data Realizada: %s | Status: %s | Localizacao: %s | Motorista: %s | Veiculo: %s | Valor Frete: %.2f\n",
           entregas[index_entrega].id_entrega,
           entregas[index_entrega].nome_completo,
           entregas[index_entrega].id_cliente,
           entregas[index_entrega].descricao,
           entregas[index_entrega].id_carga,
           entregas[index_entrega].data_coleta,
           entregas[index_entrega].data_entrega_prevista,
           entregas[index_entrega].data_entrega_realizada,
           entregas[index_entrega].status_entrega,
           entregas[index_entrega].localizacao_atual,
           entregas[index_entrega].motorista_responsavel,
           entregas[index_entrega].veiculo_utilizado,
           entregas[index_entrega].valor_frete);

    printf("\n\t[0] Voltar\n");
    for (;;) {
        if (scanf("%d", &escolha) != 1) { limpa_stdin(); continue; }
        if (escolha == 0) { limpa_stdin(); mostrar_menu_inicial(); break; }
        printf("Essa nao e uma escolha valida!\n");
        limpa_stdin();
    }
}

void listar_todas_entregas(void) {
    int escolha = 0;

#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif

    printf("Lista de Todas as Entregas\n");
    printf("========================================\n\n");

    if (qtdEntregas == 0) {
        printf("Nenhuma entrega cadastrada.\n");
    } else {
        printf("Total de entregas: %d\n\n", qtdEntregas);
        for (int i = 0; i < qtdEntregas; i++) {
            printf("[%d] ID: %d | Cliente: %s (ID: %d) | Carga: %s (ID: %d)\n",
                   i + 1,
                   entregas[i].id_entrega,
                   entregas[i].nome_completo,
                   entregas[i].id_cliente,
                   entregas[i].descricao,
                   entregas[i].id_carga);
            printf("     Data Coleta: %s | Data Prevista: %s | Data Realizada: %s\n",
                   entregas[i].data_coleta,
                   entregas[i].data_entrega_prevista,
                   entregas[i].data_entrega_realizada);
            printf("     Status: %s | Localizacao: %s | Motorista: %s | Veiculo: %s | Valor: R$ %.2f\n\n",
                   entregas[i].status_entrega,
                   entregas[i].localizacao_atual,
                   entregas[i].motorista_responsavel,
                   entregas[i].veiculo_utilizado,
                   entregas[i].valor_frete);
        }
    }

    printf("\n\t[0] Voltar\n");
    for (;;) {
        if (scanf("%d", &escolha) != 1) { limpa_stdin(); continue; }
        if (escolha == 0) { limpa_stdin(); mostrar_menu_inicial(); break; }
        printf("Essa nao e uma escolha valida!\n");
        limpa_stdin();
    }
}

void editar_entrega(void){
    int index_entrega = -1;
    int escolha = -1;

    index_entrega = buscar_entrega();
    if (index_entrega < 0) {
        mostrar_menu_inicial();
        return;
    }

    for (;;) {
#ifdef _WIN32
        system("cls");
#else
        system("clear");
#endif
        printf("Editar Entrega\n");
        printf("----------------------------------------\n");
        printf("ID: %d | Cliente: %s (ID: %d) | Carga: %s (ID: %d) | Data Coleta: %s | Data Prevista: %s | Data Realizada: %s | Status: %s | Localizacao: %s | Motorista: %s | Veiculo: %s | Valor Frete: %.2f\n",
               entregas[index_entrega].id_entrega,
               entregas[index_entrega].nome_completo,
               entregas[index_entrega].id_cliente,
               entregas[index_entrega].descricao,
               entregas[index_entrega].id_carga,
               entregas[index_entrega].data_coleta,
               entregas[index_entrega].data_entrega_prevista,
               entregas[index_entrega].data_entrega_realizada,
               entregas[index_entrega].status_entrega,
               entregas[index_entrega].localizacao_atual,
               entregas[index_entrega].motorista_responsavel,
               entregas[index_entrega].veiculo_utilizado,
               entregas[index_entrega].valor_frete);
        printf("----------------------------------------\n");
        printf("\t [1] Data de coleta\n");
        printf("\t [2] Data de entrega prevista\n");
        printf("\t [3] Data de entrega realizada\n");
        printf("\t [4] Status da entrega\n");
        printf("\t [5] Localizacao atual\n");
        printf("\t [6] Motorista responsavel\n");
        printf("\t [7] Veiculo utilizado\n");
        printf("\t [8] Valor do frete\n");
        printf("\n\t [0] Voltar\n");

        if (scanf("%d", &escolha) != 1) { limpa_stdin(); continue; }
        limpa_stdin();

        if (escolha == 0) { mostrar_menu_inicial(); break; }
        else if (escolha == 1) {
            ler_data_com_validacao("Nova data de coleta", entregas[index_entrega].data_coleta, sizeof entregas[index_entrega].data_coleta);
        }
        else if (escolha == 2) {
            editar_data_com_validacao("Nova data de entrega prevista", entregas[index_entrega].data_entrega_prevista, sizeof entregas[index_entrega].data_entrega_prevista, entregas[index_entrega].data_coleta, "prevista");
        }
        else if (escolha == 3) {
            editar_data_com_validacao("Nova data de entrega realizada", entregas[index_entrega].data_entrega_realizada, sizeof entregas[index_entrega].data_entrega_realizada, entregas[index_entrega].data_coleta, "realizada");
        }
        else if (escolha == 4) { ler_linha("Novo status da entrega: ", entregas[index_entrega].status_entrega, sizeof entregas[index_entrega].status_entrega); }
        else if (escolha == 5) { ler_linha("Nova localizacao atual: ", entregas[index_entrega].localizacao_atual, sizeof entregas[index_entrega].localizacao_atual); }
        else if (escolha == 6) { ler_linha("Novo motorista responsavel: ", entregas[index_entrega].motorista_responsavel, sizeof entregas[index_entrega].motorista_responsavel); }
        else if (escolha == 7) { ler_linha("Novo veiculo utilizado: ", entregas[index_entrega].veiculo_utilizado, sizeof entregas[index_entrega].veiculo_utilizado); }
        else if (escolha == 8) { entregas[index_entrega].valor_frete = ler_float("Novo valor do frete: "); }
        else { printf("Escolha invalida!\n"); continue; }

        printf("\nAtualizado com sucesso!\n");
        printf("\n\t[0] Voltar\n");
        for (;;) {
            if (scanf("%d", &escolha) != 1) { limpa_stdin(); continue; }
            if (escolha == 0) { mostrar_menu_inicial(); return; }
            limpa_stdin();
        }
    }
}

void deletar_entrega(void){
    int index_entrega = -1;
    int escolha = -1;

    index_entrega = buscar_entrega();
    if (index_entrega < 0) {
        mostrar_menu_inicial();
        return;
    }

    printf("Voce deseja deletar esta entrega?\n");
    printf("ID: %d | Cliente: %s | Carga: %s\n", entregas[index_entrega].id_entrega, entregas[index_entrega].nome_completo, entregas[index_entrega].descricao);
    printf("----------------------------------------\n");
    printf("\t [1] Sim\n");
    printf("\t [2] Nao\n");
    for (;;) {
        if (scanf("%d", &escolha) != 1) { limpa_stdin(); continue; }
        limpa_stdin();
        if (escolha == 1 || escolha == 2) break;
        printf("Escolha invalida!\n");
    }

    if (escolha == 1) {
        for (int i = index_entrega; i < qtdEntregas - 1; i++) entregas[i] = entregas[i + 1];
        qtdEntregas--;
        if (qtdEntregas == 0) { free(entregas); entregas = NULL; }
        else {
            Entrega *tmp = realloc(entregas, (size_t)qtdEntregas * sizeof *tmp);
            if (tmp != NULL) entregas = tmp;
        }
        printf("\nEntrega deletada com sucesso!\n");
    } else {
        printf("\nOperacao cancelada.\n");
    }

    printf("\n\t[0] Voltar\n");
    for (;;) {
        if (scanf("%d", &escolha) != 1) { limpa_stdin(); continue; }
        if (escolha == 0) { mostrar_menu_inicial(); break; }
        limpa_stdin();
    }
}

void menu_relatorios(void) {
    int escolha = 0;

#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif

    printf("Relatorios de Entregas\n");
    printf("========================================\n\n");

    if (qtdEntregas == 0) {
        printf("Nenhuma entrega cadastrada para gerar relatorios.\n");
        printf("\n\t[0] Voltar\n");
        for (;;) {
            if (scanf("%d", &escolha) != 1) { limpa_stdin(); continue; }
            if (escolha == 0) { limpa_stdin(); mostrar_menu_inicial(); break; }
            printf("Essa nao e uma escolha valida!\n");
            limpa_stdin();
        }
        return;
    }

    printf("Escolha o tipo de relatorio:\n");
    printf("\t[1] Relatorio por Periodo de Datas\n");
    printf("\t[2] Relatorio por Status de Entrega\n");
    printf("\t[3] Relatorio por Cliente\n");
    printf("\t[4] Relatorio por Motorista\n");
    printf("\n\t[0] Voltar\n");

    for (;;) {
        if (scanf("%d", &escolha) != 1) { limpa_stdin(); continue; }
        limpa_stdin();

        if (escolha == 0) { mostrar_menu_inicial(); break; }
        else if (escolha == 1) { relatorio_por_periodo(); break; }
        else if (escolha == 2) { relatorio_por_status(); break; }
        else if (escolha == 3) { relatorio_por_cliente(); break; }
        else if (escolha == 4) { relatorio_por_motorista(); break; }
        else { printf("Escolha invalida! Tente novamente.\n"); }
    }
}

void relatorio_por_periodo(void) {
    char data_inicio[9];
    char nome_arquivo[100];
    FILE *arquivo;
    int entregas_encontradas = 0;

    printf("Relatorio por Periodo de Datas\n");
    printf("========================================\n\n");

    ler_data_com_validacao("Data de inicio (DD/MM/AA)", data_inicio, sizeof data_inicio);

    sprintf(nome_arquivo, "relatorio_periodo_%s.txt", data_inicio);
    arquivo = fopen(nome_arquivo, "w");

    if (arquivo == NULL) {
        printf("Erro ao criar arquivo de relatorio!\n");
        return;
    }

    fprintf(arquivo, "RELATORIO DE ENTREGAS POR PERIODO\n");
    fprintf(arquivo, "==================================\n\n");
    fprintf(arquivo, "Data de inicio: %s\n", data_inicio);
    fprintf(arquivo, "Data de geracao: %s\n\n", __DATE__);
    fprintf(arquivo, "ENTREGAS ENCONTRADAS:\n");
    fprintf(arquivo, "====================\n\n");

    for (int i = 0; i < qtdEntregas; i++) {
        if (comparar_datas(entregas[i].data_coleta, data_inicio) >= 0) {
            entregas_encontradas++;

            fprintf(arquivo, "ENTREGA #%d\n", entregas_encontradas);
            fprintf(arquivo, "-----------\n");
            fprintf(arquivo, "ID da Entrega: %d\n", entregas[i].id_entrega);
            fprintf(arquivo, "Data de Coleta: %s\n", entregas[i].data_coleta);
            fprintf(arquivo, "Data Prevista: %s\n", entregas[i].data_entrega_prevista);
            fprintf(arquivo, "Data Realizada: %s\n", entregas[i].data_entrega_realizada);
            fprintf(arquivo, "Status: %s\n", entregas[i].status_entrega);
            fprintf(arquivo, "Localizacao Atual: %s\n", entregas[i].localizacao_atual);
            fprintf(arquivo, "Motorista: %s\n", entregas[i].motorista_responsavel);
            fprintf(arquivo, "Veiculo: %s\n", entregas[i].veiculo_utilizado);
            fprintf(arquivo, "Valor do Frete: R$ %.2f\n\n", entregas[i].valor_frete);

            int index_cliente = buscar_cliente_porID(entregas[i].id_cliente);
            if (index_cliente >= 0) {
                fprintf(arquivo, "INFORMACOES DO CLIENTE:\n");
                fprintf(arquivo, "----------------------\n");
                fprintf(arquivo, "ID: %d\n", clientes[index_cliente].id_cliente);
                fprintf(arquivo, "Nome: %s\n", clientes[index_cliente].nome_completo);
                fprintf(arquivo, "CPF/CNPJ: %s\n", clientes[index_cliente].cpf_cnpj);
                fprintf(arquivo, "Telefone: %s\n", clientes[index_cliente].telefone);
                fprintf(arquivo, "Email: %s\n", clientes[index_cliente].email);
                fprintf(arquivo, "Endereco: %s\n\n", clientes[index_cliente].endereco);
            }

            int index_carga = -1;
            for (int j = 0; j < qtdCargas; j++) {
                if (cargas[j].id_carga == entregas[i].id_carga) {
                    index_carga = j;
                    break;
                }
            }

            if (index_carga >= 0) {
                fprintf(arquivo, "INFORMACOES DA CARGA:\n");
                fprintf(arquivo, "--------------------\n");
                fprintf(arquivo, "ID: %d\n", cargas[index_carga].id_carga);
                fprintf(arquivo, "Descricao: %s\n", cargas[index_carga].descricao);
                fprintf(arquivo, "Peso: %.2f kg\n", cargas[index_carga].peso);
                fprintf(arquivo, "Dimensoes: %s\n", cargas[index_carga].dimensoes);
                fprintf(arquivo, "Tipo de Material: %s\n", cargas[index_carga].tipo_material);
                fprintf(arquivo, "Valor Declarado: R$ %.2f\n", cargas[index_carga].valor_declarado);
                fprintf(arquivo, "Origem: %s\n", cargas[index_carga].origem);
                fprintf(arquivo, "Destino: %s\n", cargas[index_carga].destino);
                fprintf(arquivo, "Observacoes: %s\n\n", cargas[index_carga].observacoes);
            }

            fprintf(arquivo, "========================================\n\n");
        }
    }

    fprintf(arquivo, "RESUMO:\n");
    fprintf(arquivo, "=======\n");
    fprintf(arquivo, "Total de entregas encontradas: %d\n", entregas_encontradas);
    fprintf(arquivo, "Total de entregas cadastradas: %d\n", qtdEntregas);

    fclose(arquivo);

    printf("\nRelatorio gerado com sucesso!\n");
    printf("Arquivo: %s\n", nome_arquivo);
    printf("Total de entregas encontradas: %d\n", entregas_encontradas);
    printf("\n\t[0] Voltar\n");

    int escolha = 0;
    for (;;) {
        if (scanf("%d", &escolha) != 1) { limpa_stdin(); continue; }
        if (escolha == 0) { limpa_stdin(); menu_relatorios(); break; }
        printf("Essa nao e uma escolha valida!\n");
        limpa_stdin();
    }
}

void relatorio_por_status(void) {
    char status[81];
    char nome_arquivo[100];
    FILE *arquivo;
    int entregas_encontradas = 0;

    printf("Relatorio por Status de Entrega\n");
    printf("========================================\n\n");

    printf("Status disponiveis: Entregue, Em Transito, Coletado\n");
    ler_linha("Digite o status desejado: ", status, sizeof status);

    sprintf(nome_arquivo, "relatorio_status_%s.txt", status);
    arquivo = fopen(nome_arquivo, "w");

    if (arquivo == NULL) {
        printf("Erro ao criar arquivo de relatorio!\n");
        return;
    }

    fprintf(arquivo, "RELATORIO DE ENTREGAS POR STATUS\n");
    fprintf(arquivo, "=================================\n\n");
    fprintf(arquivo, "Status filtrado: %s\n", status);
    fprintf(arquivo, "Data de geracao: %s\n\n", __DATE__);
    fprintf(arquivo, "ENTREGAS ENCONTRADAS:\n");
    fprintf(arquivo, "====================\n\n");

    for (int i = 0; i < qtdEntregas; i++) {
        if (strcmp(entregas[i].status_entrega, status) == 0) {
            entregas_encontradas++;

            fprintf(arquivo, "ENTREGA #%d\n", entregas_encontradas);
            fprintf(arquivo, "-----------\n");
            fprintf(arquivo, "ID da Entrega: %d\n", entregas[i].id_entrega);
            fprintf(arquivo, "Data de Coleta: %s\n", entregas[i].data_coleta);
            fprintf(arquivo, "Data Prevista: %s\n", entregas[i].data_entrega_prevista);
            fprintf(arquivo, "Data Realizada: %s\n", entregas[i].data_entrega_realizada);
            fprintf(arquivo, "Status: %s\n", entregas[i].status_entrega);
            fprintf(arquivo, "Localizacao Atual: %s\n", entregas[i].localizacao_atual);
            fprintf(arquivo, "Motorista: %s\n", entregas[i].motorista_responsavel);
            fprintf(arquivo, "Veiculo: %s\n", entregas[i].veiculo_utilizado);
            fprintf(arquivo, "Valor do Frete: R$ %.2f\n\n", entregas[i].valor_frete);

            int index_cliente = buscar_cliente_porID(entregas[i].id_cliente);
            if (index_cliente >= 0) {
                fprintf(arquivo, "INFORMACOES DO CLIENTE:\n");
                fprintf(arquivo, "----------------------\n");
                fprintf(arquivo, "ID: %d\n", clientes[index_cliente].id_cliente);
                fprintf(arquivo, "Nome: %s\n", clientes[index_cliente].nome_completo);
                fprintf(arquivo, "CPF/CNPJ: %s\n", clientes[index_cliente].cpf_cnpj);
                fprintf(arquivo, "Telefone: %s\n", clientes[index_cliente].telefone);
                fprintf(arquivo, "Email: %s\n", clientes[index_cliente].email);
                fprintf(arquivo, "Endereco: %s\n\n", clientes[index_cliente].endereco);
            }

            int index_carga = -1;
            for (int j = 0; j < qtdCargas; j++) {
                if (cargas[j].id_carga == entregas[i].id_carga) {
                    index_carga = j;
                    break;
                }
            }

            if (index_carga >= 0) {
                fprintf(arquivo, "INFORMACOES DA CARGA:\n");
                fprintf(arquivo, "--------------------\n");
                fprintf(arquivo, "ID: %d\n", cargas[index_carga].id_carga);
                fprintf(arquivo, "Descricao: %s\n", cargas[index_carga].descricao);
                fprintf(arquivo, "Peso: %.2f kg\n", cargas[index_carga].peso);
                fprintf(arquivo, "Dimensoes: %s\n", cargas[index_carga].dimensoes);
                fprintf(arquivo, "Tipo de Material: %s\n", cargas[index_carga].tipo_material);
                fprintf(arquivo, "Valor Declarado: R$ %.2f\n", cargas[index_carga].valor_declarado);
                fprintf(arquivo, "Origem: %s\n", cargas[index_carga].origem);
                fprintf(arquivo, "Destino: %s\n", cargas[index_carga].destino);
                fprintf(arquivo, "Observacoes: %s\n\n", cargas[index_carga].observacoes);
            }

            fprintf(arquivo, "========================================\n\n");
        }
    }

    fprintf(arquivo, "RESUMO:\n");
    fprintf(arquivo, "=======\n");
    fprintf(arquivo, "Total de entregas com status '%s': %d\n", status, entregas_encontradas);
    fprintf(arquivo, "Total de entregas cadastradas: %d\n", qtdEntregas);

    fclose(arquivo);

    printf("\nRelatorio gerado com sucesso!\n");
    printf("Arquivo: %s\n", nome_arquivo);
    printf("Total de entregas encontradas: %d\n", entregas_encontradas);
    printf("\n\t[0] Voltar\n");

    int escolha = 0;
    for (;;) {
        if (scanf("%d", &escolha) != 1) { limpa_stdin(); continue; }
        if (escolha == 0) { limpa_stdin(); menu_relatorios(); break; }
        printf("Essa nao e uma escolha valida!\n");
        limpa_stdin();
    }
}

void relatorio_por_cliente(void) {
    int id_cliente;
    int index_cliente = -1;
    char nome_arquivo[100];
    FILE *arquivo;
    int entregas_encontradas = 0;

    printf("Relatorio por Cliente\n");
    printf("========================================\n\n");

    do {
        id_cliente = ler_inteiro("ID do cliente: ");
        index_cliente = buscar_cliente_porID(id_cliente);
        if (index_cliente == -1) {
            printf("Cliente com ID %d nao encontrado. Tente novamente.\n", id_cliente);
        }
    } while (index_cliente == -1);

    sprintf(nome_arquivo, "relatorio_cliente_%d.txt", id_cliente);
    arquivo = fopen(nome_arquivo, "w");

    if (arquivo == NULL) {
        printf("Erro ao criar arquivo de relatorio!\n");
        return;
    }

    fprintf(arquivo, "RELATORIO DE ENTREGAS POR CLIENTE\n");
    fprintf(arquivo, "==================================\n\n");
    fprintf(arquivo, "INFORMACOES DO CLIENTE:\n");
    fprintf(arquivo, "----------------------\n");
    fprintf(arquivo, "ID: %d\n", clientes[index_cliente].id_cliente);
    fprintf(arquivo, "Nome: %s\n", clientes[index_cliente].nome_completo);
    fprintf(arquivo, "CPF/CNPJ: %s\n", clientes[index_cliente].cpf_cnpj);
    fprintf(arquivo, "Telefone: %s\n", clientes[index_cliente].telefone);
    fprintf(arquivo, "Email: %s\n", clientes[index_cliente].email);
    fprintf(arquivo, "Endereco: %s\n\n", clientes[index_cliente].endereco);
    fprintf(arquivo, "Data de geracao: %s\n\n", __DATE__);
    fprintf(arquivo, "ENTREGAS DO CLIENTE:\n");
    fprintf(arquivo, "===================\n\n");

    for (int i = 0; i < qtdEntregas; i++) {
        if (entregas[i].id_cliente == id_cliente) {
            entregas_encontradas++;

            fprintf(arquivo, "ENTREGA #%d\n", entregas_encontradas);
            fprintf(arquivo, "-----------\n");
            fprintf(arquivo, "ID da Entrega: %d\n", entregas[i].id_entrega);
            fprintf(arquivo, "Data de Coleta: %s\n", entregas[i].data_coleta);
            fprintf(arquivo, "Data Prevista: %s\n", entregas[i].data_entrega_prevista);
            fprintf(arquivo, "Data Realizada: %s\n", entregas[i].data_entrega_realizada);
            fprintf(arquivo, "Status: %s\n", entregas[i].status_entrega);
            fprintf(arquivo, "Localizacao Atual: %s\n", entregas[i].localizacao_atual);
            fprintf(arquivo, "Motorista: %s\n", entregas[i].motorista_responsavel);
            fprintf(arquivo, "Veiculo: %s\n", entregas[i].veiculo_utilizado);
            fprintf(arquivo, "Valor do Frete: R$ %.2f\n\n", entregas[i].valor_frete);

            int index_carga = -1;
            for (int j = 0; j < qtdCargas; j++) {
                if (cargas[j].id_carga == entregas[i].id_carga) {
                    index_carga = j;
                    break;
                }
            }

            if (index_carga >= 0) {
                fprintf(arquivo, "INFORMACOES DA CARGA:\n");
                fprintf(arquivo, "--------------------\n");
                fprintf(arquivo, "ID: %d\n", cargas[index_carga].id_carga);
                fprintf(arquivo, "Descricao: %s\n", cargas[index_carga].descricao);
                fprintf(arquivo, "Peso: %.2f kg\n", cargas[index_carga].peso);
                fprintf(arquivo, "Dimensoes: %s\n", cargas[index_carga].dimensoes);
                fprintf(arquivo, "Tipo de Material: %s\n", cargas[index_carga].tipo_material);
                fprintf(arquivo, "Valor Declarado: R$ %.2f\n", cargas[index_carga].valor_declarado);
                fprintf(arquivo, "Origem: %s\n", cargas[index_carga].origem);
                fprintf(arquivo, "Destino: %s\n", cargas[index_carga].destino);
                fprintf(arquivo, "Observacoes: %s\n\n", cargas[index_carga].observacoes);
            }

            fprintf(arquivo, "========================================\n\n");
        }
    }

    fprintf(arquivo, "RESUMO:\n");
    fprintf(arquivo, "=======\n");
    fprintf(arquivo, "Total de entregas do cliente: %d\n", entregas_encontradas);
    fprintf(arquivo, "Total de entregas cadastradas: %d\n", qtdEntregas);

    fclose(arquivo);

    printf("\nRelatorio gerado com sucesso!\n");
    printf("Arquivo: %s\n", nome_arquivo);
    printf("Total de entregas encontradas: %d\n", entregas_encontradas);
    printf("\n\t[0] Voltar\n");

    int escolha = 0;
    for (;;) {
        if (scanf("%d", &escolha) != 1) { limpa_stdin(); continue; }
        if (escolha == 0) { limpa_stdin(); menu_relatorios(); break; }
        printf("Essa nao e uma escolha valida!\n");
        limpa_stdin();
    }
}

void relatorio_por_motorista(void) {
    char motorista[81];
    char nome_arquivo[100];
    FILE *arquivo;
    int entregas_encontradas = 0;

    printf("Relatorio por Motorista\n");
    printf("========================================\n\n");

    ler_linha("Nome do motorista: ", motorista, sizeof motorista);

    sprintf(nome_arquivo, "relatorio_motorista_%s.txt", motorista);
    arquivo = fopen(nome_arquivo, "w");

    if (arquivo == NULL) {
        printf("Erro ao criar arquivo de relatorio!\n");
        return;
    }

    fprintf(arquivo, "RELATORIO DE ENTREGAS POR MOTORISTA\n");
    fprintf(arquivo, "====================================\n\n");
    fprintf(arquivo, "Motorista: %s\n", motorista);
    fprintf(arquivo, "Data de geracao: %s\n\n", __DATE__);
    fprintf(arquivo, "ENTREGAS ENCONTRADAS:\n");
    fprintf(arquivo, "====================\n\n");

    for (int i = 0; i < qtdEntregas; i++) {
        if (strcmp(entregas[i].motorista_responsavel, motorista) == 0) {
            entregas_encontradas++;

            fprintf(arquivo, "ENTREGA #%d\n", entregas_encontradas);
            fprintf(arquivo, "-----------\n");
            fprintf(arquivo, "ID da Entrega: %d\n", entregas[i].id_entrega);
            fprintf(arquivo, "Data de Coleta: %s\n", entregas[i].data_coleta);
            fprintf(arquivo, "Data Prevista: %s\n", entregas[i].data_entrega_prevista);
            fprintf(arquivo, "Data Realizada: %s\n", entregas[i].data_entrega_realizada);
            fprintf(arquivo, "Status: %s\n", entregas[i].status_entrega);
            fprintf(arquivo, "Localizacao Atual: %s\n", entregas[i].localizacao_atual);
            fprintf(arquivo, "Motorista: %s\n", entregas[i].motorista_responsavel);
            fprintf(arquivo, "Veiculo: %s\n", entregas[i].veiculo_utilizado);
            fprintf(arquivo, "Valor do Frete: R$ %.2f\n\n", entregas[i].valor_frete);

            int index_cliente = buscar_cliente_porID(entregas[i].id_cliente);
            if (index_cliente >= 0) {
                fprintf(arquivo, "INFORMACOES DO CLIENTE:\n");
                fprintf(arquivo, "----------------------\n");
                fprintf(arquivo, "ID: %d\n", clientes[index_cliente].id_cliente);
                fprintf(arquivo, "Nome: %s\n", clientes[index_cliente].nome_completo);
                fprintf(arquivo, "CPF/CNPJ: %s\n", clientes[index_cliente].cpf_cnpj);
                fprintf(arquivo, "Telefone: %s\n", clientes[index_cliente].telefone);
                fprintf(arquivo, "Email: %s\n", clientes[index_cliente].email);
                fprintf(arquivo, "Endereco: %s\n\n", clientes[index_cliente].endereco);
            }

            int index_carga = -1;
            for (int j = 0; j < qtdCargas; j++) {
                if (cargas[j].id_carga == entregas[i].id_carga) {
                    index_carga = j;
                    break;
                }
            }

            if (index_carga >= 0) {
                fprintf(arquivo, "INFORMACOES DA CARGA:\n");
                fprintf(arquivo, "--------------------\n");
                fprintf(arquivo, "ID: %d\n", cargas[index_carga].id_carga);
                fprintf(arquivo, "Descricao: %s\n", cargas[index_carga].descricao);
                fprintf(arquivo, "Peso: %.2f kg\n", cargas[index_carga].peso);
                fprintf(arquivo, "Dimensoes: %s\n", cargas[index_carga].dimensoes);
                fprintf(arquivo, "Tipo de Material: %s\n", cargas[index_carga].tipo_material);
                fprintf(arquivo, "Valor Declarado: R$ %.2f\n", cargas[index_carga].valor_declarado);
                fprintf(arquivo, "Origem: %s\n", cargas[index_carga].origem);
                fprintf(arquivo, "Destino: %s\n", cargas[index_carga].destino);
                fprintf(arquivo, "Observacoes: %s\n\n", cargas[index_carga].observacoes);
            }

            fprintf(arquivo, "========================================\n\n");
        }
    }

    fprintf(arquivo, "RESUMO:\n");
    fprintf(arquivo, "=======\n");
    fprintf(arquivo, "Total de entregas do motorista '%s': %d\n", motorista, entregas_encontradas);
    fprintf(arquivo, "Total de entregas cadastradas: %d\n", qtdEntregas);

    fclose(arquivo);

    printf("\nRelatorio gerado com sucesso!\n");
    printf("Arquivo: %s\n", nome_arquivo);
    printf("Total de entregas encontradas: %d\n", entregas_encontradas);
    printf("\n\t[0] Voltar\n");

    int escolha = 0;
    for (;;) {
        if (scanf("%d", &escolha) != 1) { limpa_stdin(); continue; }
        if (escolha == 0) { limpa_stdin(); menu_relatorios(); break; }
        printf("Essa nao e uma escolha valida!\n");
        limpa_stdin();
    }
}

void handle_escolhas_menu(int indice_menu) {
    int escolha = -1;
    for (;;) {
        if (scanf("%d", &escolha) != 1) {
            printf("Entrada invalida. Digite um numero.\n");
            limpa_stdin();
            continue;
        }
        limpa_stdin();

        if (indice_menu == 0) {
            if (escolha == 0) { mostrar_menu_clientes(); break; }
            else if (escolha == 1) { mostrar_menu_cargas(); break; }
            else if (escolha == 2) { mostrar_menu_entregas(); break; }
            else if (escolha == 3) { menu_relatorios(); break; }
            else if (escolha == 4) { pre_carregar_dados(); break; }
            else if (escolha == 5) { sair_programa(); break; }
            else { printf("A sua entrada foi invalida! Tente novamente.\n\n"); }

        } else if (indice_menu == 1) {
            if (escolha == 0) { mostrar_menu_inicial(); break; }
            else if (escolha == 1) { cadastrar_cliente(&clientes, &qtdClientes); break; }
            else if (escolha == 2) {
                int idx = buscar_cliente();
                if (idx >= 0) {
                    mostrar_cliente(idx);
                } else {
                    printf("\n\t[0] Voltar\n");
                    for (;;) {
                        if (scanf("%d", &escolha) != 1) { limpa_stdin(); continue; }
                        if (escolha == 0) { limpa_stdin(); mostrar_menu_clientes(); break; }
                        printf("Essa nao e uma escolha valida!\n");
                        limpa_stdin();
                    }
                }
                break;
            }
            else if (escolha == 3) { listar_todos_clientes(); break; }
            else if (escolha == 4) { editar_cliente(); break; }
            else if (escolha == 5) { deletar_cliente(); break; }
            else { printf("A sua entrada foi invalida! Tente novamente.\n\n"); }
        } else if (indice_menu == 2) {
            if (escolha == 0) { mostrar_menu_inicial(); break; }
            else if (escolha == 1) { cadastrar_carga(&cargas, &qtdCargas); break; }
            else if (escolha == 2) {
                int idx = buscar_carga();
                if (idx >= 0) {
                    mostrar_carga(idx);
                } else {
                    printf("\n\t[0] Voltar\n");
                    for (;;) {
                        if (scanf("%d", &escolha) != 1) { limpa_stdin(); continue; }
                        if (escolha == 0) { limpa_stdin(); mostrar_menu_cargas(); break; }
                        printf("Essa nao e uma escolha valida!\n");
                        limpa_stdin();
                    }
                }
                break;
            }
            else if (escolha == 3) { listar_todas_cargas(); break; }
            else if (escolha == 4) { editar_carga(); break; }
            else if (escolha == 5) { deletar_carga(); break; }
            else { printf("A sua entrada foi invalida! Tente novamente.\n\n"); }
        } else if (indice_menu == 3) {
            if (escolha == 0) { mostrar_menu_inicial(); break; }
            else if (escolha == 1) { cadastrar_entrega(&entregas, &qtdEntregas); break; }
            else if (escolha == 2) {
                int idx = buscar_entrega();
                if (idx >= 0) {
                    mostrar_entrega(idx);
                } else {
                    printf("\n\t[0] Voltar\n");
                    for (;;) {
                        if (scanf("%d", &escolha) != 1) { limpa_stdin(); continue; }
                        if (escolha == 0) { limpa_stdin(); mostrar_menu_entregas(); break; }
                        printf("Essa nao e uma escolha valida!\n");
                        limpa_stdin();
                    }
                }
                break;
            }
            else if (escolha == 3) { listar_todas_entregas(); break; }
            else if (escolha == 4) { editar_entrega(); break; }
            else if (escolha == 5) { deletar_entrega(); break; }
            else { printf("A sua entrada foi invalida! Tente novamente.\n\n"); }
        } else {
            printf("Menu desconhecido.\n");
            break;
        }
    }
}

void mostrar_menu_inicial(void) {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
    printf("Bem vindo ao sistema de Cadastro de Logistica!\n");
    printf("-----------------------------------------------------\n\n");
    printf("Digite uma das opcoes abaixo para continuar\n");
    printf("\t[0] Opcoes de Cliente\n");
    printf("\t[1] Opcoes de Carga\n");
    printf("\t[2] Opcoes de Entrega\n");
    printf("\t[3] Relatorios de Entregas\n");
    printf("\t[4] Pre-carregar Dados de Exemplo\n");
    printf("\t[5] Sair do Programa\n");
    handle_escolhas_menu(0);
}

void mostrar_menu_clientes(void) {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
    printf("Bem vindo ao sistema de Cadastro de Logistica!\n");
    printf("-----------------------------------------------------\n\n");
    printf("Digite uma das opcoes abaixo para continuar\n");
    printf("\t[1] Cadastrar Cliente\n");
    printf("\t[2] Consultar Cliente\n");
    printf("\t[3] Listar Todos os Clientes\n");
    printf("\t[4] Editar Cliente\n");
    printf("\t[5] Deletar Cliente\n");
    printf("\n\t[0] Voltar\n");
    handle_escolhas_menu(1);
}

void mostrar_menu_cargas(void) {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
    printf("Bem vindo ao sistema de Cadastro de Logistica!\n");
    printf("-----------------------------------------------------\n\n");
    printf("Digite uma das opcoes abaixo para continuar\n");
    printf("\t[1] Cadastrar Carga\n");
    printf("\t[2] Consultar Carga\n");
    printf("\t[3] Listar Todas as Cargas\n");
    printf("\t[4] Editar Carga\n");
    printf("\t[5] Deletar Carga\n");
    printf("\n\t[0] Voltar\n");
    handle_escolhas_menu(2);
}

void mostrar_menu_entregas(void) {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
    printf("Bem vindo ao sistema de Cadastro de Logistica!\n");
    printf("-----------------------------------------------------\n\n");
    printf("Digite uma das opcoes abaixo para continuar\n");
    printf("\t[1] Cadastrar Entrega\n");
    printf("\t[2] Consultar Entrega\n");
    printf("\t[3] Listar Todas as Entregas\n");
    printf("\t[4] Editar Entrega\n");
    printf("\t[5] Deletar Entrega\n");
    printf("\n\t[0] Voltar\n");
    handle_escolhas_menu(3);
}

int main(void) {
    carregar_clientes();
    carregar_cargas();
    carregar_entregas();
    mostrar_menu_inicial();
    return 0;
}
