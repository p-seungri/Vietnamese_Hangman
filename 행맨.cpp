#include <windows.h>
#include "resource.h"
#define BITMAPSIZE 13
#define ALPHABIT 98

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK ChildTopProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK ChildBottomProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK ChildInnerProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
void DrawBitmap(HDC hdc, int x, int y, int w, int h, HBITMAP hbit);

enum ALPHA 
{
    aA = 200, aA2, aA3, aA4, aA5, aA6, aA1_1, aA1_2, aA1_3, aA1_4, aA1_5, aA1_6, aA2_1, aA2_2, aA2_3, aA2_4, aA2_5, aA2_6, aB, aC, aD, aD1, aE, aE2, aE3, aE4, aE5, aE6,
    aE1_1, aE1_2, aE1_3, aE1_4, aE1_5, aE1_6, aF, aG, aH, aI, aI2, aI3, aI4, aI5, aI6, aJ, aK, aL, aM, aN, aO, aO2, aO3, aO4, aO5, aO6, aO1_1, aO1_2, aO1_3, aO1_4, aO1_5, aO1_6,
    aO2_1, aO2_2, aO2_3, aO2_4, aO2_5, aO2_6, aP, aQ, aR, aS, aT, aU, aU2, aU3, aU4, aU5, aU6, aU1_1, aU1_2, aU1_3, aU1_4, aU1_5, aU1_6, aV, aW, aX, aY, aZ, a0, a1, a2, a3, a4, a5, a6, a7, a8, a9
};

HINSTANCE g_hInst;
LPCTSTR lpszClass = TEXT("행맨");
HWND hWndMain, hC1, hC2, hC3;
HBRUSH c1, c2, c3;
TCHAR szQuestion[1024];
BOOL bQuestionText = FALSE;
int nQuestionLen;
TCHAR szAnswer[256];
int Insertion[256] = { aA - aA };
int InsertionOnce[256] = { aA - aA };
HBITMAP newBitmap[BITMAPSIZE];
BOOL bVisible = FALSE;
HBITMAP newAlphaBit[ALPHABIT];
int InsertionCnt = 0;
int AnswerCnt = 0;
int QuestionCnt = 0;
int nFaceStat[3] = { 1, 1, 1 };
int nBodyStat = 1;
int DupliCnt = 0;
int BlankCnt = 0;
BOOL bGameOver = FALSE;
TCHAR Dupli[16];
int DupliPtr = 0;
BOOL ClickCnt[98] = { 0 };

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszCmdParam, int nCmdShow)
{
	HWND hWnd;
	MSG Message;
	WNDCLASS WndClass;
	g_hInst = hInstance;

	WndClass.cbClsExtra = 0;
	WndClass.cbWndExtra = 0;
	WndClass.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	WndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	WndClass.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ICON1));
	WndClass.hInstance = hInstance;
	WndClass.lpfnWndProc = WndProc;
	WndClass.lpszClassName = lpszClass;
    WndClass.lpszMenuName = MAKEINTRESOURCE(IDR_MENU1);
	WndClass.style = CS_HREDRAW | CS_VREDRAW;
	RegisterClass(&WndClass);

	//Child Top
	c1 = CreateSolidBrush(RGB(255, 255, 255));

	WndClass.hbrBackground = c1;
	WndClass.lpfnWndProc = ChildTopProc;
	WndClass.lpszClassName = TEXT("ChildTop");
	RegisterClass(&WndClass);

	//Child Bottom
	c2 = CreateSolidBrush(RGB(255, 252, 128));

	WndClass.hbrBackground = (HBRUSH)GetStockObject(NULL_BRUSH);
	WndClass.lpfnWndProc = ChildBottomProc;
	WndClass.lpszClassName = TEXT("ChildBottom");
	RegisterClass(&WndClass);
	
    //Child Inner Bottom
	c3 = CreateSolidBrush(RGB(240, 240, 240));

    WndClass.hbrBackground = c3;
    WndClass.lpfnWndProc = ChildInnerProc;
    WndClass.lpszClassName = TEXT("ChildInner");
    RegisterClass(&WndClass);


	hWnd = CreateWindow(lpszClass, lpszClass, WS_CAPTION | WS_SYSMENU | WS_BORDER, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, NULL, (HMENU)NULL, hInstance, NULL);
	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);


	while (GetMessage(&Message, NULL, 0, 0)) 
	{
		TranslateMessage(&Message);
		DispatchMessage(&Message);
	}
	return (int)Message.wParam;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam)
{
	HDC hdc;
	PAINTSTRUCT ps;
    RECT crt;
    static RECT center;
    int i = 0;

	switch (iMessage)
    {
	case WM_CREATE:

		hWndMain = hWnd;
        SetRect(&crt, 0, 0, 800, 760);
        AdjustWindowRect(&crt, WS_CAPTION | WS_SYSMENU | WS_BORDER, FALSE);
        SetWindowPos(hWnd, NULL, 0, 0, crt.right - crt.left, crt.bottom - crt.top, SWP_NOMOVE | SWP_NOZORDER);

        hC1 = CreateWindow(TEXT("ChildTop"), NULL, WS_VISIBLE | WS_CHILD | WS_CLIPCHILDREN | WS_BORDER, 0, 0, 0, 0, hWnd, (HMENU)0, g_hInst, NULL);
        hC2 = CreateWindow(TEXT("ChildBottom"), NULL, WS_VISIBLE | WS_CHILD | WS_CLIPCHILDREN | WS_BORDER, 0, 0, 0, 0, hWnd, (HMENU)0, g_hInst, NULL);

        for (i = 0; i < BITMAPSIZE; i++)
        {
            newBitmap[i] = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_BACKGROUND1 + i));
        }

        for (i = 0; i < ALPHABIT; i++)
        {
            newAlphaBit[i] = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_A0_1 + i));
        }

        break;
    case WM_SIZE:
        if (wParam != SIZE_MINIMIZED)
        {
            GetClientRect(hWnd, &center);

            MoveWindow(hC1, 0, 0, center.right, 380, TRUE);
            MoveWindow(hC2, 0, 380, center.right, 380, TRUE);
        }
        break;
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		EndPaint(hWnd, &ps);
        break;
    case WM_COMMAND:
        switch (LOWORD(wParam))
        {
        case ID_HELP_MANUAL:
            MessageBeep(0);
            MessageBox(hWnd, TEXT("Fatal Error occurred!! Your memory might be corrupted"), TEXT("Error"), MB_OK | MB_ICONERROR);
            MessageBox(hWnd, TEXT("는 장난으로 한 번 해봤고, 한 번 더 누르면 진짜 설명 나와"), TEXT("페이크"), MB_OK);
            MessageBox(hWnd, TEXT("1.\'여기에 입력해요\' 밑에 출제할 단어 입력\n2.우하단의 Add 버튼을 눌러 단어 출제\n3.키보드처럼 만들어 놓은 버튼을 이용해서 문제 풀기\n4.성조 및 특문은 비슷한 알파벳 클릭 후 옆에 목록에서 선택\n5.옆에 Add 버튼 눌러서 입력"), TEXT("메뉴얼"), MB_OK);
            break;
        case ID_HELP_TIP:
            MessageBox(hWnd, TEXT("팁 같은 거 없어 혼자 열심히 해"), TEXT("나약하군"), MB_OK | MB_ICONWARNING);
            break;
        case ID_FILE_EXIT:
            DestroyWindow(hWnd);
            break;
        }
        break;       
	case WM_DESTROY:
        for (i = 0; i < BITMAPSIZE; i++)
        {
            DeleteObject(newBitmap[i]);
        }

        for (i = 0; i < ALPHABIT; i++)
        {
            DeleteObject(newAlphaBit[i]);
        }

        DeleteObject(c1);
        DeleteObject(c2);
        DeleteObject(c3);

		PostQuitMessage(0);
        break;
	}
	return(DefWindowProc(hWnd, iMessage, wParam, lParam));
}

