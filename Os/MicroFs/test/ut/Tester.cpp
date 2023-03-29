
#include "Tester.hpp"
#include <stdio.h>
#include <Fw/Test/UnitTest.hpp>
#include <Fw/Types/MallocAllocator.hpp>
#include <Fw/Types/Assert.hpp>



namespace Os {

  // ----------------------------------------------------------------------
  // Construction and destruction
  // ----------------------------------------------------------------------

  Tester ::
    Tester()
  {
  }

  Tester ::
    ~Tester()
  {
  }

  Tester :: FileModel ::
      FileModel() : 
        mode(DOESNT_EXIST),
        size(0)
  {
  }

  void Tester :: FileModel ::
      clear()
  {
    this->curPtr = 0;
    this->size = 0;
    memset(this->buffOut, 0xA5, FILE_SIZE);
  }


  // ----------------------------------------------------------------------
  // Tests
  // ----------------------------------------------------------------------

  // ----------------------------------------------------------------------
  // DirectoryTest
  // ----------------------------------------------------------------------
  void Tester ::
    DirectoryTest()
  {
    const U16 NumberBins = MAX_BINS;
    const U16 NumberFiles = MAX_FILES_PER_BIN;

    clearFileBuffer();

    // Instantiate the Rules
    InitFileSystem initFileSystem(NumberBins, FILE_SIZE, NumberFiles);
    Cleanup cleanup;

    Directory* directories[MAX_BINS];
    Directory offNominalDir1("/bin10", true);
    Directory offNominalDir2("/bit0", true);
    Directory offNominalDir3("/bin", true);
    Directory offNominalDir4("bin0", true);
    char dirName[MAX_BINS][20];

    U16 binIndex =  0;
    for (U16 bin=0; bin < MAX_BINS; bin++)
    {
        snprintf(dirName[bin], 20, "/bin%d", bin);
    }
    
    // Run the Rules
    initFileSystem.apply(*this);

    for (U16 i = 0; i < MAX_BINS; i++)
    {
      directories[i] = new Directory(dirName[i], false);
      directories[i]->apply(*this);
    }

    offNominalDir1.apply(*this);
    offNominalDir2.apply(*this);
    offNominalDir3.apply(*this);
    offNominalDir4.apply(*this);

    cleanup.apply(*this);
  }

  // ----------------------------------------------------------------------
  // MoveTest
  // ----------------------------------------------------------------------
  void Tester ::
    MoveTest()
  {
    const U16 NumberBins = 1;
    const U16 NumberFiles = 2;

    const char* File1 = "/bin0/file0";
    const char* File2 = "/bin0/file1";
    const char* CrcFile = "/bin0/file0.crc32";

    clearFileBuffer();

    // Instantiate the Rules
    InitFileSystem initFileSystem(NumberBins, FILE_SIZE, NumberFiles);
    OpenRead openRead(File1);
    OpenFile openFile(File1);
    OpenFile openFile2(File2);
    WriteData writeDataSmallChunk(File1, FILE_SIZE/4, 0xFF);
    WriteData writeDataSmallChunk2(File2, FILE_SIZE/4, 0xFF);
    CheckFileSize checkFileSize(File1);
    CheckFileSize checkFileSize2(File2);
    CloseFile closeFile(File1);
    CloseFile closeFile2(File2);
    Cleanup cleanup;
    MoveFile moveFile(File1, File2);
    

    // Run the Rules
    initFileSystem.apply(*this);
    openFile.apply(*this);
    writeDataSmallChunk.apply(*this);
    checkFileSize.apply(*this);
    openFile2.apply(*this);
    checkFileSize2.apply(*this);
    closeFile.apply(*this);
    closeFile2.apply(*this);
    moveFile.apply(*this);
    checkFileSize.apply(*this);
    checkFileSize2.apply(*this);

    cleanup.apply(*this);
  }
  // ----------------------------------------------------------------------
  // OddTests
  // ----------------------------------------------------------------------
  void Tester ::
    OddTests()
  {
    const U16 NumberBins = 1;
    const U16 NumberFiles = 2;

    const char* File1 = "/bin0/file0";
    const char* File2 = "/bin0/file1";
    const char* CrcFile = "/bin0/file0.crc32";

    clearFileBuffer();

    // Instantiate the Rules
    InitFileSystem initFileSystem(NumberBins, FILE_SIZE, NumberFiles);
    OpenRead openRead(File1);
    OpenFile openFile(File1);
    OpenReadEarly openReadEarly(File1);
    OpenCreate openCreate(File1);
    WriteData writeDataSmallChunk(File1, FILE_SIZE/4, 0xFF);
    WriteData writeDataSmallChunk2(File2, FILE_SIZE/4, 0xFF);
    CheckFileSize checkFileSize(File1);
    CloseFile closeFile(File1);
    CloseFile closeFile2(File2);
    OpenAppend openAppend(File1);
    RemoveFile removeFile(File1);
    RemoveBusyFile removeBusyFile(File1);
    IsFileOpen isFileOpen(File1);
    OpenFile openFile2(File2);
    MoveFile moveFile(File1, File2);
    CheckFileSize checkFileSize2(File2);
    

    Cleanup cleanup;

    // Run the Rules
    initFileSystem.apply(*this);
    openReadEarly.apply(*this);
    openFile.apply(*this);
    isFileOpen.apply(*this);
    writeDataSmallChunk.apply(*this);
    removeBusyFile.apply(*this);
    closeFile.apply(*this);
    openAppend.apply(*this);
    writeDataSmallChunk.apply(*this);
    checkFileSize.apply(*this);
    closeFile.apply(*this);
    removeFile.apply(*this);

    cleanup.apply(*this);
  }


