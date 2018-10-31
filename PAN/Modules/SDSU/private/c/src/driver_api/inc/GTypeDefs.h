typedef int i32;
typedef int I32;
typedef short i16;
typedef short I16;
typedef unsigned short u16;
typedef unsigned short U16;
typedef unsigned int u32;
typedef unsigned int U32;
typedef float f32;
typedef float F32;
//typedef double double_t;
typedef char i8;
typedef char I8;
typedef unsigned int ptr32_t;
#ifdef OS64
	typedef unsigned long u64;
	typedef unsigned long ptr_t;
	typedef unsigned long dptr_t;
	typedef int fd_t;
#else
	typedef unsigned long long u64;
	typedef unsigned long long ptr_t;
	typedef unsigned long dptr_t;
	typedef int fd_t;
#endif
typedef dptr_t mem_t;
