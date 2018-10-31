/*
+-------------------------------------------------------------------------------------------------------+
|   File:       Arc66PCIe.c                                                    							|
|                                                                              							|
|   Author:     Scott Streit                                                   							|
|                                                                              							|
|   Abstract:   Linux device driver for the ARC-66/67 PCIe Interface Board.    							|
|                                                                              							|
|                                                                             							|
|   Revision History:                                                          							|
+-------------------------------------------------------------------------------------------------------+
|   Date        Who Version Description                                        							|
|   13-Dec-2012 sds  1.0    Initial                                            							|
|   28-Mar-2014 sds  3.5	The following changes were made to support kernels >= 3.13.73:				|
|                           Changed __devinit to __init on Arc66_mapBars().   							|
|                           Changed __devexit_p() to __exit_p() on remove().   							|
|							Changed vma->vm_flags |= (VM_IO | VM_LOCKED | VM_RESERVED);					|
|                           to vma->vm_flags |= (VM_IO | VM_LOCKED | (VM_DONTEXPAND | VM_DONTDUMP));	|
|                                                                              							|
|   Development notes:                                                         							|
+-------------------------------------------------------------------------------------------------------+
|   This driver has been tested on CentOS 6.3 x64, Kernel 2.6.32-279.14.1.el6  							|
|                                                                              							|
+-------------------------------------------------------------------------------------------------------+
*/
#include <linux/version.h>
#include <linux/kernel.h>
#include <linux/delay.h>
#include <linux/dma-mapping.h>
#include <linux/init.h>
#include <linux/interrupt.h>
#include <linux/io.h>
#include <linux/jiffies.h>
#include <linux/module.h>
#include <linux/pci.h>
#include <linux/fs.h>
#include <linux/cdev.h>

#include <asm/uaccess.h>

/* Kernel Change - 3.x */
#if LINUX_VERSION_CODE >= KERNEL_VERSION(3,0,0)
	#include <linux/spinlock_types.h>
	#include <linux/semaphore.h>
	#include <linux/sched.h>
#else
	#include <linux/smp_lock.h>
#endif

#include "Arc66PCIe.h"
#include "Arc66Ioctl.h"


/*
+------------------------------------------------------------------------------+
|       Module command line parameters                                         |
+------------------------------------------------------------------------------+
*/
//static ulong g_ulSize = 2 * 4200 * 4400;
static ulong g_ulSize = 2 * 16384 * 16384;
module_param( g_ulSize, ulong, S_IRUGO );


/*
+------------------------------------------------------------------------------+
|       Global variables                                                       |
+------------------------------------------------------------------------------+
*/
/* Kernel Change - 3.x */
#if LINUX_VERSION_CODE >= KERNEL_VERSION(3,0,0)
	static DEFINE_SPINLOCK( g_tArc66Lock );
#else
	static spinlock_t g_tArc66Lock = SPIN_LOCK_UNLOCKED;
#endif

static struct class*	g_pArc66Class		= NULL;
static unsigned long	g_ulNextStartAddr	= 0;
static int				g_dArc66Major		= 0;
static int				g_dArc66Minor		= 0;
static int				g_dUsageCount		= 0;


/*
+------------------------------------------------------------------------------+
|       Prototypes for main entry points                                       |
+------------------------------------------------------------------------------+
*/
static int	Arc66_probe( struct pci_dev* dev, const struct pci_device_id* id );
static void	Arc66_remove( struct pci_dev* dev );

static int  Arc66_open( struct inode* inode, struct file* filp );
static int  Arc66_close( struct inode* inode, struct file* filp );

static long Arc66_ioctl( struct file* filp, unsigned int cmd, unsigned long arg );

static int  Arc66_mmap( struct file* filp, struct vm_area_struct* vma );


/*
+------------------------------------------------------------------------------+
|       Prototypes for device specific functions                               |
+------------------------------------------------------------------------------+
*/
static int  Arc66_mapCommonBuffer( ArcDevExt* pDevExt, struct vm_area_struct* vma );
static void Arc66_setBufferAddresses( ArcDevExt* pDevExt );

static int	__init Arc66_mapBars( ArcDevExt *pDevExt, struct pci_dev *dev );

static void	Arc66_unMapBars( ArcDevExt *pDevExt, struct pci_dev *dev );

//static int	__devinit Arc66_cdevInit( ArcDevExt *pDevExt );
static int	Arc66_cdevInit( ArcDevExt *pDevExt );
static void Arc66_cdevCleanUp( ArcDevExt *pDevExt );

static int Arc66_setAsBusMaster( ArcDevExt* pDevExt );


/*
+------------------------------------------------------------------------------+
|       Prototypes for large memory allocation functions                       |
+------------------------------------------------------------------------------+
*/
static struct page*	Arc66_bigBufAlloc( unsigned int flags, ulong size );
static void			Arc66_bigBufFree( struct page* start, ulong size );


/*
+------------------------------------------------------------------------------+
|   As of kernel 2.4, you can rename the init and cleanup functions. They      |
|   no longer need to be called init_module and cleanup_module. The macros     |
|   module_init and module_exit, found in linux/init.h, can be used to         |
|   define your own functions. BUT, these macros MUST be called after defining |
|   init and cleanup functions.                                                |
+------------------------------------------------------------------------------+
*/
static int  __init Arc66_init( void );
static void __exit Arc66_exit( void );

module_init( Arc66_init );
module_exit( Arc66_exit );


/*
+------------------------------------------------------------------------------+
|       Structures used by the kernel PCI API                                  |
+------------------------------------------------------------------------------+
*/
static struct pci_device_id Arc66_ids[] = {
	{ PCI_DEVICE( 0x10B5, 0x9056 ), },
	{ 0, }
};
MODULE_DEVICE_TABLE( pci, Arc66_ids );


/*
+------------------------------------------------------------------------------+
|  Used to register the driver with the PCI kernel sub system                  |
+------------------------------------------------------------------------------+
*/
static struct pci_driver Arc66_driver = {
	.name		= DEVICE_NAME,
	.id_table	= Arc66_ids,
	.probe		= Arc66_probe,
	.remove		= __exit_p( Arc66_remove ),
};


/*
+------------------------------------------------------------------------------+
|  Character device file operations                                            |
+------------------------------------------------------------------------------+
*/
static struct file_operations Arc66_fops = {
		.owner			= THIS_MODULE,
		.unlocked_ioctl	= Arc66_ioctl,
		.mmap			= Arc66_mmap,
		.open			= Arc66_open,
		.release		= Arc66_close
};


/*
+------------------------------------------------------------------------------+
|  FUNCTION: Arc66_init() - Init Module                                        |
+------------------------------------------------------------------------------+
|  PURPOSE:  Initializes the module.                                           |
|                                                                              |
|  RETURNS:  Returns 0 for success, or the appropriate error number.           |
|                                                                              |
|  NOTES:    Called by Linux during insmod, not by the user.                   |
+------------------------------------------------------------------------------+
*/
static int __init Arc66_init( void )
{
	int dResult = 0;

#if LINUX_VERSION_CODE < KERNEL_VERSION( 2,6,11 )
	ArcPrintf( "INIT - Invalid kernel version, must be 2.6.11 or greater!\n" );
	dResult = -1;
#endif

	ArcPrintf( "INIT - +------------------------------------+\n" );
	ArcPrintf( "INIT - |     ARC-66/67 - Initialization     |\n" );
	ArcPrintf( "INIT - +------------------------------------+\n" );

	if ( !IS_ERR_VALUE( dResult ) )
	{
		/* Allocate a single dynamically allocated char device major
		  +--------------------------------------------------------+ */
		if ( g_dArc66Major == 0 )
		{
			dev_t tTempDev;

			dResult = alloc_chrdev_region( &tTempDev,
										   0,
										   ARC66_MAX_DEV,
										   DEVICE_NAME );

			/* Allocation failed?
			  +--------------------------------------------------------+ */
			if ( dResult < 0 )
			{
				ArcPrintf( "INIT - Alloc_chrdev_region() failed = %d\n",
							dResult );
			}

			else
			{
				g_dArc66Major = MAJOR( tTempDev );
				g_dArc66Minor = 0;
			}
		}
	}

	/*  Create the driver class
	   +---------------------------------------------+ */
	if ( !IS_ERR_VALUE( dResult ) )
	{
		g_pArc66Class = class_create( THIS_MODULE, DEVICE_NAME );

		if ( IS_ERR( g_pArc66Class ) )
		{
			dResult = PTR_ERR( g_pArc66Class );

			ArcPrintf( "INIT - Failure creating class, error %d\n", dResult );
		}
	}

	/*  Register the PCI driver
	   +---------------------------------------------+ */
	if ( !IS_ERR_VALUE( dResult ) )
	{
		dResult = pci_register_driver( &Arc66_driver );

		if ( IS_ERR_VALUE( dResult ) )
		{
			ArcPrintf( "INIT - Failed to register driver!\n" );
		}
	}

	return dResult;
}


