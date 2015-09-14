#include <stdlib.h>
#include <ncurses.h>
#include <time.h>
#include <string.h>
#include <math.h>

typedef struct
{
	int xpos;
	int holepos;
}wall;

void init_ncurses();

int main()
{
	double gravity = 0.1;

	init_ncurses();

	double player_y = 10.0;
	double player_dy = 0;

	while(true)
	{
		//clear previous player position
		attron(COLOR_PAIR(2));
		mvaddch(round(player_y), COLS/2, ' ');
		attroff(COLOR_PAIR(2));

		// if space has been pressed
		if(getch() == ' ')
		{
			player_dy = -1;
		}

		player_y += player_dy;
		player_dy += gravity;


		//draw new player position
		attron(COLOR_PAIR(1));
		mvaddch(round(player_y), COLS/2, ' ');
		attroff(COLOR_PAIR(1));

		refresh();
		usleep(60*1000);
	}

}

void init_ncurses()
{
	initscr();
	cbreak();
	noecho();
	curs_set(0);
	nodelay(stdscr, TRUE);

	start_color();
	use_default_colors();
	init_pair(1, COLOR_BLACK, COLOR_WHITE);
	init_pair(2, COLOR_WHITE, COLOR_BLACK);
}
