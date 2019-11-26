/*
File: battleship.h
Language: C
Name: Joshua Myers
Date: 11/25/2019
Course: ICSI 333 - Programming at the Hardware-Software Interface
Semester: Fall 2019
Lab Section: Wed 12:35PM
UAlbany ID: 001407117
Desc: Contains function prototype, structures, and enumerated
	  types, and libraries for the battleship game.
	  or multiplayer.
Compiled using GNU GCC 7.4.0
*/


#ifndef BATTLESHIP_H
#define BATTLESHIP_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <stdint.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <pthread.h> 
#include <sys/ioctl.h> //For ioctl


// Macros

// Board Information
#define BOARD_WIDTH 10
#define BOARD_HEIGHT 10
#define SHIP_COUNT 5

//Ship lengths
#define CARRIER_HP 5
#define BATTLESHIP_HP 4
#define CRUISER_HP 3
#define SUBMARINE_HP 3
#define DESTROYER_HP 2

//Port
#define PORT "5974"

#define member_size(type, member) sizeof(((type *)0)->member)

//Log File Path
#define LOG_PATH "log.txt"

//Debug Mode
#define DEBUG 0

// Enumerated Types

//Cardinal Directions
/*typedef enum{
	NORTH,
	EAST,
	SOUTH,
	WEST
} Cardinal;*/

//Ships Types
typedef enum{
	CARRIER, //Size 5
	BATTLESHIP, //Size 4
	CRUISER, //Size 3
	SUBMARINE, //Size 3
	DESTROYER //Size 2
} ShipTypes;

// Structs

//Holds 2D vector data
typedef struct{
	int32_t x;
	int32_t y;
} Vector2D;

//Ship information. Contains symbol for cell, hp of ship (initially
//the length of the ship and then decremented to check when ship
//sinks), and name of the ship.
typedef struct{
	char symbol;
	short hp;
	char* name;
} Ship;

//Each individual cell of battleship board contains a symbol
//and a pointer to the ship contained. NULL if no ship.
typedef struct{
	char symbol;
	Ship* ship;
} Cell;

//Simple structure to hold number of ships left.
//Intended to be able to be expanded upon
//with player name and other features if
//wanted.
typedef struct{
	uint32_t shipsAlive;
	char playerName[50];
} Player;

//Linked list for player moves
typedef struct PlayerMove{
	char moveString[50];
	Player player;
	uint32_t displayName;
	uint32_t hit;
	Vector2D coord;
	struct PlayerMove* next;
}PlayerMoveList;

//Board of battleship. Contains array of cells of
//size BOARD_WIDTH*BOARD_HEIGHT and SHIP_COUNT number
//of ships.
typedef struct{
	Cell* cells;
	Ship* ships;
} Board;


// Function Prototypes
void clearScreen();

Board* generateBoard();

int getIndex(int x, int y);

void clearScreen();

void displayWelcome();

void displayInstructions();

void printBoard(Board* board);

void printCenter(char* str, int size);

void cleanup(Board* board);

void battleshipSpawn(Board* board);

void battleshipSpawnRandom(Board* board);

int placeShip(Board* board, ShipTypes type, int row, int column, char direction);

Ship createShip(ShipTypes type);

void shuffle(char *arr, int size);

void battleshipSpawnText();

void playerTurn(Board* board, Board* guide, Player* player, char* message, Vector2D* coord, int* hit);

Vector2D getRandomCoord();

void cpuTurn(Board* board, Board* guide, Player* player, char* message, Vector2D* coord, int* hit);

PlayerMoveList* runSingleplayer(Board* playerOneBoard, Board* playerOneGuide, Board* playerTwoBoard, Board* playerTwoGuide);

PlayerMoveList* runMultiplayer(Board* playerOneBoard, Board* playerOneGuide, Board* playerTwoBoard, Board* playerTwoGuide);

void appendLog(PlayerMoveList** head, PlayerMoveList** tail, char* message, Player* player, Vector2D* coord, int* hit);

void freeLog(PlayerMoveList* head);

int saveLog(PlayerMoveList* head);

int joinServer(char* address);

int hostServer();

void* runServer();

void runOnline(int sfd);

int sendMove(int sfd, struct PlayerMove* playerMove);

int receiveMove(int sfd, struct PlayerMove** playerMove);

void handleMove(struct PlayerMove* move, Board* board, Player* player);

/*
 * Inline Function:  clearInputBuffer 
 * --------------------
 * Clears stdin buffer. Requires at least one character
 * already in buffer.
 *
 * PARAMS
 *  none
 *
 * RETURNS: none
 */
static inline void clearInputBuffer(){ while((getchar()) != '\n'); }

#endif