#include <Windows.h>
#include <WinGDI.h>
#include <windowsx.h>
#include <Wincodec.h>
#include "Graphics.h"

#include <vector>
#include "pqastar.h"
#include <time.h>
#include "character.h"
#include "core.h"
#include <queue>
#include "timer.h"
#include "game.h"

Timer fpsTimer;
extern int MouseX, MouseY;
NoticeQueue noticequeue;
// Creates a stream object initialized with the data from an executable resource.
IStream * CreateStreamOnResource(int resourceID, LPCSTR lpType)
{
	// initialize return value
	IStream * ipStream = NULL;

	// find the resource
	HRSRC hrsrc = FindResource(NULL,MAKEINTRESOURCE(resourceID), lpType);
	if (hrsrc == NULL)
		goto Return;

	// load the resource
	DWORD dwResourceSize = SizeofResource(NULL, hrsrc);
	HGLOBAL hglbImage = LoadResource(NULL, hrsrc);
	if (hglbImage == NULL)
		goto Return;

	// lock the resource, getting a pointer to its data
	LPVOID pvSourceResourceData = LockResource(hglbImage);
	if (pvSourceResourceData == NULL)
		goto Return;

	// allocate memory to hold the resource data
	HGLOBAL hgblResourceData = GlobalAlloc(GMEM_MOVEABLE, dwResourceSize);
	if (hgblResourceData == NULL)
		goto Return;

	// get a pointer to the allocated memory
	LPVOID pvResourceData = GlobalLock(hgblResourceData);
	if (pvResourceData == NULL)
		goto FreeData;

	// copy the data from the resource to the new memory block
	CopyMemory(pvResourceData, pvSourceResourceData, dwResourceSize);
	GlobalUnlock(hgblResourceData);

	// create a stream on the HGLOBAL containing the data
	if (SUCCEEDED(CreateStreamOnHGlobal(hgblResourceData, TRUE, &ipStream)))
		goto Return;

FreeData:
	// couldn't create stream; free the memory
	GlobalFree(hgblResourceData);

Return:
	// no need to unlock or free the resource
	return ipStream;
}
IWICBitmapSource * LoadBitmapFromStream(IStream * ipImageStream)
{
	// initialize return value
	IWICBitmapSource * ipBitmap = NULL;

	// load WIC's PNG decoder
	IWICBitmapDecoder * ipDecoder = NULL;
	if (FAILED(CoCreateInstance(CLSID_WICPngDecoder, NULL, CLSCTX_INPROC_SERVER, __uuidof(ipDecoder), reinterpret_cast<void**>(&ipDecoder))))
		goto Return;

	// load the PNG
	if (FAILED(ipDecoder->Initialize(ipImageStream, WICDecodeMetadataCacheOnLoad)))
		goto ReleaseDecoder;

	// check for the presence of the first frame in the bitmap
	UINT nFrameCount = 0;
	if (FAILED(ipDecoder->GetFrameCount(&nFrameCount)) || nFrameCount != 1)
		goto ReleaseDecoder;

	// load the first frame (i.e., the image)
	IWICBitmapFrameDecode * ipFrame = NULL;
	if (FAILED(ipDecoder->GetFrame(0, &ipFrame)))
		goto ReleaseDecoder;

	// convert the image to 32bpp BGRA format with pre-multiplied alpha
	//   (it may not be stored in that format natively in the PNG resource,
	//   but we need this format to create the DIB to use on-screen)
	WICConvertBitmapSource(GUID_WICPixelFormat32bppPBGRA, ipFrame, &ipBitmap);
	ipFrame->Release();

ReleaseDecoder:
	ipDecoder->Release();
Return:
	return ipBitmap;
}
// Creates a 32-bit DIB from the specified WIC bitmap.
HBITMAP CreateHBITMAP(IWICBitmapSource * ipBitmap)
{
	// initialize return value
	HBITMAP hbmp = NULL;

	// get image attributes and check for valid image
	UINT width = 0;
	UINT height = 0;
	if (FAILED(ipBitmap->GetSize(&width, &height)) || width == 0 || height == 0)
		goto Return;

	// prepare structure giving bitmap information (negative height indicates a top-down DIB)
	BITMAPINFO bminfo;
	ZeroMemory(&bminfo, sizeof(bminfo));
	bminfo.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	bminfo.bmiHeader.biWidth = width;
	bminfo.bmiHeader.biHeight = -((LONG) height);
	bminfo.bmiHeader.biPlanes = 1;
	bminfo.bmiHeader.biBitCount = 32;
	bminfo.bmiHeader.biCompression = BI_RGB;

	// create a DIB section that can hold the image
	void * pvImageBits = NULL;
	HDC hdcScreen = GetDC(NULL);
	hbmp = CreateDIBSection(hdcScreen, &bminfo, DIB_RGB_COLORS, &pvImageBits, NULL, 0);
	ReleaseDC(NULL, hdcScreen);
	if (hbmp == NULL)
		goto Return;

	// extract the image into the HBITMAP
	const UINT cbStride = width * 4;
	const UINT cbImage = cbStride * height;
	if (FAILED(ipBitmap->CopyPixels(NULL, cbStride, cbImage, static_cast<BYTE *>(pvImageBits))))
	{
		// couldn't extract image; delete HBITMAP
		DeleteObject(hbmp);
		hbmp = NULL;
	}

Return:
	return hbmp;
}
HBITMAP LoadSplashImage(int resourceID)
{
	HBITMAP hbmpSplash = NULL;

	// load the PNG image data into a stream
	IStream * ipImageStream = CreateStreamOnResource(resourceID, "PNG");
	if (ipImageStream == NULL)
		goto Return;

	// load the bitmap with WIC
	IWICBitmapSource * ipBitmap = LoadBitmapFromStream(ipImageStream);
	if (ipBitmap == NULL)
		goto ReleaseStream;

	// create a HBITMAP containing the image
	hbmpSplash = CreateHBITMAP(ipBitmap);
	ipBitmap->Release();

ReleaseStream:
	ipImageStream->Release();
Return:
	return hbmpSplash;
}

