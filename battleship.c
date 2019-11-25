#include "battleship.h"
/*
File: battleship.c
Language: C
Name: Joshua Myers
Date: 11/12/2019
Course: ICSI 333 - Programming at the Hardware-Software Interface
Semester: Fall 2019
Lab Section: Wed 12:35PM
UAlbany ID: 001407117
Desc: Definitions for battleship.h function prototypes.
Compiled using GNU GCC 7.4.0
*/



/*
 * Function:  generateBoard 
 * --------------------
 * Dynamically allocates a board and initializes
 * member variables.
 *
 * PARAMS
 *  none
 *
 * RETURNS: pointer to allocated Board
 */
Board* generateBoard(){
	
	//Allocate memory for board
	Board* board;
	if ((board = (Board*)malloc(sizeof(Board))) == NULL){
		fprintf(stderr, "Failed to allocate memory for board! Aborting...\n");
		exit(1);
	}
	
	//Allocate memory for cells
	Cell* cells = (Cell*)malloc(BOARD_WIDTH*BOARD_HEIGHT*sizeof(Cell));
	
	//Initialze cells
	for(int i = 0; i < BOARD_WIDTH*BOARD_HEIGHT; i++){
		(*(cells+i)).symbol = '~';
		(*(cells+i)).ship = NULL;
	}
	
	//Allocate ships
	Ship* ships = (Ship*)malloc(SHIP_COUNT*sizeof(Ship));
	for(int i = 0; i < SHIP_COUNT; i++)
		ships[i] = createShip(i);
	
	board->cells = cells;
	board->ships = ships;
	
	return board;
}

/*
 * Function:  getIndex 
 * --------------------
 * Converts x, y coordinate of board to 1D
 * 0, 0 at top left growing right down
 *
 * PARAMS
 *  x: column of board
 *  y: row of board
 *
 * RETURNS: integer value for 1D array
 */
int getIndex(int x, int y){
	//Convert 2D index to 1D
	return y*BOARD_WIDTH+x;
}


/*
 * Function:  displayInstructions 
 * --------------------
 * Routine for printing instructions to console.
 *
 * PARAMS
 *  none
 *
 * RETURNS: none
 */
void displayInstructions(){
	printCenter("---Instructions---", 17);
	printCenter("The goal of battleship is to sink opponent's ships.", 60);
	printCenter("Each player has two boards. A board with your own ships", 60);
	printCenter("and a guide board to keep track of attacks. Player 1", 60);
	printCenter("will start first and will select a coordinate to attack.", 60);
	printCenter("If the attack hits the enemy ship, then an 'X' will appear", 60);
	printCenter("on the attacker's guide board and the target's board.", 60);
	printCenter("If the attack misses, then a '-' will appear on the", 60);
	printCenter("attacker's guide board. Each player has 5 ships which span", 60);
	printCenter("a certain distance. When each part of the ship is hit, the", 60);
	printCenter("ship is sunk. The winner is whoever sinks all of the", 60);
	printCenter("other's ships.", 60);
}

/*
 * Function:  displayWelcome 
 * --------------------
 * Routine for printing welcome message to console.
 *
 * PARAMS
 *  none
 *
 * RETURNS: none
 */
void displayWelcome(){
	
	printCenter("Welcome to Battleship!", 30);
	printCenter("Select option from below...", 30);
	printCenter("1. Play Singleplayer", 30);
	printCenter("2. Play Local Multiplayer", 30);
	printCenter("3. Host Online", 30);
	printCenter("4. Join Online", 30);
	printCenter("5. Instructions", 30);
	printCenter("6. Quit", 30);
	
}

/*
 * Function:  printBoard 
 * --------------------
 * Prints a center formated board with along with row and
 * column identifiers. 1-10 for columns and A-J for rows
 *
 * PARAMS
 *  board: board to print to console
 *
 * RETURNS: none
 */
void printBoard(Board* board){
	printCenter("  1 2 3 4 5 6 7 8 9 10", (BOARD_WIDTH+1)*2-1);
	//print each row
	for(int row = 0; row < BOARD_HEIGHT; row++){
		char printStr[(BOARD_WIDTH+1)*2]; //counting column number, \0, and spaces
		printStr[0] = (char)(65+row); //row letter identifier
		printStr[1] = ' ';
		//add cell row characters to printStr adding spaces in between
		for(int i = 2; i < (BOARD_WIDTH+1)*2-1; i++)
			(i%2 == 0) ? printStr[i] = (*(board->cells + row*BOARD_WIDTH + (i-2)/2)).symbol : (printStr[i] = ' ');
		
		//print center
		printStr[(BOARD_WIDTH+1)*2-1] = '\0';
		printCenter(printStr, (BOARD_WIDTH+1)*2-1);
	}
}

