# Description

Additive synthesis voice

# Controls

| Control | Description | Comment |
| --- | --- | --- |
| Ctrl1 | 1v/Oct | Fundamental pitch |
| Ctrl2 | Stretch | Partial/harmonic multiplier of the fundamental |
| Ctrl3 | Constant | Constant amount that is multiplied by the partial's integer index and added to the partial base frequency |
| Ctrl4 | Partials amplitude control | When the amplitude scheme is filter-like, acts like a cutoff |
| Audio Out1 | Main output | All the partials |
| Audio Out2 | Fundamental | Just the fundamental wave |
| Audio Out3 | Last partial | Just the last partial | 
| Audio Out4 | Without fundamental | All the partials without the first one|
| Encoder Push | Next parameter | Round-robin selection of the additional parameters|
| Encoder Turn | Alter parameter | Changes the currently selected additional parameter |

# Additional Parameters

Besides the 4 controls described above, there are a number of paramters that can be
accessed via the "menu" implemented by pushing the encoder. By pushing the encoder
the next additional parameter will be selected, and by rotating the encoder
the parameter value can be changed:

| Additional parameter | Description | |
| --- | --- | --- |
| P | number of partials. To hear anything this needs to be at least 1 | |
| Amp | amplitude scheme that determines the amplitude of the partials. Some of the algorithms (such as LP, BP, HP) mimic a filter | |
| Mld | mildness of the slope of the amplitude scheme. It's the number of partials that are part of the partials amplitude ramp. For filter-like amplitude schemes, as the mildness is set as smaller values, the cutpoints become more drastic.| |
| W | waveform of the partial (see dedicated section)| |
| Poly | not implemented yet | |

# Possible waveforms

The waveforms that one can choose are the ones that are implemented in the DaisySP library and are:

| On screen | What is it | |
| --- | --- | --- |
| Sin | Sine | |
| Tri | Triangular | |
| Saw | Sawtooth | |
| Rmp | Ramp | |
| Sqr | Square | |
| PTr | Polyblep Triangular | |
| PSw | Polyblep Sawtooth | |
| PSq | Polyblep Square | |

# How to install this firmware in the daisy patch

A pre-compiled binary firmware `additive_voice.bin` is available in the sub-directory `build/` : <https://github.com/stablum/AdditiveVoice/blob/master/build/additive_voice.bin>

Please use the tool at the page <https://electro-smith.github.io/Programmer/> to write the binary firmware to the daisy patch.

# Author
Francesco 'Frablum' Stablum

# License

    Copyright (C) 2020 Francesco 'Frablum' Stablum

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <https://www.gnu.org/licenses/>.

