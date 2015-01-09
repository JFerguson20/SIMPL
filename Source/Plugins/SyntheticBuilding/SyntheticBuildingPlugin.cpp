/* ============================================================================
 * Copyright (c) 2014 Michael A. Jackson (BlueQuartz Software)
 * Copyright (c) 2014 Dr. Michael A. Groeber (US Air Force Research Laboratories)
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
 *                           FA8650-10-D-5210
 *
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */

#include "SyntheticBuildingPlugin.h"

#include "DREAM3DLib/Common/FilterManager.h"
#include "DREAM3DLib/Common/IFilterFactory.hpp"
#include "DREAM3DLib/Common/FilterFactory.hpp"


#include "moc_SyntheticBuildingPlugin.cpp"

Q_EXPORT_PLUGIN2(SyntheticBuildingPlugin, SyntheticBuildingPlugin)

namespace Detail
{
  const QString SyntheticBuildingPluginFile("SyntheticBuildingPlugin");
  const QString SyntheticBuildingPluginDisplayName("SyntheticBuildingPlugin");
  const QString SyntheticBuildingPluginBaseName("SyntheticBuildingPlugin");
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
SyntheticBuildingPlugin::SyntheticBuildingPlugin() :
m_Version(""),
m_CompatibilityVersion(""),
m_Vendor(""),
m_Group(""),
m_URL(""),
m_Location(""),
m_Platforms(QList<QString>()),
m_Description(""),
m_Copyright(""),
m_License(""),
m_Dependencies(QList<QString>())
{

}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
SyntheticBuildingPlugin::~SyntheticBuildingPlugin()
{
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
QString SyntheticBuildingPlugin::getPluginName()
{
  return (Detail::SyntheticBuildingPluginDisplayName);
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
QString SyntheticBuildingPlugin::getVersion()
{
  return "";
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
QString SyntheticBuildingPlugin::getCompatibilityVersion()
{
  return "";
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
QString SyntheticBuildingPlugin::getVendor()
{
  return "";
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
QString SyntheticBuildingPlugin::getGroup()
{
  return "";
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
QString SyntheticBuildingPlugin::getURL()
{
  return "";
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
QString SyntheticBuildingPlugin::getLocation()
{
  return "";
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
QList<QString> SyntheticBuildingPlugin::getPlatforms()
{
  QList<QString> empty;
  return empty;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
QString SyntheticBuildingPlugin::getDescription()
{
  return "";
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
QString SyntheticBuildingPlugin::getCopyright()
{
  return "";
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
QString SyntheticBuildingPlugin::getLicense()
{
  return "";
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
QList<QString> SyntheticBuildingPlugin::getDependencies()
{
  QList<QString> empty;
  return empty;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void SyntheticBuildingPlugin::writeSettings(QSettings& prefs)
{

}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void SyntheticBuildingPlugin::readSettings(QSettings& prefs)
{

}

#include "SyntheticBuildingFilters/RegisterKnownFilters.cpp"
#include "SyntheticBuilding/FilterParameterWidgets/RegisterKnownFilterParameterWidgets.cpp"
