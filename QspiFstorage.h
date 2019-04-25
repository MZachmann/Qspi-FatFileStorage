#ifndef QSPI_FSTORAGE_H
#define QSPI_FSTORAGE_H
// Copyright 2019 Mark Zachmann

#ifndef FATFS
#include <ff.h>
#endif
// provide String from somewhere
class String;

// This works well as a statically allocated object
// Create one of these and use it for file i/o
class QspiFstorage
{
	public:
        static  QspiFstorage Store;
		QspiFstorage();
		virtual ~QspiFstorage();
		int Initialize(void);
		int Mount(bool forceClear);	// mount or create the filesystem, maybe clearing it
		void Ls(const char* folder);
		int WriteFile(const char* filename, const String& stext);
		int WriteFileN(const char* filename, const uint8_t* text, uint16_t count);
		int ReadFile(const char* filename, String* pstext);
		int ReadFileN(const char* filename, uint8_t* buffer, uint16_t bufmax);
		void Uninitialize(void);
	private:
		FATFS _Filesystem;
		bool _IsInitialized;
		bool _IsMounted;
};




#endif