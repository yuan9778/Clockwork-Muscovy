#include <Windows.h>
#include<vector>
#include <queue>
#include<fstream>
#include<string>
#include <ctime>
#include <Wincodec.h>
#include "core.h"
#include "character.h"
#include "pqastar.h"
#include "astar.h"
#include "timer.h"
#include "game.h"
#include "Graphics.h"
using namespace std;

//=============shared global variables start===============//
vector<Character*> sprites;
HBITMAP hbitmap[300];
HBITMAP player_stopping[4][1];
HBITMAP player_moving[4][3];
HBITMAP player_attacking[4][2];
HBITMAP player_deading[4][2];
HBITMAP player_dead[4][1];
HBITMAP player_magic[4][1];
HBITMAP player_pistol[4][3];
HBITMAP player_hit[4][2];
HBITMAP player_active[4][1];
HBITMAP player_grenade[4][2];


HBITMAP mob_stopping[4][1];
HBITMAP mob_moving[4][2];
HBITMAP mob_attacking[4][2];
HBITMAP mob_deading[4][2];
HBITMAP mob_dead[4][1];
HBITMAP mob_hit[4][2];

HBITMAP goggles_stopping[4][1];
HBITMAP police_stopping[4][1];
HBITMAP jint_stopping[4][1];
HBITMAP picture;
HBITMAP current_journal;

struct map_info current_map;
char** map;
Timer animationTimer;
int mode;
int cursorX, cursorY;//current mouse left click position
int  temp_cursorX, temp_cursorY;//previous mouse left click position
int targetX,targetY;
bool mouseclick;
nodeptr p;
int preRow, preCol, currRow, currCol;
int step;//moving each tile(character square cell) will be divide step frame, step = 12, mean moving one tile need 12 frames, each frame moving 120/12=10 pixels in big map, and 40/12=3.33 pixel in small map
int inner_c; 
int Rd, Cd;//each moving step(frame) moves (Rd,Cd) pixel distance
//=============shared global variables end==============//


//dedicated global variable for this core.cpp
unsigned int swch;//animation frame swapping counter
bool FinalPixelDelivery = false;//special key can open the door directly to final delivery part, even inner_c % step != 0;
int MouseX;
int MouseY;
struct map_info main_map;
struct map_info jeb1;
struct map_info jeb2;
struct map_info jeb3;
struct map_info commons;

string dialogue[100];
string current_dialogue; 
HBITMAP journal[7];

int dindex;
bool change_map;

extern bool hold;

void initializeinfo()
{
  main_map.mapfilepath = ".//ASCII//main_map.txt";
  main_map.map_height = 105;
  main_map.map_width = 38;
  main_map.mapID = 0;
  
  jeb1.mapfilepath = ".//ASCII//jeb1.txt";
  jeb1.map_height = 55;
  jeb1.map_width = 18;
  jeb1.mapID = 1;
  
  jeb2.mapfilepath = ".//ASCII//jeb2.txt";
  jeb2.map_height = 55;
  jeb2.map_width = 18;
  jeb2.mapID = 2;
  
  jeb3.mapfilepath = ".//ASCII//jeb3.txt";
  jeb3.map_height = 55;
  jeb3.map_width = 18;
  jeb3.mapID = 3;
  
  commons.mapfilepath = ".//ASCII//commons.txt";
  commons.map_height = 50;
  commons.map_width = 45;
  commons.mapID = 4;
  
}

void initializemap(struct map_info mapinfo)  // load ASCII map into map
{
	ifstream in_file;
	string line;
	in_file.open(mapinfo.mapfilepath.c_str());
	if (in_file.fail())
		MessageBox(NULL, "Open failed!", "Fail", MB_ICONEXCLAMATION | MB_OK);
	map = new char*[mapinfo.map_height];
  
	for(int i=0;i<mapinfo.map_height;i++) {
		map[i]=new char[mapinfo.map_width];
    }
  
	for (int i=0; i<mapinfo.map_height; i++) {
		in_file >> line;
		for(int j=0; j<mapinfo.map_width; j++) {
			map[i][j] = line[j];
		}
    }
}   

void deletemap(struct map_info mapinfo) // delete map[][]
{
  for(int i=0;i<mapinfo.map_height;i++)
    {
      delete []map[i];
    }
  delete []map;
  
}

