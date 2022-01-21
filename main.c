/**
 * @autores:
 * Nome: Francisco Santos                   Número de Aluno: 47711
 * Nome: Tiago Borges                       Número de Aluno: 47855
 **/

#include <stdio.h>

#define B 2
#define N 8
#define M 8
#define MOVES 02
/*
* Soma destes valores tem que ser igual a B
*/
#define PA 1
#define NT 1
#define CT 0
#define SM 0

/**Representa uma coordenada*/
typedef struct
{
    int x;
    int y;
} Position;

/**Associa cada coordenada do barco com um estado**/
typedef struct
{
    int afloat;        //0 indica que está afundada, 1 indica que não
    Position pos;      //Coordenada
} StateCoord;

/**Representa um barco**/
typedef struct
{
    int afloat;        //Posições que ainda não foram afundadas 
    int tSize;         //Tamanho do tipo do barco 
    StateCoord coord[5]; //O barco maior tem 5 coordenadas, usando o tSize garantimos que acedemos apenas às existentes
    char type;         //Caracter que representa o tipo do barco
} Boat;

/**Representa um tabuleiro**/
typedef struct
{
    int numBoatsAfloat; //Número de barcos não afundados
    int numBoats; //Número de barcos que foram colocados
    Boat boats[B]; //Array de barcos. Cada barco é do tipo Boat.
    char board[N][M]; //Array que contém a informação de cada posição do tabuleiro
} Board;

typedef struct {
	char name[25]; // Nome do utilizador
	int score; // Vitorias de rondas
} Player;

typedef struct {
	Player players[2]; // Lista de players a jogar
	int num_players; // Numero de jogadores
	int score_boats; // Barcos afundados
	int first;
	int turn; // Index do player que vai atacar
	int running; // Decide se o jogo está a decorrer ou se já terminou (1 se estiver a correr, 0 se estiver parado)
	int win_lose; // Mostra se o player atacante ganha ou perde	
	int attacker_flag;
	int showed; // Guarda se usou a função 
	Board board; // Armazena a BOARD com todos os dados do jogo
} Game;

/**
 * NOTA IMPORTANTE: 
 * Não pode alterar nenhuma das assinaturas das funções abaixo. 
 * Este ficheiro tem de ser considerado em conjunção com o enunciado do TP2.
 **/


/**
 * Function: init_board
 * 
 * Inicializa um Board.
 * 
 * n: dimensão x do tabuleiro
 * m: dimensão y do tabuleiro
 * b: apontador para o Board que vai ser inicializado
 * 
 **/
void init_board(int n, int m, Board *b)
{
    b->numBoatsAfloat = 0;
    b->numBoats = 0;

    //Inicializa o array board
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < m; j++)
        { 
            b->board[i][j] = ' ';
        }
    }
}

int check_status(char status) {
	if(status == 'A' || status == 'F' || status == '*') return 1;
	return 0;
}

/**
 * Function: print_board
 * 
 * Mostra no ecrã o estado atual do tabuleiro.
 * Se a flag é 0 (zero) não mostra os barcos, se for 1, mostra. 
 * Quando a flag é 1, para indicar a posição de cada barco, é colocada em todas
 * as posições do barco a letra que representa o seu tipo.
 * 
 * n: número de linhas do tabuleiro
 * m: número de colunas do tabuleiro
 * board: estado do tabuleiro
 * flag: indicador do modo de visualização do tabuleiro -- se for 0 (zero) não 
 *       mostra os barcos; se for diferente de 0 (zero) mostra.
 */

void print_board(int n, int m, char board[n][m], int flag)
{
    printf("+");
    for (int j = 0; j < n; j++)
    {
        printf("---+");
    }
    printf("\n");

    for (int i = 0; i < m; i++)
    {
        printf("|");
        if (flag) //Mostra tudo
        {
            for (int j = 0; j < n; j++)
            {
                printf(" %c |", board[i][j]);
            }
            printf("\n");
        }
        else // Esconde os barcos
        {
			for (int j = 0; j < n; j++)
            {
				// 1 se for A ou F ou *
				// 0 se for um barco não afundado
				int status = check_status(board[i][j]); 
				if(status) {
					printf(" %c |", board[i][j]);
				}
				if(!status) {
					printf("   |");
				}
            }
            printf("\n");
        }

        printf("+");
        for (int j = 0; j < n; j++)
        {
            printf("---+");
        }
        printf("\n");
    }
}


