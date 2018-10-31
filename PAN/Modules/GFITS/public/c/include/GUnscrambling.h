#include <string.h>

#define OK              0
#define SINGLE          0
#define A11             0
#define SPLIT_PAR       1
#define A11_22          1
#define SPLIT_SER       2
#define A11_12          2
#define A11_12_21_22    3
#define QUAD            3
#define A21_22          4
#define A12             5
#define A21             6
#define A22             7
#define LOWERQUAD       15
#define A11_13          14
#define A12_14          13
#define IR_HIQUAD       20
#define IR_HIIQUAD	21                

#define MAX_AMPS	128
#define LL      0x8     /* 1000 */
#define LR      0x4     /* 0100 */
#define UL      0x2     /* 0010 */
#define UR      0x1     /* 0001 */
#define n_amptypes      4       /* num of defined amplifier types */

/*predefined combinations*/
#define LOWER           (LL | LR) /* 1100, both lower */
#define UPPER           (UL | UR) /* 0011, both upper */
#define LEFT            (LL | UL) /* 1010, both left */
#define RIGHT           (LR | UR) /* 0101, both right */
#define DIAGR           (LL | UR) /* 1001, diag right */
#define DIAGL           (LR | UL) /* 0110, diag left */
#define FOUR            (LL | LR | UL | UR)       /* 1111, quad */
#define NOROT		0
#define NOFLIP		0
#define FLIPVERT	1
#define FLIPHORI	2


typedef
struct amp_struct {
	char  		name[20];
	unsigned long	dataptr;
	int		xstart;
	int		ystart;
	int		ncols;
	int		nrows;
	int		xdetstart;
	int		ydetstart;
	int		type;
	int		rotate;
	int		flip;
	int		detno;
	int		ampno;
	int		discard;
}amp_t;

typedef
struct ccdamps {
	amp_t amp[4];
} ccdamps_t;