//draw a specific pic from (picX,picY) with picW, picH to the screen from (screenX, screenY) with screeW and screenH with double buffer method: strech if two sizes are different
void DrawScreen(HWND hWnd,  HBITMAP pic,int screenX, int screenY, int screenW, int screenH,int picX, int picY,int picW, int picH)
{
	HDC hdcfront=GetDC(hWnd);
	HDC hdcMem=CreateCompatibleDC(hdcfront);
	HDC hdcBitmapBackground=CreateCompatibleDC(hdcfront);
	HBITMAP screen=CreateCompatibleBitmap(hdcfront,ResX,ResY);//build 1024x600 screen bitmap
	HBITMAP oldscreen=(HBITMAP)SelectObject(hdcMem,screen/*background*/);
	HBITMAP oldbackground=(HBITMAP)SelectObject(hdcBitmapBackground,pic);
	StretchBlt(hdcMem,screenX,screenY,screenW,screenH,hdcBitmapBackground,picX,picY,picW,picH,SRCCOPY); //copy new pic to memory buffer (hdcMem)
	BitBlt(hdcfront,screenX,screenY,screenW,screenH,hdcMem,screenX,screenY,SRCCOPY);//copy mem buffer to front screen
	DeleteObject(screen);
	SelectObject(hdcMem,oldscreen);
	DeleteDC(hdcMem);
	SelectObject(hdcBitmapBackground,oldbackground);
	DeleteDC(hdcBitmapBackground);
	ReleaseDC(hWnd,hdcfront);
}



