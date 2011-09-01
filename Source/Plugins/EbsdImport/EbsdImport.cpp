/* ============================================================================
 * Copyright (c) 2010, Michael A. Jackson (BlueQuartz Software)
 * Copyright (c) 2010, Dr. Michael A. Groeber (US Air Force Research Laboratories)
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 * Redistributions of source code must retain the above copyright notice, this
 * list of conditions and the following disclaimer.
 *
 * Redistributions in binary form must reproduce the above copyright notice, this
 * list of conditions and the following disclaimer in the documentation and/or
 * other materials provided with the distribution.
 *
 * Neither the name of Michael A. Groeber, Michael A. Jackson, the US Air Force,
 * BlueQuartz Software nor the names of its contributors may be used to endorse
 * or promote products derived from this software without specific prior written
 * permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE
 * USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 *  This code was written under United States Air Force Contract number
 *                           FA8650-07-D-5800
 *
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
#include "EbsdImport.h"

#include "H5Support/H5Utilities.h"
#include "MXA/Utilities/MXAFileInfo.h"
#include "MXA/Utilities/StringUtils.h"

#include "EbsdLib/EbsdConstants.h"
#include "EbsdLib/EbsdImporter.h"
#include "EbsdLib/TSL/AngConstants.h"
#include "EbsdLib/TSL/H5AngImporter.h"
#include "EbsdLib/HKL/CtfConstants.h"
#include "EbsdLib/HKL/H5CtfImporter.h"

/**
 * @brief Just a dummy class in order to make the EBSDImoprt work with some
 * of the other source codes.
 */
class EbsdImportFunc
{
  public:
    MXA_SHARED_POINTERS(EbsdImportFunc);
    MXA_STATIC_NEW_MACRO(EbsdImportFunc);

    virtual ~EbsdImportFunc() {
      if (*m_FileId > 0)
      {
        std::cout << "EbsdImportFunc Closing HDF5 File" << std::endl;
        H5Utilities::closeFile(*m_FileId);
      }
    }

