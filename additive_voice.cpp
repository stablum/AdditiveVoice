/*
    Frablum's Additive Synthesis Voice
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
*/
#include "daisysp.h"
#include "daisy_patch.h"
#include <string>

#define MAX_PARTIALS 100
#define MIN_PARTIALS 1
#define STARTUP_PARTIALS 10;
#define MIN_SLOPE 1
#define MAX_VOICES 5

#define DEFAULT_FONT Font_6x8
#define ROW_HEIGHT 10
using namespace daisy;
using namespace daisysp;

DaisyPatch patch;
Oscillator oscs[MAX_VOICES][MAX_PARTIALS];
Parameter constantctrl, stretchctrl, ampctrl;
float dbg;
int partials = STARTUP_PARTIALS;
int amps = 0;
int wave = 0;
int poly = 1;
int mild = 4;
int num_waves = Oscillator::WAVE_LAST;
float normalizer = MAX_PARTIALS;
bool encoder_pressed_prev = false;
enum
{
    P_PARTIALS,
    P_AMPS,
    P_MILD,
    P_WAVE,
    P_POLY,
    P_LAST
};

enum
{
    A_EXP,
    A_LP,
    A_BP,
    A_HP,
    A_NOTCH,
    A_LAST
};

std::string amps_str[] = { "RE","LP","BP","HP","N"};
std::string wave_str[] = {"Sin","Tri","Saw","Rmp","Sqr","PTr","PSw","PSq"};
int param_selected = P_PARTIALS;

float GetFundV(void)
{
    float in = patch.GetCtrlValue((DaisyPatch::Ctrl)0);
    float voltage = in * 5.0f;
    return voltage;
}

float vtof(float voltage)
{
    float freq = powf(2.f, voltage) * 55;
    return freq;
}

static void AudioCallback(float **in, float **out, size_t size)
{
    float sum, con, stretch, amp, a, fundf, fundv;
    float samples[MAX_PARTIALS];

    patch.UpdateAnalogControls();

    stretch = stretchctrl.Process();
    con = constantctrl.Process();
    amp = ampctrl.Process();

    for (int voice=0; voice < poly; voice++) {
        if(voice==0) {
            fundv = GetFundV();
            fundf = vtof(fundv);
        } else {
            continue;
        }
        for (int par=0; par < partials; par++) {
            
            oscs[voice][par].SetFreq(fundf + par*fundf*stretch + par*con);
            //oscs[j].SetFreq(fundf+(j*con));

            oscs[voice][par].SetWaveform(wave);
            a = 0;// (1 - abs(amp*2-1)) * 1; // middle: flat

            switch(amps) {
                case A_EXP:
                    if(amp <= 0.5) { // between 0 and 0.5
                        a += (1/(par*(2*(0.5-amp))+1)); // first half to middle: exponential 
                    } else { // between 0.5 and 1
                        a += (1/(1+(partials-par-1)*((amp-0.5)*2))); //middle to second half: exponential on reversed order
                    }
                    break;
                case A_LP:
                    if(par < amp*partials) {
                        a = 1;
                    } else if(par > amp*partials + mild){
                        a = 0;
                    } else {
                        a = 1 - (par - amp*partials)/mild;
                    }
                    break;
                case A_BP:
                    if(par < amp*partials - mild) {
                        a = 0;
                    } else if(par > amp*partials + mild){
                        a = 0;
                    } else if(par < amp*partials){
                        a = 1 - (amp*partials - par)/(mild);
                    } else if(par > amp*partials){
                        a = 1 - (par - amp*partials)/(mild);
                    }
                    break;
                case A_HP:
                    if(par < amp*partials) {
                        a = 0;
                    } else if(par > amp*partials + mild){
                        a = 1;
                    } else {
                        a = (par - amp*partials)/mild;
                    }
                    break;
                case A_NOTCH:
                    if(par < amp*partials - mild) {
                        a = 1;
                    } else if(par > amp*partials + mild){
                        a = 1;
                    } else if(par < amp*partials){
                        a = (amp*partials - par)/(mild);
                    } else if(par > amp*partials){
                        a = (par - amp*partials)/(mild);
                    }
                    break;
            }
            oscs[voice][par].SetAmp(a);
        }
    }
    for (size_t i = 0; i < size; i ++)
    {
        // Read Knobs
        // Set osc params
        //.Process
    	sum = 0;

        for (int par=0; par < partials; par++) {
            samples[par] = 0;
            for(int voice=0; voice<poly; voice++) {
                samples[par] += oscs[voice][par].Process();
            }
            sum += samples[par];
        }
        out[0][i] = sum/normalizer;
	out[1][i] = samples[0];
	out[2][i] = samples[partials-1];
	out[3][i] = (sum - samples[0])/(normalizer-1);
        // Assign Synthesized Waveform to all four outputs.
        //for (size_t chn = 0; chn < 4; chn++)
        //{
        //    out[chn][i] = sig;
        //}
    }
}

void UpdateNormalizer(void)
{
    switch(amps) {
	case A_EXP:
	case A_LP:
	case A_HP:
	    normalizer = poly*partials;
	    break;
	case A_BP:
	    normalizer = poly*mild;
	    break;
	case A_NOTCH:
	    normalizer = poly*(partials - mild);
	    break;
    }
}

