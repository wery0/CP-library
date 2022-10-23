const int MAX_MEM = 500 * 1024 * 1024;
int mpos = 0;
char mem[MAX_MEM];
inline void * operator new (size_t n) {
    char *res = mem + mpos;
    mpos += n;
    assert(mpos <= MAX_MEM);
    return (void*)res;
}
inline void operator delete (void*) {}