/*
 * Function:  printCenter 
 * --------------------
 * Prints str to the center of console.
 *
 * PARAMS
 *  str: string to print
 *  size: size of string (can be larger or smaller to change
 *        center position)
 *
 * RETURNS: pointer to allocated Board
 */
void printCenter(char* str, int size){
	
	//Get console dimensions
	struct winsize dimensions;
	ioctl(STDOUT_FILENO, TIOCGWINSZ, &dimensions);
	
	int pad = (size >= dimensions.ws_col) ? 0 : (dimensions.ws_col - size) / 2; // in case negative pad
	
	printf("%*.*s%s\n", pad, pad, " ", str);
}

/*
 * Function:  cleanup 
 * --------------------
 * Frees memory for board
 *
 * PARAMS
 *  board: board to free.
 *
 * RETURNS: none
 */
void cleanup(Board* board){
	free(board->ships);
	free(board->cells);
	free(board);
}


/*
 * Function:  battleShipSpawnText 
 * --------------------
 * Routine for printing the battleship spawn menu to console.
 *
 * PARAMS
 *  none
 *
 * RETURNS: none
 */
void battleShipSpawnText(){
	printCenter("Below are the 5 ships that can be placed along with their length.", 30);
	printCenter("Enter ship number along with cardinal direction and coordinates.", 30);
	printCenter("Input format is <ship> <row><column> <direction>", 30);
	printCenter("For example: 1 A5 S", 30);
	printCenter("1. CARRIER : 5", 30);
	printCenter("2. BATTLESHIP : 4", 30);
	printCenter("3. CRUISER : 3", 30);
	printCenter("4. SUBMARINE, : 3", 30);
	printCenter("5. DESTROYER : 2", 30);
	printf("\n");
	printCenter("N", 1);
	printCenter("W   E", 5);
	printCenter("S", 1);
	printf("\n");
}

/*
 * Function:  battleshipSpawn 
 * --------------------
 * Runs routine to spawn SHIP_COUNT ships on board.
 *
 * PARAMS
 *  board: pointer to the desired battleship board
 *
 * RETURNS: none
 */

void battleshipSpawn(Board* board){
	
	//Print menu
	battleShipSpawnText();
	printBoard(board);
	
	//Counter for each ship
	int carrier = 0, battleship = 0, cruiser = 0, submarine = 0, destroyer = 0;
	
	//Variables for input
	int ship;
	char row;
	int column;
	char direction;
	
	
	//Loop until each battleship type has been entered.
	while(!carrier || !battleship || !cruiser || !submarine || !destroyer){
		
		//Accepted format: <Ship #> <row> <column> <direction>
		if(scanf("%d %c%d %c", &ship, &row, &column, &direction) == 4){
			int row_convert;
			if(row > 64 && row < 75){
				//Uppercase
				row_convert = row - 65;
			}
			else if(row > 96 && row < 107){
				//Lowercase
				row_convert = row - 97;
			}
			else
				continue;
			int column_convert = column - 1;
			
			if(DEBUG)
				printf("Row: %d Column %d\n", row_convert, column_convert);
			
			//Determine type of ship (+1 since ship type enum starts at 0)
			//and attempt to place
			switch(ship){
				case CARRIER+1:
					if(DEBUG)
						printf("Placing CARRIER...\n");
					if(!carrier){
						carrier = placeShip(board, CARRIER, row_convert, column_convert, direction);
						if(carrier){
							clearScreen();
							battleShipSpawnText();
							printBoard(board);
							printf("Placed CARRIER!\n");
						}
						else{
							printf("Could not place CARRIER!\n");
						}
					}
					else{
						printf("Ship already exists!\n");
					}
					break;
				case BATTLESHIP+1:
					if(DEBUG)
						printf("Placing BATTLESHIP...\n");
					if(!battleship){
						battleship = placeShip(board, BATTLESHIP, row_convert, column_convert, direction);
						if(battleship){
							clearScreen();
							battleShipSpawnText();
							printBoard(board);
							printf("Placed BATTLESHIP!\n");
						}
						else{
							printf("Could not place BATTLESHIP!\n");
						}
					}
					else{
						printf("Ship already exists!\n");
					}
					break;
				case CRUISER+1:
					if(DEBUG)
						printf("Placing CRUISER...\n");
					if(!cruiser){
						cruiser = placeShip(board, CRUISER, row_convert, column_convert, direction);
						if(cruiser){
							clearScreen();
							battleShipSpawnText();
							printBoard(board);
							printf("Placed CRUISER!\n");
						}
						else{
							printf("Could not place CRUISER!\n");
						}
					}
					else{
						printf("Ship already exists!\n");
					}
					break;
				case SUBMARINE+1:
					if(DEBUG)
						printf("Placing SUBMARINE...\n");
					if(!submarine){
						submarine = placeShip(board, SUBMARINE, row_convert, column_convert, direction);
						if(submarine){
							clearScreen();
							battleShipSpawnText();
							printBoard(board);
							printf("Placed SUBMARINE!\n");
						}
						else{
							printf("Could not place SUBMARINE!\n");
						}
					}
					else{
						printf("Ship already exists!\n");
					}
					break;
				case DESTROYER+1:
					if(DEBUG)
						printf("Placing DESTROYER...\n");
					if(!destroyer){
						destroyer = placeShip(board, DESTROYER, row_convert, column_convert, direction);
						if(destroyer){
							clearScreen();
							battleShipSpawnText();
							printBoard(board);
							printf("Placed DESTROYER!\n");
						}
						else{
							printf("Could not place DESTROYER!\n");
						}
					}
					else{
						printf("Ship already exists!\n");
					}
					break;
				default:
					if(DEBUG)
						printf("No ship was placed.\n");
				
			}
				
		}
		else{
			printf("Incorrect entry.\n");
		}
		clearInputBuffer();
	}
	
	printf("Press ENTER to continue...");
	clearInputBuffer();
	
}

