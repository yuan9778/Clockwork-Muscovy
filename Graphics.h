#ifndef GRAPHICS_H
#define GRAPHICS_H


//HBITMAP hBitmap;
IStream * CreateStreamOnResource(int resourceID, LPCSTR lpType);
IWICBitmapSource * LoadBitmapFromStream(IStream* ipImageStream);
HBITMAP CreateHBITMAP(IWICBitmapSource* ipBitmap);
HBITMAP LoadSplashImage(int resourceID);
void DrawScreen(HWND hWnd,  HBITMAP pic,int screenX, int screenY, int screenW, int screenH,int picX, int picY,int picW, int picH);
void DrawScreen(HWND hWnd);
void DrawScreen(HWND hWnd,  HBITMAP sprites, HBITMAP background,int spritesX, int spritesY,int backGroundX, int backGroundY);
void DrawMapMode(HWND hWnd, HBITMAP target, HBITMAP player,HBITMAP smallmap,int mapX, int mapY,  int targetX, int targetY,int playerX, int playerY);
void ShowStat(HWND hWnd);
//void shadeAvailable(vector<Position>);

#endif
