#include <windows.h>
#include <string>
#include <sstream>

// Constantes
#define IDC_1RM 1001
#define IDC_RESULTADO 1002
#define IDC_BOTON_BASE 1100  // ID base para botones de porcentaje

// Variables globales
HWND h1RM, hResultado;

// Función para calcular y mostrar resultado
void CalcularPorcentaje(HWND hwnd, float porcentaje) {
    char pesoStr[20];
    GetWindowText(h1RM, pesoStr, 20);
    float peso = atof(pesoStr);

    float resultado = peso * (porcentaje / 100.0f);

    std::ostringstream oss;
    oss.precision(2);
    oss << std::fixed;
    oss << porcentaje << "% de " << peso << " kg (1RM) es: " << resultado << " kg";

    SetWindowText(hResultado, oss.str().c_str());
}

// Procedimiento de ventana
LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    switch (msg) {
        case WM_CREATE: {
            // Campo para ingresar 1RM
            CreateWindow("STATIC", "Peso (kg):", WS_CHILD | WS_VISIBLE,
                         20, 20, 100, 25, hwnd, NULL, NULL, NULL);
            CreateWindow("EDIT", "", WS_CHILD | WS_VISIBLE | WS_BORDER,
                         120, 20, 100, 25, hwnd, (HMENU)IDC_1RM, NULL, NULL);
            h1RM = GetDlgItem(hwnd, IDC_1RM);

            // Botones de porcentaje del 50% al 120%
            int x = 20, y = 60;
            for (int pct = 50; pct <= 120; pct += 5) {
                std::ostringstream oss;
                oss << pct << "%";
                CreateWindow("BUTTON", oss.str().c_str(),
                             WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
                             x, y, 50, 25, hwnd, (HMENU)(IDC_BOTON_BASE + pct), NULL, NULL);
                x += 55;
                if (x > 300) {
                    x = 20;
                    y += 35;
                }
            }

            // Área para mostrar resultado
            CreateWindow("STATIC", "Resultado:", WS_CHILD | WS_VISIBLE,
                         20, y + 30, 100, 25, hwnd, NULL, NULL, NULL);

            CreateWindow("EDIT", "", WS_CHILD | WS_VISIBLE | WS_BORDER | ES_MULTILINE | ES_READONLY,
                         20, y + 60, 330, 60, hwnd, (HMENU)IDC_RESULTADO, NULL, NULL);
            hResultado = GetDlgItem(hwnd, IDC_RESULTADO);
            break;
        }

        case WM_COMMAND: {
            int id = LOWORD(wParam);
            if (id >= IDC_BOTON_BASE + 50 && id <= IDC_BOTON_BASE + 120) {
                int porcentaje = id - IDC_BOTON_BASE;
                CalcularPorcentaje(hwnd, (float)porcentaje);
            }
            break;
        }

        case WM_DESTROY:
            PostQuitMessage(0);
            break;

        default:
            return DefWindowProc(hwnd, msg, wParam, lParam);
    }
    return 0;
}

// Función principal
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
    WNDCLASS wc = {0};
    wc.lpfnWndProc = WndProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = "1RMCalculator";
    wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    RegisterClass(&wc);

    HWND hwnd = CreateWindow("1RMCalculator", "Calculadora de Porcentajes de 1RM",
                             WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT,
                             400, 400, NULL, NULL, hInstance, NULL);

    ShowWindow(hwnd, nCmdShow);
    UpdateWindow(hwnd);

    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return msg.wParam;
}