/*
 * Function:  shuffle 
 * --------------------
 * Shuffles array
 *
 * PARAMS
 *  arr: array to shuffle
 *  size: size of array
 *
 * RETURNS: none
 */
void shuffle(char *arr, int size){
    if (size > 1){
        for (int i = 0; i < size - 1; i++){
          int j = i + rand() / (RAND_MAX / (size - i) + 1);
          char t = arr[j];
          arr[j] = arr[i];
          arr[i] = t;
        }
    }
}

/*
 * Function:  getRandomCoord 
 * --------------------
 * Returns random coordinate on battleship board.
 * 0 - (BOARD_WIDTH-1), 0 - (BOARD_HEIGHT-1) inclusive
 *
 * PARAMS
 *  none
 *
 * RETURNS: Returns a random Vector2D within range of BOARD_WIDTH and BOARD_HEIGHT
 */
Vector2D getRandomCoord(){
	// returns x y coord (column, row)
	return (Vector2D){rand() % BOARD_WIDTH, rand() % BOARD_HEIGHT};
}


/*
 * Function:  battleshipSpawnRandom 
 * --------------------
 * Randomly spawns SHIP_COUNT battleships on given board
 *
 * PARAMS
 *  board: pointer to Board to spawn battleships
 *
 * RETURNS: none
 */
void battleshipSpawnRandom(Board* board){
	
	int row;
	int column;
	Vector2D coord;
	char directions[] = {'N', 'S', 'E', 'W'};
	
	for(int i = 0; i < SHIP_COUNT; i++){
		
		do{
			coord = getRandomCoord();
			row = coord.y;
			column = coord.x;
			shuffle(directions, 4);
		}while(!placeShip(board, i, row, column, directions[0]));
	}
	
}

/*
 * Function:  placeShip 
 * --------------------
 * Attempts to place a battleship on the board in a given direction.
 * Returns 0 if failed and 1 if successful.
 *
 * PARAMS
 *  board: pointer to Board to place battleship
 *  type: ShipType of battleship type to place
 *  row: desired row to place
 *  column: desired column to place
 *  direction: cardinal direction (N,E,S,W) to place ship from origin column, row
 *
 * RETURNS: 0 if failed, 1 if successful
 */