/*
+------------------------------------------------------------------------------+
|  FUNCTION: Arc66_exit() - Cleanup Module                                     |
+------------------------------------------------------------------------------+
|  PURPOSE:  Module cleanup, unregisters devices and allocated resources.      |
|                                                                              |
|  NOTES:    Called by Linux during rmmod, not by the user.                    |
+------------------------------------------------------------------------------+
*/
static void __exit Arc66_exit( void )
{
	int dDev = 0;

	ArcPrintf( "EXIT - +------------------------------------+\n" );
	ArcPrintf( "EXIT - |      ARC-66/67 - EXIT CALLED       |\n" );
	ArcPrintf( "EXIT - +------------------------------------+\n" );

	/* Free the dynamically allocated character device node
	  +---------------------------------------------------+ */
	unregister_chrdev_region( MKDEV( g_dArc66Major, 0 ), 1 );

	/* Remove /dev entry
	 * NOTE: This MUST be called before class_destroy!
	 *       So it's called in EXIT, not REMOVE.
	  +------------------------------------------------+ */
	for ( dDev = 0; dDev < ARC66_MAX_DEV; dDev++ )
	{
		device_destroy( g_pArc66Class, MKDEV( g_dArc66Major, dDev ) );
	}

	/* Class destroy
	  +------------------------------------------------+ */
	if ( g_pArc66Class != NULL )
	{
		class_destroy( g_pArc66Class );
	}

	/* Unregister PCI driver
	  +------------------------------------------------+ */
	pci_unregister_driver( &Arc66_driver );
}


/*
+------------------------------------------------------------------------------+
|  FUNCTION: Arc66_probe()                                                     |
+------------------------------------------------------------------------------+
|  PURPOSE:  Called when the PCI sub system thinks we can control the given    |
|            device. Inspect if we can support the device and if so take       |
|            control of it.                                                    |
|                                                                              |
|            - allocate board specific device extension                        |
|            - enable the board                                                |
|            - request regions                                                 |
|            - query DMA mask                                                  |
|            - obtain and request irq                                          |
|            - map regions into kernel address space                           |
|                                                                              |
|  RETURNS:  Returns 0 for success, or the appropriate error number.           |
|                                                                              |
|  NOTES:    See Documentation/pci.txt in the Linux sources.                   |
+------------------------------------------------------------------------------+
*/
static int Arc66_probe( struct pci_dev* dev, const struct pci_device_id* id )
{
	ArcDevExt*	pDevExt = NULL;
	int			dResult = 0;

	/* Immediately try to allocate the image buffer
	  +--------------------------------------------------------+ */
	struct page* pMemBlock =
					Arc66_bigBufAlloc( ( GFP_KERNEL | __GFP_DMA32 ), g_ulSize );

	if ( pMemBlock )
	{
		ArcPrintf( "PROBE - Allocated memory block at 0x%lX of size %lu\n",
					( ulong )page_to_phys( pMemBlock ),
					g_ulSize );
	}
	else
	{
		ArcPrintf( "PROBE - Allocation of size %lu failed.\n", g_ulSize );

		dResult = -ENOMEM;
	}

	if ( !IS_ERR_VALUE( dResult ) )
	{
		ArcPrintf( "PROBE - Looking for ARC-66 ( Gen IV ) boards ...\n" );

		/*  Allocate memory for per-board book keeping
		   +----------------------------------------------------+ */
		pDevExt = kzalloc( sizeof( ArcDevExt ), GFP_KERNEL );

		if ( !pDevExt )
		{
			ArcPrintf(
				"PROBE - Could not kzalloc()ate DEVICE EXTENSION memory.\n" );

			dResult = -ENOMEM;
		}
	}

	if ( !IS_ERR_VALUE( dResult ) )
	{
		/* Save the device reference in the device extension
		  +----------------------------------------------------+ */
		pDevExt->pPCIDev = dev;

		/*  Initialize device extension stuff
		  +----------------------------------------------------+ */
		pDevExt->pMemBlock			= pMemBlock;
		pDevExt->dOpen				= 0;
		pDevExt->ulCommonBufferVA	= 0;
		pDevExt->ulCommonBufferPA	= 0;
		pDevExt->ulCommonBufferSize	= 0;

		/* Save the device extension for later use
		  +----------------------------------------------------+ */
		dev_set_drvdata( &dev->dev, pDevExt );

		/* Initialize device before it's used by the driver
		  +----------------------------------------------------+ */
		dResult = pci_enable_device( dev );

		if ( IS_ERR_VALUE( dResult ) )
		{
			ArcPrintf( "PROBE - Failed to enable device!\n" );
		}
	}

	/* Request all PCI I/O regions associated with the device
	  +------------------------------------------------------+ */
	if ( !IS_ERR_VALUE( dResult ) )
	{
		dResult = pci_request_regions( dev, DEVICE_NAME );

		if ( dResult )
		{
			ArcPrintf( "PROBE - Failed to retrieve PCI I/O regions!\n" );
		}
	}

	/* Map BARs
	  +------------------------------------------------------+ */
	if ( !IS_ERR_VALUE( dResult ) )
	{
		dResult = Arc66_mapBars( pDevExt, dev );

		if ( dResult )
		{
			ArcPrintf( "PROBE - Failed to map PCI BAR's!\n" );
		}
	}

	/* Initialize character device
	  +----------------------------------------------------------+ */
	if ( !IS_ERR_VALUE( dResult ) )
	{
		dResult = Arc66_cdevInit( pDevExt );

		if ( dResult )
		{
			ArcPrintf( "PROBE - Failed to initialize char device!\n" );
		}
	}

	/* Create the device in /dev
	  +----------------------------------------------------------+ */
	#if LINUX_VERSION_CODE < KERNEL_VERSION(2,6,32)
	pDevExt->pDevice = device_create( g_pArc66Class,
								      NULL,
								      pDevExt->tCDevno,
								      "Arc66PCIe%d",
								      MINOR( pDevExt->tCDevno ) );
	#else
	pDevExt->pDevice = device_create( g_pArc66Class,
								      NULL,
								      pDevExt->tCDevno,
								      pDevExt,
								      "Arc66PCIe%d",
								      MINOR( pDevExt->tCDevno ) );

	#endif

	if ( IS_ERR( pDevExt->pDevice ) )
	{
		ArcPrintf( "PROBE - Failed to create /dev entry, error: %d\n",
				   ( int )PTR_ERR( pDevExt->pDevice ) );

		dResult = ( int )PTR_ERR( pDevExt->pDevice );
	}

	/* +----------------------------------------------------------+
	    !!!!!!!!!!!!!!   OF EXTREME IMPORTANCE   !!!!!!!!!!!!!!
	   +----------------------------------------------------------+
	    Make sure the board is set to BUS MASTER.
	    Linux doesn't do it automatically; Windows does.
	    On linux, if bus master is not set, the RD_IMAGE bit 2
	    of the status register ( 60H ) will not be reset after
	   the first image and all subsequent image will fail!!!
	   +----------------------------------------------------------+ */
	if ( !IS_ERR_VALUE( dResult ) )
	{
		dResult = Arc66_setAsBusMaster( pDevExt );
	}

	/*  Undo everything if we failed
	   +----------------------------------------------------------+ */
	if ( IS_ERR_VALUE( dResult ) )
	{
		Arc66_unMapBars( pDevExt, dev );

		pci_release_regions( dev );

		if ( pDevExt )
		{
			kfree( pDevExt );
		}
	}

//	else
//	{
//		ArcPrintf( "PROBE - DEV_EXT->pPCIDev: %p!\n", pDevExt->pPCIDev );
//		ArcPrintf( "PROBE - DEV_EXT->tCDev: %p!\n", &pDevExt->tCDev );
//		ArcPrintf( "PROBE - DEV_EXT->pBar: %p!\n", pDevExt->pBar );
//		ArcPrintf( "PROBE - DEV_EXT->tCDevno: %d!\n", pDevExt->tCDevno );
//		ArcPrintf( "PROBE - DEV_EXT->pDevice: %p!\n", pDevExt->pDevice );
//		ArcPrintf( "PROBE - DEV_EXT->szName: %s!\n", pDevExt->szName );
//		ArcPrintf( "PROBE - DEV_EXT->dOpen: %d!\n", pDevExt->dOpen );
//	}

	ArcPrintf( "PROBE - Finished looking for ARC-66 boards\n" );

	return dResult;
}


