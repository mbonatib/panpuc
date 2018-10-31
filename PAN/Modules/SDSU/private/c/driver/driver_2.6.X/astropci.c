/******************************************************************************
*   File:       astropci.c
*   Author:     Marco Bonati, modified by Scott Streit, 
*               Brian Taylor, and Michael Ashley
*   Abstract:   Linux device driver for the ARC PCI Interface Board.
*
*
*   Revision History:
*     
*   Date      Who Version Description
* -----------------------------------------------------------------------------
* 09-Jun-2000 sds  1.3    Initial.
* 11-Jul-2000 sds  1.4    Changed ioctl GET_REPLY
*                         case to use ASTROPCI_READ_REPLY()
*                         function.
* 13-Jul-2001 sds  1.6    Removed alot of code and added
*                         mmap to support version 1.6.
* 18-Sep-2001 sds  1.7    Fixed unload bug.
* 05-Dec-2001 sds  1.7    Added image buffer code to astropci_t
*                         struct and removed mem_base[]. Fixed
*                         PCI board probing problem. Correct boards
*                         are now found, but this could still be
*                         improved. Supports multiple boards better.
* 11-Jun-2002 sds  1.7    Improved mmap management system. Still
*                         cannot recover used memory very well, however.
* 09-Jan-2002 mcba 1.7    Using new PCI probing technique; register memory
*                         regions; spin_locks; open now enforces only
*                         one process per board; removed unnecessary includes;
*                         removed lots of code; reduced size of static structs;
*                         MAX_DEV consistency check; more use of MOD_INC/DEC_
*                         USAGE_COUNT; +ve error returns made -ve; added
*                         astropci_wait_for_condition, and made it sleep for
*                         long delays; fixed memory caching in mmap; added
*                         /proc interface; MODULE_ stuff; EXPORT_NO_SYMBOLS;
*                         LINUX_VERSION_CODE switches; removed unnecessary
*                         wait queues.
* 15-Apr-2003 sds  1.7    Re-added support for multiple boards. Fixed mmap
*			  	  to support 2.4.20-8 kernel.
* 30-Aug-2005 sds  1.7    Added Read/Write register functions, which include
*				  delays before reading/writing the PCI DSP registers
*				  (HCTR, HSTR, etc). Also includes checking bit 1 of
*				  the HCVR, if it's set, do not write to the HCVR register.
*				  Also did general cleanup, including re-writing the
*				  astropci_wait_for_condition function. Updated for current
*				  kernel PCI API.
* 30-May-2007 mbb  1.7   Small modifications for multiple cards/multiple processors (SMP)
				- pci_register_driver return number_of_devices+1, so the
				  return condition was modified to be >= 0 instead of =0
				- alloc_chrdev_region. the seconf argument was modified to
				  be "i" (device number) instead of 0. The 0 was causing a
				  weird probloem where the minor number appeared to be 
				  correct on the /dev entry, but the user-space "open"
				  routine would refuse to recognize the device. This was
				  fixed by explicitaly requesting a minor number=dev number
				- Spinlocks:
					- added spin_lock_init on the _init routine
					- added spin_lock_irqsave / spin_unlock_irqrestore
					  on the interrupt handler routine
					- Taken off the spin_lock from the ioctl routine
					  This was causing the computer to hang when both
					  cards where busy reading, probably because the
					  second processor could not access to the second
					  card while the first one had the ioctl locked
				  As for now I tested the driver reading two cards at the
				  same time in a dual processor machine without troubles
				  (Note that the PCI cards were on diferent buses and
				   tested if they are both on the same PCI bus)
				- Changed the Development notes to match kernel 2.6

* 12Mar2010 mbb 1.8	Support for kernel >2.6.24 (interruptions)

*
*   Development notes:
*   -----------------
*   This driver has been tested on RedHat Enterprise with Kernel 2.6.9
*   It compiles under at least 2.4.2, 2.4.7, 2.4.18, and 2.4.20.
*
*   This version is capable of handling multiple boards. The maximum number
*   of boards is given by the MAX_DEV parameter. Any number of boards less than
*   or equal to MAX_DEV will be automatically handled. At this time, 
*   MAX_DEV is set to 4, but it is easy to increase that number
*   (you must also add more definitions to the "board" constant).
*
******************************************************************************/
#include <linux/version.h>
#include <linux/pci.h>
#include <linux/init.h>
#include <linux/delay.h>
#include <linux/module.h>
#include <linux/sched.h>
#include <linux/proc_fs.h>
#include <linux/interrupt.h>
#include <linux/cdev.h>
#include <asm/uaccess.h>
#include "astropci_defs.h"
#include "astropci_io.h"

/******************************************************************************
        Global variables
******************************************************************************/
static astropci_t astropci[MAX_DEV];
static const char *board[] = { "astropci0", "astropci1", "astropci2", "astropci3" };
//static u_int major[MAX_DEV];
static dev_t devNum[MAX_DEV];
static uint32_t nextValidStartAddress;
static spinlock_t astropci_lock = SPIN_LOCK_UNLOCKED;
static int ndev = 0;		// Total number of boards found

//static struct proc_dir_entry *astropciProcFile;

/******************************************************************************
        Prototypes for main entry points
******************************************************************************/
static int astropci_open (struct inode *, struct file *);
static int astropci_close (struct inode *, struct file *);
static int astropci_ioctl (struct inode *, struct file *, unsigned int, unsigned long);
static int astropci_mmap (struct file *file, struct vm_area_struct *vma);

/******************************************************************************
        Prototypes for other functions
******************************************************************************/
static int __devinit astropci_probe (struct pci_dev *pdev, const struct pci_device_id *ent);
static void __devexit astropci_remove (struct pci_dev *pdev);
static irqreturn_t astropci_intr (int irq, void *dev_id, struct pt_regs *regs);
//static int astropci_read_proc (char *buf, char **start, off_t offset, int len, int *eof, void *data);
static int astropci_flush_reply_buffer (int dev);
static int astropci_check_reply_flags (int dev);
static int astropci_check_dsp_input_fifo (int dev);
static int astropci_check_dsp_output_fifo (int dev);
static uint32_t astropci_wait_for_condition (int dev, int condition_to_wait_for);
static int astropci_set_buffer_addresses (int dev);
static unsigned int ReadRegister_32(long addr);
static unsigned short ReadRegister_16(long addr);
static void WriteRegister_32(unsigned int value, long addr);
static void WriteRegister_16(unsigned short value, long addr);
static unsigned int Read_HCTR(int dev);
static unsigned int Read_HSTR(int dev);
static unsigned int Read_REPLY_BUFFER_32(int dev);
static unsigned short Read_REPLY_BUFFER_16(int dev);
static void Write_HCTR(int dev, unsigned int regVal);
static void Write_CMD_DATA_32(int dev, unsigned int regVal);
static void Write_CMD_DATA_16(int dev, unsigned short regVal);
static int Write_HCVR(int dev, unsigned int regVal);