  // ----------------------------------------------------------------------
  // NukeTest
  // ----------------------------------------------------------------------
  void Tester ::
    NukeTest()
  {
    const U16 NumberBins = 1;
    const U16 NumberFiles = 2;

    const char* File1 = "/bin0/file0";
    const char* File2 = "/bin0/file1";
    const char* CrcFile = "/bin0/file0.crc32";

    clearFileBuffer();

    // Instantiate the Rules
    InitFileSystem initFileSystem(NumberBins, FILE_SIZE, NumberFiles);
    OpenFile openFile(File1);
    OpenFileNotExist openFileNotExist(CrcFile);
    CloseFile closeFile(File1);
    WriteData writeDataSmallChunk(File1, FILE_SIZE/4, 0xFF);
    WriteData writeDataMediumChunk(File1, FILE_SIZE/2, 0xFF);
    WriteData writeDataLargeChunk(File1, 3*FILE_SIZE/4, 0xFF);
    WriteData writeDataFullChunk(File1, FILE_SIZE, 0xFF);
    ReadData readDataSmallChunk(File1, FILE_SIZE/4);
    ReadData readDataMediumChunk(File1, FILE_SIZE/2);
    ReadData readDataLargeChunk(File1, 3*FILE_SIZE/4);
    ReadData readDataFullChunk(File1, FILE_SIZE);

    CheckFileSize checkFileSize(File1);

    OpenRead openRead(File1);
    ReadData readData(File1, FILE_SIZE/2);
    ReadData readData1(File1, FILE_SIZE);
    Cleanup cleanup;
    ResetFile resetFile(File1);

    OpenCreate openCreate(File1);
    OpenAppend openAppend(File1);
    RemoveFile removeFile(File1);
    RemoveBusyFile removeBusyFile(File1);
    IsFileOpen isFileOpen(File1);


    // Run the Rules
    initFileSystem.apply(*this);

    // Run the Rules randomly
    STest::Rule<Tester>* rules[] = { 
                                     &openFile,
                                     &isFileOpen,
                                     &openCreate,
                                     &openAppend,
                                     &removeFile,
                                     &removeBusyFile,
                                     &openFileNotExist,
                                     &closeFile,
                                     &checkFileSize,
                                     &writeDataSmallChunk,
                                     &writeDataMediumChunk, 
                                     &writeDataLargeChunk,
                                     &writeDataFullChunk,
                                     &readDataSmallChunk,
                                     &readDataMediumChunk, 
                                     &readDataLargeChunk,
                                     &readDataFullChunk,
                                     &resetFile
                                  };
    STest::RandomScenario<Tester> randomScenario(
        "RandomScenario",
        rules,
        sizeof(rules) / sizeof(STest::Rule<Tester>*)
    );
    STest::BoundedScenario<Tester> boundedScenario(
        "BoundedScenario",
        randomScenario,
        2000
    );
    const U32 numSteps = boundedScenario.run(*this);
    ASSERT_EQ(2000, numSteps);

    cleanup.apply(*this);
  }