void DrawScreen(HWND hWnd)
{

	//PAINTSTRUCT ps;
	//HDC hdcfront=BeginPaint(hWnd,&ps)

	HBITMAP background = nullptr;
	switch(current_map.mapID)
	{
	case 0:
		background=hbitmap[0];
		break;
	case 1:
		background=hbitmap[10];
		break;
	case 2:
		background=hbitmap[11];
		break;
	case 3:
		background=hbitmap[12];
		break;
	case 4:
		background=hbitmap[9];
		break;
	}

	HDC hdcfront=GetDC(hWnd);
	HDC hdcMem=CreateCompatibleDC(hdcfront);
	HDC hdcBitmapBackground=CreateCompatibleDC(hdcfront);
	BLENDFUNCTION bf;
	bf.BlendOp = AC_SRC_OVER;
	bf.BlendFlags = 0;
	bf.SourceConstantAlpha = 255; //transparency value between 0-255
	bf.AlphaFormat = AC_SRC_ALPHA;    
	HBITMAP screen=CreateCompatibleBitmap(hdcfront,ResX,ResY);
	HBITMAP oldscreen=(HBITMAP)SelectObject(hdcMem,screen/*background*/);
	HBITMAP oldbackground=(HBITMAP)SelectObject(hdcBitmapBackground,background);
	//destStartPoint(0,0)
	//destWidth(current_map.endC-current_map.startC)
	//destHeight(current_map.endR-current_map.startR)
	//srcStartPoint(current_map.startC,current_map.startR)
	//srcWidth(current_map.endC-current_map.startC)
	//srcHeight(current_map.endR-current_map.startR,SRCCOPY)
	StretchBlt(hdcMem,0,0,current_map.endC-current_map.startC,current_map.endR-current_map.startR,hdcBitmapBackground,current_map.startC,current_map.startR,current_map.endC-current_map.startC,current_map.endR-current_map.startR,SRCCOPY);//map smaller than screen?

	for(int i = 0;i < static_cast<int>(sprites.size());i++)
	{
		if(sprites[i]->getMap()==current_map.mapID) //if it is in the current map
		{
			if(sprites[i]->isAlive()==true)//if it is still alive
			{
				//if it is in the current screen
				if(sprites[i]->getPosition()->getX()>=current_map.startC && sprites[i]->getPosition()->getX()<current_map.endC && sprites[i]->getPosition()->getY()>=current_map.startR && sprites[i]->getPosition()->getY()<current_map.endR)
				{
					HDC hdcBitmapSprite=CreateCompatibleDC(hdcfront);
					HBITMAP oldsprites=(HBITMAP)SelectObject(hdcBitmapSprite,sprites[i]->getCurrentSprite());
					BITMAP bitmaptemp;
					GetObject(sprites[i]->getCurrentSprite(),sizeof(bitmaptemp),&bitmaptemp);
					if(current_map.mapID==0) {//outside
						int x = sprites[i]->getPosition()->getX()-current_map.startC-60;
						int y = sprites[i]->getPosition()->getY()-current_map.startR-120;
						if (i == 0) {//player will not half body out of screen since camera is following the player.
							AlphaBlend(hdcMem,(x < 0 ? 0 : x),(y < 0 ? 0 : y),120,120,hdcBitmapSprite,0,0,bitmaptemp.bmWidth,bitmaptemp.bmHeight,bf);
						} else {// other NPC half body can be out of screen, since camera is not following them.
							AlphaBlend(hdcMem,x ,y ,120,120,hdcBitmapSprite,0,0,bitmaptemp.bmWidth,bitmaptemp.bmHeight,bf);
						}
					} else {
						int x = sprites[i]->getPosition()->getX()-current_map.startC-20;
						int y = sprites[i]->getPosition()->getY()-current_map.startR-40;
						if (i == 0) {
							AlphaBlend(hdcMem,(x < 0 ? 0 : x),(y < 0 ? 0 : y),40,40,hdcBitmapSprite,0,0,bitmaptemp.bmWidth,bitmaptemp.bmHeight,bf);
						} else {
							AlphaBlend(hdcMem,x, y, 40,40,hdcBitmapSprite,0,0,bitmaptemp.bmWidth,bitmaptemp.bmHeight,bf);
						}
					}
					SelectObject(hdcBitmapSprite,oldsprites);
					DeleteDC(hdcBitmapSprite);
				}
			}
		}
	}


				if (fpsTimer.ElapsedTime() >= 1.0f) {
					fpsTimer.Reset();
				}
				fpsTimer.Tick();

				RECT textRect;
				char buffer[256];
				SetBkMode(hdcMem, TRANSPARENT);
				SetRect(&textRect, 0, 0, 300, 20);
				sprintf_s(buffer, "%lld %s", fpsTimer.TotalTicks(), "FPS");
				DrawText(hdcMem, buffer, -1, &textRect, DT_LEFT | DT_VCENTER);


				SetRect(&textRect, 0, 20, 400, 40);
				sprintf_s(buffer, "%.2f %s", 1000.0f / fpsTimer.TotalTicks(), "mspf");
				DrawText(hdcMem, buffer, -1, &textRect, DT_LEFT | DT_VCENTER);



				SetRect (&textRect, 0, 40, 400, 60);
				sprintf_s(buffer,"%s: %.2f s","Total Game Time", gameTimer.ElapsedTime());
				DrawText(hdcMem, buffer, -1, &textRect,DT_LEFT| DT_VCENTER);
				/*
				SetRect (&textRect, 0, 60, 400, 80);
				sprintf_s(buffer,"%s: %d, %d","mouse clicked window position",cursorX,cursorY);
				DrawText(hdcMem, buffer, -1, &textRect,DT_LEFT| DT_VCENTER);

				SetRect (&textRect, 0, 90, 400, 110);
				sprintf_s(buffer,"%s: %d, %d","map.screen last screen pixel position",current_map.screenC,current_map.screenR);
				DrawText(hdcMem, buffer, -1, &textRect,DT_LEFT| DT_VCENTER);
				SetRect (&textRect, 0, 110, 400, 130);
				sprintf_s(buffer,"%s: %d, %d","map.screen last screen tile position",current_map.screenC/(sprites[currentPlayer]->getMap() == 0 ? 120 : 40),current_map.screenR/(sprites[currentPlayer]->getMap() == 0 ? 120 : 40));
				DrawText(hdcMem, buffer, -1, &textRect,DT_LEFT| DT_VCENTER);
				SetRect (&textRect, 0, 130, 400, 150);
				sprintf_s(buffer,"%s: %d, %d","player last screen pixel position",sprites[currentPlayer]->getPosition()->getX() - current_map.startC, sprites[currentPlayer]->getPosition()->getY() - current_map.screenR);
				DrawText(hdcMem, buffer, -1, &textRect,DT_LEFT| DT_VCENTER);
				SetRect (&textRect, 0, 150, 400, 170);
				sprintf_s(buffer,"%s: %d, %d","player last screen tile position",(sprites[currentPlayer]->getPosition()->getX() - current_map.startC)/(sprites[currentPlayer]->getMap() == 0 ? 120 : 40),(sprites[currentPlayer]->getPosition()->getY() - current_map.screenR)/(sprites[currentPlayer]->getMap() == 0 ? 120 : 40));
				DrawText(hdcMem, buffer, -1, &textRect,DT_LEFT| DT_VCENTER);
				SetRect (&textRect, 0, 170, 400, 190);
				sprintf_s(buffer,"%s: %d, %d","StartC, StarR",current_map.startC, current_map.startR);
				DrawText(hdcMem, buffer, -1, &textRect,DT_LEFT| DT_VCENTER);*/
				
				if(!noticequeue.empty()) {
					if (!noticequeue.showing()) {
						noticequeue.resetTimer();
						noticequeue.setShowing(true);
					}
					if (noticequeue.getElapsedTime() > 1.0f) {
						noticequeue.pop();
						noticequeue.setShowing(false);
					} else {
							LOGFONT logfont;
							logfont.lfHeight = 40;
							logfont.lfWidth	= 0;
							logfont.lfEscapement = 0;
							logfont.lfOrientation = 0;
							logfont.lfWeight = FW_BOLD;//700
							logfont.lfItalic = FALSE;
							logfont.lfUnderline = FALSE;
							logfont.lfStrikeOut = FALSE;
							logfont.lfCharSet = ANSI_CHARSET;
							logfont.lfOutPrecision = OUT_DEFAULT_PRECIS; //OUT_DEVICE_PRECIS;
							logfont.lfClipPrecision = CLIP_DEFAULT_PRECIS;
							logfont.lfQuality = DEFAULT_QUALITY; //PROOF_QUALITY 
							logfont.lfPitchAndFamily = DEFAULT_PITCH + FF_ROMAN;
							strcpy_s(logfont.lfFaceName, "Edwardian Script ITC");

							HFONT hFont = CreateFontIndirect(&logfont);
							SelectObject(hdcMem, hFont);
							SetTextColor(hdcMem, RGB(204, 50, 50));
							HPEN hPen = CreatePen(PS_SOLID,2,0);
							SelectObject(hdcMem,hPen);
							SetTextAlign(hdcMem,TA_CENTER);
							TextOut(hdcMem,512,550,noticequeue.front().c_str(),static_cast<int>(noticequeue.front().size()));
							SetTextAlign(hdcMem,TA_LEFT);
							DeleteObject(hFont);
							SetTextColor(hdcMem, RGB(0, 0, 0));
					}
				}

	BitBlt(hdcfront,0,0,ResX,ResY,hdcMem,0,0,SRCCOPY);
	DeleteObject(screen);
	SelectObject(hdcMem,oldscreen);
	DeleteDC(hdcMem);
	SelectObject(hdcBitmapBackground,oldbackground);
	DeleteDC(hdcBitmapBackground);

	ReleaseDC(hWnd,hdcfront);
	//EndPaint(hWnd,&ps);

	
}


