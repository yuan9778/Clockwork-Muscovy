#ifndef CORE_H
#define CORE_H

using namespace std;

struct map_info
{
	string mapfilepath;
	int map_width;//how many characters in a row in ASCII map layer
	int map_height;//how many characters in a row in ASCII map layer
	int screenC;//the player's current screen pixel X coordinates in this map (0~1023)
	int screenR;//the player's current screen pixel Y coordinates in this map (0~599)
	int startC;//absolute starting point pixel coordinates for X(Col) in the real large picture bmp map
	int startR;//absolute starting point pixel coordinates for Y(Row) in the real large picture bmp map
	int endC;//absolute ending point pixel coordinates for X(Col) in the real large picture bmp map
	int endR;//absolute ending point pixel coordinates for Y(Row) in the real large picture bmp map
	int mapID; // 0-main map; 1-jeb1; 2-jeb2; 3-jeb3; 4-commons1;
};




void initializeinfo();
void deletemap(struct map_info);
void initializemap(struct map_info);

void explore_update();
void dialogueupdate(HWND);
void dialogue_initial();
void draw();
void findPath(int);
void update_location(int);
void map_switch();
int isAround(int, int , int);
void BattleMode();
void initial();

#endif