/** 
 * Function: typeToSize
 * 
 * Dado o tipo do barco devolve o seu tamanho.
 * 
 * type: tipo do barco ('P', 'N', 'C', ou 'S')
 * 
 * return
 *  -1 se o tipo de barco for inválido
 *  caso contrário, devolve o tamanho do barco
 **/
int typeToSize(char type)
{
    switch(type)
    {
        case 'P':
			return 5;
        case 'N':
			return 4;
        case 'C':
			return 3;
        case 'S':
			return 2;
        default:
			return -1;
    }
}

/**
* typeToName 
* Esta função deverá retornar o nome por extenso
**/
char *typeToName(char type) {
	switch(type) {
		case 'P':
			return "Porta-Avioes\0";
		case 'N':
			return "Navio-Tanque\0";
		case 'C':
			return "Contratorpedeiro\0";
		case 'S':
			return "Submarinos\0";
		default:
			return "\0";
	}
}

/**
 * Function: init_boat
 * 
 * Dado um apontador para um barco, o tipo, a posição inicial e a direcção, 
 * inicializa o barco com todas as sua posições, tamanho e tipo. 
 * Por exemplo, se o tipo do barco é 'C' (contratorpedeiro),
 * a posição inicial (1,1), e a direcção 'H', então o barco fica inicializado 
 * com os seguintes valores
 * 
 * afloat = 3;
 * tSize = 3;
 * coord = [(1,(1,1)),(1,(1,2)),(1,(1,3))]; Onde (s,(x,y)) representa um elemento do tipo StateCoord
 * type = 'C';
 * 
 * b: barco que vai ser inicializado
 * type: tipo do barco
 * xy: posição inicial do barco
 * dir: direcção do barco
 * 
 **/
void init_boat(Boat *b, char type, Position xy, char dir)
{
	int size = typeToSize(type);

	b->afloat = size;
	b->tSize = size;
	b->type = type;

	StateCoord state_coords;
	Position pos = xy;
	state_coords.afloat = 1;

	state_coords.pos = pos;

	b->coord[0] = state_coords;

	// Adiciona todas as coordenadas precisas
	for(int i = 1; i < size; i++) {
		if(dir == 'H') {
			pos.y += 1;
		} else if(dir == 'V') {
			pos.x += 1;
		}
		state_coords.pos = pos;
		b->coord[i] = state_coords;
	}

	return;
}


/**
 * Function: check_free
 * 
 * Verifica se estão livres todas as posições que serão ocupadas por um 
 * barco no tabuleiro.
 * 
 * n: dimensão x do tabuleiro
 * m: dimensão y do tabuleiro
 * boat: barco a colocar no tabuleiro
 * board: estado atual do tabuleiro
 * 
 * returns:
 *  1 se todas as posições estão livres
 *  0 caso contrário
 * 
 **/
int check_free(int n, int m, Boat *boat, char board[n][m])
{
	/*
	if(
		(boat->coord[0].pos.x < 0 || boat->coord[0].pos.y < 0)
		|| 
		(boat->coord[boat->tSize - 1].pos.x > N || boat->coord[boat->tSize - 1].pos.y > M)) {
		return 0;
	}
	*/

	// Verificar se não há sobrepostos
	int x = 0;
	int y = 0;
	// Verifica em todas as posições do boat se existe alguma coisa dentro do board
	for(int i = 0; i < boat->tSize; i++) {
		x = boat->coord[i].pos.x;
		y = boat->coord[i].pos.y;

		// Se for diferente de ' ' quer dizer que existe algum barco
		if(board[x][y] != ' ') {
			return 0;
		}
	}

   return 1;
}

void place_on_board(Board *board, Boat *boat) {
	for(int i = 0; i < boat->tSize; i++) {
		board->board[boat->coord[i].pos.x][boat->coord[i].pos.y] = boat->type;
	}
	return;
}