  // ----------------------------------------------------------------------
  // FileSizeTest
  // ----------------------------------------------------------------------
  void Tester ::
    FileSizeTest()
  {
    const U16 NumberBins = 1;
    const U16 NumberFiles = 2;

    const char* File1 = "/bin0/file0";

    clearFileBuffer();

    // Instantiate the Rules
    InitFileSystem initFileSystem(NumberBins, FILE_SIZE, NumberFiles);
    OpenFile openFile1(File1);
    CloseFile closeFile(File1);
    WriteData writeData(File1, FILE_SIZE, 0xFF);
    CheckFileSize checkFileSize(File1);
    OpenRead openRead(File1);
    ReadData readData(File1, FILE_SIZE/2);
    Cleanup cleanup;

    // Run the Rules
    initFileSystem.apply(*this);
    openFile1.apply(*this);
    writeData.apply(*this);
    checkFileSize.apply(*this);
    closeFile.apply(*this);
    checkFileSize.apply(*this);
    openRead.apply(*this);
    checkFileSize.apply(*this);
    closeFile.apply(*this);
    checkFileSize.apply(*this);
    openRead.apply(*this);
    checkFileSize.apply(*this);
    closeFile.apply(*this);
    checkFileSize.apply(*this);
    cleanup.apply(*this);
  }


  // ----------------------------------------------------------------------
  // ReWriteTest
  // ----------------------------------------------------------------------
  void Tester ::
    ReWriteTest()
  {
    const U16 NumberBins = 1;
    const U16 NumberFiles = 2;

    const char* File1 = "/bin0/file0";

    const U16 TotalFiles = NumberBins * NumberFiles;
    clearFileBuffer();

    // Instantiate the Rules
    InitFileSystem initFileSystem(NumberBins, FILE_SIZE, NumberFiles);
    OpenFile openFile1(File1);
    CloseFile closeFile1(File1);
    Listings listings(NumberBins, NumberFiles);
    WriteData writeData1(File1, FILE_SIZE, 0xFF);
    WriteData writeDataHalf(File1, FILE_SIZE/2, 0xFF);
    WriteData writeDataQuater(File1, FILE_SIZE/4, 0xFF);
    ReadData readData(File1, FILE_SIZE);
    OpenRead openRead1(File1);
    ReadData readData1(File1, FILE_SIZE/2);
    ResetFile resetFile1(File1);
    CheckFileSize checkFileSize(File1);
    CheckFileSize checkFileSizeZero(File1);
    CheckFileSize checkFileSizeHalf(File1);
    CheckFileSize checkFileSizeQuater(File1);
    CheckFileSize checkFileSizeThreeQuaters(File1);

    Cleanup cleanup;

    // Run the Rules
    
    // Initialize
    initFileSystem.apply(*this);

    // Part 1:  Open a new file and write max bytes
    printf("Part 1\n");
    openFile1.apply(*this);
    writeData1.apply(*this);
    checkFileSize.apply(*this);
    closeFile1.apply(*this);

    // Part 2: Open the file again and write max bytes,
    // check that the size does not exceed tha max
    printf("Part 2\n");
    openFile1.apply(*this);
    writeData1.apply(*this);
    checkFileSize.apply(*this);
    closeFile1.apply(*this);

    // Part 3: Open the file again, write half the bytes,
    // check that the size still equals the max, write the 
    // other half, check that the size still equals the max.
    printf("Part 3\n");
    openFile1.apply(*this);
    writeDataHalf.apply(*this);
    checkFileSize.apply(*this);
    writeDataHalf.apply(*this);
    checkFileSize.apply(*this);
    closeFile1.apply(*this);

    // Part 4: Cleanup and reinitialize
    // Open a new file, check the size is 0, write half and check half
    printf("Part 4\n");
    cleanup.apply(*this);
    initFileSystem.apply(*this);
    openFile1.apply(*this);
    checkFileSizeZero.apply(*this);
    writeDataHalf.apply(*this);
    checkFileSizeHalf.apply(*this);
    closeFile1.apply(*this);

    // Part 5:  Open the file again.  Check size is 1/2
    // Write a 1/4 and check file is still 1/2
    // Write a 1/4 and check file is still 1/2
    // Write a 1/4 and check file is 3/4
    // Write a 1/4 again and check file is full
    printf("Part 5\n");
    openFile1.apply(*this);
    checkFileSizeHalf.apply(*this);
    writeDataQuater.apply(*this);
    checkFileSizeHalf.apply(*this);
    writeDataQuater.apply(*this);
    checkFileSizeHalf.apply(*this);
    writeDataQuater.apply(*this);
    checkFileSizeThreeQuaters.apply(*this);
    writeDataQuater.apply(*this);
    checkFileSize.apply(*this);
    closeFile1.apply(*this);

    // Part


    cleanup.apply(*this);
  }