void map_switch() // switch maps
{
  switch (current_map.mapID)
    {
    case 0: // main_map
      {
	if(sprites[0]->getPosition()->getX() > 3580 && sprites[0]->getPosition()->getX() < 3780 && sprites[0]->getPosition()->getY() > 3700 && sprites[0]->getPosition()->getY() < 4000) // switch to JEB1
	  {
	    deletemap(current_map);
	    initializemap(jeb1);
	    current_map = jeb1;
	    sprites[0]->getPosition()->moveCardinalDirectionPixel(320, 1960,1);
	    current_map.startR = 1600;
	    current_map.startC = 0;
	    current_map.screenR = 360;
	    current_map.screenC = 320;
	    current_map.endR = 2200;
	    current_map.endC = 720;
	    change_map = true;
	    // nextColp = current_map.screenC;
	    // nextRowp = current_map.screenR;
	    
	  }
	else if(sprites[0]->getPosition()->getX() > 1480 && sprites[0]->getPosition()->getX() < 1750 && sprites[0]->getPosition()->getY() > 10033 && sprites[0]->getPosition()->getY() < 10200) // switch to commons.
	  {
	    deletemap(current_map);
	    initializemap(commons);
	    current_map = commons;
	    sprites[0]->getPosition()->moveCardinalDirectionPixel(1680, 520, 4);
	    current_map.startR = 220;
	    current_map.startC = 776;
	    current_map.screenR = 300;
	    current_map.screenC = 904;
	    current_map.endR = 820;
	    current_map.endC = 1800;
	    change_map = true;
	    // nextColp = current_map.screenC;
	    // nextRowp = current_map.screenR;
	    
	  }
	break;
	
      }
    case 1: // jeb1
      {
	if(sprites[0]->getPosition()->getX() > 200 && sprites[0]->getPosition()->getX() < 320 && sprites[0]->getPosition()->getY() > 2070 && sprites[0]->getPosition()->getY() < 2160) // switch to main_map
	  {
	    deletemap(current_map);
	    initializemap(main_map);
	    current_map = main_map;
	    sprites[0]->getPosition()->moveCardinalDirectionPixel(3600, 3600,0);
	    current_map.startR = 3300;
	    current_map.startC = 3088;
	    current_map.screenR = 300;
	    current_map.screenC = 512;
	    current_map.endR = 3900;
	    current_map.endC = 4112;
	    change_map = true;
	   // nextColp = current_map.screenC;
	   // nextRowp = current_map.screenR;
	    
	  }
	else if(sprites[0]->getPosition()->getX() > 360 && sprites[0]->getPosition()->getX() < 400 && sprites[0]->getPosition()->getY() > 2070 && sprites[0]->getPosition()->getY() < 2160) // switch to jeb2 from bottom
	  {
	    deletemap(current_map);
	    initializemap(jeb2);
	    current_map = jeb2;
	    sprites[0]->getPosition()->moveCardinalDirectionPixel(360, 2040,2);
	    current_map.startR = 1600;
	    current_map.startC = 0;
	    current_map.screenR = 440;
	    current_map.screenC = 360;
	    current_map.endR = 2200;
	    current_map.endC = 720;
	    change_map = true;
	    // nextColp = current_map.screenC;
	    // nextRowp = current_map.screenR;
	    
	  }
	else if(sprites[0]->getPosition()->getX() > 50 && sprites[0]->getPosition()->getX() < 140 && sprites[0]->getPosition()->getY() > 640 && sprites[0]->getPosition()->getY() < 720) // switch to jeb2 from left
	  {
	    deletemap(current_map);
	    initializemap(jeb2);
	    current_map = jeb2;
	    sprites[0]->getPosition()->moveCardinalDirectionPixel(200, 560,2);
	    current_map.startR = 260;
	    current_map.startC = 0;
	    current_map.screenR = 300;
	    current_map.screenC = 200;
	    current_map.endR = 860;
	    current_map.endC = 720;
	    change_map = true;
	   // nextColp = current_map.screenC;
	   // nextRowp = current_map.screenR;
	    
	  }
	break;
      }
    case 2: // jeb2
      {
	if(sprites[0]->getPosition()->getX() > 280 && sprites[0]->getPosition()->getX() < 320 && sprites[0]->getPosition()->getY() > 2160 && sprites[0]->getPosition()->getY() < 2200) // switch to jeb1 bottom
	  {
	    deletemap(current_map);
	    initializemap(jeb1);
	    current_map = jeb1;
	    sprites[0]->getPosition()->moveCardinalDirectionPixel(340, 2000,1);
	    current_map.startR = 1600;
	    current_map.startC = 0;
	    current_map.screenR = 300;
	    current_map.screenC = 340;
	    current_map.endR = 2200;
	    current_map.endC = 720;
	    change_map = true;
	   // nextColp = current_map.screenC;
	   // nextRowp = current_map.screenR;
	    
	  }
	else if(sprites[0]->getPosition()->getX() > 360 && sprites[0]->getPosition()->getX() < 400 && sprites[0]->getPosition()->getY() > 2160 && sprites[0]->getPosition()->getY() < 2200) // switch to jeb3 bottom
	  {
	    deletemap(current_map);
	    initializemap(jeb3);
	    current_map = jeb3;
	    sprites[0]->getPosition()->moveCardinalDirectionPixel(340, 2000,3);
	    current_map.startR = 1600;
	    current_map.startC = 0;
	    current_map.screenR = 300;
	    current_map.screenC = 340;
	    current_map.endR = 2200;
	    current_map.endC = 720;
	    change_map = true;
	    // nextColp = current_map.screenC;
	    // nextRowp = current_map.screenR;
	    
	  }
	else if(sprites[0]->getPosition()->getX() > 54 && sprites[0]->getPosition()->getX() < 127 && sprites[0]->getPosition()->getY() > 480 && sprites[0]->getPosition()->getY() < 560) // switch to jeb1 from left
	  {
	    deletemap(current_map);
	    initializemap(jeb1);
	    current_map = jeb1;
	    sprites[0]->getPosition()->moveCardinalDirectionPixel(200, 600,1);
	    current_map.startR = 300;
	    current_map.startC = 0;
	    current_map.screenR = 300;
	    current_map.screenC = 200;
	    current_map.endR = 900;
	    current_map.endC = 720;
	    change_map = true;
	    // nextColp = current_map.screenC;
	    // nextRowp = current_map.screenR;
	    
	  }
	else if(sprites[0]->getPosition()->getX() > 54 && sprites[0]->getPosition()->getX() < 127 && sprites[0]->getPosition()->getY() > 625 && sprites[0]->getPosition()->getY() < 700) // switch to jeb3 from left
	  {
	    deletemap(current_map);
	    initializemap(jeb3);
	    current_map = jeb3;
	    sprites[0]->getPosition()->moveCardinalDirectionPixel(200, 600,3);
	    current_map.startR = 300;
	    current_map.startC = 0;
	    current_map.screenR = 300;
	    current_map.screenC = 200;
	    current_map.endR = 900;
	    current_map.endC = 720;
	    change_map = true;
	    // nextColp = current_map.screenC;
	    // nextRowp = current_map.screenR;
	    
	  }
	break;
      }
    case 3: //jeb3
      {
	if(sprites[0]->getPosition()->getX() > 80 && sprites[0]->getPosition()->getX() < 140 && sprites[0]->getPosition()->getY() > 480 && sprites[0]->getPosition()->getY() < 555) // switch to JEB2 left
	  {
	    deletemap(current_map);
	    initializemap(jeb2);
	    current_map = jeb2;
	    sprites[0]->getPosition()->moveCardinalDirectionPixel(200, 560,2);
	    current_map.startR = 260;
	    current_map.startC = 0;
	    current_map.screenR = 300;
	    current_map.screenC = 200;
	    current_map.endR = 860;
	    current_map.endC = 720;
	    change_map = true;
	    // nextColp = current_map.screenC;
	    // nextRowp = current_map.screenR;
	    
	  }
	else if(sprites[0]->getPosition()->getX() > 280 && sprites[0]->getPosition()->getX() < 320 && sprites[0]->getPosition()->getY() > 2000 && sprites[0]->getPosition()->getY() < 2150) // switch to jeb2 bottom
	  {
	    deletemap(current_map);
	    initializemap(jeb2);
	    current_map = jeb2;
	    sprites[0]->getPosition()->moveCardinalDirectionPixel(360, 2040,2);
	    current_map.startR = 1600;
	    current_map.startC = 0;
	    current_map.screenR = 440;
	    current_map.screenC = 360;
	    current_map.endR = 2200;
	    current_map.endC = 720;
	    change_map = true;
	    // nextColp = current_map.screenC;
	    // nextRowp = current_map.screenR;
	    
	  }
	break;
	
      }
    case 4:  // commons
      {
	if(sprites[0]->getPosition()->getX() > 1735 && sprites[0]->getPosition()->getX() < 1800 && sprites[0]->getPosition()->getY() > 480 && sprites[0]->getPosition()->getY() < 650) // switch to main_map
	  {
	    deletemap(current_map);
	    initializemap(main_map);
	    current_map = main_map;
	    sprites[0]->getPosition()->moveCardinalDirectionPixel(1560, 9960,0);
	    current_map.startR = 9660;
	    current_map.startC = 1048;
	    current_map.screenR = 300;
	    current_map.screenC = 512;
	    current_map.endR = 10260;
	    current_map.endC = 2070;
	    change_map = true;
	    // nextColp = current_map.screenC;
	    // nextRowp = current_map.screenR;
	    
	  }
	break;
      }
      
    }
    sprites[0]->setMap(current_map.mapID);
}

