typedef
struct address_struct
{
	unsigned int 	board;
	unsigned int 	type;
        unsigned int    address;
        int     	value;
} add_t;

int get_chunk (int expo);
void reset_buffer (void);

#define OPTICAL	0
#define NIR	1

#define	VIDREV5	0x1	
#define	IR8	0x5	
#define UTILREV3 0x20
#define TIMREV5 0x10
#define DEFBIASLEVEL 4000
#define DEFSIGLEVEL 2000
#define DEFRON 10
#define ARTIFDET	0
#define COLRAMP		1


int namps=1;
int artifdata=ARTIFDET;
int biaslevel=DEFBIASLEVEL;
int ron=DEFRON;
int siglevel=DEFSIGLEVEL;
