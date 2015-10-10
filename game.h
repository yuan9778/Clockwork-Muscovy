#ifndef GAME_H
#define GAME_H

#define ResX 1024 //game screen width is 1024 pixel
#define ResY 600 //game screen height is 600 pixel

#define ATTACKBUTTON 1001
#define ITEMLISTBOX 1002
#define MOVEBUTTON  1003
#define USEITEMBUTTON 1004
#define USESKILLBUTTON 1005
#define SKILLDROPBOX 1006
#define JOURNALBUTTON 1007
#define MAPBUTTON 1008
#define STATUSBUTTON 1009

//prototype for main.cpp
LRESULT CALLBACK WndProc (HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK ChildWndProc (HWND, UINT, WPARAM, LPARAM);


/*****   shared global variable **********/
//these bitmap variables are defined in core.cpp. The following are only declaration for other cpp files using these.
extern HBITMAP hbitmap[300];
extern HBITMAP player_stopping[4][1];
extern HBITMAP player_moving[4][3];
extern HBITMAP player_attacking[4][2];
extern HBITMAP player_deading[4][2];
extern HBITMAP player_dead[4][1];
extern HBITMAP player_magic[4][1];
extern HBITMAP player_pistol[4][3];
extern HBITMAP player_hit[4][2];
extern HBITMAP player_active[4][1];
extern HBITMAP player_grenade[4][2];


extern HBITMAP mob_stopping[4][1];
extern HBITMAP mob_moving[4][2];
extern HBITMAP mob_attacking[4][2];
extern HBITMAP mob_deading[4][2];
extern HBITMAP mob_dead[4][1];
extern HBITMAP mob_hit[4][2];

extern HBITMAP goggles_stopping[4][1];
extern HBITMAP police_stopping[4][1];
extern HBITMAP jint_stopping[4][1];
extern HBITMAP picture;
extern vector<Character*> sprites;
extern HBITMAP current_journal;

//itemindex is defined in battle.cpp. this is declaration for main.cpp also using it.
extern int itemindex;
//itemindex is defined in battle.cpp. this is declaration for main.cpp also using it.
extern int skillindex;
//currentPlayer is defined in battle.cpp. this is declararting for core.cpp also using it.
extern int currentPlayer;
//temp_cursorX, temp_cursorY are defined in battle.cpp. this is declararting for core.cpp also using it.
extern int  temp_cursorX, temp_cursorY;
//current_map is defined in core.cpp, and this is declaration for graphic.cpp, main.cpp also using it.
extern struct map_info current_map;

//mobs is defined in battle.cpp, and this is declaration for core.cpp also using it.
extern vector<int> mobs;

//mobs is defined in battle.cpp, and this is declaration for core.cpp also using it.
extern int n_mobs;

//screen is defined in core.cpp, and this is declaration for battle.cpp, graphics.cpp, astar.cpp also using it.
extern string** screen;


//map is defined in core.cpp and this is declaration for battle.cpp, astar.cpp also using it.
extern char** map;
//mode is defined in core.cpp and this is declaration for battle.cpp, main.cpp also using it.
extern int mode;
//cursorX, cursorY are defined in core.cpp and this is declaration for main.cpp, battle.cpp also using it.
extern int cursorX,cursorY;
//targetX,targetY are defined in core.cpp and this is declaration for graphic.cpp, main.cpp also using it.
extern int targetX,targetY;
//mouseclick is defined in core.cpp and this is declaration for main.cpp also using it.
extern bool mouseclick;

//hwnd/hChild is defined in main.cpp and this is declaration for core.cpp also using it.
extern HWND  hwnd; // main window handle
extern HWND hChild;
//these are defined in main.cpp and this is declaration for the future battle.cpp or core.cpp using them.
extern HWND  hwndAttckBT;     // Handle to ATTACK button
extern HWND  hwndMoveBT;		// Handle to Move button
extern HWND  hwndUseItemBT;	// Handle to use item button
extern HWND  hwndUseSkillBT;	// Handle to use skill button

//hwndItemListBox is defined in main.cpp and this is declaration for battle.cpp, core.cpp also using it.
extern HWND  hwndItemListBox;  // Handle to Item list box
//hwndJornalBT is defined in main.cpp and this is declaration for battle.cpp, core.cpp also using it.
extern HWND  hwndJornalBT;		// Handle to Jornal button
//hwndSkillDropBox is defined in main.cpp and this is declaration for battle.cpp also using it.
extern HWND  hwndSkillDropBox; // Handle to Skill drop box
//hwndMapBT is defined in main.cpp and this is declaration for core.cpp also using it.
extern HWND  hwndMapBT;		// Handle to Map button
//hwndStatusBT is defined in main.cpp and this is declaration for core.cpp also using it.
extern HWND  hwndStatusBT;	// Handle to Status Button

//lastclock is defined in main.cpp and this is declaration for core.cpp also using it.
extern int MouseX, MouseY;
//gameTimer is defined in core.cpp and this is declaration for all other cpp using it.
extern Timer gameTimer;//Record the total game time. It is defined in main.cpp. This is the declaration, core.cpp also using it in initial function to reset the timer
extern Timer animationTimer; //for control animation speed. It is defined in core.cpp
extern Timer noticeTimer; //control notice displaying speed. It is defined Graphics.cpp
extern Timer fpsTimer;//for showing FPS of the game. It is defined in Graphics.cpp This is the declaration, core.cpp also using it in initial function to reset the timer
class NoticeQueue {
	public:
		NoticeQueue() { noticeTimer.Reset(); bShowing = false;}
		~NoticeQueue() {}
		double getElapsedTime() {return noticeTimer.ElapsedTime();}
		void resetTimer() { noticeTimer.Reset(); }
		void push(std::string notice) {q.push(notice);}
		std::string front() {return q.front();}
		void pop() {q.pop();}
		bool empty() {return q.empty();}
		bool showing() { return bShowing; }
		void setShowing(bool s) { bShowing = s; }
	private:
		std::queue<std::string> q;
		Timer noticeTimer;
		bool bShowing;
};
// notification, notice noticeclock are defined in graphic.cpp and this is declaration for character.cpp also using it.
extern NoticeQueue noticequeue;
extern queue<pair<int, int>> clickqueue;
#endif