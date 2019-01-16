// fstest.cc 
//	Simple test routines for the file system.  
//
//	We implement:
//	   Copy -- copy a file from UNIX to Nachos
//	   Print -- cat the contents of a Nachos file 
//	   Perftest -- a stress test for the Nachos file system
//		read and write a really large file in tiny chunks
//		(won't work on baseline system!)
//
// Copyright (c) 1992-1993 The Regents of the University of California.
// All rights reserved.  See copyright.h for copyright notice and limitation 
// of liability and disclaimer of warranty provisions.

#include "copyright.h"

#include "utility.h"
#include "filesys.h"
#include "system.h"
#include "thread.h"
#include "disk.h"
#include "stats.h"

#define TransferSize 	10 	// make it small, just to be difficult

//----------------------------------------------------------------------
// Copy
// 	Copy the contents of the UNIX file "from" to the Nachos file "to"
//----------------------------------------------------------------------

void
Copy(const char *from, const char *to)
{
    FILE *fp;
    OpenFile* openFile;
    int amountRead, fileLength;
    char *buffer;

// Open UNIX file
    if ((fp = fopen(from, "r")) == NULL) {	 
	printf("Copy: couldn't open input file %s\n", from);
	return;
    }

// Figure out length of UNIX file
    fseek(fp, 0, 2);		
    fileLength = ftell(fp);
    fseek(fp, 0, 0);

// Create a Nachos file of the same length
    DEBUG('f', "Copying file %s, size %d, to file %s\n", from, fileLength, to);
    if (!fileSystem->Create(to, fileLength)) {	 // Create Nachos file
	printf("Copy: couldn't create output file %s\n", to);
	fclose(fp);
	return;
    }
    
    openFile = fileSystem->Open(to);
    ASSERT(openFile != NULL);
    
// Copy the data in TransferSize chunks
    buffer = new char[TransferSize];
    while ((amountRead = fread(buffer, sizeof(char), TransferSize, fp)) > 0)
	openFile->Write(buffer, amountRead);	
    delete [] buffer;

// Close the UNIX and the Nachos files
    delete openFile;
    fclose(fp);
}

//----------------------------------------------------------------------
// Print
// 	Print the contents of the Nachos file "name".
//----------------------------------------------------------------------

void
Print(char *name)
{
    OpenFile *openFile;    
    int i, amountRead;
    char *buffer;

    if ((openFile = fileSystem->Open(name)) == NULL) {
	printf("Print: unable to open file %s\n", name);
	return;
    }
    
    buffer = new char[TransferSize];
    while ((amountRead = openFile->Read(buffer, TransferSize)) > 0)
	for (i = 0; i < amountRead; i++)
	    printf("%c", buffer[i]);
    delete [] buffer;

    delete openFile;		// close the Nachos file
    return;
}

//----------------------------------------------------------------------
// PerformanceTest
// 	Stress the Nachos file system by creating a large file, writing
//	it out a bit at a time, reading it back a bit at a time, and then
//	deleting the file.
//
//	Implemented as three separate routines:
//	  FileWrite -- write the file
//	  FileRead -- read the file
//	  PerformanceTest -- overall control, and print out performance #'s
//----------------------------------------------------------------------

#define FileName 	"TestFile"
#define Contents 	"1234567890"
#define ContentSize 	strlen(Contents)
#define FileSize 	((int)(ContentSize * 375))

static void 
FileWrite()
{
    OpenFile *openFile;    
    int i, numBytes;

    printf("Sequential write of %d byte file, in %zd byte chunks\n", 
	FileSize, ContentSize);
    if (!fileSystem->Create(FileName,FileSize)) {
      printf("Perf test: can't create %s\n", FileName);
      return;
    }

    if (!fileSystem->CreateRepository("dirTest")) {
        printf("ERREUR \n");

    } else {
        printf("OK \n");
    }
    openFile = fileSystem->Open(FileName);
    if (openFile == NULL) {
	printf("Perf test: unable to open %s\n", FileName);
	return;
    }
    printf("File open, start writing\n");
    for (i = 0; i < FileSize; i += ContentSize) {
        numBytes = openFile->Write(Contents, ContentSize);
        //if (numBytes == 0) break;
	    if (numBytes < 10) {
	        printf("%d Perf test: unable to write %s\n",i,FileName);
	        delete openFile;
	        return;
	    }
    }

    printf("CA MARCHE !! \n\n");
    delete openFile;	// close file
}

static void 
FileRead()
{
    OpenFile *openFile;    
    char *buffer = new char[ContentSize];
    int i, numBytes;

    printf("Sequential read of %d byte file, in %zd byte chunks\n", 
	FileSize, ContentSize);

    if ((openFile = fileSystem->Open(FileName)) == NULL) {
	printf("Perf test: unable to open file %s\n", FileName);
	delete [] buffer;
	return;
    }
    for (i = 0; i < FileSize; i += ContentSize) {
        numBytes = openFile->Read(buffer, ContentSize);
	if ((numBytes < 10) || strncmp(buffer, Contents, ContentSize)) {
	    printf("Perf test: unable to read %s\n", FileName);
	    delete openFile;
	    delete [] buffer;
	    return;
	}
    }
    printf("CA MARCHE !! \n\n");
    delete [] buffer;
    delete openFile;	// close file
}

void testCD() {
    printf("test change directory \n\n");
    if (!fileSystem->CreateRepository("test1")) printf("ERREUR 1 \n");
    //fileSystem->printRepository();
    if (!fileSystem->changeRepository("test1")) printf("ERREUR 2 \n");

    if (!fileSystem->CreateRepository("test12")) printf("ERREUR 3 \n");
    //fileSystem->printRepository();

    if (!fileSystem->changeRepository("test12")) printf("ERREUR A \n");
    if (!fileSystem->CreateRepository("test2")) printf("ERREUR B \n");
    if (!fileSystem->changeRepository("test2")) printf("ERREUR 2 \n");
    if (!fileSystem->CreateRepository("test3")) printf("ERREUR C \n");
    if (!fileSystem->changeRepository("test3")) printf("ERREUR D \n");
    
    if (!fileSystem->changeRepository("..")) printf("ERREUR L \n");

    if (!fileSystem->Create("file", 5)) printf("ERREUR FILE CREATION \n");
    if (!fileSystem->Remove("file")) printf("ERREUR FILE REMOVE \n");
    //fileSystem->printRepository();
    if (!fileSystem->Remove("test3")) printf("ERREUR FOLDER REMOVE \n");
    //fileSystem->printRepository();

    if (!fileSystem->changeRepository("..")) printf("ERREUR M \n");
    if (!fileSystem->changeRepository("..")) printf("ERREUR N \n");

    if (!fileSystem->Remove("test12")) printf("C'est Normal FOLDER REMOVE \n");
    //fileSystem->printRepository();

    //fileSystem->printRepository();
}

void
PerformanceTest()
{
    printf("Starting file system performance test:\n");
    stats->Print();
    FileWrite();
    FileRead();
    //fileSystem->printRepository();
    testCD();

    if (!fileSystem->Remove(FileName)) {
      printf("Perf test: unable to remove %s\n", FileName);
      return;
    }
    stats->Print();
}