/** 
 * Function: place_boat
 * 
 * Cria e coloca, se tal for possível, um barco no tabuleiro.
 * 
 * x1: coordenada x inicial do barco  
 * y1: coordenada y inicial do barco  
 * dir: dirrecção do barco ('H' ou 'V')
 * type: o tipo de barco 
 * board: estado atual do tabuleiro
 *
 * returns: 
 * 0 se o barco for colocado com sucesso.
 * -1 se alguma das posições já está ocupada.
 * -2 se as coordenadas forem inválidas.
 * -3 se a direcção for inválida.
 * -4 se o tipo de barco for inválido.
 **/
int place_boat(int x1, int y1, int dir, char type, Board *board)
{
	if(dir != 'H' && dir != 'V') {
		return -3;
	}

	int size = typeToSize(type);
	if (size == -1) {
		return -4;
	}

	if(x1 < 0 || y1 < 0) {
		return -2;
	}
	if(dir == 'V') {
		if (x1 + size > M) {
			return -2;
		}
	} else if (dir == 'H') {
		if (y1 + size > N) {
			return -2;
		}
	}

	Boat new_boat;
	Position pos;
	pos.x = x1;
	pos.y = y1;
	init_boat(&new_boat, type, pos, dir);

	int free = check_free(N, M, &new_boat, board->board);
	if(!free) {
		return -1;
	}

	place_on_board(board, &new_boat);

	board->boats[board->numBoats] = new_boat;
	board->numBoats += 1;
	board->numBoatsAfloat += 1;

    return 0;
}

/**
 * Function: check_sink
 * 
 * Verifica se ao atacar a posição (x,y) algum barco é afundado. 
 * 
 * x: coordenada x a atacar
 * y: coordenada y a atacar
 * board: estado atual do tabuleiro
 * 
 * returns:
 *   'F' se não for afundado nenhum barco.
 *   Se afundar um barco devolve a letra que representa o seu tipo: 'P', 'N', 'C' ou 'S' 
 *   'I' se a coordenada for inválida.
 * 
 **/
char check_sink(int x, int y, Board *board)
{
    //Implementar
	if(x < 0 || y < 0) {
		return 'I';
	}
	if(x > M - 1 || y > N - 1) {
		return 'I';
	}

	// Verificar se o barco afundou todo!
	for(int i = 0; i < board->numBoats; i++ ) {
		// Verifica se já afundou
		if(board->boats[i].afloat == 0) {
			continue;
		}

		// Verifica quantas partes faltam afundar! 
		if(board->boats[i].afloat > 1) {
			continue;
		}

		// Verifica todas as posiçõese do parco e vê se acerta em alguma
		// Case acerte, vai afundar o barco todo e retorna o seu tipo
		for(int j = 0; j < board->boats[i].tSize; j++) {
			if((board->boats[i].coord[j].pos.x == x && board->boats[i].coord[j].pos.y == y) && board->boats[i].coord[j].afloat == 1) {
				return board->boats[i].type;
			}
		}
	}

	return 'F';
}

/*
* change_to_A
* Apos afundar o barco 
* Vamos converter todas as suas coordenadas no tabuleiro para A
*/
int change_to_A(Board *board, int i) {
	
	board->numBoatsAfloat -= 1;
	board->boats[i].afloat -= 1;

	for(int j = 0; j < board->boats[i].tSize; j++) {
		if(board->boats[i].coord[j].afloat == 1) {
			board->boats[i].coord[j].afloat = 0;
		}

		board->board[board->boats[i].coord[j].pos.x][board->boats[i].coord[j].pos.y] = 'A';
	}

	return board->boats[i].tSize;
}

void index_boat_coord(int x, int y, Board *brd, int *index_boat, int *index_coord) {
	for(int i = 0; i < brd->numBoats; i++) {
		for(int j = 0; j < brd->boats[i].tSize; j++) {
			if(brd->boats[i].coord[j].pos.x == x && brd->boats[i].coord[j].pos.y == y) {
				// Retornar o index do barco 
				*index_coord = j;
				*index_boat = i;
				return;
			}
		}
	}

	return;
}

