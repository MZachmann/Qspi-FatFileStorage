// File storage with the qspi stuff using the nrf_block sdk methods
// This is extremely slim. Once the device is initialized it can be accessed using the FatFs methods
// which is a pretty complete file i/o library. See Mount and Read/WriteFile for examples.
// Supply your own ASeries serial device instead of SerialWrap
//
// Building also requires a section of sdk_config.h to define the relevant qspi pins and characteristics. 
// See the Nordic SDK for examples of this in their qspi examples.
// Copyright 2019 Mark Zachmann

#include <stdint.h>
#include <ff.h>
#include "QspiFstorage.h"
#include "QspiFstore.h"
// the serial ASeries object definition
#include "SerialWrap.h"

QspiFstorage QspiFstorage::Store;

QspiFstorage::QspiFstorage() : _IsInitialized(false), _IsMounted(false)
{
}

QspiFstorage::~QspiFstorage()
{

}

// initialize the qspi module and then mount it as a filesystem
int QspiFstorage::Initialize(void)
{
	if(!_IsInitialized)
	{
		int frf = (int)QspiF_Initialize( );
		if(frf == FR_OK)
		{
			_IsInitialized = true;
			frf = Mount( );
		}
		return frf;
	}

	return 0;
}

// mount the flash chip as a filesystem. on failure create a new filesystem
int QspiFstorage::Mount(void)
{
    static uint8_t buf[512];
    memset(&_Filesystem, 0, sizeof(FATFS));
	FRESULT ff_result = f_mount(&_Filesystem, "", 1);
	if(ff_result == FR_NO_FILESYSTEM)
	{
		ff_result = f_mkfs("", FM_FAT, 1024, buf, sizeof(buf));
		if (ff_result != FR_OK)
		{
			return ff_result;
		}
		ff_result = f_mount(&_Filesystem, "", 1);
	}

	_IsMounted = (ff_result == 0);
    return ff_result;
}

// uninitialize (?)
void QspiFstorage::Uninitialize(void)
{
	if( _IsMounted)
		f_mount(NULL, "", 1);	// mounting NULL clears the mount
	QspiF_Uninitialize();
}

// list a folder in the filesystem
void QspiFstorage::Ls(const char* folder)
{
    DIR dir;
    FRESULT ff_result;
    FILINFO fno;

    ff_result = f_opendir(&dir, folder);
    if (ff_result != FR_OK)
    {
        ASeries.printf("Directory listing failed: %u", ff_result);
        return;
    }

    ASeries.printf("ls: List folder %s", folder);
    uint32_t entries_count = 0;
    do
    {
        ff_result = f_readdir(&dir, &fno);
        if (ff_result != FR_OK)
        {
            ASeries.printf("Directory read failed: %u", ff_result);
            return;
        }

        if (fno.fname[0])
        {
            if (fno.fattrib & AM_DIR)
            {
                ASeries.printf("ls: <DIR> %s",(uint32_t)fno.fname);
            }
            else
            {
                ASeries.printf("ls: %9lu %s", fno.fsize, (uint32_t)fno.fname);
            }
        }

        ++entries_count;
    } while (fno.fname[0]);
}

// write a file from a string
int QspiFstorage::WriteFile(const char* filename, const String& stext)
{
  return WriteFile(filename, (const uint8_t* )(stext.c_str()), stext.length());
}

// write a file from a buffer
int QspiFstorage::WriteFile(const char* filename, const uint8_t* text, uint16_t count)
{
FIL file;
size_t nout = 0;

	FRESULT frf = f_open(&file, filename, FA_WRITE | FA_CREATE_ALWAYS);
	if(frf == FR_OK)
	{
		frf = f_write(&file, text, count, &nout);
		FRESULT frc = f_close(&file);
		if(frf == FR_OK)
			frf = frc;	// if the write worked, get the close result
		if(frf != FR_OK)
		{
			ASeries.printf("File write error: %d", frf);
		}
	}
	return (int)frf;
}

int QspiFstorage::ReadFile(const char* filename, String* pstext)
{
FIL file;
size_t nout = 0;
FILINFO finf;

	FRESULT frf = f_stat(filename, &finf);
	if( frf == FR_OK && finf.fsize > 0)
	{
		uint16_t fileamt = finf.fsize + 5;
		char* bfr = (char*)malloc(fileamt);
		if(bfr)
		{
			memset(bfr, 0, fileamt);
			ReadFile(filename, (uint8_t*)bfr, finf.fsize);	// ignore result?
			*pstext = bfr;
			free(bfr);
		}
	}
	else
	{
		*pstext = "";
	}
	return frf;
}

// read a file into a buffer, return # bytes read
int QspiFstorage::ReadFile(const char* filename, uint8_t* buffer, uint16_t bufmax)
{
FIL file;
size_t nout = 0;

	FRESULT frf = f_open(&file, filename, FA_READ);
	if(frf == FR_OK)
	{
		frf = f_read(&file, buffer, bufmax, &nout);
		FRESULT frc = f_close(&file);
		if(frf == FR_OK)
			frf = frc;	// if the read worked, get the close result
		if(frf != FR_OK)
		{
			ASeries.printf("File read error: %d", frf);
		}
	}

	return nout;	// here we return # bytes read
}
