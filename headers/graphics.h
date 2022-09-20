extern int wx,wy,game_ready;

void drawWinBorders(){
    // Draws windows borders
    move(0,0);
    vline(ACS_BOARD,wy-1);
    hline(ACS_BOARD,wx);
    move(wy-2,0);
    hline(ACS_BOARD,wx);
    move(0,1);
    vline(ACS_BOARD,wy-1);
    move(0,wx-2);
    vline(ACS_BOARD,wy-1);
    move(0,wx-1);
    vline(ACS_BOARD,wy-1);
}

void infoBarMenu(){
    // Displays the commands available to the player at the bottom of the main menu
    attron(COLOR_PAIR(BLACK));
    mvprintw(wy-1,0,"DEL");
    mvprintw(wy-1,21,"❮");
    mvprintw(wy-1,23,"❯");
    mvprintw(wy-1,40,"SPACE");
    attroff(COLOR_PAIR(BLACK));
    attron(COLOR_PAIR(WHITE));
    mvprintw(wy-1,4,"Exit Forcefully");
    mvprintw(wy-1,22,"/");
    mvprintw(wy-1,25,"Change Option");
    mvprintw(wy-1,46,"Select Option");
    mvprintw(wy-1,61,"         ");
    attroff(COLOR_PAIR(WHITE));
}

void infoBarGame(){
    // Displays the commands available to the player at the bottom of the game screen
    attron(COLOR_PAIR(BLACK));
    mvprintw(wy-1,0,"DEL");
    mvprintw(wy-1,21,"❮");
    mvprintw(wy-1,23,"❯");
    mvprintw(wy-1,37,"SPACE");
    mvprintw(wy-1,57,"U");
    mvprintw(wy-1,59,"I");
    mvprintw(wy-1,72,"R");
    mvprintw(wy-1,81,"S");
    attroff(COLOR_PAIR(BLACK));
    attron(COLOR_PAIR(WHITE));
    mvprintw(wy-1,4,"Exit Forcefully");
    mvprintw(wy-1,22,"/");
    mvprintw(wy-1,25,"Select Peg");
    mvprintw(wy-1,43,"Pick Up Disc");
    mvprintw(wy-1,58,"/");
    mvprintw(wy-1,61,"Undo/Redo");
    mvprintw(wy-1,74,"Reset");
    mvprintw(wy-1,83,"Save");
    attroff(COLOR_PAIR(WHITE));
}

void infoBarCount(){
    attron(COLOR_PAIR(BLACK));
    mvprintw(wy-1,21,"￪");
    mvprintw(wy-1,23,"￬");
    mvprintw(wy-1,61,"/");
    attroff(COLOR_PAIR(BLACK));
    attron(COLOR_PAIR(WHITE));
    mvprintw(wy-1,22,"/");
    mvprintw(wy-1,25,"Change Count ");
    mvprintw(wy-1,63,"Go Back");
    attroff(COLOR_PAIR(WHITE));
}


void infoBarWin(){
	move(wy-1,21);
	hline(' ',wx-1);
	attron(COLOR_PAIR(BLACK));	
    	mvprintw(wy-1,21,"R");
	attroff(COLOR_PAIR(BLACK));
	attron(COLOR_PAIR(WHITE));
    	mvprintw(wy-1,23,"Reset");
	attroff(COLOR_PAIR(WHITE));
}

void drawGame(node** peg, int disc, int turns, int opt){

    int off_x = wx-disc_count*2-1;                          // offset from the right side of the screen

    attron(A_BOLD|COLOR_PAIR(YELLOW));
    mvprintw(10,(wx-8)/2,"Turn %-3d",turns);                // Show turns at the top of the screen
    attroff(A_BOLD|COLOR_PAIR(YELLOW));

    attron(COLOR_PAIR(WHITE));
    for(int i=0;i<3;i++){
        int offset = off_x*(i+1)/4;
        for(int j=0;j<=disc_count;j++){                     // Clean up the playing field
            mvprintw(wy*2/3-disc_count-1+j,offset,"                         ");
        }
        mvprintw(wy*2/3-disc_count-3,off_x*(i+1)/4,"                         ");
        mvprintw(wy*3/4,offset+disc_count-4,  "         ");
        mvprintw(wy*3/4+1,offset+disc_count-4,"         ");
        mvprintw(wy*3/4+2,offset+disc_count-4,"         ");
    }
                                                            // Visual representation of a cursor
    mvprintw(wy*3/4,off_x*(opt+1)/4+disc_count-4,  "    █    ");
    mvprintw(wy*3/4+1,off_x*(opt+1)/4+disc_count-4,"  ██ ██  ");
    mvprintw(wy*3/4+2,off_x*(opt+1)/4+disc_count-4,"██     ██");
    attroff(COLOR_PAIR(WHITE));

    // Draw the disc that is to be moved above the pegs
    if(disc!=0){
        attron(A_STANDOUT|COLOR_PAIR((disc-1)%6+2));
        move(wy*2/3-disc_count-3,off_x*(opt+1)/4+disc_count-disc);
        hline(ACS_BOARD,disc*2+1);
        attroff(A_STANDOUT|COLOR_PAIR((disc-1)%6+2));
    }

    for(int i=0;i<3;i++){
        //Drawing the pegs themselves
        attron(A_BOLD|COLOR_PAIR(WHITE));
        move(wy*2/3,off_x*(i+1)/4);
        hline(ACS_CKBOARD,disc_count*2+1);
        move(wy*2/3-disc_count-1,off_x*(i+1)/4+disc_count);
        vline(ACS_BOARD,disc_count+1);
        attroff(A_BOLD|COLOR_PAIR(WHITE));

        //Counting how many disks are on each peg
        int j=1;
        node* temp = peg[i];
        int peg_count = 0;
        while(temp != NULL){
            peg_count++;
            temp = temp->next;
        }
        temp = peg[i];

        //Drawing all the disks in their corresponding order and positions
        while(temp != NULL){
            attron(A_STANDOUT|COLOR_PAIR((temp->num-1)%6+2));
            move(wy*2/3+j-peg_count-1,off_x*(i+1)/4+disc_count-temp->num);
            hline(ACS_BOARD,temp->num*2+1);
            attroff(A_STANDOUT|COLOR_PAIR((temp->num-1)%6+2));
            j++;
            temp = temp->next;
        }
    }
    refresh();
}