#if LINUX_VERSION_CODE >= KERNEL_VERSION(2,6,5)
static int __init astropci_init(void);
static void __exit astropci_exit(void);
#else
int init_module (void);
void cleanup_module (void);
#endif

/* As of kernel 2.4, you can rename the init and cleanup functions. They
   no longer need to be called init_module and cleanup_module. The macros
   module_init and module_exit, found in linux/init.h, can be used to
   define your own functions. BUT, these macros MUST be called after defining
   init and cleanup functions. */
module_init(astropci_init);
module_exit(astropci_exit);

/******************************************************************************
        Structures used by the kernel PCI API
******************************************************************************/
static struct pci_device_id __devinitdata astropci_pci_tbl[] =
{
	{ PCI_DEVICE( PCI_VENDOR_ID_MOTOROLA, ARC_PCI_DEVICE_ID ) },
	{ 0, }
};
MODULE_DEVICE_TABLE(pci, astropci_pci_tbl);

static struct pci_driver astropci_driver = {
        .name      =   DRIVER_NAME,
        .id_table  =   astropci_pci_tbl,
        .probe     =   astropci_probe,
#if LINUX_VERSION_CODE >= KERNEL_VERSION(2,4,20)
        .remove    =    __devexit_p(astropci_remove),
#else
        .remove    =    astropci_remove,
#endif
};

/******************************************************************************
        file_operations - character/block entry points structure. Contains
                          entry points for drivers that support both character
                          and block entry points. We only specify the functions
                          that we provide.
******************************************************************************/
static struct file_operations astropci_fops = {
        .owner   = THIS_MODULE,
        .ioctl   = astropci_ioctl,
        .mmap    = astropci_mmap,
        .open    = astropci_open,
        .release = astropci_close
};

/******************************************************************************
 FUNCTION: INIT_MODULE()
 
 PURPOSE:  Initializes the module.
 
 NOTES:    Called by Linux during insmod, not by the user.
******************************************************************************/
#if LINUX_VERSION_CODE >= KERNEL_VERSION(2,6,5)
static int __init astropci_init(void)
#else
int init_module (void)
#endif
{
	int retval   = 0;
	int i;

	astropci_printf("+-------------------------------+\n");
	astropci_printf("|   ASTROPCI - Initialization   |\n");
	astropci_printf("+-------------------------------+\n");


	retval = pci_register_driver(&astropci_driver);

	astropci_printf("pci_register returned %d (ndev %d) (yes, me know)\n", retval, ndev);

	if ( retval >= 0 )
	{
		for ( i=0; i<ndev; i++ )
		{
			// Allocate device numbers. i.e. the major number of the device
			retval = alloc_chrdev_region(&devNum[i], i, 1, board[i]);

			if ( retval == 0 )
			{
				cdev_init(&astropci[i].cdev, &astropci_fops);
				astropci[i].cdev.owner = THIS_MODULE;
				astropci[i].cdev.ops = &astropci_fops;
			}

			retval = cdev_add(&astropci[i].cdev, devNum[i], 1);

			if ( retval == 0 )
				astropci_printf("Major device number %d\n", MAJOR(devNum[i]));
			else
				astropci_printf("Error adding major device number\n");
		}

		astropci_printf("Number of PCI boards found: %d\n", ndev);
		astropci_printf("ASTROPCI --> End init_module\n");

		nextValidStartAddress = 0;
	}
	spin_lock_init (&astropci_lock);

	return retval;
}

/******************************************************************************
 FUNCTION: CLEANUP_MODULE()
 
 PURPOSE:  Remove all resources allocated by init_module().
 
 NOTES:    Called by Linux during rmmod, not by the user.
******************************************************************************/
#if LINUX_VERSION_CODE >= KERNEL_VERSION(2,6,5)
static void __exit astropci_exit(void)
#else
void cleanup_module (void)
#endif
{
	int i; //, retval = 0;

	for ( i=0; i<ndev; i++ )
	{
		if ( devNum[i] > 0 )
		{
//                remove_proc_entry(board[i], &proc_root); //(board[i], NULL);

			astropci_printf("Unregistering board #%d\n", i);
			cdev_del(&astropci[i].cdev);
			unregister_chrdev_region(devNum[i], 1);

			pci_unregister_driver (&astropci_driver);
        	}
	}

 	astropci_printf("Driver unloaded\n");
}

/******************************************************************************
 FUNCTION: ASTROPCI_PROBE()
 
 PURPOSE:  This function is called by pci_register_driver for each PCI device
           that matches the pci_driver struct.
 
 RETURNS:  Returns 0 for success, or the appropriate error number.
 
 NOTES:    See Documentation/pci.txt in the Linux sources.
******************************************************************************/
static int __devinit 
astropci_probe (struct pci_dev *pdev, const struct pci_device_id *ent)
{
	int retval   = 0;
	int dev      = 0;
	long memaddr = 0;

	astropci_printf("ASTROPCI --> Start Probe\n");

	if ( ndev == MAX_DEV )
	{
		astropci_printf("WARNING: More than the maximum number of devices (%d) found.\n \
				    Please recompile if you need more.\n", MAX_DEV);
	}

	else
	{
		dev = ndev;
		ndev++;

		// Initialize device before it's used by the driver
		retval = pci_enable_device(pdev);

		if ( retval != 0 )
		{
			ndev--;
        		retval = -ENODEV;
		}
		else
		{
			// Mark all PCI I/O regions associated with the PCI device
			// as being reserved by the specified owner.
#if LINUX_VERSION_CODE >= KERNEL_VERSION(2,4,20)
			retval = pci_request_regions(pdev, (char *)board[dev]);
#endif
		}

		if ( retval == 0 )
		{
			// Set the private data field in pdev, so that we can determine
			// which board is referred to. Note that we are storing the 
			// current value of dev, not a pointer to it.
			pci_set_drvdata(pdev, (void *)dev);

			// Create and save a new device state structure.
			memset(&astropci[dev], 0x00, sizeof (struct astropci_state));
			astropci[dev].pdev = pdev;

			// Get the start address of the PCI's I/O resources.
			memaddr = pci_resource_start(pdev, 0);
			astropci[dev].ioaddr = (long) ioremap_nocache(memaddr, REGS_SIZE); // call iounmap(memaddr);

			// Move on if we successfully got the PCI's I/O address.
			if ( astropci[dev].ioaddr )
			{
				// Install the interrupt service routine. The pci_dev structure
				// that's passed in already contains the IRQ read from the PCI
				// board. Uses shared interrupts.
#if LINUX_VERSION_CODE < KERNEL_VERSION(2,6,24)
				retval = request_irq( pdev->irq,
					    		    &astropci_intr,
					    		    SA_INTERRUPT | SA_SHIRQ,
					    		    board[dev],
					    		    (void *)(dev+1) );
#else
                                retval = request_irq( pdev->irq,
                                                            &astropci_intr,
                                                            IRQF_SHARED,
                                                            board[dev],
                                                            (void *)(dev+1) );
#endif

				// IMPORTANT - DO NOT mess with the enable_irq and disable_irq
				// functions when using shared interrupts, because these will
				// disable the interrupt for all devices using the IRQ, which
				// may cause problems for other devices, such as the NIC.

				if ( retval == 0 )
				{
					astropci[dev].have_irq = 1;

					// Enable the device interrupts
        				astropci_printf("successfully probed dev %d, irq %d, func %d PCI IRQ: %d\n",
							    dev, pdev->irq, pdev->devfn, astropci[dev].irq);
				}
				else
				{
					astropci_printf("request IRQ %d failed\n", pdev->irq);
				}
			}
			else
			{
			#if LINUX_VERSION_CODE >= KERNEL_VERSION(2,6,9)
				astropci_printf("IOremap failed for device %s, region 0x%X @ 0x%lX\n",
					     	    pci_name(pdev), REGS_SIZE, memaddr);
			#else
				astropci_printf("IOremap failed for device %s, region 0x%X @ 0x%lX\n",
					          pdev->slot_name, REGS_SIZE, memaddr);
			#endif
			}
		}
	}

	// Undo everything if we failed.
	if ( retval != 0 )
	{
		iounmap((void *)astropci[dev].ioaddr);
		pci_release_regions(pdev);
	}

	astropci_printf("ASTROPCI --> End Probe\n");

	return retval;
}

