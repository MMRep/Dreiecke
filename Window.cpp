#include "Window.hpp"

static HWND hwnd;
static bool bDrawAll = true;

static HBRUSH brWhite = CreateSolidBrush(RGB(255, 255, 255));
static HBRUSH brBlack = CreateSolidBrush(RGB(0, 0, 0));
static HBRUSH brRed = CreateSolidBrush(RGB(255, 0, 0));

#define WIDTH (200)
#define HEIGHT (200)
#define MP (4) // multiplier

static COLORREF colors[] = {
    RGB(0xFF, 0x00, 0x00),
    RGB(0x00, 0xFF, 0x00),
    RGB(0xFF, 0xFF, 0x00),
    RGB(0x00, 0x00, 0xFF),
    RGB(0xFF, 0x00, 0xFF),
    RGB(0x00, 0xFF, 0xFF),
    RGB(0x8B, 0x45, 0x13),
    RGB(0x8A, 0x2B, 0xE2),
    RGB(0x4B, 0x00, 0x82),
    RGB(0xF0, 0x80, 0x80),
    RGB(0xFF, 0xDE, 0xAD),
    RGB(0xFF, 0xD7, 0x00),
    RGB(0xFF, 0x63, 0x47),
    RGB(0xFF, 0x23, 0xA7),
    RGB(0xFF, 0x00, 0x00),
    RGB(0x00, 0xFF, 0x00),
    RGB(0xFF, 0xFF, 0x00),
    RGB(0x00, 0x00, 0xFF),
    RGB(0xFF, 0x00, 0xFF),
    RGB(0x00, 0xFF, 0xFF),
    RGB(0x8B, 0x45, 0x13),
    RGB(0x8A, 0x2B, 0xE2),
    RGB(0x4B, 0x00, 0x82),
    RGB(0xF0, 0x80, 0x80),
    RGB(0xFF, 0xDE, 0xAD),
    RGB(0xFF, 0xD7, 0x00),
    RGB(0xFF, 0x63, 0x47),
    RGB(0xFF, 0x23, 0xA7)
};

static void drawLine(HDC hdc, const Point& A, const Point& B) {
    MoveToEx(hdc, (int)(A.x), int(A.y), NULL);
    LineTo(hdc, (int)(B.x), int(B.y));
}

static void drawText(HDC hdc, int x, int y, const char* format, ...) {
    char buffer[512];
    va_list args;
    RECT rect;

    rect.top = 0 + y + MP;
    rect.left = 0 + x + MP;
    rect.bottom =  55 + y;
    rect.right = 100 + x;

    Rectangle(hdc, x, y, 100+x+MP, 55+y+MP);

    va_start(args, format);
    vsprintf_s<512>(buffer, format, args);
    va_end(args);
    DrawTextA(hdc, buffer, -1, &rect, DT_LEFT | DT_TOP);
}

static void setColor(HDC hdc, COLORREF color) {
    LOGBRUSH lb;
    HPEN pen;
    HPEN penOld;
    lb.lbColor = color;
    lb.lbStyle = BS_SOLID;
    pen = ExtCreatePen(PS_SOLID | PS_ENDCAP_ROUND | PS_GEOMETRIC, 1*MP, &lb, 0, NULL);
    penOld = (HPEN)SelectObject(hdc, pen);
    DeleteObject(penOld);
}

LRESULT CALLBACK WindowProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    static int Cnt = 0;
    static int text_pos_x = 0;
    static int text_pos_y = 0;
    PAINTSTRUCT ps;
    RECT rect;
    HDC hdc;

    switch (msg) {
    case WM_PAINT:
        hdc = BeginPaint(hwnd, &ps);

        GetClientRect(hwnd, &rect);
        FillRect(hdc, &rect, brWhite);

        setColor(hdc, RGB(0, 0, 0));
        for (size_t i = 0; i < lines.size(); ++i) {
            drawLine(hdc, Point(lines[i].A.x*MP + rect.left, rect.bottom - lines[i].A.y*MP), Point(lines[i].B.x*MP + rect.left, rect.bottom - lines[i].B.y*MP));
        }

        for (size_t i = 0; i < triangles.size(); ++i) {
            if (bDrawAll == false && (Cnt % triangles.size()) != i) {
                continue;
            }
            setColor(hdc, colors[i]);
            drawLine(hdc, Point(triangles[i].A.x*MP + rect.left, rect.bottom - triangles[i].A.y*MP), Point(triangles[i].B.x*MP + rect.left, rect.bottom - triangles[i].B.y*MP));
            drawLine(hdc, Point(triangles[i].B.x*MP + rect.left, rect.bottom - triangles[i].B.y*MP), Point(triangles[i].C.x*MP + rect.left, rect.bottom - triangles[i].C.y*MP));
            drawLine(hdc, Point(triangles[i].C.x*MP + rect.left, rect.bottom - triangles[i].C.y*MP), Point(triangles[i].A.x*MP + rect.left, rect.bottom - triangles[i].A.y*MP));
        }

        if (bDrawAll == false) {
            Triangle& T = triangles[Cnt % triangles.size()];
            drawText(hdc, text_pos_x, text_pos_y, "%6.2f %6.2f\n%6.2f %6.2f\n%6.2f %6.2f", T.A.x, T.A.y, T.B.x, T.B.y, T.C.x, T.C.y);
        }

        break;
    case WM_KEYDOWN:
        if (wParam == VK_ESCAPE) {
            PostQuitMessage(0);
            break;
        } else if (wParam == VK_SPACE) {
            bDrawAll = !bDrawAll;
        } else if (wParam == VK_UP) {
            text_pos_y -= 4;
        } else if (wParam == VK_DOWN) {
            text_pos_y += 4;
        } else if (wParam == VK_LEFT) {
            text_pos_x -= 4;
        } else if (wParam == VK_RIGHT) {
            text_pos_x += 4;
        } else if (bDrawAll == false) {
            Cnt++;
        } else {
            break;
        }

        GetClientRect(hwnd, &rect);
        RedrawWindow(hwnd, &rect, NULL, RDW_INVALIDATE);
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hwnd, msg, wParam, lParam);
    }
    return 0;
}

void InitWindow() {
    WNDCLASSEX wc;
    HINSTANCE hInstance;

    hInstance = GetModuleHandle(NULL);  

    wc.cbClsExtra = 0;
    wc.cbSize = sizeof(WNDCLASSEX);
    wc.cbWndExtra = 0;
    wc.hbrBackground = brWhite;
    wc.hCursor = LoadCursor(hInstance, IDC_ARROW);
    wc.hIcon = LoadIcon(hInstance, IDI_APPLICATION);
    wc.hIconSm = wc.hIcon;
#
    wc.hInstance = hInstance;
    wc.lpfnWndProc = WindowProc;
    wc.lpszClassName = L"AWEProject";
    wc.lpszMenuName = NULL;
    wc.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
    
    ATOM a = RegisterClassEx(&wc);
    hwnd = CreateWindowEx(WS_EX_CLIENTEDGE, L"AWEProject", L"Dreiecke", WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, WIDTH*MP+20, HEIGHT*MP+43, 0, 0, hInstance, NULL);
    if (hwnd == NULL) {
        DWORD result = GetLastError();
        result = result;
    }
}

void StartWindow() {
    MSG msg;
    ShowWindow(hwnd, SW_SHOW);
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
}