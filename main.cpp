#include <iostream>
#include "collections.h"

using namespace std;

int main() {
    LinkedList<float> list;
    list.append(123.5f);
    list.append(645.026f);
    list.append(9.78f);
    list.append(1);

    int (*to_int)(float) = [](float a) { return (int) a; };
    float (*sq)(float) = [](float a) { return a * a; };

    for (auto it = list.iter().map(sq); it.valid(); it.next()) {
        cout << it.value() << endl;
    }
}
