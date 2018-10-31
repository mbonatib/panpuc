/* driver callas*/

#define DEF_TIMEOUT     -1
#define NONE            -1
#define DONE            0

#define ASTROPCI_GET_HCTR	  	0x1
//#define ASTROPCI_GET_PROGRESS	  	0x2
#define ASTROPCI_GET_PROGRESS	  	0x7
#define  ASTROPCI_GET_DMA_ADDR	  	0x3
#define ASTROPCI_GET_HSTR	  	0x4
#define ASTROPCI_GET_FRAMES_READ 	0x7
#define ASTROPCI_HCVR_DATA 	  	0x10
#define ASTROPCI_SET_HCTR 	  	0x11
#define ASTROPCI_SET_HCVR	  	0x12
#define ASTROPCI_PCI_DOWNLOAD	  	0x13
#define ASTROPCI_PCI_DOWNLOAD_WAIT 	0x14
#define ASTROPCI_COMMAND	  	0x15
//#define ASTROPCI_GET_PROGRESS	  	0x16


#define  READ_PIXEL_COUNT	  	0x8075
#define	 PCI_PC_RESET		  	0x8077
#define	ABORT_READOUT            	0x8079
#define	READOUT            		0x5
#define	 BOOT_EEPROM              	0x807B
#define	READ_HEADER		  	0x81
#define	RESET_CONTROLLER	  	0x87
#define	WRITE_NUM_IMAGE_BYTES	  	0x8F
#define	INITIALIZE_IMAGE_ADDRESS 	0x91
#define	WRITE_COMMAND		  	0xB1

#define HTF_BITS			0x00000038
#define HTF_MASK			0x00000200
#define HTF_CLEAR_MASK			0xFFFFFCFF
#define BIT3_CLEAR_MASK			0xFFFFFFF7
	// Define replies.
#define	REPLY_BUFFER_EMPTY	  	-1
#define	TOUT			  	0x544F5554	// Timeout
#define	ROUT			  	0x524F5554	// Readout
#define	DON                      	0x00444F4E	// Done
#define	ERR			  	0x00455252	// Error
#define	SYR                      	0x00535952	// System Reset
#define	RST                      	0x00525354	// Reset
#define	NULL 			  	0x4E554C4C
#define	TDL                      	0x0054444C	// Test Data Link
#define	RDM                      	0x0052444D	// Read Memory
#define	WRM                      	0x0057524D	// Write Memory
#define	SEX			  	0x00534558	// Start Exposure
#define	SET			  	0x00534554	// Set Exposure Time
#define	PEX			  	0x00504558	// Pause Exposure
#define	REX			  	0x00524558	// Resume Exposure
#define	RET			  	0x00524554	// Read Elapsed Time
#define	AEX			  	0x00414558	// Abort Exposure
#define	PON			  	0x00504F4E	// Power On
#define	POF			  	0x00504F46	// Power Off
#define	RDI			  	0x00524449	// Read Image
#define	SOS			  	0x00534F53	// Select Output Source
#define	MPP			  	0x004D5050	// Multi-Pinned Phase Mode
#define	DCA			  	0x00444341	// Download Coadder
#define	SNC			  	0x00534E43	// Set Number of Coadds
#define SNR             		0x534E52	// Set number of samples
#define	FPB			  	0x00465042	// Set the Frames-Per-Buffer for Coadds
#define	VID			  	0x00564944	// mnemonic that means video board
#define	SBN			  	0x0053424E	// Set Bias Number
#define	SBV			  	0x00534256 	// Set Bias Voltage
#define	SGN			  	0x0053474E	// Set Gain
#define	SMX                      	0x00534D58	// Select Multiplexer
#define	CLK                      	0x00434C4B	// mnemonic that means clock driver board
#define	SSS                      	0x00535353	// Set Subarray Sizes
#define	SSP                      	0x00535350	// Set Subarray Positions
#define	LGN                      	0x004C474E	// Set Low Gain
#define	HGN                      	0x0048474E	// Set High Gain
#define	SRM                      	0x0053524D	// Set Readout Mode - either CDS or single
#define	CDS			  	0x00434453	// Correlated Double Sampling
#define	SFS			  	0x00534653	// Send Fowler Sample
#define	SPT			  	0x00535054	// Set Pass Through Mode
#define	LDA			  	0x004C4441	// Load Application
#define	RCC                      	0x00524343	// Read Controller Configuration
#define	CLR			  	0x00434C52	// Clear Array
#define	IDL			  	0x0049444C	// Idle
#define	STP			  	0x00535450	// Stop Idle
#define CSH                      	0x00435348	// Close Shutter
#define	OSH                      	0x004F5348	// Open Shutter
#define	SUR			  	0x00535552	// Set Up The Ramp Mode

	// Define board id's.