/*
+------------------------------------------------------------------------------+
|  FUNCTION: Arc66_remove()                                                    |
+------------------------------------------------------------------------------+
|  PURPOSE:  This function is called by pci_unregister_driver for each PCI     |
|            device that was successfully probed.                              |
|                                                                              |
|  RETURNS:  Returns 0 for success, or the appropriate error number.           |
|                                                                              |
|  NOTES:    See Documentation/pci.txt in the Linux sources.                   |
+------------------------------------------------------------------------------+
*/
static void Arc66_remove( struct pci_dev* dev )
{
	ArcDevExt *pDevExt = dev_get_drvdata( &dev->dev );

	if ( pDevExt )
	{
		/* Free the image buffer memory block
		  +---------------------------------------------------+ */
		if ( pDevExt->pMemBlock )
		{
			ArcPrintf( "REMOVE - Freeing memory block at 0x%lX.\n",
					   ( ulong )page_to_phys( pDevExt->pMemBlock ) );

			Arc66_bigBufFree( pDevExt->pMemBlock, g_ulSize );
		}

		ArcPrintf( "REMOVE - Removing device #: %d [ Major: %d Minor: %d ]\n",
					pDevExt->tCDevno,
					MAJOR( pDevExt->tCDevno ),
					MINOR( pDevExt->tCDevno ) );

		/* Remove character device
		  +------------------------------------------------+ */
		Arc66_cdevCleanUp( pDevExt );

		/* UnMap the BARS
		  +------------------------------------------------+ */
		Arc66_unMapBars( pDevExt, dev );

		/* Free device extension
		  +------------------------------------------------+ */
		kfree( pDevExt );
	}

	/* Disable the device
	  +----------------------------------------------------+ */
	pci_disable_device( dev );

	/* Release the device regions
	  +----------------------------------------------------+ */
	pci_release_regions( dev );
}


/*
+------------------------------------------------------------------------------+
|  FUNCTION: Arc66_open                                                        |
+------------------------------------------------------------------------------+
|  PURPOSE:  Entry point. Open a device for access.                            |
|                                                                              |
|  RETURNS:  Returns 0 for success, or the appropriate error number.           |
+------------------------------------------------------------------------------+
*/
static int Arc66_open( struct inode* inode, struct file* filp )
{
	int dResult = 0;

	/* Pointer to containing data structure of the character device inode
	  +-------------------------------------------------------------------+ */
	ArcDevExt* pDevExt = container_of( inode->i_cdev, ArcDevExt, tCDev );

	if ( !pDevExt )
	{
		ArcPrintf( "OPEN - Failed to find device extension!\n" );

		dResult = -ENODEV;
	}

	/* Save the device extension to the file pointer for easier future use
	  +-------------------------------------------------------------------+ */
	filp->private_data = pDevExt;

	/* We use a lock to protect global variables
	  +----------------------------------------------------------+ */
	spin_lock( &g_tArc66Lock );

	/* Allow only one process to open the device at a time
	  +----------------------------------------------------------+ */
	if ( !IS_ERR_VALUE( dResult ) && pDevExt->dOpen )
	{
		ArcPrintf( "OPEN - Device %d already opened!\n",
					MINOR( pDevExt->tCDevno ) );

		dResult = -EBUSY;
	}

	if ( !IS_ERR_VALUE( dResult ) )
	{
		/* Increase the module usage count. Prevents accidental
		   unloading of the device while it's in use.
		  +----------------------------------------------------------+ */
		try_module_get( THIS_MODULE );

//		ArcPrintf( "OPEN - Device MAJOR: %d  MINOR:%d  MAX_DEV: %d  OPEN: %d COUNT: %d\n",
//					MAJOR( pDevExt->tCDevno ),
//					MINOR( pDevExt->tCDevno ),
//					ARC66_MAX_DEV,
//					pDevExt->dOpen,
//					g_dUsageCount );

		/* Set the device state to opened
		  +----------------------------------------------------------+ */
		pDevExt->dOpen = 1;

		/*  Initialize the semaphore
		 *  Kernel Change - 2.6.37
		  +----------------------------------------------------------+ */
		#ifndef init_MUTEX
			sema_init( &pDevExt->tSem, 1 );
		#else
			init_MUTEX( &pDevExt->tSem );
		#endif

		/*  Increase the driver usage count. This is for resetting
		    the image buffer "next available" address.
		  +----------------------------------------------------------+ */
		g_dUsageCount++;

//		ArcPrintf( "OPEN - Device MAJOR: %d  MINOR:%d  MAX_DEV: %d  OPEN: %d COUNT: %d\n",
//					MAJOR( pDevExt->tCDevno ),
//					MINOR( pDevExt->tCDevno ),
//					ARC66_MAX_DEV,
//					pDevExt->dOpen,
//					g_dUsageCount );
	}

	spin_unlock( &g_tArc66Lock );

	return dResult;
}

/*
+------------------------------------------------------------------------------+
|  FUNCTION: Arc66_close                                                       |
+------------------------------------------------------------------------------+
|  PURPOSE:  Entry point. Close a device for access.                           |
|                                                                              |
|  RETURNS:  Returns 0 for success, or the appropriate error number.           |
+------------------------------------------------------------------------------+
*/
static int Arc66_close( struct inode* inode, struct file* filp )
{
	int dResult = 0;

	/* Pointer to containing data structure of the character device inode
	  +-------------------------------------------------------------------+ */
	ArcDevExt* pDevExt = ( ArcDevExt * )filp->private_data;

	if ( !pDevExt )
	{
		ArcPrintf( "CLOSE - Failed to find device extension!\n" );

		dResult = -ENODEV;
	}

	if ( !IS_ERR_VALUE( dResult ) && pDevExt->dOpen )
	{
		spin_lock( &g_tArc66Lock );

		/* Set the device state to closed
		  +---------------------------------------------------------------+ */
		pDevExt->dOpen				= 0;
		pDevExt->ulCommonBufferPA	= 0;
		pDevExt->ulCommonBufferVA	= 0;

		/*  Decrease and check the driver usage count.  Reset the
		    counter if no devices are currently open.
		  +----------------------------------------------------------+ */
		g_dUsageCount--;

		if ( g_dUsageCount <= 0 )
		{
			g_ulNextStartAddr = 0;
		}

		spin_unlock( &g_tArc66Lock );
	}

	if ( pDevExt )
	{
		ArcPrintf( "CLOSE - Device: %d:%d closed! USAGE COUNT: %d\n",
					MAJOR( pDevExt->tCDevno ),
					MINOR( pDevExt->tCDevno ),
					g_dUsageCount );
	}

	filp->private_data = NULL;

	/* Decrease the module usage count. This is the "Used" column shown
	   via lsmod or the third parameter in /proc/modules. If the module
	   use count doesn't get decremented properly rmmod will not work, an
	   application may get "Device busy" error and a reboot is required.
	  +-----------------------------------------------------------------+ */
	module_put( THIS_MODULE );

	return dResult;
}


