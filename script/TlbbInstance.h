#ifndef TLBB_INSTANCE_H
#define TLBB_INSTANCE_H

class TlbbInstance
{
public:
	void SendKeyPress(HWND hWnd, int code);
	COLORREF GetColor(HWND hWnd, HDC hDC, int x, int y);

	enum OBJECT_COLOR
	{
		INVALID_OBJECT, RED_OBJECT, GREEN_OBJECT, WHITE_OBJECT
	};

	BOOL IsFullLife();
	BOOL IsSelfFullLife();
	BOOL IsGamerFullLife(int id);
	BOOL IsAlive();
	OBJECT_COLOR GetObjectColor();
	BOOL IsRed(COLORREF cr);
	BOOL IsGreen(COLORREF cr);
	BOOL IsWhite(COLORREF cr);
	BOOL IsGamer();
	BOOL IsBoss();
	void LeftClick(int x, int y);
	BOOL IsRedRed(COLORREF cr);
	BOOL IsBlack(COLORREF cr);
	BOOL IsBlue(COLORREF cr);
	BOOL IsYellow(COLORREF cr);
protected:
	HWND m_hWnd;
	HDC m_hDC;
};

#endif