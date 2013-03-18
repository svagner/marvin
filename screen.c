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

void draw_menubar(WINDOW *menubar, int menu_cnt, Menu *GMenu)
{
	int counter;

	wbkgd(menubar,COLOR_PAIR(2));

	do {
	    wmove(menubar,0,GMenu->draw);
	    waddstr(menubar,GMenu->title);
	    wattron(menubar,COLOR_PAIR(3));
	    HOTKEY_PRINT(GMenu->headerkey);
	    wattroff(menubar,COLOR_PAIR(3));
	} while((GMenu = GMenu->nextMenu));
}

void draw_debugbar(WINDOW *debugbar)
{
	wbkgd(debugbar,COLOR_PAIR(2));
}

WINDOW **draw_menu(int start_col, Menu *GMenu)
{
	int i;
	WINDOW **items;
	items=(WINDOW **)malloc(9*sizeof(WINDOW *));

	subMenu *SMenu;
	SMenu = GMenu->sub;

	items[0]=newwin(GMenu->subcnt+2,19,1,start_col);
	wbkgd(items[0],COLOR_PAIR(2));
	box(items[0],ACS_VLINE,ACS_HLINE);
	for(i=1; i<=(GMenu->subcnt)+1; i++)
	    items[i]=subwin(items[0],1,17,1+i,start_col+1);
	i=0;
	do 
	{
	    i++;	
	    wprintw(items[i],GMenu->sub->title,i);
	} while ((GMenu->sub=GMenu->sub->nextsubMenu));
	GMenu->sub=SMenu;
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

int scroll_menu(WINDOW **items,int count, int current_menu, Menu *GMenu, selectItem *ItemSelect)
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
		} else if (key==KEY_LEFT && GMenu->prevMenu) {
			delete_menu(items,count+1);
			touchwin(stdscr);
			refresh();
			GMenu = GMenu->prevMenu;
			items=draw_menu(GMenu->draw, GMenu);
			return scroll_menu(items,GMenu->subcnt, current_menu-1, GMenu, ItemSelect);
		} else if (key==KEY_RIGHT && GMenu->nextMenu) {
			delete_menu(items,count+1);
			touchwin(stdscr);
			refresh();
			GMenu = GMenu->nextMenu;
			items=draw_menu(GMenu->draw, GMenu);
			return scroll_menu(items,GMenu->subcnt, current_menu+1, GMenu, ItemSelect);
		} else if (key==ESCAPE) {
			return -1;
		} else if (key==ENTER) {
			ItemSelect->GMenu = GMenu;
			return selected;
		}
	}
}

int
init_screen (Menu *GMenu) {
	int key;
	int res;
	int counter, current_menu;

	Menu *GenMenu;

//	cnt_menu = sizeof(AllMenu)/sizeof(*AllMenu);
	bkgd(COLOR_PAIR(1));
	menubar=subwin(stdscr,1,0,0,0);
	//messagebar=subwin(stdscr,1,79,23,1);
	messagebar=subwin(stdscr,40,LINES*2,2,2);
	draw_menubar(menubar, cnt_menu, GMenu);

	debugbar=subwin(stdscr,1,0,LINES-1,0);
	draw_debugbar(debugbar);
	move(1,2);
	printw("Marvin System Management");
	//move(2,2);
	wprintw(messagebar,"Press F[1-12] to open the menus or ESC for quit.");
//	printw("\nCount menu: %d\nAddress: Amenu %s: 0x%x vs 0x%x.\nCols=%d Lines=%d", cnt_menu, AllMenu[0]->title, AllMenu[0], &AMenu, COLS, LINES);
	refresh();

	do {
		int selected_item;
		selectItem *ItemSelect, *ItemSelectSave;
		WINDOW **menu_items;

		ItemSelect = malloc(sizeof(selectItem));
		key=getch();
		werase(debugbar);
		wrefresh(debugbar);
		GenMenu=GMenu;
		do
		{
		    if (key==GenMenu->funckey || key==BUTTON_ALT) {
			current_menu=GenMenu->pos;    
			menu_items=draw_menu(GenMenu->draw, GenMenu);
			selected_item=scroll_menu(menu_items,GenMenu->subcnt, current_menu, GenMenu, ItemSelect);
			delete_menu(menu_items,9);
			if (selected_item<0)
				wprintw(debugbar,"You haven't selected any item.");
			else
			{

				//wprintw(debugbar,
				//		"You have selected menu item %d - %s.",selected_item+1,GenMenu->sub[selected_item].title);
				ItemSelectSave = ItemSelect->GMenu->sub;
				do 
				{
					if(ItemSelect->GMenu->sub->id==selected_item+1)
					{
					    (*ItemSelect->GMenu->sub->defaultAction)();	
					    wprintw(debugbar,
						"You have selected menu item %d - %s.",selected_item+1,ItemSelect->GMenu->sub->title);
					    break;
					};
//				if (AllMenu[counter]->sub[selected_item].defaultAction)
//				    (*AllMenu[counter]->sub[selected_item].defaultAction)();
				} while ((ItemSelect->GMenu->sub=ItemSelect->GMenu->sub->nextsubMenu));
				ItemSelect->GMenu->sub = ItemSelectSave;
			}
			touchwin(stdscr);
			refresh();
		    };
		} while ((GenMenu=GenMenu->nextMenu));
		free(ItemSelect);    
		
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
}

void
iface_resize(Menu *GMenu)
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
	draw_menubar(menubar, cnt_menu, GMenu);
	werase(messagebar);
	wrefresh(messagebar);
	werase(debugbar);
	wrefresh(debugbar);
	debugbar=subwin(stdscr,1,0,LINES-1,0);
	draw_debugbar(debugbar);

	touchwin(stdscr);
	refresh ();
}
