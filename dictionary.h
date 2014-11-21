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
#  include <QString>
#include "warnpop.h"

#include <vector>
#include <map>

namespace pime
{
    class dictionary
    {
    public:
        struct word {
            QString key;
            QString traditional;
            QString simplified;
            QString pinyin;
            QString description;
            quint32 meta;
            quint32 guid;
            bool erased;
        };

        dictionary();
       ~dictionary();

        // load a dictionary file
        void load(const QString& file, quint32 metakey);

        // save the in memory contents of the dictionary to a file
        void save(const QString& file, quint32 metakey);

        // lookup a list of words with the given key in the dictionary.
        std::vector<const word*> lookup(const QString& key) const;

        std::vector<const word*> lookup(const QString& key, int tone) const;

        // search the definitions of the word for the given substring
        // and return those that match.
        std::vector<const word*> search(const QString& str) const;

        // flatten the whole dictionary into a list.
        std::vector<const word*> flatten() const;

        // store a word in the dictionary.
        // if the word indentified by the given key and guid already exists
        // then it's definition is updated. otherwise it's stored as a new word.
        // returns true if word was modified, false if inserted.
        bool store(dictionary::word& word);

        bool erase(const dictionary::word& word);

        // return the number of words in the dictionary.
        std::size_t wordCount() const 
        { return words_.size(); }
    private:
        quint32 wordguid_;

    private:
        std::multimap<QString, std::size_t> index_;
        std::vector<word> words_;

    };
} // pime