/**
 * Function: target
 * 
 * Ataca a coordenada (x,y) no tabuleiro.
 * Se a coordenada (x,y) é parte de um barco, então coloca um '*' nessa 
 * posição '*'; caso contrário coloca a letra 'F' nessa posição; caso afunde 
 * um barco coloca em todas as posições desse barco a letra 'A'.
 * 
 * x: coordenada x a atacar
 * y: coordenada y a atacar
 * board: estado atual do tabuleiro
 * 
 * returns:
 *   0 se a posicao já foi atacada anteriormente
 *   1 se acerta numa coordenada de um barco mas sem o afundar
 *   -1 se nao tiver sucesso
 *   -2 se a coordenada for inválida. 
 *   caso afunde um barco, devolve o número correspondente ao tamanho do barco 
 *   que afundou (valor de 2 a 5)
 * 
 **/
int target(int x, int y, Board *board)
{
	// Verifica se as coordenadas são validas
	// Verificar se foi atacada anteriormente 
	// Verificar se falhou - Colocar letra F
	// Se acertou:
	// Substituir por *
	// Se afundou - Substituir por A

	char sink = check_sink(x, y, board);
	/// Coordenada invalida
	if(sink == 'I') {
		return -2;
	}

	// Já foi atacada anteriormente
	// Se for * ou A ou F
	// retorna 0
	int status = check_status(board->board[x][y]);
	if (status == 1) {
		return 0;
	}

	// Verifica se falhou!
	if(board->board[x][y] == ' ') {
		board->board[x][y] = 'F';
		return -1;
	}

	// Acertou mas não afundou
	if(sink == 'F') {
		// Verificar se acerta num barco
		// Caso acerte
		if(typeToSize(board->board[x][y]) != -1) {
			board->board[x][y] = '*';
			int index_boat=0, index_coord=0;
			index_boat_coord(x, y, board, &index_boat, &index_coord);	
			board->boats[index_boat].afloat -= 1;
			board->boats[index_boat].coord[index_coord].afloat = 0;
			return 1;
		}
	}

	// Encontra qual barco vai afundar
	// Afunda o e troca todos os * por A
	char type = sink;
	for(int i = 0; i < board->numBoats; i++) {
		// Verifica se já foi afundado
		// Se já:
		// Salta para o proximo barco
		if(board->boats[i].afloat == 0) {
			continue;
		}

		// Verifica se é do tipo que foi afundado
		// Se não for continua (Salta para o proximo)
		if(board->boats[i].type != type) {
			continue;
		}

		// Valida todos os barcos
		for(int j = 0; j < board->boats[i].tSize; j++) {
			// Se esta posição
			if(board->boats[i].coord[j].pos.x == x && board->boats[i].coord[j].pos.y == y) {
				// Retornar o tamanho do barco		
				return change_to_A(board, i);
			}
		}
	}

	// 
	return -1;
}

void init_game(Game *game) {
	game->num_players = 2;
	game->score_boats = 0;
	game->turn = 1;
	game->running = 1;
	game->showed = 0;
	game->win_lose = 0;
	game->first = 1;
	game->attacker_flag = 0;
}

// Inicializa um player
// Recebe o nome pelo stdin
void init_player(int i, Game *game) {
	printf("Jogador %d\nIntroduza o seu nome: ", i+1);
	Player p;
	fgets(p.name, 25, stdin);
	printf("\n");
	p.score = 0;

	game->players[i] = p;
	return;
}

/**
* get_non_attacker
* Esta funcao retorna o index do array do utilizador que não está a atacar
*/
int get_non_attacker(int turn) {
	return turn == 1 ? 0 : 1;
}

/**
* change_turn
* Altera o turno do jogador para o outro
* Do jogador 1 para o 2
* E do jogador 2 para o 1
*/
void change_turn(Game *game) {
	if(game->turn == 1) {
		game->turn = 0;
		return;
	}
	game->turn  = 1;
}