#define	PCI_ID 	   	  	1
#define	TIM_ID 	   	  	2
#define	UTIL_ID 	   	  	3
#define	MAX_LOAD_ADDR 	   	  	0x4000
#define	SEARCH_STRING 	   	  	"_DATA P"
	
	// Define argument numbers for commands that use board destination.
#define TDL_ARG_NUM		  	0x10000	// Number of "Argument" registers used by TDL
#define RDM_ARG_NUM		  	0x20000	// Number of "Argument" registers used by RDM
#define	WRM_ARG_NUM		  	0x30000	// Number of "Argument" registers used by WRM
#define	TIM_APP_ARG_NUM	  	0x10000
#define	UTIL_APP_ARG_NUM	  	0x10000
#define	ZERO_ARG_NUM	  	  	0x00000
	
	// General constants.
#define	_OPEN_SHUTTER_           	(1 << 11)
#define	_CLOSE_SHUTTER_          	~(1 << 11)
#define	_ARTIF_DATA_ON_          	(1 << 10)
#define	_ARTIF_DATA_OFF_          	~(1 << 10)
#define	_AUTO_CLR_OFF_          	(1 << 8)
#define	_AUTO_CLR_ON_	         	~(1 << 8)
#define	AUTO                     	800
#define	MANUAL		   	  	801
#define SUCCESS		  	0
#define FAILURE		  	1
#define ABORT			  	2
#define ERROR			  	0xFFFFFFFF		

	// Controller configuration bit masks.
#define	VIDEO_PROCESSOR_MASK	  	0x000007
#define	TIMING_BOARD_MASK	  	0x000018
#define UTILITY_BOARD_MASK	  	0x000060
#define	SHUTTER_MASK		  	0x000080
#define	TEMPERATURE_READOUT_MASK 	0x000300
#define SUBARRAY_READOUT_MASK	  	0x000400
#define BINNING_MASK		  	0x000800
#define	READOUT_MASK		  	0x003000
#define	MPP_MASK		  	0x004000
#define	CLOCK_DRIVER_MASK	  	0x018000
#define	SPECIAL_MASK		  	0x0E0000

	// Controller configuration bit definitions.
#define ARC41			  	0x000000
#define ARC42			  	0x000002
#define	ARC45			  	0x000004
#define	ARC46			  	0x000005
#define	CCD_GENI		  	0x000001
#define	CCD			  	0x000000
#define	IR_REV4C		  	0x000002
#define	CLOCK_DRIVER		  	0x000000
#define	TIM_REV4B		  	0x000000
#define	TIM_GENI		  	0x000008
#define	NO_UTIL_BOARD		  	0x000000
#define	UTILITY_REV3		  	0x000020
#define	SHUTTER		  	0x000080
#define	NO_TEMPERTURE_CONTROL	  	0x000000
#define	NONLINEAR_TEMP_CONV	  	0x000100
#define	LINEAR_TEMP_CONV	  	0x000200
#define	SUBARRAY		  	0x000400
#define	BINNING		  	0x000800
#define SERIAL		               0x001000
#define PARALLEL                      0x002000	// Currently not used.
#define SERIAL_PARALLEL               0x003000
#define	MPP_CAPABLE		  	0x004000
#define	SDSU_MLO		  	0x060000
#define	SDSU_WS		  	0x020000
#define	NGST			  	0x040000
#define	CONTINUOUS_READOUT	  	0x100000
#define	NIRIM			  	(IR_REV4C | TIM_GENI)
#define	DEFAULT_CONFIG_WORD	  	(NONLINEAR_TEMP_CONV | TIM_REV4B | UTILITY_REV3 | SHUTTER)  //0x0001A0

        // Select output source ('SOS') and memory type constants.
