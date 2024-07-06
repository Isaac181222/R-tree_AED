#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

struct Rectangulo {
    int x1, y1, x2, y2;
    Rectangulo(int x1, int y1, int x2, int y2) : x1(x1), y1(y1), x2(x2), y2(y2) {}

    bool overlaps(const Rectangulo& other) const {
        return !(x2 < other.x1 || x1 > other.x2 || y2 < other.y1 || y1 > other.y2);
    }

    bool contains(const Rectangulo& other) const {
        return x1 <= other.x1 && y1 <= other.y1 && x2 >= other.x2 && y2 >= other.y2;
    }

    Rectangulo combine(const Rectangulo& other) const {
        return {min(x1, other.x1), min(y1, other.y1), max(x2, other.x2), max(y2, other.y2)};
    }

    int area() const {
        return (x2 - x1) * (y2 - y1);
    }
};

class RtreeNodo {
public:
    vector<Rectangulo> rectangulos;
    vector<RtreeNodo*> hijos;
    bool hoja;
    int M;
    int m;

    RtreeNodo(int M, int m) : M(M), m(m), hoja(true) {}

    RtreeNodo(int M, int m, vector<Rectangulo> rectangulos, vector<RtreeNodo*> hijos)
            : M(M), m(m), rectangulos(rectangulos), hijos(hijos), hoja(false) {}

    void insertar(Rectangulo r) {
        if (hoja) {
            if (rectangulos.size() < M) {
                rectangulos.push_back(r);
            } else {
                // Dividir
                vector<Rectangulo> allRectangulos = rectangulos;
                allRectangulos.push_back(r);
                vector<Rectangulo> rectangulos1, rectangulos2;
                for (int i = 0; i < allRectangulos.size(); i++) {
                    if (i < allRectangulos.size() / 2) {
                        rectangulos1.push_back(allRectangulos[i]);
                    } else {
                        rectangulos2.push_back(allRectangulos[i]);
                    }
                }
                rectangulos.clear();
                hijos.push_back(new RtreeNodo(M, m, rectangulos1, {}));
                hijos.push_back(new RtreeNodo(M, m, rectangulos2, {}));
                hoja = false;
            }
        } else {
            int minArea = 1e9;
            int minIndex = -1;
            for (int i = 0; i < hijos.size(); i++) {
                Rectangulo combinedMBR = hijos[i]->rectangulos[0].combine(r);
                int area = combinedMBR.area();
                if (area < minArea) {
                    minArea = area;
                    minIndex = i;
                }
            }
            hijos[minIndex]->insertar(r);
        }
    }

    void buscar(const Rectangulo& r) {
        if (hoja) {
            for (int i = 0; i < rectangulos.size(); i++) {
                if (rectangulos[i].contains(r)) {
                    cout << "Rectangulo encontrado" << endl;
                    return;
                }
            }
            cout << "Rectangulo no encontrado" << endl;
        } else {
            for (int i = 0; i < hijos.size(); i++) {
                if (hijos[i]->rectangulos[0].contains(r)) {
                    hijos[i]->buscar(r);
                    return;
                }
            }
            cout << "Rectangulo no encontrado" << endl;
        }
    }

    void borrar(const Rectangulo& r) {
        if (hoja) {
            for (int i = 0; i < rectangulos.size(); i++) {
                if (rectangulos[i].contains(r)) {
                    rectangulos.erase(rectangulos.begin() + i);
                    return;
                }
            }
        } else {
            for (int i = 0; i < hijos.size(); i++) {
                if (hijos[i]->rectangulos[0].contains(r)) {
                    hijos[i]->borrar(r);
                    return;
                }
            }
        }
    }
};

int main() {
    RtreeNodo* rtree = new RtreeNodo(4, 2);
    rtree->insertar(Rectangulo(0, 0, 1, 1));
    rtree->insertar(Rectangulo(2, 2, 3, 3));
    rtree->insertar(Rectangulo(4, 4, 5, 5));
    rtree->insertar(Rectangulo(6, 6, 7, 7));

    rtree->buscar(Rectangulo(0, 0, 1, 1));

    rtree->borrar(Rectangulo(0, 0, 1, 1));

    rtree->buscar(Rectangulo(0, 0, 1, 1));

    return 0;
}