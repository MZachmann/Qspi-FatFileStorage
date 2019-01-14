#ifndef QSPI_FSTORAGE_H
#define QSPI_FSTORAGE_H

// Copyright 2019 Mark Zachmann


#ifndef FATFS
#include <ff.h>
#endif

class String;

// this works well as a statically allocated object
class QspiFstorage
{
	public:
        static  QspiFstorage Store;
		QspiFstorage();
		virtual ~QspiFstorage();
		int Initialize(void);
		int Mount(void);
		void Ls(const char* folder);
		int WriteFile(const char* filename, const String& stext);
		int WriteFile(const char* filename, const uint8_t* text, uint16_t count);
		int ReadFile(const char* filename, uint8_t* buffer, uint16_t bufmax);
		int ReadFile(const char* filename, String* pstext);
		void Uninitialize(void);
	private:
		FATFS _Filesystem;
		bool _IsInitialized;
		bool _IsMounted;
};




#endif