/******************************************************************************
 FUNCTION: ASTROPCI_REMOVE()
 
 PURPOSE:  This function is called by pci_unregister_driver for each 
           PCI device that was successfully probed.
 
 NOTES:    See Documentation/pci.txt in the Linux sources.
******************************************************************************/
static void __devexit 
astropci_remove (struct pci_dev *pdev)
{
	int dev = 0;
      
	dev = (int)pci_get_drvdata(pdev);

	astropci_printf("Removing device #: %d\n", dev);

	if ( (dev >= 0) && (dev < MAX_DEV) )
	{
		if ( astropci[dev].have_irq )
		{
			free_irq(pdev->irq, (void *)(dev+1));
			astropci[dev].have_irq = 0;
		}

		if ( astropci[dev].ioaddr != 0 )
			iounmap((void *)(astropci[dev].ioaddr));

		#if LINUX_VERSION_CODE >= KERNEL_VERSION(2,4,20)
		pci_release_regions(pdev);
		#endif

		#if LINUX_VERSION_CODE >= KERNEL_VERSION(2,4,3)
		pci_disable_device(pdev);
		#endif

		pci_set_drvdata(pdev, (void *)-1);
	}
	else
	{
		astropci_printf("Bad device number %d\n", dev);
	}
}

/******************************************************************************
 FUNCTION: ASTROPCI_OPEN()
 
 PURPOSE:  Entry point. Open a device for access.
 
 RETURNS:  Returns 0 for success, or the appropriate error number.
******************************************************************************/
static int
astropci_open(struct inode *inode, struct file *file)
{
	uint32_t value = 0;
	int dev        = 0;
	int retval     = 0;

	// We use a lock to protect global variables
	astropci_printf("entering to open function");
	spin_lock(&astropci_lock);

	// Obtain the minor device number, which is used to determine
	// which of the possible ARC cards we want to open
	dev = MINOR(inode->i_rdev);
	astropci_printf("dev %d, have_irq %d\n", dev, astropci[dev].have_irq);
	// Check that this device actually exists
	if ( (dev < 0) || (dev >= MAX_DEV) || (!astropci[dev].have_irq) )
		retval = -ENXIO;
	astropci_printf("passed that ...\n");

	if ( retval == 0 )
	{
 		// Allow only one process to open the device at a time
		if ( astropci[dev].opened )
			retval = -EBUSY;
	}

	if ( retval == 0 )
	{
		// Write 0xFF to the configuration-space address PCI_LATENCY_TIMER
		pci_write_config_byte(astropci[dev].pdev, PCI_LATENCY_TIMER, 0xFF);

		// Set HCTR bit 8 to 1 and bit 9 to 0 for 32-bit PCI commands -> 24-bit DSP data
		// Set HCTR bit 11 to 1 and bit 12 to 0 for 24-bit DSP reply data -> 32-bit PCI data
		value = Read_HCTR(dev);
		Write_HCTR( dev, ((value & 0xCFF) | 0x900) );

 		if ( astropci_flush_reply_buffer(dev) )
		{
			astropci_printf("Flush reply buffer failed\n");
			retval = -EACCES;
		}
	}

	if ( retval == 0 )
	{
		// Set the device state to opened
		astropci[dev].opened = 1;

		// Store the board number into the file structure,
		// so that mmap() can easily determine the board.
		file->private_data = (void *)dev;
	}
	spin_unlock(&astropci_lock);

	// Increase the module usage count. Prevents accidental
	// unloading of the device while it's in use.
	try_module_get(THIS_MODULE);
	astropci_printf("exiting open, retval %d\n", retval);

	return retval;
}

/******************************************************************************
 FUNCTION: ASTROPCI_CLOSE()
 
 PURPOSE:  Entry point. Close a device from access.
 
 RETURNS:  Returns 0 always.
******************************************************************************/
static int
astropci_close(struct inode *inode, struct file *file)
{
	int dev = 0;

	spin_lock(&astropci_lock);

	dev = MINOR(inode->i_rdev);

	// Set the device state to closed
	astropci[dev].opened              = 0;
	astropci[dev].imageBufferPhysAddr = 0;
	astropci[dev].imageBufferVirtAddr = 0;

	// Check if all devices are closed. If so, reset nextValidStartAddress
	for ( dev = 0; dev < MAX_DEV; dev++ )
	{
 		if (astropci[dev].opened) break;
	}

	if ( dev == MAX_DEV )
		nextValidStartAddress = 0;

	spin_unlock(&astropci_lock);

	// Decrease the module usage count
	module_put(THIS_MODULE);

	return 0;
}

