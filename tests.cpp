#include <iostream>
#include <string>
#include <cassert>
#include "collections.h"

using namespace std;

void ok() {
    cout << "ok" << endl;
}

void test_arraylist() {

    cout << endl << "ArrayList:" << endl;

    cout << "default constructor..";
    ArrayList<int> list1;
    assert(list1.capacity() == 0);
    ok();

    cout << "copy constructor..";
    list1.append(3);
    ArrayList<int> list2(list1);
    assert(list1 == list2);
    list1[0] = 5;
    assert(list1 != list2);
    ok();

    cout << "assignment operator..";
    list1.clear();
    list1.append(5);
    list2 = list1;
    assert(list1 == list2);
    list1[0] = 3;
    assert(list1 != list2);
    ok();

    cout << "append..";
    list1.clear();
    list1.append(23);
    list1.append(64);
    list1.append(9);
    list1.append(1);
    assert(list1.len() == 4);
    ok();

    cout << "insert..";
    list1.insert(4, 7);
    list1.insert(2, 11);
    list1.insert(0, 13);
    assert(list1[0] == 13);
    assert(list1[1] == 23);
    assert(list1[3] == 11);
    assert(list1[5] == 1);
    assert(list1[6] == 7);
    ok();

    cout << "pop..";
    list1.clear();
    list1.append(2);
    list1.append(3);
    assert(list1.pop() == 3);
    assert(list1.pop() == 2);
    assert(list1.is_empty());
    ok();

    cout << "remove..";
    list1.append(1);
    list1.append(2);
    list1.append(3);
    list1.remove(0);
    assert(list1[0] == 2);
    assert(list1[1] == 3);
    list1.remove(1);
    assert(list1[0] == 2);
    list1.remove(0);
    assert(list1.is_empty());
    ok();

    cout << "reserve..";
    ArrayList<int> list3;
    assert(list3.capacity() == 0);
    list3.reserve(1);
    list3.append(1);
    assert(list3.capacity() == 1);
    list3.append(1);
    assert(list3.capacity() == 2);
    list3.reserve(1);
    assert(list3.capacity() == 2);
    ok();

    cout << "clear..";
    list1.append(1);
    list1.append(2);
    list1.append(3);
    list1.clear();
    assert(list1.is_empty());
    assert(list1.capacity() != 0);
    ok();

    cout << "len..";
    list1.append(1);
    list1.append(2);
    list1.append(3);
    list1.pop();
    assert(list1.len() == 2);
    list1.remove(0);
    assert(list1.len() == 1);
    list1.clear();
    assert(list1.len() == 0);
    ok();

    cout << "capacity..";
    ArrayList<int> list4;
    assert(list4.capacity() == 0);
    list4.append(420);
    assert(list4.capacity() == COLLECTIONS_DEFAULT_CAPACITY);
    ok();

    cout << "is_empty..";
    assert(list1.is_empty());
    list1.append(1);
    list1.append(2);
    assert(!list1.is_empty());
    list1.remove(0);
    assert(!list1.is_empty());
    list1.remove(0);
    assert(list1.is_empty());
    ok();

    cout << "operator[]..";
    list1.append(5);
    list1[0] += 3;
    list1[0] *= 8;
    assert(list1[0] == 64);
    ok();

    cout << "operator==..";
    list1.clear();
    list2.clear();
    list1.append(1337);
    list1.append(69420);
    list2.append(1337);
    list2.append(69420);
    assert(list1 == list2);
    ok();

    cout << "operator!=..";
    list1.remove(1);
    assert(list1 != list2);
    ok();
}

int main() {

    //int (*to_int)(float) = [](float a) { return (int) a; };
    //float (*sq)(float) = [](float a) { return a * a; };
    //bool (*less100)(float) = [](float a) { return a < 100; };

    //for (auto it = list.iter_end(); it.valid(); it.prev()) {
    //    cout << it.value() << endl;
    //}
    //

    cout << "Running tests" << endl;

    test_arraylist();
}
