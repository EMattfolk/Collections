#include <iostream>
#include <string>
#include <functional>
#include "collections.h"

using namespace std;

int main() {
    LinkedList<float> list;
    list.append(123.5f);
    list.append(645.026f);
    list.append(9.78f);
    list.append(1);
    //list.append("Yeet");

    HashMap<float, int> map;
    map.add(1.6f, 1);
    map.add(1.5f, 2);
    map.add(1.4f, 3);
    map.add(1.3f, 4);
    map.add(1.2f, 5);
    map.add(1.1f, 6);
    map.add(1.0f, 7);
    map.add(0.9f, 8);
    map.erase(1.6f);
    map.erase(1.2f);
    map.erase(1.0f);
    map.add(1.6f, 2);
    map.add(1.2f, 72);
    map.add(1.0f, 22);
    cout << map[1.6f] << endl;
    cout << map[1.5f] << endl;
    cout << map[1.4f] << endl;
    cout << map[1.3f] << endl;
    cout << map[1.2f] << endl;
    cout << map[1.1f] << endl;
    cout << map[1.0f] << endl;
    cout << map[0.9f] << endl;

    //int (*to_int)(float) = [](float a) { return (int) a; };
    float (*sq)(float) = [](float a) { return a * a; };
    bool (*less100)(float) = [](float a) { return a < 100; };

    for (auto it = list.iter().map(sq).filter(less100); it.valid(); it.next()) {
        cout << it.value() << endl;
    }
}
