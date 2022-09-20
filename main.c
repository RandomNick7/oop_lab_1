#include <locale.h>
#include <curses.h>
#include <string.h>
#include <stdlib.h>

// Defined the following variables as global as they will be used by several functions
int disc_count = 5;
int game_ready = FALSE;
int game_start = FALSE;
int save_valid = FALSE;
int winner = FALSE;
int wx = 0;
int wy = 0;

typedef struct node{
	int num;
	struct node* next;
}node;

void push(int* disc, node** peg){
	node* ptr = (node*)malloc(sizeof(node));
	ptr->num = *disc;
	ptr->next = *peg;
	*peg = ptr;
	*disc = 0;
}

void pop(int* disc, node** peg){
	node* ptr;
	if(*peg != NULL){
		*disc = (*peg)->num;
		ptr = *peg;
		*peg = (*peg)->next;
		free(ptr);
	}
}

/* Bonus points for implementing linked lists within the game?
⣿⢧⣿⣿⣿⣿⣿⣿⠇⣊⠳⣆⢻⣿⣿⣿⣿⣿⣿⣿⣿⢸⣷⡙⢿⡌⠽⣷⡄⢸
⣿⢪⣿⣿⣿⣿⣿⠏⣰⣿⣷⡑⡘⣿⣿⣿⣿⣿⣿⣿⣿⢈⣙⣭⣮⠻⡄⢹⣿⣆
⡿⣩⣿⣿⣿⡿⠋⣔⣛⣩⣴⣶⡁⢻⣿⣿⣿⣿⣿⣿⡏⠼⢛⣿⣿⣷⡜⢠⢿⣿
⡗⣭⣿⣿⡟⢁⣼⣿⣟⣋⣒⢹⣷⠘⣿⣿⣿⣿⣿⡿⢡⣾⣿⣿⡿⠿⢿⡄⢺⣿
⡇⣳⢻⢏⢠⣾⣿⣿⣿⣿⣿⣿⣿⡆⣿⣿⣿⣿⡟⡀⠜⠒⠀⠁⠀⡀⣸⠄⢸⣿
⡇⡱⠋⠴⠿⠿⠟⠉⠐⠒⠒⢈⣿⣷⢸⣿⡿⢋⡜⣀⣤⠀⠀⢸⠀⣧⣿⢈⠐⣿
⡇⣶⡀⣄⣤⠀⠀⠀⢰⡄⢰⣴⣿⡿⠸⣋⢶⢋⣼⣿⣿⡀⠀⡈⢰⣿⣿⠀⠆⣿
⡇⢿⣷⣼⣿⡆⠀⠀⡘⠃⢸⣿⣿⡗⡼⢉⣴⣿⣿⣿⣿⣷⣤⣴⣿⣿⢿⡌⠂⣿
⣟⢸⣿⣿⣽⣿⣤⣴⣔⣲⣾⣿⣿⣶⣾⣿⣿⣿⣿⣿⣿⣿⣻⣟⣿⣾⣿⠃⠄⣿
⣯⡄⣿⣯⣿⣟⣿⣻⡿⣟⣿⣿⣿⡛⠛⠛⠋⠁⠉⢡⣿⣿⣿⣿⣯⣿⠞⠀⡂⣿
⣧⢇⠈⢿⣻⣿⣿⣿⣿⣿⣿⣿⣿⠁⡖⣩⢣⠓⣄⢸⣿⣿⣿⣿⠟⠁⠤⠁⢄⣿
⣿⣚⠆⠀⠈⠛⠿⠠⢶⣬⣙⢿⣿⣆⠱⣅⢣⡛⠔⣾⠟⣩⣴⣶⠆⡘⠰⢈⢸⣿
⣿⣏⢾⣹⡀⠀⠀⠀⠀⠈⣿⣿⣧⣔⣀⣤⣶⣿⣿⠟⣩⠄⠀⠀⢀⠰⡈⢰⣿⣿
⣏⣟⡳⣲⢝⡦⡀⠀⠀⢸⣿⣿⣿⣿⣿⣿⣿⠟⣡⣾⡿⠀⠀⠀⠈⠀⣠⣿⣿⣿
*/

#include "colors.h"
#include "graphics.h"
#include "save.h"
#include "menus.h"

void checkWin(node** peg){
	int i=0;
	node* temp = peg[1];
	while(temp!=NULL){
		i++;
		temp = temp->next;
	}
	if(i==0){
		temp = peg[2];
		while(temp!=NULL){
			i++;
			temp = temp->next;
		}
	}
	if(i==disc_count){
		attron(COLOR_PAIR(GREEN));
		mvprintw(4,(wx-39)/2,"__      ___      _                   _ ");
		mvprintw(5,(wx-39)/2,"\\ \\    / (_)    | |                 | |");
		mvprintw(6,(wx-39)/2," \\ \\  / / _  ___| |_ ___  _ __ _   _| |");
		mvprintw(7,(wx-39)/2,"  \\ \\/ / | |/ __| __/ _ \\| '__| | | | |");
		mvprintw(8,(wx-39)/2,"   \\  /  | | (__| || (_) | |  | |_| |_|");
		mvprintw(9,(wx-39)/2,"    \\/   |_|\\___|\\__\\___/|_|   \\__, (_)");
		mvprintw(10,(wx-39)/2,"                                __/ | ");
		mvprintw(11,(wx-39)/2,"                               |___/  ");
		drawWinBorders();
		attroff(COLOR_PAIR(GREEN));
		winner = TRUE;
		refresh();
	}
}

