/*
+-----------------------------------------------------------------------------------+
|   File:       Arc66Ioctl.h                                                        |
+-----------------------------------------------------------------------------------+
|   Author:     Scott Streit                                                        |
|   Abstract:   I/O control commands for PCIe device driver. These commands are for |
|				use with ioctl().                                                   |
|                                                                                   |
|                                                                                   |
|   Revision History:     Date       Who   Version    Description                   |
|   --------------------------------------------------------------------------      |
|                       12/01/2010   sds     1.0      Initial Release               |
+-----------------------------------------------------------------------------------+
*/
#ifndef __ARC66_IOCTL_H__
#define __ARC66_IOCTL_H__

/*
+-----------------------------------------------------------------------------------+
|  Macro to create ioctl command values. Forms a value of the form: 'ARC'<cmd>      |
+-----------------------------------------------------------------------------------+
*/
#define MKCMD( cmd )		( 0x41524300 | cmd )
#define EXCMD( cmd )		( cmd & 0x000000FF )


/*
+-----------------------------------------------------------------------------------+
|  Define ioctl commands                                                            |
+-----------------------------------------------------------------------------------+
*/
#define ARC_READ_BAR		0x01			// Read PCI/e base address register
#define ARC_WRITE_BAR		0x02			// Write PCI/e base address register
#define ARC_BAR_SIZE		0x03			// Get PCI/e base address register size

#define ARC_READ_CFG_8		0x04			// Read 8-bits of PCI/e config space
#define ARC_READ_CFG_16		0x05			// Read 16-bits of PCI/e config space
#define ARC_READ_CFG_32		0x06			// Read 32-bits of PCI/e config space

#define ARC_WRITE_CFG_8		0x07			// Write 8-bits to PCI/e config space
#define ARC_WRITE_CFG_16	0x08			// Write 16-bits to PCI/e config space
#define ARC_WRITE_CFG_32	0x09			// Write 32-bits to PCI/e config space

#define ARC_BUFFER_PROP		0x0A			// Get common buffer properties
#define ARC_DEV_OPEN		0x0B			// 1 = device opened; 0 = closed

#endif	/* __ARC66_IOCTL_H__ */