LRESULT CALLBACK ChildTopProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    HDC hdc;
    PAINTSTRUCT ps;
    int i = 0, j = 0, k = 0;
    static RECT center;
    static HFONT newFont, oldFont;
    static int x = 100, y;

    switch (message)
    {
    case WM_CREATE:
        break;
    case WM_TIMER:     
        switch (wParam)
        {
        case 1:
            KillTimer(hC1, 1);

            MessageBeep(0);
            MessageBox(hWndMain, TEXT("게임 클리어에 실패하셨습니다. 곧 재시작합니다.\n(※확인 버튼을 누르세요. 이외 동작금지)"), TEXT("게임 오버"), MB_OK | MB_ICONINFORMATION);

            SendMessage(hC2, WM_COMMAND, 5, 0);
            break;
        case 2:
            KillTimer(hC1, 2);

            MessageBeep(0);
            MessageBox(hWndMain, TEXT("단어를 맞추셨습니다!. 곧 재시작합니다.\n(※확인 버튼을 누르세요. 이외 동작금지)"), TEXT("게임 클리어!"), MB_OK | MB_ICONINFORMATION);

            SendMessage(hC2, WM_COMMAND, 5, 0);
            break;
        }
        break;
    case WM_PAINT:
        nQuestionLen = lstrlen(szQuestion);

        hdc = BeginPaint(hWnd, &ps);
        
        SetBkMode(hdc, TRANSPARENT);

        GetClientRect(hWnd, &center);

        x = 10;

        y = center.bottom - 30;

        newFont = CreateFont(40, 0, 0, 0, FW_NORMAL, 0, 0, 0, HANGEUL_CHARSET, 0, 0, 0, VARIABLE_PITCH | FF_ROMAN, TEXT("나눔고딕"));
        oldFont = (HFONT)SelectObject(hdc, newFont);

        DrawBitmap(hdc, 0, 0, center.right, center.bottom - 100, newBitmap[10]);
        DrawBitmap(hdc, center.right - 180, center.bottom - 100, 180, 100, newBitmap[12]);

        MoveToEx(hdc, center.right - 180, center.bottom - 100, NULL);
        LineTo(hdc, center.right - 180, center.bottom);


        for (i = 0; i < nQuestionLen; i++)
        {
            if (szQuestion[i] != ' ')
            {
                DrawBitmap(hdc, x, y, 40, 30, newBitmap[11]);
            }

            for (j = 0; j < InsertionCnt; j++)
            {  
                switch (Insertion[j])
                {
                case aA:
                    if (szQuestion[i] == L'a')
                    {
                        DrawBitmap(hdc, x, y - 35, 40, 40, newAlphaBit[aA - aA]);
                        
                        if (!InsertionOnce[j])
                        {
                            if (ClickCnt[aA - aA] == 1)
                            {
                                QuestionCnt--;
                                AnswerCnt++;
                            }
                            else
                            {
                                QuestionCnt--;
                            }
                        }

                        InsertionOnce[j] = 1;

                    }
                    break;
                case aA2:
                    if (szQuestion[i] == L'á')
                    {
                        DrawBitmap(hdc, x, y - 35, 40, 40, newAlphaBit[aA2 - aA]);

                        if (!InsertionOnce[j])
                        {
                            if (ClickCnt[aA2 - aA] == 1)
                            {
                                QuestionCnt--;
                                AnswerCnt++;
                            }
                            else
                            {
                                QuestionCnt--;
                            }
                        }

                        InsertionOnce[j] = 1;
                    }
                    break;
                case aA3:
                    if (szQuestion[i] == L'à')
                    {
                        DrawBitmap(hdc, x, y - 35, 40, 40, newAlphaBit[aA3 - aA]);

                        if (!InsertionOnce[j])
                        {
                            if (ClickCnt[aA3 - aA] == 1)
                            {
                                QuestionCnt--;
                                AnswerCnt++;
                            }
                            else
                            {
                                QuestionCnt--;
                            }
                        }

                        InsertionOnce[j] = 1;
                    }
                    break;
                case aA4:
                    if (szQuestion[i] == L'ả')
                    {
                        DrawBitmap(hdc, x, y - 35, 40, 40, newAlphaBit[aA4 - aA]);

                        if (!InsertionOnce[j])
                        {
                            if (ClickCnt[aA4 - aA] == 1)
                            {
                                QuestionCnt--;
                                AnswerCnt++;
                            }
                            else
                            {
                                QuestionCnt--;
                            }
                        }

                        InsertionOnce[j] = 1;
                    }
                    break;
                case aA5:
                    if (szQuestion[i] == L'ã')
                    {
                        DrawBitmap(hdc, x, y - 35, 40, 40, newAlphaBit[aA5 - aA]);

                        if (!InsertionOnce[j])
                        {
                            if (ClickCnt[aA5 - aA] == 1)
                            {
                                QuestionCnt--;
                                AnswerCnt++;
                            }
                            else
                            {
                                QuestionCnt--;
                            }
                        }

                        InsertionOnce[j] = 1;
                    }
                    break;
                case aA6:
                    if (szQuestion[i] == L'ạ')
                    {
                        DrawBitmap(hdc, x, y - 35, 40, 40, newAlphaBit[aA6 - aA]);

                        if (!InsertionOnce[j])
                        {
                            if (ClickCnt[aA6 - aA] == 1)
                            {
                                QuestionCnt--;
                                AnswerCnt++;
                            }
                            else
                            {
                                QuestionCnt--;
                            }
                        }

                        InsertionOnce[j] = 1;
                    }
                    break;
                case aA1_1:
                    if (szQuestion[i] == L'ă')
                    {
                        DrawBitmap(hdc, x, y - 35, 40, 40, newAlphaBit[aA1_1 - aA]);

                        if (!InsertionOnce[j])
                        {
                            if (ClickCnt[aA1_1 - aA] == 1)
                            {
                                QuestionCnt--;
                                AnswerCnt++;
                            }
                            else
                            {
                                QuestionCnt--;
                            }
                        }

                        InsertionOnce[j] = 1;
                    }
                    break;
                case aA1_2:
                    if (szQuestion[i] == L'ắ')
                    {
                        DrawBitmap(hdc, x, y - 35, 40, 40, newAlphaBit[aA1_2 - aA]);

                        if (!InsertionOnce[j])
                        {
                            if (ClickCnt[aA1_2 - aA] == 1)
                            {
                                QuestionCnt--;
                                AnswerCnt++;
                            }
                            else
                            {
                                QuestionCnt--;
                            }
                        }

                        InsertionOnce[j] = 1;
                    }
                    break;
                case aA1_3:
                    if (szQuestion[i] == L'ằ')
                    {
                        DrawBitmap(hdc, x, y - 35, 40, 40, newAlphaBit[aA1_3 - aA]);

                        if (!InsertionOnce[j])
                        {
                            if (ClickCnt[aA1_3 - aA] == 1)
                            {
                                QuestionCnt--;
                                AnswerCnt++;
                            }
                            else
                            {
                                QuestionCnt--;
                            }
                        }

                        InsertionOnce[j] = 1;
                    }
                    break;
                case aA1_4:
                    if (szQuestion[i] == L'ẳ')
                    {
                        DrawBitmap(hdc, x, y - 35, 40, 40, newAlphaBit[aA1_4 - aA]);

                        if (!InsertionOnce[j])
                        {
                            if (ClickCnt[aA1_4 - aA] == 1)
                            {
                                QuestionCnt--;
                                AnswerCnt++;
                            }
                            else
                            {
                                QuestionCnt--;
                            }
                        }

                        InsertionOnce[j] = 1;
                    }
                    break;
                case aA1_5:
                    if (szQuestion[i] == L'ẵ')
                    {
                        DrawBitmap(hdc, x, y - 35, 40, 40, newAlphaBit[aA1_5 - aA]);

                        if (!InsertionOnce[j])
                        {
                            if (ClickCnt[aA1_5 - aA] == 1)
                            {
                                QuestionCnt--;
                                AnswerCnt++;
                            }
                            else
                            {
                                QuestionCnt--;
                            }
                        }

                        InsertionOnce[j] = 1;
                    }
                    break;
                case aA1_6:
                    if (szQuestion[i] == L'ặ')
                    {
                        DrawBitmap(hdc, x, y - 35, 40, 40, newAlphaBit[aA1_6 - aA]);

                        if (!InsertionOnce[j])
                        {
                            if (ClickCnt[aA1_6 - aA] == 1)
                            {
                                QuestionCnt--;
                                AnswerCnt++;
                            }
                            else
                            {
                                QuestionCnt--;
                            }
                        }

                        InsertionOnce[j] = 1;
                    }
                    break;
                case aA2_1:
                    if (szQuestion[i] == L'â')
                    {
                        DrawBitmap(hdc, x, y - 35, 40, 40, newAlphaBit[aA2_1 - aA]);

                        if (!InsertionOnce[j])
                        {
                            if (ClickCnt[aA2_1 - aA] == 1)
                            {
                                QuestionCnt--;
                                AnswerCnt++;
                            }
                            else
                            {
                                QuestionCnt--;
                            }
                        }

                        InsertionOnce[j] = 1;
                    }
                    break;
                case aA2_2:
                    if (szQuestion[i] == L'ấ')
                    {
                        DrawBitmap(hdc, x, y - 35, 40, 40, newAlphaBit[aA2_2 - aA]);

                        if (!InsertionOnce[j])
                        {
                            if (ClickCnt[aA2_2 - aA] == 1)
                            {
                                QuestionCnt--;
                                AnswerCnt++;
                            }
                            else
                            {
                                QuestionCnt--;
                            }
                        }

                        InsertionOnce[j] = 1;
                    }
                    break;
                case aA2_3:
                    if (szQuestion[i] == L'ầ')
                    {
                        DrawBitmap(hdc, x, y - 35, 40, 40, newAlphaBit[aA2_3 - aA]);

                        if (!InsertionOnce[j])
                        {
                            if (ClickCnt[aA2_3 - aA] == 1)
                            {
                                QuestionCnt--;
                                AnswerCnt++;
                            }
                            else
                            {
                                QuestionCnt--;
                            }
                        }

                        InsertionOnce[j] = 1;
                    }
                    break;
                case aA2_4:
                    if (szQuestion[i] == L'ẩ')
                    {
                        DrawBitmap(hdc, x, y - 35, 40, 40, newAlphaBit[aA2_4 - aA]);

                        if (!InsertionOnce[j])
                        {
                            if (ClickCnt[aA2_4 - aA] == 1)
                            {
                                QuestionCnt--;
                                AnswerCnt++;
                            }
                            else
                            {
                                QuestionCnt--;
                            }
                        }

                        InsertionOnce[j] = 1;
                    }
                    break;
                case aA2_5:
                    if (szQuestion[i] == L'ẫ')
                    {
                        DrawBitmap(hdc, x, y - 35, 40, 40, newAlphaBit[aA2_5 - aA]);

                        if (!InsertionOnce[j])
                        {
                            if (ClickCnt[aA2_5 - aA] == 1)
                            {
                                QuestionCnt--;
                                AnswerCnt++;
                            }
                            else
                            {
                                QuestionCnt--;
                            }
                        }

                        InsertionOnce[j] = 1;
                    }
                    break;
                case aA2_6:
                    if (szQuestion[i] == L'ậ')
                    {
                        DrawBitmap(hdc, x, y - 35, 40, 40, newAlphaBit[aA2_6 - aA]);

                        if (!InsertionOnce[j])
                        {
                            if (ClickCnt[aA2_6 - aA] == 1)
                            {
                                QuestionCnt--;
                                AnswerCnt++;
                            }
                            else
                            {
                                QuestionCnt--;
                            }
                        }

                        InsertionOnce[j] = 1;
                    }
                    break;
                case aB:
                    if (szQuestion[i] == L'b')
                    {
                        DrawBitmap(hdc, x, y - 50, 40, 60, newAlphaBit[aB - aA]);

                        if (!InsertionOnce[j])
                        {
                            if (ClickCnt[aB - aA] == 1)
                            {
                                QuestionCnt--;
                                AnswerCnt++;
                            }
                            else
                            {
                                QuestionCnt--;
                            }
                        }

                        InsertionOnce[j] = 1;
                    }
                    break;
                case aC:
                    if (szQuestion[i] == L'c')
                    {
                        DrawBitmap(hdc, x, y - 35, 40, 40, newAlphaBit[aC - aA]);

                        if (!InsertionOnce[j])
                        {
                            if (ClickCnt[aC - aA] == 1)
                            {
                                QuestionCnt--;
                                AnswerCnt++;
                            }
                            else
                            {
                                QuestionCnt--;
                            }
                        }

                        InsertionOnce[j] = 1;
                    }
                    break;
                case aD:
                    if (szQuestion[i] == L'd')
                    {
                        DrawBitmap(hdc, x, y - 50, 40, 60, newAlphaBit[aD - aA]);

                        if (!InsertionOnce[j])
                        {
                            if (ClickCnt[aD - aA] == 1)
                            {
                                QuestionCnt--;
                                AnswerCnt++;
                            }
                            else
                            {
                                QuestionCnt--;
                            }
                        }

                        InsertionOnce[j] = 1;
                    }
                    break;
                case aD1:
                    if (szQuestion[i] == L'đ')
                    {
                        DrawBitmap(hdc, x, y - 50, 40, 60, newAlphaBit[aD1 - aA]);

                        if (!InsertionOnce[j])
                        {
                            if (ClickCnt[aD1 - aA] == 1)
                            {
                                QuestionCnt--;
                                AnswerCnt++;
                            }
                            else
                            {
                                QuestionCnt--;
                            }
                        }

                        InsertionOnce[j] = 1;
                    }
                    break;
                case aE:
                    if (szQuestion[i] == L'e')
                    {
                        DrawBitmap(hdc, x, y - 35, 40, 40, newAlphaBit[aE - aA]);

                        if (!InsertionOnce[j])
                        {
                            if (ClickCnt[aE - aA] == 1)
                            {
                                QuestionCnt--;
                                AnswerCnt++;
                            }
                            else
                            {
                                QuestionCnt--;
                            }
                        }

                        InsertionOnce[j] = 1;
                    }
                    break;
                case aE2:
                    if (szQuestion[i] == L'é')
                    {
                        DrawBitmap(hdc, x, y - 35, 40, 40, newAlphaBit[aE2 - aA]);

                        if (!InsertionOnce[j])
                        {
                            if (ClickCnt[aE2 - aA] == 1)
                            {
                                QuestionCnt--;
                                AnswerCnt++;
                            }
                            else
                            {
                                QuestionCnt--;
                            }
                        }

                        InsertionOnce[j] = 1;
                    }
                    break;
                case aE3:
                    if (szQuestion[i] == L'è')
                    {
                        DrawBitmap(hdc, x, y - 35, 40, 40, newAlphaBit[aE3 - aA]);

                        if (!InsertionOnce[j])
                        {
                            if (ClickCnt[aE3 - aA] == 1)
                            {
                                QuestionCnt--;
                                AnswerCnt++;
                            }
                            else
                            {
                                QuestionCnt--;
                            }
                        }

                        InsertionOnce[j] = 1;
                    }
                    break;
                case aE4:
                    if (szQuestion[i] == L'ẻ')
                    {
                        DrawBitmap(hdc, x, y - 35, 40, 40, newAlphaBit[aE4 - aA]);

                        if (!InsertionOnce[j])
                        {
                            if (ClickCnt[aE4 - aA] == 1)
                            {
                                QuestionCnt--;
                                AnswerCnt++;
                            }
                            else
                            {
                                QuestionCnt--;
                            }
                        }

                        InsertionOnce[j] = 1;
                    }
                    break;
                case aE5:
                    if (szQuestion[i] == L'ẽ')
                    {
                        DrawBitmap(hdc, x, y - 35, 40, 40, newAlphaBit[aE5 - aA]);

                        if (!InsertionOnce[j])
                        {
                            if (ClickCnt[aE5 - aA] == 1)
                            {
                                QuestionCnt--;
                                AnswerCnt++;
                            }
                            else
                            {
                                QuestionCnt--;
                            }
                        }

                        InsertionOnce[j] = 1;
                    }
                    break;
                case aE6:
                    if (szQuestion[i] == L'ẹ')
                    {
                        DrawBitmap(hdc, x, y - 35, 40, 40, newAlphaBit[aE6 - aA]);

                        if (!InsertionOnce[j])
                        {
                            if (ClickCnt[aE6 - aA] == 1)
                            {
                                QuestionCnt--;
                                AnswerCnt++;
                            }
                            else
                            {
                                QuestionCnt--;
                            }
                        }

                        InsertionOnce[j] = 1;
                    }
                    break;
                case aE1_1:
                    if (szQuestion[i] == L'ê')
                    {
                        DrawBitmap(hdc, x, y - 35, 40, 40, newAlphaBit[aE1_1 - aA]);

                        if (!InsertionOnce[j])
                        {
                            if (ClickCnt[aE1_1 - aA] == 1)
                            {
                                QuestionCnt--;
                                AnswerCnt++;
                            }
                            else
                            {
                                QuestionCnt--;
                            }
                        }

                        InsertionOnce[j] = 1;
                    }
                    break;
                case aE1_2:
                    if (szQuestion[i] == L'ế')
                    {
                        DrawBitmap(hdc, x, y - 35, 40, 40, newAlphaBit[aE1_2 - aA]);

                        if (!InsertionOnce[j])
                        {
                            if (ClickCnt[aE1_2 - aA] == 1)
                            {
                                QuestionCnt--;
                                AnswerCnt++;
                            }
                            else
                            {
                                QuestionCnt--;
                            }
                        }

                        InsertionOnce[j] = 1;
                    }
                    break;
                case aE1_3:
                    if (szQuestion[i] == L'ề')
                    {
                        DrawBitmap(hdc, x, y - 35, 40, 40, newAlphaBit[aE1_3 - aA]);

                        if (!InsertionOnce[j])
                        {
                            if (ClickCnt[aE1_3 - aA] == 1)
                            {
                                QuestionCnt--;
                                AnswerCnt++;
                            }
                            else
                            {
                                QuestionCnt--;
                            }
                        }

                        InsertionOnce[j] = 1;
                    }
                    break;
                case aE1_4:
                    if (szQuestion[i] == L'ể')
                    {
                        DrawBitmap(hdc, x, y - 35, 40, 40, newAlphaBit[aE1_4 - aA]);

                        if (!InsertionOnce[j])
                        {
                            if (ClickCnt[aE1_4 - aA] == 1)
                            {
                                QuestionCnt--;
                                AnswerCnt++;
                            }
                            else
                            {
                                QuestionCnt--;
                            }
                        }

                        InsertionOnce[j] = 1;
                    }
                    break;
                case aE1_5:
                    if (szQuestion[i] == L'ễ')
                    {
                        DrawBitmap(hdc, x, y - 35, 40, 40, newAlphaBit[aE1_5 - aA]);

                        if (!InsertionOnce[j])
                        {
                            if (ClickCnt[aE1_5 - aA] == 1)
                            {
                                QuestionCnt--;
                                AnswerCnt++;
                            }
                            else
                            {
                                QuestionCnt--;
                            }
                        }

                        InsertionOnce[j] = 1;
                    }
                    break;
                case aE1_6:
                    if (szQuestion[i] == L'ệ')
                    {
                        DrawBitmap(hdc, x, y - 35, 40, 40, newAlphaBit[aE1_6 - aA]);

                        if (!InsertionOnce[j])
                        {
                            if (ClickCnt[aE1_6 - aA] == 1)
                            {
                                QuestionCnt--;
                                AnswerCnt++;
                            }
                            else
                            {
                                QuestionCnt--;
                            }
                        }

                        InsertionOnce[j] = 1;
                    }
                    break;
                case aF:
                    if (szQuestion[i] == L'f')
                    {
                        DrawBitmap(hdc, x, y - 48, 40, 60, newAlphaBit[aF - aA]);

                        if (!InsertionOnce[j])
                        {
                            if (ClickCnt[aF - aA] == 1)
                            {
                                QuestionCnt--;
                                AnswerCnt++;
                            }
                            else
                            {
                                QuestionCnt--;
                            }
                        }

                        InsertionOnce[j] = 1;
                    }
                    break;
                case aG:
                    if (szQuestion[i] == L'g')
                    {
                        DrawBitmap(hdc, x, y - 35, 40, 40, newAlphaBit[aG - aA]);

                        if (!InsertionOnce[j])
                        {
                            if (ClickCnt[aG - aA] == 1)
                            {
                                QuestionCnt--;
                                AnswerCnt++;
                            }
                            else
                            {
                                QuestionCnt--;
                            }
                        }

                        InsertionOnce[j] = 1;
                    }
                    break;
                case aH:
                    if (szQuestion[i] == L'h')
                    {
                        DrawBitmap(hdc, x + 5, y - 50, 30, 60, newAlphaBit[aH - aA]);

                        if (!InsertionOnce[j])
                        {
                            if (ClickCnt[aH - aA] == 1)
                            {
                                QuestionCnt--;
                                AnswerCnt++;
                            }
                            else
                            {
                                QuestionCnt--;
                            }
                        }

                        InsertionOnce[j] = 1;
                    }
                    break;
                case aI:
                    if (szQuestion[i] == L'i')
                    {
                        DrawBitmap(hdc, x + 5, y - 45, 30, 55, newAlphaBit[aI - aA]);

                        if (!InsertionOnce[j])
                        {
                            if (ClickCnt[aI - aA] == 1)
                            {
                                QuestionCnt--;
                                AnswerCnt++;
                            }
                            else
                            {
                                QuestionCnt--;
                            }
                        }

                        InsertionOnce[j] = 1;
                    }
                    break;
                case aI2:
                    if (szQuestion[i] == L'í')
                    {
                        DrawBitmap(hdc, x + 5, y - 45, 30, 55, newAlphaBit[aI2 - aA]);

                        if (!InsertionOnce[j])
                        {
                            if (ClickCnt[aI2 - aA] == 1)
                            {
                                QuestionCnt--;
                                AnswerCnt++;
                            }
                            else
                            {
                                QuestionCnt--;
                            }
                        }

                        InsertionOnce[j] = 1;
                    }
                    break;
                case aI3:
                    if (szQuestion[i] == L'ì')
                    {
                        DrawBitmap(hdc, x + 5, y - 45, 30, 55, newAlphaBit[aI3 - aA]);

                        if (!InsertionOnce[j])
                        {
                            if (ClickCnt[aI3 - aA] == 1)
                            {
                                QuestionCnt--;
                                AnswerCnt++;
                            }
                            else
                            {
                                QuestionCnt--;
                            }
                        }

                        InsertionOnce[j] = 1;
                    }
                    break;
                case aI4:
                    if (szQuestion[i] == L'ỉ')
                    {
                        DrawBitmap(hdc, x + 5, y - 45, 30, 55, newAlphaBit[aI4 - aA]);

                        if (!InsertionOnce[j])
                        {
                            if (ClickCnt[aI4 - aA] == 1)
                            {
                                QuestionCnt--;
                                AnswerCnt++;
                            }
                            else
                            {
                                QuestionCnt--;
                            }
                        }

                        InsertionOnce[j] = 1;
                    }
                    break;
                case aI5:
                    if (szQuestion[i] == L'ĩ')
                    {
                        DrawBitmap(hdc, x + 5, y - 45, 30, 55, newAlphaBit[aI5 - aA]);

                        if (!InsertionOnce[j])
                        {
                            if (ClickCnt[aI5 - aA] == 1)
                            {
                                QuestionCnt--;
                                AnswerCnt++;
                            }
                            else
                            {
                                QuestionCnt--;
                            }
                        }

                        InsertionOnce[j] = 1;
                    }
                    break;
                case aI6:
                    if (szQuestion[i] == L'ị')
                    {
                        DrawBitmap(hdc, x + 5, y - 45, 30, 55, newAlphaBit[aI6 - aA]);

                        if (!InsertionOnce[j])
                        {
                            if (ClickCnt[aI6 - aA] == 1)
                            {
                                QuestionCnt--;
                                AnswerCnt++;
                            }
                            else
                            {
                                QuestionCnt--;
                            }
                        }

                        InsertionOnce[j] = 1;
                    }
                    break;
                case aJ:
                    if (szQuestion[i] == L'j')
                    {
                        DrawBitmap(hdc, x + 5, y - 50, 30, 60, newAlphaBit[aJ - aA]);

                        if (!InsertionOnce[j])
                        {
                            if (ClickCnt[aJ - aA] == 1)
                            {
                                QuestionCnt--;
                                AnswerCnt++;
                            }
                            else
                            {
                                QuestionCnt--;
                            }
                        }

                        InsertionOnce[j] = 1;
                    }
                    break;
                case aK:
                    if (szQuestion[i] == L'k')
                    {
                        DrawBitmap(hdc, x, y - 50, 40, 60, newAlphaBit[aK - aA]);

                        if (!InsertionOnce[j])
                        {
                            if (ClickCnt[aK - aA] == 1)
                            {
                                QuestionCnt--;
                                AnswerCnt++;
                            }
                            else
                            {
                                QuestionCnt--;
                            }
                        }

                        InsertionOnce[j] = 1;
                    }
                    break;
                case aL:
                    if (szQuestion[i] == L'l')
                    {
                        DrawBitmap(hdc, x + 5, y - 50, 30, 60, newAlphaBit[aL - aA]);

                        if (!InsertionOnce[j])
                        {
                            if (ClickCnt[aL - aA] == 1)
                            {
                                QuestionCnt--;
                                AnswerCnt++;
                            }
                            else
                            {
                                QuestionCnt--;
                            }
                        }

                        InsertionOnce[j] = 1;
                    }
                    break;
                case aM:
                    if (szQuestion[i] == L'm')
                    {
                        DrawBitmap(hdc, x, y - 40, 40, 50, newAlphaBit[aM - aA]);

                        if (!InsertionOnce[j])
                        {
                            if (ClickCnt[aM - aA] == 1)
                            {
                                QuestionCnt--;
                                AnswerCnt++;
                            }
                            else
                            {
                                QuestionCnt--;
                            }
                        }

                        InsertionOnce[j] = 1;
                    }
                    break;
                case aN:
                    if (szQuestion[i] == L'n')
                    {
                        DrawBitmap(hdc, x + 5, y - 40, 30, 50, newAlphaBit[aN - aA]);

                        if (!InsertionOnce[j])
                        {
                            if (ClickCnt[aN - aA] == 1)
                            {
                                QuestionCnt--;
                                AnswerCnt++;
                            }
                            else
                            {
                                QuestionCnt--;
                            }
                        }

                        InsertionOnce[j] = 1;
                    }
                    break;
                case aO:
                    if (szQuestion[i] == L'o')
                    {
                        DrawBitmap(hdc, x, y - 35, 40, 45, newAlphaBit[aO - aA]);

                        if (!InsertionOnce[j])
                        {
                            if (ClickCnt[aO - aA] == 1)
                            {
                                QuestionCnt--;
                                AnswerCnt++;
                            }
                            else
                            {
                                QuestionCnt--;
                            }
                        }

                        InsertionOnce[j] = 1;
                    }
                    break;
                case aO2:
                    if (szQuestion[i] == L'ó')
                    {
                        DrawBitmap(hdc, x, y - 35, 40, 45, newAlphaBit[aO2 - aA]);

                        if (!InsertionOnce[j])
                        {
                            if (ClickCnt[aO2 - aA] == 1)
                            {
                                QuestionCnt--;
                                AnswerCnt++;
                            }
                            else
                            {
                                QuestionCnt--;
                            }
                        }

                        InsertionOnce[j] = 1;
                    }
                    break;
                case aO3:
                    if (szQuestion[i] == L'ò')
                    {
                        DrawBitmap(hdc, x, y - 35, 40, 45, newAlphaBit[aO3 - aA]);

                        if (!InsertionOnce[j])
                        {
                            if (ClickCnt[aO3 - aA] == 1)
                            {
                                QuestionCnt--;
                                AnswerCnt++;
                            }
                            else
                            {
                                QuestionCnt--;
                            }
                        }

                        InsertionOnce[j] = 1;
                    }
                    break;
                case aO4:
                    if (szQuestion[i] == L'ỏ')
                    {
                        DrawBitmap(hdc, x, y - 35, 40, 45, newAlphaBit[aO4 - aA]);

                        if (!InsertionOnce[j])
                        {
                            if (ClickCnt[aO4 - aA] == 1)
                            {
                                QuestionCnt--;
                                AnswerCnt++;
                            }
                            else
                            {
                                QuestionCnt--;
                            }
                        }

                        InsertionOnce[j] = 1;
                    }
                    break;
                case aO5:
                    if (szQuestion[i] == L'õ')
                    {
                        DrawBitmap(hdc, x, y - 35, 40, 45, newAlphaBit[aO5 - aA]);

                        if (!InsertionOnce[j])
                        {
                            if (ClickCnt[aO5 - aA] == 1)
                            {
                                QuestionCnt--;
                                AnswerCnt++;
                            }
                            else
                            {
                                QuestionCnt--;
                            }
                        }

                        InsertionOnce[j] = 1;
                    }
                    break;
                case aO6:
                    if (szQuestion[i] == L'ọ')
                    {
                        DrawBitmap(hdc, x, y - 35, 40, 45, newAlphaBit[aO6 - aA]);

                        if (!InsertionOnce[j])
                        {
                            if (ClickCnt[aO6 - aA] == 1)
                            {
                                QuestionCnt--;
                                AnswerCnt++;
                            }
                            else
                            {
                                QuestionCnt--;
                            }
                        }

                        InsertionOnce[j] = 1;
                    }
                    break;
                case aO1_1:
                    if (szQuestion[i] == L'ô')
                    {
                        DrawBitmap(hdc, x, y - 35, 40, 45, newAlphaBit[aO1_1 - aA]);

                        if (!InsertionOnce[j])
                        {
                            if (ClickCnt[aO1_1 - aA] == 1)
                            {
                                QuestionCnt--;
                                AnswerCnt++;
                            }
                            else
                            {
                                QuestionCnt--;
                            }
                        }

                        InsertionOnce[j] = 1;
                    }
                    break;
                case aO1_2:
                    if (szQuestion[i] == L'ố')
                    {
                        DrawBitmap(hdc, x, y - 35, 40, 45, newAlphaBit[aO1_2 - aA]);

                        if (!InsertionOnce[j])
                        {
                            if (ClickCnt[aO1_2 - aA] == 1)
                            {
                                QuestionCnt--;
                                AnswerCnt++;
                            }
                            else
                            {
                                QuestionCnt--;
                            }
                        }

                        InsertionOnce[j] = 1;
                    }
                    break;
                case aO1_3:
                    if (szQuestion[i] == L'ồ')
                    {
                        DrawBitmap(hdc, x, y - 35, 40, 45, newAlphaBit[aO1_3 - aA]);

                        if (!InsertionOnce[j])
                        {
                            if (ClickCnt[aO1_3 - aA] == 1)
                            {
                                QuestionCnt--;
                                AnswerCnt++;
                            }
                            else
                            {
                                QuestionCnt--;
                            }
                        }

                        InsertionOnce[j] = 1;
                    }
                    break;
                case aO1_4:
                    if (szQuestion[i] == L'ổ')
                    {
                        DrawBitmap(hdc, x, y - 35, 40, 45, newAlphaBit[aO1_4 - aA]);

                        if (!InsertionOnce[j])
                        {
                            if (ClickCnt[aO1_4 - aA] == 1)
                            {
                                QuestionCnt--;
                                AnswerCnt++;
                            }
                            else
                            {
                                QuestionCnt--;
                            }
                        }

                        InsertionOnce[j] = 1;
                    }
                    break;
                case aO1_5:
                    if (szQuestion[i] == L'ỗ')
                    {
                        DrawBitmap(hdc, x, y - 35, 40, 45, newAlphaBit[aO1_5 - aA]);

                        if (!InsertionOnce[j])
                        {
                            if (ClickCnt[aO1_5 - aA] == 1)
                            {
                                QuestionCnt--;
                                AnswerCnt++;
                            }
                            else
                            {
                                QuestionCnt--;
                            }
                        }

                        InsertionOnce[j] = 1;
                    }
                    break;
                case aO1_6:
                    if (szQuestion[i] == L'ộ')
                    {
                        DrawBitmap(hdc, x, y - 35, 40, 45, newAlphaBit[aO1_6 - aA]);

                        if (!InsertionOnce[j])
                        {
                            if (ClickCnt[aO1_6 - aA] == 1)
                            {
                                QuestionCnt--;
                                AnswerCnt++;
                            }
                            else
                            {
                                QuestionCnt--;
                            }
                        }

                        InsertionOnce[j] = 1;
                    }
                    break;
                case aO2_1:
                    if (szQuestion[i] == L'ơ')
                    {
                        DrawBitmap(hdc, x, y - 35, 40, 45, newAlphaBit[aO2_1 - aA]);

                        if (!InsertionOnce[j])
                        {
                            if (ClickCnt[aO2_1 - aA] == 1)
                            {
                                QuestionCnt--;
                                AnswerCnt++;
                            }
                            else
                            {
                                QuestionCnt--;
                            }
                        }

                        InsertionOnce[j] = 1;
                    }
                    break;
                case aO2_2:
                    if (szQuestion[i] == L'ớ')
                    {
                        DrawBitmap(hdc, x, y - 35, 40, 45, newAlphaBit[aO2_2 - aA]);

                        if (!InsertionOnce[j])
                        {
                            if (ClickCnt[aO2_2 - aA] == 1)
                            {
                                QuestionCnt--;
                                AnswerCnt++;
                            }
                            else
                            {
                                QuestionCnt--;
                            }
                        }

                        InsertionOnce[j] = 1;
                    }
                    break;
                case aO2_3:
                    if (szQuestion[i] == L'ờ')
                    {
                        DrawBitmap(hdc, x, y - 35, 40, 45, newAlphaBit[aO2_3 - aA]);

                        if (!InsertionOnce[j])
                        {
                            if (ClickCnt[aO2_3 - aA] == 1)
                            {
                                QuestionCnt--;
                                AnswerCnt++;
                            }
                            else
                            {
                                QuestionCnt--;
                            }
                        }

                        InsertionOnce[j] = 1;
                    }
                    break;
                case aO2_4:
                    if (szQuestion[i] == L'ở')
                    {
                        DrawBitmap(hdc, x, y - 35, 40, 45, newAlphaBit[aO2_4 - aA]);

                        if (!InsertionOnce[j])
                        {
                            if (ClickCnt[aO2_4 - aA] == 1)
                            {
                                QuestionCnt--;
                                AnswerCnt++;
                            }
                            else
                            {
                                QuestionCnt--;
                            }
                        }

                        InsertionOnce[j] = 1;
                    }
                    break;
                case aO2_5:
                    if (szQuestion[i] == L'ỡ')
                    {
                        DrawBitmap(hdc, x, y - 35, 40, 45, newAlphaBit[aO2_5 - aA]);

                        if (!InsertionOnce[j])
                        {
                            if (ClickCnt[aO2_5 - aA] == 1)
                            {
                                QuestionCnt--;
                                AnswerCnt++;
                            }
                            else
                            {
                                QuestionCnt--;
                            }
                        }

                        InsertionOnce[j] = 1;
                    }
                    break;
                case aO2_6:
                    if (szQuestion[i] == L'ợ')
                    {
                        DrawBitmap(hdc, x, y - 35, 40, 45, newAlphaBit[aO2_6 - aA]);

                        if (!InsertionOnce[j])
                        {
                            if (ClickCnt[aO2_6 - aA] == 1)
                            {
                                QuestionCnt--;
                                AnswerCnt++;
                            }
                            else
                            {
                                QuestionCnt--;
                            }
                        }

                        InsertionOnce[j] = 1;
                    }
                    break;
                case aP:
                    if (szQuestion[i] == L'p')
                    {
                        DrawBitmap(hdc, x + 5, y - 35, 30, 40, newAlphaBit[aP - aA]);

                        if (!InsertionOnce[j])
                        {
                            if (ClickCnt[aP - aA] == 1)
                            {
                                QuestionCnt--;
                                AnswerCnt++;
                            }
                            else
                            {
                                QuestionCnt--;
                            }
                        }

                        InsertionOnce[j] = 1;
                    }
                    break;
                case aQ:
                    if (szQuestion[i] == L'q')
                    {
                        DrawBitmap(hdc, x, y - 35, 40, 45, newAlphaBit[aQ - aA]);

                        if (!InsertionOnce[j])
                        {
                            if (ClickCnt[aQ - aA] == 1)
                            {
                                QuestionCnt--;
                                AnswerCnt++;
                            }
                            else
                            {
                                QuestionCnt--;
                            }
                        }

                        InsertionOnce[j] = 1;
                    }
                    break;
                case aR:
                    if (szQuestion[i] == L'r')
                    {
                        DrawBitmap(hdc, x, y - 35, 40, 40, newAlphaBit[aR - aA]);

                        if (!InsertionOnce[j])
                        {
                            if (ClickCnt[aR - aA] == 1)
                            {
                                QuestionCnt--;
                                AnswerCnt++;
                            }
                            else
                            {
                                QuestionCnt--;
                            }
                        }

                        InsertionOnce[j] = 1;
                    }
                    break;
                case aS:
                    if (szQuestion[i] == L's')
                    {
                        DrawBitmap(hdc, x, y - 33, 40, 40, newAlphaBit[aS - aA]);

                        if (!InsertionOnce[j])
                        {
                            if (ClickCnt[aS - aA] == 1)
                            {
                                QuestionCnt--;
                                AnswerCnt++;
                            }
                            else
                            {
                                QuestionCnt--;
                            }
                        }

                        InsertionOnce[j] = 1;
                    }
                    break;
                case aT:
                    if (szQuestion[i] == L't')
                    {
                        DrawBitmap(hdc, x, y - 48, 40, 60, newAlphaBit[aT - aA]);

                        if (!InsertionOnce[j])
                        {
                            if (ClickCnt[aT - aA] == 1)
                            {
                                QuestionCnt--;
                                AnswerCnt++;
                            }
                            else
                            {
                                QuestionCnt--;
                            }
                        }

                        InsertionOnce[j] = 1;
                    }
                    break;
                case aU:
                    if (szQuestion[i] == L'u')
                    {
                        DrawBitmap(hdc, x, y - 35, 40, 40, newAlphaBit[aU - aA]);

                        if (!InsertionOnce[j])
                        {
                            if (ClickCnt[aU - aA] == 1)
                            {
                                QuestionCnt--;
                                AnswerCnt++;
                            }
                            else
                            {
                                QuestionCnt--;
                            }
                        }

                        InsertionOnce[j] = 1;
                    }
                    break;
                case aU2:
                    if (szQuestion[i] == L'ú')
                    {
                        DrawBitmap(hdc, x, y - 35, 40, 40, newAlphaBit[aU2 - aA]);

                        if (!InsertionOnce[j])
                        {
                            if (ClickCnt[aU2 - aA] == 1)
                            {
                                QuestionCnt--;
                                AnswerCnt++;
                            }
                            else
                            {
                                QuestionCnt--;
                            }
                        }

                        InsertionOnce[j] = 1;
                    }
                    break;
                case aU3:
                    if (szQuestion[i] == L'ù')
                    {
                        DrawBitmap(hdc, x, y - 35, 40, 40, newAlphaBit[aU3 - aA]);

                        if (!InsertionOnce[j])
                        {
                            if (ClickCnt[aU3 - aA] == 1)
                            {
                                QuestionCnt--;
                                AnswerCnt++;
                            }
                            else
                            {
                                QuestionCnt--;
                            }
                        }

                        InsertionOnce[j] = 1;
                    }
                    break;
                case aU4:
                    if (szQuestion[i] == L'ủ')
                    {
                        DrawBitmap(hdc, x, y - 35, 40, 40, newAlphaBit[aU4 - aA]);

                        if (!InsertionOnce[j])
                        {
                            if (ClickCnt[aU4 - aA] == 1)
                            {
                                QuestionCnt--;
                                AnswerCnt++;
                            }
                            else
                            {
                                QuestionCnt--;
                            }
                        }

                        InsertionOnce[j] = 1;
                    }
                    break;
                case aU5:
                    if (szQuestion[i] == L'ũ')
                    {
                        DrawBitmap(hdc, x, y - 35, 40, 40, newAlphaBit[aU5 - aA]);

                        if (!InsertionOnce[j])
                        {
                            if (ClickCnt[aU5 - aA] == 1)
                            {
                                QuestionCnt--;
                                AnswerCnt++;
                            }
                            else
                            {
                                QuestionCnt--;
                            }
                        }

                        InsertionOnce[j] = 1;
                    }
                    break;
                case aU6:
                    if (szQuestion[i] == L'ụ')
                    {
                        DrawBitmap(hdc, x, y - 35, 40, 40, newAlphaBit[aU6 - aA]);

                        if (!InsertionOnce[j])
                        {
                            if (ClickCnt[aU6 - aA] == 1)
                            {
                                QuestionCnt--;
                                AnswerCnt++;
                            }
                            else
                            {
                                QuestionCnt--;
                            }
                        }

                        InsertionOnce[j] = 1;
                    }
                    break;
                case aU1_1:
                    if (szQuestion[i] == L'ư')
                    {
                        DrawBitmap(hdc, x, y - 35, 40, 40, newAlphaBit[aU1_1 - aA]);

                        if (!InsertionOnce[j])
                        {
                            if (ClickCnt[aU1_1 - aA] == 1)
                            {
                                QuestionCnt--;
                                AnswerCnt++;
                            }
                            else
                            {
                                QuestionCnt--;
                            }
                        }

                        InsertionOnce[j] = 1;
                    }
                    break;
                case aU1_2:
                    if (szQuestion[i] == L'ứ')
                    {
                        DrawBitmap(hdc, x, y - 35, 40, 40, newAlphaBit[aU1_2 - aA]);

                        if (!InsertionOnce[j])
                        {
                            if (ClickCnt[aU1_2 - aA] == 1)
                            {
                                QuestionCnt--;
                                AnswerCnt++;
                            }
                            else
                            {
                                QuestionCnt--;
                            }
                        }

                        InsertionOnce[j] = 1;
                    }
                    break;
                case aU1_3:
                    if (szQuestion[i] == L'ừ')
                    {
                        DrawBitmap(hdc, x, y - 35, 40, 40, newAlphaBit[aU1_3 - aA]);

                        if (!InsertionOnce[j])
                        {
                            if (ClickCnt[aU1_3 - aA] == 1)
                            {
                                QuestionCnt--;
                                AnswerCnt++;
                            }
                            else
                            {
                                QuestionCnt--;
                            }
                        }

                        InsertionOnce[j] = 1;
                    }
                    break;
                case aU1_4:
                    if (szQuestion[i] == L'ử')
                    {
                        DrawBitmap(hdc, x, y - 35, 40, 40, newAlphaBit[aU1_4 - aA]);

                        if (!InsertionOnce[j])
                        {
                            if (ClickCnt[aU1_4 - aA] == 1)
                            {
                                QuestionCnt--;
                                AnswerCnt++;
                            }
                            else
                            {
                                QuestionCnt--;
                            }
                            AnswerCnt++;
                        }

                        InsertionOnce[j] = 1;
                    }
                    break;
                case aU1_5:
                    if (szQuestion[i] == L'ữ')
                    {
                        DrawBitmap(hdc, x, y - 35, 40, 40, newAlphaBit[aU1_5 - aA]);

                        if (!InsertionOnce[j])
                        {
                            if (ClickCnt[aU1_5 - aA] == 1)
                            {
                                QuestionCnt--;
                                AnswerCnt++;
                            }
                            else
                            {
                                QuestionCnt--;
                            }
                        }

                        InsertionOnce[j] = 1;
                    }
                    break;
                case aU1_6:
                    if (szQuestion[i] == L'ự')
                    {
                        DrawBitmap(hdc, x, y - 35, 40, 40, newAlphaBit[aU1_6 - aA]);

                        if (!InsertionOnce[j])
                        {
                            if (ClickCnt[aU1_6 - aA] == 1)
                            {
                                QuestionCnt--;
                                AnswerCnt++;
                            }
                            else
                            {
                                QuestionCnt--;
                            }
                        }

                        InsertionOnce[j] = 1;
                    }
                    break;
                case aV:
                    if (szQuestion[i] == L'v')
                    {
                        DrawBitmap(hdc, x, y - 35, 40, 40, newAlphaBit[aV - aA]);

                        if (!InsertionOnce[j])
                        {
                            if (ClickCnt[aV - aA] == 1)
                            {
                                QuestionCnt--;
                                AnswerCnt++;
                            }
                            else
                            {
                                QuestionCnt--;
                            }
                        }

                        InsertionOnce[j] = 1;
                    }
                    break;
                case aW:
                    if (szQuestion[i] == L'w')
                    {
                        DrawBitmap(hdc, x, y - 35, 40, 45, newAlphaBit[aW - aA]);

                        if (!InsertionOnce[j])
                        {
                            if (ClickCnt[aW - aA] == 1)
                            {
                                QuestionCnt--;
                                AnswerCnt++;
                            }
                            else
                            {
                                QuestionCnt--;
                            }
                        }

                        InsertionOnce[j] = 1;
                    }
                    break;
                case aX:
                    if (szQuestion[i] == L'x')
                    {
                        DrawBitmap(hdc, x, y - 35, 40, 40, newAlphaBit[aX - aA]);

                        if (!InsertionOnce[j])
                        {
                            if (ClickCnt[aX - aA] == 1)
                            {
                                QuestionCnt--;
                                AnswerCnt++;
                            }
                            else
                            {
                                QuestionCnt--;
                            }
                        }

                        InsertionOnce[j] = 1;
                    }
                    break;
                case aY:
                    if (szQuestion[i] == L'y')
                    {
                        DrawBitmap(hdc, x, y - 35, 40, 40, newAlphaBit[aY - aA]);

                        if (!InsertionOnce[j])
                        {
                            if (ClickCnt[aY - aA] == 1)
                            {
                                QuestionCnt--;
                                AnswerCnt++;
                            }
                            else
                            {
                                QuestionCnt--;
                            }
                        }

                        InsertionOnce[j] = 1;
                    }
                    break;
                case aZ:
                    if (szQuestion[i] == L'z')
                    {
                        DrawBitmap(hdc, x, y - 25, 40, 30, newAlphaBit[aZ - aA]);

                        if (!InsertionOnce[j])
                        {
                            if (ClickCnt[aZ - aA] == 1)
                            {
                                QuestionCnt--;
                                AnswerCnt++;
                            }
                            else
                            {
                                QuestionCnt--;
                            }
                        }

                        InsertionOnce[j] = 1;
                    }
                    break;
                case a0:
                    if (szQuestion[i] == L'0')
                    {
                        DrawBitmap(hdc, x + 5, y - 45, 30, 55, newAlphaBit[a0 - aA]);

                        if (!InsertionOnce[j])
                        {
                            if (ClickCnt[a0 - aA] == 1)
                            {
                                QuestionCnt--;
                                AnswerCnt++;
                            }
                            else
                            {
                                QuestionCnt--;
                            }
                        }

                        InsertionOnce[j] = 1;
                    }
                    break;
                case a1:
                    if (szQuestion[i] == L'1')
                    {
                        DrawBitmap(hdc, x + 5, y - 45, 30, 55, newAlphaBit[a1 - aA]);

                        if (!InsertionOnce[j])
                        {
                            if (ClickCnt[a1 - aA] == 1)
                            {
                                QuestionCnt--;
                                AnswerCnt++;
                            }
                            else
                            {
                                QuestionCnt--;
                            }
                        }

                        InsertionOnce[j] = 1;
                    }
                    break;
                case a2:
                    if (szQuestion[i] == L'2')
                    {
                        DrawBitmap(hdc, x + 5, y - 45, 30, 55, newAlphaBit[a2 - aA]);

                        if (!InsertionOnce[j])
                        {
                            if (ClickCnt[a2 - aA] == 1)
                            {
                                QuestionCnt--;
                                AnswerCnt++;
                            }
                            else
                            {
                                QuestionCnt--;
                            }
                        }

                        InsertionOnce[j] = 1;
                    }
                    break;
                case a3:
                    if (szQuestion[i] == L'3')
                    {
                        DrawBitmap(hdc, x + 5, y - 45, 30, 55, newAlphaBit[a3 - aA]);

                        if (!InsertionOnce[j])
                        {
                            if (ClickCnt[a3 - aA] == 1)
                            {
                                QuestionCnt--;
                                AnswerCnt++;
                            }
                            else
                            {
                                QuestionCnt--;
                            }
                        }

                        InsertionOnce[j] = 1;
                    }
                    break;
                case a4:
                    if (szQuestion[i] == L'4')
                    {
                        DrawBitmap(hdc, x + 5, y - 45, 30, 55, newAlphaBit[a4 - aA]);

                        if (!InsertionOnce[j])
                        {
                            if (ClickCnt[a4 - aA] == 1)
                            {
                                QuestionCnt--;
                                AnswerCnt++;
                            }
                            else
                            {
                                QuestionCnt--;
                            }
                        }

                        InsertionOnce[j] = 1;
                    }
                    break;
                case a5:
                    if (szQuestion[i] == L'5')
                    {
                        DrawBitmap(hdc, x + 5, y - 45, 30, 55, newAlphaBit[a5 - aA]);

                        if (!InsertionOnce[j])
                        {
                            if (ClickCnt[a5 - aA] == 1)
                            {
                                QuestionCnt--;
                                AnswerCnt++;
                            }
                            else
                            {
                                QuestionCnt--;
                            }
                        }

                        InsertionOnce[j] = 1;
                    }
                    break;
                case a6:
                    if (szQuestion[i] == L'6')
                    {
                        DrawBitmap(hdc, x + 5, y - 45, 30, 55, newAlphaBit[a6 - aA]);

                        if (!InsertionOnce[j])
                        {
                            if (ClickCnt[a6 - aA] == 1)
                            {
                                QuestionCnt--;
                                AnswerCnt++;
                            }
                            else
                            {
                                QuestionCnt--;
                            }
                        }

                        InsertionOnce[j] = 1;
                    }
                    break;
                case a7:
                    if (szQuestion[i] == L'7')
                    {
                        DrawBitmap(hdc, x + 5, y - 45, 30, 55, newAlphaBit[a7 - aA]);

                        if (!InsertionOnce[j])
                        {
                            if (ClickCnt[a7 - aA] == 1)
                            {
                                QuestionCnt--;
                                AnswerCnt++;
                            }
                            else
                            {
                                QuestionCnt--;
                            }
                        }

                        InsertionOnce[j] = 1;
                    }
                    break;
                case a8:
                    if (szQuestion[i] == L'8')
                    {
                        DrawBitmap(hdc, x + 5, y - 45, 30, 55, newAlphaBit[a8 - aA]);

                        if (!InsertionOnce[j])
                        {
                            if (ClickCnt[a8 - aA] == 1)
                            {
                                QuestionCnt--;
                                AnswerCnt++;
                            }
                            else
                            {
                                QuestionCnt--;
                            }
                        }

                        InsertionOnce[j] = 1;
                    }
                    break;
                case a9:
                    if (szQuestion[i] == L'9')
                    {
                        DrawBitmap(hdc, x + 5, y - 45, 30, 55, newAlphaBit[a9 - aA]);

                        if (!InsertionOnce[j])
                        {
                            if (ClickCnt[a9 - aA] == 1)
                            {
                                QuestionCnt--;
                                AnswerCnt++;
                            }
                            else
                            {
                                QuestionCnt--;
                            }
                        }

                        InsertionOnce[j] = 1;
                    }
                    break;
                default:
                    if (InsertionCnt > 0)
                    {
                        Insertion[InsertionCnt--] = aA - aA;
                    }                    
                    break;
                }
            }
            x += 40;


        }
        
        if (bQuestionText)
        {

            switch (QuestionCnt)
            {
            case 7:
                DrawBitmap(hdc, 220, 100, 50, 50, newBitmap[8]);
                DrawBitmap(hdc, 220, 150, 50, 70, newBitmap[9]);
                nFaceStat[2] = 0;
                nBodyStat = 0;
                bGameOver = TRUE;
            case 6:
                DrawBitmap(hdc, 250, 220, 20, 50, newBitmap[7]);
            case 5:
                if (nFaceStat[2])
                {
                    DrawBitmap(hdc, 220, 100, 50, 50, newBitmap[2]);
                }
                DrawBitmap(hdc, 220, 220, 20, 50, newBitmap[7]);
                nFaceStat[1] = 0;
            case 4:
                DrawBitmap(hdc, 270, 150, 40, 20, newBitmap[6]);
            case 3:
                if (nFaceStat[1])
                {
                    DrawBitmap(hdc, 220, 100, 50, 50, newBitmap[1]);
                }

                DrawBitmap(hdc, 180, 150, 40, 20, newBitmap[5]);

                nFaceStat[0] = 0;
            case 2:
                if (nBodyStat)
                {
                    DrawBitmap(hdc, 220, 150, 50, 70, newBitmap[4]);
                }
            case 1:
                if (nFaceStat[0])
                {
                    DrawBitmap(hdc, 220, 100, 50, 50, newBitmap[3]);
                }
                break;      
            }
        }

        if (bGameOver)
        {
            SetTimer(hC1, 1, 2000, NULL);
            SendMessage(hC1, WM_TIMER, 1, 0);
        }

        if (AnswerCnt == (nQuestionLen - BlankCnt - DupliCnt) && AnswerCnt != 0)
        {
            SetTimer(hC1, 2, 2000, NULL);
            SendMessage(hC1, WM_TIMER, 2, 0);
        }

        MoveToEx(hdc, 0, center.bottom - 100, NULL);
        LineTo(hdc, center.right, center.bottom - 100);


        SelectObject(hdc, oldFont);
        DeleteObject(newFont);

        EndPaint(hWnd, &ps);
        break;
    }


    return DefWindowProc(hWnd, message, wParam, lParam);
}