int placeShip(Board* board, ShipTypes type, int row, int column, char direction){
		
	if(DEBUG)
		printf("Direction: %c\n", direction);
	
	//Ship type out of range
	if(type < 0 || type > SHIP_COUNT-1)
		return 0;
	
	//Get ship from board
	Ship* ship = &(board->ships[type]);
	
	//Get direction step for 1D cell array
	int endRow = row, endColumn = column;
	int direction_convert;
	switch(direction){
		case 'N': direction_convert = -BOARD_WIDTH; endRow -= ship->hp; break;
		case 'n': direction_convert = -BOARD_WIDTH; endRow -= ship->hp; break;
		case 'S': direction_convert = BOARD_WIDTH; endRow += ship->hp; break;
		case 's': direction_convert = BOARD_WIDTH; endRow += ship->hp; break;
		case 'E': direction_convert = 1; endColumn += ship->hp; break;
		case 'e': direction_convert = 1; endColumn += ship->hp; break;
		case 'W': direction_convert = -1; endColumn -= ship->hp; break;
		case 'w': direction_convert = -1; endColumn -= ship->hp; break;
		default: return 0;
	}
	
	int index = getIndex(column, row);
	
	if(DEBUG)
		printf("endRow: %d endColumn: %d, height: %d, width: %d\n", endRow, endColumn, BOARD_HEIGHT, BOARD_WIDTH);
	
	//Check start and end boundaries
	if(row >= 0 && row < BOARD_HEIGHT && column >= 0 && column < BOARD_WIDTH){
		if(endRow >= 0 && endRow < BOARD_HEIGHT && endColumn >= 0 && endColumn < BOARD_WIDTH){
			//First check if any ships are present
			int shipPresent = 0;
			for(int i = 0; i < ship->hp && !shipPresent; i++){
				shipPresent = (board->cells[index + i*direction_convert].ship != NULL);
			}
			if(shipPresent)
				return 0;
			else{
				//No ship present, change cells to point to ship
				for(int i = 0; i < ship->hp; i++){
					board->cells[index + i*direction_convert].symbol = ship->symbol;
					board->cells[index + i*direction_convert].ship = ship;
				}
				return 1;
			}
		}		
	}
	return 0;
}

/*
 * Function:  createShip 
 * --------------------
 * Return an initialize ship from a given type
 *
 * PARAMS
 *  type: ShipType to return appropriate ship
 *
 * RETURNS: Returns Ship value
 */
Ship createShip(ShipTypes type){
	Ship ship;
	switch(type){
		case CARRIER:
			ship.symbol = 'C';
			ship.hp = 5;
			ship.name = "CARRIER";
			return ship;
		case BATTLESHIP:
			ship.symbol = 'B';
			ship.hp = 4;
			ship.name = "BATTLESHIP";
			return ship;
		case CRUISER:
			ship.symbol = 'R';
			ship.hp = 3;
			ship.name = "CRUISER";
			return ship;
		case SUBMARINE:
			ship.symbol = 'S';
			ship.hp = 3;
			ship.name = "SUBMARINE";
			return ship;
		case DESTROYER:
			ship.symbol = 'D';
			ship.hp = 2;
			ship.name = "DESTROYER";
			return ship;;			
	}
	ship.symbol = '?';
	ship.hp = 1;
	ship.name = "UNKNOWN";
	return ship;
}

/*
 * Function:  cpuTurn 
 * --------------------
 * Simulates the turn of enemy AI when in singleplayer mode.
 *
 * PARAMS
 *  board: defender player's board
 *  guide: guide board of attacking player
 *  player: pointer to Player struct of defending player
 *  message: assigns message to AI turn string.
 *  coord: store the attacked coordinate
 *  hit: store 1 if hit, 0 if miss
 *
 * RETURNS: none
 */
void cpuTurn(Board* board, Board* guide, Player* player, char* message, Vector2D* coordinate, int* hit){
	
	Vector2D coord;
	int index;
	//Get random coordinate which hasn't been attacked
	do{
		coord = getRandomCoord();
		index = getIndex(coord.x, coord.y);
	}while(guide->cells[index].symbol != '~');
	if(DEBUG)
		printf("Enemy attack coord: %d %d\nEnemy attack index: %d\n", coord.x, coord.y, index);
	
	*coordinate = coord;
		
	if(board->cells[index].ship != NULL){
		//Hit ship
		*hit = 1;
		board->cells[index].symbol = 'X';
		board->cells[index].ship->hp--;
		if(board->cells[index].ship->hp == 0){
			//ship sunk
			player->shipsAlive--;
		}
		guide->cells[index].symbol = 'X';

		//Assign message string
		(board->cells[index].ship->hp == 0) ? sprintf(message, "Fired at %c%d! %s sunk!\n", (coord.y+65), coord.x+1, board->cells[index].ship->name) 
				: sprintf(message, "Fired at %c%d! %s hit!\n", (coord.y+65), coord.x+1, board->cells[index].ship->name);
	}
	else{
		//Missed
		*hit = 0;
		guide->cells[index].symbol = '-';
		//Assign message string
		sprintf(message, "Fired at %c%d! Missed!\n", (coord.y+65), coord.x+1);
	}

}

