#include <iostream>
#include <string>
#include "collections.h"

using namespace std;

// Struct for creating and displaying information about tests
struct TestCase {

    static const size_t MESSAGE_WIDTH = 25;

    static void start_test(string test_name) {
        int padding = (MESSAGE_WIDTH - test_name.size()) / 2 - 1;
        cout << endl
            << string(padding, ' ') << "-" << test_name << "-" << endl;
    }

    static void end_test() {
        cout << endl;
    }

    TestCase() : result(true) {}

    TestCase(string msg) : result(true) {
            cout << msg << string(MESSAGE_WIDTH - msg.size(), '.');
    }

    TestCase& operator=(const TestCase& other) {
        result = other.result;
        return *this;
    }

    void assert(bool res) {
        result &= res;
    }

    void show_result() {
        cout << (result ? "ok" : "failed") << endl;
    }

    bool result;
};

void test_arraylist() {

    TestCase::start_test("ArrayList");

    TestCase test_case;


    test_case = TestCase("default constructor");

    ArrayList<int> list1;
    test_case.assert(list1.capacity() == 0);

    test_case.show_result();


    test_case = TestCase("copy constructor");

    list1.append(3);
    ArrayList<int> list2(list1);
    test_case.assert(list1 == list2);
    list1[0] = 5;
    test_case.assert(list1 != list2);

    test_case.show_result();


    test_case = TestCase("assignment operator");

    list1.clear();
    list1.append(5);
    list2 = list1;
    test_case.assert(list1 == list2);
    list1[0] = 3;
    test_case.assert(list1 != list2);

    test_case.show_result();


    test_case = TestCase("append");

    list1.clear();
    list1.append(23);
    list1.append(64);
    list1.append(9);
    list1.append(1);
    test_case.assert(list1.len() == 4);

    test_case.show_result();


    test_case = TestCase("insert");

    list1.insert(4, 7);
    list1.insert(2, 11);
    list1.insert(0, 13);
    test_case.assert(list1[0] == 13);
    test_case.assert(list1[1] == 23);
    test_case.assert(list1[3] == 11);
    test_case.assert(list1[5] == 1);
    test_case.assert(list1[6] == 7);

    test_case.show_result();


    test_case = TestCase("pop");

    list1.clear();
    list1.append(2);
    list1.append(3);
    test_case.assert(list1.pop() == 3);
    test_case.assert(list1.pop() == 2);
    test_case.assert(list1.is_empty());

    test_case.show_result();


    test_case = TestCase("remove");

    list1.append(1);
    list1.append(2);
    list1.append(3);
    list1.remove(0);
    test_case.assert(list1[0] == 2);
    test_case.assert(list1[1] == 3);
    list1.remove(1);
    test_case.assert(list1[0] == 2);
    list1.remove(0);
    test_case.assert(list1.is_empty());

    test_case.show_result();


    test_case = TestCase("reserve");

    ArrayList<int> list3;
    test_case.assert(list3.capacity() == 0);
    list3.reserve(1);
    list3.append(1);
    test_case.assert(list3.capacity() == 1);
    list3.append(1);
    test_case.assert(list3.capacity() == 2);
    list3.reserve(1);
    test_case.assert(list3.capacity() == 2);

    test_case.show_result();


    test_case = TestCase("clear");

    list1.append(1);
    list1.append(2);
    list1.append(3);
    list1.clear();
    test_case.assert(list1.is_empty());
    test_case.assert(list1.capacity() != 0);

    test_case.show_result();


    test_case = TestCase("len");

    list1.append(1);
    list1.append(2);
    list1.append(3);
    list1.pop();
    test_case.assert(list1.len() == 2);
    list1.remove(0);
    test_case.assert(list1.len() == 1);
    list1.clear();
    test_case.assert(list1.len() == 0);

    test_case.show_result();


    test_case = TestCase("capacity");

    ArrayList<int> list4;
    test_case.assert(list4.capacity() == 0);
    list4.append(420);
    test_case.assert(list4.capacity() == COLLECTIONS_DEFAULT_CAPACITY);

    test_case.show_result();


    test_case = TestCase("is_empty");

    test_case.assert(list1.is_empty());
    list1.append(1);
    list1.append(2);
    test_case.assert(!list1.is_empty());
    list1.remove(0);
    test_case.assert(!list1.is_empty());
    list1.remove(0);
    test_case.assert(list1.is_empty());

    test_case.show_result();


    test_case = TestCase("operator[]");

    list1.append(5);
    list1[0] += 3;
    list1[0] *= 8;
    test_case.assert(list1[0] == 64);

    test_case.show_result();


    test_case = TestCase("operator==");

    list1.clear();
    list2.clear();
    list1.append(1337);
    list1.append(69420);
    list2.append(1337);
    list2.append(69420);
    test_case.assert(list1 == list2);

    test_case.show_result();


    test_case = TestCase("operator!=");

    list1.remove(1);
    test_case.assert(list1 != list2);

    test_case.show_result();

    TestCase::end_test();
}

int main() {

    //int (*to_int)(float) = [](float a) { return (int) a; };
    //int (*sq)(int) = [](int a) { return a * a; };
    //bool (*even)(int) = [](int a) { return a % 2 == 0; };

    //ArrayList<int> list;
    //list.append(4);
    //list.append(134);
    //list.append(334);
    //list.append(1);
    //list.append(734);
    //list.append(2);
    //list.append(3);
    //list.append(8923);
    //list.append(234);
    //list.sort();

    //for (auto it = list.iter().filter(even)/*.map(sq)*/; it.valid(); it.next()) {
    //    cout << *it << endl;
    //    *it = 0;
    //}

    //cout << endl;

    //for (auto it = list.iter(); it.valid(); it.next()) {
    //    cout << *it << endl;
    //}

    for (auto it = Range(10); it.valid(); it.next()) {
        cout << *it << endl;
    }

    test_arraylist();
}
