
#ifndef point_hpp
#define point_hpp
#include "ofMain.h"
#include "ofxMaxim.h"
#include "point.hpp"

#include <stdio.h>

class point{

public:
    //Constructor, Deconstructor and Functions
    point();
    ~point();
    void update(float dt);
    void draw();
    void addPoints(int n);
    void setRadius(float rad);
    
    float Rad;  //Cloud Radius
    float Vel;  //Cloud points Velocity
    int bandRad; //Band index in spectrum affecting Rad
    int bandVel;  //Band index in spectrum affecting Vel
    float Dist;   //Distance for the draw line
    
    int n; //Number of Cloud points
  
    bool triggered; //Is referenced by the ofMaxim envelopes to trigger the audio

    //vectors of floats to store the update computations and the locations of the points.
    vector<float> tx;
    vector<float> ty;
    
    vector<float> px;
    vector<float> py;
    
    float time0;  //Used to calculate the dynamic time
};

#endif /* point_hpp */
