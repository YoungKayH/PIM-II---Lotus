#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <conio.h>
#include <time.h>

#ifdef _WIN32
#define Limpar "cls"
#endif

// Definindo limites de produtos, usuários e itens no carrinho
#define MAX_PRODUTOS 1000
#define MAX_USUARIOS 10
#define MAX_CARRINHO 50

// Estrutura para armazenar informações dos produtos
typedef struct {
    int id;
    char nome[50];
    float preco;
    int estoque;
    int quantidade;
    char validade[11]; //Data de validade no formato dd/mm/aa
} Produto;

// Estrutura para armazenar informações dos usuários
typedef struct {
    char nome[50];
    char senha[20];
    int nivel;
} Usuario;

// Estrutura para armazenar itens do carrinho de compras
typedef struct {
    Produto produto;
    int quantidade;
} ItemCarrinho;

//Funções utilizados para o programa
int autenticarUsuario(Usuario usuarios[], int quantidade, Usuario *usuarioLogado);
void Caixa(Produto produtos[], int quantidade);
void visualizarProdutos();
void cadastrarProduto(Produto produtos[], int *quantidade);
void ordenarProdutosPorNome(Produto produtos[], int quantidade);
float validarEntradaNumericaFloat(char *prompt);
void limparBuffer();
void editarProduto();
int cadastrarUsuario(Usuario usuarios[], int *quantidadeUsuarios);
int validarNome(char *nome);
void minusculo(char *str);
void menuDoacao(Produto produtos[], int quantidade);
void removerProduto(Produto produtos[], int *quantidade);
void Doacao(Produto produtos[], int quantidade);
int LeituraUsu(Usuario usuarios[], int *quantidadeUsuarios);
void vendas(ItemCarrinho carrinho[], int carrinhoCount, float subtotal, char *formaPagamento, char *horario);
void gerarRelatorio();
void leituraprodutos(Produto produtos[], int *quantidade);
void Caixaestoque(ItemCarrinho carrinho[], int carrinhoCount);

// Função principal
int main() 
{
    Produto produtos[MAX_PRODUTOS];
    Usuario usuarios[MAX_USUARIOS] = {"admin", "admin", 1};
    int quantidadeProdutos = 0;
    int quantidadeUsuarios = 0; //numero inicial de usuarios
    Usuario usuarioLogado;
    char opcao;
    int p;
    LeituraUsu(usuarios, &quantidadeUsuarios);
    leituraprodutos(produtos, &quantidadeProdutos);

    do 
	{
    	printf("\t____________________________________________\n	|					   |");
        printf("\n\t| Digite suas credenciais para fazer login |\n");
        printf("\t|__________________________________________|\n");

        if (!autenticarUsuario(usuarios, MAX_USUARIOS, &usuarioLogado)) 
		{
            printf("\tUsuario ou senha invalidos.\n");
            sleep(2);
            system(Limpar);
            continue;
        }
        do 
		{
        	printf("\t------------------------------------\n");
        	printf("\t|| Bem vindo ao hortifruti Lotus! ||\n");
       		printf("---------------------------------------------------------");
        	printf("\n|\t##        ####    ######   ##  ##    ####\t|");
        	printf("\n|\t##       ##  ##     ##     ##  ##   ##  ##\t|"); 
        	printf("\n|\t##       ##  ##     ##     ##  ##   ##\t\t|"); 
        	printf("\n|\t##       ##  ##     ##     ##  ##    ####\t|"); 
        	printf("\n|\t##       ##  ##     ##     ##  ##       ##\t|");
        	printf("\n|\t##       ##  ##     ##     ##  ##   ##  ##\t|"); 
        	printf("\n|\t######    ####      ##      ####     ####\t|\n");
        	printf("---------------------------------------------------------\n");
            printf("\t\t--- Menu Principal ---\n");
            printf("\t1. Visualizar produtos\n");
            printf("\t2. Cadastrar produto (somente gerente)\n");
            printf("\t3. Caixa\n");
            printf("\t4. Cadastro de usuario (somente gerente)\n");
            printf("\t5. Doacao de Produtos\n");
            printf("\t6. Editar produto\n");
            printf("\t7. Sair ao login\n");
            printf("\tEscolha uma opcao: ");
			scanf(" %c", &opcao);

            switch (opcao) {
                case '1':
                    visualizarProdutos(produtos, quantidadeProdutos);
                break;
                case '2':
                    if (usuarioLogado.nivel == 1)
					{
                        cadastrarProduto(produtos, &quantidadeProdutos);
                    }
					else 
					{
                        printf("\tAcesso negado. Funcao restrita ao gerente.\n");
                        printf("\tPressione Enter para continuar!");
                        limparBuffer();
                        getchar();
                        system(Limpar);
                    }
                break;
                case '3':
                    Caixa(produtos, quantidadeProdutos);
                break;
                case '4':
                	if (usuarioLogado.nivel == 1) 
					{
                        cadastrarUsuario(usuarios, &quantidadeUsuarios);
                    } 
					else 
					{
                        printf("\tAcesso negado. Funcao restrita ao gerente.\n");
                        printf("\tPressione Enter para continuar!");
                        getchar();
                    }
                break;
                case '5':
    				menuDoacao(produtos, quantidadeProdutos);
                break;
                case '6':
                    if (usuarioLogado.nivel == 1) 
					{
                        editarProduto(produtos, quantidadeProdutos);
                    } 
					else 
					{
                        printf("\nAcesso negado.\n");
                    }
                break;
                case '7':
                	printf("\tRetornando a tela de login");
                    for(p = 1; p <= 4; p++) 
					{
                    	printf(".");
                    	sleep(1);
                	}
                    system(Limpar);
                    limparBuffer();
                break;
                default:
                    printf("Opcao invalida. Tente novamente.\n");
            }
        } while (opcao != '7');

    } while (1);

    return 0;
}

