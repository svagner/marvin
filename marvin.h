#pragma once
#ifndef MARVIN_H_GUARD
#define MARVIN_H_GUARD
#include <ncurses.h> 
#include <stdlib.h>
#include <signal.h>
#include <string.h>
#include <sys/types.h>
#include <dirent.h>
#include <errno.h>
#include <stdio.h>
#include <dlfcn.h>
#include <unistd.h>

#define ENTER		    10
#define ESCAPE		    27
#define MAXSUBITEM	    15
#define MAXMENUITEM	    10
#define KEY_Q		    113
#define KEY_ALT		    195
#define KEY_BORDER_H	    124 
#define KEY_BORDER_V	    45 
#define MAXNAMELEN	    54
#define MODULEPATH	    "./modules/"

int pair_count;
 
/* Colours */
#define BLACK 0
#define DARK_BLUE 1
#define DARK_GREEN 2
#define DARK_CYAN 3
#define DARK_RED 4
#define DARK_PURPLE 5
#define DARK_YELLOW 6
#define LIGHT_GREY 7
#define DARK_GREY 8
#define LIGHT_BLUE 9
#define LIGHT_GREEN 10
#define LIGHT_CYAN 11
#define LIGHT_RED 12
#define LIGHT_PURPLE 13
#define LIGHT_YELLOW 14
#define WHITE 15


#define HOTKEY_PRINT(a)	    do { waddstr(menubar,"("); waddstr(menubar,a); waddstr(menubar,")"); } while(0)

#define ATTR_UNUSED __attribute__((unused))

WINDOW *menubar, *messagebar, *debugbar;

typedef struct _submenu {
	int id;				/* Identification	    */
	char title[MAXNAMELEN];		/* Title for sub menu	    */
	void (*defaultAction)(WINDOW *messagebar);	/* Function for action	    */
	struct _submenu *nextsubMenu;
	struct _submenu *prevsubMenu;
} subMenu;				/* Sub menus type desc	    */

typedef struct _menu {
	int pos;			/* Menu position (0..n)	    */
	char title[MAXNAMELEN];		/* Menu title		    */
	char headerkey[5];		/* Hotkey name (for menu)	    */
	int funckey;			/* Hotkey identify from ncurses */
	int draw;			/* Indent menu item from 0 pos  */
	int subcnt;			/* Count submenus for this	    */
	subMenu *sub;			/* This - array submenus        */
	struct _menu *nextMenu;		/* Next Menu in List            */
	struct _menu *prevMenu;		/* Prev Menu in List            */
} Menu;					/* General menu type	    */

typedef struct _module {
	char *name;             /* Module name */
	Menu *ModMenu;          /* Menu for module */
	void (*initMod)();
} ModInfo;

typedef struct _moduleinfo {
	int id;
	char *name;
	Menu *ModMenu;
	int addr;
	struct _moduleinfo *next;
//	char *description;
} ModSysInfo;	

typedef struct _selectItem {
	int selected;
	struct _menu *GMenu;
	struct _submenu *SMenu;
} selectItem;

/*  Start static initiolization for menu items	*/

#ifdef SIGWINCH 
/* If we get SIGWINCH. */
//volatile int want_resize = 0;
static int want_resize = 0;
void sig_winch(int sig ATTR_UNUSED);
void do_resize();
void iface_resize();
#endif

/* End static init	*/
int cnt_menu;	/* Counter of general menu    */
ModSysInfo *modlist;

void init_curses();
void draw_menubar(WINDOW *, int, Menu*);
void draw_debugbar(WINDOW *debugbar);
void delete_menu(WINDOW **items,int count);
WINDOW **draw_menu(int, Menu*);
int scroll_menu(WINDOW **items,int, int, Menu*, selectItem*);
void get_current_winsize(int *x, int *y);
int init_modules();
int init_screen();
void ModuleList();

#endif
