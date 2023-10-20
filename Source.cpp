#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <locale.h>
#include <time.h>

typedef struct Node {
    int inf;
    struct Node* next;
}Node;

void addSp(Node** arr_sp, int** arr, int rows) {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < rows; j++) {
            if (arr[i][j] == 1) {
                Node* newNode = (Node*)malloc(sizeof(Node));
                newNode->inf = j + 1;
                newNode->next = arr_sp[i];
                arr_sp[i] = newNode;
            }
        }
    }
}

void printSp(Node** arr_sp, int rows) {
    for (int i = 0; i < rows; i++) {
        printf("Вершина %d смежна с: ", i + 1);
        Node* current = arr_sp[i];
        while (current != NULL) {
            printf("%d ", current->inf);
            current = current->next;
        }
        printf("\n");
    }
}

void freeList(Node* head) {
    Node* current = head;
    while (current != NULL) {
        Node* temp = current;
        current = current->next;
        free(temp);
    }
}

void mergeRowsSp(Node** arr_sp, int rows, int rows1, int rows2) {
    if (rows1 >= rows || rows2 >= rows || rows1 < 0 || rows2 < 0) {
        printf("Ошибка: некорректные номера вершин!\n");
        return;
    }
    Node* current = arr_sp[rows2];
    while (current != NULL) {
        int temp = current->inf;
        if (temp != rows1+1) {
            Node* check = arr_sp[rows1];
            bool found = false;
            while (check != NULL) {
                if (check->inf == temp) {
                    found = true;
                    break;
                }
                check = check->next;
            }
            if (!found) {
                Node* newNode = (Node*)malloc(sizeof(Node));
                newNode->inf = temp;
                newNode->next = arr_sp[rows1];
                arr_sp[rows1] = newNode;
            }
        }
        current = current->next;
    }
    // Удаление вершины rows2 из списков смежности
    freeList(arr_sp[rows2]);
    arr_sp[rows2] = NULL;
}

void deleteSp(Node** arr_sp, int rows, int rows1) {
    
}

void contractEdgeSp(Node** arr_sp, int rows, int rows1, int rows2) {
    if (rows1 >= rows || rows2 >= rows || rows1 < 0 || rows2 < 0) {
        printf("Ошибка: некорректные номера вершин!\n");
        return;
    }
    int k = 0;
    Node* current = arr_sp[rows2];
    while (current != NULL) {
        if (current->inf == rows1+1) {
            k++;
        }
        current = current->next;
    }
    if (k == 0) {
        printf("Введите вершины между которыми есть ребро!\n");
        return;
    }
    Node* current_2 = arr_sp[rows2];
    while (current_2 != NULL) {
        int temp = current_2->inf;
        if (temp != rows1+1) {
            Node* newNode = (Node*)malloc(sizeof(Node));
            newNode->inf = temp;
            newNode->next = arr_sp[rows1];
            arr_sp[rows1] = newNode;
        }
        current_2 = current_2->next;
    }
    // Удаление вершины rows2 из списков смежности
    freeList(arr_sp[rows2]);
    arr_sp[rows2] = NULL;
}

void mergeRows(int** arr, int rows, int rows1, int rows2) {
    // Отождествление вершин
    if (rows1 >= rows || rows2 >= rows || rows1 < 0 || rows2 < 0) {
        printf("Ошибка: некорректные номера вершин!\n");
        return;
    }
    for (int i = 0; i < rows; i++) {
        if (i != rows1 && arr[rows2][i] == 1) {
            arr[rows1][i] = 1;
            arr[i][rows1] = 1;
        }
    }
    for (int i = 0; i < rows; i++) {
        if (i == rows1 && arr[rows2][i] == 1) {
            arr[rows1][i] = 1;
        }
        arr[i][rows2] = 0;
        arr[rows2][i] = 0;
    }
}