void explore_update() {
    // get input from mouseclick on map.
    if(MouseX == temp_cursorX && MouseY == temp_cursorY) {//if last click position and current click position are the same-> either player is in the moving animation or player is idle
		//update: change to MouseXY from cursorXY, using absolute large map position to avoid click the same window position, player does not move.
		  if(p != nullptr) {//player is in the moving animation
			  if(animationTimer.ElapsedTime() >= 0.1 / 3.0f)//there are at most 3 frames in each animation. if we want these 3 frames finished playing in 0.1s then each frame uses 0.1/3.0 = 0.033 sec per frame
			  {											//each frame consume 10 pixel, 0.1 second moving 3 x 10 = 30 pixel. 1 second moving 30 x 10 = 600 pixel
				  animationTimer.Reset();
				  //here it might change mode from 1 to 2, 4 or switch map.
				  if(current_map.mapID == 0) {
					  update_location(120); 
				  }
				  else {
					  update_location(40);
				  }
			  }
		  } else {//player is in the idle mode, draw the same idle picture
			  if (hold) {//if the mouse left button hold and have not release, then when we finish this path moving, windows system will not sent WM_LBUTTONDOWN message continuely,
				  //so we have to send out fake WM_LBUTTONDOWN message to the wndProc of the current cursor position
				  SendMessage(hChild, WM_LBUTTONDOWN, 0, MAKELPARAM(cursorX, cursorY));//cursorXY never change from last mouse button down, send back to recevie new global large map MouseXY
			  }
			  if (!clickqueue.empty()) {
				  MouseX = clickqueue.front().first;
				  MouseY = clickqueue.front().second;
				  clickqueue.pop();
			  }
		  // idle
			sprites[0]->setCurrentSprite(player_stopping[sprites[0]->getPosition()->getDir()][0]);
		  }
	} else {//mouse left button is just clicked
      temp_cursorX = MouseX;//update clicked position to prev mouse click position
      temp_cursorY = MouseY;//to let the program re-rount the new path in next round.
      while(p != nullptr) {//clear the un-finished path if the click happens during the walking
		nodeptr temp = p;
		p = p->next;
		delete temp;
		temp = nullptr;
	  }
	  if(current_map.mapID == 0) {
		  findPath(120);
	  } else {
		 findPath(40);
	  }
    }
}

void findPath(int tile_size) {
  p = astar1_search(map, current_map.map_height, current_map.map_width, MouseY/tile_size, MouseX/tile_size, (current_map.startR+current_map.screenR)/tile_size, (current_map.startC+current_map.screenC)/tile_size);
  /*************************************************/
  /*                                               */
  /*  If the mouse click at a place has no rounts, */
  /*  we implement two methods to let the player   */
  /*  reach the target neary                       */
  /*                                               */
  /*************************************************/
  /*************************************************/
  /*                                               */
  /*  First version is let the player keep walking */
  /*  towads the target until reach the first      */
  /*  obstacle then the player stop walking        */
  /*                                               */
  /*************************************************/
/* 
  int sentryX = sprites[0]->getPosition()->getCol(), sentryY = sprites[0]->getPosition()->getRow();
  int newGoalX = sprites[0]->getPosition()->getCol(), newGoalY = sprites[0]->getPosition()->getRow();
  //if mouse click on a obstacle ('w'), then send a sentry to locate the first non-'w' tile in the line between player and target(mouse click)
  //sentry will arrive and stop at the first obstacle tile of 'w' and the new goal(target) will be the previous sentry(which is not 'w')
  if (path == nullptr) {
      while (sentryY >= 0 && sentryY < current_map.map_height &&
			 sentryX >= 0 && sentryX < current_map.map_width &&
			 map[sentryY][sentryX] != 'w') {
			 newGoalX = sentryX;
			 newGoalY = sentryY;
			  if (sentryY < MouseY / tile_size) {
				  ++sentryY;
			  }
			  else if (sentryY > MouseY / tile_size) {
				  --sentryY;
			  }
			  if (sentryX < MouseX / tile_size) {
				  ++sentryX;
			  }
			  else if (sentryX > MouseX / tile_size) {
				  --sentryX;
			  }
		}
		path = astar1_search(map, current_map.map_height, current_map.map_width, newGoalY, newGoalX, (current_map.startR + current_map.screenR) / tile_size, (current_map.startC + current_map.screenC) / tile_size);
		//if we found a new possible moving tile, update the MouseXY in case it will become crazy due to final pixel delivery part.
		if (path != nullptr) {
			MouseX = newGoalX * tile_size + tile_size / 2;
			MouseY = newGoalY * tile_size + tile_size / 2;
		}
  }*/
  /***********************************************/
  /*                                             */
  /* Second version is let the player walk towads*/
  /* the target as close as possible, the player */
  /* will reach the closest possible place  near */
  /* the target. (Better solution)               */
  /*                                             */
  /***********************************************/
  int newGoalX = MouseX / tile_size; //let the newGoalXY first be the target in the large map
  int newGoalY = MouseY / tile_size;
  while (p == nullptr && newGoalX != sprites[0]->getPosition()->getCol() && newGoalY != sprites[0]->getPosition()->getRow()) {
	  while (newGoalY >= 0 && newGoalY < current_map.map_height &&	//keep try to find the first none 'w' place in the line between target and player-->(staring from target),
		  newGoalX >= 0 && newGoalX < current_map.map_width &&      //if the new found one still cannot be rounted to the player, keep finding next none 'w' in this line with this pattern,
		  map[newGoalY][newGoalX] == 'w') {							//until found one or finally to be the player itself.
		  if (newGoalY < sprites[0]->getPosition()->getRow()) {
			  ++newGoalY;
		  }
		  else if (newGoalY > sprites[0]->getPosition()->getRow()) {
			  --newGoalY;
		  }
		  if (newGoalX < sprites[0]->getPosition()->getCol()) {
			  ++newGoalX;
		  }
		  else if (newGoalX > sprites[0]->getPosition()->getCol()) {
			  --newGoalX;
		  }
	  }
	  p = astar1_search(map, current_map.map_height, current_map.map_width, newGoalY, newGoalX, (current_map.startR + current_map.screenR) / tile_size, (current_map.startC + current_map.screenC) / tile_size);
  }
  //the new GoalXY will(must) ultimately be found: the wrost case, it will be player itself.
  //update the MouseXY in case it will become crazy due to final pixel delivery part.
  MouseX = newGoalX * tile_size + tile_size / 2;
  MouseY = newGoalY * tile_size + tile_size / 2;

}

