extern int disc_count, game_ready, game_start, save_valid, wx, wy;

void endGame(int exit_status, char* exit_msg){
	// Function for quitting the program
	delwin(stdscr);
	endwin();
	printf("%s",exit_msg);
	exit(exit_status);
}

void menuInput(int* opt){
    // Process keys pressed while in the menu
    int ch = getch();
    if(ch == KEY_DC){                                                   // Quit the game when pressing Del
        endGame(0,"");
    }else if(ch == KEY_RIGHT && *opt<2){
        (*opt)++;
    }else if(ch == KEY_LEFT && *opt>0){
        (*opt)--;
    }else if(ch == ' '){                                                // Spacebar selects the current option
        if(*opt == 0){
            mvprintw(wy*2/3+2,(wx-44)/2,"                                            "); //Cleanup after failed load
            game_ready = TRUE;                                          // Exit the infinite loop, move on to the next function in main()
        }else if(*opt == 1){
            // Picked a specific name for the savefile, I don't wanna deal with OS-specific ways for checking a directory
            FILE* save = fopen("ToH_save.txt", "r");
            if(save == NULL){
                attron(COLOR_PAIR(RED));
                mvprintw(wy*2/3+2,(wx-44)/2,"\"ToH_save.txt\" does not exist in the folder!");
                attroff(COLOR_PAIR(RED));
            }else{
                int bad_save = checkSave(save);                         // Check if Savefile is valid
                if(bad_save == 0){
                    save_valid = TRUE;                                  // Load game with the data
                    game_ready = TRUE;
                    game_start = TRUE;
                }else{
                    mvprintw(5,5,"%d",bad_save);
                    attron(COLOR_PAIR(RED));
                    mvprintw(wy*2/3+2,(wx-25)/2,"Liar, liar, file on fire!");
                    remove("ToH_save.txt");                             // Invalid savefiles get deleted
                    attroff(COLOR_PAIR(RED));
                }
                fclose(save);
            }
        }else{
            endGame(0,"");
        }
    }
}