/*
 * Function:  playerTurn 
 * --------------------
 * Routine for real player turn.
 *
 * PARAMS
 *  board: defender player's board
 *  guide: guide board of attacking player
 *  player: pointer to Player struct of defending player
 *  coord: store the attacked coordinate
 *  hit: store 1 if hit, 0 if miss
 *
 * RETURNS: none
 */
void playerTurn(Board* board, Board* guide, Player* player, char* message, Vector2D* coord, int* hit){
	
	printf("Enter coordinates to attack (eg. A1): ");
	char row;
	int column;
	//Loop until player successfully attacks
	while(1){
		//Accepts input: <row><column>
		//for attacking enemy
		if(scanf("%c%d", &row, &column) == 2){
			clearInputBuffer();
			
			int row_convert;
			if(row > 64 && row < 75){
				//Uppercase
				row_convert = row - 65;
			}
			else if(row > 96 && row < 107){
				//Lowercase
				row_convert = row - 97;
				//Convert row to uppercase
				row -= 32;
			}
			else{
				printf("Enter valid entry...\n");
				continue;
			}
			int column_convert = column - 1;
			
			//Check boundaries
			if(row_convert >= 0 && row_convert < BOARD_HEIGHT && column_convert >= 0 && column_convert < BOARD_WIDTH){
				int index = getIndex(column_convert, row_convert);
				//Check if didn't hit coordinate before
				if(guide->cells[index].symbol == '~'){
					//Save coord
					(*coord).x = column_convert;
					(*coord).y = row_convert;
					if(board->cells[index].ship != NULL){
						//Hit ship
						*hit = 1;
						board->cells[index].symbol = 'X';
						board->cells[index].ship->hp--;
						if(board->cells[index].ship->hp == 0){
							//ship sunk
							player->shipsAlive--;
						}
						guide->cells[index].symbol = 'X';
						
						(board->cells[index].ship->hp == 0) ? sprintf(message, "Fired at %c%d. %s sunk!\n", row, column, board->cells[index].ship->name) 
								: sprintf(message, "Fired at %c%d. %s hit!\n", row, column, board->cells[index].ship->name);
						return;
					}
					else{
						//Missed
						*hit = 0;
						guide->cells[index].symbol = '-';
						sprintf(message, "Fired at %c%d. Missed!\n", row, column);
						return;
					}
				}
				else{
					printf("Already Hit!\n");
				}
			}
			else{
				printf("Enter valid entry...\n");
			}
			
		}
		else{
			printf("Enter valid entry...\n");
			clearInputBuffer();
		}
	}
}


/*
 * Function:  runSingleplayer 
 * --------------------
 * Routine for running singleplayer mode and returns log
 *
 * PARAMS
 *  playerOneBoard: real player's ship board
 *  playerOneGuide: real player's guide board
 *  playerTwoBoard: AI's ship board
 *  playerTwoGuide: AI's guide board
 *
 * RETURNS: PlayerMoveList*
 */
PlayerMoveList* runSingleplayer(Board* playerOneBoard, Board* playerOneGuide, Board* playerTwoBoard, Board* playerTwoGuide){
	
	//Setup players
	Player playerOne;
	playerOne.shipsAlive = 5;
	strcpy(playerOne.playerName, "Player");
	Player playerTwo;
	playerTwo.shipsAlive = 5;
	strcpy(playerTwo.playerName, "Enemy");
	
	//Turn message
	char message[50] = "Enemy waits patiently...";	
	
	//Log
	PlayerMoveList* logHead = NULL;
	PlayerMoveList* logTail = NULL;
	
	Vector2D coord;
	int hit;
	
	//Loop until someone wins
	while(1){
	
		//Player 1
		clearScreen();
		printf("\n");
		printCenter("Player Turn", 13);
		printf("\n\n");
		printBoard(playerOneBoard);
		printf("\n\n");
		printBoard(playerOneGuide);
		printf("Enemy: %s\n", message);
		
		playerTurn(playerTwoBoard, playerOneGuide, &playerTwo, message, &coord, &hit);
		printf("%s\nPress ENTER to continue...", message);
		clearInputBuffer();
		
		//Add to log
		appendLog(&logHead, &logTail, message, &playerOne, &coord, &hit);
		
		if(playerTwo.shipsAlive == 0){
			//Player 1 wins
			clearScreen();
			printCenter("Player has won!", 17);
			printf("Press ENTER to continue...");
			clearInputBuffer();
			
			//Add to log
			appendLog(&logHead, &logTail, "Player has won!", NULL, NULL, NULL);
			break;
		}
		
		//Player 2
		cpuTurn(playerOneBoard, playerTwoGuide, &playerOne, message, &coord, &hit);
		
		//Add to log
		appendLog(&logHead, &logTail, message, &playerTwo, &coord, &hit);
		
		if(playerOne.shipsAlive == 0){
			//Player 2 wins
			clearScreen();
			printCenter("Enemy has won!", 17);
			printf("Press ENTER to continue...");
			clearInputBuffer();
			
			//Add to log
			appendLog(&logHead, &logTail, "Enemy has won!", NULL, NULL, NULL);
			break;
		}
			
	
	}
	return logHead;
}

