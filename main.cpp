#include <cmath>
#include <ctime>
#include <vector>
#include <queue>
#include <string>
#include <Windows.h>
#include <windowsx.h>
#include <Wincodec.h>
#include <fstream>
#include "character.h"
#include "pqastar.h"
#include "timer.h"
#include "game.h"
#include "Graphics.h"
#include "core.h"
#include "resource.h"
using namespace std;


#pragma comment(lib, "WindowsCodecs.lib")
#pragma comment(lib,"Msimg32.lib")

//========main.cpp shared global variables definitions start==============//
HWND  hwnd; // main window handle
HWND  hChild;//game screen window handl
HWND  hwndAttckBT;     // Handle to ATTACK button
HWND  hwndMoveBT;		// Handle to Move button
HWND  hwndUseItemBT;	// Handle to use item button
HWND  hwndUseSkillBT;	// Handle to use skill button
HWND  hwndItemListBox;  // Handle to Item list box
HWND  hwndJornalBT;		// Handle to Jornal button
HWND  hwndSkillDropBox; // Handle to Skill drop box
HWND  hwndMapBT;		// Handle to Map button
HWND  hwndStatusBT;	// Handle to Status Button
Timer gameTimer;
bool hold;
queue<pair<int, int>> clickqueue;
unsigned long long fps = 0;
double mspf = 0;
unsigned long long frameCnt = 0;
double elapsetime = 0.0f;
//========main.cpp shared global variables definitions end==============//



/**************main.cpp dedicated global variables definitions**************/
int currentX,currentY;
string whichbutton;
int userchoice;
int ycounter=0;
bool jornalclicked;
bool mapclicked;
bool statusclicked;


PAINTSTRUCT ps;

HINSTANCE    pInstance;
HDC          hdc, childDc;

