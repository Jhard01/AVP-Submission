#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    //Audio Settings
    sampleRate = 44100;
    bufferSize = 512;
    
    //Gui Setup
    mGui.setup();
    mGui.add(speed.setup("Speed", 0.9999, -4.0, 4.0));
    mGui.add(pitch.setup("Pitch",1.0, 0.0, 10.0));// 0.025, 0.45));
    mGui.add(grainSize.setup("GrainSize",0.25, 0.025, 0.45));
    mGui.add(overLaps.setup("OverLaps",3,1,5));
    mGui.add(recordIn.setup("Record", true));
    mGui.add(volume.setup("Volume",0.5, 0.0, 1.0));
    
    //OF Setup
    ofSetFrameRate(60);
    ofSetVerticalSync(true);
    ofEnableAlphaBlending();
    ofEnableSmoothing();
    
    //Drawing The Waveform in the Buffer
    prevPos.x = prevPos.y = 0;
    curPos.x = curPos.y   = 0;
    Length = 294000;
    playHead = 0.0;
    
    //SoundSteam & Maxi Setup//
    samp.load(ofToDataPath("Audio.wav"));
    ps = new maxiPitchStretch<grainPlayerWin>(&samp); //this referenced the sample in a different way so had to change the maxiTimePitchStretch template to take the hannWinFunctor. I tried using the maxiTimePitchStretch<grainPlayerWin,samp> I can't quite understand the way in which the template deals with sample and hanningWindow to work out the pitch so changed the template in the maxiGrains.h file.
    soundSt.printDeviceList(); ///CHANGE TO SOUNDCARD?
    soundSt.setup(this, 2, 2, sampleRate, bufferSize, 4);
    ofxMaxiSettings::setup(sampleRate, 2, bufferSize);

}

//--------------------------------------------------------------
void ofApp::update(){

}

//--------------------------------------------------------------
void ofApp::draw(){
    ofBackgroundGradient(ofColor::white, ofColor::gray);
    ofSetColor(255);
    ofFill();
    const float waveformWidth = ofGetWidth() - 40;
    const float waveformHeight = 100;
    
    float top = waveformHeight + 150;
    float left = 20;
    
    // draw the audio waveform
    for(int i = 0; i < Length; i+=512){
        curPos.x = ofMap(i,0,Length,left,waveformWidth+20); //static in the horizontal as it loops through the buffer
        curPos.y = ofMap(samp.temp[i],-32768,32768,top,waveformHeight+top); // will fluctuate on the verticle
        //what about visualising as a z pos as well like a spectograph
        ofSetColor(ofColor::grey);
        ofEllipse(curPos.x, curPos.y, 2, 2); // draws the circle at the current positions.
        ofLine(curPos.x, curPos.y, prevPos.x, prevPos.y); //draw a line between the current and last positions
        if(i < Length-512)
        {
            prevPos = curPos;
        }else{
            prevPos.x = left;
            prevPos.y = waveformHeight+top;
        }
    }
    
    // draw a playhead over the waveform
    ofSetColor(ofColor::white);
    ofLine(left + ps->getNormalisedPosition() * waveformWidth, top, left + ps->getNormalisedPosition() * waveformWidth, top + waveformHeight);
    ofDrawBitmapString("PlayHead", left + ps->getNormalisedPosition()- 52 * waveformWidth, top+30);
    
    // draw current recording position
    float sampRecPos = (float)samp.recordPosition / (float) Length;
    ofSetColor(ofColor::red);
    ofLine(left + sampRecPos * waveformWidth, top, left + sampRecPos * waveformWidth, top + waveformHeight);
    ofDrawBitmapString("RecPos", left + sampRecPos * waveformWidth - 52, top+15);
    mGui.draw();
}

//--------------------------------------------------------------
void ofApp::audioIn(float * input, int bufferSize, int nChannels)
{
    for (int i=0; i < bufferSize;i++)
    {
        samp.loopRecord(input[i*nChannels], recordIn, 0);
    }
}

//--------------------------------------------------------------
void ofApp::audioOut(float * output, int bufferSize, int nChannels)
{
    for (int i = 0; i < bufferSize; i++)
    {
        wave = ps->play(pitch, speed, grainSize,(int)overLaps);
        mix.stereo(wave, outputs, 0.5);
        output[i*nChannels]     = outputs[0] * volume;
        output[i*nChannels + 1] = outputs[1] * volume;
    }
}

//--------------------------------------------------------------
void ofApp::exit(){
    soundSt.close();
}
//--------------------------------------------------------------
