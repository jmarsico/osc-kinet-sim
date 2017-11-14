#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    
    
    
    
    ofSetFrameRate(44);
    
    
    ofxJSONElement config;
    bool success = config.open("config.json");
    int numChannels = 48*3;
    
    if(success){
        ofLog() << "loaded config";
        
        int oscPort = config["osc_in_port"].asInt();
        string kIP = config["kinet_ip"].asString();
        int kinet_port = config["kinet_port"].asInt();
        int kinet_psPort = config["kinet_psPort"].asInt();
        string dmx_port = config["dmx_port"].asString();
        
        bFlipped = config["bFlipped"].asBool();
        
        osc.setup(oscPort);
        
        kinet.init(kIP, kinet_port, kinet_psPort);
        
        
        dmx.connect(dmx_port, numChannels);
        
    } else {
        ofLog() << "could not load config";
        osc.setup(12345);
        kinet.init("10.41.57.114", 6038, 1);
        dmx.connect(0, numChannels );
        bFlipped = true;
    }
    
    
    
    //send DMX by default
    bSendDMX = true;
    bSendKinet = false;
    
    
    int w = 20;
    int s = 3;
    int h = 8;
    ofPoint start;
    
    start.set(10, 10);

    for(int i = 0; i < 24; i++){
        Light l;
        
        if(i%4 == 0){
            start.x += 10;
        } else {
            start.x +=3;
        }
        
        start.x += w;
        
        l.rect.set(start, w, h);
        l.col.set(0, 0, 0);
        lights.push_back(l);
    }
    
    start.set(10,ofGetHeight() - 18);
    for(int i = 0; i < 24; i++){
        Light l;
        
        if(i%4 == 0){
            start.x += 10;
        } else {
            start.x +=3;
        }
        
        start.x += w;
        
        l.rect.set(start, w, h);
        l.col.set(0, 0, 0);
        lights.push_back(l);
    }
}

//--------------------------------------------------------------
void ofApp::update(){

    while(osc.hasWaitingMessages()){
        ofxOscMessage m;
        osc.getNextMessage(m);
        if(m.getAddress() == "/lights"){
            if(m.getNumArgs() == 48 * 3){
                
                for(int i = 0; i < m.getNumArgs(); i+=3){
                    
                    int r,g,b;
                    if(bFlipped){
                        if(i < (24*3)){
                            int j = i+(24*3);
                            r = m.getArgAsInt(j);
                            g = m.getArgAsInt(j+1);
                            b = m.getArgAsInt(j+2);
                        } else if (i >= 24*3){
                            int j = i - (24*3);
                            r = m.getArgAsInt(j);
                            g = m.getArgAsInt(j+1);
                            b = m.getArgAsInt(j+2);
                        }
                    }else {
                        
                        r = m.getArgAsInt(i);
                        g = m.getArgAsInt(i+1);
                        b = m.getArgAsInt(i+2);
                        
                    }
                    
                    
                    
                    
                    lights[i/3].col.r = r;
                    lights[i/3].col.g = g;
                    lights[i/3].col.b = b;
                    
                    
                    
                    kinet.setValue(i, r);
                    kinet.setValue(i+1, g);
                    kinet.setValue(i+2, b);
                    
                    
                    
                    
                
                    
                }
                
                for(int i = 0; i < lights.size();i++){
                    
                    int r = lights[i].col.r;
                    int g = lights[i].col.g;
                    int b = lights[i].col.b;
                    
                    dmx.setLevel((i*3)+1, r);
                    dmx.setLevel((i*3)+2, g);
                    dmx.setLevel((i*3)+3, b);
                }
            }
        }
    }
    
    if(bSendKinet)  kinet.send();
    if(bSendDMX)    dmx.update();
    

}

//--------------------------------------------------------------
void ofApp::draw(){
    for(auto& l: lights){
        ofSetColor(l.col);
        ofRect(l.rect);
        ofSetColor(0,0,0);
    }
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){
    if(key == 'd'){
        bSendDMX = !bSendDMX;
    }
    if(key == 'k'){
        bSendKinet = !bSendKinet;
    }
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
