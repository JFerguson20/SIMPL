/* ============================================================================
 * Copyright (c) 2012 Michael A. Jackson (BlueQuartz Software)
 * Copyright (c) 2012 Dr. Michael A. Groeber (US Air Force Research Laboratories)
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
#ifndef _InputPathWidget_H_
#define _InputPathWidget_H_




#include <QtCore/QObject>
#include <QtCore/QPointer>
#include <QtGui/QWidget>


#include "QtSupport/FaderWidget.h"


#include "DREAM3DLib/Common/AbstractFilter.h"

#include "DREAM3DWidgetsLib/DREAM3DWidgetsLib.h"

#include "DREAM3DWidgetsLib/ui_InputPathWidget.h"


/**
* @brief
* @author
* @version
*/
class DREAM3DWidgetsLib_EXPORT InputPathWidget : public QWidget, private Ui::InputPathWidget
{
    Q_OBJECT

  public:
    /**
    * @brief Constructor
    * @param parameter The FilterParameter object that this widget represents
    * @param filter The instance of the filter that this parameter is a part of
    * @param parent The parent QWidget for this Widget
    */
    InputPathWidget(FilterParameter* parameter, AbstractFilter* filter = NULL, QWidget* parent = NULL);

    virtual ~InputPathWidget();

    /**
    * @brief This method does additional GUI widget connections
    */
    void setupGui();

    /**
    * @brief
    */
    bool verifyPathExists(QString filePath, QLineEdit* lineEdit);

  public slots:
    void widgetChanged(const QString& msg);// when something in the widget changes
    void filterNeedsInputParameters(AbstractFilter* filter); // When the filter is ready for us to update its input parameter(s) that we are responsible for
    void beforePreflight(); // Called just before the "dataCheck()" is called
    void afterPreflight(); // Called just after the dataCheck() is called.

    void on_value_textChanged(const QString& text);
    void on_value_editingFinished();
    void on_selectBtn_clicked();
    void setLinkedConditionalState(int state);
    void fadeWidget(QWidget* widget, bool in);

  signals:
    void errorSettingFilterParameter(const QString& msg);
    void parametersChanged();

  private:
    AbstractFilter*   m_Filter;
    FilterParameter*  m_FilterParameter;
    static QString    m_OpenDialogLastDirectory;
    bool m_DidCausePreflight;
    QPointer<FaderWidget> faderWidget;

    InputPathWidget(const InputPathWidget&); // Copy Constructor Not Implemented
    void operator=(const InputPathWidget&); // Operator '=' Not Implemented

};

#endif /* _InputPathWidget_H_ */


