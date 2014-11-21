// Copyright (c) 2014 Sami Väisänen, Ensisoft 
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
#  include <QString>
#include "warnpop.h"
namespace pime
{
    template<typename T>
    QString format(const T& value)
    {
        return QString("%1").arg(value);
    }

    namespace detail {
        inline
        QString key(int index)
        {
            return QString("_%1").arg(index);
        }

        template<typename T>
        void format_arg(QString& s, int index, const T& t)
        {
            const auto& str = pime::format(t);
            s = s.replace(key(index), str);
        }

        template<typename T, typename... Rest>
        void format_arg(QString& s, int index, const T& t, const Rest&... rest)
        {
            const auto& str = pime::format(t);
            s = s.replace(key(index), str);
            format_arg(s, index + 1, rest...);
        }

    } // detail

template<typename... Args>
QString str(const char* fmt, const Args&... args)
{
    QString s(fmt);
    detail::format_arg(s, 1, args...);
    return s;
}

template<typename... Args>
std::string utf8(const char* fmt, const Args&... args)
{
    QString ret = str(fmt, args...);
    QByteArray bytes = ret.toUtf8();
    return std::string(bytes.constBegin(), bytes.size());
}


} // pime