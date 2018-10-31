#ifndef _ARC66PCIe_H_
#define _ARC66PCIe_H_


/*
+------------------------------------------------------------------------------+
|       General Definitions                                                    |
+------------------------------------------------------------------------------+
*/
#define DEVICE_NAME			( char * )"Arc66PCIe"

#define ARC66_MAX_DEV		9

#define BAR_COUNT			6
#define BAR_NUM				0
#define BAR_OFF				1
#define BAR_VAL				2

#define CFG_OFFSET			0
#define CFG_VALUE			1

#define BUF_PROP_PHYS		0
#define BUF_PROP_SIZE		1


/*
+------------------------------------------------------------------------------+
|       Print Definition                                                       |
+------------------------------------------------------------------------------+
*/
#define ArcPrintf( fmt, args... ) printk( "[ Arc66 ]: " fmt, ## args )


/*
+------------------------------------------------------------------------------+
|  Define PCI/e Base Address Register ( BAR ) Macros                           |
+------------------------------------------------------------------------------+
*/
#define GET_BAR_TYPE( ulFlags )														\
	( ( ( ulFlags & IORESOURCE_IO ) == IORESOURCE_IO ) ? IORESOURCE_IO : 			\
	( ( ( ulFlags & IORESOURCE_MEM ) == IORESOURCE_MEM ) ? IORESOURCE_MEM : 0 ) )


/*
+------------------------------------------------------------------------------+
|  PCI/e Configuration Space Header Byte Offsets                               |
+------------------------------------------------------------------------------+
*/
#define CFG_VENDOR_ID           0x000
#define CFG_DEVICE_ID			0x002
#define CFG_COMMAND             0x004
#define CFG_REV_ID              0x008
#define CFG_CACHE_SIZE          0x00C
#define CFG_BAR0                0x010
#define CFG_BAR1                0x014
#define CFG_BAR2                0x018
#define CFG_BAR3                0x01C
#define CFG_BAR4                0x020
#define CFG_BAR5                0x024
#define CFG_CIS_PTR             0x028
#define CFG_SUB_VENDOR_ID       0x02C
#define CFG_EXP_ROM_BASE        0x030
#define CFG_CAP_PTR             0x034
#define CFG_RESERVED1           0x038
#define CFG_INT_LINE            0x03C


/*
+------------------------------------------------------------------------------+
|       State Structure                                                        |
+------------------------------------------------------------------------------+
|  Driver state structure. All state variables related to the state of the     |
|  driver are kept in here.                                                    |
+------------------------------------------------------------------------------+
*/
typedef struct ArcDevExt_t
{
	/* PCI device structure */
	struct pci_dev* pPCIDev;

	/* PCI I/O start address (HSTR, etc) */
	void* __iomem pBar[ BAR_COUNT ];

	/* Char device structure */
	struct cdev tCDev;

	/* Char device count */
	dev_t tCDevno;

	/* Device name for probing purposes */
	char szName[ 20 ];

	/* Device registered with sysfs ( /dev entry ) */
	struct device* pDevice;

	/* Device in use = 1; available = 0 */
	int dOpen;

	/* Semaphore to protect non-global access. i.e. ioctl commands */
	struct semaphore tSem;

	/* Big contiguous memory block for image buffer */
	struct page* pMemBlock;

	/* Virtual start address of image buffer */
	unsigned long ulCommonBufferVA;

	/* Physical start address of image buffer */
	unsigned long ulCommonBufferPA;

	/* Image buffer size (bytes) */
	unsigned long ulCommonBufferSize;

} ArcDevExt;


#endif	/* _ARC66PCIe_H_ */
