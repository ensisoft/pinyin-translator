// Copyright (c) 2010-2014 Sami Väisänen, Ensisoft 
//
// http://www.ensisoft.com
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
//  of this software and associated documentation files (the "Software"), to deal
//  in the Software without restriction, including without limitation the rights
//  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
//  copies of the Software, and to permit persons to whom the Software is
//  furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
//  all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
//  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
//  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
//  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
//  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
//  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
//  THE SOFTWARE.

#pragma once

#include "config.h"

#include "warnpush.h"
#  include <QtGui/QDialog>
#  include "ui_dlgword.h"
#include "warnpop.h"

namespace pime
{
    class DlgWord : public QDialog
    {
        Q_OBJECT

    public:
        DlgWord(QFont font, QWidget* parent) : QDialog(parent)
        {
            ui_.setupUi(this);
            ui_.editPinyin->setFocus();
            ui_.editSimplified->setFont(font);
            ui_.editTraditional->setFont(font);
        }

        DlgWord(QFont font, QWidget* parent,
            const QString& pinyin,
            const QString& traditional,
            const QString& simplified,
            const QString& desc) : QDialog(parent)
        {
            ui_.setupUi(this);
            ui_.editPinyin->setText(pinyin);
            ui_.editTraditional->setText(traditional);            
            ui_.editTraditional->setFont(font);
            ui_.editSimplified->setText(simplified);
            ui_.editSimplified->setFont(font);
            ui_.editDescription->setText(desc);
            ui_.editDescription->setCursorPosition(0);
        }

       ~DlgWord()
        {}

        QString traditional() const 
        {
            return ui_.editTraditional->text();
        }
        QString simplified() const 
        {
            return ui_.editSimplified->text();
        }

        QString pinyin() const 
        {
            return ui_.editPinyin->text();
        }

        QString desc() const 
        {
            return ui_.editDescription->text();
        }

    private slots:
        void on_btnAccept_clicked()
        {
            const auto& pinyin = ui_.editPinyin->text();
            if (pinyin.isEmpty())
            {
                ui_.editPinyin->setFocus();
                return;
            }
            const auto& trad = ui_.editTraditional->text();
            if (trad.isEmpty())
            {
                ui_.editTraditional->setFocus();
                return;
            }
            const auto& simp = ui_.editSimplified->text();
            if (simp.isEmpty())
            {
                ui_.editSimplified->setFocus();
                return;
            }

            accept();
        }

        void on_btnCancel_clicked()
        {
            reject();
        }

    private:
        Ui::DlgWord ui_;
    };
} // pime