/*
* Spawna um boat por vez com o seu tipo
*/
int each_boat(Game *game, char type, int i) {
	while(getchar() != '\n');
	char *name;
	name = typeToName(type);
	int size = typeToSize(type);
	Position xy;
	char dir;

	printf("\t\033[34m%d de %d barcos colocados\033[0m\n", i + 1, B);
	printf("\t\033[34m%s - Ocupa %d espaços:\033[0m\n", name, size);

	printf("Direção [H - Horizontal, V - Vertical]: ");
	scanf("%c", &dir);

	printf("\n");

	printf("Posição inicial (x|y -> Exemplo: 3|3 ): ");
	scanf("%d|%d", &xy.x, &xy.y);

	printf("\n");

	init_boat(&game->board.boats[i], type, xy, dir);

	/*
	 * 0 se o barco for colocado com sucesso.
	 * -1 se alguma das posições já está ocupada.
	 * -2 se as coordenadas forem inválidas.
	 * -3 se a direcção for inválida.
	 * -4 se o tipo de barco for inválido.
	*/
	int place_err = place_boat(xy.x, xy.y, dir, type, &game->board);
	if(place_err == -1) {
		printf("Alguma posição já foi ocupada. Vamos repetir!\n\n");
		return -1;
	}

	if(place_err == -2) {
		printf("Coordenada invalida. Vamos repetir!\n\n");
		return -1;
	}

	if(place_err == -3) {
		printf("Direção invalida. Vamos repetir!\n\n");
		return -1;
	}

	if(place_err == -4) {
		printf("Tipo de barco invalido. Vamos repetir!\n\n");
		return -1;
	}

	print_board(N, M, game->board.board, 1);
	return 0;
}

/*
* Introduz todos os barcos de todos os tipos existentes
*/
void spawn_boats(Game *game) {
	int pa = PA, nt = NT, ct = CT, sm = SM;
	int i = 0;
	int res = 0;
	while(pa > 0 || nt > 0 || ct > 0 || sm > 0) {
		
		if(pa > 0) {
			res = each_boat(game, 'P', i);	
			if(res == -1) {
				continue;
			}
			pa--;
			i++;
		}

		if(nt > 0) {
			res = each_boat(game, 'N', i);	
			if(res == -1) {
				continue;
			}
			nt--;
			i++;
		}

		if(ct > 0) {
			res = each_boat(game, 'C', i);	
			if(res == -1) {
				continue;
			}
			ct--;
			i++;
		}

		if(sm > 0) {
			res = each_boat(game, 'S', i);	
			if(res == -1) {
				continue;
			}
			sm--;
			i++;
		}
	}
}

/*
* Display winnir
*/
void display_winner(Game *game) {
	printf("\n\n\n\nO jogador ");
	printf("%s ", game->players[game->turn].name);
	if(game->win_lose == 1) {
		printf("venceu!\n");
		return;
	}
	printf("perdeu!\n");
}

/* 
* Attack 
* Ataca os barcos 
* Guarda o numero de vezes que tentou atacar
* Verifica se ganhou o jogo 
*/
void attack(Game *game) {
	printf("Carregue enter!\n");
	for(int i = 0; i < MOVES; i++) {
		print_board(N, M, game->board.board, game->attacker_flag);
		while(getchar() != '\n');
		printf("1 - Atacar\n");
		printf("2 - Mostrar todos os barcos (Ativa/Desativa) (Perde se escolher esta opção)\n");
		int option = 0;
		printf("Opção: ");
		scanf("%d", &option);
		switch(option) {
			case 1:
				break;
			case 2:
				/* 
				* Opcao para mostrar/esconder todos os barcos
				* (Atacante)
				*/ 
				if(game->attacker_flag == 0) {
					game->attacker_flag = 1;
					printf("Todos os barcos escondidos são agora visiveis!\n");
				} else {
					game->attacker_flag = 0;
					printf("Os barcos estão escondidos!\n");
				}
				i--;
				continue;
			default:
				continue;
		}
		Position xy;
		printf("Posição inicial (x|y -> Exemplo: 3|3 ): ");
		scanf("%d|%d", &xy.x, &xy.y);

		/*
		*   0 se a posicao já foi atacada anteriormente
		*   1 se acerta numa coordenada de um barco mas sem o afundar
		*   -1 se nao tiver sucesso
		*   -2 se a coordenada for inválida. 
		*   caso afunde um barco, devolve o número correspondente ao tamanho do barco 
		*   que afundou (valor de 2 a 5)
		*/
		int target_err = target(xy.x, xy.y, &game->board);

		switch(target_err) {
			case 0:
				printf("Já foi atacado anteriormente! Vamos repetir!\n\n");
				i--;
				continue;
				break;
			case -2:
				printf("Coordenada invalida. Vamos repetir!\n\n");
				i--;
				continue;
				break;
			default: 
			break;
		}
		// Verifica se o jogo já acabou
		if(game->board.numBoatsAfloat == 0) {
			if(game->showed == 0) {
				game->win_lose = 1;
			}
			break;
		}
	}

	// Mostra o vencedor na consola
	display_winner(game);
}

