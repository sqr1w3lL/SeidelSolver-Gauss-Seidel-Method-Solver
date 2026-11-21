#include <iostream>
#include <cmath>
#include <iomanip>
#include <windows.h>
using namespace std;

const int n = 4;

double A[n][n] = {
    {0.63, 1.00, 0.11, 0.34},
    {0.17, 1.18, -0.45, 0.11},
    {0.31, -0.15, 1.17, -2.35},
    {0.58, 0.21, -3.45, -1.18}
};
double B[n] = { 2.08, 0.17, 1.28, 0.05 };

void printMatrix(double M[n][n], double V[n]) {
    cout << fixed << setprecision(3);
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++)
            cout << setw(10) << M[i][j];
        cout << " | " << setw(8) << V[i] << endl;
    }
}

bool checkDiagonalDominance(double M[n][n]) {
    bool isDominant = true;
    cout << "\nПеревірка діагональної домінантності:\n";
    cout << fixed << setprecision(3);

    for (int i = 0; i < n; i++) {
        double diag = fabs(M[i][i]);
        double sumRow = 0.0;
        for (int j = 0; j < n; j++) {
            if (i != j)
                sumRow += fabs(M[i][j]);
        }

        cout << "Рядок " << i + 1 << ": |A[" << i + 1 << "][" << i + 1 << "]| = "
            << diag << ", Σ|інших| = " << sumRow;

        if (diag >= sumRow)
            cout << "  -> виконується\n";
        else {
            cout << "  -> не виконується\n";
            isDominant = false;
        }
    }

    cout << "\n========================\n";
    if (isDominant)
        cout << "Матриця має діагональну домінантність.\n";
    else
        cout << "Матриця не є діагонально домінантною.\n";
    cout << "========================\n";

    return isDominant;
}

void makeDiagonallyDominant(double M[n][n], double V[n]) {
    for (int i = 0; i < n; i++) {
        int maxRow = i;
        double maxVal = fabs(M[i][i]);

        for (int k = i + 1; k < n; k++) {
            if (fabs(M[k][i]) > maxVal) {
                maxVal = fabs(M[k][i]);
                maxRow = k;
            }
        }

        if (maxRow != i) {
            for (int j = 0; j < n; j++)
                swap(M[i][j], M[maxRow][j]);
            swap(V[i], V[maxRow]);
        }
    }
}

// ---------------- Метод Зейделя ----------------
void gaussSeidelMethod(double M[n][n], double V[n]) {
    double X[n] = { 0, 0, 0, 0 };
    double eps = 0.001;
    int iter = 0;
    bool converged = false;

    cout << "\nПочаткове наближення: X = [0, 0, 0, 0]\n";
    cout << "\nІтерації методу Зейделя:\n";

    while (true) {
        iter++;
        double maxDiff = 0.0;

        cout << "Ітерація " << iter << ": ";

        for (int i = 0; i < n; i++) {
            if (M[i][i] == 0) {
                cout << "Помилка: нульовий діагональний елемент у рядку " << i + 1 << "!\n";
                return;
            }

            double sum = 0.0;
            for (int j = 0; j < n; j++) {
                if (i != j)
                    sum += M[i][j] * X[j];
            }

            double X_old = X[i];
            X[i] = (V[i] - sum) / M[i][i];
            double diff = fabs(X[i] - X_old);
            if (diff > maxDiff)
                maxDiff = diff;
        }

        for (int i = 0; i < n; i++)
            cout << "x" << i + 1 << "=" << setw(10) << setprecision(6) << X[i] << "  ";
        cout << endl;

        if (maxDiff < eps) {
            converged = true;
            break;
        }

        if (iter > 10000) break;
    }

    cout << "\n========================\n";
    if (converged) {
        cout << "Метод Зейделя збігся після " << iter << " ітерацій.\n";
        cout << "Розв'язок системи:\n";
        for (int i = 0; i < n; i++)
            cout << "x" << i + 1 << " = " << X[i] << endl;
    }
    else {
        cout << "Метод не збігся (можливо, матриця не діагонально домінантна).\n";
    }
    cout << "========================\n";
}


int main() {
    SetConsoleOutputCP(65001);
    SetConsoleCP(65001);
    setlocale(LC_ALL, "ukr");

    int choice;

    cout << "Початкова матриця A та вектор B:\n";
    printMatrix(A, B);

    cout << "\nПерестановка рядків для покращення збіжності...\n";
    makeDiagonallyDominant(A, B);
    cout << "Результат після перестановки:\n";
    printMatrix(A, B);

    do {
        cout << "\n========== Меню ==========\n";
        cout << "1. Перевірити діагональну домінантність\n";
        cout << "2. Розв'язати систему методом Зейделя\n";
        cout << "3. Вихід\n";
        cout << "==========================\n";
        cout << "Ваш вибір: ";
        cin >> choice;

        switch (choice) {
        case 1:
            checkDiagonalDominance(A);
            break;

        case 2:
            gaussSeidelMethod(A, B);
            break;

        case 3:
            cout << "\nЗавершення роботи програми.\n";
            break;

        default:
            cout << "\nНевірний вибір. Спробуйте ще раз.\n";
        }

    } while (choice != 3);

    return 0;
}