// Função para limpar o buffer do teclado
void limparBuffer() 
{
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

// Função para validar a entrada de números inteiros
int validarEntradaNumerica(char *prompt) 
{
    char entrada[20];
    int numero;
    int i;

    while (1) {
        printf("%s", prompt);
        fgets(entrada, sizeof(entrada), stdin);

        int valido = 1;
        for (i = 0; i < strlen(entrada) - 1; i++) 
		{
            if (!isdigit(entrada[i])) // Verifica se cada caractere é um dígito
			{ 
                valido = 0;
                printf("\n\tEntrada invalida! Apenas numeros sao permitidos ou numeros positivos.\n");
                break;
            }
        }

        if (valido) 
		{
            numero = atoi(entrada); // Converte a string em int
            break;
        }
    }
    return numero;
}

// Função para validar a entrada de floats
float validarEntradaNumericaFloat(char *prompt) 
{
    char entrada[20];
    float numero;
    int i;

    while (1) 
	{
        printf("%s", prompt);
        fgets(entrada, sizeof(entrada), stdin);

        int valido = 1;
        for (i = 0; i < strlen(entrada) - 1; i++) 
		{
            if (!isdigit(entrada[i]) && entrada[i] != '.') // Permite ponto decimal 
			{ 
                valido = 0;
                printf("\n\tEntrada invalida! Apenas numeros ou ponto decimal sao permitidos.\n");
                break;
            }
        }

        if (valido) 
		{
            numero = atof(entrada); // Converte a string em float
            break;
        }
    }
    return numero;
}
// Cadastramento de um novo produto no sistema
void cadastrarProduto(Produto produtos[], int *quantidade) 
{
	int i;
    if (*quantidade >= MAX_PRODUTOS) {
        printf("\tLimite de produtos atingido.\n");
        sleep(3);
        return;
    }

    Produto novoProduto;
    system(Limpar);
    printf("\t\t\t-----Menu de Cadastro de Produtos-----\n\n");
    novoProduto.id = *quantidade > 0 ? produtos[*quantidade - 1].id + 1 : 1;  // Geração de ID automatica
    printf("\tID do Produto: %d\n", novoProduto.id);
	
	limparBuffer();
    printf("\tDigite o nome do produto: ");
    fgets(novoProduto.nome, sizeof(novoProduto.nome), stdin);
    novoProduto.nome[strcspn(novoProduto.nome, "\n")] = '\0'; // Remove a nova linha
    
    if (!validarNome(novoProduto.nome)) 
	{
    	printf("\n\tErro: Nome invalido. Use apenas letras.\n");
    	sleep(3);
    	return;
	}
	minusculo(novoProduto.nome);
    novoProduto.preco = validarEntradaNumericaFloat("\tDigite o preco do produto(Un): ");
    novoProduto.estoque = validarEntradaNumerica("\tDigite a quantidade em estoque(Un): ");

    printf("\tDigite a data de validade (DD/MM/AAAA): ");
    fgets(novoProduto.validade, sizeof(novoProduto.validade), stdin);
    novoProduto.validade[strcspn(novoProduto.validade, "\n")] = '\0';
    
    for (i = 0; i < *quantidade; i++) 
	{
        if (produtos[i].id == novoProduto.id) 
		{
            printf("\tErro: ID do produto ja cadastrado.\n");
            sleep(3);
            system(Limpar);
            return;
        }
        if (strcmp(produtos[i].nome, novoProduto.nome) == 0) 
		{
            printf("\tErro: Nome do produto ja cadastrado.\n");
            sleep(3);
            system(Limpar);
            return;
        }
    }

    produtos[*quantidade] = novoProduto;
    (*quantidade)++;
    
    FILE *produtolista = fopen("Produtos.csv", "a+");
    if (produtolista == NULL) 
	{
        printf("\tErro ao abrir o arquivo para salvar o produto.\n");
        return;
    }
    
    fseek(produtolista, 0, SEEK_END);// Verifica se o arquivo está vazio
    if (ftell(produtolista) == 0) 
    {
        
        fprintf(produtolista, "ID;NOME;PRECO;ESTOQUE;VALIDADE\n");// caso não tiver cabeçalho, ele será criado
    }
    
    fprintf(produtolista, "%d;%s;%.2f;%d;%s\n", 
        novoProduto.id, 
        novoProduto.nome,  
        novoProduto.preco, 
        novoProduto.estoque,
		novoProduto.validade);
    fclose(produtolista);
    
    printf("\tProduto cadastrado com sucesso!\n");
    printf("\tPressione Enter para continuar\n");
    getchar();
    system(Limpar);
}

// Visualizar a lista de produtos cadastrados
void visualizarProdutos()  
{
    FILE *produtolista = fopen("Produtos.csv", "r");
    if (produtolista == NULL) 
    {
        printf("\tErro ao abrir o arquivo de produtos!\n");
        sleep(3);
        return;
    }

    Produto produto;
    char linha[256];
    int quantidadeProdutos = 0;

    system(Limpar);
    printf("\t--- Lista de Produtos Cadastrados ---\n");
    printf("\t| %-10s | %-30s | %-10s | %-10s |\n", "ID", "Nome", "Preco", "Quantidade");
    printf("\t--------------------------------------------------------------\n");

    // Ignora o cabeçalho do arquivo
    fgets(linha, sizeof(linha), produtolista);

    // Lê os produtos do arquivo
    while (fgets(linha, sizeof(linha), produtolista)) 
    {
        sscanf(linha, "%d;%49[^;];%f;%d;%19[^\n]", 
               &produto.id, 
               produto.nome, 
               &produto.preco, 
               &produto.estoque, 
               produto.validade);

        // Exibe o produto na tela
        printf("\t| %-10d | %-30s | %-10.2f | %-10d |\n", 
               produto.id, 
               produto.nome, 
               produto.preco, 
               produto.estoque);

        quantidadeProdutos++;
    }

    printf("\t--------------------------------------------------------------\n");

    if (quantidadeProdutos == 0) 
    {
        printf("\tNenhum produto cadastrado.\n");
    }

    fclose(produtolista);

    printf("\n\tPressione Enter para continuar!");
    limparBuffer();
    getchar();
    system(Limpar);
}

// Adiciona produtos ao caixa e calcula o subtotal
void Caixa(Produto produtos[], int quantidade) 
{
    ItemCarrinho carrinho[MAX_CARRINHO];
    int carrinhoCount = 0;
    int id, encontrado, qtd, opPaga, i, opMenu;
    float subtotal = 0.0;
    char nome[50];
    char escolha;
    float valorCalc;
    float taxa = 2.69;

    while (1) 
	{
		system(Limpar);
		printf("\t----Menu do Caixa----\n\n");
		printf("\tDeseja fazer o que: \n\t1. Adicionar item\n\t2. Gerar relatorio de vendas\n\t3. Voltar\n\t");
		scanf("%d", &opMenu);
		
		switch(opMenu)
		{
			case 1:
			printf("\tDeseja adicionar um produto a lista por (i)ID ou (n)Nome? (Digite 'S' para sair): ");
        	scanf(" %c", &escolha); // Captura a escolha do usuário
        	
        	if (escolha == 'S' || escolha == 's') break;
        	encontrado = 0;
        	
        	// Procura pelo produto usando ID ou nome
        	if (escolha == 'I' || escolha == 'i') 
			{
				limparBuffer();
            	id = validarEntradaNumerica("\tDigite o ID do produto: ");
            	for (i = 0; i < quantidade; i++) 
				{
                	if (produtos[i].id == id) 
					{
						limparBuffer();
                    	qtd = validarEntradaNumerica("\tQuantos desse produto deseja adicionar ao carrinho? \n\t");
                    	
                    	// Verifica se o estoque é suficiente
                    	if (produtos[i].estoque < qtd) 
						{
                            printf("\tEstoque insuficiente para este produto!\n");
                            sleep(2);
                            break;
                        }
                        
                    	// Adiciona o produto ao carrinho
                    	carrinho[carrinhoCount].produto = produtos[i];
                    	carrinho[carrinhoCount].quantidade = qtd;
                    	carrinhoCount++;
                    	subtotal += produtos[i].preco * qtd;
                        
                    	// Atualiza o estoque do produto
                    	produtos[i].estoque -= qtd;
                    	printf("\n\tProduto(s) adicionado!");
                    	encontrado = 1;
                    	break;
                	}
            	}
        	} 
			else if (escolha == 'N' || escolha == 'n') 
			{
				limparBuffer();
            	printf("\tDigite o nome do produto: ");
            	fgets(nome, sizeof(nome), stdin);
            	nome[strcspn(nome, "\n")] = '\0';

            	for (i = 0; i < quantidade; i++) 
				{
                	if (strcmp(produtos[i].nome, nome) == 0) 
					{
                    	qtd = validarEntradaNumerica("\tQuantos desse produto deseja adicionar ao carrinho? \n\t");
                    	
                    	// Verifica se o estoque é suficiente
                    	if (produtos[i].estoque < qtd) 
						{
                            printf("\tEstoque insuficiente para este produto!\n");
                            sleep(2);
                            break;
                        }
                        
                    	// Adiciona o produto ao carrinho
                    	carrinho[carrinhoCount].produto = produtos[i];
                    	carrinho[carrinhoCount].quantidade = qtd;
                    	carrinhoCount++;
                    	subtotal += produtos[i].preco * qtd;
                        
                    	// Atualiza o estoque do produto
                    	produtos[i].estoque -= qtd;
                    	printf("\n\tProduto(s) adicionado!");
                    	encontrado = 1;
                    	break;
                	}
            	}
        	}
        	else 
			{
				printf("\n\tOpcao nao aceita!");
				limparBuffer();
				printf("\tPressione Enter para continuar...");
				getchar();
			}

        	if (!encontrado) 
			{
            	printf("\tProduto nao encontrado!\n");
            	sleep(5);
        	}
        	// Crie um array temporário apenas com os itens vendidos

			// Chame a função com os produtos vendidos
			Caixaestoque(carrinho, carrinhoCount);
        	
        	// Exibe o resumo da venda
    		printf("\n\tDeseja pagar com o que: \n\t1. Dinheiro\n\t2. Cartao\n\t3. Pix\n\t");
    		scanf("%d", &opPaga);
    		
    		time_t t = time(NULL);
            struct tm tm = *localtime(&t);
            char horario[20];
            strftime(horario, sizeof(horario), "%Y-%m-%d %H:%M:%S", &tm);
            
    		switch(opPaga)
    		{
    			case 1: // Dinheiro
    				printf("\n\n\t--- Resumo da Venda ---\n");
    				printf("\t| %-10s | %-30s | %-10s | %-10s |\n", "ID", "Nome", "Quantidade", "Preco Total");
    				printf("\t-----------------------------------------------------------\n");
    				for (i = 0; i < carrinhoCount; i++) 
					{
        				printf("\t| %-10d | %-30s | %-10d | %-10.2f |\n", 
            			carrinho[i].produto.id, carrinho[i].produto.nome, carrinho[i].quantidade, 
            			carrinho[i].produto.preco * carrinho[i].quantidade);
    				}
    				printf("\t-----------------------------------------------------------\n");
    				printf("\tSubtotal da venda: %.2f\n", subtotal);
    				vendas(carrinho, carrinhoCount, subtotal, "Dinheiro", horario);
    				
    				limparBuffer();
    				printf("\tPressione Enter para continuar...");
    				getchar();
    				system(Limpar);
    			break;
    			
    			case 2: // Cartao
    				valorCalc = subtotal * (taxa / 100);
    				printf("\n\n\t--- Resumo da Venda ---\n");
    				printf("\t| %-10s | %-30s | %-10s | %-10s |\n", "ID", "Nome", "Quantidade", "Preco Total");
    				printf("\t-----------------------------------------------------------\n");
    				for (i = 0; i < carrinhoCount; i++) 
					{
        				printf("\t| %-10d | %-30s | %-10d | %-10.2f |\n", 
            			carrinho[i].produto.id, carrinho[i].produto.nome, carrinho[i].quantidade, 
            			carrinho[i].produto.preco * carrinho[i].quantidade);
    				}
    				printf("\t-----------------------------------------------------------\n");
    				subtotal += valorCalc;
    				printf("\tSubtotal da venda: %.2f, taxa da maquininha ja aplicada valor de: %.2f \n\n", subtotal, valorCalc);
    				vendas(carrinho, carrinhoCount, subtotal, "Cartao", horario);
    				
    				limparBuffer();
    				printf("\tPressione Enter para continuar...");
    				getchar();
    				system(Limpar);
    			break;
    			
    			case 3:// Pix
    				printf("\n\n\t--- Resumo da Venda ---\n");
    				printf("\t| %-10s | %-30s | %-10s | %-10s |\n", "ID", "Nome", "Quantidade", "Preco Total");
    				printf("\t-----------------------------------------------------------\n");
    				for (i = 0; i < carrinhoCount; i++) 
					{
        				printf("\t| %-10d | %-30s | %-10d | %-10.2f |\n", 
            			carrinho[i].produto.id, carrinho[i].produto.nome, carrinho[i].quantidade, 
            			carrinho[i].produto.preco * carrinho[i].quantidade);
    				}
    				printf("\t-----------------------------------------------------------\n");
    				printf("\tSubtotal da venda: %.2f\n", subtotal);
    				vendas(carrinho, carrinhoCount, subtotal, "PIX", horario);
    				
    				limparBuffer();
    				printf("\tPressione Enter para continuar...");
    				getchar();
    				system(Limpar);
    			break;
    			default:
    				printf("Opcao nao valida!");
    			break;
			}
        	break;
        	case 2:
        		gerarRelatorio();
        	break;
        	case 3:
        		printf("\n\tPressione Enter para continuar!");
        		limparBuffer();
        		getchar();
        		system(Limpar);
				return;        		
        	break;
    	}
	}      
}


int autenticarUsuario(Usuario usuarios[], int quantidade, Usuario *usuarioLogado) //Autenticar usuario
{
    char nome[50], senha[20];
    int i;

    printf("\n\tLogin: ");
    fgets(nome, sizeof(nome), stdin);
    nome[strcspn(nome, "\n")] = '\0'; // Remove a quebra de linha

    printf("\tSenha: ");
    fgets(senha, sizeof(senha), stdin);
    senha[strcspn(senha, "\n")] = '\0';

    for (i = 0; i < quantidade; i++) 
	{
        if (strcmp(usuarios[i].nome, nome) == 0 && strcmp(usuarios[i].senha, senha) == 0) 
		{
            *usuarioLogado = usuarios[i];
            sleep(1);
            system(Limpar); 
            return 1;
        }
    }
    return 0;
}

int cadastrarUsuario(Usuario usuarios[], int *quantidadeUsuarios) 
{
    if (*quantidadeUsuarios >= MAX_USUARIOS) 
    {
        printf("\nLimite de usuarios atingido.\n");
        return 1;
    }

    Usuario novoUsuario;
    limparBuffer();

    FILE *usuario = fopen("usuarios.csv", "a+"); // "a+" permite leitura e escrita
    if (usuario == NULL)
    {
        printf("\tErro ao abrir o arquivo!");
    	fopen("usuarios.csv","w");
        return 1;
    }

    fseek(usuario, 0, SEEK_END);// Verifica se o Arquivo está vazio
    if (ftell(usuario) == 0) 
    {
        // Arquivo vazio: escreve o cabeçalho
        fprintf(usuario, "NOME DO USUARIO;CARGO;SENHA\n");
    }
    
    printf("\tDigite o nome do novo usuario: ");
    fgets(novoUsuario.nome, sizeof(novoUsuario.nome), stdin);
    novoUsuario.nome[strcspn(novoUsuario.nome, "\n")] = '\0';

    if (!validarNome(novoUsuario.nome)) 
    {
        printf("\n\tErro: Nome invalido. Use apenas letras.\n");
        fclose(usuario);
        return 1;
    }

    printf("\tDigite a senha do novo usuario: ");
    fgets(novoUsuario.senha, sizeof(novoUsuario.senha), stdin);
    novoUsuario.senha[strcspn(novoUsuario.senha, "\n")] = '\0';

    novoUsuario.nivel = validarEntradaNumerica("\tDigite o nivel do usuario (0 = vendedor, 1 = gerente): ");
    if (novoUsuario.nivel < 0 || novoUsuario.nivel > 1) 
    {
        printf("\tNivel invalido. Usuario nao cadastrado.\n");
        fclose(usuario);
        return 1;
    }

    fprintf(usuario, "%s;%d;%s\n", novoUsuario.nome, novoUsuario.nivel, novoUsuario.senha);

    usuarios[*quantidadeUsuarios] = novoUsuario;
    (*quantidadeUsuarios)++;

    printf("\n\tUsuario cadastrado com sucesso!\n");
    printf("\tPressione Enter para continuar...");
    getchar();
	system(Limpar);
	
    fclose(usuario); // Fecha o arquivo após o uso
    return 0;
}


int validarNome(char *nome) //Basicamente serve para impedir do usuario colocar numeros no nome do produto
{
    int i;
    for (i = 0; nome[i]; i++) {
        if (!isalpha(nome[i]) && nome[i] != ' ') { // Permite apenas letras e espaços
            return 0;
        }
    }
    return 1;
}
void minusculo(char *str) //serve pra ver a difereciação de minusculo e maiusculo e deixar os dois iguais
{
    int i;
    for (i = 0; str[i]; i++) {
        str[i] = tolower(str[i]);
    }
}

void editarProduto() 
{
    FILE *produtolista = fopen("Produtos.csv", "r+");
    if (produtolista == NULL) 
    {
        printf("\tErro ao abrir o arquivo de produtos!\n");
        return;
    }

    Produto produto;
    char linha[256];
    int id, opcao, opEdit;
    int i;
    int produtoEncontrado = 0;
    long int posicaoAtual;
    FILE *tempFile; // Arquivo temporário

    printf("\n\tDeseja editar ou remover: \n\t1. Editar\n\t2. Remover\n");
    scanf("%d", &opEdit);
    limparBuffer();

    switch (opEdit) 
    {
        case 1: // Editar Produto
            printf("\n\tDigite o ID do produto a ser editado: ");
            scanf("%d", &id);
            limparBuffer();

            // Cria um arquivo temporário para armazenar os produtos editados
            tempFile = fopen("Produtos_temp.csv", "w");
            if (tempFile == NULL) 
            {
                printf("\tErro ao criar o arquivo temporário!\n");
                fclose(produtolista);
                return;
            }

            // Escreve o cabeçalho no arquivo temporário
            fprintf(tempFile, "ID;NOME;PRECO;ESTOQUE;VALIDADE\n");

            // Lê o arquivo linha por linha
            while (fgets(linha, sizeof(linha), produtolista)) 
            {
                // Ignora o cabeçalho
                if (strstr(linha, "ID;NOME;PRECO;ESTOQUE;VALIDADE") != NULL) 
                {
                    continue;
                }

                // Encontra o produto e edita
                sscanf(linha, "%d;%49[^;];%f;%d;%19[^\n]", 
                       &produto.id, 
                       produto.nome, 
                       &produto.preco, 
                       &produto.estoque, 
                       produto.validade);

                if (produto.id == id) 
                {
                    produtoEncontrado = 1;

                    // Menu de edição
                    system(Limpar);
                    printf("\t----Menu de Alteracao----");
                    printf("\n\tEditar: \n\t1. Nome \n\t2. Preco \n\t3. Estoque\n");
                    printf("\tEscolha uma opcao: ");
                    scanf("%d", &opcao);
                    limparBuffer();

                    // Editar produto conforme a opção
                    if (opcao == 1) 
                    {
                        printf("\tNovo nome: ");
                        fgets(produto.nome, sizeof(produto.nome), stdin);
                        produto.nome[strcspn(produto.nome, "\n")] = '\0'; // Remove o '\n'
                        if (!validarNome(produto.nome)) 
                        {
                            printf("\n\tErro: Nome invalido. Use apenas letras.\n");
                        } 
                        else 
                        {
                            minusculo(produto.nome);
                            printf("\tNome atualizado com sucesso!\n");
                        }
                    } 
                    else if (opcao == 2) 
                    {
                        produto.preco = validarEntradaNumericaFloat("\tNovo preco: ");
                        printf("\tPreco atualizado com sucesso!\n");
                    } 
                    else if (opcao == 3) 
                    {
                        produto.estoque = validarEntradaNumerica("\tNova quantidade em estoque: ");
                        printf("\tNova validade desses produtos(dd/mm/aa): ");
                        fgets(produto.validade, sizeof(produto.validade), stdin);
                        produto.validade[strcspn(produto.validade, "\n")] = '\0';
                        printf("\tEstoque e validade atualizados com sucesso!\n");
                    } 
                    else 
                    {
                        printf("\tOpcao invalida.\n");
                    }
                }
                
                // Escreve no arquivo temporário a linha (modificada ou não)
                fprintf(tempFile, "%d;%s;%.2f;%d;%s\n", 
                        produto.id, 
                        produto.nome,  
                        produto.preco, 
                        produto.estoque, 
                        produto.validade);
            }

            fclose(produtolista);
            fclose(tempFile);

            if (produtoEncontrado) 
            {
                // Substitui o arquivo original pelo temporário
                remove("Produtos.csv");
                rename("Produtos_temp.csv", "Produtos.csv");
                printf("\tProduto editado com sucesso!\n");
                sleep(2);
                system(Limpar);
            } 
            else 
            {
                printf("\tProduto com ID %d nao encontrado.\n", id);
                remove("Produtos_temp.csv");
            }
            break;

        case 2: // Remover Produto
            printf("\n\tDigite o ID do produto a ser removido: ");
            scanf("%d", &id);
            limparBuffer();

            // Cria um arquivo temporário para armazenar os produtos restantes
            tempFile = fopen("Produtos_temp.csv", "w");
            if (tempFile == NULL) 
            {
                printf("\tErro ao criar o arquivo temporário!\n");
                fclose(produtolista);
                return;
            }

            // Escreve o cabeçalho no arquivo temporário
            fprintf(tempFile, "ID;NOME;PRECO;ESTOQUE;VALIDADE\n");

            // Lê o arquivo original e escreve no temporário, ignorando o produto a ser removido
            produtoEncontrado = 0;
            while (fgets(linha, sizeof(linha), produtolista)) 
            {
                if (strstr(linha, "ID;NOME;PRECO;ESTOQUE;VALIDADE") != NULL) 
                {
                    continue;
                }

                sscanf(linha, "%d;%49[^;];%f;%d;%19[^\n]", 
                       &produto.id, 
                       produto.nome, 
                       &produto.preco, 
                       &produto.estoque, 
                       produto.validade);

                if (produto.id != id) 
                {
                    // Escreve os produtos que não são o removido
                    fprintf(tempFile, "%d;%s;%.2f;%d;%s\n", 
                            produto.id, 
                            produto.nome,  
                            produto.preco, 
                            produto.estoque, 
                            produto.validade);
                } 
                else 
                {
                    produtoEncontrado = 1;
                }
            }
            fclose(produtolista);
            fclose(tempFile);
            
            if (produtoEncontrado) 
            {
                // Substitui o arquivo original pelo temporário
                remove("Produtos.csv");
                rename("Produtos_temp.csv", "Produtos.csv");
                printf("\tProduto removido com sucesso!\n");
            } 
            else 
            {
                printf("\tProduto com ID %d nao encontrado.\n", id);
                remove("Produtos_temp.csv");
            }
            break;

        default:
            printf("\tOpcao invalida.\n");
            break;
    }
}

void Doacao(Produto produtos[], int quantidade) 
{
    time_t t = time(NULL);
    struct tm *dataAtual = localtime(&t);  // Certifique-se de que <time.h> está incluída
	int i;
    char validadeProduto[11];
    int diaProduto, mesProduto, anoProduto;
    
    printf("\t----Produtos proximos do vencimento----\n");
    for (i = 0; i < quantidade; i++) 
	{
        sscanf(produtos[i].validade, "%d/%d/%d", &diaProduto, &mesProduto, &anoProduto);

        if (anoProduto == dataAtual->tm_year + 1900 && mesProduto == dataAtual->tm_mon + 1 && diaProduto == dataAtual->tm_mday + 1) 
		{
            printf("ID: %d | Nome: %s | Validade: %s | Estoque: %d\n\n", produtos[i].id, produtos[i].nome, produtos[i].validade, produtos[i].estoque);
        }
    }
}

void removerProduto(Produto produtos[], int *quantidade) 
{
    if (*quantidade == 0) 
	{
        printf("\tNenhum produto cadastrado.\n");
        return;
    }

    int id, i, encontrado = 0;
    printf("\tDigite o ID do produto a ser removido: ");
    scanf("%d", &id);

    for (i = 0; i < *quantidade; i++) 
	{
        if (produtos[i].id == id) 
		{
            produtos[i] = produtos[*quantidade - 1];
            (*quantidade)--;
            encontrado = 1;
            printf("\tProduto removido com sucesso!\n");
            break;
        }
    }
    if (!encontrado) 
	printf("\tProduto nao encontrado.\n");
}

void menuDoacao(Produto produtos[], int quantidade) 
{
    if (quantidade == 0) 
	{
        printf("\tNenhum produto disponivel para doacao.\n");
        return;
    }

    int id, i, quantidadeDoada;
    system(Limpar);
    printf("\t--- Menu de Doacao ---\n\n");
    Doacao(produtos, quantidade);
    printf("\tDigite o ID do produto a ser doado: ");
    scanf("%d", &id);

    for (i = 0; i < quantidade; i++) 
	{
        if (produtos[i].id == id) 
		{
            printf("\tProduto encontrado: %s (Estoque: %d)\n", produtos[i].nome, produtos[i].estoque);
            printf("\tDigite a quantidade a ser doada: ");
            scanf("%d", &quantidadeDoada);

            if (quantidadeDoada > 0 && quantidadeDoada <= produtos[i].estoque) 
			{
                produtos[i].estoque -= quantidadeDoada;
                printf("\tDoacao realizada com sucesso!\n");
                sleep(2);
                system(Limpar);
            } 
			else 
			{
                printf("\tQuantidade invalida.\n");
                sleep(2);
                system(Limpar);
            }
            return;
        }
    }
    printf("\tProduto com ID %d nao encontrado.\n", id);
}

int LeituraUsu(Usuario usuarios[], int *quantidadeUsuarios) 
{
    FILE *usuario = fopen("usuarios.csv", "r");
    
    if (usuario == NULL) 
	{
        printf("\tErro ao abrir o arquivo de usuarios!\n");
        printf("\tCriando admin temporario!\n");
        printf("\tlogin:admin senha:admin");
        sleep(2);
        system(Limpar);
        return 1;
    }

    char linha[200];
    fgets(linha, sizeof(linha), usuario);

    while (fgets(linha, sizeof(linha), usuario)) 
	{
        Usuario user;
        if (sscanf(linha, "%99[^;];%d;%49[^\n]", user.nome, &user.nivel, user.senha) == 3) 
		{
            usuarios[*quantidadeUsuarios] = user;
            (*quantidadeUsuarios)++;
        }
    }

    fclose(usuario);
    return 0;
}
void vendas(ItemCarrinho carrinho[], int carrinhoCount, float subtotal, char *formaPagamento, char *horario) 
{
	int i;
    FILE *file = fopen("Vendas.csv", "a");
    if (file == NULL) 
	{
        printf("\tErro ao abrir o arquivo de vendas.\n");
        return;
    }
    static int primeiraExecucao = 1;
    if (primeiraExecucao) 
	{
        fprintf(file, "ID;Nome;Quantidade;Preco Unitario;Preco Total;Forma Pagamento;Data/Horario\n");
        primeiraExecucao = 0;
    }

    for (i = 0; i < carrinhoCount; i++) 
	{
        fprintf(file, "%d;%s;%d;%.2f;%.2f;%s;%s\n", 
                carrinho[i].produto.id, carrinho[i].produto.nome, 
                carrinho[i].quantidade, 
                carrinho[i].produto.preco, 
                carrinho[i].produto.preco * carrinho[i].quantidade, 
                formaPagamento,
				horario);
    }
    fprintf(file, "Subtotal;, ;, ;, ;%.2f; %s\n", subtotal, formaPagamento);
    fclose(file);
}
void gerarRelatorio() 
{
    FILE *venda = fopen("Vendas.csv", "r"); // Abre o arquivo em modo leitura
    if (venda == NULL) 
    {
        printf("\tErro: Nenhuma venda registrada ou arquivo 'Vendas.csv' não encontrado.\n");
        printf("\tPressione Enter para voltar ao menu.\n");
        limparBuffer();
        getchar();
        return;
    }

    char linha[512]; // Buffer para leitura
    int primeiraLinha = 1; // Flag para ignorar a primeira linha (cabeçalho)

    printf("\n\t--- Relatorio de Vendas ---\n\n");
    printf("\t| %-10s | %-20s | %-10s | %-15s | %-15s | %-15s |\n", 
           "ID", "Nome", "Quantidade", "Preco Unitario", "Preco Total", "Forma Pagamento");
    printf("\t---------------------------------------------------------------------------------------------\n");

    while (fgets(linha, sizeof(linha), venda)) 
    {
        if (primeiraLinha) 
        {
            // Ignora a primeira linha (cabeçalho)
            primeiraLinha = 0;
            continue;
        }

        // Verifica se a linha não está vazia antes de imprimir
        if (linha[0] != '\n' && linha[0] != '\0') 
        {
            printf("\t%s", linha);
        }
    }

    fclose(venda); // Fecha o arquivo
    printf("\n\tPressione Enter para voltar ao menu.\n");
    limparBuffer();
    getchar();
}
void leituraprodutos(Produto produtos[], int *quantidade) 
{
    FILE *produtosFile = fopen("Produtos.csv", "r");
    if (produtosFile == NULL) 
	{
        printf("\tNenhum produto encontrado. Arquivo 'Produtos.csv' nao existe.\n");
        return;
    }

    char linha[256];
    int i = 0;

    fgets(linha, sizeof(linha), produtosFile);// Esta linha serve apenas para ignorar o cabeçalho

    // Lê os produtos do arquivo e os adiciona ao vetor
    while (fgets(linha, sizeof(linha), produtosFile)) 
	{
        Produto produto;
        
        sscanf(linha, "%d;%49[^;];%f;%d;%49[^\n]", &produto.id, produto.nome, &produto.preco, &produto.estoque, produto.validade);// Lê os dados de cada produto

        produtos[i] = produto;  // Adiciona o produto ao array
        i++;
    }

    *quantidade = i;  // Serve para atualizar a lista de produtos
    fclose(produtosFile);
}
void Caixaestoque(ItemCarrinho carrinho[], int carrinhoCount) 
{
    FILE *produtolista = fopen("Produtos.csv", "r");
    if (produtolista == NULL) 
    {
        printf("\tErro ao abrir o arquivo de produtos!\n");
        return;
    }

    Produto listaProdutos[MAX_PRODUTOS];
    int totalProdutos = 0;
    char linha[256];
    int i, j;

    while (fgets(linha, sizeof(linha), produtolista)) 
    {
        if (strstr(linha, "ID;NOME;PRECO;ESTOQUE;VALIDADE") != NULL)
            continue;

        sscanf(linha, "%d;%49[^;];%f;%d;%19[^\n]", 
               &listaProdutos[totalProdutos].id, 
               listaProdutos[totalProdutos].nome, 
               &listaProdutos[totalProdutos].preco, 
               &listaProdutos[totalProdutos].estoque, 
               listaProdutos[totalProdutos].validade);
        totalProdutos++;
    }

    fclose(produtolista);

    // Atualizar o estoque com base nos itens do carrinho
    for (i = 0; i < carrinhoCount; i++) 
    {
        for (j = 0; j < totalProdutos; j++) 
        {
            if (carrinho[i].produto.id == listaProdutos[j].id) 
            {
                if (listaProdutos[j].estoque < carrinho[i].quantidade) 
                {
                    printf("\tErro: Estoque insuficiente para o produto %s (ID: %d)\n", 
                           carrinho[i].produto.nome, carrinho[i].produto.id);
                } 
                else 
                {
                    listaProdutos[j].estoque -= carrinho[i].quantidade;
                    printf("\tProduto %s (ID: %d) - Estoque atualizado: %d -> %d\n", 
                           listaProdutos[j].nome, 
                           listaProdutos[j].id, 
                           listaProdutos[j].estoque + carrinho[i].quantidade, 
                           listaProdutos[j].estoque);
                }
                break;
            }
        }
    }
    
    FILE *tempFile = fopen("Produtos_temp.csv", "w");
    if (tempFile == NULL) 
    {
        printf("\tErro ao criar o arquivo temporário!\n");
        return;
    }

    fprintf(tempFile, "ID;NOME;PRECO;ESTOQUE;VALIDADE\n");

    for (i = 0; i < totalProdutos; i++) 
    {
        fprintf(tempFile, "%d;%s;%.2f;%d;%s\n", 
                listaProdutos[i].id, 
                listaProdutos[i].nome, 
                listaProdutos[i].preco, 
                listaProdutos[i].estoque, 
                listaProdutos[i].validade);
    }

    fclose(tempFile);

    remove("Produtos.csv");
    rename("Produtos_temp.csv", "Produtos.csv");

    printf("\tEstoque atualizado com sucesso!\n");
}
