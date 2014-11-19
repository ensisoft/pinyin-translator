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
#  include <QtGui/QKeyEvent>
#  include <QtGui/QClipboard>
#  include <QtDebug>
#  include <QDir>
#  include <QFileInfo>
#  include <QSettings>
#include "warnpop.h"
#include <stdexcept>
#include <algorithm>

#include "mainwindow.h"
#include "dlgword.h"
#include "dlgdictionary.h"

#define NOTE(x) \
    ui_.statusbar->showMessage(x, 5000)

namespace pime
{

MainWindow::MainWindow()
{
    ui_.setupUi(this);

    const auto& home = QDir::homePath();
    const auto& pime = home + "/.pime";
    const auto& file = pime + "/dictionary";

    QDir dir(pime);
    if (!dir.mkpath(pime))
        throw std::runtime_error("failed to create ~/.pime");

    if (QFileInfo(file).exists())
    {
        dic_.load(file);
        NOTE(QString("Loaded dictionary with %1 words").arg(dic_.wordCount()));
    }

    QSettings settings("Ensisoft", "Pime");
    const auto wwidth  = settings.value("window/width", width()).toInt();
    const auto wheight = settings.value("window/height", height()).toInt();
    const auto xpos   = settings.value("window/xpos", x()).toInt();
    const auto ypos   = settings.value("window/ypos", y()).toInt();

    move(xpos, ypos);
    resize(wwidth, wheight);

    ui_.editInput->installEventFilter(this);
}

MainWindow::~MainWindow()
{
    QSettings settings("Ensisoft", "Pime");
    settings.setValue("window/width", width());
    settings.setValue("window/height", height());
    settings.setValue("window/xpos", x());
    settings.setValue("window/ypos", y());

    const auto& home = QDir::homePath();
    const auto& pime = home + "/.pime";
    const auto& file = pime + "/dictionary";
    dic_.save(file);
}

void MainWindow::on_actionExit_triggered()
{
    close();
}

void MainWindow::on_actionNewWord_triggered()
{
    const auto key = ui_.editInput->text();

    DlgWord dlg(this, key);
    if (dlg.exec() == QDialog::Rejected)
        return;

    dictionary::word word;
    word.key         = dlg.key();
    word.chinese     = dlg.chinese();
    word.pinyin      = dlg.pinyin();
    word.description = dlg.desc();
    word.tags        = 0;
    dic_.store(word);

    NOTE(QString("Added %1").arg(word.chinese));

    updateDictionary(key);
}

void MainWindow::on_actionNewText_triggered()
{
    ui_.editInput->clear();
    ui_.editInput->setFocus();

    line_.clear();
    updateTranslation();    
    updateDictionary("");
}

void MainWindow::on_actionDictionary_triggered()
{
    DlgDictionary dlg(this, dic_);
    dlg.exec();
}


void MainWindow::on_editInput_textEdited(const QString& text)
{
    updateDictionary(text);
}

bool MainWindow::eventFilter(QObject* receiver, QEvent* event)
{
    if (receiver != ui_.editInput)
        return QMainWindow::eventFilter(receiver, event);
    if (event->type() != QEvent::KeyPress)
        return QMainWindow::eventFilter(receiver, event);        

    const auto* keypress = static_cast<const QKeyEvent*>(event);
    const auto input = ui_.editInput->text();

    int wordindex = 0;
    bool copy_to_clip = false;
    switch (keypress->key())
    {
        case Qt::Key_1: wordindex = 1; break;
        case Qt::Key_2: wordindex = 2; break;
        case Qt::Key_3: wordindex = 3; break;
        case Qt::Key_4: wordindex = 4; break;
        case Qt::Key_5: wordindex = 5; break;
        case Qt::Key_6: wordindex = 6; break;
        case Qt::Key_7: wordindex = 7; break;
        case Qt::Key_8: wordindex = 8; break;
        case Qt::Key_9: wordindex = 9; break;
        case Qt::Key_Space: wordindex = 1; break;
        case Qt::Key_Enter: wordindex = 1; copy_to_clip = true; break;
    }

    if (!wordindex)
        return QMainWindow::eventFilter(receiver, event);

    qDebug() << "List index: " << wordindex;
    qDebug() << "Input word: " << input;

    translate(wordindex-1, input);                
    updateTranslation();
    updateDictionary("");

    ui_.editInput->clear();

    if (copy_to_clip)
    {
        QString chinese = ui_.editChinese->text();
        QClipboard* clip = QApplication::clipboard();
        clip->setText(chinese);
    }
    return true;
}

void MainWindow::translate(int index, const QString& key)
{
    if (index >= words_.size())
    {
        token tok;
        tok.pinyin  = key;
        tok.chinese = key;
        line_.push_back(tok);
    }
    else
    {
        const auto& word = words_[index];
        token tok;
        tok.pinyin  = word.pinyin;
        tok.chinese = word.chinese;
        line_.push_back(tok);
    }
}

void MainWindow::updateDictionary(const QString& key)
{
    qDebug() << "Dictionary key: " << key;

    ui_.listWords->clear();

    words_ = dic_.lookup(key);
    for (size_t i=0; i<words_.size(); ++i)
    {
        const auto& word = words_[i];

        QListWidgetItem* item = new QListWidgetItem();
        item->setText(QString("%1.\t%2\t%3\t%4")
            .arg(i + 1)
            .arg(word.chinese)
            .arg(word.pinyin)
            .arg(word.description));
        ui_.listWords->addItem(item);
    }
}

void MainWindow::updateTranslation()
{
    QString chinese;
    QString pinyin;
    for (const auto& tok : line_)
    {
        chinese += tok.chinese;
        pinyin  += QString("%1 ").arg(tok.pinyin);
    }

    ui_.editPinyin->setText(pinyin);
    ui_.editChinese->setText(chinese);
}

} // pime