void DrawMapMode(HWND hWnd, HBITMAP target, HBITMAP player,HBITMAP smallmap,int mapX, int mapY,  int targetX, int targetY,int playerX, int playerY)
{
	HBITMAP background=hbitmap[0];
	HDC hdcfront=GetDC(hWnd);
	HDC hdcMem=CreateCompatibleDC(hdcfront);
	HDC hdcBitmapBackground=CreateCompatibleDC(hdcfront);
	BLENDFUNCTION bf;
	bf.BlendOp = AC_SRC_OVER;
	bf.BlendFlags = 0;
	bf.SourceConstantAlpha = 255; //transparency value between 0-255
	bf.AlphaFormat = AC_SRC_ALPHA;    
	HBITMAP screen=CreateCompatibleBitmap(hdcfront,ResX,ResY);
	HBITMAP oldscreen=(HBITMAP)SelectObject(hdcMem,screen/*background*/);
	HBITMAP oldbackground=(HBITMAP)SelectObject(hdcBitmapBackground,background);
	StretchBlt(hdcMem,0,0,current_map.endC-current_map.startC,current_map.endR-current_map.startR,hdcBitmapBackground,current_map.startC,current_map.startR,current_map.endC-current_map.startC,current_map.endR-current_map.startR,SRCCOPY);


	HDC hdcBitmapSprite=CreateCompatibleDC(hdcfront);
	HBITMAP oldsprites=(HBITMAP)SelectObject(hdcBitmapSprite,smallmap);
	BITMAP bitmaptemp;
	GetObject(smallmap,sizeof(bitmaptemp),&bitmaptemp);
	//AlphaBlend(hdcMem,mapX,mapY,bitmaptemp.bmWidth,bitmaptemp.bmHeight,hdcBitmapSprite,0,0,bitmaptemp.bmWidth,bitmaptemp.bmHeight,bf);
	StretchBlt(hdcMem,mapX,mapY,bitmaptemp.bmWidth,bitmaptemp.bmHeight,hdcBitmapSprite,0,0,bitmaptemp.bmWidth,bitmaptemp.bmHeight,SRCCOPY);
	SelectObject(hdcBitmapSprite,oldsprites);
	DeleteDC(hdcBitmapSprite);

	hdcBitmapSprite=CreateCompatibleDC(hdcfront);
	oldsprites=(HBITMAP)SelectObject(hdcBitmapSprite,target);
	bitmaptemp;
	GetObject(target,sizeof(bitmaptemp),&bitmaptemp);
	AlphaBlend(hdcMem,targetX,targetY,bitmaptemp.bmWidth,bitmaptemp.bmHeight,hdcBitmapSprite,0,0,bitmaptemp.bmWidth,bitmaptemp.bmHeight,bf);
	SelectObject(hdcBitmapSprite,oldsprites);
	DeleteDC(hdcBitmapSprite);

	hdcBitmapSprite=CreateCompatibleDC(hdcfront);
	oldsprites=(HBITMAP)SelectObject(hdcBitmapSprite,player);
	bitmaptemp;
	GetObject(player,sizeof(bitmaptemp),&bitmaptemp);
	AlphaBlend(hdcMem,playerX,playerY,bitmaptemp.bmWidth,bitmaptemp.bmHeight,hdcBitmapSprite,0,0,bitmaptemp.bmWidth,bitmaptemp.bmHeight,bf);
	SelectObject(hdcBitmapSprite,oldsprites);
	DeleteDC(hdcBitmapSprite);


	BitBlt(hdcfront,0,0,ResX,ResY,hdcMem,0,0,SRCCOPY);
	DeleteObject(screen);
	SelectObject(hdcMem,oldscreen);
	DeleteDC(hdcMem);
	SelectObject(hdcBitmapBackground,oldbackground);
	DeleteDC(hdcBitmapBackground);
	ReleaseDC(hWnd,hdcfront);

}




