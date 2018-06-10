#include <ncurses.h>

int main()
{
	initscr();
	printw("Hello world");
	refresh();
	getch();
	endwin();

	return 0;
}
