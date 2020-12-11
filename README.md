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
| Audio Out3 | Last partial | |
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
| Slp | slope size of the amplitude scheme. It's the number of partials that are part of the "slope", for filter-like amplitude schemes, as the slope gets smaller, the cutpoints become more drastic.| |
| Wave | waveform of the partial (0 is sine wave) | |
| Poly | not implemented yet | |

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

