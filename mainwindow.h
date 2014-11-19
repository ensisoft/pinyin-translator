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
#  include <QtGui/QMainWindow>
#  include <QObject>
#  include "ui_mainwindow.h"
#include "warnpop.h"
#include <list>
#include "dictionary.h"

namespace pime
{
    class MainWindow : public QMainWindow
    {
        Q_OBJECT

    public:
        MainWindow();
       ~MainWindow();

    private slots:
        void on_actionExit_triggered();
        void on_actionNewWord_triggered();
        void on_actionNewText_triggered();
        void on_actionDictionary_triggered();
        void on_editInput_textEdited(const QString& text);
    private:
        bool eventFilter(QObject* reciver, QEvent* event) override;
        void translate(int index, const QString& key);
        void updateDictionary(const QString& key);
        void updateTranslation();

    private:
        struct token {
            QString pinyin;
            QString chinese;
        };
        std::list<token> line_;
        std::vector<dictionary::word> words_;

    private:
        Ui::MainWindow ui_;

    private:
        dictionary dic_; 
    };

} // pime