LRESULT CALLBACK ChildBottomProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    HDC hdc;
    PAINTSTRUCT ps;
    static RECT center;
    static HFONT oldFont, newFont;
    static HBRUSH oldBrush, newBrush, hBrush;
    static HWND hEdit, hBtn, hAnswer, hCheck, hClear;
    LONG Style;
    int i = 0, j = 0, k = 0;
    WPARAM game = 0;

    switch (message)
    {
    case WM_CREATE:
        GetClientRect(hWnd, &center);
        hC3 = CreateWindow(TEXT("ChildInner"), NULL, WS_VISIBLE | WS_CHILD | WS_CLIPCHILDREN | WS_BORDER, 0, 0, 0, 0, hWnd, (HMENU)0, g_hInst, NULL);
        hEdit = CreateWindowEx(WS_EX_CLIENTEDGE, TEXT("edit"), NULL, WS_VISIBLE | WS_CHILD | ES_AUTOHSCROLL , 590, 40, 200, 90, hWnd, (HMENU)1, g_hInst, NULL);
        hBtn = CreateWindow(TEXT("button"), TEXT("Add"), WS_VISIBLE | WS_CHILD | WS_BORDER, 720, 140, 70, 30, hWnd, (HMENU)2, g_hInst, NULL);
        hAnswer = CreateWindowEx(WS_EX_CLIENTEDGE, TEXT("edit"), NULL, WS_VISIBLE | WS_CHILD | ES_READONLY | ES_AUTOVSCROLL | ES_MULTILINE, 40, 140, 240, 60, hWnd, (HMENU)3, g_hInst, NULL);
        hCheck = CreateWindow(TEXT("button"), TEXT("Visibility"), WS_VISIBLE | WS_CHILD | BS_AUTOCHECKBOX, 40, 210, 80, 25, hWnd, (HMENU)4, g_hInst, NULL);
        hClear = CreateWindow(TEXT("button"), TEXT("Clear"), WS_VISIBLE | WS_CHILD | WS_BORDER, 720, 175, 70, 30, hWnd, (HMENU)5, g_hInst, NULL);

        newFont = CreateFont(20, 0, 0, 0, FW_NORMAL, 0, 0, 0, HANGEUL_CHARSET, 0, 0, 0, VARIABLE_PITCH | FF_ROMAN, TEXT("나눔고딕"));

        SendMessage(hEdit, WM_SETFONT, (WPARAM)newFont, MAKELPARAM(FALSE, 0));
        SendMessage(hAnswer, WM_SETFONT, (WPARAM)newFont, MAKELPARAM(FALSE, 0));
        SendMessage(hCheck, BM_SETCHECK, BST_UNCHECKED, 0);

        break;
    case WM_SIZE:
        if (wParam != SIZE_MINIMIZED)
        {
            GetClientRect(hWnd, &center);
                      
            MoveWindow(hC3, 0, 0, 580, 135, TRUE);
        }

        break;
    case WM_CTLCOLORSTATIC:
        if ((HWND)lParam == hCheck)
        {
            hBrush = CreateSolidBrush(RGB(255, 201, 14));

            SetBkColor((HDC)wParam, RGB(255, 201, 14));
            return (LRESULT)hBrush;
        }
        break;
    case WM_PAINT:
        hdc = BeginPaint(hWnd, &ps);
        
        SetBkMode(hdc, TRANSPARENT);

        DrawBitmap(hdc, 0, 0, center.right, center.bottom, newBitmap[0]);
        

        newFont = CreateFont(0, 0, 0, 0, FW_NORMAL, 0, 0, 0, HANGEUL_CHARSET, 0, 0, 0, VARIABLE_PITCH | FF_ROMAN, TEXT("나눔고딕"));
        newBrush = CreateSolidBrush(RGB(182, 243, 243));

        oldFont = (HFONT)SelectObject(hdc, newFont);
        oldBrush = (HBRUSH)SelectObject(hdc, newBrush);

        SetTextAlign(hdc, TA_CENTER);

        Rectangle(hdc, 610, 8, 770, 33);  
        TextOut(hdc, 690, 10, TEXT("★여기에 입력해요!!★"), lstrlen(TEXT("★여기에 입력해요!!★")));
        TextOut(hdc, 20, 140, TEXT("정답:"), lstrlen(TEXT("정답:")));
        
        SelectObject(hdc, oldBrush);
        SelectObject(hdc, oldFont);

        DeleteObject(newBrush);
        DeleteObject(newFont);
        EndPaint(hWnd, &ps);
        break;
    case WM_COMMAND:
        switch (wParam)
        {
        case 2: //텍스트 추가 버튼
            DupliCnt = 0;
            BlankCnt = 0;
            memset(InsertionOnce, 0, sizeof(InsertionOnce));
            memset(Dupli, 0, sizeof(Dupli));
            memset(ClickCnt, 0, sizeof(ClickCnt));
            DupliPtr = 0;
            

            GetWindowText(hEdit, szQuestion, 1024);
            SetWindowText(hEdit, TEXT(""));
            nQuestionLen = lstrlen(szQuestion);

            if (nQuestionLen >= 16)
            {
                MessageBeep(0);
                MessageBox(hWndMain, TEXT("글자는 15자를 초과할 수 없습니다."), TEXT("글자 길이 초과"), MB_OK | MB_ICONWARNING);
                SendMessage(hC2, WM_COMMAND, 5, 0);
                break;
            }

            for (i = 0; i < nQuestionLen; i++)
            {

                if (iswupper(szQuestion[i]))
                { 
                    szQuestion[i] = towlower(szQuestion[i]);
                }               

                if (szQuestion[i] == ' ')
                {
                    BlankCnt++;
                }
            }

            
            for (i = 0; i < nQuestionLen; i++)
            {
                for (j = i+1; j < nQuestionLen; j++)
                {
                    if (szQuestion[i] == szQuestion[j] && szQuestion[i] != ' ')
                    {
                        int flag = 1;

                        for (k = i - 1; k >= 0; k--)
                        {
                            if (szQuestion[k] == szQuestion[i])
                            {
                                flag = 0;
                                
                            }   

                        }           

                        for (k = 0; k < DupliPtr; k++)
                        {
                            if (Dupli[k] == szQuestion[i])
                            {
                                break;
                            }
                        }

                        if (k == DupliPtr)
                        {
                            Dupli[DupliPtr++] = szQuestion[i];
                        }

                        if (flag)
                        {
                            DupliCnt++;
                        }                       

                    }
                }

            }

            if (BlankCnt == nQuestionLen)
            {
                MessageBeep(0);
                MessageBox(hWndMain, TEXT("공백만으로 구성된 문자는 입력할 수 없습니다."), TEXT("공백 문자 오류"), MB_OK | MB_ICONWARNING);
                SendMessage(hC2, WM_COMMAND, 5, 0);
                break;
            }
      

            InsertionCnt = 0;
            QuestionCnt = 0;
            nBodyStat = 1;
            //SetWindowText(hAnswer, szQuestion);
            bQuestionText = TRUE;
            memset(Insertion, 0, sizeof(Insertion));
            memset(nFaceStat, 1, sizeof(nFaceStat));
            AnswerCnt = 0;

            if (bQuestionText)
            {
                Style = GetWindowLongPtr(hEdit, GWL_STYLE);

                Style |= ES_READONLY;

                SetWindowLongPtr(hEdit, GWL_STYLE, Style);
            }
            
            EnableWindow(hEdit, FALSE);
            EnableWindow(hBtn, FALSE);

            InvalidateRect(hC2, NULL, TRUE);
            InvalidateRect(hC1, NULL, TRUE);
            break;
        case 4: //가시성 설정 버튼
            bVisible = !bVisible;

            if (bVisible)
            {
                SetWindowText(hAnswer, szQuestion);
            }
            else
            {
                SetWindowText(hAnswer, TEXT(""));
            }

            break;
        case 5: //초기화 버튼
            memset(Insertion, 0, sizeof(Insertion));
            memset(nFaceStat, 1, sizeof(nFaceStat));
            memset(InsertionOnce, 0, sizeof(InsertionOnce));
            memset(Dupli, 0, sizeof(Dupli));
            memset(ClickCnt, 0, sizeof(ClickCnt));
            DupliPtr = 0;
            InsertionCnt = 0;
            bQuestionText = FALSE;
            SetWindowText(hAnswer, TEXT(""));
            SetWindowText(hEdit, TEXT(""));
            QuestionCnt = 0;
            nBodyStat = 1;
            bVisible = 0;
            bGameOver = FALSE;
            DupliCnt = 0;
            BlankCnt = 0;
            AnswerCnt = 0;

            Style = GetWindowLongPtr(hEdit, GWL_STYLE);

            Style &= ~ES_READONLY;

            SetWindowLongPtr(hEdit, GWL_STYLE, Style);

            memset(szQuestion, 0, sizeof(szQuestion));

            InvalidateRect(hC1, NULL, TRUE);

            EnableWindow(hEdit, TRUE);
            EnableWindow(hBtn, TRUE);

            SendMessage(hCheck, BM_SETCHECK, BST_UNCHECKED, 0);

            MessageBeep(0);
            MessageBox(hWndMain, TEXT("게임이 재시작되었습니다."), TEXT("재시작"), MB_OK | MB_ICONASTERISK);

            break;
        }
    }

    DeleteObject(hBrush);

    return DefWindowProc(hWnd, message, wParam, lParam);
}

