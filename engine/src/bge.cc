#include "bge.h"

#include <iostream>

BGE::BGE() { printHello(); }

BGE::~BGE() = default;

void BGE::printHello() { std::cout << "Hello from BGE" << std::endl; }
