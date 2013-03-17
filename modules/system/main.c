#include "../../marvin.h"

void init_module ();

struct _module Mod = {
	"System module",
	NULL,
	init_module,
};

void
init_module () {
	subMenu *SMenu, *SMenu_new;
	SMenu = malloc(sizeof(subMenu));
	SMenu->id = 1;
	strcpy(SMenu->title, "Sysctls");
	SMenu_new = malloc(sizeof(subMenu));
	SMenu_new->id = 2;
	strcpy(SMenu_new->title, "Packages");
	SMenu->nextsubMenu=SMenu_new;

	Menu *GMenu;
	GMenu = malloc(sizeof(Menu));
	GMenu->pos = 0;
	strcpy(GMenu->title,"System");
	strcpy(GMenu->headerkey,"F2");
	GMenu->funckey = KEY_F(2);
	GMenu->draw = 20;
	GMenu->subcnt = 2;
	GMenu->sub = SMenu;
	GMenu->nextMenu = NULL;
	Mod.ModMenu = GMenu;
}
