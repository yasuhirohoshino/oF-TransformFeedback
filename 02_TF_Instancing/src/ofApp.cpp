#include "ofApp.h"

const int numParticles = 100000;

//--------------------------------------------------------------
void ofApp::setup(){
    float * position = new float[numParticles * 3];
    for(int i = 0; i<numParticles; i++){
        position[i * 3 + 0] = ofRandom(-10, 10);
        position[i * 3 + 1] = ofRandom(-10, 10);
        position[i * 3 + 2] = ofRandom(-10, 10);
    }
    
    float * velocity = new float[numParticles * 3];
    for(int i = 0; i<numParticles; i++){
        velocity[i * 3 + 0] = ofRandom(-1, 1);
        velocity[i * 3 + 1] = ofRandom(-1, 1);
        velocity[i * 3 + 2] = ofRandom(-1, 1);
    }
    
    float * age = new float[numParticles];
    for(int i = 0; i<numParticles; i++){
        age[i] = 1;
    }
    
    float * lifetime = new float[numParticles];
    for(int i = 0; i<numParticles; i++){
        lifetime[i] = 1;
    }
    
    shader.setupShaderFromFile(GL_VERTEX_SHADER, "shaders/transformFeedback.vert");
    const GLchar* feedbackVaryings[] = { "outPosition", "outVelocity", "outAge", "outLifetime"};
    glTransformFeedbackVaryings(shader.getProgram(), 4, feedbackVaryings, GL_SEPARATE_ATTRIBS);
    shader.linkProgram();
    
    ofBoxPrimitive box;
    box.set(2, 2, 20, 1, 1, 1);
    mesh = box.getMesh();
    
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
    mesh.getVbo().setAttributeDivisor(posLocation, 1);
    mesh.getVbo().setAttributeDivisor(velLocation, 1);
    mesh.getVbo().setAttributeDivisor(ageLocation, 1);
    mesh.getVbo().setAttributeDivisor(lifetimeLocation, 1);
    
    lightPos.set(1000, 1000, 1000);
    
    gui.setup("particles");
    gui.add(startColor.set("start color", ofFloatColor(1.0, 0.0, 0.0, 1.0), ofFloatColor(0.0, 0.0, 0.0, 1.0), ofFloatColor(1.0, 1.0, 1.0, 1.0)));
    gui.add(endColor.set("end color", ofFloatColor(1.0, 0.0, 0.0, 1.0), ofFloatColor(0.0, 0.0, 0.0, 1.0), ofFloatColor(1.0, 1.0, 1.0, 1.0)));
    gui.add(timestep.set("timestep", 0.25, 0.0, 1.0));
    gui.add(scale.set("scale", 0.01, 0.0, 1.0));
}

//--------------------------------------------------------------
void ofApp::update(){
    ofSetWindowTitle(ofToString(ofGetFrameRate()));
}

//--------------------------------------------------------------
void ofApp::draw(){
    ofBackgroundGradient(ofColor(255), ofColor(150));
    
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
    
    mesh.getVbo().setAttributeBuffer(posLocation, posBuffer[frame], 3, 0);
    mesh.getVbo().setAttributeBuffer(velLocation, velBuffer[frame], 3, 0);
    mesh.getVbo().setAttributeBuffer(ageLocation, ageBuffer[frame], 1, 0);
    mesh.getVbo().setAttributeBuffer(lifetimeLocation, lifetimeBuffer[frame], 1, 0);
    
    ofEnableDepthTest();
    
    cam.begin();
    renderShader.begin();
    renderShader.setUniform3f("lightPos", lightPos);
    renderShader.setUniform4f("startColor", startColor);
    renderShader.setUniform4f("endColor", endColor);
    renderShader.setUniformMatrix4f("normalMatrix", ofGetCurrentNormalMatrix());
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    mesh.drawInstanced(OF_MESH_FILL, numParticles);
    glDisable(GL_CULL_FACE);
    renderShader.end();
    cam.end();
    
    frame = 1 - frame;
    
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