#define A_AMP                    	0x5F5F41       //  Ascii __A amp A.
#define B_AMP                    	0x5F5F42       //  Ascii __B amp B.
#define C_AMP                    	0x5F5F43       //  Ascii __C amp C.
#define D_AMP                    	0x5F5F44       //  Ascii __D amp D.
#define L_AMP                    	0x5F5F4C       //  Ascii __L left amp.
#define	R_AMP                    	0x5F5F52	//  Ascii __R left amp.
#define LR_AMP                   	0x5F4C52       //  Ascii _LR right two amps.
#define AB_AMP                   	0x5F4142       //  Ascii _AB top two amps A & B.
#define CD_AMP                   	0x5F4344       //  Ascii _CD bottom two amps C & D.
#define ALL_AMP                  	0x414C4C       //  Ascii ALL four amps (quad).
#define	P                        	0x100000	//  Bit 20
#define	P_MEM                        	0x100000	//  Bit 20
#define	X                        	0x200000	//  Bit 21
#define	X_MEM                        	0x200000	//  Bit 21
#define	Y                        	0x400000	//  Bit 22
#define	Y_MEM                        	0x400000	//  Bit 22
#define	R                        	0x800000	//  Bit 23
#define	R_MEM                        	0x800000	//  Bit 23

	// Define CCD setup constants.
#define	HARDWARE_DATA_MAX        	1000000

/*command names*/
#define         MAX_CMDS        60
#define         NOT_IN_TABLE    -1
#define         FRAMESPERBUFCMD	   "framesperbuffer"
#define         SYNCCMD		   "sync"
#define         GAINSPEEDCMD	   "gainspeed"
#define         ROILOCCMD	   "roiloc"
#define         ROILENCMD	   "roilen"
#define         FRAMETIMECMD       "frametime"
#define         ROWSLOC            "rows"
#define         COLSLOC            "cols"
#define         XBINLOC            "xbin"
#define         YBINLOC            "ybin"
#define         COADDSCMD          "coadds"
#define         SURCMD             "sur"
#define         NREADSCMD          "nsamples"
#define         INTEGTIMECMD       "acinttime"
#define		GEXPT		   "gexptime"
#define		GMODE		   "gmode"
#define		GLEN		   "glen"
#define		GNIM		   "gnimages"
#define         ABSCMD             "stopsur"
#define         AMPROWSLOC         "arows"
#define         AMPCOLSLOC         "acols"

#define		DSPCMD		0
#define		ADDRESS		1
#define		BIT		2

/*default three letters commands*/
#define         DEFCOADDSCMD       	"SNF"
#define         DEFSURCMD         	"SUR"
#define         DEFNREADSCMD       	"SNR"
#define         DEFFRAMESPERBUFCMD   	"FPB"
#define         DEFGAINSPEEDCMD	  	"SGN"
#define		DEFFRAMETIMECMD		"RFT"
#define         DEFROILOCCMD	   	"SSP"
#define         DEFROILENCMD	   	"SSS"
#define         DEFSYNCCMD	   	"SSM"
#define		DEFINTEGTIMECMD		"RIT"
#define		DEFGEXPT		"GEX"
#define		DEFGMODE		"GMO"
#define		DEFGLEN			"GLE"
#define		DEFGNIM			"GNI"
#define		DEFABSCMD		"ABS"

typedef
struct cmd_struct {
        char            command[20];
        char            dsp[10];
        int             type;
        int             memtype;
        int             address;
        int             bit;
} cmd_t;