/*
 * Function:  runMultiplayer 
 * --------------------
 * Routine for running multiplayer mode and returns log
 *
 * PARAMS
 *  playerOneBoard: player one's ship board
 *  playerOneGuide: player one's guide board
 *  playerTwoBoard: player two's ship board
 *  playerTwoGuide: player two's guide board
 *
 * RETURNS: PlayerMoveList*
 */
PlayerMoveList* runMultiplayer(Board* playerOneBoard, Board* playerOneGuide, Board* playerTwoBoard, Board* playerTwoGuide){
	
	//Setup players
	Player playerOne;
	playerOne.shipsAlive = 5;
	strcpy(playerOne.playerName, "Player 1");
	Player playerTwo;
	playerTwo.shipsAlive = 5;
	strcpy(playerTwo.playerName, "Player 2");
	
	//Message buffer
	char message[50];
	
	//Player move log
	PlayerMoveList* logHead = NULL;
	PlayerMoveList* logTail = NULL;
	
	Vector2D coord;
	int hit;
	
	//Loop until someone wins
	while(1){
	
		//Player 1
		clearScreen();
		printf("\n");
		printCenter("Player 1 Turn", 13);
		printf("\n\n");
		printBoard(playerOneBoard);
		printf("\n\n");
		printBoard(playerOneGuide);
		
		playerTurn(playerTwoBoard, playerOneGuide, &playerTwo, message, &coord, &hit);
		printf("%s\nPress ENTER to continue...", message);
		clearInputBuffer();
		
		//Add to log
		appendLog(&logHead, &logTail, message, &playerOne, &coord, &hit);
		
		if(playerTwo.shipsAlive == 0){
			//Player 1 wins
			clearScreen();
			printCenter("Player 1 has won!", 17);
			printf("Press ENTER to continue...");
			clearInputBuffer(); 
			
			//Add to log
			appendLog(&logHead, &logTail, "Player 1 has won!", NULL, NULL, NULL);
			break;
		}
		
		//Player 2
		clearScreen();
		printf("\n");
		printCenter("Player 2 Turn", 13);
		printf("\n\n");
		printBoard(playerTwoBoard);
		printf("\n\n");
		printBoard(playerTwoGuide);
		
		playerTurn(playerOneBoard, playerTwoGuide, &playerOne, message, &coord, &hit);
		printf("%s\nPress ENTER to continue...", message);
		clearInputBuffer();
		
		//Add to log
		appendLog(&logHead, &logTail, message, &playerTwo, &coord, &hit);
		
		if(playerOne.shipsAlive == 0){
			//Player 2 wins
			clearScreen();
			printCenter("Player 2 has won!", 17);
			printf("Press ENTER to continue...");
			clearInputBuffer();
			
			//Add to log
			appendLog(&logHead, &logTail, "Player 2 has won!", NULL, NULL, NULL);
			break;
		}
			
	
	}
	return logHead;
}

/*
 * Function:  receiveMove 
 * --------------------
 * Reads PlayerMove over connected socket with file descriptor sfd
 *
 * PARAMS
 *  sfd: socket file descriptor
 *  playerMove: result of read
 *
 * RETURNS: 1 if success, 0 if failed.
 */