  // ----------------------------------------------------------------------
  // OpenStressTest
  // ----------------------------------------------------------------------
  void Tester ::
    OpenStressTest()
  {
    const U16 NumberBins = MAX_BINS;
    const U16 NumberFiles = MAX_FILES_PER_BIN;

    const U16 TotalFiles = NumberBins * NumberFiles;
    clearFileBuffer();

    // Instantiate the Rules
    OpenFile* openFile[TotalFiles];
    CloseFile* closeFile[TotalFiles];
    InitFileSystem initFileSystem(NumberBins, FILE_SIZE, NumberFiles);
    Cleanup cleanup;

    char fileName[TotalFiles][20];

    getFileNames(fileName, MAX_BINS, MAX_FILES_PER_BIN);

    // Run the Rules
    initFileSystem.apply(*this);

    for (U16 i = 0; i < TotalFiles; i++)
    {
      openFile[i] = new OpenFile(fileName[i]);
      openFile[i]->apply(*this);
    }

    for (U16 i = 0; i < TotalFiles; i++)
    {
      closeFile[i] = new CloseFile(fileName[i]);
      closeFile[i]->apply(*this);
    }

    cleanup.apply(*this);
  }

  // ----------------------------------------------------------------------
  // InitTest
  // ----------------------------------------------------------------------
  void Tester ::
    InitTest()
  {
    clearFileBuffer();

    // Instantiate the Rules
    const U16 NumberBins = MAX_BINS;
    const U16 NumberFiles = MAX_FILES_PER_BIN;

    InitFileSystem initFileSystem(NumberBins, FILE_SIZE, NumberFiles);
    Cleanup cleanup;

    // Run the Rules
    initFileSystem.apply(*this);
    cleanup.apply(*this);


  }

  // ----------------------------------------------------------------------
  // OpenWriteReadTest
  // ----------------------------------------------------------------------
  void Tester ::
    OpenWriteReadTest()
  {
    clearFileBuffer();

    // Instantiate the Rules
    const U16 NumberBins = MAX_BINS;
    const U16 NumberFiles = MAX_FILES_PER_BIN;
    const char* FileName = "/bin9/file9";

    InitFileSystem initFileSystem(NumberBins, FILE_SIZE, NumberFiles);
    OpenFile openFile(FileName);
    ResetFile resetFile(FileName);
    CloseFile closeFile(FileName);
    OpenRead openRead(FileName);
    Cleanup cleanup;
    WriteData writeData(FileName, FILE_SIZE, 0xFF);
    ReadData readData(FileName, FILE_SIZE);

    // Run the Rules
    initFileSystem.apply(*this);
    openFile.apply(*this);
    writeData.apply(*this);
    closeFile.apply(*this);
    openRead.apply(*this);
    resetFile.apply(*this);
    readData.apply(*this);

    cleanup.apply(*this);
    
  }


  // ----------------------------------------------------------------------
  // OpenWriteTwiceReadOnceTest
  // ----------------------------------------------------------------------
  void Tester ::
    OpenWriteTwiceReadOnceTest()
  {
    clearFileBuffer();

    // Instantiate the Rules
    const U16 NumberBins = MAX_BINS;
    const U16 NumberFiles = MAX_FILES_PER_BIN;
    const char* FileName = "/bin0/file0";

    InitFileSystem initFileSystem(NumberBins, FILE_SIZE, NumberFiles);
    OpenFile openFile(FileName);
    ResetFile resetFile(FileName);
    Cleanup cleanup;
    WriteData writeData1(FileName, FILE_SIZE/2, 0x11);
    WriteData writeData2(FileName, FILE_SIZE/2, 0x22);
    ReadData readData(FileName, FILE_SIZE);
    CloseFile closeFile(FileName);
    OpenRead openRead(FileName);

    // Run the Rules
    initFileSystem.apply(*this);
    openFile.apply(*this);
    writeData1.apply(*this);
    writeData2.apply(*this);
    closeFile.apply(*this);
    openRead.apply(*this);
    resetFile.apply(*this);
    readData.apply(*this);

    cleanup.apply(*this);
    
  }
  // ----------------------------------------------------------------------
  // OpenWriteOnceReadTwiceTest
  // ----------------------------------------------------------------------
  void Tester ::
    OpenWriteOnceReadTwiceTest()
  {
    clearFileBuffer();

    // Instantiate the Rules
    const U16 NumberBins = MAX_BINS;
    const U16 NumberFiles = MAX_FILES_PER_BIN;
    const char* FileName = "/bin0/file0";
    
    InitFileSystem initFileSystem(NumberBins, FILE_SIZE, NumberFiles);
    OpenFile openFile(FileName);
    ResetFile resetFile(FileName);
    CloseFile closeFile(FileName);
    Cleanup cleanup;
    OpenRead openRead(FileName);
    WriteData writeData(FileName, FILE_SIZE, 0xFF);
    ReadData readData(FileName, FILE_SIZE/2);

    // Run the Rules
    initFileSystem.apply(*this);
    openFile.apply(*this);
    writeData.apply(*this);
    closeFile.apply(*this);
    openRead.apply(*this);
    resetFile.apply(*this);
    readData.apply(*this);
    readData.apply(*this);

    cleanup.apply(*this);
    
  }

