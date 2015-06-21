/* ============================================================================
* Copyright (c) 2009-2015 BlueQuartz Software, LLC
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
* Neither the name of BlueQuartz Software, the US Air Force, nor the names of its
* contributors may be used to endorse or promote products derived from this software
* without specific prior written permission.
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
* The code contained herein was partially funded by the followig contracts:
*    United States Air Force Prime Contract FA8650-07-D-5800
*    United States Air Force Prime Contract FA8650-10-D-5210
*    United States Prime Contract Navy N00173-07-C-2068
*
* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */


#include "SampleSurfaceMeshSpecifiedPoints.h"

#include "DREAM3DLib/Common/Constants.h"
#include "DREAM3DLib/FilterParameters/AbstractFilterParametersReader.h"
#include "DREAM3DLib/FilterParameters/AbstractFilterParametersWriter.h"
#include "DREAM3DLib/FilterParameters/FileSystemFilterParameter.h"

#include "Sampling/SamplingConstants.h"

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
SampleSurfaceMeshSpecifiedPoints::SampleSurfaceMeshSpecifiedPoints() :
  m_InputFilePath(""),
  m_OutputFilePath(""),
  m_FeatureIds(NULL),
  m_NumPoints(0)
{
  setupFilterParameters();
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
SampleSurfaceMeshSpecifiedPoints::~SampleSurfaceMeshSpecifiedPoints()
{
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void SampleSurfaceMeshSpecifiedPoints::setupFilterParameters()
{
  FilterParameterVector parameters = getFilterParameters();
  parameters.push_back(FileSystemFilterParameter::New("Specified Points File Path", "InputFilePath", FilterParameterWidgetType::InputFileWidget, getInputFilePath(), FilterParameter::Parameter, "", "*.raw, *.bin"));
  parameters.push_back(FileSystemFilterParameter::New("Sampled Values File Path", "OutputFilePath", FilterParameterWidgetType::OutputFileWidget, getOutputFilePath(), FilterParameter::Parameter, "", "*.txt"));
  setFilterParameters(parameters);
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void SampleSurfaceMeshSpecifiedPoints::readFilterParameters(AbstractFilterParametersReader* reader, int index)
{
  SampleSurfaceMesh::readFilterParameters(reader, index);
  reader->openFilterGroup(this, index);
  setInputFilePath(reader->readString("InputFilePath", getInputFilePath()));
  setOutputFilePath(reader->readString("OutputFilePath", getOutputFilePath()));
  reader->closeFilterGroup();
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
int SampleSurfaceMeshSpecifiedPoints::writeFilterParameters(AbstractFilterParametersWriter* writer, int index)
{
  SampleSurfaceMesh::writeFilterParameters(writer, index);
  writer->openFilterGroup(this, index);
  DREAM3D_FILTER_WRITE_PARAMETER(InputFilePath)
  DREAM3D_FILTER_WRITE_PARAMETER(OutputFilePath)
  writer->closeFilterGroup();
  return ++index; // we want to return the next index that was just written to
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void SampleSurfaceMeshSpecifiedPoints::updateVertexInstancePointers()
{
  setErrorCondition(0);
  if (NULL != m_FeatureIdsPtr.lock().get()) /* Validate the Weak Pointer wraps a non-NULL pointer to a DataArray<T> object */
  { m_FeatureIds = m_FeatureIdsPtr.lock()->getPointer(0); } /* Now assign the raw pointer to data from the DataArray<T> object */
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void SampleSurfaceMeshSpecifiedPoints::dataCheck()
{
  setErrorCondition(0);

  DataArrayPath tempPath;

  if (true == m_InputFilePath.isEmpty())
  {
    QString ss = QObject::tr("The input file must be set");
    setErrorCondition(-1);
    notifyErrorMessage(getHumanLabel(), ss, -1);
  }
  if (true == m_OutputFilePath.isEmpty())
  {
    QString ss = QObject::tr("The output file must be set");
    setErrorCondition(-1);
    notifyErrorMessage(getHumanLabel(), ss, -1);
  }

  DataContainer::Pointer v = getDataContainerArray()->createNonPrereqDataContainer<AbstractFilter>(this, "SpecifiedPoints");
  if (getErrorCondition() < 0 || NULL == v.get()) { return; }

  VertexGeom::Pointer vertices = VertexGeom::CreateGeometry(1, DREAM3D::Geometry::VertexGeometry);
  v->setGeometry(vertices);

  QVector<size_t> tDims(1, 0);
  v->createNonPrereqAttributeMatrix<AbstractFilter>(this, "SpecifiedPointsData", tDims, DREAM3D::AttributeMatrixType::Vertex);

  QVector<size_t> cDims(1, 1);
  tempPath.update("SpecifiedPoints", "SpecifiedPointsData", "FeatureIds");
  m_FeatureIdsPtr = getDataContainerArray()->createNonPrereqArrayFromPath<DataArray<int32_t>, AbstractFilter, int32_t>(this, tempPath, 0, cDims); /* Assigns the shared_ptr<>(this, tempPath, -301, dims);  Assigns the shared_ptr<> to an instance variable that is a weak_ptr<> */
  if (NULL != m_FeatureIdsPtr.lock().get()) /* Validate the Weak Pointer wraps a non-NULL pointer to a DataArray<T> object */
  { m_FeatureIds = m_FeatureIdsPtr.lock()->getPointer(0); } /* Now assign the raw pointer to data from the DataArray<T> object */
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void SampleSurfaceMeshSpecifiedPoints::preflight()
{
  // These are the REQUIRED lines of CODE to make sure the filter behaves correctly
  setInPreflight(true); // Set the fact that we are preflighting.
  emit preflightAboutToExecute(); // Emit this signal so that other widgets can do one file update
  emit updateFilterParameters(this); // Emit this signal to have the widgets push their values down to the filter
  dataCheck(); // Run our DataCheck to make sure everthing is setup correctly
  emit preflightExecuted(); // We are done preflighting this filter
  SampleSurfaceMesh::preflight();
  setInPreflight(false); // Inform the system this filter is NOT in preflight mode anymore.
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
VertexGeom::Pointer SampleSurfaceMeshSpecifiedPoints::generate_points()
{
  m_NumPoints = 0;

  std::ifstream inFile;
  inFile.open(m_InputFilePath.toLatin1().data());

  // get the number of points in the specified points file
  inFile >> m_NumPoints;

  if (m_NumPoints <= 0)
  {
    QString ss = QObject::tr("Number of points to sample (%1) must be positive").arg(m_NumPoints);
    setErrorCondition(-1);
    notifyErrorMessage(getHumanLabel(), ss, getErrorCondition());
    return VertexGeom::NullPointer();
  }

  DataContainer::Pointer v = getDataContainerArray()->getDataContainer("SpecifiedPoints");
  VertexGeom::Pointer points = VertexGeom::CreateGeometry(m_NumPoints, "Points");

  float coords[3] = { 0.0f, 0.0f, 0.0f };

  for (int64_t i = 0; i < m_NumPoints; i++)
  {
    inFile >> coords[0] >> coords[1] >> coords[2];
    points->setCoords(i, coords);
  }

  v->setGeometry(points);

  return points;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void SampleSurfaceMeshSpecifiedPoints::assign_points(Int32ArrayType::Pointer iArray)
{
  QVector<size_t> tDims(1, m_NumPoints);
  getDataContainerArray()->getDataContainer("SpecifiedPoints")->getAttributeMatrix("SpecifiedPointsData")->resizeAttributeArrays(tDims);
  updateVertexInstancePointers();

  int32_t* ids = iArray->getPointer(0);
  for (int64_t i = 0; i < m_NumPoints; i++)
  {
    m_FeatureIds[i] = ids[i];
  }
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void SampleSurfaceMeshSpecifiedPoints::execute()
{
  setErrorCondition(0);
  dataCheck();
  if(getErrorCondition() < 0) { return; }

  SampleSurfaceMesh::execute();

  std::ofstream outFile;
  outFile.open(m_OutputFilePath.toLatin1().data());
  for (int64_t i = 0; i < m_NumPoints; i++)
  {
    outFile << m_FeatureIds[i] << std::endl;
  }

  /* Let the GUI know we are done with this filter */
  notifyStatusMessage(getHumanLabel(), "Complete");
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
AbstractFilter::Pointer SampleSurfaceMeshSpecifiedPoints::newFilterInstance(bool copyFilterParameters)
{
  SampleSurfaceMeshSpecifiedPoints::Pointer filter = SampleSurfaceMeshSpecifiedPoints::New();
  if(true == copyFilterParameters)
  {
    copyFilterParameterInstanceVariables(filter.get());
  }
  return filter;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
const QString SampleSurfaceMeshSpecifiedPoints::getCompiledLibraryName()
{ return SamplingConstants::SamplingBaseName; }

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
const QString SampleSurfaceMeshSpecifiedPoints::getGroupName()
{ return DREAM3D::FilterGroups::SamplingFilters; }

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
const QString SampleSurfaceMeshSpecifiedPoints::getSubGroupName()
{ return DREAM3D::FilterSubGroups::ResolutionFilters; }

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
const QString SampleSurfaceMeshSpecifiedPoints::getHumanLabel()
{ return "Sample Triangle Geometry at Specified Points"; }