int receiveMove(int sfd, struct PlayerMove** playerMove){
	
	//Size of player move
	int moveSize = member_size(struct PlayerMove, moveString)
					+ member_size(Player, shipsAlive)
					+ member_size(Player, playerName)
					+ member_size(struct PlayerMove, displayName)
					+ member_size(struct PlayerMove, hit)
					+ member_size(Vector2D, x)
					+ member_size(Vector2D, y);
					
	char packet[moveSize];
	
	//Send
	if(read(sfd, packet, sizeof(packet)) == -1)
		return 0;
	
	//Create new player move
	struct PlayerMove* newMove;
	if ((newMove = (struct PlayerMove*)malloc(sizeof(struct PlayerMove))) == NULL){
		fprintf(stderr, "Failed to allocate memory for log! Aborting...\n");
		return 0;
	}
	
	//READ PACKET///////
	int packetPos = 0;
	//Move string
	memcpy(&(newMove->moveString), packet, member_size(struct PlayerMove, moveString));
	packetPos += member_size(struct PlayerMove, moveString);
	
	//Player
	memcpy(&(newMove->player.shipsAlive), packet+packetPos, sizeof(uint32_t));
	packetPos += sizeof(uint32_t);
	memcpy(&(newMove->player.playerName), packet+packetPos, member_size(Player, playerName));
	packetPos += member_size(Player, playerName);
	
	//DisplayName
	memcpy(&(newMove->displayName), packet+packetPos, sizeof(uint32_t));
	packetPos += sizeof(uint32_t);
	
	//Hit
	memcpy(&(newMove->hit), packet+packetPos, sizeof(uint32_t));
	packetPos += sizeof(uint32_t);
	
	//Vector2D
	memcpy(&(newMove->coord.x), packet+packetPos, sizeof(int32_t));
	packetPos += sizeof(int32_t);
	memcpy(&(newMove->coord.y), packet+packetPos, sizeof(int32_t));
	///////////////////////////////////////////////'
	
	*playerMove = newMove;
	
	return 1;
	
}


/*
 * Function:  sendMove 
 * --------------------
 * Sends PlayerMove over connected socket with file descriptor sfd
 *
 * PARAMS
 *  sfd: socket file descriptor
 *  playerMove: PlayerMove to send
 *
 * RETURNS: 1 if success, 0 if failed.
 */
int sendMove(int sfd, struct PlayerMove* playerMove){
	
	//Size of player move
	int moveSize = member_size(struct PlayerMove, moveString)
					+ member_size(Player, shipsAlive)
					+ member_size(Player, playerName)
					+ member_size(struct PlayerMove, displayName)
					+ member_size(struct PlayerMove, hit)
					+ member_size(Vector2D, x)
					+ member_size(Vector2D, y);
	
	char packet[moveSize];
	
	//FILL PACKET///////
	int packetPos = 0;
	//Move string
	memcpy(packet, playerMove->moveString, member_size(struct PlayerMove, moveString));
	packetPos += member_size(struct PlayerMove, moveString);
	
	//Player
	uint32_t shipsAlive = playerMove->player.shipsAlive;
	memcpy(packet+packetPos, &shipsAlive, sizeof(uint32_t));
	packetPos += sizeof(uint32_t);
	memcpy(packet+packetPos, playerMove->player.playerName, member_size(Player, playerName));
	packetPos += member_size(Player, playerName);
	
	//DisplayName
	uint32_t displayName = playerMove->displayName;
	memcpy(packet+packetPos, &displayName, sizeof(uint32_t));
	packetPos += sizeof(uint32_t);
	
	//Hit
	uint32_t hit = playerMove->hit;
	memcpy(packet+packetPos, &hit, sizeof(uint32_t));
	packetPos += sizeof(uint32_t);
	
	//Vector2D
	int32_t x = playerMove->coord.x;
	memcpy(packet+packetPos, &x, sizeof(int32_t));
	packetPos += sizeof(int32_t);
	int32_t y = playerMove->coord.y;
	memcpy(packet+packetPos, &y, sizeof(int32_t));
	///////////////////////////////////////////////
	
	//Send
	if(write(sfd, packet, sizeof(packet)) == -1)
		return 0;
	return 1;
}


/*
 * Function:  hostServer 
 * --------------------
 * Creates a server for battleship
 *
 * PARAMS
 *  none
 *
 * RETURNS: socket file descriptor of connection. -1 if failed
 */
int hostServer(){
	
	struct addrinfo hints;
	struct addrinfo *result, *r;
	int sfd, s;

	memset(&hints, 0, sizeof(struct addrinfo));

	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_flags = AI_PASSIVE;

	//Get addresses
	s = getaddrinfo(NULL, PORT, &hints, &result);
	if(s != 0){
	   if(DEBUG)
		   fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(s));
	   return -1;
	}

	//Loop results and try to connect
	for (r = result; r != NULL; r = r->ai_next) {
		
		//Create socket
		sfd = socket(r->ai_family, r->ai_socktype, r->ai_protocol);
		if (sfd == -1)
		   continue;

		//Bind address
		if (bind(sfd, r->ai_addr, r->ai_addrlen) == 0){
		   //Binded
		   break;                  
		}

		close(sfd);
	}

	//Done with results
	if(r == NULL){
		freeaddrinfo(result);
		return -1;
	}
	freeaddrinfo(result);
	
	//Set to listen for 2 incoming connections
	 if ((listen(sfd, 2)) != 0) { 
        if(DEBUG)
		   fprintf(stderr, "Listen failed!\n");
        return -1; 
    } 
	return sfd;
}


