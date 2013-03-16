#include "marvin.h"

#ifdef SIGWINCH 
void
sig_winch(int sig ATTR_UNUSED) {
    want_resize = 1;
    do_resize();

}

/* Handle resizeing xterm */
void 
do_resize () {
	iface_resize ();
	touchwin(stdscr);
	refresh();
}
#endif

void draw_menubar(WINDOW *menubar, int menu_cnt)
{
	int counter;

	wbkgd(menubar,COLOR_PAIR(2));

	for(counter=0; counter<menu_cnt; counter++)
	{
	    wmove(menubar,0,AllMenu[counter]->draw);
	    waddstr(menubar,AllMenu[counter]->title);
	    wattron(menubar,COLOR_PAIR(3));
	    HOTKEY_PRINT(AllMenu[counter]->headerkey);
	    wattroff(menubar,COLOR_PAIR(3));
	};
}

void draw_debugbar(WINDOW *debugbar)
{
	wbkgd(debugbar,COLOR_PAIR(2));
}

WINDOW **draw_menu(int start_col, Menu *Menus)
{
	int i;
	WINDOW **items;
	items=(WINDOW **)malloc(9*sizeof(WINDOW *));

	items[0]=newwin(Menus->subcnt+2,19,1,start_col);
	wbkgd(items[0],COLOR_PAIR(2));
	box(items[0],ACS_VLINE,ACS_HLINE);
	for(i=1; i<=(Menus->subcnt)+1; i++)
	    items[i]=subwin(items[0],1,17,1+i,start_col+1);
	for (i=2;i<(Menus->subcnt)+2;i++)
		wprintw(items[i-1],Menus->sub[i-2].title,i);
	wbkgd(items[1],COLOR_PAIR(1));
	wrefresh(items[0]);
	return items;
}

void delete_menu(WINDOW **items,int count)
{
	int i;
	for (i=0;i<count;i++)
		delwin(items[i]);
	free(items);
}

int scroll_menu(WINDOW **items,int count, int current_menu)
{
	int key;
	int selected=0;
	while (1) {
		key=getch();
		if (key==KEY_DOWN || key==KEY_UP) {
			wbkgd(items[selected+1],COLOR_PAIR(2));
			wnoutrefresh(items[selected+1]);
			if (key==KEY_DOWN) {
				selected=(selected+1) % count;
			} else {
				selected=(selected+count-1) % count;
			}
			wbkgd(items[selected+1],COLOR_PAIR(1));
			wnoutrefresh(items[selected+1]);
			doupdate();
		} else if (key==KEY_LEFT && current_menu>0) {
			    delete_menu(items,count+1);
			    touchwin(stdscr);
			    refresh();
			    items=draw_menu(AllMenu[current_menu-1]->draw, AllMenu[current_menu-1]);
			    return scroll_menu(items,AllMenu[current_menu-1]->subcnt, current_menu-1);
		} else if (key==KEY_RIGHT && current_menu<cnt_menu-1) {
			delete_menu(items,count+1);
			touchwin(stdscr);
			refresh();
			items=draw_menu(AllMenu[current_menu+1]->draw, AllMenu[current_menu+1]);
			return scroll_menu(items,AllMenu[current_menu+1]->subcnt, current_menu+1);
		} else if (key==ESCAPE) {
			return -1;
		} else if (key==ENTER) {
			return selected;
		}
	}
}

int
init_screen () {
	int key;
	int res;
	int counter, current_menu;

	cnt_menu = sizeof(AllMenu)/sizeof(*AllMenu);
	bkgd(COLOR_PAIR(1));
	menubar=subwin(stdscr,1,0,0,0);
	messagebar=subwin(stdscr,1,79,23,1);
	draw_menubar(menubar, cnt_menu);

	debugbar=subwin(stdscr,1,0,LINES-1,0);
	draw_debugbar(debugbar);
	move(1,2);
	printw("Welcome to Marvin System Management!");
	move(2,2);
	printw("Press F[1-12] to open the menus or ESC for quit.");
	printw("\nCount menu: %d\nAddress: Amenu %s: 0x%x vs 0x%x.\nCols=%d Lines=%d", cnt_menu, AllMenu[0]->title, AllMenu[0], &AMenu, COLS, LINES);
	refresh();

	do {
		int selected_item;
		WINDOW **menu_items;

		key=getch();
		werase(debugbar);
		wrefresh(debugbar);
		for(counter=0; counter<cnt_menu; counter++)
		{
		    if (key==AllMenu[counter]->funckey || key==BUTTON_ALT) {
			current_menu=AllMenu[counter]->pos;    
			menu_items=draw_menu(AllMenu[counter]->draw, AllMenu[counter]);
			selected_item=scroll_menu(menu_items,AllMenu[counter]->subcnt, current_menu);
			delete_menu(menu_items,9);
			if (selected_item<0)
				wprintw(debugbar,"You haven't selected any item.");
			else
			{
				wprintw(debugbar,
						"You have selected menu item %d - %s.",selected_item+1,AllMenu[counter]->sub[selected_item].title);
				if (AllMenu[counter]->sub[selected_item].defaultAction)
				    (*AllMenu[counter]->sub[selected_item].defaultAction)();
			}
			touchwin(stdscr);
			refresh();
		    };
		};
	} while (key!=KEY_Q);

	delwin(menubar);
	delwin(messagebar);
	endwin();
	return 0;
}

void 
check_term_size ()
{
if (COLS < 79 || LINES < 27)
	exit(1);
//	FATAL("The terminal is too small after resizeing.");
}

void
iface_resize()
{
	check_term_size();
	endwin ();
	delwin(debugbar);
	delwin(messagebar);
	bkgd(COLOR_PAIR(1));
	werase(menubar);
	wrefresh(menubar);
	menubar=subwin(stdscr,1,0,0,0);
	messagebar=subwin(stdscr,1,79,23,1);
	draw_menubar(menubar, cnt_menu);
	werase(messagebar);
	wrefresh(messagebar);
	werase(debugbar);
	wrefresh(debugbar);
	debugbar=subwin(stdscr,1,0,LINES-1,0);
	draw_debugbar(debugbar);

	touchwin(stdscr);
	refresh ();
}
