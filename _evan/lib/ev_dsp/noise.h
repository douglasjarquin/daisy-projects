/* 
 * Copyright (C) 2022 Evan Pernu. Author: Evan Pernu
 * 
 * You may use, distribute and modify this code under the
 * terms of the GNU AGPLv3 license.
 * 
 * This program is part of "Evan's Daisy Projects".
 * 
 * "Evan's Daisy Projects" is free software: you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as published
 * by the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

#pragma once

#include "daisysp.h"
#include "daisy_patch.h"
#include "djfilter.h"

using namespace daisy;
using namespace daisysp;

namespace ev_dsp {
    
    // White noise (sweep) generator 
    class Noise{
    public:
        DjFilter* djf;
        WhiteNoise* whiteNoise;

        // Don't use
        Noise(){};

        // @param patch->AudioSampleRate()
        Noise(float samplerate){
            djf = new DjFilter(samplerate);
            djf->dontMute = false;
            
            whiteNoise = new WhiteNoise();
            whiteNoise->Init();
        };

        // @param audio in
        // @return audio out
        float processAudio(){
            return djf->processAudio(whiteNoise->Process());
        }
    };

} // namespace ev_dsp