    hid_t* m_FileId;

protected:
  EbsdImportFunc() :m_FileId(NULL) {}

private:
  EbsdImportFunc(const EbsdImportFunc&); // Copy Constructor Not Implemented
  void operator=(const EbsdImportFunc&); // Operator '=' Not Implemented

};

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
EbsdImport::EbsdImport( ) :
m_ZStartIndex(0),
m_ZEndIndex(0),
m_ZResolution(1.0)
{
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
EbsdImport::~EbsdImport()
{
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void EbsdImport::execute()
{
  herr_t err = 0;
  hid_t fileId = -1;
  // This is just a dummy variable to keep the macros happy
  EbsdImportFunc::Pointer m = EbsdImportFunc::New();
  m->m_FileId = &fileId;

  // Start the Benchmark clock
  START_CLOCK()

  if (m_OutputFile.empty() == true)
  {
    std::string s("EbsdImport Error: The output file was not set correctly or is empty. The current value is '");
    s.append("'. Please set the output file before running the importer. ");
    updateProgressAndMessage(s.c_str(), 100);
    err = -1;
    CHECK_FOR_ERROR(EbsdImportFunc, "EbsdImport input filename was empty", err)
    return;
  }
  // Create File
  fileId = H5Utilities::createFile(m_OutputFile);
  if (fileId < 0) {
    setCancel(true);
    updateProgressAndMessage("The Output HDF5 file could not be created. Check Permissions, if the File is in use by another program.", 100);
    CHECK_FOR_ERROR(EbsdImportFunc, "Output HDF5 file could not be created. Check permissions on parent folder??", err)
    return;
  }

  // Write Z index start, Z index end and Z Resolution to the HDF5 file
  err = H5Lite::writeScalarDataset(fileId, Ebsd::H5::ZStartIndex, m_ZStartIndex);
  CHECK_FOR_ERROR(EbsdImportFunc, "Could not write the Z Start Index Scalar", err)
  err = H5Lite::writeScalarDataset(fileId, Ebsd::H5::ZEndIndex, m_ZEndIndex);
  CHECK_FOR_ERROR(EbsdImportFunc, "Could not write the Z End Index Scalar", err)
  err = H5Lite::writeScalarDataset(fileId, Ebsd::H5::ZResolution, m_ZResolution);
  CHECK_FOR_ERROR(EbsdImportFunc, "Could not write the Z Resolution Scalar", err)

  EbsdImporter::Pointer fileImporter;

  // Write the Manufacturer of the OIM file here
  // This list will grow to be the number of EBSD file formats we support
  std::string ext = MXAFileInfo::extension(m_EbsdFileList.front());
  if (ext.compare(Ebsd::Ang::FileExt) == 0)
  {
    err = H5Lite::writeStringDataset(fileId, Ebsd::H5::Manufacturer, Ebsd::Ang::Manufacturer );
    CHECK_FOR_ERROR(EbsdImportFunc, "Could not write the Manufacturer Scalar", err)
    fileImporter = H5AngImporter::New();
  }
  else if (ext.compare(Ebsd::Ctf::FileExt) == 0)
  {
    err = H5Lite::writeStringDataset(fileId, Ebsd::H5::Manufacturer, Ebsd::Ctf::Manufacturer );
    CHECK_FOR_ERROR(EbsdImportFunc, "Could not write the Manufacturer Scalar", err)
    fileImporter = H5CtfImporter::New();
  }
  else
  {
    updateProgressAndMessage("The File extension was not detected correctly", 100);
    setErrorCondition(-1);
    return;
  }

  std::vector<int> indices;
  // Loop on Each EBSD File
  float total = m_ZEndIndex - m_ZStartIndex;
  int progress = 0;
  int z = m_ZStartIndex;
  int xDim, yDim;
  float xRes, yRes;

  /* There is a frailness about the z index and the file list. The programmer
   * using this code MUST ensure that the list of files that is sent into this
   * class is in the appropriate order to match up with the z index (slice index)
   * otherwise the import will have subtle errors. The programmer is urged NOT to
   * simply gather a list from the file system as those lists are sorted in such
   * a way that if the number of digits appearing in the filename are NOT the same
   * then the list will be wrong, ie, this example:
   *
   * slice_1.ang
   * slice_2.ang
   * ....
   * slice_10.ang
   *
   * Most, if not ALL C++ libraries when asked for that list will return the list
   * sorted like the following:
   *
   * slice_1.ang
   * slice_10.ang
   * slice_2.ang
   *
   * which is going to cause problems because the data is going to be placed
   * into the HDF5 file at the wrong index. YOU HAVE BEEN WARNED.
   */
  int biggestxDim = 0;
  int biggestyDim = 0;
  for (std::vector<std::string>::iterator filepath = m_EbsdFileList.begin(); filepath != m_EbsdFileList.end(); ++filepath )
  {
    std::string ebsdFName = *filepath;
    progress = z - m_ZStartIndex;
    progress = (int)(100.0f * (float)(progress)/total);
    std::string msg = "Importing: " + ebsdFName;
    updateProgressAndMessage(msg.c_str(), progress );
    //H5AngImporter::Pointer conv = H5AngImporter::New();
    err = fileImporter->importFile(fileId, z, ebsdFName);
    fileImporter->getDims(xDim, yDim);
    fileImporter->getResolution(xRes, yRes);
    if(xDim > biggestxDim) biggestxDim = xDim;
    if(yDim > biggestyDim) biggestyDim = yDim;

    if (err < 0)
    {
      CHECK_FOR_ERROR(EbsdImportFunc, "Could not write dataset for slice.", err)
    }
    indices.push_back(z);
    ++z;
    CHECK_FOR_CANCELED(EbsdImportFunc, "EbsdImport was Canceled", import_data)
  }

  err = H5Lite::writeScalarDataset(fileId, Ebsd::H5::XPoints, biggestxDim);
  CHECK_FOR_ERROR(EbsdImportFunc, "Could not write the XPoints Scalar", err)
  err = H5Lite::writeScalarDataset(fileId, Ebsd::H5::YPoints, biggestyDim);
  CHECK_FOR_ERROR(EbsdImportFunc, "Could not write the YPoints Scalar", err)
  err = H5Lite::writeScalarDataset(fileId, Ebsd::H5::XResolution, xRes);
  CHECK_FOR_ERROR(EbsdImportFunc, "Could not write the XResolution Scalar", err)
  err = H5Lite::writeScalarDataset(fileId, Ebsd::H5::YResolution, yRes);
  CHECK_FOR_ERROR(EbsdImportFunc, "Could not write the YResolution Scalar", err)

  if (false == getCancel())
  {
  // Write an Index data set which contains all the z index values which
  // should help speed up the reading side of this file
    std::vector<hsize_t> dims(1, indices.size());
    err = H5Lite::writeVectorDataset(fileId, Ebsd::H5::Index, dims, indices);
  }
  err = H5Utilities::closeFile(fileId);
  // err = H5Fclose(fileId);
  m = EbsdImportFunc::NullPointer();
  updateProgressAndMessage("Import Complete", 100);
}

