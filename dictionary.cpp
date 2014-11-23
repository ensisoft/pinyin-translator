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
#  include <QFile>
#  include <QTextStream>
#  include <QStringList>
#  include "pinyin.h"
#include "warnpop.h"
#include <stdexcept>

#include "dictionary.h"
#include "format.h"

QString make_dictionary_key(const QString& pinyin)
{
    std::wstring wide = pinyin.toStdWString();
    std::wstring ret;
    for (const auto& c : wide)
    {
        ret.push_back(pinyin::toneunmap(c));
    }
    return QString::fromStdWString(ret);
}

QString make_dictionary_syllable(const QString& key, int tone)
{
    std::wstring wide = key.toStdWString();
    std::wstring syllable = pinyin::make_pinyin_syllable(wide, tone);
    return QString::fromStdWString(syllable);
}

namespace pime
{

dictionary::dictionary() : wordguid_(1)
{}

dictionary::~dictionary()
{}

void dictionary::load(const QString& file, quint32 metakey)
{
    QFile io(file);
    if (!io.open(QIODevice::ReadOnly))
        throw std::runtime_error(utf8("open dictionary failed: _1", file));

    QTextStream stream(&io);
    stream.setCodec("UTF-8");
    while (!stream.atEnd())
    {
        const auto& line = stream.readLine();
        const auto& toks = line.split("|");
        const auto& key  = make_dictionary_key(toks[2]);
        const auto index = words_.size();        

        dictionary::word word;
        word.key         = key;
        word.traditional = toks[0];
        word.simplified  = toks[1];
        word.pinyin      = toks[2];
        word.description = toks[3];
        word.guid        = wordguid_++;
        word.meta        = metakey;
        word.erased      = false;

        // bool duplicate = false;
        // auto lower = words_.lower_bound(key);
        // auto upper = words_.upper_bound(key);
        // for (; lower != upper; ++lower)
        // {
        //     if (lower->second.pinyin == word.pinyin)
        //     {
        //         duplicate = true;
        //         break;
        //     }
        // }
        //if (!duplicate)
        words_.push_back(word);
        index_.insert(std::make_pair(key, index));
    }
}

void dictionary::save(const QString& file, quint32 metakey)
{
    QFile io(file);
    if (!io.open(QIODevice::WriteOnly | QIODevice::Truncate))
        throw std::runtime_error(utf8("save dictionary failed: _1", file));

    QTextStream stream(&io);
    stream.setCodec("UTF-8");

    auto beg = index_.begin();
    auto end = index_.end();
    for (; beg != end; ++beg)
    {
        const auto& key  = beg->first;
        const auto& word = words_[beg->second];
        if (word.meta != metakey)
            continue;
        if (word.erased)
            continue;
        stream << word.traditional << "|" << word.simplified << "|" << word.pinyin << "|" << word.description;
        stream << "\n";
    }
}

std::vector<const dictionary::word*> dictionary::lookup(const QString& key) const
{
    std::vector<const word*> ret;

    auto lower = index_.lower_bound(key);
    auto upper = index_.end();
    for (; lower != upper; ++lower)
    {
        const auto& k = lower->first;
        if (!k.startsWith(key))
            break;
        ret.push_back(&words_[lower->second]);
    }

    return ret;
}

std::vector<const dictionary::word*> dictionary::search(const QString& str) const
{
    std::vector<const word*> ret;

    auto beg = index_.begin();
    auto end = index_.end();
    for (; beg != end; ++beg)
    {
        const auto& definition = words_[beg->second].description;
        if (definition.indexOf(str) != -1)
            ret.push_back(&words_[beg->second]);
    }

    return ret;
}

std::vector<const dictionary::word*> dictionary::flatten() const
{
    std::vector<const word*> ret;

    for (const auto& pair : index_)
        ret.push_back(&words_[pair.second]);

    return ret;
}

bool dictionary::store(dictionary::word& word)
{
    Q_ASSERT(!word.traditional.isEmpty());
    Q_ASSERT(!word.simplified.isEmpty());
    Q_ASSERT(!word.pinyin.isEmpty());

    word.key = make_dictionary_key(word.pinyin);

    auto lower = index_.lower_bound(word.key);
    auto upper = index_.upper_bound(word.key);
    for (; lower != upper; ++lower)
    {
        auto& w = words_[lower->second];
        if (w.guid != word.guid)
            continue;

        w.traditional = word.traditional;
        w.simplified  = word.simplified;
        w.pinyin      = word.pinyin;
        w.description = word.description;
        qDebug() << "Updated word: " << word.key << "Pinyin: " << word.pinyin << "Ch: " << word.traditional;
        return true;
    }

    const auto index = words_.size();

    word.guid = wordguid_++;
    index_.insert(std::make_pair(word.key, index));
    words_.push_back(word);

    qDebug() << "Stored new word: " << word.key << "Pinyin: " << word.pinyin << " Ch: " << word.traditional;
    return false;
}

bool dictionary::erase(const dictionary::word& word)
{
    Q_ASSERT(!word.key.isEmpty());
    Q_ASSERT(word.guid);

    auto lower = index_.lower_bound(word.key);
    auto upper = index_.upper_bound(word.key);
    for (; lower != upper; ++lower)
    {
        const auto& w = words_[lower->second];
        if (w.guid != word.guid)
            continue;

        words_[lower->second].erased = true;
        index_.erase(lower);
        return true;
    }
    return false;
}

} // pime