void contractEdge(int** arr, int rows, int rows1, int rows2) {
    // Стягивание ребра
    if (rows1 >= rows || rows2 >= rows || rows1 < 0 || rows2 < 0) {
        printf("Ошибка: некорректные номера вершин!\n");
        return;
    }
    for (int i = 0; i < rows; i++) {
        if (i != rows1 && i != rows2) {
            arr[rows1][i] = arr[rows1][i] || arr[rows2][i];
            arr[i][rows1] = arr[rows1][i];
        }
    }
    
    for (int i = 0; i < rows; i++) {
        arr[i][rows2] = 0;
        arr[rows2][i] = 0;
    }
}

void deleteRows(int** arr, int rows, int rows1, Node** arr_sp) {
    // расщепление вершины
    int new_rows = rows + 1;
    int** arr_split = (int**)malloc(new_rows * sizeof(int*));
    for (int i = 0; i < new_rows; i++) {
        arr_split[i] = (int*)malloc(new_rows * sizeof(int));

        for (int j = 0; j < new_rows; j++) {
            if (i < rows && j < rows) {
                arr_split[i][j] = arr[i][j];  // копируем связи исходной матрицы
            }
            else if (i == rows && j < rows) {
                arr_split[i][j] = arr[rows1][j];  // новая вершина соединена со смежными вершинами выбранной вершины
            }
            else if (i < rows && j == rows) {
                arr_split[i][j] = arr[i][rows1];  // смежные вершины соединены с новой вершиной
            }
            else {
                arr_split[i][j] = 0;  // новая вершина не связана с остальными вершинами
            }
        }
    }
    printf("Результат расщепления вершины:\n");
    for (int i = 0; i < new_rows; i++) {
        for (int j = 0; j < new_rows; j++) {
            printf("%-2d ", arr_split[i][j]);
        }
        printf("\n");
    }
    for (int i = 0; i < rows; i++) {
        freeList(arr_sp[i]);
        arr_sp[i] = NULL;
    }
    addSp(arr_sp, arr_split, new_rows);
}

void combineArr(int** arr, int** arr_2, int rows, int rows_2) { // обьединение графов
    int max_rows = (rows > rows_2) ? rows : rows_2;  // максимальный размер для объединенного массива
    int** arr_union = (int**)malloc(max_rows * sizeof(int*));
    for (int i = 0; i < max_rows; i++) {
        arr_union[i] = (int*)malloc(max_rows * sizeof(int));
        for (int j = 0; j < max_rows; j++) {
            int val_1 = (i < rows && j < rows) ? arr[i][j] : 0;  // значение из arr, если индексы в пределах размеров массива
            int val_2 = (i < rows_2 && j < rows_2) ? arr_2[i][j] : 0;  // значение из arr_2, если индексы в пределах размеров массива
            arr_union[i][j] = val_1 || val_2;  // логическое ИЛИ значений
        }
    }

    printf("Результат объединения графов:\n");
    for (int i = 0; i < max_rows; i++) {
        for (int j = 0; j < max_rows; j++) {
            printf("%-2d ", arr_union[i][j]);
        }
        printf("\n");
    }
}

void crossArr(int** arr, int** arr_2, int rows, int rows_2) { // пересечение графов
    int max_rows = (rows > rows_2) ? rows : rows_2;
    int** arr_intersect = (int**)malloc(max_rows * sizeof(int*));
    for (int i = 0; i < max_rows; i++) {
        arr_intersect[i] = (int*)malloc(max_rows * sizeof(int));

        for (int j = 0; j < max_rows; j++) {
            int val_1 = (i < rows && j < rows) ? arr[i][j] : 0;
            int val_2 = (i < rows_2 && j < rows_2) ? arr_2[i][j] : 0;
            arr_intersect[i][j] = val_1 && val_2;
        }
    }
    printf("Результат пересечения графов:\n");
    for (int i = 0; i < max_rows; i++) {
        for (int j = 0; j < max_rows; j++) {
            printf("%-2d ", arr_intersect[i][j]);
        }
        printf("\n");
    }
}

