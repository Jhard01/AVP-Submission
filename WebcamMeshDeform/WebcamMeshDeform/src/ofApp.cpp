#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    
    //Audio Settings
    sampleRate = 44100;
    bufferSize = 512;
    
    //Gui Setup
    gui.setup(); // most of the time you don't need a name
    gui.add(vidOn.setup("Video On", false));
    gui.add(isSine.setup("Sine", false));
    gui.add(isSquare.setup("Square",false));
    gui.add(isSaw.setup("Saw", false));
    gui.add(isTriangle.setup("Triange",false));
    gui.add(Timer.setup("Sequencer Speed",5,0,10));
    gui.add(vibrato.setup("Vibrato",0.1,0,2));
    gui.add(lpCuttOff.setup("Filter Cuttoff", 50,0,1500));
    gui.add(lPQ.setup("Filter Q",1,0,10));
    gui.add(lfo1Speed.setup("Lfo Speed", 1, 0, 10));
    gui.add(lfo1Amp.setup("Lfo Amplitude",10,1,100));
    gui.add(circleDeformX.setup("Sphere Deform X",10,1,400));
    gui.add(circleDeformY.setup("Sphere Deform Y",10,1,400));
    
    //OF Setup
    ofSetVerticalSync(true);
    ofEnableAlphaBlending();
    ofEnableSmoothing();
    vidGrabber.setup(640, 480, true);
    
    //Set the Plane?
    sphere.set(300, 50); //Radius, Resolution
    sphere.setPosition(ofGetWidth()/2, ofGetHeight()/2, 0);
    sphere.rotate(45, 1, 0, 0);
    strength = 0.0001;

    //Lighting Setup
    ofSetSmoothLighting(true);
    light.enable();
    
    //SoundStream
    soundSt.setup(this, 2, 2, sampleRate,bufferSize,4);
}

//--------------------------------------------------------------
void ofApp::update(){
    
    //Rotate the sphere
    sphere.rotate(0.1,1.2, 0.9, 0.3); // * RMS or something

    //Update the camera if on then capture if off then pause.
    if(vidOn){
        vidGrabber.update();
    }
}

//--------------------------------------------------------------
void ofApp::draw(){
    
    ofBackgroundGradient(ofColor::white, ofColor::gray);
    
    //Render in Z-Plane order with depth testing
    ofEnableDepthTest();
    
    //WrapSphere with texture.
    vidGrabber.getTexture().bind();

    sphere.mapTexCoordsFromTexture(vidGrabber.getTexture());
    deformMesh =  sphere.getMesh();
    
    sphere.mapTexCoordsFromTexture( vidGrabber.getTexture() );
    
    //Variable to store the vertex coordinates
    ofVec3f currentVertex;
    
    //Loop through the verticies of the sphere
    for(int i=0; i < deformMesh.getNumIndices(); i++){
        
        strength = 0.00002 +(lfo1Out*0.0000001);
        
        //Grab the index of this vertex
        int vertexIndex = deformMesh.getIndex(i);
        //Get the corresponding vertex
        currentVertex = deformMesh.getVertex(vertexIndex);
        
        //Create new coordinates from the mouse position
        float newX = cos(cos((circleDeformX)*strength*i));
        float newY = cos(sin((circleDeformY)*strength*i));
        
        //Update the current coordinates with the the new x & y values
        currentVertex.x *= newX;
        currentVertex.y *= newY;
        currentVertex.z *= newX*newY;
        deformMesh.setVertex(vertexIndex, currentVertex);
    }
    
    sphere.transformGL();
    
    //Now draw deformed plane
    deformMesh.draw();
    vidGrabber.getTexture().unbind();
    ofDisableDepthTest();
    ofPopStyle();
    ofPopMatrix();
    
    //Draw the Gui
    gui.draw();
}

//--------------------------------------------------------------
void ofApp::audioOut(float * output,int bufferSize, int nChannels){
    
    float outSum   = 0;
    
    for(int i=0; i < bufferSize; i++){
        
        //Different Lfos
        lfo1Out = lfo1.sinebuf(lfo1Speed)*lfo1Amp;
        VibratoLfoOut = VibratoLfo.sinewave(1)*vibrato;
        
        //Use the phasor to step through the sequence
        
        seqIndex = sequencer.phasor(0.75, 1, Timer);
        
        //Notes are generated as a kind of arpegiator and scaled by the last keyPressed.
        
        if(isSine == false && isTriangle == false && isSaw == true && isSquare == false){
            amp = 1;
            oscOut=osc.sawn(notes[seqIndex]+VibratoLfoOut*5);
        }
        if(isSine == false && isTriangle == false && isSaw == false && isSquare == true){
            amp = 0.5;
            oscOut=osc.square(notes[seqIndex]+VibratoLfoOut*5);
        }
        if(isSine == true && isTriangle == false && isSaw == false && isSquare == false){
            amp = 1;
            oscOut=osc.sinewave(notes[seqIndex]+VibratoLfoOut*5);
        }
        if(isSine == false && isTriangle == true && isSaw == false && isSquare == false){
            amp = 1;
            oscOut=osc.triangle(notes[seqIndex]+VibratoLfoOut*5);
        }
        
        //Apply the lowPass filter
        lpOut = lpass.lores(oscOut,(lpCuttOff+lfo1Out), lPQ);
        
        //Absolute value for the RMS.
        outSum += fabs(lpOut*amp);
        
        //Send the Audio Out
        output[i*nChannels]  = (lpOut*amp);
        output[i*nChannels+1]= (lpOut*amp);
        
        //Assigning the RMS
        RMSOut = sqrt(outSum/(bufferSize*15));
    }
}

//--------------------------------------------------------------
void ofApp::exit() {
    soundSt.close();
}
//--------------------------------------------------------------