/*
 * Function:  joinServer 
 * --------------------
 * Establishes connection to a battleship server.
 *
 * PARAMS
 *  address: name of address to connect
 *
 * RETURNS: socket file descriptor of connection. -1 if failed
 */
int joinServer(char* address){
	//Get player name
	char playerName[_SC_LOGIN_NAME_MAX];
	getlogin_r(playerName, _SC_LOGIN_NAME_MAX);


	struct addrinfo hints;
	struct addrinfo *result, *r;
	int sfd, s;

	memset(&hints, 0, sizeof(struct addrinfo));

	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;

	//Get addresses
	s = getaddrinfo("localhost", PORT, &hints, &result);
	if(s != 0){
	   if(DEBUG)
		   fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(s));
	   return -1;
	}

	//Loop results and try to connect
	for (r = result; r != NULL; r = r->ai_next) {
		
		//Create socket
		sfd = socket(r->ai_family, r->ai_socktype, r->ai_protocol);
		if (sfd == -1)
		   continue;

		//Connect to address
		if (connect(sfd, r->ai_addr, r->ai_addrlen) != -1){
		   //Connected
		   break;                  
		}

		close(sfd);
	}

	//Done with results
	if(r == NULL){
		freeaddrinfo(result);
		return -1;
	}
	freeaddrinfo(result);
	return sfd;
}

/*
 * Function:  appendLog 
 * --------------------
 * Adds new PlayerMove from arguments to end of passed PlayerMoveList.
 *
 * PARAMS
 *  head: pointer to head of list
 *  tail: pointer to tail of list
 *  message: message to keep in log
 *  player: player information for log. NULL if not display name
 *  coord: store the attacked coordinate
 *  hit: store 1 if hit, 0 if miss
 *
 * RETURNS: none
 */
void appendLog(PlayerMoveList** head, PlayerMoveList** tail, char* message, Player* player, Vector2D* coord, int* hit){
	//Create new player move
	struct PlayerMove* newMove;
	if ((newMove = (struct PlayerMove*)malloc(sizeof(struct PlayerMove))) == NULL){
		fprintf(stderr, "Failed to allocate memory for log! Aborting...\n");
		exit(1);
	}
	strcpy(newMove->moveString, message);

	if(player == NULL){
		newMove->displayName = 0;
	}
	else{
		newMove->displayName = 1;
		newMove->player = *player;
		newMove->coord = *coord;
		newMove->hit = *hit;
	}
	newMove->next = NULL;
	if(*head == NULL){
		//New list
		*head = newMove;
		*tail = newMove;
	}
	else{
		//Append from tail
		(*tail)->next = newMove;
		(*tail) = newMove;
	}
	
}

/*
 * Function:  freeLog 
 * --------------------
 * Frees memory of PlayerMoveList log.
 *
 * PARAMS
 *  head: pointer to head of list
 *
 * RETURNS: none
 */
void freeLog(PlayerMoveList* head){
	while(head != NULL){
		PlayerMoveList* temp = head;
		head = head->next;
		free(temp);
	}
}

/*
 * Function:  saveLog 
 * --------------------
 * Saves PlayerMoveList log.
 *
 * PARAMS
 *  head: pointer to head of list
 *
 * RETURNS: returns 0 if failed, 1 if success
 */
 int saveLog(PlayerMoveList* head){
	 FILE* file;
	 file = fopen(LOG_PATH, "w");
	 if(file == NULL)
		 return 0;
	 PlayerMoveList* temp = head;
	 while(temp != NULL){
		if(temp->displayName){
			fprintf(file, "%s: %s", temp->player.playerName, temp->moveString);
		}
		else{
			fprintf(file, "%s", temp->moveString);
		}
		
		temp = temp->next;
	 }
	 //Success
	 if(fclose(file) == 0)
		 return 1;
	 //Failed
	 return 0;
 }

/*
 * Function:  clearScreen 
 * --------------------
 * Clears the console.
 *
 * PARAMS
 *  none
 *
 * RETURNS: none
 */
void clearScreen(){
#ifdef WINDOWS
    system("cls");
#else
    system("clear");
#endif
}
