#include <iostream>
#include <string>
#include "collections.h"

using namespace std;

int main() {
    ArrayList<float> list;
    list.append(123.5f);
    list.append(645.026f);
    list.append(9.78f);
    list.append(1);
    //list.append("Yeet");

    int (*to_int)(float) = [](float a) { return (int) a; };
    float (*sq)(float) = [](float a) { return a * a; };
    bool (*less100)(float) = [](float a) { return a < 100; };

    for (auto it = list.iter().map(sq).filter(less100); it.valid(); it.next()) {
        cout << it.value() << endl;
    }
}