  // ----------------------------------------------------------------------
  // ListTest
  // ----------------------------------------------------------------------
  void Tester ::
    ListTest()
  {
    clearFileBuffer();

    // Instantiate the Rules
    const U16 NumberBins = MAX_BINS;
    const U16 NumberFiles = MAX_FILES_PER_BIN;
    const U16 TotalFiles = MAX_BINS * MAX_FILES_PER_BIN;

    OpenFile* openFile[TotalFiles];

    char fileName[TotalFiles][20];
    getFileNames(fileName, MAX_BINS, MAX_FILES_PER_BIN);
    
    // Instantiate the Rules
    InitFileSystem initFileSystem(NumberBins, FILE_SIZE, NumberFiles);
    Cleanup cleanup;
    Listings listings(NumberBins, NumberFiles);

    // Run the Rules
    initFileSystem.apply(*this);

    for (U16 i = 0; i < TotalFiles; i++)
    {
      openFile[i] = new OpenFile(fileName[i]);
      openFile[i]->apply(*this);
    }

    listings.apply(*this);

    cleanup.apply(*this);
    
  }

  // ----------------------------------------------------------------------
  // OpenWriteReadTest
  // ----------------------------------------------------------------------
  void Tester ::
    OpenFreeSpaceTest()
  {
    clearFileBuffer();

    // Instantiate the Rules
    U32 numBins = 1;
    U32 numFiles = 1;
    const char* FileName = "/bin0/file0";
    InitFileSystem initFileSystem(numBins, FILE_SIZE, numFiles);

    FreeSpace freeSpace;
    OpenFile openFile(FileName);
    // ResetFile resetFile;
    Cleanup cleanup;
    WriteData writeData(FileName, FILE_SIZE, 0xFF);

    // Run the Rules
    initFileSystem.apply(*this);
    this->m_expFreeBytes = 100;
    this->m_expTotalBytes = 100;
    freeSpace.apply(*this);
    openFile.apply(*this);
    writeData.apply(*this);
    this->m_expFreeBytes = 0;
    this->m_expTotalBytes = 100;
    freeSpace.apply(*this);

    cleanup.apply(*this);

  }


  // Helper functions
  void Tester::clearFileBuffer()
  {
        for (U32 i=0; i < MAX_TOTAL_FILES; i++)
        {
          this->fileModels[i].clear();
        }
  }

  I16 Tester::getIndex(const char *fileName) const
  {
    const char* filePathSpec = "/bin%d/file%d";

    FwNativeUIntType binIndex = 0;
    FwNativeUIntType fileIndex = 0;

    I16 stat = sscanf(fileName, filePathSpec, &binIndex, &fileIndex);
    if (stat != 2)
    {
      return -1;

    } else {
      return binIndex * MAX_BINS + fileIndex;
    }

  }

  Tester::FileModel* Tester::getFileModel(const char *filename)
  {
      I16 fileIndex = this->getIndex(filename);

      FW_ASSERT(fileIndex < MAX_TOTAL_FILES && fileIndex >= 0, fileIndex);

      return &(this->fileModels[fileIndex]);
  }


  void Tester::getFileNames(char fileNames[][20], U16 numBins, U16 numFiles) {
    int count = 0;
    for (int i = 0; i < numBins; i++) {
        for (int j = 0; j < numFiles; j++) {
            sprintf(fileNames[count], "/bin%d/file%d", i, j); // format the string
            count++;
        }
    }
  }


} // end namespace Os
