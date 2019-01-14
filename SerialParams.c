// the nrf5 sdk 15. doesn't support the GD25Q32C so I watched the ID
// and empirically set the new id (?) since I couldn't find it in any doc
// This understates available flash since my requirements are tiny and keeping some
// free of the filesystem seems prudent
// this has no native include file since it's a link thing
// This is Copyright Nordic Semiconductor... (see nrf_serial_flash.c)

#include "nrf_serial_flash_params.h"

static const nrf_serial_flash_params_t m_sflash_params[] = {
    {    /*MXIC MX25R6435F*/
        .read_id = { 0xC2, 0x28, 0x17 },
        .capabilities = 0x00,
        .size = 8 * 1024 * 1024,
        .erase_size = 4 * 1024,
        .program_size = 256,
    },
    {    /*IC GD25Q32C*/
        .read_id = { 0xc2, 0x20, 0x16 },
        .capabilities = 0x00,
        .size = 8 * 1024 * 1024, // only use half of it
        .erase_size = 4 * 1024,
        .program_size = 256,
    }
};

// if you don't include the actual nrf_serial_flash c file then this will be used
// as a way to identify the flash chip for the block driver
nrf_serial_flash_params_t const * nrf_serial_flash_params_get(const uint8_t * p_read_id)
{
    size_t i;
    for (i = 0; i < ARRAY_SIZE(m_sflash_params); ++i)
    {
		// does our read ID match one above?
        if (memcmp(m_sflash_params[i].read_id, p_read_id, sizeof(m_sflash_params[i].read_id)) == 0)
        {
            return &m_sflash_params[i];
        }
    }
	// no matching chip
    return NULL;
}
