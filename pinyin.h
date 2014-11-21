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

#include <map>

namespace pinyin
{

// definitions in unicode for characters used in pinyin
// http://unicodelookup.com
// http://en.wikipedia.org/wiki/Pinyin

// 1. The first tone (Flat or High Level Tone) is represented by a macron (ˉ) added to the pinyin wovel:
// ā ē ī ō ū ǖ
const wchar_t a_macron = 0x0101;
const wchar_t e_macron = 0x0113;
const wchar_t i_macron = 0x012b;
const wchar_t o_macron = 0x014d;
const wchar_t u_macron = 0x016b;
const wchar_t u_diaresis_macron = 0x01d6;

const wchar_t A_macron = 0x0100;
const wchar_t E_macron = 0x0112;
const wchar_t I_macron = 0x012a;
const wchar_t O_macron = 0x014c;
const wchar_t U_macron = 0x016a;
const wchar_t U_diaresis_macron = 0x01d5;

// 2. The second tone (Rising or High Rising tone) is denoted by an acute accent (ˊ):
// á é í ó ú ǘ
const wchar_t a_acute = 0x00e1;
const wchar_t e_acute = 0x00e9;
const wchar_t i_acute = 0x00ed;
const wchar_t o_acute = 0x00f3;
const wchar_t u_acute = 0x00fa;
const wchar_t u_diaresis_acute = 0x01d8;

const wchar_t A_acute = 0x00c1;
const wchar_t E_acute = 0x00c9;
const wchar_t I_acute = 0x00cd;
const wchar_t O_acute = 0x00d3;
const wchar_t U_acute = 0x00da;
const wchar_t U_diaresis_acute = 0x01d7;


// 3. The third tone (Falling-Rising or Low Tone) is maked by a charon (ˇ)
// ǎ ě ǐ ǒ ǔ ǚ
const wchar_t a_charon = 0x01ce;
const wchar_t e_charon = 0x011b;
const wchar_t i_charon = 0x01d0;
const wchar_t o_charon = 0x01d2;
const wchar_t u_charon = 0x01d4;
const wchar_t u_diaresis_charon = 0x01da;

const wchar_t A_charon = 0x01cd;
const wchar_t E_charon = 0x011a;
const wchar_t I_charon = 0x01cf;
const wchar_t O_charon = 0x01d1;
const wchar_t U_charon = 0x01d3;
const wchar_t U_diaresis_charon = 0x01d9;

// 4. The fourth tone (Falling or High-Falling) tone is represented by grave accent (ˋ)
// à è ì ò ù ǜ
const wchar_t a_grave = 0x00e0;
const wchar_t e_grave = 0x00e8;
const wchar_t i_grave = 0x00ec;
const wchar_t o_grave = 0x00f2;
const wchar_t u_grave = 0x00f9;
const wchar_t u_diaresis_grave = 0x01dc;

const wchar_t A_grave = 0x00c0;
const wchar_t E_grave = 0x00c8;
const wchar_t I_grave = 0x00cc;
const wchar_t O_grave = 0x00d2;
const wchar_t U_grave = 0x00d9;
const wchar_t U_diaresis_grave = 0x01db;

// pinyin vowels
const wchar_t a_latin = 0x0061;
const wchar_t e_latin = 0x0065;
const wchar_t i_latin = 0x0069;
const wchar_t o_latin = 0x006f;
const wchar_t u_latin = 0x0075;
const wchar_t u_diaresis_latin = 0x00fc;

const wchar_t A_latin = 0x0041;
const wchar_t E_latin = 0x0045;
const wchar_t I_latin = 0x0049;
const wchar_t O_latin = 0x004f;
const wchar_t U_latin = 0x0055;
const wchar_t U_diaresis_latin = 0x00dc;

// map a pinyin vowel letter with the given tone to 
// another letter used to represent the combination of the pinyin vowel 
// and the tone. so for example a, 1 -> ā  e, 2 -> é
static 
wchar_t tonemap(wchar_t vowel, unsigned tone)
{
    static const std::map<wchar_t, wchar_t> lower_case = 
    {
        // first tone
        {a_latin + 1, a_macron},
        {e_latin + 1, e_macron},
        {i_latin + 1, i_macron},
        {o_latin + 1, o_macron},
        {u_latin + 1, u_macron},
        {u_diaresis_latin + 1, u_diaresis_macron},

        // second tone
        {a_latin + 2, a_acute},
        {e_latin + 2, e_acute},
        {i_latin + 2, i_acute},
        {o_latin + 2, o_acute},
        {u_latin + 2, u_acute},
        {u_diaresis_latin + 2, u_diaresis_acute},

        // third tone
        {a_latin + 3, a_charon},
        {e_latin + 3, e_charon},
        {i_latin + 3, i_charon},
        {o_latin + 3, o_charon},
        {u_latin + 3, u_charon},
        {u_diaresis_latin + 3, u_diaresis_charon},

        // fourth tone
        {a_latin + 4, a_grave},
        {e_latin + 4, e_grave},
        {i_latin + 4, i_grave},
        {o_latin + 4, o_grave},
        {u_latin + 4, u_grave},
        {u_diaresis_latin + 4, u_diaresis_grave}
    };

    const auto lower = lower_case.find(vowel + tone);
    if (lower != std::end(lower_case))
        return lower->second;

    static const std::map<wchar_t, wchar_t> upper_case = 
    {
        // first tone
        {A_latin + 1, A_macron},
        {E_latin + 1, E_macron},
        {I_latin + 1, I_macron},
        {O_latin + 1, O_macron},
        {U_latin + 1, U_macron},
        {U_diaresis_latin + 1, U_diaresis_macron},

        // second tone
        {A_latin + 2, A_acute},
        {E_latin + 2, E_acute},
        {I_latin + 2, I_acute},
        {O_latin + 2, O_acute},
        {U_latin + 2, U_acute},
        {U_diaresis_latin + 2, U_diaresis_acute},

        // third tone
        {A_latin + 3, A_charon},
        {E_latin + 3, E_charon},
        {I_latin + 3, I_charon},
        {O_latin + 3, O_charon},
        {U_latin + 3, U_charon},
        {U_diaresis_latin + 3, U_diaresis_charon},

        // fourth tone
        {A_latin + 4, A_grave},
        {E_latin + 4, E_grave},
        {I_latin + 4, I_grave},
        {O_latin + 4, O_grave},
        {U_latin + 4, U_grave},
        {U_diaresis_latin + 4, U_diaresis_grave}
    };

    const auto upper = upper_case.find(vowel + tone);
    if (upper != std::end(upper_case))
        return upper->second;

    return vowel;
}

static
wchar_t toneunmap(wchar_t vowel)
{
    static const std::map<wchar_t, wchar_t> lower_case = {
        // first tone
        {a_macron, a_latin},
        {e_macron, e_latin},
        {i_macron, i_latin},
        {o_macron, o_latin},
        {u_macron, u_latin},
        {u_diaresis_macron, u_latin},

        // second tone.
        {a_acute, a_latin},
        {e_acute, e_latin},
        {i_acute, i_latin},
        {o_acute, o_latin},
        {u_acute, u_latin},
        {u_diaresis_acute, u_latin},

        // third tone
        {a_charon, a_latin},
        {e_charon, e_latin},
        {i_charon, i_latin},
        {o_charon, o_latin},
        {u_charon, u_latin},
        {u_diaresis_charon, u_latin},

        // fourth tone
        {a_grave, a_latin},
        {e_grave, e_latin},
        {i_grave, i_latin},
        {o_grave, o_latin},
        {u_grave, u_latin},
        {u_diaresis_grave, u_latin}
    };

    const auto lower = lower_case.find(vowel);
    if (lower != std::end(lower_case))
        return lower->second;

    static const std::map<wchar_t, wchar_t> upper_case = {
        // first tone
        {A_macron, A_latin},
        {E_macron, E_latin},
        {I_macron, I_latin},
        {O_macron, O_latin},
        {U_macron, U_latin},
        {U_diaresis_macron, U_latin},

        // second tone.
        {A_acute, A_latin},
        {E_acute, E_latin},
        {I_acute, I_latin},
        {O_acute, O_latin},
        {U_acute, U_latin},
        {U_diaresis_acute, U_latin},

        // third tone
        {A_charon, A_latin},
        {E_charon, E_latin},
        {I_charon, I_latin},
        {O_charon, O_latin},
        {U_charon, U_latin},
        {U_diaresis_charon, U_latin},

        // fourth tone
        {A_grave, A_latin},
        {E_grave, E_latin},
        {I_grave, I_latin},
        {O_grave, O_latin},
        {U_grave, U_latin},
        {U_diaresis_grave, U_latin}
    };    
    const auto upper = upper_case.find(vowel);
    if (upper != std::end(upper_case))
        return upper->second;

    return vowel;
}

static
bool is_vowel(wchar_t c)
{
    switch (c)
    {
        case a_latin:
        case e_latin:
        case i_latin:
        case o_latin:
        case u_latin:
        case u_diaresis_latin:
            return true;

        case A_latin:
        case E_latin:
        case I_latin:
        case O_latin:
        case U_latin:
        case U_diaresis_latin:
            return true;

        default: break;
    }
    return false;
}

} // pinyin