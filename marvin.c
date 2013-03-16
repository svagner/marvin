#include "marvin.h" 
		
void init_curses()
{
	initscr();
	start_color();
	init_pair(1,COLOR_WHITE,COLOR_BLUE);
	init_pair(2,COLOR_BLUE,COLOR_WHITE);
	init_pair(3,COLOR_RED,COLOR_WHITE);
	curs_set(0);
	noecho();
	keypad(stdscr,TRUE);
}

int
init_modules() {
	return 0;
};

int main()
{
	int res;
#ifdef SIGWINCH
	signal(SIGWINCH, sig_winch);
#endif
	init_curses();
	if((res=init_screen())<0)
		return 1;
	if((res=init_modules())<0)
		return 1;
	return 0;
}