void showMainMenu(){
    // Main title
    attron(A_BOLD);
    mvprintw((wy-5)/4,(wx-72)/2,  "  _____                                   __   _   _                   _ ");
    mvprintw((wy-5)/4+1,(wx-72)/2," |_   _|____      _____ _ __ ___    ___  / _| | | | | __ _ _ __   ___ (_)");
    mvprintw((wy-5)/4+2,(wx-72)/2,"   | |/ _ \\ \\ /\\ / / _ \\ '__/ __|  / _ \\| |_  | |_| |/ _` | '_ \\ / _ \\| |");
    mvprintw((wy-5)/4+3,(wx-72)/2,"   | | (_) \\ V  V /  __/ |  \\__ \\ | (_) |  _| |  _  | (_| | | | | (_) | |");
    mvprintw((wy-5)/4+4,(wx-72)/2,"   |_|\\___/ \\_/\\_/ \\___|_|  |___/  \\___/|_|   |_| |_|\\__,_|_| |_|\\___/|_|");
    attroff(A_BOLD);

    // Decorations to make the screen feel less empty
    // Unfortunately, ncurses' printw doesn not take multiple predefined strings as arguments

    // Pillar design by llizard/ejm
    // https://ascii.co.uk/art/column

    int o=3;                            // offset from borders
    attron(COLOR_PAIR(YELLOW));
    mvprintw(1,o,"%s","(@^,^,^,^,^,@)"); mvprintw(1,wx-14-o,"%s","(@,^,^,^,^,^@)");
    mvprintw(2,o,"%s","  )`){o}(`(   "); mvprintw(2,wx-14-o,"%s","  )`){o}(`(   ");
    mvprintw(3,o,"%s","  ,`,`,`,`,`  "); mvprintw(3,wx-14-o,"%s","  `,`,`,`,`,  ");
    mvprintw(4,o,"%s","  ==========  "); mvprintw(4,wx-14-o,"%s","  ==========  ");
    for(int i=5;i<wy-7;i++){
        mvprintw(i,o,"%s","   ||||||||   ");
        mvprintw(i,wx-14-o,"%s","   ||||||||   ");
    };
    mvprintw(wy-7,o,"%s","  ,________, "); mvprintw(wy-7,wx-14-o,"%s", "  ,________,  ");
    mvprintw(wy-6,o,"%s","    )    (   "); mvprintw(wy-6,wx-14-o,"%s", "    )    (    ");
    mvprintw(wy-5,o,"%s","  ,       `  "); mvprintw(wy-5,wx-14-o,"%s", "   '      .   ");
    mvprintw(wy-4,o,"%s"," /_________\\ "); mvprintw(wy-4,wx-14-o,"%s"," /_________\\ ");
    mvprintw(wy-3,o,"%s","|___________|");  mvprintw(wy-3,wx-14-o,"%s","|___________|");

    int btn_choice = 0;
    while(game_ready == FALSE){
        // Menu buttons
        attron(A_DIM|COLOR_PAIR(GREEN));
        if(btn_choice == 0){attroff(A_DIM);}
        mvprintw((wy-3)*3/5-2,(wx-19)/4,"%s",  "▗▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▖");
        if(btn_choice == 0){
            mvprintw((wy-3)*3/5-1,(wx-19)/4,"%s","▐ >>START  GAME<< ▌");
        }else{
            mvprintw((wy-3)*3/5-1,(wx-19)/4,"%s","▐   START  GAME   ▌");
        }
        mvprintw((wy-3)*3/5,(wx-19)/4,"%s","▝▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▘");
        if(btn_choice == 0){attron(A_DIM);}
        attroff(COLOR_PAIR(GREEN));

        attron(COLOR_PAIR(YELLOW));
        if(btn_choice == 1){attroff(A_DIM);}
        mvprintw((wy-3)*3/5-2,(wx-19)/2,"%s",  "▗▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▖");
        if(btn_choice == 1){
            mvprintw((wy-3)*3/5-1,(wx-19)/2,"%s","▐>>LOAD TXT SAVE<<▌");
        }else{
            mvprintw((wy-3)*3/5-1,(wx-19)/2,"%s","▐  LOAD TXT SAVE  ▌");
        }
        mvprintw((wy-3)*3/5,(wx-19)/2,"%s","▝▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▘");
        if(btn_choice == 1){attron(A_DIM);}
        attroff(COLOR_PAIR(YELLOW));

        attron(COLOR_PAIR(RED));
        if(btn_choice == 2){attroff(A_DIM);}
        mvprintw((wy-3)*3/5-2,(wx-19)*3/4,"%s",  "▗▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▖");
        if(btn_choice == 2){
            mvprintw((wy-3)*3/5-1,(wx-19)*3/4,"%s","▐ >> QUIT GAME << ▌");
        }else{
            mvprintw((wy-3)*3/5-1,(wx-19)*3/4,"%s","▐    QUIT GAME    ▌");
        }
        mvprintw((wy-3)*3/5,(wx-19)*3/4,"%s","▝▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▘");
        attron(COLOR_PAIR(RED));
        attroff(A_DIM);
        refresh();

        menuInput(&btn_choice);         // Processing input
    }
}

void countMenuPrompt(){
    // Adds additional instructions at the bottom of the screen
    infoBarCount();

    while(game_start == FALSE){
        // Draws a prompt in the menu asking for number of discs
        attron(A_BOLD|COLOR_PAIR(CYAN));
        mvprintw(wy*3/5+3,wx/2-9,"Number of disks: %-2d",disc_count);
        attroff(A_BOLD|COLOR_PAIR(CYAN));

        // Handles keyboard input
        int ch = getch();
        if(ch == KEY_DC){
            endGame(0,"");
        }else if(ch == KEY_UP && disc_count<12){
            disc_count++;
        }else if(ch == KEY_DOWN && disc_count>3){
            disc_count--;
        }else if(ch == '/'){
            game_ready = FALSE;
            attron(COLOR_PAIR(WHITE));
            mvprintw(wy*3/5+3,wx/2-9,"                   ");
            attroff(COLOR_PAIR(WHITE));
            break;
        }else if(ch == ' '){
            game_start = TRUE;
        };
        refresh();
    }
}