//***********************************
//            WinMain()
//***********************************
int WINAPI WinMain (HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR szCmdLine, int iCmdShow){
	gameTimer.Reset();
	jornalclicked=false;
	mapclicked=false;
	statusclicked=false;

    static char szAppName[] = "Game" ;

    MSG         msg ;
    WNDCLASSEX  wndclass ;
	RECT rect = {0, 0, ResX-1, ResY+168-1};//1024x768   600+168 for the menu
	AdjustWindowRect(&rect, WS_OVERLAPPED|WS_SYSMENU|WS_MINIMIZEBOX, FALSE);


     wndclass.cbSize        = sizeof (wndclass) ;
     wndclass.style         = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
     wndclass.lpfnWndProc   = WndProc ;
     wndclass.cbClsExtra    = 0 ;
     wndclass.cbWndExtra    = sizeof (WORD);
     wndclass.hInstance     = hInstance ;
     wndclass.hIcon         = LoadIcon (NULL, IDI_APPLICATION) ;
     wndclass.hCursor       = LoadCursor (NULL, IDC_ARROW) ;
     wndclass.hbrBackground = (HBRUSH) GetStockObject (WHITE_BRUSH) ;
     wndclass.lpszMenuName  = NULL;
     wndclass.lpszClassName = szAppName ;
     wndclass.hIconSm       = LoadIcon (NULL, IDI_APPLICATION) ;

     RegisterClassEx (&wndclass) ;

     pInstance = hInstance;

		

	hbitmap[0]=(HBITMAP)LoadImage((HINSTANCE)NULL, ".\\Maps\\main_map.bmp",IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
	hbitmap[1]=(HBITMAP)LoadImage((HINSTANCE)NULL, ".\\HUD\\CharBar1024Item.bmp",IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
	hbitmap[2]=(HBITMAP)LoadImage((HINSTANCE)NULL, ".\\Buttons\\attack.bmp",IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
	hbitmap[3]=(HBITMAP)LoadImage((HINSTANCE)NULL, ".\\Buttons\\move.bmp",IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
	hbitmap[4]=(HBITMAP)LoadImage((HINSTANCE)NULL, ".\\Buttons\\jornal.bmp",IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
	hbitmap[5]=(HBITMAP)LoadImage((HINSTANCE)NULL, ".\\Buttons\\useitem.bmp",IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
	hbitmap[6]=(HBITMAP)LoadImage((HINSTANCE)NULL, ".\\Buttons\\useskill.bmp",IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
	hbitmap[7]=(HBITMAP)LoadImage((HINSTANCE)NULL, ".\\Buttons\\map.bmp",IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
	hbitmap[8]=(HBITMAP)LoadImage((HINSTANCE)NULL, ".\\Buttons\\status.bmp",IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
	hbitmap[9]=(HBITMAP)LoadImage((HINSTANCE)NULL, ".\\Maps\\commons.bmp",IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
	hbitmap[10]=(HBITMAP)LoadImage((HINSTANCE)NULL, ".\\Maps\\jeb1.bmp",IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
	hbitmap[11]=(HBITMAP)LoadImage((HINSTANCE)NULL, ".\\Maps\\jeb2.bmp",IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
	hbitmap[12]=(HBITMAP)LoadImage((HINSTANCE)NULL, ".\\Maps\\jeb3.bmp",IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
	hbitmap[13]=(HBITMAP)LoadImage((HINSTANCE)NULL, ".\\Buttons\\status.bmp",IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
	hbitmap[14]=(HBITMAP)LoadImage((HINSTANCE)NULL, ".\\Buttons\\status.bmp",IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
	hbitmap[15]=(HBITMAP)LoadImage((HINSTANCE)NULL, ".\\Buttons\\status.bmp",IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
	hbitmap[16]=(HBITMAP)LoadImage((HINSTANCE)NULL, ".\\Buttons\\status.bmp",IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
	hbitmap[17]=(HBITMAP)LoadImage((HINSTANCE)NULL, ".\\Buttons\\status.bmp",IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
	hbitmap[18]=(HBITMAP)LoadImage((HINSTANCE)NULL, ".\\Buttons\\status.bmp",IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
	hbitmap[19]=(HBITMAP)LoadImage((HINSTANCE)NULL, ".\\Buttons\\status.bmp",IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
	hbitmap[20]=(HBITMAP)LoadImage((HINSTANCE)NULL, ".\\Buttons\\status.bmp",IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
	hbitmap[21]=(HBITMAP)LoadImage((HINSTANCE)NULL, ".\\Buttons\\status.bmp",IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
	hbitmap[22]=(HBITMAP)LoadImage((HINSTANCE)NULL, ".\\Buttons\\status.bmp",IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
	hbitmap[23]=(HBITMAP)LoadImage((HINSTANCE)NULL, ".\\Dialogue\\background.bmp",IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
	hbitmap[24]=(HBITMAP)LoadImage((HINSTANCE)NULL, ".\\Dialogue\\help.bmp",IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
	hbitmap[25]=(HBITMAP)LoadImage((HINSTANCE)NULL, ".\\Dialogue\\stat.bmp",IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
	hbitmap[26]=(HBITMAP)LoadImage((HINSTANCE)NULL, ".\\Maps\\map.bmp",IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
	hbitmap[27]=(HBITMAP)LoadImage((HINSTANCE)NULL, ".\\HUD\\CharBar1024.bmp",IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);

	hwnd = CreateWindow (szAppName,
		       "CS428/528 RPG Game Clockwork Muscovy 2D version Ver 5.0", // Window caption
		       WS_OVERLAPPED|WS_SYSMENU|WS_MINIMIZEBOX,
                200,20,
                 1030,788,/*rect.right-rect.left, rect.bottom-rect.top,*/
                NULL, NULL, hInstance, NULL) ;

     ShowWindow (hwnd, iCmdShow) ;
     UpdateWindow (hwnd) ;


	 
	hbitmap[100]=LoadSplashImage(IDB_PNG1);
	hbitmap[101]=LoadSplashImage(IDB_PNG2);
	hbitmap[102]=LoadSplashImage(IDB_PNG3);
	hbitmap[103]=LoadSplashImage(IDB_PNG4);
	hbitmap[104]=LoadSplashImage(IDB_PNG5);
	hbitmap[105]=LoadSplashImage(IDB_PNG6);
	hbitmap[106]=LoadSplashImage(IDB_PNG7);
	hbitmap[107]=LoadSplashImage(IDB_PNG8);
	hbitmap[108]=LoadSplashImage(IDB_PNG9);
	hbitmap[109]=LoadSplashImage(IDB_PNG10);
	hbitmap[110]=LoadSplashImage(IDB_PNG11);
	hbitmap[111]=LoadSplashImage(IDB_PNG12);
	hbitmap[112]=LoadSplashImage(IDB_PNG13);
	hbitmap[113]=LoadSplashImage(IDB_PNG14);
	hbitmap[114]=LoadSplashImage(IDB_PNG15);
	hbitmap[115]=LoadSplashImage(IDB_PNG16);
	hbitmap[116]=LoadSplashImage(IDB_PNG17);
	hbitmap[117]=LoadSplashImage(IDB_PNG18);
	hbitmap[118]=LoadSplashImage(IDB_PNG19);
	hbitmap[119]=LoadSplashImage(IDB_PNG20);
	hbitmap[120]=LoadSplashImage(IDB_PNG21);
	hbitmap[121]=LoadSplashImage(IDB_PNG22);
	hbitmap[122]=LoadSplashImage(IDB_PNG23);
	hbitmap[123]=LoadSplashImage(IDB_PNG24);
	hbitmap[124]=LoadSplashImage(IDB_PNG25);
	hbitmap[125]=LoadSplashImage(IDB_PNG26);
	hbitmap[126]=LoadSplashImage(IDB_PNG27);
	hbitmap[127]=LoadSplashImage(IDB_PNG28);
	hbitmap[128]=LoadSplashImage(IDB_PNG29);
	hbitmap[129]=LoadSplashImage(IDB_PNG30);
	hbitmap[130]=LoadSplashImage(IDB_PNG31);
	hbitmap[131]=LoadSplashImage(IDB_PNG32);
	hbitmap[132]=LoadSplashImage(IDB_PNG33);
	hbitmap[133]=LoadSplashImage(IDB_PNG34);
	hbitmap[134]=LoadSplashImage(IDB_PNG35);
	hbitmap[135]=LoadSplashImage(IDB_PNG36);
	hbitmap[136]=LoadSplashImage(IDB_PNG37);
	hbitmap[137]=LoadSplashImage(IDB_PNG38);
	hbitmap[138]=LoadSplashImage(IDB_PNG39);
	hbitmap[139]=LoadSplashImage(IDB_PNG40);
	hbitmap[140]=LoadSplashImage(IDB_PNG41);
	hbitmap[141]=LoadSplashImage(IDB_PNG42);
	hbitmap[142]=LoadSplashImage(IDB_PNG43);
	hbitmap[143]=LoadSplashImage(IDB_PNG44);
	hbitmap[144]=LoadSplashImage(IDB_PNG45);
	hbitmap[145]=LoadSplashImage(IDB_PNG46);
	hbitmap[146]=LoadSplashImage(IDB_PNG47);
	hbitmap[147]=LoadSplashImage(IDB_PNG48);
	hbitmap[148]=LoadSplashImage(IDB_PNG49);
	hbitmap[149]=LoadSplashImage(IDB_PNG50);
	hbitmap[150]=LoadSplashImage(IDB_PNG51);
	hbitmap[151]=LoadSplashImage(IDB_PNG52);
	hbitmap[152]=LoadSplashImage(IDB_PNG53);
	hbitmap[153]=LoadSplashImage(IDB_PNG54);
	hbitmap[154]=LoadSplashImage(IDB_PNG55);
	hbitmap[155]=LoadSplashImage(IDB_PNG56);
	hbitmap[156]=LoadSplashImage(IDB_PNG57);
	hbitmap[157]=LoadSplashImage(IDB_PNG58);
	hbitmap[158]=LoadSplashImage(IDB_PNG59);
	hbitmap[159]=LoadSplashImage(IDB_PNG60);
	hbitmap[160]=LoadSplashImage(IDB_PNG61);
	hbitmap[161]=LoadSplashImage(IDB_PNG62);
	hbitmap[162]=LoadSplashImage(IDB_PNG63);
	hbitmap[163]=LoadSplashImage(IDB_PNG64);
	hbitmap[164]=LoadSplashImage(IDB_PNG65);
	hbitmap[165]=LoadSplashImage(IDB_PNG66);
	hbitmap[166]=LoadSplashImage(IDB_PNG67);
	hbitmap[167]=LoadSplashImage(IDB_PNG68);
	hbitmap[168]=LoadSplashImage(IDB_PNG69);
	hbitmap[169]=LoadSplashImage(IDB_PNG70);
	hbitmap[170]=LoadSplashImage(IDB_PNG71);
	hbitmap[171]=LoadSplashImage(IDB_PNG72);
	hbitmap[172]=LoadSplashImage(IDB_PNG73);
	hbitmap[173]=LoadSplashImage(IDB_PNG74);
	hbitmap[174]=LoadSplashImage(IDB_PNG75);
	hbitmap[175]=LoadSplashImage(IDB_PNG76);
	hbitmap[176]=LoadSplashImage(IDB_PNG77);
	hbitmap[177]=LoadSplashImage(IDB_PNG78);
	hbitmap[178]=LoadSplashImage(IDB_PNG79);
	hbitmap[179]=LoadSplashImage(IDB_PNG80);
	hbitmap[180]=LoadSplashImage(IDB_PNG81);
	hbitmap[181]=LoadSplashImage(IDB_PNG82);
	hbitmap[182]=LoadSplashImage(IDB_PNG83);
	hbitmap[183]=LoadSplashImage(IDB_PNG84);
	hbitmap[184]=LoadSplashImage(IDB_PNG85);
	hbitmap[185]=LoadSplashImage(IDB_PNG86);
	hbitmap[186]=LoadSplashImage(IDB_PNG87);
	hbitmap[187]=LoadSplashImage(IDB_PNG88);
	hbitmap[188]=LoadSplashImage(IDB_PNG89);
	hbitmap[189]=LoadSplashImage(IDB_PNG90);
	hbitmap[190]=LoadSplashImage(IDB_PNG91);
	hbitmap[191]=LoadSplashImage(IDB_PNG92);
	hbitmap[192]=LoadSplashImage(IDB_PNG93);
	hbitmap[193]=LoadSplashImage(IDB_PNG94);
	hbitmap[194]=LoadSplashImage(IDB_PNG95);
	hbitmap[195]=LoadSplashImage(IDB_PNG96);
	hbitmap[196]=LoadSplashImage(IDB_PNG97);
	hbitmap[197]=LoadSplashImage(IDB_PNG98);
	hbitmap[198]=LoadSplashImage(IDB_PNG99);
	hbitmap[199]=LoadSplashImage(IDB_PNG100);
	hbitmap[200]=LoadSplashImage(IDB_PNG101);
	hbitmap[201]=LoadSplashImage(IDB_PNG102);
	hbitmap[202]=LoadSplashImage(IDB_PNG103);
	hbitmap[203]=LoadSplashImage(IDB_PNG104);
	hbitmap[204]=LoadSplashImage(IDB_PNG105);
	hbitmap[205]=LoadSplashImage(IDB_PNG106);
	hbitmap[206]=LoadSplashImage(IDB_PNG107);
	hbitmap[207]=LoadSplashImage(IDB_PNG108);
	hbitmap[208]=LoadSplashImage(IDB_PNG109);
	hbitmap[209]=LoadSplashImage(IDB_PNG110);
	hbitmap[210]=LoadSplashImage(IDB_PNG111);
	hbitmap[211]=LoadSplashImage(IDB_PNG112);
	hbitmap[212]=LoadSplashImage(IDB_PNG113);
	hbitmap[213]=LoadSplashImage(IDB_PNG114);
	hbitmap[214]=LoadSplashImage(IDB_PNG115);
	hbitmap[215]=LoadSplashImage(IDB_PNG116);
	hbitmap[216]=LoadSplashImage(IDB_PNG117);
	hbitmap[217]=LoadSplashImage(IDB_PNG118);
	hbitmap[218]=LoadSplashImage(IDB_PNG119);
	hbitmap[219]=LoadSplashImage(IDB_PNG120);
	hbitmap[220]=LoadSplashImage(IDB_PNG121);
	hbitmap[221]=LoadSplashImage(IDB_PNG122);
	hbitmap[222]=LoadSplashImage(IDB_PNG123);
	hbitmap[223]=LoadSplashImage(IDB_PNG124);
	hbitmap[224]=LoadSplashImage(IDB_PNG125);
	hbitmap[225]=LoadSplashImage(IDB_PNG126);
	hbitmap[226]=LoadSplashImage(IDB_PNG127);
	hbitmap[227]=LoadSplashImage(IDB_PNG128);
	hbitmap[228]=LoadSplashImage(IDB_PNG129);
	hbitmap[229]=LoadSplashImage(IDB_PNG130);
	hbitmap[230]=LoadSplashImage(IDB_PNG131);
	hbitmap[231]=LoadSplashImage(IDB_PNG132);
	hbitmap[232]=LoadSplashImage(IDB_PNG133);
 initial();	
  while(TRUE)
    {
      // Check to see if any messages are waiting in the queue
      if(PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
	{
	  // translate keystroke messages into the right format
	  TranslateMessage(&msg);
	  // send the message to the WindowProc function
	  DispatchMessage(&msg);
	  // check to see if it's time to quit
	  if(msg.message == WM_QUIT)
	    break;
	}
	  

	 //mode==0: starting animation: background introduction, mode=0 is initialized in initial()

	if (mode == 1)    // exploration mode
	  {
		  explore_update();
		  DrawScreen(hChild);
	   }
	else if (mode == 2)	// combat mode
	  {
	    //battle.BattleLoop(hdc,hero,mob);
		  mode=1;
		  ShowWindow(hwndJornalBT,SW_SHOW);
		  ShowWindow(hwndMapBT,SW_SHOW);
	      ShowWindow(hwndStatusBT,SW_SHOW);
		  ShowWindow(hwndItemListBox,SW_SHOW);  
		  
	  }
	else if (mode == 3)	// in menu
	  {
		  if(whichbutton=="journal")
		  {
			  DrawScreen(hChild,current_journal,hbitmap[0],175,50,current_map.startC,current_map.startR);
		  }
		  else if(whichbutton=="map")
		  {
			  //mapID==1:jeb1.txt      mapID==2:jeb2.txt      mapID==3:jeb3.txt
			  if(current_map.mapID==1||current_map.mapID==2||current_map.mapID==3)
			  {
				  currentX=190;
				  currentY=162;
			  }
			  else if(current_map.mapID==4)//mapID==4:commons.txt
			  {
				  currentX=72;
				  currentY=500;
			  }
			  else//mapID==0:main_map.txt
			  {
				  currentX=sprites[0]->getPosition()->getX()/21;
				  currentY=sprites[0]->getPosition()->getY()/21;
			  }
			  DrawMapMode(hChild,hbitmap[232],hbitmap[231],hbitmap[26],362,0,targetX+362,targetY+0,currentX+362,currentY+0);
		  }
		  else if(whichbutton=="stat")
		  {
			  ShowStat(hChild);
		  }

	    
	  }

	else if (mode == 4) // dialogue mode
	{

		dialogueupdate(hwnd);
		DrawScreen(hChild);

	    
	}







	
   }

return (int)msg.wParam ;
}



//*********************************
//        Windows Procedure
// ********************************
LRESULT CALLBACK WndProc(HWND hwnd, UINT iMsg, WPARAM wParam, LPARAM lParam)
     {
   

     WNDCLASSEX  chiclass ;


   switch (iMsg)
          {
		   case WM_ACTIVATE:
			   if (LOWORD(wParam) == WA_INACTIVE)
			   {
				   gameTimer.Pause();
			   }
			   else
			   {
				   gameTimer.Resume();
			   }
			   return 0;
			case WM_CREATE:
			 {
			   hdc = GetDC (hwnd) ;
			   // The system monospaced font is selected
			   SelectObject (hdc, GetStockObject (SYSTEM_FIXED_FONT)) ;




		   	hwndAttckBT=CreateWindow("BUTTON",
			"ATTACK",
			WS_CHILD | BS_BITMAP | BS_PUSHBUTTON,
			300,600+30,
			90,40,
			hwnd,
			(HMENU)ATTACKBUTTON,
			pInstance,
			NULL);
 
			SendMessage(hwndAttckBT,BM_SETIMAGE,IMAGE_BITMAP,(LPARAM)hbitmap[2]); 


		   	hwndMoveBT=CreateWindow("BUTTON",
			"Move",
			WS_CHILD | BS_BITMAP | BS_PUSHBUTTON,
			300,600+95,
			90,40,
			hwnd,
			(HMENU)MOVEBUTTON,
			pInstance,
			NULL);
 
			SendMessage(hwndMoveBT,BM_SETIMAGE,IMAGE_BITMAP,(LPARAM)hbitmap[3]); 



			hwndUseItemBT=CreateWindow("BUTTON",
			"UseItem",
			WS_CHILD | BS_BITMAP | BS_PUSHBUTTON,
			750,600+30,
			150,30,
			hwnd,
			(HMENU)USEITEMBUTTON,
			pInstance,
			NULL);
			SendMessage(hwndUseItemBT,BM_SETIMAGE,IMAGE_BITMAP,(LPARAM)hbitmap[5]); 

			hwndItemListBox=CreateWindow("COMBOBOX",
				"Item",
				//WS_CHILD|LBS_STANDARD/*LBS_NOTIFY|WS_VSCROLL*/,
				CBS_DROPDOWNLIST | WS_VSCROLL| WS_CHILD,
				750,600+100,
				150,150,
				hwnd,
				(HMENU)ITEMLISTBOX,
				pInstance,
				NULL);
			ShowWindow(hwndItemListBox,SW_SHOW); 



			hwndUseSkillBT=CreateWindow("BUTTON",
			"UseSkill",
			WS_CHILD | BS_BITMAP | BS_PUSHBUTTON,
			630,600+95,
			150,30,
			hwnd,
			(HMENU)USEITEMBUTTON,
			pInstance,
			NULL);
			SendMessage(hwndUseSkillBT,BM_SETIMAGE,IMAGE_BITMAP,(LPARAM)hbitmap[6]); 

			hwndSkillDropBox=CreateWindow("COMBOBOX",
				"Skill",
				//WS_CHILD|LBS_STANDARD/*LBS_NOTIFY|WS_VSCROLL*/,
				CBS_DROPDOWNLIST | WS_VSCROLL| WS_CHILD,
				630,730,
				150,150,
				hwnd,
				(HMENU)SKILLDROPBOX,
				pInstance,
				NULL);
			//ShowWindow(hwndSkillDropBox,SW_SHOW);

			hwndJornalBT=CreateWindow("BUTTON",
			"Jornal",
			WS_CHILD | BS_BITMAP | BS_PUSHBUTTON,
			300,600+30,
			90,40,
			hwnd,
			(HMENU)JOURNALBUTTON,
			pInstance,
			NULL);
			SendMessage(hwndJornalBT,BM_SETIMAGE,IMAGE_BITMAP,(LPARAM)hbitmap[4]); 
			ShowWindow(hwndJornalBT,SW_SHOW);

			hwndMapBT=CreateWindow("BUTTON",
			"Map",
			WS_CHILD | BS_BITMAP | BS_PUSHBUTTON,
			300,600+95,
			90,40,
			hwnd,
			(HMENU)MAPBUTTON,
			pInstance,
			NULL);
			SendMessage(hwndMapBT,BM_SETIMAGE,IMAGE_BITMAP,(LPARAM)hbitmap[7]); 
			ShowWindow(hwndMapBT,SW_SHOW);


			hwndStatusBT=CreateWindow("BUTTON",
			"Status",
			WS_CHILD | BS_BITMAP | BS_PUSHBUTTON,
			630,600+30,
			90,40,
			hwnd,
			(HMENU)STATUSBUTTON,
			pInstance,
			NULL);
			SendMessage(hwndStatusBT,BM_SETIMAGE,IMAGE_BITMAP,(LPARAM)hbitmap[8]); 
			ShowWindow(hwndStatusBT,SW_SHOW);

//		    Create a child window
				chiclass.cbSize        = sizeof (chiclass) ;
				chiclass.style         = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
				chiclass.lpfnWndProc   = ChildWndProc ;
				chiclass.cbClsExtra    = 0 ;
				chiclass.cbWndExtra    = 0 ;
				chiclass.hInstance     = pInstance ;
				chiclass.hIcon         = NULL;
				chiclass.hCursor       = LoadCursor (NULL, IDC_ARROW) ;
				chiclass.hbrBackground = (HBRUSH) GetStockObject (WHITE_BRUSH) ;
				chiclass.lpszMenuName  = NULL;
				chiclass.lpszClassName = "ChildWindow" ;
				chiclass.hIconSm       = NULL;

			RegisterClassEx (&chiclass) ;
            hChild = CreateWindow ("ChildWindow",
				  "A Child Window",  // caption
                  WS_CHILD ,
                  //WS_CHILD | WS_VISIBLE | WS_OVERLAPPEDWINDOW ,
                  0, 0,          // x and y of window location
                  ResX, ResY,        // x and y of window size
                  hwnd,            // handle to the parent window 
                  (HMENU) 1001,    // child window designation
                  pInstance,       // program instance
                  NULL) ;

				  

			 return 0 ;
			 }
        case WM_PAINT:
			{
           hdc=BeginPaint (hwnd, &ps) ;
		   if(hChild!=NULL)
		   {
		   UpdateWindow(hChild);
		   ShowWindow(hChild, SW_SHOWNORMAL);
		   }

		   if(mode==1||mode==0)//intro mode or exploring mode: use map
		   {
			
            HDC hdcBanner = CreateCompatibleDC(hdc);
            HBITMAP hbmOldHorizontal = (HBITMAP)SelectObject(hdcBanner,hbitmap[1]); //hbitmap[1]=".\\HUD\\CharBar1024Item.bmp" with inventory fake button lower screen background
            StretchBlt(hdc,0,600,1024,160,hdcBanner,0,0,1021,160,SRCCOPY);
            SelectObject(hdcBanner,hbmOldHorizontal);
            DeleteDC(hdcBanner);
		   }
		   else if(mode==4)//dialogue mode, use picture global variable dynamically determins the dialogue picture
		   {
            HDC hdcBanner = CreateCompatibleDC(hdc);
            HBITMAP hbmOldHorizontal = (HBITMAP)SelectObject(hdcBanner,picture); 
            StretchBlt(hdc,0,600,1024,160,hdcBanner,0,0,784,160,SRCCOPY);
            SelectObject(hdcBanner,hbmOldHorizontal);
            DeleteDC(hdcBanner);
		   }
		   else if(mode==2)// battle mode
		   {
            HDC hdcBanner = CreateCompatibleDC(hdc);
            HBITMAP hbmOldHorizontal = (HBITMAP)SelectObject(hdcBanner,hbitmap[27]); //hbitmap[27]=".\\HUD\\CharBar1024.bmp" no inventory button lower screen background
            StretchBlt(hdc,0,600,1024,160,hdcBanner,0,0,1021,160,SRCCOPY);
            SelectObject(hdcBanner,hbmOldHorizontal);
            DeleteDC(hdcBanner);
		   }
           EndPaint (hwnd, &ps);
		   
           return 0 ;
			}
           case	WM_LBUTTONDOWN:
			   if(mode==4)//dialogue mode click the lower screen to continue to the next page dialogue picture
			   {
				InvalidateRect(hwnd,NULL,true);
				mouseclick=true;
			   }
			   return 0;


		  case WM_COMMAND:
			   {
				   switch (LOWORD(wParam))
				   {
				   case ITEMLISTBOX:
					   {
						   switch(HIWORD(wParam))
						   {
						   case CBN_SELCHANGE:
							   {
								int lbItem = (int)SendMessage(hwndItemListBox,CB_GETCURSEL,0,0);
								itemindex=static_cast<int>(SendMessage(hwndItemListBox,CB_GETITEMDATA,lbItem,0));
								   return TRUE;
							   }
						   }
						   return TRUE;
					   }
				   case SKILLDROPBOX:
					   {
						   switch(HIWORD(wParam))
						   {
						   case CBN_SELCHANGE:
							   {
								int lbItem = (int)SendMessage(hwndSkillDropBox,CB_GETCURSEL,0,0);
								skillindex=static_cast<int>(SendMessage(hwndSkillDropBox,CB_GETITEMDATA,lbItem,0));
								   return TRUE;
							   }
						   }
						   return TRUE;
					   }
				   case ATTACKBUTTON:
					   {
						   switch(HIWORD(wParam))
						   {
						   case BN_CLICKED:
							   userchoice=1;
							   break;
						   }
						   return true;
					   }
				   case MOVEBUTTON:
					   {
						   switch(HIWORD(wParam))
						   {
							   case BN_CLICKED:
							   userchoice=2;
							   break;
						   }
						   return true;
					   }
				   case USEITEMBUTTON:
					   {
						   switch(HIWORD(wParam))
						   {
							   case BN_CLICKED:
							   userchoice=3;
							   break;
						   }
						   return true;
					   }
				   case USESKILLBUTTON:
					   {
						   switch(HIWORD(wParam))
						   {
							   case BN_CLICKED:
							   userchoice=4;
							   break;
						   }
						   return true;
					   }
				   case JOURNALBUTTON:
					   {
						   switch(HIWORD(wParam))
						   {
							   case BN_CLICKED:
								if (mode != 0){//disable the button for intro animation
								   if(jornalclicked==false)//not clicked->jornal is not opened
								   {
									   jornalclicked=true;
									   mapclicked=false;
									   statusclicked=false;
									   mode=3;
									   whichbutton="journal";
								   }
								   else
								   {
									   jornalclicked=false;
									   whichbutton="";
									   mode=1;
								   }
								}
							   break;
						   }
						   return true;
					   }
				   case MAPBUTTON:
					   {
						   switch(HIWORD(wParam))
						   {
							   case BN_CLICKED:
									if (mode != 0){//disable the button for intro animation
										if(mapclicked==false)//not clicked->map is not opened
										{
											mapclicked=true;
											jornalclicked=false;
											statusclicked=false;
											whichbutton="map";
											mode=3;
										}
										else//if map is already open, turn off the map
										{
											mapclicked=false;
											whichbutton="";
											mode=1;
										}
									}
							   break;
						   }
						   return true;
					   }
				   case STATUSBUTTON:
					   {
						   switch(HIWORD(wParam))
						   {
							   case BN_CLICKED:
							if (mode != 0){//disable the button for intro animation
							   if(statusclicked==false)//not clicked->status is not opened
							   {
								   statusclicked=true;
									mapclicked=false;
									jornalclicked=false;								   
								   whichbutton="stat";
								   mode=3;
							   }
							   else
							   {
								   statusclicked=false;
								   whichbutton="";
								   mode=1;
							   }
							}
							   break;
						   }
						   return true;
					   }
				   }
				   return 0;
			   }


         case WM_DESTROY:
          PostQuitMessage (0) ;
           return 0 ;
          }

     return DefWindowProc (hwnd, iMsg, wParam, lParam) ;
}


//**********************************
//    child windows procedure
//**********************************


LRESULT CALLBACK ChildWndProc (HWND hChild, UINT iMsg, WPARAM wParam,LPARAM lParam) 
{

	switch (iMsg) 
	{

	   case WM_CREATE:
          childDc = GetDC(hChild);
          SelectObject (childDc, GetStockObject (SYSTEM_FIXED_FONT)) ;
          return 0;

	   case WM_LBUTTONUP:
		   hold = false;
		   return 0;

	   case WM_MOUSEMOVE:
		   if (GET_X_LPARAM(lParam) < current_map.endC - current_map.startC && GET_Y_LPARAM(lParam) < current_map.endR - current_map.startR) {// if mouse click X < 1024 && Y < 600->mouse could click in the bottom HUD screen which we do not receive it as moving command
			   cursorX = GET_X_LPARAM(lParam);
			   cursorY = GET_Y_LPARAM(lParam);
			   if (hold && clickqueue.empty()) { //if we hold mouse left key, then we help system send WM_LBUTTONDOWN message to notify the new updating cursor position.
				   SendMessage(hChild, WM_LBUTTONDOWN, 0, MAKELPARAM(cursorX, cursorY));//cursorXY never change from last mouse button down, send back to recevie new global large map MouseXY
			   }//check if clickqueue has pre determined path to go, if there are some, then don't send WMLBUTTONDOWN message, or it will destroy all pre record the mouse clicks.
			   //eve you click once, before WM_LBUTTONUP set hold false, there still will be a lot of message of WM_MOUSEMOVE with hold=true come in then caused WM_LBUTTONDOWN message sent destroy the clickqueue
		   }
			return 0;

	   case WM_LBUTTONDOWN:
		   {
			   hold = true;
			   switch(mode)
			   {
			   case 0:
				   {
						if(ycounter>1400) {//intro animation ends, and the waiting mouse left button click has been done on upper screen, switch to the explore mode
							mode=1;
						} else {//skip the intro animation to the game guide help
							ycounter = 1401;
						}
				   }
					break;
			   case 1:
				   {
					   if (wParam & MK_SHIFT) {
						   int localX = GET_X_LPARAM(lParam);
						   int localY = GET_Y_LPARAM(lParam);
						   if ( localX < current_map.endC - current_map.startC && localY < current_map.endR - current_map.startR) {// if mouse click X < 1024 && Y < 600->mouse could click in the bottom HUD screen which we do not receive it as moving command
							   int globalX = current_map.startC + localX;
							   int globalY = current_map.startR + localY;
							   if (current_map.mapID == 0) {
								   globalX = globalX / 120 * 120 + 60;
								   globalY = globalY / 120 * 120 + 60;
							   }
							   else {
								   globalX = globalX / 40 * 40 + 20;
								   globalY = globalY / 40 * 40 + 20;
							   }
							   clickqueue.push(make_pair(globalX, globalY));
								return 0;
						   }
					   }
					   if(GET_X_LPARAM(lParam) < current_map.endC - current_map.startC && GET_Y_LPARAM(lParam) < current_map.endR - current_map.startR)// if mouse click X < 1024 && Y < 600->mouse could click in the bottom HUD screen which we do not receive it as moving command
					   {
						   queue<pair<int, int>> empty;
						   swap(clickqueue, empty); //clear the click queue
						  //cursorX = LOWORD(lParam);
						  //cursorY = HIWORD(lParam);

						  /*
						  Do not use the LOWORD or HIWORD macros to extract the x - and y - coordinates of the cursor position
						  because these macros return incorrect results on systems with multiple monitors.
						  Systems with multiple monitors can have negative x - and y - coordinates, and LOWORD and HIWORD treat the coordinates as unsigned quantities.
						  */

						  cursorX = GET_X_LPARAM(lParam);
						  cursorY = GET_Y_LPARAM(lParam);
						  MouseX = current_map.startC + cursorX;
						  MouseY = current_map.startR + cursorY;
						  if(current_map.mapID == 0) {
							  MouseX = MouseX / 120 * 120 + 60;
							  MouseY = MouseY / 120 * 120 + 60;
						  } else {
							  MouseX = MouseX /40 * 40 + 20;
							  MouseY = MouseY /40 * 40 + 20;
						  }
					   }
				   }
				   break;
			   case 2:
				   {

				   }
				   break;
			   case 4://click upper screen, the dialogue also continue to the next page.
				   {
						InvalidateRect(hwnd,NULL,true);
						mouseclick=true;
				   }
				   break;
			   default:
				   break;
			   }
			   return 0;
		   }
	  case WM_PAINT:
		   {
			if(ycounter<=1400)//intro anitmation not finished: the large picture is 800x2000, each frame shows 800x600 start from (0,0), (0,3), (0,6)... each time shows 800 width and 600 height
			{
			if (animationTimer.ElapsedTime() < 1/60.0) { //1s moving 60 pixels then, each pixel uses 1 / 60.0 second
				return 0;
			} else {
				animationTimer.Reset();
			}
			DrawScreen(hChild,hbitmap[23],0,0,1024,600,0,ycounter,800,600); //hbitmap[23]=".\\Dialogue\\background.bmp"--> it is the intro anitmation about story background
			++ycounter;														//ycounter is the starting coordinates of Y-axis of the large intro background picture:hbitmap[23]
			}
			else//ycounter > 1400 finished intro animation, automatically shows the game guid help
			{
				if(mode==0)//if this is just finished intro animation, show the game guide help
				{
				BeginPaint (hChild, &ps) ;
				HDC hdcBanner = CreateCompatibleDC(childDc);
				HBITMAP hbmOldHorizontal = (HBITMAP)SelectObject(hdcBanner,hbitmap[24]); //hbitmap[24]=".\\Dialogue\\help.bmp"-->game guide
				StretchBlt(childDc,0,0,1024,600,hdcBanner,0,0,1024,600,SRCCOPY);
				SelectObject(hdcBanner,hbmOldHorizontal);
				DeleteObject(hbmOldHorizontal);
				DeleteDC(hdcBanner);
				EndPaint (hChild, &ps);				
				}
			}
			
			InvalidateRect(hChild,NULL,false);
			return 0;
		   }
	   case WM_DESTROY:
          return 0;
	 }
	    return DefWindowProc (hChild, iMsg, wParam, lParam) ;
}