void DrawScreen(HWND hWnd,  HBITMAP sprites, HBITMAP background,int spritesX, int spritesY,int backGroundX, int backGroundY)
{
	//PAINTSTRUCT ps;
	//HDC hdcfront=BeginPaint(hWnd,&ps)

	HDC hdcfront=GetDC(hWnd);
	HDC hdcMem=CreateCompatibleDC(hdcfront);
	HDC hdcBitmapSprite=CreateCompatibleDC(hdcfront);
	HDC hdcBitmapBackground=CreateCompatibleDC(hdcfront);
	BLENDFUNCTION bf;
	bf.BlendOp = AC_SRC_OVER;
	bf.BlendFlags = 0;
	bf.SourceConstantAlpha = 255; //transparency value between 0-255
	bf.AlphaFormat = AC_SRC_ALPHA;    
	HBITMAP screen=CreateCompatibleBitmap(hdcfront,ResX,ResY);
	HBITMAP oldscreen=(HBITMAP)SelectObject(hdcMem,screen/*background*/);
	HBITMAP oldbackground=(HBITMAP)SelectObject(hdcBitmapBackground,background);
	StretchBlt(hdcMem,0,0,ResX,ResY,hdcBitmapBackground,backGroundX,backGroundY,ResX,ResY,SRCCOPY);
	HBITMAP oldsprites=(HBITMAP)SelectObject(hdcBitmapSprite,sprites);
	BITMAP bitmaptemp;
	GetObject(sprites,sizeof(bitmaptemp),&bitmaptemp);
	AlphaBlend(hdcMem,spritesX,spritesY,bitmaptemp.bmWidth,bitmaptemp.bmHeight,hdcBitmapSprite,0,0,bitmaptemp.bmWidth,bitmaptemp.bmHeight,bf);
	BitBlt(hdcfront,0,0,ResX,ResY,hdcMem,0,0,SRCCOPY);
	DeleteObject(screen);
	SelectObject(hdcMem,oldscreen);
	DeleteDC(hdcMem);
	SelectObject(hdcBitmapBackground,oldbackground);
	DeleteDC(hdcBitmapBackground);
	SelectObject(hdcBitmapSprite,oldsprites);
	DeleteDC(hdcBitmapSprite);
	ReleaseDC(hWnd,hdcfront);
	//EndPaint(hWnd,&ps);

	
}

