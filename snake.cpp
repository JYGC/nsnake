/*
 * Author: TheCPlusPlusGuy
 */

#include "snake.h"

/*
 * TODO:
 * -make a menu
 * -toplist
 * -colors
 */

snakepart::snakepart(int col, int row){
	x = col;
	y = row;
}

snakepart::snakepart()
{
	x = 0;
	y = 0;
}

snakeclass::snakeclass()
{
	initscr();
	start_color();
	init_pair(1, COLOR_GREEN, COLOR_BLACK);
	init_pair(2, COLOR_BLUE, COLOR_BLUE);
	init_pair(3, COLOR_RED, COLOR_BLACK);

	//bkgd(COLOR_WHITE);

	nodelay(stdscr, true); /* Program not wait until user pesses a key
							*/
	
	keypad(stdscr, true);
	noecho();
	curs_set(0);
	getmaxyx(stdscr,maxheight,maxwidth);
	
	/*
	 * Initialize few variables
	 */
	partchar = 'x';
	oldalchar = (char)219; /* (char)219 is a full retangle */
	foo = '@';
	food.x = 0;
	food.y = 0;
	
	for(int i = 0; i < 5; i++)
		snake.push_back(snakepart(40 + i, 10));
	
	points = 0;
	del = 110000;
	get = false;
	direction = 'l';
	srand(time(0));
	putfood();
	
	for(int i = 0; i < maxwidth - 1; i++){
		move(maxheight-2,i);
		addch(oldalchar);
	}
	
	for(int i = 0; i < maxheight-2; i++){
		move(i,maxwidth-2);
		addch(oldalchar);
	}
	
	for(int i = 0; i < snake.size(); i++){
		move(snake[i].y, snake[i].x);
		attron(COLOR_PAIR(1));
		addch(partchar);
		attroff(COLOR_PAIR(1));
	}
	move(maxheight-1, 0);
	printw("%d", points);
	move(food.y, food.x);
	attron(COLOR_PAIR(2));
	addch(foo);
	attroff(COLOR_PAIR(2));
	refresh();
}

snakeclass::~snakeclass(){
	
	nodelay(stdscr, false);
	getch();
	endwin();
}

void snakeclass::putfood(){
	
	while(1){
		int tmpx = (rand() % maxwidth) + 1;
		int tmpy = (rand() % maxheight) + 1;
		
		for(int i = 0; i < snake.size(); i++)
			if(snake[i].x == tmpx && snake[i].y == tmpy)
				continue;
		
		if(tmpx >= maxwidth - 2 || tmpy >= maxheight -  3)
			continue;
		
		food.x = tmpx;
		food.y = tmpy;
		break;
	}
	move(food.y, food.x);
	attron(COLOR_PAIR(2));
	addch(foo);
	attroff(COLOR_PAIR(2));
	refresh();
}

bool snakeclass::collision(){
	/* if snake collides with wall */
	if(snake[0].x == 0 || snake[0].x == maxwidth - 1 || snake[0].y == 0 ||
			snake[0].y == maxheight - 2)
		return true;
	
	/* if snake collides with self */
	for(int i = 2; i < snake.size(); i ++)
		if(snake[0].x == snake[i].x && snake[0].y == snake[i].y)
			return true;
	
	/* if snake collides with food */
	if(snake[0].x == food.x && snake[0].y == food.y){
		get = true;
		putfood();
		points += 10;
		move(maxheight - 1, 0);
		printw("%d", points);
		
		/* delay shortened, snake moves faster */
		if(points % 100 == 0)
			del -= 10000;
	}else
		get = false;
	return false;
}

void snakeclass::movesnake(){
	int tmp = getch();
	switch(tmp){
		case KEY_LEFT:
			if(direction != 'r')
				direction = 'l';
			break;
		case KEY_UP:
			if(direction != 'd')
				direction = 'u';
			break;
		case KEY_DOWN:
			if(direction != 'u')
				direction = 'd';
			break;
		case KEY_RIGHT:
			if(direction != 'l')
				direction = 'r';
			break;
		case KEY_BACKSPACE:
			direction = 'q';
			break;
	}
	
	if(!get){
		move(snake[snake.size() - 1].y, snake[snake.size() - 1].x);
		addch(' ');
		refresh();
		snake.pop_back();
	}
	
	if(direction == 'l')
		snake.insert(snake.begin(), snakepart(snake[0].x - 1, snake[0].y));
	else if(direction == 'r')
		snake.insert(snake.begin(), snakepart(snake[0].x + 1, snake[0].y));
	else if(direction == 'u')
		snake.insert(snake.begin(), snakepart(snake[0].x, snake[0].y - 1));
	else if(direction == 'd')
		snake.insert(snake.begin(), snakepart(snake[0].x, snake[0].y + 1));
	
	move(snake[0].y, snake[0].x);
	attron(COLOR_PAIR(1));
	addch(partchar);
	attroff(COLOR_PAIR(1));
	refresh();
}

void snakeclass::start(){
	while(1){
		if(collision()){
			move((maxwidth / 2) - 4, maxheight / 2);
			attron(COLOR_PAIR(3));
			printw("game over");
			attroff(COLOR_PAIR(3));
			break;
		}
		movesnake();
	
		if(direction == 'q')
			break;
	
		usleep(del);
	}
}