void update_location(int tile_size)
{
	if (p != nullptr) {
					//every 12 frame recalculate a new Rd,Cd for a new cell/tile move.
					  if(inner_c%step == 0 || FinalPixelDelivery)//FinalPixelDelivery: special key can open the door directly to final delivery part, even inner_c % step != 0;
						{
						  inner_c = 0;
						  if (p->next != nullptr) {  //at least still have 2+ tiles--> at least 1+ big moves, starting from here: inner_c % step == 0->just finish last big move, start a new big move
							  if (p->next->next == nullptr) {//if there are only 2 tiles-->1 tile to move, we remove the last move and make it directly to final pixel delivery to avoid silly final step back walking due to big tile inaccuracy.
								  delete p->next;	//delete p->next, make it only p left, return and next time it will go to final pixel delivery section.
								  p->next = nullptr;
								  return;
							  } else {
								  preRow = p->info.y;
								  preCol = p->info.x;
								  nodeptr temp = p;
								  p = p->next;
								  delete temp;
								  temp = nullptr;

								  currRow = p->info.y;
								  currCol = p->info.x;
								  Rd = (currRow - preRow)*tile_size/step;
								  Cd = (currCol - preCol)*tile_size/step;
							  }
						  } else { //p=nullptr and p->next == nullptr==> last big moving proceeding, and in this part, inner_c%step == 0 means, the last big move has finished all small steps, it's time to stop moving
							  //before completely stop this moving animation, there may be a little difference between Astar last move finishing point(pixel) and the real mouse left click pixel due to the big cell/tile inaccuracy.
							  //let handle this last pixel delivery.


							  //MouseXY is not record here, since this is after final step, starR/C has already been changed.
							  //So MouseXY is record immediately after mouse left click in WndProc WM_LBUTTONDOWN, same place as cursorXY
//							  if (FinalPixelDelivery == false) {//first time enter in the final pixel delivery part, convert mouse left click cursorX,cursorY to large map position for only once
								  //you cannot update mouse absolute large map position every time since startR/C will update(move) then the mouse + changed startR/C means mouse click moves
//									MouseY = current_map.startR + cursorY;//must use absolute coordinate in the large map to compare not the window coordinate, so first convert mouse left click to the real large map coordinates
//									MouseX = current_map.startC + cursorX;//if you don't use large map position, you will never get the final point since player's windows coordinates is always the same(center) except to four borders.
//							  }

							  if ((MouseY == sprites[0]->getPosition()->getY()) && (MouseX == sprites[0]->getPosition()->getX())) { //if there is no difference between player and mouse left click point then reach the true final pixel termination delete astar last node, shut down the animation door(p!=nullptr-->p==nullptr)
								  FinalPixelDelivery = false;//destroy the special key to this final pixel delivery door.
								  delete p;
								  p = nullptr;
								  return;
							  } else {// there is a problem in this final pixel delivery part, we can enter it first time to get Rd, Cd, but next time inner_c++, inner_c%step!=0, then can not enter any more
									//but this final pixel delivery need compare cursorXY and lastXY every step/frame to calculate a new update Rd, Cd, not the same as normal move which calcalute a Rd/Cd for all the same 12 steps
									//therefore, we need to create a special key to open this and let the program walk around the inner_c%step==0 and directly access to final pixel delivery part.
								  FinalPixelDelivery = true;//create a key
								  if (MouseY - sprites[0]->getPosition()->getY() >= 10) {
									  Rd = 10;
								  } else if (MouseY - sprites[0]->getPosition()->getY() <= -10) {
									  Rd = -10;
								  } else {//MouseY - sprites[0]->getPosition()->getY() > -10 && MouseY - sprites[0]->getPosition()->getY() < 10
									  Rd = MouseY - sprites[0]->getPosition()->getY();
								  }
								  if (MouseX - sprites[0]->getPosition()->getX() >= 10) {
									  Cd = 10;
								  } else if (MouseX - sprites[0]->getPosition()->getX() <= -10 ) {
									  Cd = -10;
								  } else {//MouseX - sprites[0]->getPosition()->getX() > -10 && MouseX - sprites[0]->getPosition()->getX() < 10
									  Cd = MouseX - sprites[0]->getPosition()->getX();
								  }
								  //Rd = abs(MouseY - sprites[0]->getPosition()->getY()) < 10 ? MouseY - sprites[0]->getPosition()->getY() : (MouseY - sprites[0]->getPosition()->getY() > 0 ? 10 : -10);
								  //Cd = abs(MouseX - sprites[0]->getPosition()->getX()) < 10 ? MouseX - sprites[0]->getPosition()->getX() : (MouseX - sprites[0]->getPosition()->getX() > 0 ? 10 : -10);
							  }
						  }
						}
					  //if after moving a frame, the player is still in the background picture's absolute Y coordinate: 300 ~ (height-300) range, (300=600/2) half screen height
					  //update the background starting point at the same time--> moving 2D camera with player(scrolling the map), make player in the center.
					  if((sprites[0]->getPosition()->getY() + Rd)>300 && (sprites[0]->getPosition()->getY() + Rd)<(current_map.map_height*tile_size-300))  
					{
					  current_map.startR += Rd;
					  current_map.endR += Rd;
					}
					  else//if player is moving to the egdes(Y directions: bottom and top) of the background(map), then the map stop scrolling, only moving the player.
					{
					  current_map.screenR += Rd;
					}
 					  //if after moving a frame, the player is still in the background picture's absolute X coordinate: 512 ~ (width-512) range, (512=1024/2) half screen width
					  //update the background starting point at the same time--> moving 2D camera with player(scrolling the map), make player in the center.     
					  if((sprites[0]->getPosition()->getX() + Cd)>512 && (sprites[0]->getPosition()->getX() + Cd)<(current_map.map_width*tile_size-512))  
					{
					  current_map.startC += Cd;
					  current_map.endC += Cd;
					}
					  else//if player is moving to the egdes(X directions: left edge and right edge) of the background(map), then the map stop scrolling, only moving the player.
					{
					  current_map.screenC += Cd;
					}
					  //update player's internal coordinates
					  sprites[0]->getPosition()->moveCardinalDirectionPixel(sprites[0]->getPosition()->getX() + Cd, sprites[0]->getPosition()->getY() + Rd,current_map.mapID);


						  if (Cd>0) {sprites[0]->setCurrentSprite( player_moving[3][swch%3]);sprites[0]->getPosition()->setDir(3);} // move right;
						  else if (Cd<0) {sprites[0]->setCurrentSprite(player_moving[2][swch%3]);sprites[0]->getPosition()->setDir(2);} // move left;
						  else if (Rd >0) {sprites[0]->setCurrentSprite(player_moving[1][swch%3]);sprites[0]->getPosition()->setDir(1);} // move down;
						  else {sprites[0]->setCurrentSprite(player_moving[0][swch%3]);sprites[0]->getPosition()->setDir(0);} // move up
						  swch++;
						  inner_c++;

	  



  
					  if (isAround(300, 7, static_cast<int>(sprites.size()))) // detect enermy; switch to combat mode;
					{
					  mode = 2;//change to the combat mode
					  while(p != nullptr)//clear the rest path steps list
						{
						  nodeptr temp = p;
						  p = p->next;
						  delete temp;
						  temp = nullptr;
						}
					  FinalPixelDelivery = false;
					}

					  if (isAround(100, 4, 7)) // detect NPC, switch to dialogue mode;
					  {
						  mode = 4;//change to the dialogue mode
						 ShowWindow(hwndJornalBT,SW_HIDE);
						 ShowWindow(hwndMapBT,SW_HIDE);
						 ShowWindow(hwndStatusBT,SW_HIDE);
						 ShowWindow(hwndItemListBox,SW_HIDE); 
						 InvalidateRect(hwnd,NULL,true);//clear lower screen picture and buttons

						 //let NPCs turn towards the player by differen direction screnarios
						 if(sprites[0]->getPosition()->getDir()==1)
						 {
							 sprites[4]->setCurrentSprite(goggles_stopping[0][0]);
							 sprites[5]->setCurrentSprite(jint_stopping[0][0]);
							 sprites[6]->setCurrentSprite(police_stopping[0][0]);
						 }
						 else 	 if(sprites[0]->getPosition()->getDir()==0)
						 {
							 sprites[4]->setCurrentSprite(goggles_stopping[1][0]);
							 sprites[5]->setCurrentSprite(jint_stopping[1][0]);
							 sprites[6]->setCurrentSprite(police_stopping[1][0]);
						 }
						 else 	 if(sprites[0]->getPosition()->getDir()==2)
						 {
							 sprites[4]->setCurrentSprite(goggles_stopping[3][0]);
							 sprites[5]->setCurrentSprite(jint_stopping[3][0]);
							 sprites[6]->setCurrentSprite(police_stopping[3][0]);
						 }
						 else 	 if(sprites[0]->getPosition()->getDir()==3)
						 {
							 sprites[4]->setCurrentSprite(goggles_stopping[2][0]);
							 sprites[5]->setCurrentSprite(jint_stopping[2][0]);
							 sprites[6]->setCurrentSprite(police_stopping[2][0]);
						 }

						  while(p != nullptr)//clear the rest unfinished path steps
							{
							  nodeptr temp = p;
							  p = p->next;
							  delete temp;
							  temp = nullptr;
							}
						  //inner_c = 0;
						  FinalPixelDelivery = false;
					  }

					  map_switch();//judge if the player walked in the map switch point
					  if (change_map)//if the map is changed
					  {	  
					  FinalPixelDelivery = false;
					  change_map = false;
					  if (current_map.mapID != 0) {
						  step = 4;
					  } else {
						  step = 12;
					  }
					  while(p != nullptr)//clear the rest path steps
						{
						  nodeptr temp = p;
						  p = p->next;
						  delete temp;
						  temp = nullptr;
						}
						inner_c = 0;
						Rd=0;
						Cd=0;
					  }
	  }
}



