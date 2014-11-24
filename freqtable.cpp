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
#  include <QIODevice>
#  include <QTextStream>
#  include <QStringList>
#include "warnpop.h"
#include <stdexcept>

#include "freqtable.h"
#include "format.h"

// frequency table data from 
// http://lingua.mtsu.edu/chinese-computing/statistics/char/list.php?Which=MO
// copy/paste all text into a txt file and manually remove non-list lines from the top
// and then bottom and then simply use cut to create trimmed down character frequencly list.
// "cut -f 2,3 junda.txt > frequency.txt"

namespace pime
{

freqtable::freqtable()
{}

freqtable::~freqtable()
{}

void freqtable::load(const QString& file)
{
    QFile io(file);
    if (!io.open(QIODevice::ReadOnly))
        throw std::runtime_error(utf8("open frequence table failed: _1", file));

    QTextStream stream(&io);
    stream.setCodec("UTF-8");
    while (!stream.atEnd())
    {
        const auto& line  = stream.readLine();
        const auto& words = line.split("\t");
        if (words.size() != 2)
            throw std::runtime_error("unexpected frequency table data");

        const auto& word = words[0];
        const auto& freq = words[1].toLongLong();
        table_[word] = freq;
    }
}

} // pime
