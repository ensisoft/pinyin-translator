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
#  include <QtDebug>
#  include <QDir>
#  include <QFileInfo>
#  include <QSettings>
#  include <QtGui/QKeyEvent>
#include "warnpop.h"
#include <stdexcept>
#include <algorithm>

#include "mainwindow.h"
#include "dlgword.h"

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

    ui_.editPinyin->installEventFilter(this);
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

void MainWindow::on_btnAddWord_clicked()
{
    DlgWord dlg(this, key_);

    if (dlg.exec() == QDialog::Accepted)
    {
        dictionary::word word;
        word.chinese = dlg.chinese();
        word.pinyin  = dlg.pinyin();
        word.description = dlg.desc();

        auto key = word.pinyin.toAscii();

        qDebug() << "New word key: " << key;

        dic_.store(key, word);

        NOTE(QString("Added %1").arg(word.chinese));

        updateDictionary(key_);
    }
}

void MainWindow::on_btnEditWord_clicked()
{

}


void MainWindow::on_actionExit_triggered()
{
    close();
}

void MainWindow::on_actionAddWord_triggered()
{
    on_btnAddWord_clicked();
}

void MainWindow::on_editPinyin_textEdited(const QString& text)
{
    if (text.isEmpty())
    {
        if (!line_.empty())
        {
            const auto& back = line_.back();
            key_ = back.key;
        }
    }
    else
    {
        key_ = text;
    }
    updateDictionary(key_);
    updateTranslation();
}

bool MainWindow::eventFilter(QObject* receiver, QEvent* event)
{
    if (receiver != ui_.editPinyin)
        return QMainWindow::eventFilter(receiver, event);
    if (event->type() != QEvent::KeyPress)
        return QMainWindow::eventFilter(receiver, event);        

    const auto* key = static_cast<const QKeyEvent*>(event);

    int translation_index = 0;
    switch (key->key())
    {
        case Qt::Key_1: translation_index = 1; break;
        case Qt::Key_2: translation_index = 2; break;
        case Qt::Key_3: translation_index = 3; break;
        case Qt::Key_4: translation_index = 4; break;
        case Qt::Key_5: translation_index = 5; break;
        case Qt::Key_6: translation_index = 6; break;
        case Qt::Key_7: translation_index = 7; break;
        case Qt::Key_8: translation_index = 8; break;
        case Qt::Key_9: translation_index = 9; break;
    }

    if (!translation_index)
        return QMainWindow::eventFilter(receiver, event);

    qDebug() << "Keyboard translation_index: " << translation_index;
    qDebug() << "Pinyin word: " << key_;
    translate(translation_index-1);                
    key_.clear();
    ui_.editPinyin->clear();

    updateDictionary(key_);
    updateTranslation();
    return true;
}

void MainWindow::translate(int index)
{
    if (index >= words_.size())
        return;

    const auto& word = words_[index];

    QString pinyin  = key_;
    QString chinese = word.chinese;

    token tok;
    tok.key = pinyin;
    tok.chinese = chinese;
    line_.push_back(tok);
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
        item->setText(QString("%1. %2\t%3")
            .arg(i + 1)
            .arg(word.chinese)
            .arg(word.description));
        ui_.listWords->addItem(item);
    }
}

void MainWindow::updateTranslation()
{
    // concatenate chinese
    QString text;
    for (const auto& tok : line_)
        text += tok.chinese;

    ui_.editChinese->setText(text);
}

} // pime