void help_page(Game *game) {
	printf("Ajuda!\n");
	// Inicio da partida!
	printf("Vamos proceder á colocação dos barcos.\n");
	printf("\033[31m\n\tRecomendados que o %s não veja onde são colocados os barcos\033[0m\n\n", game->players[game->turn].name);

	printf("\nFunciona da seguinte forma:\n    Vai aparecer na tela o tipo de barco e o numero de espaços que ocupa.\n    Dera inserir os seguintes valores:\n\t- Direção do barco\n\t- Coordenadas do ponto de inicio barco\n");
	printf("\n\n\t\033[31m IMPORTANTE \033[0m\n\n");
	printf("\033[34mTodas as coordenadas do tabuleiro começam em (0, 0) e acabam em (%d, %d) \033[0m\n", N - 1, M - 1);
}

void game_loop(Game *game) {
	int option = 1;
	while(game->running == 1) {
		// Fim da primeira partida

		if(game->first == 1) {
			printf("1 - Jogar\n");	
		} else {
			printf("1 - Jogar novamente\n");
		}
		printf("2 - Pagina de ajuda\n");
		printf("3 - Sair do jogo :(\n");
		printf("Opcao: ");
		scanf("%d", &option);
		switch(option) {
			case 1:
				break;
			case 2:
				help_page(game);
				continue;
			case 3:
				game->running = 0;
				break;
			default:
				break;
		}

		printf("\n\033[34mEste é o tabuleiro:\033[0m\n");
		print_board(N, M, game->board.board, 1);

		// Cria todos os barcos 
		// Verifica todo e repete se for necessario
		spawn_boats(game);
		for(int sn = 0; sn < 100; sn++) {
			printf("\n");
		}

		printf("Vamos começar a atacar\n");

		attack(game);
	}	
}

int main() 
{
	/*
	Board brd;
    init_board(N, M, &brd);
    print_board(N, M, brd.board, 0);
    
	int a = place_boat(3, 3, 'H', 'P', &brd);
	printf("Place error: %d\n", a);
    print_board(N, M, brd.board, 0);

	printf("%c", brd.boats[0].type);

	target(3, 4, &brd);
    print_board(N, M, brd.board, 0);
	target(3, 5, &brd);
    print_board(N, M, brd.board, 0);
	target(3, 6, &brd);
    print_board(N, M, brd.board, 0);
	target(3, 3, &brd);
    print_board(N, M, brd.board, 0);

	a = target(3, 7, &brd);
	printf("Place error: %d\n", a);
    print_board(N, M, brd.board, 0);
	*/

	printf("\033[34m BATALHA NAVAL EM C \033[0m\n");

	printf("Jogo simples e intuitivo\n");

	Game game;
	// Inicializa o jogo
	init_game(&game);

	// Inicializar a board
	init_board(N, M, &game.board);

	for(int i = 0; i < game.num_players; i++) {
		// Recebe o nome dos utilizadores
		// e começa o contador de score
		init_player(i, &game);
	}

	// Game Loop
	game_loop(&game);

	// Messagem de despedida!
	printf("Esperamos que tenha gostado da experiencia e até á proxima!\n\n");
	return 0;
}
