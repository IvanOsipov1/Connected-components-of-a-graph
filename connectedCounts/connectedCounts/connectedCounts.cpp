#include <iostream>
#include <fstream>

using namespace std;

bool inLines(int* arr, int item, int SIZE) {
    for (int k = 0; k < SIZE; k++) { // Функция, проверющая наличие номера строки
        if (arr[k] == item) {             // в массиве
            return true;
        }
    }
    return false;
}

class Graph {                  // Класс графа, содержащий в себе различные объекты

private:

    int size;                 //размер двумерного массива, который будет инициализирован в методе
    bool** matrix;            //двумерный массив (матрица смежности)
    int CountComponent = 0;   //кол-во компонент

    ifstream fin;
    ofstream fout;


public:

    ~Graph();                 //деструктор класса, освобождающий память от двумерного массива и закрывающий файлы

    void SearchComponents();  // метод поиска компонент связности, выводит в консоль кол-во компонент связности
    void CreatingFile();      // метод открывющий файл с матрицей и преобразующий из нее дополнение заданного матрицей графа

};

void Graph::CreatingFile() {


    fin.open("Matrix.txt");
    if (!fin.is_open()) {
        cout << "Error file opening" << endl; // если файл не удалось открыть, программа завершается
        exit(1);

    }
    else {
        cout << "File open" << endl;
        char ElementOfMatrix;                   //элемент матрицы в файле
        int countLF = 1;
        while (fin.get(ElementOfMatrix)) {
            if (int(ElementOfMatrix) == 10) { // если элемент матрицы равен символу отступа, то увеличиваем счетчик
                countLF++;                    // строк в матрице
            }
        }
        this->size = countLF;                   //количество строк это и есть размер матрицы (в том числе кол-во столбцов)

        fin.clear();
        fin.seekg(0, fin.beg);                  //методы, позволяющие заново прочитать файл

        int i = 0;                              //счетчик строк
        int j = 0;                              //счетчик столбцов
        matrix = new bool* [countLF];
        matrix[0] = new bool[countLF];
        while (fin.get(ElementOfMatrix)) {
            if (ElementOfMatrix == '1' or ElementOfMatrix == '0') { // если элемент равен 1 или 0, то заполняем
                if (ElementOfMatrix == '1') {                      // соответствующий элемент массива (если 1 то 0 и наоборот)
                    matrix[i][j] = 1;
                }
                else {
                    matrix[i][j] = 0;
                }
                j++;                                              // увпличиваем счетчик столбцов
                if (j == countLF) {                              //если столбцов = кол-ву строк, то обнуляем счетчк столбцов
                    j = 0;
                    i++;                                         //при этом увеличиваем счетчик строк
                    matrix[i] = new bool[countLF];               //выделяем память под строку соответственно
                }
            }
        }
    }
}


Graph::~Graph() {
    for (int i = 0; i < size; i++) {
        delete[] matrix[i];
    }
    delete[] matrix;
    fout.close();
    fin.close();


}

void Graph::SearchComponents() {

    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) { // функция, делающая матрицу симметричной
            if (matrix[i][j] == 1) {
                matrix[j][i] = 1;
            }
            if (i == j) {
                matrix[i][j] = 0; // удаление петель (по горизонатали должны быть только нули)
            }

        }

    }


    int SizeLines = 0;
    for (int i = 0; i < size; i++) {
        bool CountUnitSize = false;
        for (int j = 0; j < size; j++) { // выделение памяти под динамический массив строк
            if (matrix[i][j] == 1) {
                CountUnitSize = true;
            }
        }
        if (CountUnitSize) {
            SizeLines++;
        }
    }


    int* lines = new int[SizeLines];
    for (int i = 0; i < SizeLines; i++) {
        lines[i] = 0;
    }

    int CountLines = 0;


    for (int i = 0; i < size; i++) {
        if (inLines(lines, i, SizeLines) and i != 0) { //если номер строки в массиве, то приступает к

            continue;                      // следующей итерации
        }

        bool CountUnit = false;                                //флаг единиц
        int CountZero = 0;                                //флаг нулей
        bool  CountMinus = false;                              // флаг дублкатов столбцов

        for (int j = 0; j < size; j++) {
            if (matrix[i][j] == 1) {                      //если элемент равен один, то флаг = true
                CountUnit = true;
                if (inLines(lines, j, SizeLines)) {      //если номер столбца есть в векторе,
                    CountMinus = true;                //то флаг = true

                }
                lines[CountLines] = j;                 //заносим номер столбца в массив, чтобы в будущем
                CountLines++;
            }
            else {                                     //игнорировать строку с этим номером и после приступить к следующей итерации
                CountZero++;                            // если элемент равен 0, увеличиваем счетчик нулей на 1
            }
        }
        if (CountUnit) {       //Если флаг == true, то увеличиваем счетчик компонент
            CountComponent++;

        }
        if (CountZero == size) {   //Если кол-во нулей в строке равно числу всех элементов строки,
            CountComponent++;    //то увеличиваем счетчик компонент (элемент с нулевой строкой представляет

        }                         //собой точку, а значит является отдельным компонентом связности

        if (CountMinus) {          // если флаг == true, уменьшаем кол-во компонент
            CountComponent--;
        }

    }

    cout << endl;
    cout << "Components = " << CountComponent; // выводим кол-во компонент связности
    fout.open("Components.txt");
    fout << "Components = " << CountComponent;
    delete[] lines;
}


int main() {
    Graph graph;
    graph.CreatingFile();
    graph.SearchComponents();
}