/******************************************************************************
 FUNCTION: ASTROPCI_IOCTL()
 
 PURPOSE:  Entry point. Control a character device.
 
 RETURNS:  Returns 0 for success, or the appropriate error number.
 
 NOTES:    Locking is used to prevent simultaneous access.
******************************************************************************/
static int
astropci_ioctl(struct inode *inode, struct file *file, unsigned int cmd,
               unsigned long arg)
{
	uint32_t reg   = 0;
	uint32_t reply = 0;
	int retval     = 0;
	int dev        = 0;

	dev = MINOR(inode->i_rdev);

	// Check that this device actually exists
	if ( dev < 0 || dev >= MAX_DEV || !astropci[dev].have_irq )
	{
		retval = -ENXIO;
	}

	else
	{
        	switch ( cmd )
		{
			// -----------------------------------------------
			//  GET HCTR
			// -----------------------------------------------
        		case ASTROPCI_GET_HCTR:
			{
				reg = Read_HCTR(dev);

                		if ( put_user(reg, (uint32_t *) arg) )
					retval = -EFAULT;
			} break;

 			// -----------------------------------------------
			//  GET PROGRESS
			// -----------------------------------------------
       		case ASTROPCI_GET_PROGRESS:
			{
                		uint32_t progress = 0;
                		uint32_t upper    = 0;
                		uint32_t lower    = 0;

                		// Ask the PCI board for the current image address
                		retval = Write_HCVR( dev, (uint32_t)READ_PCI_IMAGE_ADDR );

                		// Read the current image address
				if ( retval == 0 )
				{
                			if ( astropci_check_dsp_output_fifo(dev) )
					{
                        			lower = Read_REPLY_BUFFER_16(dev);
                        			upper = Read_REPLY_BUFFER_16(dev);
                        			progress = ((upper << 16) | lower);
                			}
					else
                        			retval = -EFAULT;
				}

               		if ( put_user(progress, (uint32_t *)arg) )
                       		retval = -EFAULT;

			} break;

			// -----------------------------------------------
			//  GET FRAMES READ
			// -----------------------------------------------
        		case ASTROPCI_GET_FRAMES_READ:
			{
 	    			uint32_t pciFrameCount = 0;
  	    			uint32_t upper         = 0;
  	    			uint32_t lower         = 0;

  				// Ask the PCI board for the current image address
   				retval = Write_HCVR( dev, (uint32_t)READ_NUMBER_OF_FRAMES_READ );

				// Read the current image address
				if ( retval == 0 )
				{
					if ( astropci_check_dsp_output_fifo(dev) == OUTPUT_FIFO_OK_MASK )
					{
	 					lower = Read_REPLY_BUFFER_16(dev);
	 					upper = Read_REPLY_BUFFER_16(dev);
	 					pciFrameCount = ((upper << 16) | lower);
					}
	 				else
	 					retval = -EFAULT;
				}

				if ( put_user (pciFrameCount, (uint32_t *)arg) )
					retval = -EFAULT;
			} break;

			// -----------------------------------------------
			//  GET HSTR
			// -----------------------------------------------
        		case ASTROPCI_GET_HSTR:
			{
                		reg = Read_HSTR(dev);
                
				if ( put_user(reg, (uint32_t *) arg) )
 					retval = -EFAULT;
			} break;

			// -----------------------------------------------
			//  GET DMA ADDR
			// -----------------------------------------------
			case ASTROPCI_GET_DMA_ADDR:
			{
                		if ( put_user((uint32_t) astropci[dev].imageBufferPhysAddr, (uint32_t *) arg) )
                        		retval = -EFAULT;
			} break;

			// -----------------------------------------------
			//  SET HCTR
			// -----------------------------------------------
        		case ASTROPCI_SET_HCTR:
			{
				if ( get_user(reg, (uint32_t *) arg) )
					retval = -EFAULT;
				else
					Write_HCTR(dev, reg);
			} break;

			// -----------------------------------------------
			//  SET HCVR
			// -----------------------------------------------
        		case ASTROPCI_SET_HCVR:
			{
                		uint32_t Hcvr;

                		if ( get_user(Hcvr, (uint32_t *) arg) )
				{
                        		retval = -EFAULT;
                		}
				else
				{
					// Clear the status bits if command not ABORT_READOUT. 
					// The pci board can't handle maskable commands 
					// (0xXX) during readout.
					if ( Hcvr != ABORT_READOUT )
						retval = Write_HCVR( dev, (uint32_t) CLEAR_REPLY_FLAGS );

					// Pass the command to the PCI board
					if ( retval == 0 )
	                       		retval = Write_HCVR( dev, Hcvr );

					if ( retval == 0 )
					{
                        		// Return reply
                        		reply = astropci_check_reply_flags(dev);

						if ( reply == RDR )
						{
	                              	// Flush the reply buffer
                                		astropci_flush_reply_buffer(dev);

							// Go read some data
                                		retval = Write_HCVR( dev, (uint32_t) READ_REPLY_VALUE );

							if ( retval == 0 )
							{
								if ( astropci_check_dsp_output_fifo(dev) )
                                    			reply = Read_REPLY_BUFFER_32(dev);
 								else
                                    			retval = -EFAULT;
							}
						}
					}
                		}

				// A value must be returned to the user,
				// so do not protect it.
      			if ( put_user(reply, (uint32_t *) arg) )
               			retval = -EFAULT;
        		} break;

			// -----------------------------------------------
			//  SET HCVR DATA
			// -----------------------------------------------
        		case ASTROPCI_HCVR_DATA:
			{
                		uint32_t Cmd_data;

                		if ( get_user(Cmd_data, (uint32_t *) arg) )
                        	retval = -EFAULT;
				else
				{
                        	if ( astropci_check_dsp_input_fifo(dev) )
                              	Write_CMD_DATA_32(dev, Cmd_data);
                        	else
                              	retval = -EIO;
                		}
			} break;

			// -----------------------------------------------
			//  SEND COMMAND
			// -----------------------------------------------
        		case ASTROPCI_COMMAND:
			{
                		uint32_t Cmd_data[6] = { 0, 0, 0, 0, 0, 0 };
				int numberOfParams   = 0;
				int i                = 0;

                		if ( copy_from_user(Cmd_data, (uint32_t *) arg, sizeof (Cmd_data)) )
                			retval = -EFAULT;

                		else
				{
					// Check that the command isn't maskable and that
					// we're currently not in readout.
					uint32_t currStatus = ( Read_HSTR(dev) & HTF_BIT_MASK ) >> 3;
					if ( (Cmd_data[1] & 0x8000) == 0 && currStatus == READOUT_STATUS )
					{
						retval = -EIO;
					}

                       		// Clear the status bits
					if ( retval == 0 )
	                       		retval = Write_HCVR( dev, (uint32_t)CLEAR_REPLY_FLAGS );
                        
					// Wait for the FIFO to be empty.
                        	if ( retval == 0 )
					{
                        		if ( !astropci_check_dsp_input_fifo(dev) )
                               		retval = -EIO;
                       		}
						
					if ( retval == 0 )
					{
						// Get the number of command parameters.
						numberOfParams = Cmd_data[0] & 0x000000FF;
						if ( numberOfParams > CMD_MAX )
						{
							astropci_printf("Incorrect number of command parameters!\n");
							retval = -EFAULT;
						}
						else
						{
                                		// All is well, so write rest of the data.
                                		for ( i = 0; i < numberOfParams; i++ )
								Write_CMD_DATA_32(dev,Cmd_data[i]);

							// Tell the PCI board to do a WRITE_COMMAND vector command
							retval = Write_HCVR( dev, (uint32_t)WRITE_COMMAND );
						}
					}

					if ( retval == 0 )
					{
						// Set the reply
		                        reply =  astropci_check_reply_flags(dev);

						if ( reply == RDR )
						{
                                    	// Flush the reply buffer
                                    	astropci_flush_reply_buffer(dev);

                                        	// Go read some data
                                        	retval = Write_HCVR( dev, (uint32_t)READ_REPLY_VALUE );

							if ( retval == 0 )
							{
                                        		if ( astropci_check_dsp_output_fifo(dev) )
	                                    		reply = Read_REPLY_BUFFER_32(dev);
								else
									retval = -EFAULT;
							}
						}
					}
				}

				// Return reply
				Cmd_data[0] =  reply;
				if ( copy_to_user((uint32_t *) arg, Cmd_data, sizeof (Cmd_data)) )
					retval = -EFAULT;
        		} break;

			// -----------------------------------------------
			//  PCI DOWNLOAD
			// -----------------------------------------------
        		case ASTROPCI_PCI_DOWNLOAD:
			{
                		// This vector command is here because it expects NO reply.
                		retval = Write_HCVR( dev, (uint32_t)PCI_DOWNLOAD );
			} break;

			// -----------------------------------------------
			//  PCI DOWNLOAD WAIT
			// -----------------------------------------------
        		case ASTROPCI_PCI_DOWNLOAD_WAIT:
			{
                		reply = astropci_check_reply_flags(dev);

                		if ( put_user(reply, (uint32_t *) arg) )
                  		retval = -EFAULT;
			} break;

			// -----------------------------------------------
			//  MEMORY UNMAP
			// -----------------------------------------------
        		case ASTROPCI_MUNMAP:
			{
                		astropci[dev].imageBufferPhysAddr = 0;
				astropci[dev].imageBufferVirtAddr = 0;
                		astropci_printf("board[%d] unmapped!\n", dev);
			} break;

#ifdef DEBUG_ON
			case 999:
			{
				unsigned long *buffer;

				astropci_printf("----------------------------------\n");
				astropci_printf("Reading values from image buffer\n");
				astropci_printf("----------------------------------\n");
				buffer = (unsigned long *)astropci[dev].imageBufferVirtAddr; //imageBufferPhysAddr;
				if ( buffer != NULL )
				{
					buffer[0] = 0x11111111;
					buffer[1] = 0x22222222;
					buffer[2] = 0x33333333;
					buffer[3] = 0x44444444;

					astropci_printf("ASTROPCI buffer[0]: 0x%lX\n", buffer[0]);
					astropci_printf("ASTROPCI buffer[1]: 0x%lX\n", buffer[1]);
					astropci_printf("ASTROPCI buffer[2]: 0x%lX\n", buffer[2]);
					astropci_printf("ASTROPCI buffer[3]: 0x%lX\n", buffer[3]);
				}
				else
				{
					astropci_printf("ASTROPCI ERROR, failed to access image buffer (0x%X)!\n",
							    astropci[dev].imageBufferPhysAddr);
				}
			} break;
#endif

			// -----------------------------------------------
			//  DEFAULT
			// -----------------------------------------------
        		default:
			{
                		retval = -EINVAL;
			} break;
		}  // else
	}  // switch


        return retval;
}

