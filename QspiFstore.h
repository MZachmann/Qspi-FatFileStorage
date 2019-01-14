#ifndef QSPI_FSTORE_H
#define QSPI_FSTORE_H

// most of the fstorage setup needs to be built in C so this contains
// those code chunks as exportable stuff. These set up the FATFS object.
// Copyright 2019 Mark Zachmann

#ifdef __cplusplus
extern "C" {
#endif

	// init the qspi disk subsystem
	FRESULT	QspiF_Initialize(void);
	// uninitialize the subsystem
	void	QspiF_Uninitialize(void);

#ifdef __cplusplus
}
#endif

#endif