/*******************************************************************************
*   File:	astropci_io.h
*   Author: 	Scott Streit
*   Abstract: 	I/O control commands for Linux device driver for the SDSU PCI
*             	Interface Card. These commands are for use with ioctl().
*
*
*   Revision History:     Date      Who   Version    Description
*   --------------------------------------------------------------------------
*                       11/12/99    sds     1.0      Initial Release
*				07/11/00	sds		1.4		 Added binning constants.
*
*******************************************************************************/ 
#ifndef __ASTROPCI_IO_H__
#define __ASTROPCI_IO_H__

/*********************************************
	Information retrieval commands
*********************************************/
#define ASTROPCI_GET_HCTR			0x1
#define ASTROPCI_GET_PROGRESS		0x2
#define ASTROPCI_GET_DMA_ADDR		0x3
#define ASTROPCI_GET_HSTR			0x4

/*********************************************
	Information set commands
*********************************************/
#define ASTROPCI_HCVR_DATA 	 		0x10
#define ASTROPCI_SET_HCTR 	        0x11
#define ASTROPCI_SET_HCVR 	        0x12
#define ASTROPCI_PCI_DOWNLOAD		0x13
#define ASTROPCI_PCI_DOWNLOAD_WAIT	0x14
#define ASTROPCI_COMMAND			0x15
#define ASTROPCI_MUNMAP				0x16

/*********************************************

	Generic Constants
*********************************************/
#define DEBUG					0
#define REPLY_BUFFER_EMPTY		-1

/*********************************************
	DSP Fifo Check Constants
*********************************************/
#define INPUT_FIFO_OK_MASK		0x00000002
#define OUTPUT_FIFO_OK_MASK		0x00000004
#define HTF_BIT_MASK			0x00000038
#define BUSY_WAIT_DELAY			100			/* usec */
#define SLEEP_WAIT_DELAY		100			/* usec */
#define BUSY_MAX_TRIES			1000		/* max # of loops */
#define SLEEP_MAX_TRIES			8000		/* max # of loops */

/*********************************************
	Vector Commands
*********************************************/
#define READ_REPLY_VALUE		0x83
#define CLEAR_REPLY_FLAGS		0x85
#define WRITE_PCI_ADDRESS		0x8D
#define READ_PCI_IMAGE_ADDR		0x8075
#define ABORT_READOUT			0x8079
#define PCI_DOWNLOAD			0x802F
#define WRITE_COMMAND			0xB1

/*********************************************
	DSP Replies
*********************************************/
#define DON				0x00444F4E
#define RDR				0x00524452
#define ERR				0x00455252
#define SYR				0x00535952
#define TIMEOUT				0x544F5554
#define READOUT				0x524F5554

/*********************************************
	Interrupt constants
*********************************************/
#define REPLY_INTERRUPT				0x8
#define IMAGE_BUFFER1_INTERRUPT		0x10
#define IMAGE_BUFFER2_INTERRUPT		0x20
#define CLEAR_REPLY_INTERRUPT		0x8073
#define CLEAR_IMAGE_BUFF1_INTERRUPT	0x8075
#define CLEAR_IMAGE_BUFF2_INTERRUPT	0x8077
#define DMA_INTERRUPTING			0x40
#define ABORT_INTERRUPT				0x78
#define REPLY_DATA_EXISTS			0x4

/*********************************************
	DMA constants
*********************************************/
#define DMA_INTERRUPTING		0x40
#define IMAGE_BUFF1_BUSY		0x10
#define IMAGE_BUFF1_CLEAR		0xFFFFFFEF
#define IMAGE_BUFF2_BUSY		0x20
#define IMAGE_BUFF2_CLEAR		0xFFFFFFDF
#define ASTROPCI_DEVICE_ID		0x1801
#define ASTROPCI_VENDOR_ID		0x1057

#endif