/*
+------------------------------------------------------------------------------+
|  FUNCTION: Arc66_ioctl                                                       |
+------------------------------------------------------------------------------+
|  PURPOSE:  Entry point. Command control for device.                          |
|                                                                              |
|  RETURNS:  Returns 0 for success, or the appropriate error number.           |
+------------------------------------------------------------------------------+
*/
static long Arc66_ioctl( struct file* filp, unsigned int cmd, unsigned long arg )
{
	int dCtrlCode	= 0;
	int dResult		= 0;

	/* Pointer to containing data structure of the character device inode
	  +-------------------------------------------------------------------+ */
	ArcDevExt* pDevExt = ( ArcDevExt * )filp->private_data;

	/* Check that this device actually exists
	  +-------------------------------------------------------------------+ */
	if ( !pDevExt )
	{
		dResult = -ENXIO;
	}

	else
	{
		if ( down_interruptible( &pDevExt->tSem ) )
		{
			ArcPrintf( "IOCTL - Failed to obtain SEMA4!\n" );

			dResult = -EINTR;
		}
	}

	if ( !IS_ERR_VALUE( dResult ) )
	{
		dCtrlCode = EXCMD( cmd );

		#if LINUX_VERSION_CODE < KERNEL_VERSION( 2,6,7 )
		lock_kernel();
		#endif

        switch ( dCtrlCode )
		{
			/*
			+----------------------------------------------------+
			|  READ BAR                                          |
			+----------------------------------------------------+
			|  Parameter definition:                             |
			|  --------------------------                        |
			|  Index 0: Bar number 0 to 5                        |
			|  Index 1: 32-bit bar offset                        |
        	+----------------------------------------------------+
        	*/
			case ARC_READ_BAR:
			{
				uint32_t u32Bar[ 2 ] = { 0, 0 };

        		dResult = copy_from_user( u32Bar,
        								 ( uint32_t * )arg,
        								 sizeof( u32Bar ) );

				if ( !IS_ERR_VALUE( dResult ) )
				{
					void* __iomem pBarAddr =
							pDevExt->pBar[ u32Bar[ BAR_NUM ] ] +
							u32Bar[ BAR_OFF ];

					u32Bar[ 0 ] = ioread32( pBarAddr );

					dResult = copy_to_user( ( uint32_t * )arg,
											u32Bar,
											sizeof( u32Bar ) );
				}
			}
			break;

			/*
			+----------------------------------------------------+
			|  WRITE BAR                                         |
			+----------------------------------------------------+
			|  Parameter definition:                             |
			|  --------------------------                        |
			|  Index 0: Bar number 0 to 5                        |
			|  Index 1: 32-bit bar offset                        |
			|  Index 2: Value to write                           |
        	+----------------------------------------------------+
        	*/
			case ARC_WRITE_BAR:
			{
				uint32_t u32Bar[ 3 ] = { 0, 0, 0 };

        		dResult = copy_from_user( u32Bar,
        								 ( uint32_t * )arg,
        								 sizeof( u32Bar ) );

				if ( !IS_ERR_VALUE( dResult ) )
				{
					void* __iomem pBarAddr =
							pDevExt->pBar[ u32Bar[ BAR_NUM ] ] +
							u32Bar[ BAR_OFF ];

					iowrite32( u32Bar[ BAR_VAL ], pBarAddr );
				}
			}
			break;

			/*
			+----------------------------------------------------+
			|  READ CFG REG 8-Bits                               |
			+----------------------------------------------------+
        	*/
			case ARC_READ_CFG_8:
			{
				uint32_t u32CfgReg = 0;
				uint8_t  u8Value   = 0;

        		dResult = get_user( u32CfgReg, ( uint32_t * )arg );

				if ( !IS_ERR_VALUE( dResult ) )
				{
					dResult = pci_read_config_byte( pDevExt->pPCIDev,
												    u32CfgReg,
												    &u8Value );
				}

				if ( !IS_ERR_VALUE( dResult ) )
				{
					dResult = put_user( ( uint32_t )u8Value,
									    ( uint32_t * )arg );
				}
			}
			break;

			/*
			+----------------------------------------------------+
			|  READ CFG REG 16-Bits                              |
			+----------------------------------------------------+
        	*/
			case ARC_READ_CFG_16:
			{
				uint32_t u32CfgReg = 0;
				uint16_t u16Value  = 0;


        		dResult = get_user( u32CfgReg, ( uint32_t * )arg );

				if ( !IS_ERR_VALUE( dResult ) )
				{
					dResult = pci_read_config_word( pDevExt->pPCIDev,
												    u32CfgReg,
												    &u16Value );
				}

				if ( !IS_ERR_VALUE( dResult ) )
				{
					dResult = put_user( ( uint32_t )u16Value,
									    ( uint32_t * )arg );
				}
			}
			break;

			/*
			+----------------------------------------------------+
			|  READ CFG REG 32-Bits                              |
			+----------------------------------------------------+
        	*/
			case ARC_READ_CFG_32:
			{
				uint32_t u32CfgReg = 0;
				uint32_t u32Value  = 0;


        		dResult = get_user( u32CfgReg, ( uint32_t * )arg );

				if ( !IS_ERR_VALUE( dResult ) )
				{
					dResult = pci_read_config_dword( pDevExt->pPCIDev,
												     u32CfgReg,
												     &u32Value );
				}

				if ( !IS_ERR_VALUE( dResult ) )
				{
					dResult = put_user( u32Value, ( uint32_t * )arg );
				}
			}
			break;

			/*
			+----------------------------------------------------+
			|  WRITE CFG REG 8-Bits                              |
			+----------------------------------------------------+
			|  Parameter definition:                             |
			|  --------------------------                        |
			|  Index 0: Configuration space byte offset          |
			|  Index 1: Value to write                           |
        	+----------------------------------------------------+
        	*/
			case ARC_WRITE_CFG_8:
			{
				uint32_t u32Param[ 2 ] = { 0, 0 };

        		dResult = copy_from_user( u32Param,
        								  ( uint32_t * )arg,
        								  sizeof( u32Param ) );

				if ( !IS_ERR_VALUE( dResult ) )
				{
					dResult =
						pci_write_config_byte(
								pDevExt->pPCIDev,
								u32Param[ CFG_OFFSET ],
								( uint8_t )u32Param[ CFG_VALUE ] );
				}
			}
			break;

			/*
			+----------------------------------------------------+
			|  WRITE CFG REG 16-Bits                             |
			+----------------------------------------------------+
			|  Parameter definition:                             |
			|  --------------------------                        |
			|  Index 0: Configuration space byte offset          |
			|  Index 1: Value to write                           |
        	+----------------------------------------------------+
        	*/
			case ARC_WRITE_CFG_16:
			{
				uint32_t u32Param[ 2 ] = { 0, 0 };

        		dResult = copy_from_user( u32Param,
        								  ( uint32_t * )arg,
        								  sizeof( u32Param ) );

				if ( !IS_ERR_VALUE( dResult ) )
				{
					dResult =
						pci_write_config_word(
								pDevExt->pPCIDev,
								u32Param[ CFG_OFFSET ],
								( uint16_t )u32Param[ CFG_VALUE ] );
				}
			}
			break;

			/*
			+----------------------------------------------------+
			|  WRITE CFG REG 32-Bits                             |
			+----------------------------------------------------+
			|  Parameter definition:                             |
			|  --------------------------                        |
			|  Index 0: Configuration space byte offset          |
			|  Index 1: Value to write                           |
        	+----------------------------------------------------+
        	*/
			case ARC_WRITE_CFG_32:
			{
				uint32_t u32Param[ 2 ] = { 0, 0 };

        		dResult = copy_from_user( u32Param,
        								  ( uint32_t * )arg,
        								  sizeof( u32Param ) );

				if ( !IS_ERR_VALUE( dResult ) )
				{
					dResult =
						pci_write_config_dword(
									pDevExt->pPCIDev,
									u32Param[ CFG_OFFSET ],
									u32Param[ CFG_VALUE ] );
				}
			}
			break;

			/*
			+----------------------------------------------------+
			|  GET BUFFER PROPERTIES                             |
			+----------------------------------------------------+
			|  Parameter definition:                             |
			|  --------------------------                        |
			|  Index 0: Physical address                         |
			|  Index 1: Buffer size ( in bytes )                 |
        	+----------------------------------------------------+
        	*/
			case ARC_BUFFER_PROP:
			{
				ulong ulParam[ 2 ] = { 0, 0 };

        		dResult = copy_from_user( ulParam,
        								  ( ulong * )arg,
        								  sizeof( ulParam ) );

				if ( !IS_ERR_VALUE( dResult ) )
				{
					ulParam[ BUF_PROP_PHYS ] =
							( ulong )pDevExt->ulCommonBufferPA;

					ulParam[ BUF_PROP_SIZE ] =
							( ulong )pDevExt->ulCommonBufferSize;

					dResult = copy_to_user( ( ulong * )arg,
											ulParam,
											sizeof( ulParam ) );
				}
			}
			break;

			/*
			+----------------------------------------------------+
			|  GET BAR SIZE                                      |
			+----------------------------------------------------+
        	*/
			case ARC_BAR_SIZE:
			{
				uint32_t u32BarSize = 0;
				uint32_t u32Bar     = 0;

        		dResult = get_user( u32Bar, ( uint32_t * )arg );

				if ( !IS_ERR_VALUE( dResult ) )
				{
					u32BarSize =
						pci_resource_len( pDevExt->pPCIDev, u32Bar );

					dResult = put_user( u32BarSize, ( uint32_t * )arg );
				}
			}
			break;

			/*
			+----------------------------------------------------+
			|  DEVICE IS OPEN ?                                  |
			+----------------------------------------------------+
        	*/
			case ARC_DEV_OPEN:
			{
				dResult = put_user( pDevExt->dOpen,
									( uint32_t * )arg );
			}
			break;

			/*
			+----------------------------------------------------+
			|  DEFAULT                                           |
			+----------------------------------------------------+
        	*/
			default:
			{
				dResult = -EINVAL;
			}
			break;

		}	// end switch

		#if LINUX_VERSION_CODE < KERNEL_VERSION( 2,6,7 )
        unlock_kernel();
		#endif

		up( &pDevExt->tSem );

	}  // end else

	return ( long )dResult;
}