/******************************************************************************
 FUNCTION: ASTROPCI_INTR()
 
 PURPOSE:  Entry point. Interrupt handler.
 *****************************************************************************/
static irqreturn_t
astropci_intr(int irq, void *dev_id, struct pt_regs *regs)
{
	uint32_t int_flag = 0;
	int dev           = 0;
	unsigned int	flags;

	dev = (int)dev_id;
	dev--;

	spin_lock_irqsave (&astropci_lock, flags);
	if ( (dev >= 0) && (dev < MAX_DEV) && (astropci[dev].have_irq) )
		int_flag = Read_HSTR(dev);

	// If no devices match the interrupting device, then exit
	if ( !(int_flag & DMA_INTERRUPTING) )
	{
//		astropci_printf ("couldn't find interrupt dev (%d) among the known devices\n", dev);
		return -IRQ_NONE;
	}

	// Clear the interrupt, no questions asked
	astropci_printf("Clearing Interrupts!\n");
	Write_HCVR( dev, (uint32_t)CLEAR_INTERRUPT );

	// OK, we found the device which is interrupting
	PDEBUG("device %d interrupting\n", dev);
	spin_unlock_irqrestore (&astropci_lock, flags);

	return 0;
}

/******************************************************************************
 FUNCTION: ASTROPCI_SET_BUFFER_ADDRESSES
 
 PURPOSE:  Pass the DMA kernel buffer address to the DSP
 
 RETURNS:  0 on success, -ENXIO on failure.
******************************************************************************/
static int
astropci_set_buffer_addresses(int dev)
{
	uint32_t phys_address = 0;
	int retval            = 0;

	// Pass the DMA kernel buffer address to the DSP
	if ( astropci_check_dsp_input_fifo(dev) )
	{
		phys_address = astropci[dev].imageBufferPhysAddr;
		Write_CMD_DATA_16( dev, (uint16_t) (phys_address & 0x0000FFFF) );
		Write_CMD_DATA_16( dev, (uint16_t) ((phys_address & 0xFFFF0000) >> 16) );

		astropci_printf("DMA buffer address ( 0x%X )set on PCI board number %d\n", phys_address, dev);
 	}
	else
	{
		astropci_printf("Timeout while setting DMA buffer address of board %d\n", dev);
		retval = -ENXIO;
	}

	if ( retval == 0 )
	{
		retval = Write_HCVR( dev, (uint32_t) WRITE_PCI_ADDRESS );

 		// Check the reply
		if ( retval == 0 )
		{
			if ( astropci_check_reply_flags(dev) != DON )
				retval = -ENXIO;
		}
	}

	return retval;
}

/******************************************************************************
 FUNCTION: ASTROPCI_FLUSH_REPLY_BUFFER
 
 PURPOSE:  Utility function to clear DSP reply buffer and driver 
           internal value.
 RETURNS:  Returns 0 if successful. Non-zero otherwise.
******************************************************************************/
static int
astropci_flush_reply_buffer(int dev)
{
	uint32_t reply_status = 0;
	int reply_value       = 0;
	int x;

	// Flush the reply buffer FIFO on the PCI DSP.
	// 6 is the number of 24 bit words the FIFO can hold.
	for ( x = 0; x < 6; x++ )
	{
		reply_status = Read_HSTR(dev);

		if ( (reply_status & OUTPUT_FIFO_OK_MASK) == OUTPUT_FIFO_OK_MASK )
			reply_value = Read_REPLY_BUFFER_32(dev);
		else
			break;
	}

	return reply_value;
}

