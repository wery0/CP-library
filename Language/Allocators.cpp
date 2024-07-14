const int MAX_MEM = 500 * 1024 * 1024;
int mpos = 0;
char mem[MAX_MEM];
inline void* operator new (size_t n) {
    char *res = mem + mpos;
    mpos += n;
    //assert(mpos <= MAX_MEM);
    return (void*)res;
}
inline void operator delete (void*) {}


//BumpAllocator
static char buf[450 << 20];
void* operator new(size_t s) {
	static size_t i = sizeof buf;
	assert(s < i);
	return (void*)&buf[i -= s];
}
void operator delete(void*) {}


//BumpAllocator for stl containers
//Usage: vector<int, small<int>> ed(N);
char buf[450 << 20] alignas(16);
size_t buf_ind = sizeof buf;
template<class T> struct small {
	typedef T value_type;
	small() {}
	template<class U> small(const U&) {}
	T* allocate(size_t n) {
		buf_ind -= n * sizeof(T);
		buf_ind &= 0 - alignof(T);
		return (T*)(buf + buf_ind);
	}
	void deallocate(T*, size_t) {}
};
