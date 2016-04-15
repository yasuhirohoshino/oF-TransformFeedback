#include "ofApp.h"

const int numParticles = 500000;

//--------------------------------------------------------------
void ofApp::setup(){
    ofBackground(0);
    
    float * position = new float[numParticles * 3];
    for(int i = 0; i<numParticles; i+=3){
        position[i * 3 + 0] = ofRandom(-10, 10);
        position[i * 3 + 1] = ofRandom(-10, 10);
        position[i * 3 + 2] = ofRandom(-10, 10);
    }
    
    float * velocity = new float[numParticles * 3];
    for(int i = 0; i<numParticles; i+=3){
        velocity[i * 3 + 0] = ofRandom(-1, 1);
        velocity[i * 3 + 1] = ofRandom(-1, 1);
        velocity[i * 3 + 2] = ofRandom(-1, 1);
    }
    
    float * age = new float[numParticles];
    for(int i = 0; i<numParticles; i++){
        age[i] = ofRandom(1, 100);
    }
    
    float * lifetime = new float[numParticles];
    for(int i = 0; i<numParticles; i++){
        lifetime[i] = ofRandom(50, 100);
    }
    
    shader.setupShaderFromFile(GL_VERTEX_SHADER, "shaders/transformFeedback.vert");
    const GLchar* feedbackVaryings[] = { "outPosition", "outVelocity", "outAge", "outLifetime"};
    glTransformFeedbackVaryings(shader.getProgram(), 4, feedbackVaryings, GL_SEPARATE_ATTRIBS);
    shader.linkProgram();
    
    glGenTransformFeedbacks(2, feedback);
    
    for(int i=0;i<2;i++){
        posBuffer[i].allocate();
        posBuffer[i].setData(sizeof(float) * numParticles * 3, position, GL_STREAM_DRAW);
        
        velBuffer[i].allocate();
        velBuffer[i].setData(sizeof(float) * numParticles * 3, velocity, GL_STREAM_DRAW);
        
        ageBuffer[i].allocate();
        ageBuffer[i].setData(sizeof(float) * numParticles, age, GL_STREAM_DRAW);
        
        lifetimeBuffer[i].allocate();
        lifetimeBuffer[i].setData(sizeof(float) * numParticles, lifetime, GL_STREAM_DRAW);
        
        vbo[i].setAttributeBuffer(shader.getAttributeLocation("inPosition"), posBuffer[i], 3, 0);
        vbo[i].setAttributeBuffer(shader.getAttributeLocation("inVelocity"), velBuffer[i], 3, 0);
        vbo[i].setAttributeBuffer(shader.getAttributeLocation("inAge"), ageBuffer[i], 1, 0);
        vbo[i].setAttributeBuffer(shader.getAttributeLocation("inLifetime"), lifetimeBuffer[i], 1, 0);
        
        glBindTransformFeedback(GL_TRANSFORM_FEEDBACK, feedback[i]);
        glBindBufferBase(GL_TRANSFORM_FEEDBACK_BUFFER, 0, posBuffer[i].getId());
        glBindBufferBase(GL_TRANSFORM_FEEDBACK_BUFFER, 1, velBuffer[i].getId());
        glBindBufferBase(GL_TRANSFORM_FEEDBACK_BUFFER, 2, ageBuffer[i].getId());
        glBindBufferBase(GL_TRANSFORM_FEEDBACK_BUFFER, 3, lifetimeBuffer[i].getId());
    }
    
    glBindTransformFeedback(GL_TRANSFORM_FEEDBACK, 0);
    
    frame = 0;
    
    renderShader.load("shaders/render");
    posLocation = renderShader.getAttributeLocation("inPosition");
    velLocation = renderShader.getAttributeLocation("inVelocity");
    ageLocation = renderShader.getAttributeLocation("inAge");
    lifetimeLocation = renderShader.getAttributeLocation("inLifetime");
    
    ofDisableArbTex();
    particleImage.load("spark.png");
    ofEnableArbTex();
    
    gui.setup("particles");
    gui.add(startColor.set("start color", ofFloatColor(1.0, 0.5, 0.0, 1.0), ofFloatColor(0.0, 0.0, 0.0, 1.0), ofFloatColor(1.0, 1.0, 1.0, 1.0)));
    gui.add(endColor.set("end color", ofFloatColor(0.0, 0.5, 1.0, 1.0), ofFloatColor(0.0, 0.0, 0.0, 1.0), ofFloatColor(1.0, 1.0, 1.0, 1.0)));
    gui.add(timestep.set("timestep", 0.5, 0.0, 1.0));
    gui.add(scale.set("scale", 0.05, 0.0, 1.0));
}

//--------------------------------------------------------------
void ofApp::update(){
    ofSetWindowTitle(ofToString(ofGetFrameRate()));
}

//--------------------------------------------------------------
void ofApp::draw(){
    ofEnableAlphaBlending();
    
    shader.begin();
    shader.setUniform1f("time", ofGetElapsedTimef());
    shader.setUniform1f("timestep", timestep);
    shader.setUniform1f("scale", scale);
    
    glEnable(GL_RASTERIZER_DISCARD);
    glBindTransformFeedback(GL_TRANSFORM_FEEDBACK, feedback[frame]);
    glBeginTransformFeedback(GL_POINTS);
    vbo[1 - frame].draw(GL_POINTS, 0, numParticles);
    glEndTransformFeedback();
    glBindTransformFeedback(GL_TRANSFORM_FEEDBACK, 0);
    glDisable(GL_RASTERIZER_DISCARD);
    shader.end();
    
    renderVbo.setAttributeBuffer(posLocation, posBuffer[frame], 3, 0);
    renderVbo.setAttributeBuffer(velLocation, velBuffer[frame], 3, 0);
    renderVbo.setAttributeBuffer(ageLocation, ageBuffer[frame], 1, 0);
    renderVbo.setAttributeBuffer(lifetimeLocation, lifetimeBuffer[frame], 1, 0);
    
    ofEnableBlendMode(OF_BLENDMODE_ADD);
    
    cam.begin();
    ofEnablePointSprites();
    renderShader.begin();
    renderShader.setUniformTexture("spriteTex", particleImage.getTexture(), 1);
    renderShader.setUniform4f("startColor", startColor);
    renderShader.setUniform4f("endColor", endColor);
    renderVbo.draw(GL_POINTS, 0, numParticles);
    renderShader.end();
    ofDisablePointSprites();
    cam.end();
    
    frame = 1 - frame;
    
    ofDisableAlphaBlending();
    ofDisableDepthTest();
    gui.draw();
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){
    
}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}