void ShowStat(HWND hWnd)
{
	HBITMAP pic = hbitmap[25]; int screenX = 0; int screenY = 0; int screenW = 1024; int screenH = 600; int picX = 0; int picY = 0; int picW = 1023; int picH = 599;
	HDC hdcfront=GetDC(hWnd);
	HDC hdcMem=CreateCompatibleDC(hdcfront);
	HDC hdcBitmapBackground=CreateCompatibleDC(hdcfront);
	HBITMAP screen=CreateCompatibleBitmap(hdcfront,ResX,ResY);//build 1024x600 screen bitmap
	HBITMAP oldscreen=(HBITMAP)SelectObject(hdcMem,screen/*background*/);
	HBITMAP oldbackground=(HBITMAP)SelectObject(hdcBitmapBackground,pic);
	StretchBlt(hdcMem,screenX,screenY,screenW,screenH,hdcBitmapBackground,picX,picY,picW,picH,SRCCOPY); //copy new pic to memory buffer (hdcMem)//put background first

	//then put text to hdMem:
	LOGFONT logfont;
	logfont.lfHeight = 40;
	logfont.lfWidth	= 0;
	logfont.lfEscapement = 0;
	logfont.lfOrientation = 0;
	logfont.lfWeight = FW_BOLD;//700
	logfont.lfItalic = FALSE;
	logfont.lfUnderline = FALSE;
	logfont.lfStrikeOut = FALSE;
	logfont.lfCharSet = ANSI_CHARSET;
	logfont.lfOutPrecision = OUT_DEFAULT_PRECIS; //OUT_DEVICE_PRECIS;
	logfont.lfClipPrecision = CLIP_DEFAULT_PRECIS;
	logfont.lfQuality = DEFAULT_QUALITY; //PROOF_QUALITY 
	logfont.lfPitchAndFamily = DEFAULT_PITCH + FF_ROMAN;
	strcpy_s(logfont.lfFaceName, "Edwardian Script ITC");

	HFONT hFont = CreateFontIndirect(&logfont);
	SelectObject(hdcMem, hFont);
	SetTextColor(hdcMem, RGB(204, 50, 50));
	HPEN hPen = CreatePen(PS_SOLID,2,0);
	SelectObject(hdcMem,hPen);
	//BeginPath(hdcMem);
	RECT textRect;
	SetRect (&textRect, 176, 285, 366, 303);
	SetBkMode(hdcMem, TRANSPARENT);
	char buffer[256];
	sprintf_s(buffer,"%d/%d",sprites[currentPlayer]->getCurrentHP(),sprites[currentPlayer]->getMajor()->getHealth());
	//DrawText(hdcMem, buffer, -1, &textRect,DT_LEFT| DT_VCENTER);
	TextOut( hdcMem,176,273, buffer,static_cast<int>(strlen(buffer)));
	SetRect (&textRect, 203, 320, 394, 336);
	sprintf_s(buffer,"%d",sprites[currentPlayer]->getLevel());
	TextOut( hdcMem,203,310, buffer, static_cast<int>(strlen(buffer)));
	//DrawText( hdcMem, buffer, -1, &textRect,DT_LEFT| DT_VCENTER);
	SetRect (&textRect, 278, 361, 398, 383);
	sprintf_s(buffer,"%d",((Player*)sprites[currentPlayer])->getExperience());
	//DrawText( hdcMem, buffer, -1, &textRect,DT_LEFT| DT_VCENTER);
	TextOut( hdcMem,278,349, buffer, static_cast<int>(strlen(buffer)));
	SetRect (&textRect, 248, 396, 420, 415);
	sprintf_s(buffer,"%d",sprites[currentPlayer]->getMajor()->getDexterity());
	//DrawText( hdcMem, buffer, -1, &textRect,DT_LEFT| DT_VCENTER);
	TextOut( hdcMem,248,384, buffer, static_cast<int>(strlen(buffer)));
	SetRect (&textRect, 268, 432, 420, 456);
	sprintf_s(buffer,"%d",sprites[currentPlayer]->getMajor()->getWillPower());
	//DrawText( hdcMem, buffer, -1, &textRect,DT_LEFT| DT_VCENTER);
	TextOut( hdcMem,268,420, buffer, static_cast<int>(strlen(buffer)));
	SetRect (&textRect, 273, 470, 420, 489);
	sprintf_s(buffer,"%d",sprites[currentPlayer]->getMajor()->getEndurance());
	//DrawText( hdcMem, buffer, -1, &textRect,DT_LEFT| DT_VCENTER);
	TextOut( hdcMem,273,458, buffer, static_cast<int>(strlen(buffer)));
	SetRect (&textRect, 243, 506, 420, 530);
	sprintf_s(buffer,"%d",sprites[currentPlayer]->getMajor()->getWeaponSkill());
	//DrawText( hdcMem, buffer, -1, &textRect,DT_LEFT| DT_VCENTER);
	TextOut( hdcMem,243,494, buffer, static_cast<int>(strlen(buffer)));

	//EndPath(hdcMem);
	DeleteObject(hFont);
	SetTextColor(hdcMem, RGB(0,0,0));


	BitBlt(hdcfront,screenX,screenY,screenW,screenH,hdcMem,screenX,screenY,SRCCOPY);//copy mem buffer to front screen
	DeleteObject(screen);
	SelectObject(hdcMem,oldscreen);
	DeleteDC(hdcMem);
	SelectObject(hdcBitmapBackground,oldbackground);
	DeleteDC(hdcBitmapBackground);
	ReleaseDC(hWnd,hdcfront);
}