void dialogueupdate(HWND hwnd)
{
    if (mouseclick)//everytime mouse left click->set mouseclick=true, then in this dialog update
    {
	mouseclick = false;//set mouseclick=false wait another mouse left click to set it to true to come in again. if no left click, it will not come in this dialogue update.

	if (dindex == 4) // practice moving  dialogue 0-3: welcome and let player to try go around a bit for move control
	{
		//show the exploring mode menu
	 ShowWindow(hwndJornalBT,SW_SHOW);
	 ShowWindow(hwndMapBT,SW_SHOW);
	 ShowWindow(hwndStatusBT,SW_SHOW);
	 ShowWindow(hwndItemListBox,SW_SHOW);  
	    mode = 1;//changing to exploring mode
	    dindex++;//go to dindex=5 next dialogue;

	    // change the location of the main character a little far from trigger place to avoid trigger next dialogue immediately
	    sprites[0]->getPosition()->moveCardinalDirectionPixel(3800, 200,current_map.mapID);
		
		current_map.startC=3288;
		current_map.startR=0;
		current_map.endC=4312;
		current_map.endR=600;
		current_map.screenC=512;
		current_map.screenR=200;
	    
	}
	else if (dindex == 5) 
	{
	 ShowWindow(hwndJornalBT,SW_SHOW);
	 ShowWindow(hwndMapBT,SW_SHOW);
	 ShowWindow(hwndStatusBT,SW_SHOW);
	 ShowWindow(hwndItemListBox,SW_SHOW);  
	    mode = 1;
	    //change the location of tour guide
	    sprites[4]->getPosition()->moveCardinalDirectionPixel(2527, 904,current_map.mapID);
	    dindex = 6;

	    //experience add 2;
		((Player*)sprites[0])->addExperience(2);

	    current_journal =  journal[1];
	
	}
	else if (dindex == 9) //get Dagger and Pistol in dialogue[10]
	{
		dindex++;
	    //add dagger and pistol into item
		Item* dagger=new Item("Dagger",20,567,5,3,5,1);
		Item* pistol=new Item("Pistol",80,234,10,3,10,5);
	    sprites[0]->getInventory()->addItem(dagger);
		sprites[0]->getInventory()->addItem(pistol);
	}
	else if (dindex == 12) // move to arena
	{
	    dindex++;
	    mode = 1;//next enter mode 4 dialogue mode will be in commons arena, it will loading tourguide13.bmp
	 ShowWindow(hwndJornalBT,SW_SHOW);
	 ShowWindow(hwndMapBT,SW_SHOW);
	 ShowWindow(hwndStatusBT,SW_SHOW);
	 ShowWindow(hwndItemListBox,SW_SHOW);  
	     // change the location of tour guide to arena (commons1);
	    sprites[4]->getPosition()->moveCardinalDirectionPixel(1458, 481,current_map.mapID);
	    // change the map for tour guide
		sprites[4]->setMap(4);
	    //experience add 5;
		((Player*)sprites[0])->addExperience(5);
	    current_journal = journal[2];
		targetX=77;
		targetY=487;
	}
	else if (dindex == 13) 
	{
	    dindex = 16;	//first dialoge in arena (dindex=13, tourguide13.bmp), then move to dindex = 16 (tourguide16.bmp)
	}
	else if (dindex == 16) // second dialogue in commons: now try your hand defeating a fecious creature...
	{
	    dindex++;
	    mode = 1;
	 ShowWindow(hwndJornalBT,SW_SHOW);
	 ShowWindow(hwndMapBT,SW_SHOW);
	 ShowWindow(hwndStatusBT,SW_SHOW);
	 ShowWindow(hwndItemListBox,SW_SHOW);  
	     // make the tour guide disappear
		sprites[4]->setAlive(false);
		sprites[6]->setAlive(true);
	    sprites[6]->getPosition()->moveCardinalDirectionPixel(1481, 9857,0);//now the current_map is main_map
	}
	else if (dindex == 19) // 17,18,19, three dialogues with policeman
	{
		ShowWindow(hwndJornalBT, SW_SHOW);
		ShowWindow(hwndMapBT, SW_SHOW);
		ShowWindow(hwndStatusBT, SW_SHOW);
		ShowWindow(hwndItemListBox, SW_SHOW);
	    mode = 1;	//combat mode;
	    dindex = 21;
	    // make the police guard disappear
		sprites[6]->setAlive(false);
	    // place the tour guide near the entrance and change the map for her
		sprites[4]->setMap(0);
	    sprites[4]->getPosition()->moveCardinalDirectionPixel(469, 9918,current_map.mapID);
		sprites[4]->setAlive(true);
		targetX = 30;
		targetY = 470;
	}
	else if (dindex == 21) // move northwest to find chemistry booth
	{
	    dindex++;
	    mode = 1;
	 ShowWindow(hwndJornalBT,SW_SHOW);
	 ShowWindow(hwndMapBT,SW_SHOW);
	 ShowWindow(hwndStatusBT,SW_SHOW);
	 ShowWindow(hwndItemListBox,SW_SHOW);  
	    //change the location of tour guide to chemistry booth;
	    sprites[4]->getPosition()->moveCardinalDirectionPixel(858, 9111,current_map.mapID);
	    //experience add 10;
		((Player*)sprites[0])->addExperience(10);
	    current_journal = journal[3];
		targetX=45;
		targetY=438;
	}
	else if (dindex == 22)
	{

	    dindex = 23;
	    // get two grenades into items
		Item* g1=new Item("Grenades",20,456,23,1,20,5);
		Item* g2=new Item("Grenades",20,500,23,1,20,5);;
	    sprites[0]->getInventory()->addItem(g1);
		sprites[0]->getInventory()->addItem(g2);
	}
	else if (dindex == 27)
	{
	    dindex = 15;
	}
	else if (dindex == 15) dindex = 28;
	else if (dindex == 28)
	{
	    dindex++;
	    mode = 1;
	 ShowWindow(hwndJornalBT,SW_SHOW);
	 ShowWindow(hwndMapBT,SW_SHOW);
	 ShowWindow(hwndStatusBT,SW_SHOW);
	 ShowWindow(hwndItemListBox,SW_SHOW);  
	   // change the location of tour guide to np booth;.
	    sprites[4]->getPosition()->moveCardinalDirectionPixel(2297, 4103,current_map.mapID);
	    //experience add 15;
		((Player*)sprites[0])->addExperience(15);
	    current_journal = journal[4];
		targetX=90;
		targetY=206;
	}
	else if (dindex == 32)
	{
	    dindex = 37;
	    //experience add 20;
		((Player*)sprites[0])->addExperience(20);
	    current_journal = journal[5];
		targetX=187;
		targetY=167;
	}
	else if (dindex == 39)
	{
	    dindex++;
	    mode = 1;
	 ShowWindow(hwndJornalBT,SW_SHOW);
	 ShowWindow(hwndMapBT,SW_SHOW);
	 ShowWindow(hwndStatusBT,SW_SHOW);
	 ShowWindow(hwndItemListBox,SW_SHOW);  
	    //make the tour guide disappear.
		sprites[4]->setAlive(false);
	    //place Jint Clevery in JEB 2end floor.
		sprites[5]->setAlive(true);
		sprites[5]->setMap(2);
	    sprites[5]->getPosition()->moveCardinalDirectionPixel(144, 1586,current_map.mapID);

	}
	else if (dindex == 40)
	{
	    dindex = 14;
	}
	else if (dindex == 14) dindex = 41;
	else if (dindex == 41) dindex = 45;
	else if (dindex == 73) 
	{
	    mode = 1;
	 ShowWindow(hwndJornalBT,SW_SHOW);
	 ShowWindow(hwndMapBT,SW_SHOW);
	 ShowWindow(hwndStatusBT,SW_SHOW);
	 ShowWindow(hwndItemListBox,SW_SHOW);  
	    //make Jint disappear;
		sprites[5]->setAlive(false);
	    //experience add 25;
		((Player*)sprites[0])->addExperience(25);
	    current_journal = journal[6];
		targetX=-40;
		targetY=-40;
	}
	else if ((dindex >= 0 && dindex <=3) || (dindex >= 6 && dindex < 9) || (dindex > 9 && dindex<= 11) ||  (dindex <= 18 && dindex >= 17) || (dindex <= 26 && dindex >= 23) || (dindex <= 31 && dindex >= 29) || dindex == 33 || (dindex <= 38 && dindex >= 37) || (dindex <= 72 && dindex >= 45))
	dindex++;


	current_dialogue = ".//Dialogue//"+dialogue[dindex];
	picture=(HBITMAP)LoadImage((HINSTANCE)NULL,current_dialogue.c_str(),IMAGE_BITMAP,0,0,LR_LOADFROMFILE);
	InvalidateRect(hwnd,NULL,true);
    }
}

