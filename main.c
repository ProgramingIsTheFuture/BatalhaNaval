/**
 * @autores:
 * Nome: Francisco Santos                   Número de Aluno: 47711
 * Nome: Tiago Borges                       Número de Aluno: 47855
 **/

#include <stdio.h>

#define B 6
#define N 8
#define M 8

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
		if(board->boats[i].afloat == 0) {
			continue;
		}

		if(board->boats[i].afloat > 1) {
			continue;
		}

		for(int j = 0; j < board->boats[i].tSize; j++) {
			if((board->boats[i].coord[j].pos.x == x && board->boats[i].coord[j].pos.y == y) && board->boats[i].coord[j].afloat == 1) {
				return board->boats[i].type;
			}
		}
	}

	return 'F';
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
	int sink = check_sink(x, y, board);
	if(sink == 'I') {
		return -2;
	}

	// Se for * ou A ou F
	// retorna 0
	int status = check_status(board->board[x][y]);
	if (status == 1) {
		return 0;
	}

	if(board->board[x][y] == ' ') {
		board->board[x][y] = 'F';
		return -1;
	}

	int i = 0;
	for(int i = 0; i < board->numBoats; i++) {
		if(board->boats[i].afloat == 0) {
			continue;
		}

		for(int j = 0; j < board->numBoats; j++) {
			if(board->boats[i].coord[j].pos.x == x && board->boats[i].coord[j].pos.y == y) {
				
			}
		}
	}

	if(sink == 'F') {
		// Verificar se acerta num barco
		// Caso acerte
		if(typeToSize(board->board[x][y]) != -1) {
			board->board[x][y] = '*';
		}
		return 1;
	}

	// O barco afundou temos que retornar o tamanho
	// Verificar qual é o barco e trocar na board as posições do barco por A


	// Retornar o tamanho do barco
	return 00;
}

void bbb(Boat *b) {
	b->tSize = 10;
}

//int colocaNavio()
int main(void)
{

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
	target(3, 7, &brd);
    print_board(N, M, brd.board, 0);

	brd.boats[0].afloat = 1;
    print_board(N, M, brd.board, 0);
	for(int i = 1; i < brd.boats[0].tSize; i++) {
		brd.boats[0].coord[i].afloat = 0;
	}

	char b = check_sink(3, 3, &brd);
	printf("CHAR: %c\n", b);
    print_board(N, M, brd.board, 0);

    return 0;
}
