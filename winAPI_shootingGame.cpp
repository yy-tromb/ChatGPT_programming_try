#include <Windows.h>

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

HBITMAP hbmBack = NULL;
int cxClient, cyClient;

HBITMAP hbmPlayer = NULL;
int cxPlayer, cyPlayer, xPlayer, yPlayer;

void DrawPlayer(HDC hdc)
{
    HDC hdcMem = CreateCompatibleDC(hdc);
    HBITMAP hbmOld = (HBITMAP)SelectObject(hdcMem, hbmPlayer);

    BitBlt(hdc, xPlayer, yPlayer, cxPlayer, cyPlayer, hdcMem, 0, 0, SRCCOPY);

    SelectObject(hdcMem, hbmOld);
    DeleteDC(hdcMem);
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
    WNDCLASSEX wc;
    HWND hwnd;
    MSG msg;

    ZeroMemory(&wc, sizeof(wc));
    wc.cbSize = sizeof(wc);
    wc.style = CS_HREDRAW | CS_VREDRAW;
    wc.lpfnWndProc = WndProc;
    wc.hInstance = hInstance;
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wc.lpszClassName = TEXT("WindowClass");

    if (!RegisterClassEx(&wc))
    {
        MessageBox(NULL, TEXT("Window Registration Failed!"), TEXT("Error"), MB_ICONERROR | MB_OK);
        return 0;
    }

    hwnd = CreateWindowEx(0,
        TEXT("WindowClass"),
        TEXT("My Shooting Game"),
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT, 800, 600,
        NULL,
        NULL,
        hInstance,
        NULL);

    if (hwnd == NULL)
    {
        MessageBox(NULL, TEXT("Window Creation Failed!"), TEXT("Error"), MB_ICONERROR | MB_OK);
        return 0;
    }

    ShowWindow(hwnd, nCmdShow);
    UpdateWindow(hwnd);

    while (GetMessage(&msg, NULL, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return (int)msg.wParam;
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    switch (msg)
    {
    case WM_CREATE:
        hbmBack = (HBITMAP)LoadImage(NULL, TEXT("background.bmp"), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
        if (hbmBack == NULL)
            MessageBox(hwnd, TEXT("Could not load background.bmp!"), TEXT("Error"), MB_ICONERROR | MB_OK);
          hbmPlayer = (HBITMAP)LoadImage(NULL, TEXT("player.bmp"), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
    if (hbmPlayer == NULL)
        MessageBox(hwnd, TEXT("Could not load player.bmp!"), TEXT("Error"), MB_ICONERROR | MB_OK);

    BITMAP bm;
    GetObject(hbmPlayer, sizeof(bm), &bm);
    cxPlayer = bm.bmWidth;
    cyPlayer = bm.bmHeight;
    xPlayer = (cxClient - cxPlayer) / 2;
    yPlayer = cyClient - cyPlayer;
    break;

   

    case WM_SIZE:
    cxClient = LOWORD(lParam);
    cyClient = HIWORD(lParam);
    xPlayer = (cxClient - cxPlayer) / 2;
    yPlayer = cyClient - cyPlayer;
    break;

case WM_PAINT:
    {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hwnd, &ps);

        HDC hdcMem = CreateCompatibleDC(hdc);
        HBITMAP hbmOld = (HBITMAP)SelectObject(hdcMem, hbmBack);

        BitBlt(hdc, 0, 0, cxClient, cyClient, hdcMem, 0, 0, SRCCOPY);

        SelectObject(hdcMem, hbmOld);
        DeleteDC(hdcMem);

        DrawPlayer(hdc);

        EndPaint(hwnd, &ps);
    }
    break;


    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    }

    return DefWindowProc(hwnd, msg, wParam, lParam);
}