void sumArr(int** arr, int** arr_2, int rows, int rows_2) { // кольцевая сумма
    int max_rows = (rows > rows_2) ? rows : rows_2;
    int** arr_ring_sum = (int**)malloc(max_rows * sizeof(int*));
    for (int i = 0; i < max_rows; i++) {
        arr_ring_sum[i] = (int*)malloc(max_rows * sizeof(int));

        for (int j = 0; j < max_rows; j++) {
            if (i == j) {
                arr_ring_sum[i][j] = 0;
            }
            else {
                int val_1 = (i < rows && j < rows) ? arr[i][j] : 0;
                int val_2 = (i < rows_2 && j < rows_2) ? arr_2[i][j] : 0;
                arr_ring_sum[i][j] = val_1 || val_2;
            }
        }
    }
    printf("Результат кольцевой суммы графов:\n");
    for (int i = 0; i < max_rows; i++) {
        for (int j = 0; j < max_rows; j++) {
            printf("%-2d ", arr_ring_sum[i][j]);
        }
        printf("\n");
    }
}

void decartArr(int** arr, int** arr_2, int rows, int rows_2) { // декартово произведение
    int decart_rows = rows * rows_2;
    int decart_cols = rows * rows_2;

    int** arr_decart = (int**)malloc(decart_rows * sizeof(int*));
    for (int i = 0; i < decart_rows; i++) {
        arr_decart[i] = (int*)malloc(decart_cols * sizeof(int));

        for (int j = 0; j < decart_cols; j++) {
            int u = i / rows;  // вершина из G1
            int v = j / rows_2;  // вершина из G2
            int u_temp = i % rows;  // сосед из G1
            int v_temp = j % rows_2;  // сосед из G2

            // Если вершины u и v соединены в G1 и вершины u_temp и v_temp соединены в G2,
            // устанавливаем соответствующее ребро в G
            if (arr[u_temp][u] == 1 && arr_2[v_temp][v] == 1) {
                arr_decart[i][j] = 1;
            }
            else {
                arr_decart[i][j] = 0;
            }
        }
    }
    printf("Результат декартового произведения графов:\n");
    for (int i = 0; i < decart_rows; i++) {
        for (int j = 0; j < decart_cols; j++) {
            printf("%-2d ", arr_decart[i][j]);
        }
        printf("\n");
    }
}