/******************************************************************************
 FUNCTION: ASTROPCI_CHECK_REPLY_FLAGS
 
 PURPOSE:  Check the current PCI DSP status. Uses HSTR HTF bits 3,4,5.
 
 RETURNS:  Returns DON if HTF bits are a 1 and command successfully completed.
           Returns RDR if HTF bits are a 2 and a reply needs to be read.
           Returns ERR if HTF bits are a 3 and command failed.
           Returns SYR if HTF bits are a 4 and a system reset occurred.

 NOTES:    This function must be called after sending a command to the PCI
           board or controller.
******************************************************************************/
static int
astropci_check_reply_flags(int dev)
{
	uint32_t status = 0;
	int retval      = TIMEOUT;

	do {
		status = astropci_wait_for_condition(dev, CHECK_REPLY);

		if ( status == DONE_STATUS )
			retval = DON;

		else if ( status == READ_REPLY_STATUS )
			retval = RDR;

		else if ( status == ERROR_STATUS )
			retval = ERR;

		else if ( status == SYSTEM_RESET_STATUS )
			retval = SYR;

		else if ( status == READOUT_STATUS )
			retval = READOUT;

		// Clear the status bits if not in READOUT
		if ( retval != READOUT )
 			Write_HCVR( dev, (uint32_t)CLEAR_REPLY_FLAGS );

	} while (status == BUSY_STATUS);

	return retval;
}

/******************************************************************************
 FUNCTION: ASTROPCI_CHECK_DSP_INPUT_FIFO
 
 PURPOSE:  Check that the DSP input buffer (fifo) is not full.
 
 RETURNS:  Returns INPUT_FIFO_OK_MASK if HSTR bit 1 is set and buffer is
           available for input.

           Returns 0 if HSTR bit 1 is unset and buffer is unavailable 
           for input.

 NOTES:    This function must be called before writing to any register in the
           astropci_regs structure. Otherwise, data may be overwritten in the
           DSP input buffer, since the DSP cannot keep up with the input rate.
           This function will exit after "max_tries". This will help prevent
           the system from hanging in case the PCI DSP hangs.
******************************************************************************/
static int
astropci_check_dsp_input_fifo(int dev)
{
	return astropci_wait_for_condition(dev, INPUT_FIFO);
}

/******************************************************************************
 FUNCTION: ASTROPCI_CHECK_DSP_OUTPUT_FIFO
 
 PURPOSE:  Check that the DSP output buffer (fifo) has data.
 
 RETURNS:  Returns OUTPUT_FIFO_OK_MASK if HSTR bit 1 is set and buffer is
           available for output?
           
 NOTES:    Please insert documentation here... 
******************************************************************************/
static int
astropci_check_dsp_output_fifo(int dev)
{
	return astropci_wait_for_condition(dev, OUTPUT_FIFO);
}

/******************************************************************************
 FUNCTION: ASTROPCI_WAIT_FOR_CONDITION
 
 PURPOSE:  Waits for a particular state of the HSTR register. The
           condition can be INPUT_FIFO, OUTPUT_FIFO, or CHECK_REPLY.
 
 RETURNS:  0 on timeout, else a masked copy of the value of HSTR.
           
 NOTES:    The condition is first tested, and if satisfied, the routine
           returns immediately. Else, up to BUSY_MAX_WAIT microseconds
           are spent polling every BUSY_WAIT_DELAY microseconds. If
           the condition is still unsatisfied, up to SLEEP_MAX_WAIT
           microseconds total time will be spent, sleeping in intervals
           of SLEEP_WAIT_DELAY microseconds.

           So, this routine will busy wait for at most BUSY_MAX_WAIT 
           microseconds, and is guaranteed to return within SLEEP_MAX_WAIT
           microseconds (provided SLEEP_MAX_WAIT is greater than 
           BUSY_MAX_WAIT) plus or minus a jiffy.

           BUSY_WAIT_DELAY should be choosen so as not to overly
           tax the PCI card.

           BUSY_MAX_WAIT should be short enough not to cause unacceptable
           non-responsiveness of the computer, but long enough to cope
           with typical hardware delays.

           SLEEP_WAIT_DELAY should be at least a jiffy or three.

           SLEEP_MAX_WAIT should be the longest time before we clearly
           have a timeout problem.
******************************************************************************/
static uint32_t
astropci_wait_for_condition(int dev, int condition_to_wait_for)
{
	uint32_t status  = 0;
	long elapsedTime = 0L;
	long sleepDelay  = 0L;
	struct timeval initial, now;

	// Set the sleep delay (in jiffies)
	sleepDelay = (SLEEP_WAIT_DELAY * HZ) / 1000000L;
	if ( sleepDelay < 1 ) sleepDelay = 1;
 
	// Get the current time
	do_gettimeofday (&initial);

	// Loop for the specified number of usec
	while ( elapsedTime < SLEEP_MAX_WAIT )
	{
		status = Read_HSTR(dev);

		switch ( condition_to_wait_for )
		{
			case INPUT_FIFO:
                        status &= INPUT_FIFO_OK_MASK;
                        break;

			case OUTPUT_FIFO:
				status &= OUTPUT_FIFO_OK_MASK;
                        break;

			case CHECK_REPLY:
                        status = (status & HTF_BIT_MASK) >> 3;
                        break;
		}

		if ( status > 0 )
			break;

		// Get the current time and the elapsed time (usec)
		do_gettimeofday (&now);
		elapsedTime = (now.tv_sec  - initial.tv_sec) * 1000000L + (now.tv_usec - initial.tv_usec);

		if ( elapsedTime < BUSY_MAX_WAIT )
		{
			// Busy wait delay
			udelay(BUSY_WAIT_DELAY);
 		}
		else
		{
			// Sleep delay
			current->state = TASK_UNINTERRUPTIBLE;
			schedule_timeout(sleepDelay);
		}
	}

	return status;
}

