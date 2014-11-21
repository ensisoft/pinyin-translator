// Copyright (c) 2010-2013 Sami Väisänen, Ensisoft 
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

// $Id: utf8.h,v 1.8 2010/07/25 12:59:51 svaisane Exp $

#pragma once

#include <cstdint>
#include <string>
#include <iterator>

namespace utf8
{
    namespace detail {
        template<typename T>
        struct integer_type;
        
        template<>
        struct integer_type<int>
        {
            typedef unsigned int unsigned_type;
        };

        template<>
        struct integer_type<long>
        {
            typedef unsigned long unsigned_type;
        };
        
        template<>
        struct integer_type<unsigned long>
        {
            typedef unsigned long unsigned_type;
        };
        
        template<>
        struct integer_type<unsigned int>
        {
            typedef unsigned int unsigned_type;
        };

        template<>
        struct integer_type<char>
        {
            typedef unsigned char unsigned_type;
        };
        template<>
        struct integer_type<wchar_t>
        {
            //typedef unsigned wchar_t unsigned_type;
            typedef unsigned short unsigned_type;
        };
    };

    // encode the range specified by beg and end iterators 
    // as an utf8 encoded byte stream into the output iterator dest.
    template<typename InputIterator, typename OutputIterator>
    void encode(InputIterator beg, InputIterator end, OutputIterator dest)
    {
        typedef typename std::iterator_traits<InputIterator>::value_type value_type;
        typedef typename detail::integer_type<value_type>::unsigned_type unsigned_type;

        // Unicode conversion table
        // number range (4 bytes)| binary representation (octets) 
        // -----------------------------------------------------------
        // 0000 0000 - 0000 007F | 0xxxxxxx                 (US-ASCII) 
        // 0000 0080 - 0000 07FF | 110xxxxx 10xxxxxx
        // 0000 0800 - 0000 FFFF | 1110xxxx 10xxxxxx 10xxxxxx
        // 0001 0000 - 0010 FFFF | 11110xxx 10xxxxxx 10xxxxxx 10xxxxxx
        while (beg != end)
        {
            // maximum size for a Unicode character is 4 bytes
            const uint32_t wchar = static_cast<unsigned_type>(*beg++);

            if (wchar <= 0x007F)
            {
                *dest++ = static_cast<char>(wchar);
            }
            else if (wchar >= 0x0080 &&  wchar <= 0x07FF)
            {
                *dest++ = static_cast<char>((wchar >> 6)   | 0xC0);
                *dest++ = static_cast<char>((wchar & 0x3F) | 0x80);
            }
            else if (wchar >= 0x0800 && wchar <= 0xFFFF)
            {
                *dest++ = static_cast<char>((wchar >> 12)  | 0xE0);
                *dest++ = static_cast<char>(((wchar >> 6)  & 0x3F) | 0x80);
                *dest++ = static_cast<char>((wchar & 0x3F) | 0x80);
            }
            else
            {
                *dest++ = static_cast<char>((wchar >> 18) | 0xF0);
                *dest++ = static_cast<char>(((wchar >> 12) & 0x3F) | 0x80);
                *dest++ = static_cast<char>(((wchar >>  6) & 0x3F) | 0x80);
                *dest++ = static_cast<char>((wchar & 0x3F) | 0x80);
            }
        } // while
    }

    // convenience function to encode (extended) ascii string to utf8
    inline std::string encode(const std::string& ascii)
    {
        std::string utf8;
        encode(ascii.begin(), ascii.end(), 
            std::back_inserter(utf8));
        return utf8;
    }

    // convenience function to encode a wide unicode string to utf8
    inline std::string encode(const std::wstring& unicode)
    {
        std::string utf8;
        encode(unicode.begin(), unicode.end(),
            std::back_inserter(utf8));
        return utf8;
    }
    

    template<typename WideChar, typename InputIterator, typename OutputIterator>
    InputIterator decode(InputIterator beg, InputIterator end, OutputIterator dest)
    {
        //typedef typename std::iterator_traits<OutputIterator>::value_type wide_t;
        typedef WideChar wide_t;

        InputIterator pos;

        while (beg != end)
        {
            pos = beg;
            wide_t wide = 0;
            switch (*beg & 0xF0)
            { 
                case 0xF0: // 4 byte sequence
                    if (sizeof(wide_t) < 4)
                        return pos;
                    wide |= wide_t(0x03 & *beg) << 3;
                    for (int i=2; i>=0; --i)
                    {
                        if (++beg == end)
                            return pos;
                        wide |= wide_t(0x3F & *beg) << i*6;
                    }
                    *dest++ = wide;
                    break;

                case 0xE0: // 3 byte sequence (fits in 16 bits)
                    if (sizeof(wide_t) < 2)
                        return pos;
                    wide |= wide_t(0x0F & *beg) << 12;
                    if (++beg == end)
                        return pos;
                    wide |= wide_t(0x3F & *beg) << 6;
                    if (++beg == end)
                        return pos;
                    wide |= 0x3F & *beg;
                    *dest++ = wide;
                    break;

                case 0xC0: // 2 byte sequence
                case 0xD0:
                    if (sizeof(wide_t) < 2)
                        return pos;
                    wide |= wide_t(0x1F & *beg) << 6;
                    if (++beg == end)
                        return pos;
                    wide |= 0x3F & *beg;
                    if (wide < 0x7F)
                        return pos; // illegal sequence, multibyte but just ascii
                    *dest++ = wide;
                    break;

                default:   // 1 byte sequnce (ascii)
                    *dest++ = *beg;
                    break;
            }
            ++beg;
        }
        return beg;
    }

    inline std::wstring decode(const std::string& utf8)
    {
        std::wstring ret;
        decode<wchar_t>(utf8.begin(), utf8.end(), 
            std::back_inserter(ret));
        return ret;
    }

} // utf8



