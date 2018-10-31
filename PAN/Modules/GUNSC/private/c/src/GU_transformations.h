/*functions under GU_transformations.c*/
#include "../../../public/c/include/GUnscrambling.h"

#ifdef _DEBUG_
#  define pdebug(fmt, args...) printf ("GUNSC: " fmt, ## args)
#else
#  define pdebug(fmt, args...)
#endif


i32 Rotate_Image (ptr_t imageptr, u32 ncols, u32 nrows, i32 bpp, i32 rotation);

i32 RI_SHORT(i16 *imptr, u32 ncols, u32 nrows, i32 rotation);
i32 RI_LONG(i32 *imptr, u32 ncols, u32 nrows, i32 rotation);
i32 RI_FLOAT(f32 *imptr, u32 ncols, u32 nrows, i32 rotation);


