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

#include "../config.h"
#include "../warnpush.h"
#  include <boost/spirit/include/classic.hpp>
#  include "../pinyin.h"
#include "../warnpop.h"


#include <string>
#include <iostream>
#include <fstream>
#include <stack>
#include <map>
#include "utf8.h"

struct word {
    std::string traditional;
    std::string simplified;
    std::string pinyin;
    std::string definition;
};

bool parse_word(const std::string& line, word& w)
{
    using namespace boost::spirit::classic;

    chset<> trad_p(chset<>(anychar_p) - '\r' - '\n' - ' ');
    chset<> simp_p(chset<>(anychar_p) - '\r' - '\n' - ' ');    
    chset<> pinyin_p(chset<>(anychar_p) - '\r' - '\n' - ']');    

    const auto ret = parse(std::begin(line), std::end(line),
        (
            (*trad_p)[assign(w.traditional)] >> ch_p(' ') >>
            (*simp_p)[assign(w.simplified)] >> ch_p(' ')  >>
            (ch_p('[') >> (*pinyin_p)[assign(w.pinyin)] >> ch_p(']')) >> ch_p(' ') >> !ch_p('/') >>
            (*anychar_p)[assign(w.definition)]
        ));
    return ret.hit;
}

std::string make_dictionary_key(std::string pinyin)
{
    auto it = std::remove_if(std::begin(pinyin), std::end(pinyin),
        [&](char c) {
            return std::isdigit(c) || std::isspace(c);
        });
    pinyin.erase(it, std::end(pinyin));
    return pinyin;
}

std::string make_dictionary_pinyin(std::string pinyin)
{
    // transform something like "ban4 fa3" into "bànfǎ"
    // the algorithm for placing the tone mark in the syllable is
    // as follows:
    // if there's an "a" or an "e" it will take the tone mark
    // if there's an "ou", then the "o" takes the tone mark
    // else the second vowel takes the tone mark


    std::wstring wide = utf8::decode(pinyin);
    std::wstring ret;
    for (auto it = wide.begin(); it != wide.end(); ++it)
    {
        if (std::isspace(*it))
            continue;

        std::wstring syllable;
        int tonepos = -1;
        for (; it != wide.end(); ++it)
        {
            const auto next_letter = *it;
            const auto prev_letter = syllable.empty() ? 0 : syllable.back();
           // each syllable is terminated by a tone mark            
            if (std::isdigit(next_letter))
            {
                const auto tone = next_letter - '0';
                // the CEDIC dictionary format uses 5 tones but the 5th tone 
                // is the neutral tone and is simply represented by the "toneless" vowel
                assert(tone >= 1);
                assert(tone <= 5);

                if (tonepos == -1)
                {
                    // take the last vowel
                    for (tonepos = syllable.size()-1; tonepos > 0; --tonepos)
                    {
                        if (pinyin::is_vowel(syllable[tonepos]))
                            break;
                    }
                }
                assert(tonepos >= 0);
                assert(tonepos < syllable.size());
                syllable[tonepos] = pinyin::tonemap(syllable[tonepos], tone);
                break;
            }
            else if (next_letter == ':')
            {
                assert(!syllable.empty());
                assert(syllable.back() == 'u');
                syllable.pop_back();
                syllable.push_back(pinyin::u_diaresis_latin);
            }
            else
            {
                syllable.push_back(next_letter);
                if (tonepos == -1)
                {
                    if ((next_letter == pinyin::a_latin) || (next_letter == pinyin::A_latin))
                    {
                        // a takes the tone mark
                        tonepos = syllable.size() - 1;
                    }
                    if ((next_letter == pinyin::e_latin) || (next_letter == pinyin::E_latin))
                    {
                        // e takes the tone mark
                        tonepos = syllable.size() - 1;
                    }
                    if ((next_letter == pinyin::u_latin) || (next_letter == pinyin::U_latin))
                    {
                        // if there's on ou then the o takes the tone mark
                        if ((prev_letter == pinyin::o_latin) || (prev_letter == pinyin::O_latin))
                            tonepos = syllable.size() - 2;
                    }
                }
            }
        }
        ret.append(syllable);
        if (it == wide.end())
            break;
    }
    return utf8::encode(ret);
}

std::string make_dictionary_definition(std::string s)
{
    assert(s.back() == '/');
    s.pop_back();

    std::string ret;
    for (const auto& c : s)
    {
        if (c == '/')
        {
            ret.append(" / ");
        }
        else ret.push_back(c);
    }
    return ret;
}

void process_word(word w, std::ostream& out)
{
    const auto& key  = make_dictionary_key(w.pinyin);
    const auto& pin  = make_dictionary_pinyin(w.pinyin);
    const auto& def  = make_dictionary_definition(w.definition);
    const auto& trad = w.traditional;
    const auto& simp = w.simplified;

    out << trad << "|" << simp << "|" << pin << "|" << def;
    out << "\n";
}

int main(int argc, char* argv[])
{
    //std::cout << make_dictionary_pinyin("chao3") << "\n";
    //std::cout << make_dictionary_pinyin("chao2") << "\n";   
    //std::cout << make_dictionary_pinyin("mei3 guo2") << "\n";
    //return 0;

    if (argc < 3)
    {
        std::cerr << "Incorrect parameters\n";
        std::cerr << "cedict input-file output-file\n";
        return 1;
    }

    std::string input = argv[1];
    std::ifstream in(input, std::ios::binary);
    if (!in.is_open())
    {
        std::cerr << "Failed to open: " << input;
        return 1;
    }

    std::string output = argv[2];
    std::ofstream out(output, std::ios::binary | std::ios::trunc);
    if (!out.is_open())
    {
        std::cerr << "Failed to open: " << output;
        return 1;
    }


    std::string line;
    std::size_t lineno = 0;
    while (!in.eof())
    {
        std::getline(in, line);
        lineno++;
        if (!(lineno % 100))
        {
            std::cout << "\rAt line " << lineno;
            std::cout.flush();
        }

        if (line.empty())
            continue;
        if (line[0] == '#')
            continue;

        if (line.back() == '\r')
            line.pop_back();

        word w;
        if (!parse_word(line, w))
        {
            std::cerr << "Failed to parse: " << line;
            return 1;
        }
        process_word(std::move(w), out);
    }

    return 0;
}