/*
+------------------------------------------------------------------------------+
|  FUNCTION: Arc66_mmap                                                        |
+------------------------------------------------------------------------------+
|  PURPOSE:  Maps the kernel image buffer to user space.                       |
|                                                                              |
|  RETURNS:  Returns 0 for success, or the appropriate error number.           |
|                                                                              |
|  NOTES:    For this function to work, the user must specify enough buffer    |
|            space by passing "mem=xxxM" to the kernel as a boot parameter.    |
|            If you have 128M RAM and want a 28M image buffer, then use:       |
|            "mem=100M".                                                       |
|                                                                              |
|		From: "Eli Rykoff" <erykoff@umich.edu>:                                |
|                                                                              |
|		The bug is in the astropci_mmap() function, and applies to all         |
|		versions of the astropci driver (RH9, FC2, FC4), when using at least   |
|		1GB of system ram.  It has to do with how the Linux kernel defines     |
|		"low memory" and "high memory." (Which I didn't know before, but I     |
|		sure do now). The Linux kernel can only directly address up to 915MB   |
|		of memory (with usual stock compile options), and the rest of the      |
|		system ram is the "high memory".  The problem comes in determining     |
|		where the top of the system ram is.  In the current version of the     |
|		driver, this is set as "__pa(high_memory)", which ( as it turns out )  |
|		is only equal to the top of the system ram if you have <915MB, and     |
|		maxes out at 0x38000000.  If you have >= 1GB of RAM then you can       |
|		allocate this memory, but as soon as you try to use it you get         |
|		problems. Unfortunately, I have found that the "right" way to do this  |
|		is not very well documented (or documented at all, for that matter),   |
|		but poking into mm.c and page.h in the Linux source reveals that the   |
|		top of the memory is at the address num_physpages*PAGE_SIZE.           |
+------------------------------------------------------------------------------+
*/
static int Arc66_mmap( struct file* filp, struct vm_area_struct* vma )
{
	int dResult = 0;

	/* Pointer to containing data structure of the character device inode
	  +-------------------------------------------------------------------+ */
	ArcDevExt* pDevExt = ( ArcDevExt * )filp->private_data;

	ArcPrintf( "MMAP - called!\n" );

	/* Check that this device actually exists
	  +-------------------------------------------------------------------+ */
	if ( !pDevExt )
	{
		dResult = -ENXIO;
	}

	else
	{
		spin_lock( &g_tArc66Lock );

		/*  Sanity check, failure here should be impossible
		  +---------------------------------------------------------------+ */
		if ( !pDevExt->dOpen )
		{
			ArcPrintf( "MMAP - Invalid device, not open\n" );

			dResult = -EAGAIN;
		}

		if ( !IS_ERR_VALUE( dResult ) )
		{
			ArcPrintf( "MMAP - Mapping memory for device %s\n", pDevExt->szName );

			dResult = Arc66_mapCommonBuffer( pDevExt, vma );
		}

		spin_unlock( &g_tArc66Lock );
	}

	return dResult;
}