void main() {

    setlocale(LC_ALL, "RUS");
    srand(time(NULL));
    int** arr = 0; // матрица смежности
    int** arr_2 = 0; // матрица смежности 2
    int i, j, rows, rows_2, k, unit = 0;
    printf("Введите количество вершин графа: ");
    scanf("%d", &rows);
    printf("Введите количество вершин графа: ");
    scanf("%d", &rows_2);
    Node** arr_sp = NULL;
    Node** arr_sp_2 = NULL;
    // Выделение памяти для массива списков смежности
    arr_sp = (Node**)malloc(rows * sizeof(Node*));
    if (arr_sp == NULL) {
        printf("Не удалось выделить память!\n");
        return;
    }
    for (i = 0; i < rows; i++) {
        arr_sp[i] = NULL;
    }
    arr_sp_2 = (Node**)malloc(rows_2 * sizeof(Node*));
    if (arr_sp_2 == NULL) {
        printf("Не удалось выделить память!\n");
        return;
    }
    for (i = 0; i < rows_2; i++) {
        arr_sp_2[i] = NULL;
    }
    arr = (int**)malloc(rows * sizeof(int*));
    if (arr == NULL) {
        printf("Не удалось выделить память!\n");
        return;
    }
    for (i = 0; i < rows; i++) {
        arr[i] = (int*)malloc(rows * sizeof(int));
    }
    // генерировать случайные значения для матрицы смежности
    for (i = 0; i < rows; i++) {
        for (j = i; j < rows; j++) {
            if (i == j) {
                arr[i][j] = 0; // на главной диагонали нули
            }
            else {
                arr[i][j] = rand() % 2; // случайные значения 0 или 1
                arr[j][i] = arr[i][j]; // симметрично заполнять значения для неориентированного графа
            }
        }
    }

    // выводить матрицу смежности на экран
    printf("Матрица смежности для графа №1:\n");
    for (i = 0; i < rows; i++) {
        for (j = 0; j < rows; j++) {
            printf("%-2d ", arr[i][j]);
        }
        printf("\n");
    }

    printf("Списки смежности для графа №1:\n");
    addSp(arr_sp, arr, rows);
    printSp(arr_sp, rows);

    arr_2 = (int**)malloc(rows_2 * sizeof(int*));
    if (arr_2 == NULL) {
        printf("Не удалось выделить память!\n");
        return;
    }
    for (i = 0; i < rows_2; i++) {
        arr_2[i] = (int*)malloc(rows_2 * sizeof(int));
    }

    // генерировать случайные значения для матрицы смежности 2
    for (i = 0; i < rows_2; i++) {
        for (j = i; j < rows_2; j++) {
            if (i == j) {
                arr_2[i][j] = 0; // на главной диагонали нули
            }
            else {
                arr_2[i][j] = rand() % 2; // случайные значения 0 или 1
                arr_2[j][i] = arr_2[i][j]; // симметрично заполнять значения для неориентированного графа
            }
        }
    }

    printf("Матрица смежности для графа №2:\n");
    // выводить матрицу смежности на экран
    for (i = 0; i < rows_2; i++) {
        for (j = 0; j < rows_2; j++) {
            printf("%-2d ", arr_2[i][j]);
        }
        printf("\n");
    }
    printf("Списки смежности для графа №2:\n");
    addSp(arr_sp_2, arr_2, rows_2);
    printSp(arr_sp_2, rows_2);

    int choice, rows1, rows2, what;
    do {
        printf("1. Отождествить вершины\n");
        printf("2. Стянуть ребро\n");
        printf("3. Расщипить вершину\n");
        printf("4. Обьеденить графы\n");
        printf("5. Выполнить операцию пересечения графов\n");
        printf("6. Найти кольцевую сумму\n");
        printf("7. Найти декартово произведение\n");
        printf("8. Просмотреть массивы\n");
        printf("9. Выход\n");
        printf("Выберите действие: ");
        scanf("%d", &choice);
        switch (choice) {
        case 1:
            printf("Выберите в каком массиве хотите это сделать (1 или 2): ");
            scanf("%d", &what);
            if (what == 1) {
                printf("Введите 1 вершину, которую хотите отождествить: ");
                scanf("%d", &rows1);
                printf("Введите 2 вершину, которую хотите отождествить: ");
                scanf("%d", &rows2);
                mergeRows(arr, rows, rows1 - 1, rows2 - 1);
                printf("Вершины %d и %d успешно отождествлены.\n", rows1, rows2);
                for (i = 0; i < rows; i++) {
                    for (j = 0; j < rows; j++) {
                        printf("%-2d ", arr[i][j]);
                    }
                    printf("\n");
                }
                mergeRowsSp(arr_sp, rows, rows1 - 1, rows2 - 1);
                printSp(arr_sp, rows);
            }
            else if (what == 2) {
                printf("Введите 1 вершину, которую хотите отождествить: ");
                scanf("%d", &rows1);
                printf("Введите 2 вершину, которую хотите отождествить: ");
                scanf("%d", &rows2);
                mergeRows(arr_2, rows_2, rows1 - 1, rows2 - 1);
                printf("Вершины %d и %d успешно отождествлены.\n", rows1, rows2);
                for (i = 0; i < rows_2; i++) {
                    for (j = 0; j < rows_2; j++) {
                        printf("%-2d ", arr_2[i][j]);
                    }
                    printf("\n");
                }
                mergeRowsSp(arr_sp_2, rows_2, rows1 - 1, rows2 - 1);
                printSp(arr_sp_2, rows_2);
            }
            else {
                printf("Некорректный выбор!\n");
                break;
            }
            break;
        case 2:
            printf("Выберите в каком массиве хотите это сделать (1 или 2): ");
            scanf("%d", &what);
            if (what == 1) {
                printf("Введите 1 вершину, между которыми нужно стянуть ребро: ");
                scanf("%d", &rows1);
                printf("Введите 2 вершину, между которыми нужно стянуть ребро: ");
                scanf("%d", &rows2);
                contractEdge(arr, rows, rows1 - 1, rows2 - 1);
                printf("Ребро между вершинами %d и %d успешно стянуто.\n", rows1, rows2);
                for (i = 0; i < rows; i++) {
                    for (j = 0; j < rows; j++) {
                        printf("%-2d ", arr[i][j]);
                    }
                    printf("\n");
                }
                contractEdgeSp(arr_sp, rows, rows1 - 1, rows2 - 1);
                printSp(arr_sp, rows);
            }
            else if (what == 2) {
                printf("Введите 1 вершину, между которыми нужно стянуть ребро: ");
                scanf("%d", &rows1);
                printf("Введите 2 вершину, между которыми нужно стянуть ребро: ");
                scanf("%d", &rows2);
                contractEdge(arr_2, rows_2, rows1 - 1, rows2 - 1);
                printf("Ребро между вершинами %d и %d успешно стянуто.\n", rows1, rows2);
                for (i = 0; i < rows_2; i++) {
                    for (j = 0; j < rows_2; j++) {
                        printf("%-2d ", arr_2[i][j]);
                    }
                    printf("\n");
                }
                contractEdgeSp(arr_sp_2, rows_2, rows1 - 1, rows2 - 1);
                printSp(arr_sp_2, rows_2);
            }
            else {
                printf("Некорректный выбор!\n");
                break;
            }
            break;
        case 3:

            printf("Выберите в каком массиве хотите это сделать (1 или 2): ");
            scanf("%d", &what);
            if (what == 1) {
                printf("Введите вершину, которую хотите расщипить: ");
                scanf("%d", &rows1);
                arr_sp = (Node**)malloc((rows+1) * sizeof(Node*));
                if (arr_sp == NULL) {
                    printf("Не удалось выделить память!\n");
                    return;
                }
                for (i = 0; i < rows + 1; i++) {
                    arr_sp[i] = NULL;
                }
                deleteRows(arr, rows, rows1 - 1, arr_sp);
                printSp(arr_sp, rows+1);
            }
            else if (what == 2) {
                printf("Введите вершину, которую хотите расщипить: ");
                scanf("%d", &rows1);
                arr_sp_2 = (Node**)malloc((rows_2+1) * sizeof(Node*));
                if (arr_sp_2 == NULL) {
                    printf("Не удалось выделить память!\n");
                    return;
                }
                for (i = 0; i < rows_2+1; i++) {
                    arr_sp_2[i] = NULL;
                }
                deleteRows(arr_2, rows_2, rows1 - 1, arr_sp_2);
                printSp(arr_sp, rows_2+1);
            }
            else {
                printf("Некорректный выбор!\n");
                break;
            }
            break;
        case 4:
            combineArr(arr, arr_2, rows, rows_2);
            break;
        case 5:
            crossArr(arr, arr_2, rows, rows_2);
            break;
        case 6:
            sumArr(arr, arr_2, rows, rows_2);
            break;
        case 7:
            decartArr(arr, arr_2, rows, rows_2);
            break;
        case 8:
            printf("Матрица смежности для графа №1:\n");
            for (i = 0; i < rows; i++) {
                for (j = 0; j < rows; j++) {
                    printf("%-2d ", arr[i][j]);
                }
                printf("\n");
            }
            printSp(arr_sp, rows);
            printf("Матрица смежности для графа №2:\n");
            // выводить матрицу смежности на экран
            for (i = 0; i < rows; i++) {
                for (j = 0; j < rows; j++) {
                    printf("%-2d ", arr_2[i][j]);
                }
                printf("\n");
            }
            printSp(arr_sp_2, rows);
            break;
        case 9:
            system("cls");
            printf("До свидания!\n");
            break;
        default:
            printf("Некорректный выбор!\n");
            break;
        }
    } while (choice != 9);

    // Освобождение памяти
    for (i = 0; i < rows; i++) {
        free(arr[i]);
    }
    free(arr);

    for (i = 0; i < rows_2; i++) {
        free(arr_2[i]);
    }
    free(arr_2);
}