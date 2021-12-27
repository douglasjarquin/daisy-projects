/* 
 * Copyright (C) 2021 Evan Pernu - All Rights Reserved
 * You may use, distribute and modify this code under the
 * terms of the GNU AGPLv3 license.
 * 
 * You should have received a copy of the GNU AGPLv3 license with
 * this file (LICENSE.md). If not, please write to: evanpernu@gmail.com, 
 * or visit: https://www.gnu.org/licenses/agpl-3.0.en.html
 */

#include "arp.h"
#include "chord.h"
#include "theory.h"

using namespace jellybeans;

Arp::Arp() : Arp(8, "Up") {}

Arp::Arp(int theMaxSteps, std::string thePattern){
    resetState();
    maxSteps = theMaxSteps;
    dacValue = 0.f;
    pattern = thePattern;
    chord = new DiatonicChord();
    string = "EMPTY ARP";

    this->updateTraversal();
}

/*
 * Updaters
 */

// Updates the arp traversal values based on the current pattern
void Arp::updateTraversal(){
    traversal = std::vector<int>{};

    // TODO: create a "one-off random" pattern that fills traversal with random values.
    // this could givbe the user interesting results without being perma-random
    if (pattern == "Down") {
        for (int i = chord->getLength()-1; i >= 0; i--) {
            traversal.push_back(i);
        }

    } else if (pattern == "U+D In") {
        for (int i = 0; i < chord->getLength(); i++) {
            traversal.push_back(i);
        }
                for (int i = chord->getLength()-1; i > 0; i--) {
            traversal.push_back(i);
        }

    } else if (pattern == "U+D Ex") {
        for (int i = 0; i < chord->getLength()-1; i++) {
            traversal.push_back(i);
        }
        for (int i = chord->getLength()-1; i > 0; i--) {
            traversal.push_back(i);
        }

    // } else if (pattern == "Random") {
        // Do nothing

    } else { // pattern == "Up"
        for (int i = 0; i < chord->getLength(); i++) {
            traversal.push_back(i);
        }
    }    

    resetState();
}

void Arp::resetState(){
    traversalIndex = 0;
    step = 0;
    trig = false;
}

// Intended to be called every time a clock pulse is received
void Arp::onClockPulse(){
    this->updateStep();
}

void Arp::updateStep(){
    int semi;
    if (pattern == "Random"){
        // TODO this random method is biased, created a stronger random function
        // Maybe keep this one as a separate option (e.g. "BadRandom") if it produces interesting
        // musical results
        step = rand() % chord->getLength();
    } else {
        step = traversal[traversalIndex];
    }
    
    semi = chord->getNoteAt(step);
    
    trig = true;
    dacValue = semitoneToDac(semi);
    
    traversalIndex++;
    traversalIndex = traversalIndex % static_cast<int>(traversal.size());

    this->updateString();
    // step and traversalindex are not reset at this point
}

// E.g. "_ _ C#_ _" or  "A _ _"
void Arp::updateString(){
    string = "";

    for(int i = 0; i < chord->getLength(); i++){
        if (i == step){
            string += allNotes[chord->getNoteAt(i) % 12];
            if (string.back() != '#'){
                string += " ";
            }
        } else {
            string += "_ ";
        }
    }
}

/* Getters */

bool Arp::getTrig(){
    bool ret = trig;
    trig = false;
    return ret;
}

float Arp::getDacValue(){
    return dacValue;
}

DiatonicChord* Arp::getChord(){
    return chord;
}

std::string Arp::toString(){
    return string;
}

/* Setters */

void Arp::setPattern(std::string s){
    pattern = s;
    updateTraversal();
}