/******************************************************************************
 FUNCTION: ASTROPCI_MMAP
        
         - Brian Taylor contributed to the original version of this function.

 PURPOSE:  Map the image buffer from unused RAM.
 
 RETURNS:  Returns 0 if memory was mapped, else a negative error code.
 
 NOTES:    For this function to work, the user must specify enough buffer
           space by passing "mem=xxxM" to the kernel as a boot parameter. 
           If you have 128M RAM and want a 28M image buffer, then use: 
           "mem=100M".

		From: "Eli Rykoff" <erykoff@umich.edu>:

		The bug is in the astropci_mmap() function, and applies to all
		versions of the astropci driver (RH9, FC2, FC4), when using at
		least 1GB of system ram.  It has to do with how the Linux kernel
		defines "low memory" and "high memory." (Which I didn't know
		before, but I sure do now). The Linux kernel can only directly
		address up to 915MB of memory (with usual stock compile options),
		and the rest of the system ram is the "high memory".  The problem
		comes in determining where the top of the system ram is.  In the
		current version of the driver, this is set as "__pa(high_memory)",
		which (as it turns out) is only equal to the top of the system ram
		if you have <915MB, and maxes out at 0x38000000.  If you have >= 1GB
		of ram then you can allocate this memory, but as soon as you try to
		use it you get problems. Unfortunately, I have found that the
		"right" way to do this is not very well documented (or documented
		at all, for that matter), but poking into mm.c and page.h in the
		Linux source reveals that the top of the memory is at the address
		num_physpages*PAGE_SIZE.  So, the fix is pretty simple:

		in astropci.c(FC4)/astropci_main.c(RH9), astropci_mmap():

            // Identify a suitable high memory area to map
            if ( nextValidStartAddress == 0 )
		{
			#ifdef CONFIG_HIGHMEM
                  nextValidStartAddress = (unsigned long) num_physpages * PAGE_SIZE;

			// this is the top of the Linux memory addresses
			#else
                  nextValidStartAddress = (unsigned long) __pa(high_memory);
			#endif
            }

		I tested this on both SL3 and FC4 (both systems with 2gb ram) and
		it works as it should.
******************************************************************************/
static int
astropci_mmap(struct file *file, struct vm_area_struct *vma)
{
	unsigned long prot = 0;
	int dev            = 0;
	int retval         = 0;

	#ifdef DEBUG_ON
	unsigned long *buffer;
	#endif

	spin_lock(&astropci_lock);

	dev = (int)file->private_data;

	// Sanity check, failure here should be impossible.
	if ( (dev < 0) || (dev >= MAX_DEV) || (!astropci[dev].opened) )
	{
		astropci_printf("Serious programming error\n");
		retval = -EAGAIN;
	}

	else
	{
		astropci_printf("Memory mapping for dev %d\n", dev);

            // Identify a suitable high memory area to map
            if ( nextValidStartAddress == 0 )
		{
			#ifdef CONFIG_HIGHMEM
                  nextValidStartAddress = (unsigned long) num_physpages * PAGE_SIZE;
			#else
			// this is the top of the Linux memory addresses
                  nextValidStartAddress = (unsigned long) __pa(high_memory);
			#endif
            }

		// Save the physical address
		astropci[dev].imageBufferPhysAddr = nextValidStartAddress;
		astropci[dev].imageBufferSize = (uint32_t)(vma->vm_end - vma->vm_start);
		nextValidStartAddress = astropci[dev].imageBufferPhysAddr + astropci[dev].imageBufferSize + PAGE_SIZE;

		astropci_printf("board %d  buffer start: 0x%X  end: 0x%X size: %u\n",
				    dev, astropci[dev].imageBufferPhysAddr, 
				    astropci[dev].imageBufferPhysAddr + astropci[dev].imageBufferSize,
				    astropci[dev].imageBufferSize);

		// Ensure that the memory will not be cached; see drivers/char/mem.c
		if ( boot_cpu_data.x86 > 3 )
		{
			prot = pgprot_val(vma->vm_page_prot) | _PAGE_PCD | _PAGE_PWT;
			vma->vm_page_prot = __pgprot(prot);
		}
 
 		// Don't try to swap out physical pages
		vma->vm_flags |= VM_RESERVED;

		// Don't dump addresses that are not real memory to a core file
		vma->vm_flags |= VM_IO;

		// Remap the page range to see the high memory
#if LINUX_VERSION_CODE >= KERNEL_VERSION(2,6,11)
		astropci_printf("Remapping page range for kernel >= 2.6.11\n");
		retval = remap_pfn_range(vma,
                                     vma->vm_start,
				      	 (astropci[dev].imageBufferPhysAddr >> PAGE_SHIFT),
                                     (vma->vm_end - vma->vm_start),
                                     vma->vm_page_prot);

		if ( retval != 0 )
			astropci_printf("Remap page range failed.\n");

#elif LINUX_VERSION_CODE >= KERNEL_VERSION(2,4,20)
		astropci_printf("Remapping page range for 2.6.11 > kernel >= 2.4.20 \n");
		retval = remap_page_range(vma,
                                      vma->vm_start,
				       	  astropci[dev].imageBufferPhysAddr,
                                      (vma->vm_end - vma->vm_start),
                                      vma->vm_page_prot);

		if ( retval != 0 )
                astropci_printf("Remap page range failed.\n");

#else
		astropci_printf("Remapping page range for some other kernel\n");
 		retval = remap_page_range(vma->vm_start, 
                                      astropci[dev].imageBufferPhysAddr, 
                                      astropci[dev].imageBufferSize, 
                                      vma->vm_page_prot);

		if ( retval != 0 )
                astropci_printf("Remap page range failed.\n");
 #endif

		if ( retval == 0 )
		{
        		astropci_printf("%u bytes mapped: 0x%lX - 0x%lX --> 0x%X - 0x%X\n",
					    astropci[dev].imageBufferSize, vma->vm_start, vma->vm_end,
					    astropci[dev].imageBufferPhysAddr, 
 					    astropci[dev].imageBufferPhysAddr + astropci[dev].imageBufferSize);

			// Save the virtual address, this seems to be what the driver
			// needs in order to access the image buffer.
			astropci[dev].imageBufferVirtAddr = (uint32_t)vma->vm_start;

	#ifdef DEBUG_ON
			// Write some test values to the image buffer.
			astropci_printf("Writing test values to image buffer\n");
			buffer = (unsigned long *)astropci[dev].imageBufferVirtAddr; //.imageBufferPhysAddr;
			if ( buffer != NULL )
			{
				buffer[0] = 0x11111111;
				buffer[1] = 0x22222222;
				buffer[2] = 0x33333333;
				buffer[3] = 0x44444444;
				astropci_printf("ASTROPCI buffer[0]: 0x%lX\n", buffer[0]);
				astropci_printf("ASTROPCI buffer[1]: 0x%lX\n", buffer[1]);
				astropci_printf("ASTROPCI buffer[2]: 0x%lX\n", buffer[2]);
				astropci_printf("ASTROPCI buffer[3]: 0x%lX\n", buffer[3]);
			}
			else
				astropci_printf("ASTROPCI ERROR, failed to access image buffer!\n");
	#endif

        		// Assign the image buffers
        		retval = astropci_set_buffer_addresses(dev);

			if ( retval != 0 )
				astropci_printf("Set DMA buffer addresses failed.\n");
		}
	}

	// If there was a failure, set imageBufferPhysAddr to zero as a flag
	if ( (retval != 0) && (dev != MAX_DEV) )
	{
		astropci[dev].imageBufferPhysAddr = 0;
		astropci[dev].imageBufferVirtAddr = 0;
	}

	spin_unlock(&astropci_lock);

	return retval;
}

/******************************************************************************
 FUNCTION: READREGISTER_32

 PURPOSE:  This function wraps the basic "readl" function, which has a different
	   version for kernels after 2.4.x.
 
 RETURNS:  Returns the 32-bit value read from "addr".
******************************************************************************/
static unsigned int ReadRegister_32(long addr)
{
	ndelay(REGISTER_ACCESS_DELAY);

#if LINUX_VERSION_CODE >= KERNEL_VERSION(2,6,9)
        return readl( (unsigned long *)(addr) );
#else
        return readl(addr);
#endif
}

/******************************************************************************
 FUNCTION: READREGISTER_16

 PURPOSE:  This function wraps the basic "readw" function, which has a different
	   version for kernels after 2.4.x.
 
 RETURNS:  Returns the 16-bit value read from "addr".
******************************************************************************/
static unsigned short ReadRegister_16(long addr)
{
	ndelay(REGISTER_ACCESS_DELAY);

#if LINUX_VERSION_CODE >= KERNEL_VERSION(2,6,9)
        return readw( (unsigned long *)(addr) );
#else
        return readw(addr);
#endif
}

