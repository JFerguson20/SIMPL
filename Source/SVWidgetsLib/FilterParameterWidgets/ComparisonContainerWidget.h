/* ============================================================================
* Copyright (c) 2009-2016 BlueQuartz Software, LLC
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

#ifndef _ComparisonContainerWidget_h_
#define _ComparisonContainerWidget_h_

#include <QtWidgets/QFrame>
#include <QtGui/QMouseEvent>

#include "SIMPLib/Common/AbstractComparison.h"

#include "SVWidgetsLib/SVWidgetsLib.h"
#include "SVWidgetsLib/FilterParameterWidgets/IComparisonWidget.h"

#include "ui_ComparisonContainerWidget.h"

class ComparisonSetWidget;

class SVWidgetsLib_EXPORT ComparisonContainerWidget : public QFrame, private Ui::ComparisonContainerWidget
{
  Q_OBJECT

public:
  ComparisonContainerWidget(QWidget* parent, AbstractComparison::Pointer comparison);
  ~ComparisonContainerWidget();

  int getUnionOperator();
  void setUnionOperator(int unionOperator);

  void showUnionOperator(bool enabled = true);
  void hideUnionOperator();

  void setComparison(AbstractComparison::Pointer comparison);
  void setComparisonWidget(IComparisonWidget* widget);

  AbstractComparison::Pointer getCurrentComparison();
  IComparisonWidget* getComparisonWidget();

  ComparisonSetWidget* getComparisonSetWidget();
  void setComparisonSetWidget(ComparisonSetWidget* comparisonSetWidget);

  void select();
  void deselect();

  /**
  * @brief This method does additional GUI widget connections
  */
  void setupGui();

signals:
  void comparisonChanged();

protected:
  static ComparisonContainerWidget* SelectedItem;
  static QString BorderStyleSheet;

  void mousePressEvent(QMouseEvent* event);
  void mouseMoveEvent(QMouseEvent* event);
  void mouseReleaseEvent(QMouseEvent* event);

protected slots:
  void deleteItem();
  void unionOperatorChanged(int unionOp);

private:
  IComparisonWidget* m_comparisonWidget;
  ComparisonSetWidget* m_comparisonSetWidget;

  QPoint m_startDragPoint;
};

#endif
