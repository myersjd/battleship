/*
File: battleshipP3.c
Language: C
Name: Joshua Myers
Date: 11/12/2019
Course: ICSI 333 - Programming at the Hardware-Software Interface
Semester: Fall 2019
Lab Section: Wed 12:35PM
UAlbany ID: 001407117
Desc: Program runs command-line battleship using functionality from
	  the battleship.h header file. User can play in singleplayer
	  or multiplayer. Log will be created or overwritten in local
	  directory with name "log.txt"
Compiled using GNU GCC 7.4.0
*/



#include "battleship.h"

int main(int argc, char **argv)
{
	
	//Seed rand function
	srand(time(0));
	
	//Welcome message
	clearScreen();
	displayWelcome();
	
	char response;
	
	int mode; //Mode: 0 - Singleplayer
			  //	  1 - Local
			  //      2 - Host Online
			  //      3 - Join Online

	int prompt = 1;
	
	//Select welcome options
	//1: Single Player
	//2: Multiplayer
	//3: Instructions
	//4: Exit
	do{
		
		response = getchar();
		clearInputBuffer();
		
		switch(response){
				
			case '1':
				//Single Player
				prompt = 0;
				mode = 0;
				break;
			case '2':
				//Local Multiplayer
				prompt = 0;
				mode = 1;
				break;
			case '3':
				//Multiplayer
				prompt = 0;
				mode = 2;
				break;
			case '4':
				//Multiplayer
				prompt = 0;
				mode = 3;
				break;
			case '5':
				//Instructions
				clearScreen();
				displayInstructions();
				printf("Press ENTER to continue...");
				clearInputBuffer();
				clearScreen();
				displayWelcome();
				break;
			case '6':
				//Exit
				return 0;
				break;
			default:
				printf("Incorrect entry.\n");
		}
		
	}while(prompt);
	
	//If not online
	if(mode < 2){
		//Generate 2 boards for each player.
		//Ship board and guide board.
		Board* playerOneBoard = generateBoard();
		Board* playerOneGuide = generateBoard();
		Board* playerTwoBoard = generateBoard();
		Board* playerTwoGuide = generateBoard();
		
		//Should user place ships?
		clearScreen();
		printCenter("Would you like to choose your battleship locations? (y/n)", 57);
		prompt = 1;
		do{
			
			response = getchar();
			clearInputBuffer();
			
			switch(response){
					
				case 'y':
				case 'Y':
					clearScreen();
					printCenter("Player One Board", 20);
					battleshipSpawn(playerOneBoard);
					clearScreen();
					
					
					if(mode != 0){
						//If not singleplayer
						printCenter("Player Two Board", 20);
						battleshipSpawn(playerTwoBoard);
						clearScreen();
					}
					prompt = 0;
					break;
				case 'n':
				case 'N':
					battleshipSpawnRandom(playerOneBoard);
					battleshipSpawnRandom(playerTwoBoard);
					prompt = 0;
					clearScreen();
					break;
					
			}

		}while(prompt);
		
		printCenter("Player 1 will go first...", 30);
		printf("Press ENTER to continue...");
		clearInputBuffer();
		
		//Player log
		PlayerMoveList* log;
			
		//Run game in proper mode
		if(mode == 0)
			log = runSingleplayer(playerOneBoard, playerOneGuide, playerTwoBoard, playerTwoGuide);
		else
			log = runMultiplayer(playerOneBoard, playerOneGuide, playerTwoBoard, playerTwoGuide);
		
		if(!saveLog(log)){
			fprintf(stderr, "Failed to save log!\n");
		}
		
		//Free memory
		cleanup(playerOneBoard);
		cleanup(playerOneGuide);
		cleanup(playerTwoBoard);
		cleanup(playerTwoGuide);
		freeLog(log);
	}
	else if(mode == 2){
		//Online Host
		int sfd;
		//Host server and get file descriptor
		if((sfd = hostServer()) == -1){
			printf("Could not host server! Aborting...\n");
			exit(0);
		}
		//Create host thread
		pthread_t thread_id; 
		pthread_create(&thread_id, NULL, runServer, &sfd); 
		
		//Join server
		int joinsfd;
		if((joinsfd = joinServer("localhost")) == -1){
			printf("Server failure! Aborting...\n");
			exit(0);
		}
		
		clearScreen();
		printCenter("Waiting for another player...", 30);
		runOnline(joinsfd);
				
	}
	else if(mode == 3){
		//Online Join
		char buffer[50];
		int sfd;
		clearScreen();
		printCenter("Enter address to connect...", 30);
		//loop for input while prompting user to connect
		for(;;){
			//enter server address
			fgets(buffer, sizeof(buffer), stdin);
			//fgets adds newline so removing it
			buffer[strcspn(buffer, "\n")] = 0;
			if((sfd = joinServer(buffer)) != -1)
				break;
			else{
				clearScreen();
				printCenter("Enter address to connect...", 30);
				printf("Could not connect to server...\nEnter again: ");
			}
			
		}
		//Connected
		printf("Connected...\n");
		runOnline(sfd);
		// struct PlayerMove* move;
		// if(receiveMove(sfd, &move)){
			// printf("%s, %d, %s, %d, %d, %d, %d\n", move->moveString, move->player.shipsAlive, move->player.playerName, move->displayName, move->hit, move->coord.x, move->coord.y);
		// }
		// else
			// printf("failure");
	}
		
	
    return 0;
}