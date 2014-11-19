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
#include "warnpop.h"
#include <stdexcept>

#include "dictionary.h"

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
        throw std::runtime_error("open dictionary failed");

    QTextStream stream(&io);
    stream.setCodec("UTF-8");
    while (!stream.atEnd())
    {
        const auto& line = stream.readLine();
        const auto& toks = line.split("|");
        const auto& key  = toks[0];

        dictionary::word word;
        word.key         = key;
        word.chinese     = toks[1];
        word.pinyin      = toks[2];
        word.description = toks[3];
        word.guid        = wordguid_++;
        word.meta        = metakey;

        bool duplicate = false;
        auto lower = words_.lower_bound(key);
        auto upper = words_.upper_bound(key);
        for (; lower != upper; ++lower)
        {
            if (lower->second.pinyin == word.pinyin)
            {
                duplicate = true;
                break;
            }
        }
        if (!duplicate)
            words_.insert(std::make_pair(key, word));
    }
}

void dictionary::save(const QString& file, quint32 metakey)
{
    QFile io(file);
    if (!io.open(QIODevice::WriteOnly | QIODevice::Truncate))
        throw std::runtime_error("open dictionary failed");

    QTextStream stream(&io);
    stream.setCodec("UTF-8");

    auto beg = words_.begin();
    auto end = words_.end();
    for (; beg != end; ++beg)
    {
        const auto& key  = beg->first;
        const auto& word = beg->second;
        if (word.meta != metakey)
            continue;
        stream << key << "|" << word.chinese << "|" << word.pinyin << "|" << word.description << "\n";
    }
}

std::vector<dictionary::word> dictionary::lookup(const QString& key) const
{
    std::vector<word> ret;

    // auto lower = words_.lower_bound(key);
    // auto upper = words_.upper_bound(key);
    // for (; lower != upper; ++lower)
    //     ret.push_back(lower->second);

    auto lower = words_.lower_bound(key);
    auto upper = words_.end();
    for (; lower != upper; ++lower)
    {
        const auto& k = lower->first;
        if (!k.startsWith(key))
            break;
        ret.push_back(lower->second);
    }

    return ret;
}

std::vector<dictionary::word> dictionary::search(const QString& str) const
{
    std::vector<word> ret;

    auto beg = words_.begin();
    auto end = words_.end();
    for (; beg != end; ++beg)
    {
        const auto& definition = beg->second.description;
        if (definition.indexOf(str) != -1)
            ret.push_back(beg->second);
    }

    return ret;
}

std::vector<dictionary::word> dictionary::flatten() const
{
    std::vector<word> ret;

    for (auto pair : words_)
        ret.push_back(pair.second);

    return ret;
}

bool dictionary::store(dictionary::word& word)
{
    Q_ASSERT(!word.key.isEmpty());
    Q_ASSERT(!word.chinese.isEmpty());
    Q_ASSERT(!word.pinyin.isEmpty());

    auto lower = words_.lower_bound(word.key);
    auto upper = words_.upper_bound(word.key);
    for (; lower != upper; ++lower)
    {
        const auto& w = lower->second;
        if (w.guid != word.guid)
            continue;

        lower->second.chinese = word.chinese;
        lower->second.pinyin  = word.pinyin;
        lower->second.description = word.description;
        qDebug() << "Updated word: " << word.key << "Pinyin: " << word.pinyin << "Ch: " << word.chinese;
        return true;
    }

    word.guid = wordguid_++;
    words_.insert(std::make_pair(word.key, word));
    qDebug() << "Stored new word: " << word.key << "Pinyin: " << word.pinyin << " Ch: " << word.chinese;
    return false;
}

bool dictionary::erase(const dictionary::word& word)
{
    Q_ASSERT(!word.key.isEmpty());
    Q_ASSERT(word.guid);

    auto lower = words_.lower_bound(word.key);
    auto upper = words_.upper_bound(word.key);
    for (; lower != upper; ++lower)
    {
        const auto& w = lower->second;
        if (w.guid != word.guid)
            continue;

        words_.erase(lower);
        return true;
    }
    return false;
}

} // pime