/*
+------------------------------------------------------------------------------+
|  FUNCTION: Arc66_mapCommonBuffer                                             |
+------------------------------------------------------------------------------+
|  PURPOSE:  Maps the kernel image buffer to user space.                       |
|                                                                              |
|  RETURNS:  Returns 0 for success, or the appropriate error number.           |
+------------------------------------------------------------------------------+
*/
static int Arc66_mapCommonBuffer( ArcDevExt* pDevExt, struct vm_area_struct* vma )
{
	uint uProt		= 0;
	int  dResult	= 0;
	int  i			= 0;

	unsigned long* pBuffer;


	if ( pDevExt == NULL )
	{
		ArcPrintf( "MMAP - NULL device extension pointer!\n" );

		dResult = -ENODEV;
	}

	if ( !IS_ERR_VALUE( dResult ) )
	{
		if ( ( vma->vm_end - vma->vm_start ) > g_ulSize )
		{
			ArcPrintf(
				"MMAP - Requested buffer size ( %lu ) too large! Common buffer size: %lu.\n",
				( vma->vm_end - vma->vm_start ),
				g_ulSize );

			dResult = -EINVAL;
		}
	}

	if ( !IS_ERR_VALUE( dResult ) )
	{
		/*  Save the image buffer physical address and size
		  +---------------------------------------------------------------------+ */
		pDevExt->ulCommonBufferPA	= ( unsigned long )page_to_phys( pDevExt->pMemBlock );
		pDevExt->ulCommonBufferSize	= ( vma->vm_end - vma->vm_start );

		ArcPrintf( "MMAP - board %s  buffer PA start: 0x%lX  end: 0x%lX size: %lu\n",
				    pDevExt->szName,
				    pDevExt->ulCommonBufferPA,
				    ( pDevExt->ulCommonBufferPA + pDevExt->ulCommonBufferSize ),
				    pDevExt->ulCommonBufferSize );

		/*  Ensure that the memory will not be cached; see drivers/char/mem.c
		  +---------------------------------------------------------------------+ */
		if ( boot_cpu_data.x86 > 3 )
		{
			uProt = pgprot_val( vma->vm_page_prot ) | _PAGE_PCD | _PAGE_PWT;

			vma->vm_page_prot = __pgprot( uProt );
		}

		/*  Set flag for I/O resource AND don't try to swap out physical pages
		  +---------------------------------------------------------------------+ */
#if LINUX_VERSION_CODE >= KERNEL_VERSION(3,1,0)
		vma->vm_flags |= ( VM_IO | VM_LOCKED | (VM_DONTEXPAND | VM_DONTDUMP) );
#else
		vma->vm_flags |= ( VM_IO | VM_RESERVED );
#endif

		/*  Set flag for I/O resource
		  +---------------------------------------------------------------------+ */
	//	vma->vm_flags |= VM_IO;

		/*  Remap the page range to see the high memory
		  +---------------------------------------------------------------------+ */
//		ArcPrintf( "MMAP - Remapping page range ...\n" );

		dResult = remap_pfn_range( vma,
								   vma->vm_start,
								   page_to_pfn( pDevExt->pMemBlock ),
								   ( vma->vm_end - vma->vm_start ),
								   vma->vm_page_prot );

		if ( IS_ERR_VALUE( dResult ) )
		{
			ArcPrintf( "MMAP - Remap page range failed.\n" );
		}
		else
		{
			ArcPrintf( "MMAP - %lu bytes mapped: 0x%lX - 0x%lX --> 0x%lX - 0x%lX\n",
						 pDevExt->ulCommonBufferSize,
						 vma->vm_start,
						 vma->vm_end,
						 pDevExt->ulCommonBufferPA,
						 ( pDevExt->ulCommonBufferPA + pDevExt->ulCommonBufferSize ) );

			/*  Save the virtual address, this seems to be what the driver needs in
			    order to access the image buffer.
			  +---------------------------------------------------------------------+ */
			pDevExt->ulCommonBufferVA = vma->vm_start;

			ArcPrintf( "MMAP - Buffer VA: 0x%lX\n", pDevExt->ulCommonBufferVA );

			/*  Write some test values to the image buffer.
			  +---------------------------------------------------------------------+ */
			ArcPrintf( "MMAP - Writing test values to image buffer\n" );

			pBuffer = ( unsigned long * )pDevExt->ulCommonBufferVA;
			if ( pBuffer != NULL )
			{
#if 0
				for ( i = 0; i < pDevExt->ulCommonBufferSize/sizeof( unsigned long ); i++ )
				{
					#if defined( __x86_64 ) || defined( __x86_64__ )
						pBuffer[ i ] = 0xDEADDEADDEADDEAD;
					#else
						pBuffer[ i ] = 0xDEADDEAD;
					#endif
				}

				ArcPrintf( "MMAP - buffer[ 0 ]: 0x%lX\n", pBuffer[ 0 ] );
				ArcPrintf( "MMAP - buffer[ 1 ]: 0x%lX\n", pBuffer[ 1 ] );
				ArcPrintf( "MMAP - buffer[ 2 ]: 0x%lX\n", pBuffer[ 2 ] );
				ArcPrintf( "MMAP - buffer[ 3 ]: 0x%lX\n", pBuffer[ 3 ] );
#else
				ArcPrintf( "MMAP OK\n" );
#endif
			}
			else
			{
				ArcPrintf( "MMAP - Failed to access image buffer!\n" );
			}

			/*  Assign the image buffers
			  +---------------------------------------------------------------+ */
			Arc66_setBufferAddresses( pDevExt );
		}
	}

	/*  If there was a failure, set imageBufferPhysAddr to zero as a flag
	  +-----------------------------------------------------------------------+ */
	if ( IS_ERR_VALUE( dResult ) )
	{
		pDevExt->ulCommonBufferPA	= 0;
		pDevExt->ulCommonBufferVA	= 0;
		pDevExt->ulCommonBufferSize	= 0;
	}

	return dResult;
}


/*
+------------------------------------------------------------------------------+
|  FUNCTION: Arc66_setBufferAddresses                                          |
+------------------------------------------------------------------------------+
|  PURPOSE:  Pass the DMA kernel buffer address to the board.                  |
|                                                                              |
|  RETURNS:  N/A. No error checking occurs.                                    |
+------------------------------------------------------------------------------+
*/
static void Arc66_setBufferAddresses( ArcDevExt* pDevExt )
{
	void* __iomem pBarAddr = pDevExt->pBar[ 2 ] + ( ulong )0x38;

	iowrite32( pDevExt->ulCommonBufferPA, pBarAddr );
}


/*
+------------------------------------------------------------------------------+
|  FUNCTION: Arc66_mapBars                                                     |
+------------------------------------------------------------------------------+
|  PURPOSE:  Map the device memory regions into kernel virtual address space.  |
|                                                                              |
|  RETURNS:  Returns 0 for success, or the appropriate error number.           |
+------------------------------------------------------------------------------+
*/
static int __init Arc66_mapBars( ArcDevExt *pDevExt, struct pci_dev *dev )
{
	int dResult	= 0;
	int i		= 0;

	for ( i=0; i<BAR_COUNT; i++ )
	{
		/* Get the BAR start address
		  +----------------------------------------------------+ */
		unsigned long ulBarStart  = pci_resource_start( dev, i );
		unsigned long ulBarEnd    = pci_resource_end( dev, i );
		unsigned long ulBarLength = ulBarEnd - ulBarStart + 1;

		pDevExt->pBar[ i ] = NULL;

		/* Only map non-zero memory BARs
		  +----------------------------------------------------+ */
		if ( ulBarStart && ulBarEnd )
		{
			/* Map I/O region into kernel virtual address space
			  +-------------------------------------------------+ */
			pDevExt->pBar[ i ] = pci_iomap( dev, i, ulBarLength );

			/* Move on if we successfully got the I/O address
			  +--------------------------------------------------+ */
			if ( !pDevExt->pBar[ i ] )
			{
				ArcPrintf( "MAP BARS - Failed to map BAR #%d!\n", i );

				dResult = -1;

				break;
			}

//			ArcPrintf( "MAP BARS - Successfully mapped BAR #%d [ size: 0x%lX addr: 0x%lX ]\n",
//						i, ulBarLength, ulBarStart );
		}
	}

	if ( dResult == -1 )
	{
		Arc66_unMapBars( pDevExt, dev );
	}

	return dResult;
}


/*
+------------------------------------------------------------------------------+
|  FUNCTION: Arc66_unMapBars                                                   |
+------------------------------------------------------------------------------+
|  PURPOSE:  Unmap the BAR regions that had been mapped earlier using          |
|            map_bars().                                                       |
|                                                                              |
|  RETURNS:  N/A                                                               |
+------------------------------------------------------------------------------+
*/
static void Arc66_unMapBars( ArcDevExt *pDevExt, struct pci_dev *dev )
{
	int i = 0;

//	ArcPrintf( "UNMAP BARS - Unmapping ALL PCI BARs!\n" );

	for ( i=0; i<BAR_COUNT; i++ )
	{
		if ( pDevExt->pBar[ i ] )
		{
			pci_iounmap( dev, pDevExt->pBar[ i ] );

			pDevExt->pBar[ i ] = NULL;
		}
	}
}


/*
+------------------------------------------------------------------------------+
|  FUNCTION: Arc66_cdevInit                                                    |
+------------------------------------------------------------------------------+
|  PURPOSE:  Initialize character device                                       |
|                                                                              |
|  RETURNS:  Returns 0 for success, or the appropriate error number.           |
+------------------------------------------------------------------------------+
*/
//static int __devinit Arc66_cdevInit( ArcDevExt *pDevExt )
static int Arc66_cdevInit( ArcDevExt *pDevExt )
{
	int dResult	= 0;

	/* Create a new device using the ARC-66 major and the next available minor
	  +-------------------------------------------------------------------------+ */
	if ( g_dArc66Minor >= ARC66_MAX_DEV )
	{
		dResult = -ENODEV;
	}

	else
	{
		pDevExt->tCDevno = MKDEV( g_dArc66Major, g_dArc66Minor++ );

		/* Couple the device file operations to the character device
		  +----------------------------------------------------------+ */
		cdev_init( &pDevExt->tCDev, &Arc66_fops );

		pDevExt->tCDev.owner = THIS_MODULE;
		pDevExt->tCDev.ops	= &Arc66_fops;

		/* Bring character device live
		  +----------------------------------------------------------+ */
		dResult = cdev_add( &pDevExt->tCDev, pDevExt->tCDevno, 1 );

		if ( dResult < 0 )
		{
			ArcPrintf( "CDEV INIT - cdev_add() = %d\n", dResult );

			/* Free the dynamically allocated character device node
			  +-------------------------------------------------------+ */
			unregister_chrdev_region( pDevExt->tCDevno, 1 );
		}

		else
		{
			/* Set the device name in the device extension
			  +-------------------------------------------------------+ */
			sprintf( pDevExt->szName, "%s%d", DEVICE_NAME, MINOR( pDevExt->tCDevno ) );

//			ArcPrintf( "CDEV INIT - Device: %s Major:Minor = %d:%d\n",
//						pDevExt->szName,
//						MAJOR( pDevExt->tCDevno ),
//						MINOR( pDevExt->tCDevno ) );
		}
	}

	return dResult;
}


