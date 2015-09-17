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
	int score = 0;
	double gravity = 0.15;
	bool lost = false;

	init_ncurses();

	// player variables
	int player_x = COLS/4;
	double player_y = LINES/2;
	double player_dy = 0;

	// initialize wall array
	int wallnum = (COLS/20)+1;
	wall* walls = malloc(wallnum*sizeof(wall));
	for(int i=0; i < wallnum; i++)
	{
		walls[i] = (wall){(COLS/4)+(20*i)+50, (rand()%(LINES-12)) + 6};
	}

	// draw screen black
	bkgd(COLOR_PAIR(2));

	

	int wallcounter = 0;
	while(!lost)
	{
		//clear previous player position
		attron(COLOR_PAIR(2));
		mvaddch(round(player_y), player_x, ' ');
		attroff(COLOR_PAIR(2));

		//clear previous wall positions
		for(int i=0; i < wallnum; i++)
		{
			attron(COLOR_PAIR(2));
			for(int r=0; r < LINES; r++)
			{
				mvaddch(r, walls[i].xpos-1, ' ');
				mvaddch(r, walls[i].xpos, ' ');
				mvaddch(r, walls[i].xpos+1, ' ');
			}
			attroff(COLOR_PAIR(2));

		}

		// jump if space has been pressed
		if(getch() == ' ')
		{
			player_dy = -1;
		}

		// handle player gravity
		player_y += player_dy;
		player_dy += gravity;

		// move walls
		wallcounter++;
		if(wallcounter == 2)
		{
			for (int i = 0; i < wallnum; i++)
			{
				walls[i].xpos--;
			}
			wallcounter = 0;
		}

		// remove offscreen walls and shift array
		if(walls[0].xpos < -1)
		{
			// shift walls left in array
			for(int i = 1; i < wallnum; i++)
			{
				walls[i-1].xpos = walls[i].xpos;
				walls[i-1].holepos = walls[i].holepos;
			}

			// add new wall to end of array
			walls[wallnum-1] = (wall){walls[wallnum-2].xpos+20, rand()%(LINES-12)+6};
		}

		// draw new wall positions
		for(int i=0; i < wallnum; i++)
		{
			attron(COLOR_PAIR(3));
			for(int r=0; r < LINES; r++)
			{
				if(abs(walls[i].holepos - r) > 3)
				{
					mvaddch(r, walls[i].xpos-1, ' ');
					mvaddch(r, walls[i].xpos, ' ');
					mvaddch(r, walls[i].xpos+1, ' ');
				}
			}
			attroff(COLOR_PAIR(3));
		}
		
		

		// draw new player position
		attron(COLOR_PAIR(1));
		mvaddch(round(player_y), player_x, ' ');
		attroff(COLOR_PAIR(1));

		// detect collisions
		for(int i = 0; i < wallnum; i++)
		{
			if(abs(walls[i].xpos - player_x) <= 1 && abs(walls[i].holepos - round(player_y)) > 3)
			{
				lost = true;
			}else if(abs(walls[i].xpos - player_x) <= 1){
				score++;
			}
		}

		// detect player offscreen
		if(player_y > LINES+2)
		{
			lost = true;
		}

		// score has 6 points add for every wall so divide to display correctly
		mvprintw(0, 0, "%d", score/6);

		refresh();
		getch();
		usleep(40*1000);
	}
	
	endwin();

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
	init_pair(3, COLOR_GREEN, COLOR_GREEN);
}
