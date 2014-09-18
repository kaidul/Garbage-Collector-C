#include "vmachine.hpp"
#include <cstdlib>
#include <cstdio>

int main(int argc, const char* argv[]) {
    VirtualMachine *vm = new VirtualMachine(nullptr, 0, 0, 10);

    vm->pushInt(1);
    vm->pushInt(2);
    vm->pushPair();
    vm->pushInt(3);
    vm->pushInt(4);
    vm->pushPair();
    vm->pushPair();
//    vm->pushPair();

    // force collect
    vm->gc();

//    delete vm;

    return EXIT_SUCCESS;
}
