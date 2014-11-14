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
#  include <QFile>
#  include <QTextStream>
#  include <QStringList>
#include "warnpop.h"
#include <stdexcept>

#include "dictionary.h"

namespace pime
{

dictionary::dictionary(const QString& file)
{
    load(file);
}

dictionary::dictionary()
{}

dictionary::~dictionary()
{}

void dictionary::load(const QString& file)
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
        word.chinese     = toks[1];
        word.pinyin      = toks[2];
        word.description = toks[3];
        words_.insert(std::make_pair(key, word));
    }
}

void dictionary::save(const QString& file)
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
        stream << key << "|" << word.chinese << "|" << word.pinyin << "|" << word.description << "\n";
    }
}

std::vector<dictionary::word> dictionary::lookup(const QString& key)
{
    auto lower = words_.lower_bound(key);
    auto upper = words_.upper_bound(key);

    std::vector<word> ret;
    for (; lower != upper; ++lower)
        ret.push_back(lower->second);

    return ret;
}

void dictionary::store(const QString& key, const word& w)
{
    words_.insert(std::make_pair(key, w));
}

} // pime