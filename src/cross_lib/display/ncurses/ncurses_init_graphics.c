
	void INIT_GRAPHICS(void) 
	{						
		#elif defined(__NCURSES__)
            #include <ncurses.h>
			
			initscr();   
            noecho();
            curs_set(0);
			start_color();
			cbreak();
			intrflush(stdscr, TRUE);
			init_pair(COLOR_YELLOW, COLOR_YELLOW, COLOR_BLACK);
			init_pair(COLOR_CYAN, COLOR_CYAN, COLOR_BLACK);
			init_pair(COLOR_RED, COLOR_RED, COLOR_BLACK);
			init_pair(COLOR_GREEN, COLOR_GREEN, COLOR_BLACK);
			init_pair(COLOR_BLUE, COLOR_BLUE, COLOR_BLACK);
			init_pair(COLOR_WHITE, COLOR_WHITE, COLOR_BLACK);
			init_pair(COLOR_MAGENTA, COLOR_MAGENTA, COLOR_BLACK);
			#if !defined(TURN_BASED)
				nodelay(stdscr,TRUE);
			#endif
		#endif	
	}
	
	