void startGame(){
	int disc = 0, turns = 0, max_turns = 0, opt = 0, last_opt = 0;

	node** peg = (node**)malloc(sizeof(node*)*3);			// Initializing pegs...
	char* s;

	for(int i=0;i<3;i++){
		peg[i]=NULL;
	}

	if(save_valid == FALSE){
		for(int i=disc_count;i>0;i--){
			disc = i;
			push(&disc,&(peg[0]));
		}
	}else{
		s = loadGame(peg, &turns, &max_turns);
	}

	int** turn_seq = (int**)malloc(sizeof(int*)*(max_turns+1));	// Array of moves in the format [[from,to], [from,to], ...]
	for(int i=0;i<=max_turns;i++){
		turn_seq[i] = (int*)malloc(sizeof(int)*2);
		if(save_valid == TRUE){
			turn_seq[i][0] = s[i*2]-'0';
			turn_seq[i][1] = s[i*2+1]-'0';
		}
	}
	infoBarGame();

	//Handle keyboard input
	while(TRUE){
		while(winner == FALSE){
			drawGame(peg, disc, turns, opt);
			checkWin(peg);
			if(winner == TRUE){break;};
			int ch = getch();
			if(ch == KEY_DC){
				endGame(0,"");
			}else if(ch == KEY_RIGHT && opt<2){
				opt++;
			}else if(ch == KEY_LEFT && opt>0){
				opt--;
			}else if(ch == ' '){
				if(disc == 0){									// If no disc picked up, pick up one
					if(peg[opt]!=NULL){
						pop(&disc,&peg[opt]);
						last_opt = opt;
					}
				}else{											//If you have a disc, place it down somewhere
					if(peg[opt] == NULL || peg[opt]->num>disc){
						if(last_opt!=opt){
							turn_seq[turns][0] = last_opt;
							turn_seq[turns][1] = opt;
							turns++;
							if(turns > max_turns){
								turn_seq = (int**)realloc(turn_seq, sizeof(int*)*(turns+1));
								turn_seq[turns] = (int*)malloc(sizeof(int)*2);
								max_turns++;
							}
						}
						push(&disc,&peg[opt]);
					}
				}
			}else if(ch == 'u' && turns>0){						// "You can't undo moves you haven't done yet, dumbass!"
				if(disc != 0){
					push(&disc,&peg[last_opt]);					// If you're holding a disc, move it where it was
				}
				turns--;
				pop(&disc,&peg[turn_seq[turns][1]]);			// Do the previous registered move backwards
				push(&disc,&peg[turn_seq[turns][0]]);
			}else if(ch == 'i' && turns < max_turns){			// "You can't redo moves you never done, dumbass!"
				if(disc != 0){
					push(&disc,&peg[last_opt]);					// If you're holding a disc, move it where it was
				}
				if(peg[turn_seq[turns][0]] != NULL && peg[turn_seq[turns][1]]!=NULL){
					if(peg[turn_seq[turns][0]]->num < peg[turn_seq[turns][1]]->num){	//If the move is still valid...
						pop(&disc,&peg[turn_seq[turns][0]]);		// Do the current move you're at in order
						push(&disc,&peg[turn_seq[turns][1]]);
						turns++;
					}
				}else{											// The move will be valid by default if either peg is empty
						pop(&disc,&peg[turn_seq[turns][0]]);
						push(&disc,&peg[turn_seq[turns][1]]);
						turns++;
				}
			}else if(ch == 'r'){
				resetGame(peg, turn_seq, &turns, &max_turns);
			}else if(ch == 's'){
				saveGame(peg, turn_seq, &turns, &max_turns);
			}
		}
		infoBarWin();
		while(winner == TRUE){
			int ch = getch();
			if(ch == KEY_DC){
				endGame(0,"");
			}else if(ch == 'r'){
				resetGame(peg, turn_seq, &turns, &max_turns);
				for(int i=4;i<=11;i++){
					mvprintw(i,(wx-39)/2,"                                       ");
				}
				attron(COLOR_PAIR(YELLOW));
				drawWinBorders();
				attroff(COLOR_PAIR(YELLOW));
				mvprintw(wy-1,20,"        ");
				infoBarGame();
				winner = FALSE;
			}
		}
	}
}

int main(){
	// Setup stuff
	setlocale(LC_ALL, "");
	initscr();								// Curse the Terminal
	noecho();								// Turn off char echoing
	raw();									// Turn off line buffering
	keypad(stdscr, TRUE);					// Enable reading function & arrow keys
	curs_set(0);

	if(has_colors()==FALSE){				// Check if colors are supported in the first place
		endGame(1,"Your terminal does not support color\n");
	}
	start_color();							// Enable terminal colors

	if(can_change_color()==FALSE){			// Check if colors can be changed too
		endGame(1,"Your terminal does not support changing colors\n");
	}
	setupColors();							// Set up our 8-color palette

	getmaxyx(stdscr,wy,wx);					// Get current screen size

	if(wx<140 || wy<40){					// Check if Terminal is big enough to fit the game
		endGame(1,"Screen size too small, resize the window to at least 140x40\n");
	}

	attron(COLOR_PAIR(CYAN));
	drawWinBorders();						// Draw the borders around the screen
	attroff(COLOR_PAIR(CYAN));

	while(game_start == FALSE){
		infoBarMenu();						// Write controls at the bottom of the screen
		showMainMenu();						// Draw the main menu, manage the game from then on
		if(save_valid == FALSE){
			countMenuPrompt();				// If the player starts a new game, ask them for the number of discs
		}
	}

	erase();								// Wipe the screen clean

	attron(COLOR_PAIR(YELLOW));
	drawWinBorders();						// Draw the borders around the screen
	attroff(COLOR_PAIR(YELLOW));

	startGame();							// Shows the game screen

	delwin(stdscr);							// Pour Holy Water over the Terminal
	endwin();								// Lift the nCurses
	return 0;
}
