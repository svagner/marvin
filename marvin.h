#pragma once
#ifndef MARVIN_H_GUARD
#define MARVIN_H_GUARD
#include <ncurses.h> 
#include <stdlib.h>
#include <signal.h>

#define ENTER		    10
#define ESCAPE		    27
#define MAXSUBITEM	    15
#define MAXMENUITEM	    10
#define KEY_Q		    113
#define KEY_ALT		    195
#define KEY_BORDER_H	    124 
#define KEY_BORDER_V	    45 

#define HOTKEY_PRINT(a)	    do { waddstr(menubar,"("); waddstr(menubar,a); waddstr(menubar,")"); } while(0)

#define ATTR_UNUSED __attribute__((unused))

WINDOW *menubar, *messagebar, *debugbar;

typedef struct _submenu {
	int id;				/* Identification	    */
	char *title;			/* Title for sub menu	    */
	void (*defaultAction)();		/* Function for action	    */
} subMenu;				/* Sub menus type desc	    */

typedef struct _menu {
	int pos;	    /* Menu position (0..n)	    */
	char *title;	    /* Menu title		    */
	char *headerkey;    /* Hotkey name (for menu)	    */
	int funckey;	    /* Hotkey identify from ncurses */
	int draw;	    /* Indent menu item from 0 pos  */
	int subcnt;	    /* Count submenus for this	    */
	subMenu sub[];	    /* This - array submenus	    */
} Menu;			    /* General menu type	    */

typedef struct _moduleinfo {
	int id;
	char *name;
	int addr;
	char *description;
} ModuleInfo;	

/*  Start static initiolization for menu items	*/


static Menu AMenu = {
	    0,
	    "System",
	    "F1",
	    KEY_F(1),
	    0,
	    3,
	    {{0, "Packages", NULL}, 
	    {1, "Ports", NULL}, 
	    {2, "Sysctls", NULL}},
};

static Menu BMenu = {
	    1,
	    "Monitoring", 
	    "F2",
	    KEY_F(2), 
	    20,
	    4,
	    {{0, "Process", NULL}, 
	    {1, "Input/Output", NULL},
	    {2, "Network", NULL},
	    {3, "Memory", NULL}},
};
static Menu CMenu = {
	    2,
	    "Settings", 
	    "F3",
	    KEY_F(3), 
	    40,
	    1,
	    {{0, "Modules", NULL}}, 
};

#ifdef SIGWINCH 
/* If we get SIGWINCH. */
//volatile int want_resize = 0;
static int want_resize = 0;
void sig_winch(int sig ATTR_UNUSED);
void do_resize();
void iface_resize();
#endif

static Menu *AllMenu[] = { &AMenu, &BMenu, &CMenu };
/* End static init	*/
int cnt_menu;	/* Counter of general menu    */

void init_curses();
void draw_menubar(WINDOW *, int);
void draw_debugbar(WINDOW *debugbar);
void delete_menu(WINDOW **items,int count);
WINDOW **draw_menu(int, Menu*);
int scroll_menu(WINDOW **items,int, int);
void get_current_winsize(int *x, int *y);
int init_modules();
int init_screen();

#endif
