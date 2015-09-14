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

	// player variables
	double player_y = 10.0;
	double player_dy = 0;

	// initialize wall array
	int wallnum = COLS/20;
	wall* walls = malloc(wallnum*sizeof(wall));
	for(int i=0; i < wallnum; i++)
	{
		walls[i] = (wall){20*i, (rand()%LINES) + 1};
	}

	

	int wallcounter = 0;
	while(true)
	{
		//clear previous player position
		attron(COLOR_PAIR(2));
		mvaddch(round(player_y), COLS/4, ' ');
		attroff(COLOR_PAIR(2));

		//clear previous wall positions and move position
		for(int i=0; i < wallnum; i++)
		{
			attron(COLOR_PAIR(2));
			for(int r=0; r < LINES; r++)
			{
				mvaddch(r, walls[i].xpos, ' ');
			}
			attroff(COLOR_PAIR(2));

			walls[i].xpos--;
		}

		// if space has been pressed
		if(getch() == ' ')
		{
			player_dy = -1;
		}

		player_y += player_dy;
		player_dy += gravity;

		wallcounter++;
		if(wallcounter == 90)
		{
			for (int i = 0; i < wallnum; i++)
			{
				walls[i].xpos--;
			}
			wallcounter = 0;
		}

		//draw new wall positions
		for(int i=0; i < wallnum; i++)
		{
			attron(COLOR_PAIR(1));
			for(int r=0; r < LINES; r++)
			{
				if(abs(walls[i].holepos - r) > 3)
				{
					mvaddch(r, walls[i].xpos, ' ');
				}
			}
			attroff(COLOR_PAIR(1));
		}
		
		

		//draw new player position
		attron(COLOR_PAIR(1));
		mvaddch(round(player_y), COLS/4, ' ');
		attroff(COLOR_PAIR(1));

		refresh();
		getch();
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
