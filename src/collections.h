#pragma once

// TODO: Tests
// TODO: More datastructures
// TODO: Better collection interface
// TODO: public / private on things
// TODO: operator<< for iterator

#include <cstddef>    // size_t
#include <functional> // Hashing
#include <utility>    // std::swap

// Amount of space allocated if not otherwise specified
const size_t COLLECTIONS_DEFAULT_CAPACITY = 32;

#include "arraylist.h"
#include "linkedlist.h"
#include "hashmap.h"
