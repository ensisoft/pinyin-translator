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

#include "config.h"

#include <algorithm>
#include <cstdlib>
#include "game.h"
#include "level.h"

namespace invaders
{

Game::Game(unsigned width, unsigned height) : tick_(0), width_(width), height_(height), identity_(1), level_(nullptr)
{}

Game::~Game()
{}

void Game::tick()
{
    if (!level_)
       return;

    // prune invaders
    auto end = std::partition(std::begin(invaders_), std::end(invaders_),
        [&](const invader& i) {
            return i.xpos != 0;
        });
    for (auto it = end; it != std::end(invaders_); ++it)
    {
        on_invader_victory(*it);
    }

    invaders_.erase(end, std::end(invaders_));

    // update live invaders
    for (auto& i : invaders_)
    {
        i.xpos -= 1;
    }

    // spawn invaders
    const auto spawn_count = level_->spawnCount();
    const auto spawn_inter = level_->spawnInterval();
    if (!(tick_ % spawn_inter))
    {
        for (auto i=0; i<spawn_count; ++i)
        {
            const auto enemy = level_->spawn();
            //if (enemy.character == 0)
                //break;
            invader inv;
            inv.killstring = enemy.killstring;
            inv.string     = enemy.string;
            inv.score      = enemy.score;            
            inv.ypos       = std::rand() % height_;
            inv.xpos       = width_ + i;
            inv.identity   = identity_++;
            invaders_.push_back(inv);
            on_invader_spawn(inv);
        }
    }

    ++tick_;
}

void Game::fire(const Game::missile& missile)
{
    auto it = std::find_if(std::begin(invaders_), std::end(invaders_),
        [&](const invader& i) {
            return i.killstring == missile.string;
        });
    if (it == std::end(invaders_))
        return;

    auto inv = *it;

    score_ += inv.score;

    on_invader_kill(*it, missile);

    invaders_.erase(it);
}

void Game::play(Level& level)
{
    invaders_.clear();
    level_  = &level;
    tick_   = 0;
    score_  = 0;
}

} // invaders