void CheckEncoder(void)
{
    patch.DebounceControls();
    if(patch.encoder.Pressed()) {
	if(encoder_pressed_prev == false){
	    // trigger pressed action
	    param_selected = (param_selected + 1) % P_LAST;
	}
	encoder_pressed_prev = true;
    } else {
	encoder_pressed_prev = false;
    }
    switch(param_selected) {
    	case P_PARTIALS:
            partials = std::min(MAX_PARTIALS,std::max(MIN_PARTIALS,(int)(partials + patch.encoder.Increment())));
            break;
    	case P_AMPS:
            amps = ((int)(amps + patch.encoder.Increment()) % A_LAST);
	    if(amps < 0) {
	        amps = A_LAST - 1;
	    }
            break;
    	case P_MILD:
            mild = std::min(MAX_PARTIALS,std::max(MIN_SLOPE,(int)(mild + patch.encoder.Increment())));
	    break;
    	case P_WAVE:
            wave = ((int)(wave + patch.encoder.Increment()) % num_waves);
	    if(wave < 0) {
	        wave = num_waves - 1;
	    }
            break;
    	case P_POLY:
            poly = ((int)(poly + patch.encoder.Increment()) % (MAX_VOICES+1));
	    if(poly <= 0) {
	        poly = 1;
	    }
            break;
    }
    UpdateNormalizer();
}

int main(void)
{
    float samplerate;
    patch.Init(); // Initialize hardware (daisy seed, and patch)
    samplerate = patch.AudioSampleRate();
   
    for(int voice=0; voice<MAX_VOICES; voice++){
	for(int par=0; par<MAX_PARTIALS; par++){
	    oscs[voice][par].Init(samplerate); // Init oscillator
        }
    }
    //fundctrl.Init(patch.controls[patch.CTRL_1], 10.0, 110.0f, Parameter::LINEAR);
    stretchctrl.Init(patch.controls[patch.CTRL_2], 0.0, 5.0, Parameter::EXPONENTIAL);
    constantctrl.Init(patch.controls[patch.CTRL_3], 0.0, 5000.0, Parameter::EXPONENTIAL);
    ampctrl.Init(patch.controls[patch.CTRL_4], 0.0, 1.0f, Parameter::LINEAR);

    //briefly display module name
    std::string str = "";
    char* cstr = &str[0];
    patch.display.SetCursor(0,0);
    str = "FRABLUM's";
    patch.display.WriteString(cstr, Font_11x18, true);
    patch.display.SetCursor(0,16);
    str = "Additive";
    patch.display.WriteString(cstr, Font_11x18, true);
    patch.display.SetCursor(0,32);
    str = "Voice";
    patch.display.WriteString(cstr, Font_11x18, true);
    patch.display.SetCursor(64,36);
    str = "Version";
    patch.display.WriteString(cstr, Font_6x8, true);
    patch.display.SetCursor(72,44);
    str = VERSION;
    patch.display.WriteString(cstr, Font_7x10, true);
    patch.display.Update();
    patch.DelayMs(2000);

    UpdateNormalizer();

    patch.StartAdc();
    patch.StartAudio(AudioCallback);
    while(1) 
    {
	//update the oled
	patch.display.Fill(false);

        //patch.DisplayControls(false);
	//patch.DelayMs(20);
	CheckEncoder();

	patch.display.SetCursor(0,0);
	std::string str = "F:";
	float fundv = GetFundV();//fundctrl.Process();
        float fundf = vtof(fundv);
	str += std::to_string((int)fundv);
	str += ":";
	str += std::to_string((int)fundf);
	str += "Hz ";
	str += "P";
        str += (param_selected == P_PARTIALS)?">":":";
	str += std::to_string(partials);
        str += (partials>38||mild>partials?"!":((param_selected == P_PARTIALS)?"<":""));
	char* cstr = &str[0];
	patch.display.WriteString(cstr, DEFAULT_FONT, true);

	patch.display.SetCursor(0,1*ROW_HEIGHT);
	str = "Amp";
        str += (param_selected == P_AMPS)?">":":";
	str += amps_str[amps];
        str += (param_selected == P_AMPS)?"<":" ";
	str += "Mld";
        str += (param_selected == P_MILD)?">":":";
	str += std::to_string(mild);
        str += (mild>partials?"!":((param_selected == P_MILD)?"<":" "));
	patch.display.WriteString(cstr, DEFAULT_FONT, true);

	patch.display.SetCursor(0,2*ROW_HEIGHT);
	str = "W";
        str += (param_selected == P_WAVE)?">":":";
	str += wave_str[wave];
        str += (param_selected == P_WAVE)?"<":" ";
	str += "Poly";
        str += (param_selected == P_POLY)?">":":";
	str += std::to_string(poly);
        str += (param_selected == P_POLY)?"<":"";
	patch.display.WriteString(cstr, DEFAULT_FONT, true);

	float stretch = stretchctrl.Process();
	patch.display.SetCursor(0,3*ROW_HEIGHT);
	str = "Stretch:";
	if(stretch<0){
	    str += "-";
	}
	str += std::to_string(((int)abs(stretch)));
	str += ".";
	str += std::to_string((int)(abs(stretch)*10)%10);
	str += std::to_string((int)(abs(stretch)*100)%10);
	str += std::to_string((int)(abs(stretch)*1000)%10);

	patch.display.WriteString(cstr, DEFAULT_FONT, true);
	
	float con = constantctrl.Process();
	patch.display.SetCursor(0,4*ROW_HEIGHT);
	str = "Const:";
	str += std::to_string(((int)con));
	if(con < 100) {
	    str += ".";
	    str += std::to_string((int)(abs(con)*10)%10);
	}
	if(con < 10) {
	    str += std::to_string((int)(abs(con)*100)%10);
	}
	str+="Hz";
	patch.display.WriteString(cstr,DEFAULT_FONT, true);
	patch.display.Update();
	patch.display.SetCursor(0,5*ROW_HEIGHT);
	//str = "Dbg:";
	//str += std::to_string(((int)(dbg*10000)));
	//patch.display.WriteString(cstr,DEFAULT_FONT, true);
	patch.display.Update();
    }
}
