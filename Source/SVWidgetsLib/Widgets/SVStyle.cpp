/* ============================================================================
* Copyright (c) 2018 BlueQuartz Software, LLC
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

#include "SVStyle.h"

#include <QtCore/QDebug>
#include <QtCore/QJsonDocument>
#include <QtCore/QJsonObject>
#include <QtCore/QFile>
#include <QtCore/QFileInfo>
#include <QtCore/QTextStream>
#include <QtCore/QJsonValue>

#include <QtWidgets/QApplication>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>

#include "SIMPLib/Common/Constants.h"

static QMap<QString, QImage> s_NameToImage;

SVStyle* SVStyle::self = nullptr;

namespace  {
const QString kNormalColor("#8f8f91");
const QString kErrorColor("#BC0000");
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
SVStyle::SVStyle()
{
  Q_ASSERT_X(!self, "SVStyle", "There should be only one SVStyle object");
  SVStyle::self = this;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
SVStyle::~SVStyle() = default;



// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
SVStyle* SVStyle::Instance()
{
  if(self == nullptr)
  {
    self = new SVStyle();
  }
  return self;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
bool SVStyle::loadStyleSheet(const QString &jsonFilePath)
{
  bool success = true;
  
  QFileInfo jsonFileInfo(jsonFilePath);
  QString cssFileName = QString("%1/%2.css").arg(jsonFileInfo.absolutePath(), 1).arg(jsonFileInfo.baseName(), 2);
  
  
  //QString cssFileName(":/StyleSheets/" + name + ".css");
  QFile cssFile(cssFileName);
  if(!cssFile.open(QFile::ReadOnly))
  {
    qDebug() << "Could not open CSS File " << cssFileName;
    return false; 
  }
  
//  QString jsonFilePath(":/StyleSheets/" + name + ".json");
  QFile jsonFile(jsonFilePath);
  if(!jsonFile.open(QFile::ReadOnly))
  {
    qDebug() << "Could not open JSON File " << jsonFilePath;
    return false; 
  }
  
  QString cssContent = QString::fromLatin1(cssFile.readAll());
  
  QByteArray jsonContent = jsonFile.readAll();
  QJsonParseError parseError;
  QJsonDocument jsonDoc = QJsonDocument::fromJson(jsonContent, &parseError);
  if(parseError.error != QJsonParseError::NoError)
  {
    qDebug() << "JSON Parsing Error:";
    qDebug() << parseError.errorString();
    return false;
  }
  QJsonObject rootObj = jsonDoc.object();
  
  QStringList keys = rootObj.keys();
  QStringList::const_iterator constIterator;
  for (constIterator = keys.constBegin(); constIterator != keys.constEnd(); ++constIterator)
  {
    const QString key = *constIterator;
    QString value = rootObj[key].toString();
    cssContent = cssContent.replace(key, value);
    if(value.startsWith("#"))
    {
      this->setProperty( key.toLocal8Bit().constData(), QColor(value));
    }
    else
    {
      bool ok = false;
      value = value.replace("rgb(", "");
      value = value.replace(")", "");
      value = value.replace(" ", "");
      QStringList tokens = value.split(",");
      if(tokens.size() == 3)
      {
        int r = tokens[0].toInt(&ok);
        int g = tokens[1].toInt(&ok);
        int b = tokens[2].toInt(&ok);
        bool didSet = this->setProperty( key.toLocal8Bit().constData(), QColor(r, g, b));
        if(!didSet)
        {
          qDebug() << "Property: " << key << " was not set correctly";
        }
      }
    }
    
    // QFile tmp("/tmp/style.css");
    // tmp.open(QFile::WriteOnly);
    // QTextStream out(&tmp);
    // out << cssContent;
    // tmp.close();
    
  
  }
  

  qApp->setStyleSheet(cssContent);
  
  return success;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
QString SVStyle::GetUIFont()
{
#if defined(Q_OS_MAC)
  QString fontString("FiraSans");
#elif defined(Q_OS_WIN)
  QString fontString("FiraSans");
#else
  QString fontString("Arial");
#endif

  QFont font(fontString);
  if(font.fromString(fontString))
  {
    return font.toString();
  }
  else
  {
    QFont font;
    return font.toString();
  }
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
QFont SVStyle::GetHumanLabelFont()
{
  QFont humanLabelFont(GetUIFont());
  humanLabelFont.setBold(true);
  humanLabelFont.setItalic(false);
  humanLabelFont.setWeight(QFont::Bold);
  humanLabelFont.setStyleStrategy(QFont::PreferAntialias);
  humanLabelFont.setFamily(GetUIFont());

#if defined(Q_OS_MAC)
  humanLabelFont.setPointSize(16);
#elif defined(Q_OS_WIN)
  humanLabelFont.setPointSize(13);
#else
  humanLabelFont.setPointSize(12);
#endif
  return humanLabelFont;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
QFont SVStyle::GetBrandingLabelFont()
{
  QFont brandingFont(GetUIFont());
  brandingFont.setBold(true);
  brandingFont.setItalic(true);
  brandingFont.setWeight(QFont::Bold);
  brandingFont.setStyleStrategy(QFont::PreferAntialias);
  brandingFont.setFamily(GetUIFont());

#if defined(Q_OS_MAC)
  brandingFont.setPointSize(11);
#elif defined(Q_OS_WIN)
  brandingFont.setPointSize(10);
#else
  brandingFont.setPointSize(10);
#endif
  return brandingFont;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
QFont SVStyle::GetCategoryFont()
{
  QFont categoryFont(GetUIFont());
  categoryFont.setBold(true);
  categoryFont.setWeight(QFont::Bold);
  categoryFont.setStyleStrategy(QFont::PreferAntialias);
  categoryFont.setFamily(GetUIFont());

#if defined(Q_OS_MAC)
  categoryFont.setPointSize(11);
#elif defined(Q_OS_WIN)
  categoryFont.setPointSize(10);
#else
  categoryFont.setPointSize(10);
#endif

  return categoryFont;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
QFont SVStyle::GetTitleFont()
{
  QFont categoryFont(GetUIFont());
  categoryFont.setBold(true);
  categoryFont.setWeight(QFont::Bold);
  categoryFont.setStyleStrategy(QFont::PreferAntialias);
  categoryFont.setFamily(GetUIFont());

#if defined(Q_OS_MAC)
  categoryFont.setPointSize(16);
#elif defined(Q_OS_WIN)
  categoryFont.setPointSize(12);
#else
  categoryFont.setPointSize(12);
#endif

  return categoryFont;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void SVStyle::LineEditErrorStyle(QLineEdit* lineEdit)
{
  QString str;
  QTextStream ss(&str);
  ss << "QLineEdit#" << lineEdit->objectName() << "{";
  //  ss << "border: 1px solid rgb(180, 0, 0);";
  ss << "background-color: rgb(255, 246, 179);"; // Yellow background
  ss << "}";
  lineEdit->setStyleSheet(str);
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void SVStyle::LineEditClearStyle(QLineEdit* lineEdit)
{
  lineEdit->setStyleSheet("");
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void SVStyle::LineEditRedErrorStyle(QLineEdit* lineEdit)
{
  QString str;
  QTextStream ss(&str);
  ss << "QLineEdit#" << lineEdit->objectName() << "{";
  //  ss << "border: 1px solid rgb(180, 0, 0);";
  ss << "background-color: rgb(208, 128, 139);"; // Yellow background
  ss << "}";
  lineEdit->setStyleSheet(str);
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
QString SVStyle::QToolSelectionButtonStyle(bool exists)
{
  QString str;
  #if 0
  QTextStream ss(&str);

  QFont font;
  font.setBold(true);
  font.setItalic(true);
  font.setWeight(QFont::Bold);
  font.setStyleStrategy(QFont::PreferAntialias);
  font.setFamily(GetUIFont());

  QString fontString;
  QTextStream in(&fontString);

#if defined(Q_OS_MAC)
  font.setPointSize(12);
#elif defined(Q_OS_WIN)
  font.setPointSize(10);
#else
  font.setPointSize(11);
  in << "color; #000000;\n";
  in << "font-weight: Medium;";
#endif

  in << "font: " << font.weight() << " " << font.pointSize() << "pt \"" << font.family()  << "\";";

  ss << "QToolButton {\n";
  if(exists)
  {
    ss << " border: 1px solid " << ::kNormalColor << ";\n";
  }
  else
  {
    ss << " border: 1px solid " << ::kErrorColor << ";\n";
  }
  ss << " border-radius: 4px;\n";
  ss << " background-color: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1, stop: 0 #DDDDDD, stop: 1 #FFFFFF);\n";
  ss << fontString << "\n";
  ss << " padding-left: 16px;\n";
  ss << " padding-right: 12px;\n";
  ss << " padding-top: 2px;\n";
  ss << " padding-bottom: 2px;\n";
  ss << "}\n";

  ss << "QToolButton::menu-indicator {\n";
  ss << " subcontrol-origin: content;\n";
  ss << " subcontrol-position:  right; /* */\n";
  ss << "}\n";

  ss << "QToolButton::menu-indicator:pressed, QToolButton::menu-indicator:open {\n";
  ss << " position: relative;\n";
  ss << "}\n";

  ss << "QToolButton:pressed {\n";
  ss << " background-color: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1,\nstop: 0 " << QApplication::palette().highlight().color().name() << ", stop: 1 #FFFFFF);\n";
  ss << "}\n";

  ss << "QToolButton:flat {\n";
  ss << " border: none;\n";
  ss << "}\n";

  ss << " QToolTip {\
              border: 2px solid #434343;\
              padding: 2px;\
              border-radius: 3px;\
              opacity: 255;\
              background-color: #FFFCEA;\
              color: #000000;\
              }";