int isAround(int d_pixel, int a, int b)
{
  mobs.clear();
  n_mobs = 0;
  int r=0;
  for (int i = a; i < b; i++)
    {
	//if(this character is avile and in the current map
		if(sprites[i]->isAlive()&&sprites[i]->getMap()==current_map.mapID)
		{
      if (sqrt(pow((float)(sprites[i]->getPosition()->getY()-sprites[0]->getPosition()->getY()),2) + pow((float)(sprites[i]->getPosition()->getX()-sprites[0]->getPosition()->getX()),2)) < d_pixel) 
      {
	  if(((NPC*)sprites[i])->isHostile())
	  {
	  mobs.push_back(i);
	  n_mobs++;
	  }
	  r++;
      }

		}
    }
  
  return r;
}


void dialogue_initial()
{
    dialogue[0] = "Introduction.bmp";
    dialogue[1] = "tourguide1.bmp";
    dialogue[2] = "tourguide2.bmp";
    dialogue[3] = "tourguide3.bmp";
    dialogue[4] = "tourguide4.bmp";
    dialogue[5] = "tourguide5.bmp";
    dialogue[6] = "tourguide6.bmp";
    dialogue[7] = "tourguide7.bmp";
    dialogue[8] = "tourguide8.bmp";
    dialogue[9] = "tourguide9.bmp";
    dialogue[10] = "tourguide10.bmp";
    dialogue[11] = "tourguide11.bmp";
    dialogue[12] = "tourguide12.bmp";
    dialogue[13] = "tourguide13.bmp";
    dialogue[14] = "tourguide14.bmp";
    dialogue[15] = "tourguide15.bmp";
    dialogue[16] = "tourguide16.bmp";
    dialogue[17] = "pg1.bmp";
    dialogue[18] = "Information1.bmp";
    dialogue[19] = "pg2.bmp";
    dialogue[20] = "pg3.bmp";
    dialogue[21] = "tourguide17.bmp";
    dialogue[22] = "tourguide18.bmp";
    dialogue[23] = "tourguide19.bmp";
    dialogue[24] = "female1.bmp";
    dialogue[25] = "information0.bmp";
    dialogue[26] = "female2.bmp";
    dialogue[27] = "tourguide20.bmp";
    dialogue[28] = "tourguide21.bmp";
    dialogue[29] = "tourguide22.bmp";
    dialogue[30] = "tourguide23.bmp";
    dialogue[31] = "tourguide24.bmp";
    dialogue[32] = "female3.bmp";
    dialogue[33] = "pg4.bmp";
    dialogue[34] = "information1.bmp";
    dialogue[35] = "pg5.bmp";
    dialogue[36] = "pg6.bmp";
    dialogue[37] = "tourguide25.bmp";
    dialogue[38] = "tourguide26.bmp";
    dialogue[39] = "tourguide27.bmp";
    dialogue[40] = "jc1.bmp";
    dialogue[41] = "jc2.bmp";
    dialogue[42] = "jc3.bmp";
    dialogue[43] = "jc4.bmp";
    dialogue[44] = "jc5.bmp";
    dialogue[45] = "Transition.bmp";
    dialogue[46] = "jc6.bmp";
    dialogue[47] = "jc7.bmp";
    dialogue[48] = "female_engineer1.bmp";
    dialogue[49] = "jc8.bmp";
    dialogue[50] = "jc9.bmp";
    dialogue[51] = "male_guardsman1.bmp";
    dialogue[52] = "jc10.bmp";
    dialogue[53] = "male_np1.bmp";
    dialogue[54] = "male_chemist1.bmp";
    dialogue[55] = "jc11.bmp";
    dialogue[56] = "jc12.bmp";
    dialogue[57] = "jc13.bmp";
    dialogue[58] = "male_np2.bmp";
    dialogue[59] = "jc14.bmp";
    dialogue[60] = "jc15.bmp";
    dialogue[61] = "male_guardsman2.bmp";
    dialogue[62] = "female_engineer2.bmp";
    dialogue[63] = "male_guardsman3.bmp";
    dialogue[64] = "jc16.bmp";
    dialogue[65] = "jc17.bmp";
    dialogue[66] = "jc18.bmp";
    dialogue[67] = "male_chemist2.bmp";
    dialogue[68] = "jc19.bmp";
    dialogue[69] = "jc20.bmp";
    dialogue[70] = "jc21.bmp";
    dialogue[71] = "jc22.bmp";
    dialogue[72] = "jc23.bmp";
    dialogue[73] = "freemode.bmp";
    dialogue[74] = "";
    dialogue[75] = "";
    dialogue[76] = "";
    dialogue[77] = "";
    dialogue[78] = "";

    journal[0] = hbitmap[224];
    journal[1] = hbitmap[225];
    journal[2] = hbitmap[226];
    journal[3] = hbitmap[227];
    journal[4] = hbitmap[228];
    journal[5] = hbitmap[229];
    journal[6] = hbitmap[230];
    
    mouseclick = true;
    dindex = 0;
    current_dialogue = ".//Dialogue//"+dialogue[0];
    current_journal = journal[0];
}