LRESULT CALLBACK ChildInnerProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    static HWND hA, hB, hC, hD, hE, hF, hG, hH, hI, hJ, hK, hL, hM, hN, hO, hP, hQ, hR, hS, hT, hU, hV, hW, hX, hY, hZ;
    static HWND hList, hBtn, hNumber;
    static RECT center;
    HDC hdc;
    PAINTSTRUCT ps;
    int count = 0;
    static int pressA = 0, pressE = 0, pressO = 0, pressU = 0;
    int i = 0;

    switch (message)
    {
    case WM_CREATE:
        GetClientRect(hWnd, &center);

        //First Alphabet line

        hList = CreateWindow(TEXT("listbox"), NULL, WS_VISIBLE | WS_CHILD | LBS_NOTIFY | LBS_NOINTEGRALHEIGHT | WS_VSCROLL, 0, 0, 0, 0, hWnd, (HMENU)0, g_hInst, NULL);
        hBtn = CreateWindow(TEXT("button"), TEXT("Add"), WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON | WS_BORDER, 530, 20, 40, 100, hWnd, (HMENU)1, g_hInst, NULL);
        hNumber = CreateWindow(TEXT("button"), TEXT("Number"), WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON | WS_BORDER, 350, 90, 60, 30, hWnd, (HMENU)2, g_hInst, NULL);

        hQ = CreateWindow(TEXT("button"), TEXT("Q"), WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON, 10, 10, 30, 30, hWnd, (HMENU)aQ, g_hInst, NULL);
        hW = CreateWindow(TEXT("button"), TEXT("W"), WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON, 50, 10, 30, 30, hWnd, (HMENU)aW, g_hInst, NULL);
        hE = CreateWindow(TEXT("button"), TEXT("E"), WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON, 90, 10, 30, 30, hWnd, (HMENU)aE, g_hInst, NULL);
        hR = CreateWindow(TEXT("button"), TEXT("R"), WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON, 130,10, 30, 30, hWnd, (HMENU)aR, g_hInst, NULL);
        hT = CreateWindow(TEXT("button"), TEXT("T"), WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON, 170, 10, 30, 30, hWnd, (HMENU)aT, g_hInst, NULL);
        hY = CreateWindow(TEXT("button"), TEXT("Y"), WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON, 210, 10, 30, 30, hWnd, (HMENU)aY, g_hInst, NULL);
        hU = CreateWindow(TEXT("button"), TEXT("U"), WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON, 250, 10, 30, 30, hWnd, (HMENU)aU, g_hInst, NULL);
        hI = CreateWindow(TEXT("button"), TEXT("I"), WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON, 290, 10, 30, 30, hWnd, (HMENU)aI, g_hInst, NULL);
        hO = CreateWindow(TEXT("button"), TEXT("O"), WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON, 330, 10, 30, 30, hWnd, (HMENU)aO, g_hInst, NULL);
        hP = CreateWindow(TEXT("button"), TEXT("P"), WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON, 370, 10, 30, 30, hWnd, (HMENU)aP, g_hInst, NULL);

        //Second Alphabet line

        hA = CreateWindow(TEXT("button"), TEXT("A"), WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON, 30, 50, 30, 30, hWnd, (HMENU)aA, g_hInst, NULL);
        hS = CreateWindow(TEXT("button"), TEXT("S"), WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON, 70, 50, 30, 30, hWnd, (HMENU)aS, g_hInst, NULL);
        hD = CreateWindow(TEXT("button"), TEXT("D"), WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON, 110, 50, 30, 30, hWnd, (HMENU)aD, g_hInst, NULL);
        hF = CreateWindow(TEXT("button"), TEXT("F"), WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON, 150, 50, 30, 30, hWnd, (HMENU)aF, g_hInst, NULL);
        hG = CreateWindow(TEXT("button"), TEXT("G"), WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON, 190, 50, 30, 30, hWnd, (HMENU)aG, g_hInst, NULL);
        hH = CreateWindow(TEXT("button"), TEXT("H"), WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON, 230, 50, 30, 30, hWnd, (HMENU)aH, g_hInst, NULL);
        hJ = CreateWindow(TEXT("button"), TEXT("J"), WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON, 270, 50, 30, 30, hWnd, (HMENU)aJ, g_hInst, NULL);
        hK = CreateWindow(TEXT("button"), TEXT("K"), WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON, 310, 50, 30, 30, hWnd, (HMENU)aK, g_hInst, NULL);
        hL = CreateWindow(TEXT("button"), TEXT("L"), WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON, 350, 50, 30, 30, hWnd, (HMENU)aL, g_hInst, NULL);

        //Third Alphabet line
        hZ = CreateWindow(TEXT("button"), TEXT("Z"), WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON, 70, 90, 30, 30, hWnd, (HMENU)aZ, g_hInst, NULL);
        hX = CreateWindow(TEXT("button"), TEXT("X"), WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON, 110, 90, 30, 30, hWnd, (HMENU)aX, g_hInst, NULL);
        hC = CreateWindow(TEXT("button"), TEXT("C"), WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON, 150, 90, 30, 30, hWnd, (HMENU)aC, g_hInst, NULL);
        hV = CreateWindow(TEXT("button"), TEXT("V"), WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON, 190, 90, 30, 30, hWnd, (HMENU)aV, g_hInst, NULL);
        hB = CreateWindow(TEXT("button"), TEXT("B"), WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON, 230, 90, 30, 30, hWnd, (HMENU)aB, g_hInst, NULL);
        hN = CreateWindow(TEXT("button"), TEXT("N"), WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON, 270, 90, 30, 30, hWnd, (HMENU)aN, g_hInst, NULL);
        hM = CreateWindow(TEXT("button"), TEXT("M"), WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON, 310, 90, 30, 30, hWnd, (HMENU)aM, g_hInst, NULL);
        break;
    case WM_SIZE:
        if (wParam != SIZE_MINIMIZED)
        {
            MoveWindow(hList, 420, 0, 100, 140, TRUE);
        }
        break;
    case WM_PAINT: //구분선 그림
        hdc = BeginPaint(hWnd, &ps);

        MoveToEx(hdc, 419, 0, NULL);
        LineTo(hdc, 419, 140);

        MoveToEx(hdc, 521, 0, NULL);
        LineTo(hdc, 521, 140);


        EndPaint(hWnd, &ps);
        break;
    case WM_COMMAND:
        switch (LOWORD(wParam))
        {
        case 0: //추가 문자 리스트 박스
            break;
        case 1: //추가 버튼
            i = SendMessage(hList, LB_GETCURSEL, 0, 0);

            SendMessage(hList, LB_GETTEXT, i, (LPARAM)szAnswer);

            if (bQuestionText)
            {
                switch (szAnswer[0])
                {
                case L'a':
                    switch (szAnswer[2])
                    {
                    case L'1':
                        Insertion[InsertionCnt++] = aA;
                        QuestionCnt++;
                        ClickCnt[aA - aA]++;
                        break;
                    case L'2':
                        Insertion[InsertionCnt++] = aA2;
                        QuestionCnt++;
                        ClickCnt[aA2 - aA]++;
                        break;
                    case L'3':
                        Insertion[InsertionCnt++] = aA3;
                        QuestionCnt++;
                        ClickCnt[aA3 - aA]++;
                        break;
                    case L'4':
                        Insertion[InsertionCnt++] = aA4;
                        QuestionCnt++;
                        ClickCnt[aA4 - aA]++;
                        break;
                    case L'5':
                        Insertion[InsertionCnt++] = aA5;
                        QuestionCnt++;
                        ClickCnt[aA5 - aA]++;
                        break;
                    case L'6':
                        Insertion[InsertionCnt++] = aA6;
                        QuestionCnt++;
                        ClickCnt[aA6 - aA]++;
                        break;
                    }
                    break;
                case L'ă':
                    switch (szAnswer[2])
                    {
                    case L'1':
                        Insertion[InsertionCnt++] = aA1_1;
                        QuestionCnt++;
                        ClickCnt[aA1_1 - aA]++;
                        break;
                    case L'2':
                        Insertion[InsertionCnt++] = aA1_2;
                        QuestionCnt++;
                        ClickCnt[aA1_2 - aA]++;
                        break;
                    case L'3':
                        Insertion[InsertionCnt++] = aA1_3;
                        QuestionCnt++;
                        ClickCnt[aA1_3 - aA]++;
                        break;
                    case L'4':
                        Insertion[InsertionCnt++] = aA1_4;
                        QuestionCnt++;
                        ClickCnt[aA1_4 - aA]++;
                        break;
                    case L'5':
                        Insertion[InsertionCnt++] = aA1_5;
                        QuestionCnt++;
                        ClickCnt[aA1_5 - aA]++;
                        break;
                    case L'6':
                        Insertion[InsertionCnt++] = aA1_6;
                        QuestionCnt++;
                        ClickCnt[aA1_6 - aA]++;
                        break;
                    }
                    break;
                case L'â':
                    switch (szAnswer[2])
                    {
                    case L'1':
                        Insertion[InsertionCnt++] = aA2_1;
                        QuestionCnt++;
                        ClickCnt[aA2_1 - aA]++;
                        break;
                    case L'2':
                        Insertion[InsertionCnt++] = aA2_2;
                        QuestionCnt++;
                        ClickCnt[aA2_2 - aA]++;
                        break;
                    case L'3':
                        Insertion[InsertionCnt++] = aA2_3;
                        QuestionCnt++;
                        ClickCnt[aA2_3 - aA]++;
                        break;
                    case L'4':
                        Insertion[InsertionCnt++] = aA2_4;
                        QuestionCnt++;
                        ClickCnt[aA2_4 - aA]++;
                        break;
                    case L'5':
                        Insertion[InsertionCnt++] = aA2_5;
                        QuestionCnt++;
                        ClickCnt[aA2_5 - aA]++;
                        break;
                    case L'6':
                        Insertion[InsertionCnt++] = aA2_6;
                        QuestionCnt++;
                        ClickCnt[aA2_6 - aA]++;
                        break;
                    }
                    break;
                case L'd':
                    Insertion[InsertionCnt++] = aD;
                    QuestionCnt++;
                    ClickCnt[aD - aA]++;
                    break;
                case L'đ':
                    Insertion[InsertionCnt++] = aD1;
                    QuestionCnt++;
                    ClickCnt[aD1 - aA]++;
                    break;
                case L'e':
                    switch (szAnswer[2])
                    {
                    case L'1':
                        Insertion[InsertionCnt++] = aE;
                        QuestionCnt++;
                        ClickCnt[aE - aA]++;
                        break;
                    case L'2':
                        Insertion[InsertionCnt++] = aE2;
                        QuestionCnt++;
                        ClickCnt[aE2 - aA]++;
                        break;
                    case L'3':
                        Insertion[InsertionCnt++] = aE3;
                        QuestionCnt++;
                        ClickCnt[aE3 - aA]++;
                        break;
                    case L'4':
                        Insertion[InsertionCnt++] = aE4;
                        QuestionCnt++;
                        ClickCnt[aE4 - aA]++;
                        break;
                    case L'5':
                        Insertion[InsertionCnt++] = aE5;
                        QuestionCnt++;
                        ClickCnt[aE5 - aA]++;
                        break;
                    case L'6':
                        Insertion[InsertionCnt++] = aE6;
                        QuestionCnt++;
                        ClickCnt[aE6 - aA]++;
                        break;
                    }
                    break;
                case L'ê':
                    switch (szAnswer[2])
                    {
                    case L'1':
                        Insertion[InsertionCnt++] = aE1_1;
                        QuestionCnt++;
                        ClickCnt[aE1_1 - aA]++;
                        break;
                    case L'2':
                        Insertion[InsertionCnt++] = aE1_2;
                        QuestionCnt++;
                        ClickCnt[aE1_2 - aA]++;
                        break;
                    case L'3':
                        Insertion[InsertionCnt++] = aE1_3;
                        QuestionCnt++;
                        ClickCnt[aE1_3 - aA]++;
                        break;
                    case L'4':
                        Insertion[InsertionCnt++] = aE1_4;
                        QuestionCnt++;
                        ClickCnt[aE1_4 - aA]++;
                        break;
                    case L'5':
                        Insertion[InsertionCnt++] = aE1_5;
                        QuestionCnt++;
                        ClickCnt[aE1_5 - aA]++;
                        break;
                    case L'6':
                        Insertion[InsertionCnt++] = aE1_6;
                        QuestionCnt++;
                        ClickCnt[aE1_6 - aA]++;
                        break;
                    }
                    break;
                case L'i':
                    switch (szAnswer[2])
                    {
                    case L'1':
                        Insertion[InsertionCnt++] = aI;
                        QuestionCnt++;
                        ClickCnt[aI - aA]++;
                        break;
                    case L'2':
                        Insertion[InsertionCnt++] = aI2;
                        QuestionCnt++;
                        ClickCnt[aI2 - aA]++;
                        break;
                    case L'3':
                        Insertion[InsertionCnt++] = aI3;
                        QuestionCnt++;
                        ClickCnt[aI3 - aA]++;
                        break;
                    case L'4':
                        Insertion[InsertionCnt++] = aI4;
                        QuestionCnt++;
                        ClickCnt[aI4 - aA]++;
                        break;
                    case L'5':
                        Insertion[InsertionCnt++] = aI5;
                        QuestionCnt++;
                        ClickCnt[aI5 - aA]++;
                        break;
                    case L'6':
                        Insertion[InsertionCnt++] = aI6;
                        QuestionCnt++;
                        ClickCnt[aI6 - aA]++;
                        break;
                    }
                    break;
                case L'o':
                    switch (szAnswer[2])
                    {
                    case L'1':
                        Insertion[InsertionCnt++] = aO;
                        QuestionCnt++;
                        ClickCnt[aO - aA]++;
                        break;
                    case L'2':
                        Insertion[InsertionCnt++] = aO2;
                        QuestionCnt++;
                        ClickCnt[aO2 - aA]++;
                        break;
                    case L'3':
                        Insertion[InsertionCnt++] = aO3;
                        QuestionCnt++;
                        ClickCnt[aO3 - aA]++;
                        break;
                    case L'4':
                        Insertion[InsertionCnt++] = aO4;
                        QuestionCnt++;
                        ClickCnt[aO4 - aA]++;
                        break;
                    case L'5':
                        Insertion[InsertionCnt++] = aO5;
                        QuestionCnt++;
                        ClickCnt[aO5 - aA]++;
                        break;
                    case L'6':
                        Insertion[InsertionCnt++] = aO6;
                        QuestionCnt++;
                        ClickCnt[aO6 - aA]++;
                        break;
                    }

                    switch (szAnswer[3])
                    {
                    case L'1':
                        Insertion[InsertionCnt++] = aO2_1;
                        QuestionCnt++;
                        ClickCnt[aO2_1 - aA]++;
                        break;
                    case L'2':
                        Insertion[InsertionCnt++] = aO2_2;
                        QuestionCnt++;
                        ClickCnt[aO2_2 - aA]++;
                        break;
                    case L'3':
                        Insertion[InsertionCnt++] = aO2_3;
                        QuestionCnt++;
                        ClickCnt[aO2_3 - aA]++;
                        break;
                    case L'4':
                        Insertion[InsertionCnt++] = aO2_4;
                        QuestionCnt++;
                        ClickCnt[aO2_4 - aA]++;
                        break;
                    case L'5':
                        Insertion[InsertionCnt++] = aO2_5;
                        QuestionCnt++;
                        ClickCnt[aO2_5 - aA]++;
                        break;
                    case L'6':
                        Insertion[InsertionCnt++] = aO2_6;
                        QuestionCnt++;
                        ClickCnt[aO2_6 - aA]++;
                        break;
                    }
                    break;
                case L'ô':
                    switch (szAnswer[2])
                    {
                    case L'1':
                        Insertion[InsertionCnt++] = aO1_1;
                        QuestionCnt++;
                        ClickCnt[aO1_1 - aA]++;
                        break;
                    case L'2':
                        Insertion[InsertionCnt++] = aO1_2;
                        QuestionCnt++;
                        ClickCnt[aO1_2 - aA]++;
                        break;
                    case L'3':
                        Insertion[InsertionCnt++] = aO1_3;
                        QuestionCnt++;
                        ClickCnt[aO1_3 - aA]++;
                        break;
                    case L'4':
                        Insertion[InsertionCnt++] = aO1_4;
                        QuestionCnt++;
                        ClickCnt[aO1_4 - aA]++;
                        break;
                    case L'5':
                        Insertion[InsertionCnt++] = aO1_5;
                        QuestionCnt++;
                        ClickCnt[aO1_5 - aA]++;
                        break;
                    case L'6':
                        Insertion[InsertionCnt++] = aO1_6;
                        QuestionCnt++;
                        ClickCnt[aO1_6 - aA]++;
                        break;
                    }
                    break;
                case L'u':
                    switch (szAnswer[2])
                    {
                    case L'1':
                        Insertion[InsertionCnt++] = aU;
                        QuestionCnt++;
                        ClickCnt[aU - aA]++;
                        break;
                    case L'2':
                        Insertion[InsertionCnt++] = aU2;
                        QuestionCnt++;
                        ClickCnt[aU2 - aA]++;
                        break;
                    case L'3':
                        Insertion[InsertionCnt++] = aU3;
                        QuestionCnt++;
                        ClickCnt[aU3 - aA]++;
                        break;
                    case L'4':
                        Insertion[InsertionCnt++] = aU4;
                        QuestionCnt++;
                        ClickCnt[aU4 - aA]++;
                        break;
                    case L'5':
                        Insertion[InsertionCnt++] = aU5;
                        QuestionCnt++;
                        ClickCnt[aU5 - aA]++;
                        break;
                    case L'6':
                        Insertion[InsertionCnt++] = aU6;
                        QuestionCnt++;
                        ClickCnt[aU6 - aA]++;
                        break;
                    }

                    switch (szAnswer[3])
                    {
                    case L'1':
                        Insertion[InsertionCnt++] = aU1_1;
                        QuestionCnt++;
                        ClickCnt[aU1_1 - aA]++;
                        break;
                    case L'2':
                        Insertion[InsertionCnt++] = aU1_2;
                        QuestionCnt++;
                        ClickCnt[aU1_2 - aA]++;
                        break;
                    case L'3':
                        Insertion[InsertionCnt++] = aU1_3;
                        QuestionCnt++;
                        ClickCnt[aU1_3 - aA]++;
                        break;
                    case L'4':
                        Insertion[InsertionCnt++] = aU1_4;
                        QuestionCnt++;
                        ClickCnt[aU1_4 - aA]++;
                        break;
                    case L'5':
                        Insertion[InsertionCnt++] = aU1_5;
                        QuestionCnt++;
                        ClickCnt[aU1_5 - aA]++;
                        break;
                    case L'6':
                        Insertion[InsertionCnt++] = aU1_6;
                        QuestionCnt++;
                        ClickCnt[aU1_6 - aA]++;
                        break;
                    }
                    break;
                case L'0':
                    Insertion[InsertionCnt++] = a0;
                    QuestionCnt++;
                    ClickCnt[a0 - aA]++;
                    break;
                case L'1':
                    Insertion[InsertionCnt++] = a1;
                    QuestionCnt++;
                    ClickCnt[a1 - aA]++;
                    break;
                case L'2':
                    Insertion[InsertionCnt++] = a2;
                    QuestionCnt++;
                    ClickCnt[a2 - aA]++;
                    break;
                case L'3':
                    Insertion[InsertionCnt++] = a3;
                    QuestionCnt++;
                    ClickCnt[a3 - aA]++;
                    break;
                case L'4':
                    Insertion[InsertionCnt++] = a4;
                    QuestionCnt++;
                    ClickCnt[a4 - aA]++;
                    break;
                case L'5':
                    Insertion[InsertionCnt++] = a5;
                    QuestionCnt++;
                    ClickCnt[a5 - aA]++;
                    break;
                case L'6':
                    Insertion[InsertionCnt++] = a6;
                    QuestionCnt++;
                    ClickCnt[a6 - aA]++;
                    break;
                case L'7':
                    Insertion[InsertionCnt++] = a7;
                    QuestionCnt++;
                    ClickCnt[a7 - aA]++;
                    break;
                case L'8':
                    Insertion[InsertionCnt++] = a8;
                    QuestionCnt++;
                    ClickCnt[a8 - aA]++;
                    break;
                case L'9':
                    Insertion[InsertionCnt++] = a9;
                    QuestionCnt++;
                    ClickCnt[a9 - aA]++;
                    break;
                }
            }

            count = SendMessage(hList, LB_GETCOUNT, 0, 0);

            while (count > 0)
            {
                count = SendMessage(hList, LB_DELETESTRING, count - 1, 0);
            }

            pressA = 0;
            pressE = 0;
            pressO = 0;
            pressU = 0;

            break;
        case 2: //숫자 추가 버튼
            count = SendMessage(hList, LB_GETCOUNT, 0, 0);

            while (count > 0)
            {
                count = SendMessage(hList, LB_DELETESTRING, count - 1, 0);
            }

            SendMessage(hList, LB_ADDSTRING, 0, (LPARAM)TEXT("0"));
            SendMessage(hList, LB_ADDSTRING, 0, (LPARAM)TEXT("1"));
            SendMessage(hList, LB_ADDSTRING, 0, (LPARAM)TEXT("2"));
            SendMessage(hList, LB_ADDSTRING, 0, (LPARAM)TEXT("3"));
            SendMessage(hList, LB_ADDSTRING, 0, (LPARAM)TEXT("4"));
            SendMessage(hList, LB_ADDSTRING, 0, (LPARAM)TEXT("5"));
            SendMessage(hList, LB_ADDSTRING, 0, (LPARAM)TEXT("6"));
            SendMessage(hList, LB_ADDSTRING, 0, (LPARAM)TEXT("7"));
            SendMessage(hList, LB_ADDSTRING, 0, (LPARAM)TEXT("8"));
            SendMessage(hList, LB_ADDSTRING, 0, (LPARAM)TEXT("9"));

            break;
        case aQ:
            if (bQuestionText)
            {
                Insertion[InsertionCnt++] = aQ;
                QuestionCnt++;
                ClickCnt[aQ - aA]++;
            }
            break;
        case aW:
            if (bQuestionText)
            {
                Insertion[InsertionCnt++] = aW;
                QuestionCnt++;
                ClickCnt[aW - aA]++;
            }
            break;
        case aE:
            count = SendMessage(hList, LB_GETCOUNT, 0, 0);

            while (count > 0)
            {
                count = SendMessage(hList, LB_DELETESTRING, count - 1, 0);
            }
            pressE++;

            switch (pressE)
            {
            case 1:
                SendMessage(hList, LB_ADDSTRING, 0, (LPARAM)TEXT("e 1성"));
                SendMessage(hList, LB_ADDSTRING, 0, (LPARAM)TEXT("e 2성"));
                SendMessage(hList, LB_ADDSTRING, 0, (LPARAM)TEXT("e 3성"));
                SendMessage(hList, LB_ADDSTRING, 0, (LPARAM)TEXT("e 4성"));
                SendMessage(hList, LB_ADDSTRING, 0, (LPARAM)TEXT("e 5성"));
                SendMessage(hList, LB_ADDSTRING, 0, (LPARAM)TEXT("e 6성"));
                break;
            case 2:
                SendMessage(hList, LB_ADDSTRING, 0, (LPARAM)TEXT("ê 1성"));
                SendMessage(hList, LB_ADDSTRING, 0, (LPARAM)TEXT("ê 2성"));
                SendMessage(hList, LB_ADDSTRING, 0, (LPARAM)TEXT("ê 3성"));
                SendMessage(hList, LB_ADDSTRING, 0, (LPARAM)TEXT("ê 4성"));
                SendMessage(hList, LB_ADDSTRING, 0, (LPARAM)TEXT("ê 5성"));
                SendMessage(hList, LB_ADDSTRING, 0, (LPARAM)TEXT("ê 6성"));
            case 3:
                pressE = 0;
                break;
            }
            break;
        case aR:
            if (bQuestionText)
            {
                Insertion[InsertionCnt++] = aR;
                QuestionCnt++;
                ClickCnt[aR - aA]++;
            }
            break;
        case aT:
            if (bQuestionText)
            {
                Insertion[InsertionCnt++] = aT;
                QuestionCnt++;
                ClickCnt[aT - aA]++;
            }
            break;
        case aY:
            if (bQuestionText)
            {
                Insertion[InsertionCnt++] = aY;
                QuestionCnt++;
                ClickCnt[aY - aA]++;
            }
            break;
        case aU:
            count = SendMessage(hList, LB_GETCOUNT, 0, 0);

            while (count > 0)
            {
                count = SendMessage(hList, LB_DELETESTRING, count - 1, 0);
            }

            pressU++;

            switch (pressU)
            {
            case 1:
                SendMessage(hList, LB_ADDSTRING, 0, (LPARAM)TEXT("u 1성"));
                SendMessage(hList, LB_ADDSTRING, 0, (LPARAM)TEXT("u 2성"));
                SendMessage(hList, LB_ADDSTRING, 0, (LPARAM)TEXT("u 3성"));
                SendMessage(hList, LB_ADDSTRING, 0, (LPARAM)TEXT("u 4성"));
                SendMessage(hList, LB_ADDSTRING, 0, (LPARAM)TEXT("u 5성"));
                SendMessage(hList, LB_ADDSTRING, 0, (LPARAM)TEXT("u 6성"));
                break;
            case 2:
                SendMessage(hList, LB_ADDSTRING, 0, (LPARAM)TEXT("u' 1성"));
                SendMessage(hList, LB_ADDSTRING, 0, (LPARAM)TEXT("u' 2성"));
                SendMessage(hList, LB_ADDSTRING, 0, (LPARAM)TEXT("u' 3성"));
                SendMessage(hList, LB_ADDSTRING, 0, (LPARAM)TEXT("u' 4성"));
                SendMessage(hList, LB_ADDSTRING, 0, (LPARAM)TEXT("u' 5성"));
                SendMessage(hList, LB_ADDSTRING, 0, (LPARAM)TEXT("u' 6성"));
            case 3:
                pressU = 0;
                break;
            }
            break;

        case aI:
            count = SendMessage(hList, LB_GETCOUNT, 0, 0);

            while (count > 0)
            {
                count = SendMessage(hList, LB_DELETESTRING, count - 1, 0);
            }

            SendMessage(hList, LB_ADDSTRING, 0, (LPARAM)TEXT("i 1성"));
            SendMessage(hList, LB_ADDSTRING, 0, (LPARAM)TEXT("i 2성"));
            SendMessage(hList, LB_ADDSTRING, 0, (LPARAM)TEXT("i 3성"));
            SendMessage(hList, LB_ADDSTRING, 0, (LPARAM)TEXT("i 4성"));
            SendMessage(hList, LB_ADDSTRING, 0, (LPARAM)TEXT("i 5성"));
            SendMessage(hList, LB_ADDSTRING, 0, (LPARAM)TEXT("i 6성"));
            break;

        case aO:
            count = SendMessage(hList, LB_GETCOUNT, 0, 0);

            while (count > 0)
            {
                count = SendMessage(hList, LB_DELETESTRING, count - 1, 0);
            }

            pressO++;

            switch (pressO)
            {
            case 1:
                SendMessage(hList, LB_ADDSTRING, 0, (LPARAM)TEXT("o 1성"));
                SendMessage(hList, LB_ADDSTRING, 0, (LPARAM)TEXT("o 2성"));
                SendMessage(hList, LB_ADDSTRING, 0, (LPARAM)TEXT("o 3성"));
                SendMessage(hList, LB_ADDSTRING, 0, (LPARAM)TEXT("o 4성"));
                SendMessage(hList, LB_ADDSTRING, 0, (LPARAM)TEXT("o 5성"));
                SendMessage(hList, LB_ADDSTRING, 0, (LPARAM)TEXT("o 6성"));
                break;
            case 2:
                SendMessage(hList, LB_ADDSTRING, 0, (LPARAM)TEXT("ô 1성"));
                SendMessage(hList, LB_ADDSTRING, 0, (LPARAM)TEXT("ô 2성"));
                SendMessage(hList, LB_ADDSTRING, 0, (LPARAM)TEXT("ô 3성"));
                SendMessage(hList, LB_ADDSTRING, 0, (LPARAM)TEXT("ô 4성"));
                SendMessage(hList, LB_ADDSTRING, 0, (LPARAM)TEXT("ô 5성"));
                SendMessage(hList, LB_ADDSTRING, 0, (LPARAM)TEXT("ô 6성"));
                break;
            case 3:
                SendMessage(hList, LB_ADDSTRING, 0, (LPARAM)TEXT("o' 1성"));
                SendMessage(hList, LB_ADDSTRING, 0, (LPARAM)TEXT("o' 2성"));
                SendMessage(hList, LB_ADDSTRING, 0, (LPARAM)TEXT("o' 3성"));
                SendMessage(hList, LB_ADDSTRING, 0, (LPARAM)TEXT("o' 4성"));
                SendMessage(hList, LB_ADDSTRING, 0, (LPARAM)TEXT("o' 5성"));
                SendMessage(hList, LB_ADDSTRING, 0, (LPARAM)TEXT("o' 6성"));
            case 4:
                pressO = 0;
                break;
            }
            break;
        case aP:
            if (bQuestionText)
            {
                Insertion[InsertionCnt++] = aP;
                QuestionCnt++;
                ClickCnt[aP - aA]++;
            }
            break;
        case aA:
            count = SendMessage(hList, LB_GETCOUNT, 0, 0);

            while (count > 0)
            {
                count = SendMessage(hList, LB_DELETESTRING, count - 1, 0);
            }

            pressA++;

            switch (pressA)
            {
            case 1:
                SendMessage(hList, LB_ADDSTRING, 0, (LPARAM)TEXT("a 1성"));
                SendMessage(hList, LB_ADDSTRING, 0, (LPARAM)TEXT("a 2성"));
                SendMessage(hList, LB_ADDSTRING, 0, (LPARAM)TEXT("a 3성"));
                SendMessage(hList, LB_ADDSTRING, 0, (LPARAM)TEXT("a 4성"));
                SendMessage(hList, LB_ADDSTRING, 0, (LPARAM)TEXT("a 5성"));
                SendMessage(hList, LB_ADDSTRING, 0, (LPARAM)TEXT("a 6성"));
                break;
            case 2:
                SendMessage(hList, LB_ADDSTRING, 0, (LPARAM)TEXT("ă 1성"));
                SendMessage(hList, LB_ADDSTRING, 0, (LPARAM)TEXT("ă 2성"));
                SendMessage(hList, LB_ADDSTRING, 0, (LPARAM)TEXT("ă 3성"));
                SendMessage(hList, LB_ADDSTRING, 0, (LPARAM)TEXT("ă 4성"));
                SendMessage(hList, LB_ADDSTRING, 0, (LPARAM)TEXT("ă 5성"));
                SendMessage(hList, LB_ADDSTRING, 0, (LPARAM)TEXT("ă 6성"));
                break;
            case 3:
                SendMessage(hList, LB_ADDSTRING, 0, (LPARAM)TEXT("â 1성"));
                SendMessage(hList, LB_ADDSTRING, 0, (LPARAM)TEXT("â 2성"));
                SendMessage(hList, LB_ADDSTRING, 0, (LPARAM)TEXT("â 3성"));
                SendMessage(hList, LB_ADDSTRING, 0, (LPARAM)TEXT("â 4성"));
                SendMessage(hList, LB_ADDSTRING, 0, (LPARAM)TEXT("â 5성"));
                SendMessage(hList, LB_ADDSTRING, 0, (LPARAM)TEXT("â 6성"));
            case 4:
                pressA = 0;
                break;
            }

            break;
        case aS:
            if (bQuestionText)
            {
                Insertion[InsertionCnt++] = aS;
                QuestionCnt++;
                ClickCnt[aS - aA]++;
            }
            break;
        case aD:
            count = SendMessage(hList, LB_GETCOUNT, 0, 0);

            while (count > 0)
            {
                count = SendMessage(hList, LB_DELETESTRING, count - 1, 0);
            }

            SendMessage(hList, LB_ADDSTRING, 0, (LPARAM)TEXT("d"));
            SendMessage(hList, LB_ADDSTRING, 0, (LPARAM)TEXT("đ"));
            break;
        case aF:
            if (bQuestionText)
            {
                Insertion[InsertionCnt++] = aF;
                QuestionCnt++;
                ClickCnt[aF- aA]++;
            }
            break;
        case aG:
            if (bQuestionText)
            {
                Insertion[InsertionCnt++] = aG;
                QuestionCnt++;
                ClickCnt[aG - aA]++;
            }
            break;
        case aH:
            if (bQuestionText)
            {
                Insertion[InsertionCnt++] = aH;
                QuestionCnt++;
                ClickCnt[aH - aA]++;
            }
            break;
        case aJ:
            if (bQuestionText)
            {
                Insertion[InsertionCnt++] = aJ;
                QuestionCnt++;
                ClickCnt[aJ - aA]++;
            }
            break;
        case aK:
            if (bQuestionText)
            {
                Insertion[InsertionCnt++] = aK;
                QuestionCnt++;
                ClickCnt[aK - aA]++;
            }
            break;
        case aL:
            if (bQuestionText)
            {
                Insertion[InsertionCnt++] = aL;
                QuestionCnt++;
                ClickCnt[aL - aA]++;
            }
            break;
        case aZ:
            if (bQuestionText)
            {
                Insertion[InsertionCnt++] = aZ;
                QuestionCnt++;
                ClickCnt[aZ - aA]++;
            }
            break;
        case aX:
            if (bQuestionText)
            {
                Insertion[InsertionCnt++] = aX;
                QuestionCnt++;
                ClickCnt[aX - aA]++;
            }
            break;
        case aC:
            if (bQuestionText)
            {
                Insertion[InsertionCnt++] = aC;
                QuestionCnt++;
                ClickCnt[aC - aA]++;
            }
            break;
        case aV:
            if (bQuestionText)
            {
                Insertion[InsertionCnt++] = aV;
                QuestionCnt++;
                ClickCnt[aV - aA]++;
            }
            break;
        case aB:
            if (bQuestionText)
            {
                Insertion[InsertionCnt++] = aB;
                QuestionCnt++;
                ClickCnt[aB - aA]++;
            }
            break;
        case aN:
            if (bQuestionText)
            {
                Insertion[InsertionCnt++] = aN;
                QuestionCnt++;
                ClickCnt[aN - aA]++;
            }
            break;
        case aM:
            if (bQuestionText)
            {
                Insertion[InsertionCnt++] = aM;
                QuestionCnt++;
                ClickCnt[aM - aA]++;
            }
            break;
        default:
            count = SendMessage(hList, LB_GETCOUNT, 0, 0);
            while (count > 0)
            {
                count = SendMessage(hList, LB_DELETESTRING, count - 1, 0);
            }
            break;
        }           
        InvalidateRect(hC1, NULL, TRUE);
        break;
    }

    return DefWindowProc(hWnd, message, wParam, lParam);
}


void DrawBitmap(HDC hdc, int x, int y, int w, int h, HBITMAP hbit)
{
    HDC MemDC;
    HBITMAP oldBitmap;
    BITMAP bit;
    int bx, by;

    MemDC = CreateCompatibleDC(hdc);
    oldBitmap = (HBITMAP)SelectObject(MemDC, hbit);

    GetObject(hbit, sizeof(BITMAP), &bit);
    bx = bit.bmWidth;
    by = bit.bmHeight;

    SetStretchBltMode(hdc, HALFTONE);

    StretchBlt(hdc, x, y, w, h, MemDC, 0, 0, bx, by, SRCCOPY);

    SelectObject(MemDC, oldBitmap);
    DeleteDC(MemDC);
}