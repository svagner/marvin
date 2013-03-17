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

void
ModuleList()
{
	ModSysInfo *modlist_bak;
	modlist_bak = modlist;
	werase(messagebar);

	wprintw(messagebar, "\n\tId\t\tName\t\tAddress");
	mvwhline(messagebar,2, 2, 0, 10); 
	do
	{
	    nl();
	    wprintw(messagebar, "\n\t%d\t\t%s\t0x%x",modlist->id,modlist->name,modlist->addr);
	} while ((modlist=modlist->next));
	modlist=modlist_bak;
	box(messagebar,0,0);
	wrefresh(messagebar);
/*	waddch(messagebar, ACS_ULCORNER);
	waddch(messagebar, ACS_HLINE);
	waddch(messagebar, ACS_URCORNER);*/
	touchwin(stdscr);
	refresh();
};

int
init_modules(ModSysInfo *modlist, Menu *GMenu) {
	struct dirent *entry;
	DIR *dp;
	char name[50];
	char dlname[50];
	void *module;
	char pwd[255];
	int error, cnt=0;

	ModInfo *Mod;
	ModSysInfo *modlist_new;

	if((dp=opendir(MODULEPATH)))
	{
		while((entry = readdir(dp)))
		{
			bzero(name, sizeof(name));
			bzero(dlname, sizeof(dlname));
			bzero(pwd, sizeof(pwd));
			sscanf(entry->d_name,"%[^.].%s",name,dlname);
			if (!strcmp(dlname, "so"))
			{

				strcat(pwd, MODULEPATH);
				strcat(pwd, entry->d_name);
				module = dlopen(pwd, RTLD_LAZY);
				if (!module) {
				    fprintf(stderr, "Couldn't open %s: %s\n", pwd, dlerror());
				    exit(1);
                                }
				if (!(Mod = dlsym(module, "Mod"))) {
				    fprintf(stderr, "Couldn't find hello: %s\n", dlerror());
				    exit(1);
				}
				Mod->initMod();
				if (modlist->id && cnt>0) 
				{
					modlist_new = malloc(sizeof(ModSysInfo));
					modlist->next = modlist_new;
					modlist = modlist_new;
				}
				printf("%s\n", Mod->ModMenu->title);
				cnt++;
				modlist->id = cnt;
				modlist->name = Mod->name;
				modlist->addr = Mod;

				if (Mod->ModMenu)
				{
					GMenu->nextMenu = Mod->ModMenu;
					GMenu->nextMenu->prevMenu = GMenu;
					GMenu=GMenu->nextMenu;
				};


				///* Now call the function in the DL library */
				//(*demo_function)();
				//
				///* All done, close things cleanly */
				//dlclose(module);
				//return 0;
			}
			modlist->next=NULL;
		}
	};

	return 0;
};

int main()
{
	int res;
#ifdef SIGWINCH
	signal(SIGWINCH, sig_winch);
#endif
	modlist = malloc(sizeof(ModSysInfo));

	subMenu *SMenu;
/* init default menu */	
	SMenu = malloc(sizeof(subMenu));
	SMenu->id = 1;
	strcpy(SMenu->title, "Modules");
	SMenu->defaultAction = (void*)ModuleList;

	Menu *GMenu;
	GMenu = malloc(sizeof(Menu));
	GMenu->pos = 0;
	strcpy(GMenu->title,"Settings");
	strcpy(GMenu->headerkey,"F1");
	GMenu->funckey = KEY_F(1);
	GMenu->draw = 0;
	GMenu->subcnt = 1;
	GMenu->sub = SMenu;
	GMenu->nextMenu = NULL;
	GMenu->prevMenu = NULL;


	init_curses();
	if((res=init_modules(modlist,GMenu))<0)
		return 1;
	/* for debug */
/*	do
	{
		printf("ID: %d, Module: %s, Addr: 0x%x\n", modlist->id, modlist->name, modlist->addr);
	} while ((modlist=modlist->next));
	do
	{
		printf("GMenu: %s\n", GMenu->title);
	} while ((GMenu=GMenu->nextMenu));*/
	if((res=init_screen(GMenu))<0)
		return 1;

	return 0;
}