void initial()
{

	gameTimer.Reset();
	animationTimer.Reset();
	fpsTimer.Reset();
 	//currentPlayer=0;
	srand (static_cast<unsigned int>(time((time_t)NULL)));
	step=12;
	inner_c = 0;
	swch = 0;
	mode=0;//starting animation: background introduction
	dialogue_initial(); 
	initializeinfo();
	initializemap(main_map);
	current_map = main_map;
  
  string namepool[]={"Lewis K. Mack",
		     "Julius Petrel Gilligan Jr.",
		     "H. Drassilis Gerenraip",
		     "ElishaEmmett Conneeny Gregson",
		     "E. W. Cosgriff III",
		     "Michael Sy Naghton Jr.",
		     "Buck I. Hardyng III",
		     "Donald G. Whitehill IV",
		     "Gabriel M. Velvetscoter III",
		     "Simeon Earle Fry",
		     "C. Campion Brown Jr.",
		     "Abraham A. Deegan",
		     "Leonard McNamee Hoskyns Jr.",
		     "B. Camin McAnnually",
		     "Levi B. Proud",
		     "G. Freyn Reamsbottom",
		     "Clemence Grinan Proctor Jr.",
		     "Victor Hussey Wotton",
		     "Ora Hefernan Macken",
		     "Alexander T. Callan",
		     "Jeptha Brookes Reddan Jr.",
		     "Arthur Brickleband Remington IV",
		     "Bingley H. Forbes Jr.",
		     "H. Meyer Lancey Jr.",
		     "Jeptha Carden Hayton",
		     "G. R. Jenkins",
		     "I. Nichols Gough",
		     "Elijah Hoban Hickey Jr.",
		     "Lucian Hank Velvetscoter",
		     "Perry T. Egars"
  };


player_active[0][0]=hbitmap[100];
player_active[1][0]=hbitmap[101];
player_active[2][0]=hbitmap[102];
player_active[3][0]=hbitmap[103];

player_dead[0][0]=hbitmap[104];
player_dead[1][0]=hbitmap[105];
player_dead[2][0]=hbitmap[106];
player_dead[3][0]=hbitmap[107];

player_deading[0][0]=hbitmap[108];
player_deading[0][1]=hbitmap[109];
player_deading[1][0]=hbitmap[110];
player_deading[1][1]=hbitmap[111];
player_deading[2][0]=hbitmap[112];
player_deading[2][1]=hbitmap[113];
player_deading[3][0]=hbitmap[114];
player_deading[3][1]=hbitmap[115];

player_grenade[0][0]=hbitmap[116];
player_grenade[0][1]=hbitmap[117];
player_grenade[1][0]=hbitmap[118];
player_grenade[1][1]=hbitmap[119];
player_grenade[2][0]=hbitmap[120];
player_grenade[2][1]=hbitmap[121];
player_grenade[3][0]=hbitmap[122];
player_grenade[3][1]=hbitmap[123];

player_hit[0][0]=hbitmap[124];
player_hit[0][1]=hbitmap[125];
player_hit[1][0]=hbitmap[126];
player_hit[1][1]=hbitmap[127];
player_hit[2][0]=hbitmap[128];
player_hit[2][1]=hbitmap[129];
player_hit[3][0]=hbitmap[130];
player_hit[3][1]=hbitmap[131];

player_stopping[0][0]=hbitmap[132];
player_stopping[1][0]=hbitmap[133];
player_stopping[2][0]=hbitmap[134];
player_stopping[3][0]=hbitmap[135];

player_magic[0][0]=hbitmap[136];
player_magic[1][0]=hbitmap[137];
player_magic[2][0]=hbitmap[138];
player_magic[3][0]=hbitmap[139];

player_pistol[0][0]=hbitmap[140];
player_pistol[0][1]=hbitmap[141];
player_pistol[0][2]=hbitmap[142];
player_pistol[1][0]=hbitmap[143];
player_pistol[1][1]=hbitmap[144];
player_pistol[1][2]=hbitmap[145];
player_pistol[2][0]=hbitmap[146];
player_pistol[2][1]=hbitmap[147];
player_pistol[2][2]=hbitmap[148];
player_pistol[3][0]=hbitmap[149];
player_pistol[3][1]=hbitmap[150];
player_pistol[3][2]=hbitmap[151];

player_attacking[0][0]=hbitmap[152];
player_attacking[0][1]=hbitmap[153];
player_attacking[1][0]=hbitmap[154];
player_attacking[1][1]=hbitmap[155];
player_attacking[2][0]=hbitmap[156];
player_attacking[2][1]=hbitmap[157];
player_attacking[3][0]=hbitmap[158];
player_attacking[3][1]=hbitmap[159];

player_moving[0][0]=hbitmap[160];
player_moving[0][1]=hbitmap[161];
player_moving[0][2]=hbitmap[162];
player_moving[1][0]=hbitmap[163];
player_moving[1][1]=hbitmap[164];
player_moving[1][2]=hbitmap[165];
player_moving[2][0]=hbitmap[166];
player_moving[2][1]=hbitmap[167];
player_moving[2][2]=hbitmap[168];
player_moving[3][0]=hbitmap[169];
player_moving[3][1]=hbitmap[170];
player_moving[3][2]=hbitmap[171];



mob_attacking[0][0]=hbitmap[172];
mob_attacking[0][1]=hbitmap[173];
mob_attacking[1][0]=hbitmap[174];
mob_attacking[1][1]=hbitmap[175];
mob_attacking[2][0]=hbitmap[176];
mob_attacking[2][1]=hbitmap[177];
mob_attacking[3][0]=hbitmap[178];
mob_attacking[3][1]=hbitmap[179];

mob_dead[0][0]=hbitmap[180];
mob_dead[1][0]=hbitmap[181];
mob_dead[2][0]=hbitmap[182];
mob_dead[3][0]=hbitmap[183];

mob_deading[0][0]=hbitmap[184];
mob_deading[0][1]=hbitmap[185];
mob_deading[1][0]=hbitmap[186];
mob_deading[1][1]=hbitmap[187];
mob_deading[2][0]=hbitmap[188];
mob_deading[2][1]=hbitmap[189];
mob_deading[3][0]=hbitmap[190];
mob_deading[3][1]=hbitmap[191];

mob_hit[0][0]=hbitmap[192];
mob_hit[0][1]=hbitmap[193];
mob_hit[1][0]=hbitmap[194];
mob_hit[1][1]=hbitmap[195];
mob_hit[2][0]=hbitmap[196];
mob_hit[2][1]=hbitmap[197];
mob_hit[3][0]=hbitmap[198];
mob_hit[3][1]=hbitmap[199];

mob_stopping[0][0]=hbitmap[200];
mob_stopping[1][0]=hbitmap[201];
mob_stopping[2][0]=hbitmap[202];
mob_stopping[3][0]=hbitmap[203];

mob_moving[0][0]=hbitmap[204];
mob_moving[0][1]=hbitmap[205];
mob_moving[1][0]=hbitmap[206];
mob_moving[1][1]=hbitmap[207];
mob_moving[2][0]=hbitmap[208];
mob_moving[2][1]=hbitmap[209];
mob_moving[3][0]=hbitmap[210];
mob_moving[3][1]=hbitmap[211];




goggles_stopping[0][0]=hbitmap[212];
goggles_stopping[1][0]=hbitmap[213];
goggles_stopping[2][0]=hbitmap[214];
goggles_stopping[3][0]=hbitmap[215];



police_stopping[0][0]=hbitmap[216];
police_stopping[1][0]=hbitmap[217];
police_stopping[2][0]=hbitmap[218];
police_stopping[3][0]=hbitmap[219];



jint_stopping[0][0]=hbitmap[220];
jint_stopping[1][0]=hbitmap[221];
jint_stopping[2][0]=hbitmap[222];
jint_stopping[3][0]=hbitmap[223];
  
  Player* engineer;
  Player* guardsman;
  Player* chemist;
  Player* philosopher;
  engineer=new Player("Engineer",0,4380,180,true);//initialize to main_map (4438,165) absolute pixel coordinates in main_map.bmp
  guardsman=new Player("Guardsman",0,4438,180,false);
  chemist=new Player("Chemist",0,4438,165,false);
  philosopher=new Player("Philosopher",0,4438,180,false);



  NPC* tourguide=new NPC(namepool[rand()%30],false,0,0,4127, 264,goggles_stopping[3][0],goggles_stopping[3][0],true);
  NPC* jint=new NPC("Jint Clevery",false,0,2,144, 1586,jint_stopping[2][0],jint_stopping[2][0],false);
  NPC* police=new NPC(namepool[rand()%30],false,0,0,1481, 9857,police_stopping[1][0],police_stopping[1][0],false);

  NPC* mob1=new NPC(namepool[rand()%30],true,1,0,33, 16,mob_stopping[rand()%4][0],mob_stopping[rand()%4][0],true);
  NPC* mob2=new NPC(namepool[rand()%30],true,1,0,26, 727,mob_stopping[rand()%4][0],mob_stopping[rand()%4][0],true);
  NPC* mob3=new NPC(namepool[rand()%30],true,1,0,600, 241,mob_stopping[rand()%4][0],mob_stopping[rand()%4][0],true);
  NPC* mob4=new NPC(namepool[rand()%30],true,1,0,430, 3360,mob_stopping[rand()%4][0],mob_stopping[rand()%4][0],true);
  NPC* mob5=new NPC(namepool[rand()%30],true,1,0,36, 4790,mob_stopping[rand()%4][0],mob_stopping[rand()%4][0],true);
  NPC* mob6=new NPC(namepool[rand()%30],true,1,0,607, 5801,mob_stopping[rand()%4][0],mob_stopping[rand()%4][0],true);
  NPC* mob7=new NPC(namepool[rand()%30],true,1,0,4125, 9606,mob_stopping[rand()%4][0],mob_stopping[rand()%4][0],true);
  NPC* mob8=new NPC(namepool[rand()%30],true,1,0,2467, 12241,mob_stopping[rand()%4][0],mob_stopping[rand()%4][0],true);
  NPC* mob9=new NPC(namepool[rand()%30],true,1,0,3306, 12273,mob_stopping[rand()%4][0],mob_stopping[rand()%4][0],true);



  



  sprites.push_back(engineer);
  sprites.push_back(guardsman);
  sprites.push_back(chemist);
  sprites.push_back(philosopher);
  sprites.push_back(tourguide);
  sprites.push_back(jint);
  sprites.push_back(police);
  sprites.push_back(mob1);
  sprites.push_back(mob2);
  sprites.push_back(mob3);
  sprites.push_back(mob4);
  sprites.push_back(mob5);
  sprites.push_back(mob6);
  sprites.push_back(mob7);
  sprites.push_back(mob8);
  sprites.push_back(mob9);




  
  //initial current/last mouse click at the player fisrt position
  cursorX = sprites[0]->getPosition()->getX();
  cursorY = sprites[0]->getPosition()->getY();
  temp_cursorX = cursorX;
  temp_cursorY = cursorY;

  current_map.startC=3536;//absolute starting point pixel coordinates for X(Col) in the real large picture bmp map
  current_map.startR=0;//absolute starting point pixel coordinates for Y(Row) in the real large picture bmp map
  current_map.endC=4560;//absolute ending point pixel coordinates for X(Col) in the real large picture bmp map    4560x12600 bmp map.    4560-3560 = 1024, the screen is 1024 width
  current_map.endR=600;//absolute ending point pixel coordinates for Y(Row) in the real large picture bmp map  600 - 0 = 600, the screen is 600 height
  current_map.screenC=878;
  current_map.screenR=165;

  targetX=103;
  targetY=49;
 
}