/******************************************************************************
 FUNCTION: WRITEREGISTER_32

 PURPOSE:  This function wraps the basic "writel" function, which has a different
	   version for kernels after 2.4.x. Writes a 32-bit value to "addr".
 
 RETURNS:  Returns nothing.
******************************************************************************/
static void WriteRegister_32(unsigned int value, long addr)
{
	ndelay(REGISTER_ACCESS_DELAY);
//	udelay(1);

#if LINUX_VERSION_CODE >= KERNEL_VERSION(2,6,9)
        writel( value, (unsigned long *)(addr) );
#else
        writel( value, addr );
#endif
}

/******************************************************************************
 FUNCTION: WRITEREGISTER_16

 PURPOSE:  This function wraps the basic "writew" function, which has a different
	   version for kernels after 2.4.x. Writes a 16-bit value to "addr".
 
 RETURNS:  Returns nothing.
******************************************************************************/
static void WriteRegister_16(unsigned short value, long addr)
{
	ndelay(REGISTER_ACCESS_DELAY);
//	udelay(1);

#if LINUX_VERSION_CODE >= KERNEL_VERSION(2,6,9)
        writew( value, (unsigned long *)(addr) );
#else
        writew( value, addr );
#endif
}

/******************************************************************************
 FUNCTION: READ_HCTR

 PURPOSE:  Reads a 32-bit value from the HCTR. Calls ReadRegister_32.
 
 RETURNS:  Returns the value read.
******************************************************************************/
static unsigned int Read_HCTR(int dev)
{
	return ReadRegister_32(astropci[dev].ioaddr + HCTR);
}

/******************************************************************************
 FUNCTION: READ_HSTR

 PURPOSE:  Reads a 32-bit value from the HSTR. Calls ReadRegister_32.
 
 RETURNS:  Returns the value read.
******************************************************************************/
static unsigned int Read_HSTR(int dev)
{
	return ReadRegister_32(astropci[dev].ioaddr + HSTR);
}

/******************************************************************************
 FUNCTION: READ_REPLY_BUFFER_32

 PURPOSE:  Reads a 32-bit value from the REPLY_BUFFER. Calls ReadRegister_32.
 
 RETURNS:  Returns the value read.
******************************************************************************/
static unsigned int Read_REPLY_BUFFER_32(int dev)
{
	return ReadRegister_32(astropci[dev].ioaddr + REPLY_BUFFER);
}

/******************************************************************************
 FUNCTION: READ_REPLY_BUFFER_16

 PURPOSE:  Reads a 16-bit value from the REPLY_BUFFER. Calls ReadRegister_16.
 
 RETURNS:  Returns the value read.
******************************************************************************/
static unsigned short Read_REPLY_BUFFER_16(int dev)
{
	return ReadRegister_16(astropci[dev].ioaddr + REPLY_BUFFER);
}

/******************************************************************************
 FUNCTION: WRITE_HCTR

 PURPOSE:  Writes a 32-bit value to the HCTR. Calls WriteRegister_32.
 
 RETURNS:  None
******************************************************************************/
static void Write_HCTR(int dev, unsigned int regVal)
{
	WriteRegister_32(regVal, astropci[dev].ioaddr + HCTR);
}

/******************************************************************************
 FUNCTION: WRITE_CMD_DATA_32

 PURPOSE:  Writes a 32-bit value to the CMD_DATA register. Calls
	     WriteRegister_32.
 
 RETURNS:  None
******************************************************************************/
static void Write_CMD_DATA_32(int dev, unsigned int regVal)
{
	WriteRegister_32(regVal, astropci[dev].ioaddr + CMD_DATA);
}

/******************************************************************************
 FUNCTION: WRITE_CMD_DATA_16

 PURPOSE:  Writes a 16-bit value to the CMD_DATA register. Calls
	     WriteRegister_16.
 
 RETURNS:  None
******************************************************************************/
static void Write_CMD_DATA_16(int dev, unsigned short regVal)
{
	WriteRegister_16(regVal, astropci[dev].ioaddr + CMD_DATA);
}

/******************************************************************************
 FUNCTION: WRITE_HCVR

 PURPOSE:  Writes a 32-bit value to the HCVR. Checks that the HCVR register
	     bit 1 is not set, otherwise a command is still in the register.
	     Calls WriteRegister_32.
 
 RETURNS:  None
******************************************************************************/
static int Write_HCVR(int dev, unsigned int regVal)
{
	unsigned int currentHcvrValue = 0;
	int i, status = -EIO;

	for ( i=0; i<100; i++ )
	{
		currentHcvrValue = ReadRegister_32(astropci[dev].ioaddr + HCVR);

		if ( (currentHcvrValue & (unsigned int)0x1) == 0 )
		{
			status = 0;
			break;
		}
		astropci_printf("HCVR not ready. Count: %d  Value: 0x%X\n", i, currentHcvrValue);
	}

	if ( status == 0 )
		WriteRegister_32(regVal, astropci[dev].ioaddr + HCVR);

	return status;
}

#if 0
/******************************************************************************
 FUNCTION: ASTROPCI_READ_PROC

 PURPOSE:  This function is called when the user reads /proc/astropci.
 
 RETURNS:  Returns the number of bytes available to be read.
******************************************************************************/
static int
astropci_read_proc(char *buf, char **start, off_t offset, int len, int *eof, void *data)
{
        int dev = (int)data;

	/* 
	 * We give all of our information in one go, so if the
	 * user asks us if we have more information the answer
	 * should always be no.
	 *
	 * This is important because the standard read
	 * function from the library would continue to issue
	 * the read system call until the kernel replies
	 * that it has no more information, or until its
	 * buffer is filled.
	 */
        if (offset > 0) {
                return offset;
        }

        len = 0;
	len += sprintf(buf + len, "\n");
       	len += sprintf(buf + len, "Device Name: %s\n", board[dev]);
       	len += sprintf(buf + len, "Driver Version: %s\n", DRIVER_VERSION);
       	len += sprintf(buf + len, "Kernel Support: %s\n", SUPPORTED_KERNELS);

        if (astropci[dev].have_irq)
        	len += sprintf(buf + len, "IRQ: %d\n", astropci[dev].pdev->irq);

        if (astropci[dev].imageBufferPhysAddr) {
		len += sprintf(buf + len, "Start: 0x%X\n", astropci[dev].imageBufferPhysAddr); 
		len += sprintf(buf + len, "Size: %u\n", astropci[dev].imageBufferSize);
        }
	len += sprintf(buf + len, "\n");

	return len;
}
#endif

MODULE_AUTHOR("Marco Bonati, Scott Streit, Brian Taylor, Michael Ashley");
MODULE_DESCRIPTION("ARC PCI Interface Driver");
MODULE_SUPPORTED_DEVICE("astropci");

#if LINUX_VERSION_CODE >= KERNEL_VERSION(2,4,10)
MODULE_LICENSE("GPL");
#endif
