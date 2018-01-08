#include "point.hpp"

//Constructor
point::point(){
    Rad = 500;  //Cloud Radius
    Vel = 0.1;  //Cloud points Velocity
    n = 5; //Number of Cloud points
    time0 = 0;
    Dist = 40; //The threshold for the draw line function
    addPoints(n);
}
point::~point(){}

void point::addPoints(int n){
    //Initialize the Vectors to random numbers and 0.
    for(int i=0; i != n; ++i){
        tx.push_back(ofRandom(1000));
        ty.push_back(ofRandom(1000));
        py.push_back(0);
        px.push_back(0);
        
    }
}

void point::setRadius(float rad){
    Rad = rad;   //Setter for the radius, referenced by the GUI
}

void point::update(float dt){

    //Update particles positions
    for (int j=0; j<n; j++) {
        tx[j] += Vel * dt;    //move offset
        ty[j] += Vel * dt;    //move offset
        //Calculate Perlin's noise in [-1, 1] and
        //multiply on Rad
        px[j] = (ofSignedNoise( tx[j] ) * Rad);
        py[j] = (ofSignedNoise( ty[j] ) * Rad);
    }
}
void point::draw(){
    
    ofBackground( 255, 255, 255 );    //Set up the background
    
    //Move center of coordinate system to the screen center
    ofPushMatrix();
    ofTranslate( ofGetWidth() / 2, ofGetHeight() / 2 );
    
    //Draw points
    ofSetColor( 0, 0, 0, 100);
    ofFill();
    for (int i=0; i<n; i++) {
        ofCircle( px[i], py[i], 2);
    }
    
    //Draw lines between near points
    for (int j=0; j<n; j++) {
        int trigHelper;
        for (int k=j+1; k<n; k++) {
            if ( ofDist( px[j], py[j], px[k], py[k] )
                < Dist ) {
                trigHelper ++;
                ofLine(px[j], py[j], px[k], py[k]);
            }else {

            }
            if(trigHelper >0){
                triggered = true;
                cout << triggered << endl;
            } else {
                triggered = false;
            }
        }
    }
    //Call to pop matrix to restore the translate to original.
    ofPopMatrix();
}


