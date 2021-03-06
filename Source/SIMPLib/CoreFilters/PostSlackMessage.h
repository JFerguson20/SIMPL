/* ============================================================================
 * Copyright (c) 2017 BlueQuartz Software, LLC
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
 * Neither the names of any of the BlueQuartz Software contributors
 * may be used to endorse or promote products derived from this software without
 * specific prior written permission.
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
 *
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */

#pragma once

#include <QtCore/QWaitCondition>

#include "SIMPLib/Common/SIMPLibSetGetMacros.h"
#include "SIMPLib/Filtering/AbstractFilter.h"
#include "SIMPLib/SIMPLib.h"

class QNetworkAccessManager;
class QNetworkReply;

/**
 * @brief The PostSlackMessage class. See [Filter documentation](@ref postslackmessage) for details.
 */
class SIMPLib_EXPORT PostSlackMessage : public AbstractFilter
{
  Q_OBJECT
  PYB11_CREATE_BINDINGS(PostSlackMessage SUPERCLASS AbstractFilter)
  PYB11_PROPERTY(QString SlackUser READ getSlackUser WRITE setSlackUser)
  PYB11_PROPERTY(QString SlackUrl READ getSlackUrl WRITE setSlackUrl)
  PYB11_PROPERTY(QString SlackMessage READ getSlackMessage WRITE setSlackMessage)
  PYB11_PROPERTY(bool WarningsAsError READ getWarningsAsError WRITE setWarningsAsError)

  public:
    SIMPL_SHARED_POINTERS(PostSlackMessage)
    SIMPL_FILTER_NEW_MACRO(PostSlackMessage)
    SIMPL_TYPE_MACRO_SUPER_OVERRIDE(PostSlackMessage, AbstractFilter)

    ~PostSlackMessage() override;

  SIMPL_FILTER_PARAMETER(QString, SlackUser)
  Q_PROPERTY(QString SlackUser READ getSlackUser WRITE setSlackUser)

  SIMPL_FILTER_PARAMETER(QString, SlackUrl)
  Q_PROPERTY(QString SlackUrl READ getSlackUrl WRITE setSlackUrl)

  SIMPL_FILTER_PARAMETER(QString, SlackMessage)
  Q_PROPERTY(QString SlackMessage READ getSlackMessage WRITE setSlackMessage)

  SIMPL_FILTER_PARAMETER(bool, WarningsAsError)
  Q_PROPERTY(bool WarningsAsError READ getWarningsAsError WRITE setWarningsAsError)

    /**
     * @brief getCompiledLibraryName Reimplemented from @see AbstractFilter class
     */
    const QString getCompiledLibraryName() const override;

    /**
     * @brief getBrandingString Returns the branding string for the filter, which is a tag
     * used to denote the filter's association with specific plugins
     * @return Branding string
    */
    const QString getBrandingString() const override;

    /**
     * @brief getFilterVersion Returns a version string for this filter. Default
     * value is an empty string.
     * @return
     */
    const QString getFilterVersion() const override;

    /**
     * @brief newFilterInstance Reimplemented from @see AbstractFilter class
     */
    AbstractFilter::Pointer newFilterInstance(bool copyFilterParameters) const override;

    /**
     * @brief getGroupName Reimplemented from @see AbstractFilter class
     */
    const QString getGroupName() const override;

    /**
     * @brief getSubGroupName Reimplemented from @see AbstractFilter class
     */
    const QString getSubGroupName() const override;

    /**
     * @brief getUuid Return the unique identifier for this filter.
     * @return A QUuid object.
     */
    const QUuid getUuid() override;

    /**
     * @brief getHumanLabel Reimplemented from @see AbstractFilter class
     */
    const QString getHumanLabel() const override;

    /**
     * @brief setupFilterParameters Reimplemented from @see AbstractFilter class
     */
    void setupFilterParameters() override;

    /**
     * @brief execute Reimplemented from @see AbstractFilter class
     */
    void execute() override;

    /**
    * @brief preflight Reimplemented from @see AbstractFilter class
    */
    void preflight() override;

signals:
  /**
   * @brief updateFilterParameters Emitted when the Filter requests all the latest Filter parameters
   * be pushed from a user-facing control (such as a widget)
   * @param filter Filter instance pointer
   */
  void updateFilterParameters(AbstractFilter* filter);

  /**
   * @brief parametersChanged Emitted when any Filter parameter is changed internally
   */
  void parametersChanged();

  /**
   * @brief preflightAboutToExecute Emitted just before calling dataCheck()
   */
  void preflightAboutToExecute();

  /**
   * @brief preflightExecuted Emitted just after calling dataCheck()
   */
  void preflightExecuted();

protected:
  PostSlackMessage();

  /**
  * @brief dataCheck Checks for the appropriate parameter values and availability of arrays
  */
  void dataCheck();

  /**
  * @brief Initializes all the private instance variables.
  */
  void initialize();

private:
  QWaitCondition m_WaitCondition;
  QMutex m_Mutex;
  QNetworkAccessManager* m_NetworkManager;

  public:
    PostSlackMessage(const PostSlackMessage&) = delete; // Copy Constructor Not Implemented
    PostSlackMessage(PostSlackMessage&&) = delete;      // Move Constructor Not Implemented
    PostSlackMessage& operator=(const PostSlackMessage&) = delete; // Copy Assignment Not Implemented
    PostSlackMessage& operator=(PostSlackMessage&&) = delete;      // Move Assignment
};