/*
+------------------------------------------------------------------------------+
|  FUNCTION: Arc66_cdevCleanUp                                                 |
+------------------------------------------------------------------------------+
|  PURPOSE:  Initialize character device                                       |
|                                                                              |
|  RETURNS:  Returns 0 for success, or the appropriate error number.           |
+------------------------------------------------------------------------------+
*/
static void Arc66_cdevCleanUp( ArcDevExt *pDevExt )
{
	/* Remove the character device
	  +---------------------------------------------------+ */
	cdev_del( &pDevExt->tCDev );
}


/*
+------------------------------------------------------------------------------+
|  FUNCTION: Arc66_setAsBusMaster                                              |
+------------------------------------------------------------------------------+
|  PURPOSE:  !!!!!!!!!!!!  OF EXTREME IMPORTANCE   !!!!!!!!!!!!!!              |
|                                                                              |
|  	  Make sure the board is set to BUS MASTER.                                |
|  	  Linux doesn't do it automatically; Windows does.                         |
|  	  On linux, if bus master is not set, the RD_IMAGE bit 2                   |
|  	  of the status register ( 60H ) will not be reset after                   |
|  	  the first image and all subsequent image will fail!!!                    |
|                                                                              |
|  RETURNS:  Returns 0 for success, or the appropriate error number.           |
+------------------------------------------------------------------------------+
*/
static int Arc66_setAsBusMaster( ArcDevExt* pDevExt )
{
	uint16_t	u16CfgVal	= 0;
	int			dResult		= 0;

	dResult = pci_read_config_word( pDevExt->pPCIDev,
								    CFG_COMMAND,
								    &u16CfgVal );

	if ( IS_ERR_VALUE( dResult ) )
	{
		ArcPrintf( "SET AS BUS MASTER - %s %s %d\n",
				   "Failed to read PCI/e BUS MASTER bit!",
				   "Readout WILL likely fail! errno:",
					dResult );
	}

	else
	{
		dResult = pci_write_config_word( pDevExt->pPCIDev,
										CFG_COMMAND,
										( u16CfgVal | 0x4 ) );

		if ( IS_ERR_VALUE( dResult ) )
		{
			ArcPrintf( "SET AS BUS MASTER - %s %s %d\n",
					   "Failed to write PCI/e BUS MASTER bit!",
					   "Readout WILL likely fail! errno:",
					    dResult );
		}
		else
		{
			u16CfgVal = 0;

			dResult = pci_read_config_word( pDevExt->pPCIDev,
										   CFG_COMMAND,
										   &u16CfgVal );

			if ( IS_ERR_VALUE( dResult ) )
			{
				ArcPrintf( "SET AS BUS MASTER - %s %s %d\n",
							"Failed to read PCI/e BUS MASTER bit back!",
							"Readout WILL likely fail! errno:",
							 dResult );
			}
			else
			{
				ArcPrintf(
						"SET AS BUS MASTER - %s Cmd Reg: 0x%X\n",
						"Device set as BUS MASTER.",
						 u16CfgVal );
			}
		}
	}

	return dResult;
}


/*
+=====================================================================================================================+
||                                                                                                                   ||
||                                      Large Memory Allocation Section                                              ||
||                                                                                                                   ||
+=====================================================================================================================+
*/

/*
+------------------------------------------------------------------------------+
|  Un-Comment to allow debug printf statements                                 |
+------------------------------------------------------------------------------+
*/
//#define ARC_BIGBUF_DEBUG


/*
+------------------------------------------------------------------------------+
|  Chapter: basic allocation unit retrieved via the buddy allocator            |
+------------------------------------------------------------------------------+
*/
#define CHAPTER_ORDER	( MAX_ORDER - 1 )		/* page order of chapter */
#define CHAPTER_PAGES	( 1 << CHAPTER_ORDER )		/* pages in a chapter    */
#define CHAPTER_SIZE	( PAGE_SIZE * CHAPTER_PAGES )	/* chapter size in bytes */


/*
+------------------------------------------------------------------------------+
|  Cluster definition                                                          |
+------------------------------------------------------------------------------+
|  We join adjacent chapters into clusters, keeping track of allocations as an |
|  ordered set of clusters.                                                    |
|                                                                              |
|  Note that the physical page frame number (pfn) is stored in the hopes that  |
|  continuous pfn's represent continuous memory. Should we merge clusters via  |
|  dma_addr_t physical addresses?                                              |
+------------------------------------------------------------------------------+
*/
struct cluster
{
	struct list_head head;	/* ordered list of clusters */
	ulong page_first;		/* first page in cluster    */
	ulong page_count;		/* number of pages          */
};


/*
+------------------------------------------------------------------------------+
|  Cluster set definition                                                      |
+------------------------------------------------------------------------------+
*/
struct cluster_set
{
	struct list_head clusters;	/* allocated clusters */
};


/*
+------------------------------------------------------------------------------+
|  Declare and initialize a cluster set                                        |
+------------------------------------------------------------------------------+
*/
#define CLUSTER_SET( name ) \
		struct cluster_set name = { clusters: LIST_HEAD_INIT( name.clusters ) }


/*
+------------------------------------------------------------------------------+
|  Retrieve the cluster from it's list head                                    |
+------------------------------------------------------------------------------+
*/
static struct cluster *get_cluster( struct list_head *node )
{
	return list_entry( node, struct cluster, head );
}


/*
+------------------------------------------------------------------------------+
|  Retrieve the physical address of the first page in the cluster              |
+------------------------------------------------------------------------------+
*/
static inline dma_addr_t phys_start( const struct cluster *cl )
{
	return page_to_phys( pfn_to_page( cl->page_first ) );
}


/*
+------------------------------------------------------------------------------+
|  Retrieve the physical address of the last page in the cluster               |
+------------------------------------------------------------------------------+
*/
static inline dma_addr_t phys_end( const struct cluster *cl )
{
	return page_to_phys(
				nth_page( pfn_to_page( cl->page_first ), cl->page_count ) );
}


/*
+------------------------------------------------------------------------------+
|  Return node after target location for new chapter ( passed as pfn )         |
+------------------------------------------------------------------------------+
*/
static struct list_head *find_insert_location( struct cluster_set* set, ulong chapter_start )
{
	struct list_head *p;

	list_for_each( p, &set->clusters )
	{
		if ( get_cluster( p )->page_first > chapter_start )
		{
			return p;
		}
	}

	return &set->clusters;
}


/*
+------------------------------------------------------------------------------+
|  Try to merge a new chapter by prepending it to the cluster at pos.          |
|  Return true on success, false if unable to merge.                           |
+------------------------------------------------------------------------------+
*/
static bool try_prepend( struct cluster_set *set, struct list_head *pos, ulong chapter_start )
{
	if ( pos != &set->clusters )
	{
		struct cluster *cl = get_cluster( pos );

		if ( chapter_start + CHAPTER_PAGES == cl->page_first )
		{
			cl->page_first = chapter_start;
			cl->page_count += CHAPTER_PAGES;

			return true;
		}
	}

	return false;
}