#endif
  return str;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
QColor SVStyle::ColorForFilterGroup(const QString &grpName)
{
  QColor color(102, 96, 255);

  QString jsonString;
  QFile jsonFile;
  jsonFile.setFileName(":/QtSupportResources/FilterStyle/SVFilterColors.json");

  if(jsonFile.exists())
  {
    jsonFile.open(QIODevice::ReadOnly | QIODevice::Text);
    jsonString = jsonFile.readAll();
    jsonFile.close();

    QJsonDocument doc = QJsonDocument::fromJson(jsonString.toUtf8());
    QJsonObject jsonObj = doc.object();
    QJsonValue jsonValue = jsonObj.value("Filter Group Colors");

    if(jsonValue.isObject())
    {
      QJsonValue jsonColor = jsonValue.toObject().value(grpName);
      if(jsonColor.isString())
      {
        color.setNamedColor(jsonColor.toString());
      }
      else
      {
        jsonColor = jsonValue.toObject().value("Filter Group Not Found");
        if(jsonColor.isString())
        {
          color.setNamedColor(jsonColor.toString());
        }
      }
    }
  }
  else
  {
    int saturation = 110;
    int brightness = 190;
    if(grpName.compare(SIMPL::FilterGroups::Unsupported) == 0)
    {
      color = QColor::fromHsv(0, saturation, brightness);
    }
    else if(grpName.compare(SIMPL::FilterGroups::Generic) == 0)
    {
      color = QColor::fromHsv(30, saturation, brightness);
    }
    else if(grpName.compare(SIMPL::FilterGroups::ReconstructionFilters) == 0)
    {
      color = QColor::fromHsv(54, saturation, brightness);
    }
    else if(grpName.compare(SIMPL::FilterGroups::SamplingFilters) == 0)
    {
      color = QColor::fromHsv(84, saturation, brightness);
    }
    else if(grpName.compare(SIMPL::FilterGroups::StatisticsFilters) == 0)
    {
      color = QColor::fromHsv(120, saturation, brightness);
    }
    else if(grpName.compare(SIMPL::FilterGroups::SyntheticBuildingFilters) == 0)
    {
      color = QColor::fromHsv(150, saturation, brightness);
    }
    else if(grpName.compare(SIMPL::FilterGroups::SurfaceMeshingFilters) == 0)
    {
      color = QColor::fromHsv(180, saturation, brightness);
    }
    else if(grpName.compare(SIMPL::FilterGroups::ProcessingFilters) == 0)
    {
      color = QColor::fromHsv(210, saturation, brightness);
    }
    else if(grpName.compare(SIMPL::FilterGroups::CoreFilters) == 0)
    {
      color = QColor::fromHsv(240, saturation, brightness);
    }
    else if(grpName.compare(SIMPL::FilterGroups::IOFilters) == 0)
    {
      color = QColor::fromHsv(270, saturation, brightness);
    }
    else if(grpName.compare(SIMPL::FilterGroups::Utilities) == 0)
    {
      color = QColor::fromHsv(300, saturation, brightness);
    }
    else
    {
      color = QColor::fromHsv(330, saturation, brightness);
    }
  }


  return color;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
QColor SVStyle::GetFilterBackgroundColor()
{
  return self->getFilterBackgroundColor();
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
QColor SVStyle::GetFilterSelectionColor()
{
  return self->getFilterSelectionColor();
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
QColor SVStyle::GetFilterFontColor()
{
  return self->getFilterFontColor();
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
QIcon SVStyle::IconForGroup(const QString &grpName)
{
  
  QColor color = ColorForFilterGroup(grpName);
  QImage grpImage;
  
  if(s_NameToImage.contains(grpName))
  {
    return QIcon(QPixmap::fromImage(s_NameToImage[grpName]));
  }

  QIcon grpIcon(":/BlankGroup_Icon.png");
  if(!grpIcon.isNull())
  {
    grpImage = grpIcon.pixmap(QSize(48, 48)).toImage();

    QSize imageSize = grpImage.size();
    for(int h = 0; h < imageSize.height(); h++)
    {
      for(int w = 0; w < imageSize.width(); w++)
      {
        QColor pixel = grpImage.pixelColor(w, h);
        if(pixel.red() == 228 && pixel.green() == 228 && pixel.blue() == 228 && pixel.alpha() != 0)
        {
          pixel = color;
          pixel.setRedF((pixel.redF() * 1.50 > 1.0) ? 1.0 : pixel.redF() * 1.50);
          pixel.setGreenF((pixel.greenF() * 1.50 > 1.0) ? 1.0 : pixel.greenF() * 1.50);
          pixel.setBlueF((pixel.blueF() * 1.50 > 1.0) ? 1.0 : pixel.blueF() * 1.50);
          grpImage.setPixelColor(w, h, pixel);
        }

        if(pixel.red() == 150 && pixel.green() == 150 && pixel.blue() == 150 && pixel.alpha() != 0)
        {
          pixel = color;
          //          pixel.setRedF(pixel.redF() * 1.50);
          //          pixel.setGreenF(pixel.greenF() * 1.50);
          //          pixel.setBlueF(pixel.blueF() * 1.50);
          grpImage.setPixelColor(w, h, pixel);
        }

        if(pixel.red() == 53 && pixel.green() == 53 && pixel.blue() == 53 && pixel.alpha() != 0)
        {
          pixel = color;
          pixel.setRedF(pixel.redF() * .50);
          pixel.setGreenF(pixel.greenF() * .50);
          pixel.setBlueF(pixel.blueF() * .50);
          grpImage.setPixelColor(w, h, pixel);
        }
      }
    }
  }
  s_NameToImage[grpName] = grpImage;

  return QIcon(QPixmap::fromImage(grpImage));
}


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
QString SVStyle::StyleSheetForButton(const QString &objectName, const QString &cssName, const QString &imageName)
{
  QString css;
  #if 0
  QFile cssFile(QString("%1").arg(cssName));
  if(cssFile.open(QFile::ReadOnly))
  {
    css = cssFile.readAll();
    cssFile.close();
  }
  else
  {
    qDebug() << "Could not load CSS file: " << cssName;
  }

  css = css.replace("@OBJECT_NAME@", objectName);
  css = css.replace("@IMAGE_PATH@", imageName);
#endif
  return css;
}
