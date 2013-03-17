#include "../../marvin.h"

void init_module ();

struct _module Mod = {
	"Monitoring module",
	NULL,
	init_module,
};

void
init_module () {
	subMenu *SMenu;
	SMenu = malloc(sizeof(subMenu));
	SMenu->id = 1;
	strcpy(SMenu->title, "Monitoring");
	Menu *GMenu;
	GMenu = malloc(sizeof(Menu));
	GMenu->pos = 0;
	strcpy(GMenu->title,"Top");
	strcpy(GMenu->headerkey,"F3");
	GMenu->funckey = KEY_F(3);
	GMenu->draw = 40;
	GMenu->subcnt = 1;
	GMenu->sub = SMenu;
	GMenu->nextMenu = NULL;
	Mod.ModMenu = GMenu;
}