/*
+------------------------------------------------------------------------------+
|  Try to merge a new chapter by appending it to cluster at pos.               |
|  Return true on success, false if unable to merge.                           |
+------------------------------------------------------------------------------+
*/
static bool try_append( struct cluster_set* set, struct list_head* pos, ulong chapter_start )
{
	if (pos != &set->clusters)
	{
		struct cluster *cl = get_cluster( pos );

		if ( cl->page_first + cl->page_count == chapter_start )
		{
			cl->page_count += CHAPTER_PAGES;

			return true;
		}
	}

	return false;
}


/*
+------------------------------------------------------------------------------+
|  Tries to merge the "pos" cluster with the cluster previous to it.           |
|  Returns true on success (invalidates previous cluster, pos stays valid).    |
+------------------------------------------------------------------------------+
*/
static void try_merge_prev( struct cluster_set* set, struct cluster* pos )
{
	struct list_head *prev_head = pos->head.prev;

	if ( prev_head != &set->clusters )
	{
		struct cluster *prev = get_cluster( prev_head );

		if ( prev->page_first + prev->page_count == pos->page_first )
		{
			pos->page_first = prev->page_first;
			pos->page_count += prev->page_count;

			list_del( prev_head );

			kfree( prev );
		}
	}
}


/*
+------------------------------------------------------------------------------+
|  Account for another chapter allocation, returning the cluster it became     |
|  part of. Returns NULL on error ( out of memory ).                           |
+------------------------------------------------------------------------------+
*/
static struct cluster* add_alloc( struct cluster_set* set, struct page* new_chapter )
{
	ulong chapter_start = page_to_pfn( new_chapter );

	struct list_head *insert_loc = find_insert_location( set, chapter_start );

#ifdef ARC_BIGBUF_DEBUG
	ArcPrintf( "add_alloc - chapter_start: %lu\n", chapter_start );
#endif

	if ( try_prepend( set, insert_loc, chapter_start ) )
	{
		struct cluster *cl = get_cluster( insert_loc );

#ifdef ARC_BIGBUF_DEBUG
		ArcPrintf( "add_alloc - PREPEND\n" );
#endif

		try_merge_prev( set, cl );

		return cl;
	}

	else if ( try_append( set, insert_loc->prev, chapter_start ) )
	{
#ifdef ARC_BIGBUF_DEBUG
		ArcPrintf( "add_alloc - APPEND\n" );
#endif

		return get_cluster( insert_loc->prev );
	}

	else
	{
		struct cluster *new_cluster = kmalloc( GFP_KERNEL, sizeof( *new_cluster ) );

#ifdef ARC_BIGBUF_DEBUG
		ArcPrintf( "add_alloc - NEW CLUSTER\n" );
#endif

		if ( new_cluster )
		{
			new_cluster->page_first = chapter_start;
			new_cluster->page_count = CHAPTER_PAGES;
			list_add_tail( &new_cluster->head, insert_loc );
		}

		return new_cluster;
	}
}


/*
+------------------------------------------------------------------------------+
|  Give up count chapters starting at start.                                   |
+------------------------------------------------------------------------------+
*/
static void free_chapters( struct page* start, unsigned long count )
{
	unsigned long i;

#ifdef ARC_BIGBUF_DEBUG
	ArcPrintf( "Freeing %lu chapters @ 0x%lx.\n", count, ( ulong )page_to_phys( start ) );
#endif

	for ( i = 0; i < count; i++, start = nth_page( start, CHAPTER_PAGES ) )
	{
		__free_pages( start, CHAPTER_ORDER );
	}
}


/*
+------------------------------------------------------------------------------+
|  Free the set and all clusters allocated to it.                              |
+------------------------------------------------------------------------------+
*/
static void free_set( struct cluster_set* set )
{
	struct cluster *pos, *t;

#ifdef ARC_BIGBUF_DEBUG
	ArcPrintf( "Freeing clusters.\n" );
#endif

	list_for_each_entry_safe( pos, t, &set->clusters, head )
	{
		free_chapters(
					   pfn_to_page( pos->page_first ),
					   ( pos->page_count / CHAPTER_PAGES ) );

		kfree( pos );
	}
}


/*
+------------------------------------------------------------------------------+
|  Lists the allocations in the given cluster set.                             |
+------------------------------------------------------------------------------+
*/
#ifdef ARC_BIGBUF_DEBUG
static void list_allocs( struct cluster_set* set )
{
	struct cluster* cluster;

	ArcPrintf( "list_allocs - Allocations in ascending order:\n" );

	list_for_each_entry( cluster, &set->clusters, head )
	{
		ArcPrintf( "list_allocs - Cluster from 0x%08lx .. 0x%08lx ( %lu pages ).\n",
					( ulong )phys_start( cluster ),
					( ulong )phys_end( cluster ),
					cluster->page_count );
	}
}
#endif


/*
+------------------------------------------------------------------------------+
|  Remove the given cluster from the set, keeping the allocation.              |
+------------------------------------------------------------------------------+
*/
static void unlink_cluster( struct cluster_set* set, struct cluster* cl )
{
	list_del_init( &cl->head );
}


/*
+------------------------------------------------------------------------------+
|  Allocate a big buffer of given size [bytes]. Flags as in alloc_pages.       |
+------------------------------------------------------------------------------+
*/
static struct page* Arc66_bigBufAlloc( unsigned int flags, ulong size )
{
	int order = size ? get_order( size ) : 0;

#ifdef ARC_BIGBUF_DEBUG
	ArcPrintf( "size: %ld, order: %d, CHAPTER_ORDER: %d, CHAPTER_SIZE: %ld, CHAPTER_PAGES: %d\n",
			 	size, order, CHAPTER_ORDER, CHAPTER_SIZE, CHAPTER_PAGES );
#endif

	if ( order <= CHAPTER_ORDER )
	{
		return alloc_pages( flags, order );
	}
	else
	{
		struct page* result;

		int chapters = ( size + CHAPTER_SIZE - 1 ) / CHAPTER_SIZE;

		CLUSTER_SET( allocation_set );

		struct cluster* merged;

#ifdef ARC_BIGBUF_DEBUG
		ArcPrintf( "Allocate huge block of size %lu (%i chapters).\n", size, chapters );
#endif

		do
		{
			struct page* chapter = alloc_pages( flags, CHAPTER_ORDER );

			if ( !chapter )
			{
				goto fail;
			}

#ifdef ARC_BIGBUF_DEBUG
			ArcPrintf( "Allocated chapter @ %lx.\n", ( ulong )page_to_phys( chapter ) );
#endif

			merged = add_alloc( &allocation_set, chapter );

			if ( !merged )
			{
				goto fail;
			}

#ifdef ARC_BIGBUF_DEBUG
			list_allocs( &allocation_set );
#endif

		} while ( merged->page_count < chapters * CHAPTER_PAGES );

		unlink_cluster( &allocation_set, merged );

#ifdef ARC_BIGBUF_DEBUG
		ArcPrintf( "After taking result:\n" );

		list_allocs( &allocation_set );
#endif

		free_set( &allocation_set );

		result = pfn_to_page( merged->page_first );

		kfree( merged );

		return result;

fail:
		free_set( &allocation_set );

		ArcPrintf( "Allocation failed.\n" );

		return NULL;
	}
}


/*
+------------------------------------------------------------------------------+
|  Free a buffer allocates by <module>_bigBufAlloc.                            |
+------------------------------------------------------------------------------+
*/
static void Arc66_bigBufFree( struct page* start, ulong size )
{
	int size_order = size ? get_order( size ) : 0;

	if ( size_order < CHAPTER_ORDER )
	{
		__free_pages( start, size_order );
	}
	else
	{
		free_chapters( start, ( ( size + CHAPTER_SIZE - 1 ) / CHAPTER_SIZE ) );
	}
}


MODULE_AUTHOR( "Scott Streit" );
MODULE_DESCRIPTION( "ARC-66/67 PCIe Interface Driver" );
MODULE_SUPPORTED_DEVICE( DEVICE_NAME );

#if LINUX_VERSION_CODE >= KERNEL_VERSION(2,6,11)
	MODULE_LICENSE("GPL");
#endif
