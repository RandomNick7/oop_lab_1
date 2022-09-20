#define BLACK       1
#define RED         2
#define YELLOW      3
#define GREEN       4
#define CYAN        5
#define BLUE        6
#define MAGENTA     7
#define WHITE       8

void setupColors(){
    // Defining the color palette for the game
    // Can be changed to any wacky combinations
    init_color(COLOR_BLACK,0,0,0);
    init_color(COLOR_RED,1000,0,0);
    init_color(COLOR_YELLOW,1000,1000,0);
    init_color(COLOR_GREEN,0,1000,0);
    init_color(COLOR_CYAN,0,1000,1000);
    init_color(COLOR_BLUE,0,0,1000);
    init_color(COLOR_MAGENTA,1000,0,1000);
    init_color(COLOR_WHITE,1000,1000,1000);

    init_pair(BLACK,COLOR_BLACK,COLOR_WHITE);
    init_pair(RED,COLOR_RED,COLOR_BLACK);
    init_pair(YELLOW,COLOR_YELLOW,COLOR_BLACK);
    init_pair(GREEN,COLOR_GREEN,COLOR_BLACK);
    init_pair(CYAN,COLOR_CYAN,COLOR_BLACK);
    init_pair(BLUE,COLOR_BLUE,COLOR_BLACK);
    init_pair(MAGENTA,COLOR_MAGENTA,COLOR_BLACK);
    init_pair(WHITE,COLOR_WHITE,COLOR_BLACK);
}
