/*
 * YAFFS: Yet another FFS. A NAND-flash specific file system.
 * yaffscfg.c  The configuration for the "direct" use of yaffs.
 *
 * This file is intended to be modified to your requirements.
 * There is no need to redistribute this file.
 */

#include "yaffscfg.h"
#include "yaffsfs.h"
#include "yaffs_fileem2k.h"

#include <errno.h>

unsigned yaffs_traceMask = 0xFFFFFFFF;


void yaffsfs_SetError(int err)
{
	//Do whatever to set error
	errno = err;
}

void yaffsfs_Lock(void)
{
}

void yaffsfs_Unlock(void)
{
}

__u32 yaffsfs_CurrentTime(void)
{
	return 0;
}

void yaffsfs_LocalInitialisation(void)
{
	// Define locking semaphore.
}

// Configuration for:
// /ram  2MB ramdisk
// /boot 2MB boot disk (flash)
// /flash 14MB flash disk (flash)
// NB Though /boot and /flash occupy the same physical device they
// are still disticnt "yaffs_Devices. You may think of these as "partitions"
// using non-overlapping areas in the same device.
// 

#include "yaffs_ramdisk.h"
#include "yaffs_flashif.h"

static yaffs_Device ramDev;
static yaffs_Device bootDev;
static yaffs_Device flashDev;

static yaffsfs_DeviceConfiguration yaffsfs_config[] = {

	{ "/ram", &ramDev},
	{ "/boot", &bootDev},
	{ "/flash", &flashDev},
	{(void *)0,(void *)0}
};


int yaffs_StartUp(void)
{
	// Stuff to configure YAFFS
	// Stuff to initialise anything special (eg lock semaphore).
	yaffsfs_LocalInitialisation();
	
	// Set up devices
	// /ram
	memset(&ramDev,0,sizeof(ramDev));
	ramDev.nBytesPerChunk = 512;
	ramDev.nChunksPerBlock = 32;
	ramDev.nReservedBlocks = 2; // Set this smaller for RAM
	ramDev.startBlock = 1; // Can't use block 0
	ramDev.endBlock = 127; // Last block in 2MB.	
	//ramDev.useNANDECC = 1;
	ramDev.nShortOpCaches = 0;	// Disable caching on this device.
	ramDev.genericDevice = (void *) 0;	// Used to identify the device in fstat.
	ramDev.writeChunkWithTagsToNAND = yramdisk_WriteChunkWithTagsToNAND;
	ramDev.readChunkWithTagsFromNAND = yramdisk_ReadChunkWithTagsFromNAND;
	ramDev.eraseBlockInNAND = yramdisk_EraseBlockInNAND;
	ramDev.initialiseNAND = yramdisk_InitialiseNAND;

	// /boot
	memset(&bootDev,0,sizeof(bootDev));
	bootDev.nBytesPerChunk = 512;
	bootDev.nChunksPerBlock = 32;
	bootDev.nReservedBlocks = 5;
	bootDev.startBlock = 1; // Can't use block 0
	bootDev.endBlock = 63; // Last block
	//bootDev.useNANDECC = 0; // use YAFFS's ECC
	bootDev.nShortOpCaches = 10; // Use caches
	bootDev.genericDevice = (void *) 1;	// Used to identify the device in fstat.
	bootDev.writeChunkWithTagsToNAND = yflash_WriteChunkWithTagsToNAND;
	bootDev.readChunkWithTagsFromNAND = yflash_ReadChunkWithTagsFromNAND;
	bootDev.eraseBlockInNAND = yflash_EraseBlockInNAND;
	bootDev.initialiseNAND = yflash_InitialiseNAND;
	bootDev.markNANDBlockBad = yflash_MarkNANDBlockBad;
	bootDev.queryNANDBlock = yflash_QueryNANDBlock;



	// /flash
	// Set this puppy up to use
	// the file emulation space as
	// 2kpage/64chunk per block/128MB device
	memset(&flashDev,0,sizeof(flashDev));

	flashDev.nBytesPerChunk = 2048;
	flashDev.nChunksPerBlock = 64;
	flashDev.nReservedBlocks = 5;
	flashDev.startBlock = 64; // First block after /boot
	//flashDev.endBlock = 127; // Last block in 16MB
	flashDev.endBlock = (512 * 1024 * 1024)/(flashDev.nBytesPerChunk * flashDev.nChunksPerBlock) - 1; // Last block in 512MB
	flashDev.isYaffs2 = 1;
	flashDev.nShortOpCaches = 10; // Use caches
	flashDev.genericDevice = (void *) 2;	// Used to identify the device in fstat.
	flashDev.writeChunkWithTagsToNAND = yflash_WriteChunkWithTagsToNAND;
	flashDev.readChunkWithTagsFromNAND = yflash_ReadChunkWithTagsFromNAND;
	flashDev.eraseBlockInNAND = yflash_EraseBlockInNAND;
	flashDev.initialiseNAND = yflash_InitialiseNAND;
	flashDev.markNANDBlockBad = yflash_MarkNANDBlockBad;
	flashDev.queryNANDBlock = yflash_QueryNANDBlock;

	yaffs_initialise(yaffsfs_config);
	
	return 0;
}




