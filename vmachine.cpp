#include "vmachine.hpp"
#include <cstdio>
#include <cassert>

void VirtualMachine::push(pObject object) {
    assert(this->stackSize < STACK_MAX);
    this->stack[this->stackSize++] = object;
}

pObject VirtualMachine::pop() {
    assert(this->stackSize > 0);
    return this->stack[--this->stackSize];
}

void VirtualMachine::mark(pObject object) {
    if(!object or object->mark)
        return;

    object->mark = 1;

    if(object->type == OBJ_PAIR) {
        mark(object->first);
        mark(object->second);
    }
}

void VirtualMachine::markAll() {
    for(int i = 0; i < this->stackSize; ++i) {
        mark(this->stack[i]);
    }
}

void VirtualMachine:: sweep() {
    pObject *object = &this->root;
    while(*object) {
        if(!(*object)->mark) {
            pObject unreached = *object;
            *object = unreached->next;
            delete unreached;
            this->numObjects--;
        } else {
            (*object)->mark = 0;
            object = &(*object)->next;
        }
    }
}

void VirtualMachine::gc() {
    int N = this->numObjects;
    this->markAll();
    this->sweep();
    printf("Collected %d objects, %d remaining.\n", N - this->numObjects,
           this->numObjects);
}

pObject VirtualMachine::newObject(ObjectType type) {
    if(this->numObjects == this->threshold)
        gc();

    pObject object = new Object;
    object->type = type;
    object->next = this->root;
    this->root = object;
    object->mark = 0;

    this->numObjects++;

    return object;
}

void VirtualMachine::pushInt(int value) {
    pObject object = this->newObject(OBJ_INT);
    object->value = value;
    push(object);
}

pObject VirtualMachine::pushPair() {
    pObject object = this->newObject(OBJ_PAIR);
    object->first = pop();
    object->second = pop();

    push(object);
    return object;
}

