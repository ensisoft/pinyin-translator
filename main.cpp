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

#include "config.h"
#include "warnpush.h"
#  include <QtGui/QApplication>
#  include <QtGui/QMessageBox>
#include "warnpop.h"
#include <stdexcept>
#include <iostream>

#include "mainwindow.h"

int main(int argc, char* argv[])
{
    QApplication app(argc, argv);

    // have to create a window so that if there's an error and we're
    // trying to show the error message box we have  parent window
    // where to place the message. otherwise the user has no context
    // where the message popped up. (i think it makes more sense this way)
    pime::MainWindow window;
    try
    {
        window.show();
        window.loadData();        
        return app.exec();
    }
    catch (const std::exception& e) 
    {
        QMessageBox msg(&window);
        msg.setIcon(QMessageBox::Critical);
        msg.setText(QString::fromUtf8(e.what()));
        msg.setWindowTitle(window.windowTitle());
        msg.setStandardButtons(QMessageBox::Ok);
        msg.exec();
    }
    return 0;

}