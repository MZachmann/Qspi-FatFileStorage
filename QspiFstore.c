// Low-level setup code for the qspi fstorage library
// This has to be in C so,...
// This is slim and only supports qspi
// Copyright 2019 Mark Zachmann

#include <stdint.h>
// this file defines pins for the qspi chip
#include "../particle_xenon/variant.h"
#include "nrf_block_dev.h"
#include "nrf_block_dev_qspi.h"
#include "diskio_blkdev.h"

//  QSPI block device definition
NRF_BLOCK_DEV_QSPI_DEFINE( 
      m_block_dev_qspi, 
      NRF_BLOCK_DEV_QSPI_CONFIG(512, NRF_BLOCK_DEV_QSPI_FLAG_CACHE_WRITEBACK, NRF_DRV_QSPI_DEFAULT_CONFIG),
     NFR_BLOCK_DEV_INFO_CONFIG("Friend", "QSPI", "0.10")
);

// block device list
#define BLOCKDEV_LIST() (                                   \
    NRF_BLOCKDEV_BASE_ADDR(m_block_dev_qspi, block_dev)     \
)

// Initialize 
int QspiF_Initialize(void)
{
    // Initialize FATFS disk I/O interface by providing the block device.
    static diskio_blkdev_t drives[] =
    {
        DISKIO_BLOCKDEV_CONFIG(NRF_BLOCKDEV_BASE_ADDR(m_block_dev_qspi, block_dev), NULL)
    };
    diskio_blockdev_register(drives, ARRAY_SIZE(drives));
    return (int)disk_initialize(0);
}

void QspiF_Uninitialize(void)
{
    UNUSED_RETURN_VALUE(disk_uninitialize(0));
}

