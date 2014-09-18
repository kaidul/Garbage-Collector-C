#ifndef VM_H
#define VM_H
#include <cstddef>

#define STACK_MAX 256

enum ObjectType {
    OBJ_INT,
    OBJ_PAIR
};

struct Object {
    ObjectType type;
    unsigned char mark;
    Object *next;

    union {
        int value;

        struct {
            Object *first, *second;
        };
    };
//    friend ostream& operator << (ostream& os, Object const& other);
};

typedef Object * pObject;

class VirtualMachine {
public:
    VirtualMachine(pObject root, size_t stackSize, size_t numObjects, size_t capacity)
        : root(root)
        , stackSize(stackSize)
        , numObjects(numObjects)
        , threshold(capacity) {
    }
    void gc();
    pObject newObject(ObjectType type);
    void pushInt(int value);
    pObject pushPair();

private:
    void push(pObject value);
    pObject pop();
    void mark(pObject);
    void sweep();
    void markAll();
    pObject stack[STACK_MAX];
    pObject root;
    size_t stackSize;
    size_t numObjects;
    size_t threshold;
};

#endif // VM_H
