#include "../../../marvin.h"
#include "menu.h"
#include <form.h>

#define ARRAY_SIZE(a) (sizeof(a) / sizeof(a[0]))
#define CTRLD	4

void init_module ();
void openInterface();
void print_in_middle(WINDOW *win, int starty, int startx, int width, char *string, chtype color);
void get_MenuOptions(FORM *, char *str);

struct _module Mod = {
	"SSH Multi Access module",
	NULL,
	init_module,
};

void
init_module () {
	subMenu *SMenu;
	SMenu = malloc(sizeof(subMenu));
	SMenu->id = 1;
	strcpy(SMenu->title, "SSH Multi");
	SMenu->defaultAction = openInterface;
	Menu *GMenu;
	GMenu = malloc(sizeof(Menu));
	GMenu->pos = 0;
	strcpy(GMenu->title,"Utils");
	strcpy(GMenu->headerkey,"F4");
	GMenu->funckey = KEY_F(4);
	GMenu->draw = 60;
	GMenu->subcnt = 1;
	GMenu->sub = SMenu;
	GMenu->nextMenu = NULL;
	Mod.ModMenu = GMenu;
}

char *choices[] = {
	"Host list file:", 
	"Number of threads:",
	"Private key file:",
	"Public key file:",
	"Password:",
	"Exit",
};

typedef struct _config {
	char hostfile[1024];
} Config;

void
openInterface(WINDOW *messagebar) {
	ITEM **my_items;
	int c;		
	MENU *my_menu;
	int n_choices, i;
	werase(messagebar);
	WINDOW *menuwin;
	WINDOW *menuoptwin;
	Config config;


	/* Create items */
	n_choices = ARRAY_SIZE(choices);
	my_items = (ITEM **)calloc(n_choices, sizeof(ITEM *));
	for(i = 0; i < n_choices; ++i)
		my_items[i] = new_item(choices[i], "");
		//my_items[i] = new_item(choices[i], choices[i]);

	/* Crate menu */
	my_menu = new_menu((ITEM **)my_items);

	/* Create the window to be associated with the menu */
	wbkgd(messagebar, COLOR_PAIR(3));
	keypad(messagebar, TRUE);

	/* Set main window and sub window */

	menuwin = derwin(messagebar, n_choices, LINES*2-2, 3, 1);
	menuoptwin = derwin(messagebar, n_choices, LINES*2-20,3 , 19);
	wbkgd(menuwin, COLOR_PAIR(1));
	wbkgd(menuoptwin, COLOR_PAIR(1));

	set_menu_win(my_menu, menuwin);
	set_menu_sub(my_menu, menuwin);
	set_menu_format(my_menu, n_choices, 1);
	set_menu_back(my_menu, COLOR_PAIR(1));


	/* Set menu mark to the string " * " */
	set_menu_mark(my_menu, "");

	/* Print a border around the main window and print a title */
	box(messagebar, 0, 0);
	print_in_middle(messagebar, 1, 0, 30, "SSH Connection menu:", COLOR_PAIR(3));
	//mvwaddch(messagebar, 2, 0, ACS_LTEE);
	mvwhline(messagebar, 2, 1, ACS_HLINE, LINES*2-2);
	mvwhline(messagebar, 3+n_choices, 1, ACS_HLINE, LINES*2-2);
//	mvwaddch(messagebar, 2, 39, ACS_RTEE);
	mvprintw(LINES - 1, 0, "F1 to exit. COLS=%d LINES=%d", COLS, LINES);
	refresh();

	/* Post the menu */
	post_menu(my_menu);
	wrefresh(menuwin);
	touchwin(stdscr);
	refresh();

	while((c = wgetch(messagebar)) != 27)
	{       switch(c)
		{   case KEY_DOWN:
			menu_driver(my_menu, REQ_DOWN_ITEM);
			break;
		    case KEY_UP:
			menu_driver(my_menu, REQ_UP_ITEM);
			break;
		    case ENTER: /* Enter */
//			move(20, 0);
//			clrtoeol();
	//		get_MenuOptions(MenuOptForm, config.hostfile);
		//	wscanw(menuoptwin,config.hostfile);
	//		mvprintw(20, 0, "Item selected is : %s, input: %s", 
	//				item_name(current_item(my_menu)), config.hostfile);
		//	getch();
//			pos_menu_cursor(my_menu);
			break;
		}
		wrefresh(menuwin);
	}	

	/* Unpost and free all the memory taken up */
	//unpost_menu(my_menu);
	//free_menu(my_menu);
//	for(i = 0; i < n_choices; ++i)
//		free_item(my_items[i]);
//	endwin();
}

void print_in_middle(WINDOW *win, int starty, int startx, int width, char *string, chtype color)
{   int length, x, y;
	float temp;

	if(win == NULL)
		win = stdscr;
	getyx(win, y, x);
	if(startx != 0)
		x = startx;
	if(starty != 0)
		y = starty;
	if(width == 0)
		width = 80;

	length = strlen(string);
	temp = (width - length)/ 2;
	x = startx + (int)temp;
	wattron(win, color);
	mvwprintw(win, y, x, "%s", string);
	wattroff(win, color);
	refresh();
}

void 
get_MenuOptions(FORM *myForm, char *str) {
	
}