/*
void shadeAvailable(HWND hWnd,vector<Position> tile)
{
	HBITMAP background=hbitmap[0];
	HDC hdcfront=GetDC(hWnd);
	HDC hdcMem=CreateCompatibleDC(hdcfront);
	HDC hdcBitmapBackground=CreateCompatibleDC(hdcfront);
	BLENDFUNCTION bf;
	bf.BlendOp = AC_SRC_OVER;
	bf.BlendFlags = 0;
	bf.SourceConstantAlpha = 255; //transparency value between 0-255
	bf.AlphaFormat = AC_SRC_ALPHA;    
	HBITMAP screen=CreateCompatibleBitmap(hdcfront,ResX,ResY);
	HBITMAP oldscreen=(HBITMAP)SelectObject(hdcMem,screen);
	HBITMAP oldbackground=(HBITMAP)SelectObject(hdcBitmapBackground,background);
	StretchBlt(hdcMem,0,0,current_map.endC-current_map.startC,current_map.endR-current_map.startR,hdcBitmapBackground,current_map.startC,current_map.startR,current_map.endC-current_map.startC,current_map.endR-current_map.startR,SRCCOPY);//map smaller than screen?

	for(int i=0;i<sprites.size();i++)
	{
		if(sprites[i]->getMap()==current_map.mapID) //if it is in the current map
		{
			if(sprites[i]->isAlive()==true)//if it is still alive
			{
				//if it is in thecurrent screen
				if(sprites[i]->getPosition()->getX()>=current_map.startC && sprites[i]->getPosition()->getX()<current_map.endC && sprites[i]->getPosition()->getY()>=current_map.startR && sprites[i]->getPosition()->getY()<current_map.endR)
				{
					HDC hdcBitmapSprite=CreateCompatibleDC(hdcfront);
					HBITMAP oldsprites=(HBITMAP)SelectObject(hdcBitmapSprite,sprites[i]->getCurrentSprite());
					BITMAP bitmaptemp;
					GetObject(sprites[i]->getCurrentSprite(),sizeof(bitmaptemp),&bitmaptemp);
					if(current_map.mapID==0)//outside
						AlphaBlend(hdcMem,sprites[i]->getPosition()->getX()-current_map.startC,sprites[i]->getPosition()->getY()-current_map.startR,120,120,hdcBitmapSprite,0,0,bitmaptemp.bmWidth,bitmaptemp.bmHeight,bf);
					else
						AlphaBlend(hdcMem,sprites[i]->getPosition()->getX()-current_map.startC,sprites[i]->getPosition()->getY()-current_map.startR,40,40,hdcBitmapSprite,0,0,bitmaptemp.bmWidth,bitmaptemp.bmHeight,bf);
					SelectObject(hdcBitmapSprite,oldsprites);
					DeleteDC(hdcBitmapSprite);
				}
			}
		}
	}

	for(int i=0;i<tile.size();i++)
	{
		if(tile[i].getX()>=current_map.startC && tile[i].getX()<current_map.endC && tile[i].getY()>=current_map.startR && tile[i].getY()<current_map.endR)
		{
					HDC hdcBitmapSprite=CreateCompatibleDC(hdcfront);
					HBITMAP oldsprites=(HBITMAP)SelectObject(hdcBitmapSprite,tilepic);
					BITMAP bitmaptemp;
					GetObject(tilepic,sizeof(bitmaptemp),&bitmaptemp);
					AlphaBlend(hdcMem,tile[i].getX()-current_map.startC,tile[i].getY()-current_map.startR,120,120,hdcBitmapSprite,0,0,bitmaptemp.bmWidth,bitmaptemp.bmHeight,bf);
					SelectObject(hdcBitmapSprite,oldsprites);
					DeleteDC(hdcBitmapSprite);
		}
	}
	BitBlt(hdcfront,0,0,ResX,ResY,hdcMem,0,0,SRCCOPY);
	DeleteObject(screen);
	SelectObject(hdcMem,oldscreen);
	DeleteDC(hdcMem);
	SelectObject(hdcBitmapBackground,oldbackground);
	DeleteDC(hdcBitmapBackground);

	ReleaseDC(hWnd,hdcfront);

}*/