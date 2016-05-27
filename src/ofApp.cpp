#include "ofApp.h"
#include "graphics-utils.h"

void ofApp::setup() {
    ofSetWindowTitle("DEX UI");
    ofBackground(COLOR_15);
    
    ofSetFrameRate(60);
    
    layout = Layout();
    
    left = Left();
    //    left.setPos(165,165);
    left.setDelay(-50);
    
    right = Right();
    right.setPos(68*GRID_SIZE,0);
    right.setDelay(-50);
    
    term = Term();
    //    term.setPos(29*GRID_SIZE, 11*GRID_SIZE);
    term.setPos(255, 0);
    
    keyboard.setPos(16*GRID_SIZE, 43*GRID_SIZE);
    keyboard.setDelay(-100);
    
    // Other
    isDrawing = true;
    
    isRecording = false;
    recordFrameCounter = 0;
    recordMaxFrames = 1; //60*5;
    testInt = 0;
    
    
    console = new ofxConsole();
    console->setPos(255, 0);
    
    console->setToggleKey('#');
    
    console->print("===================Welcome dodola ================", CTEXT_ERROR);
    
    console->addFunction("me", this, &ofApp::drawDodola);
    
}
void ofApp::drawDodola(const std::vector<std::string> & args){
    
    console->clearTextBuffer();
    console->print(":::------:::::::-----:::::::-----:::::::-----:::::");
    console->print("::::-----:::::::-----:::::::----:::::::-----::::::");
    console->print(":::::--:/-:::::::----::::::-----::::/oyy+--:::::::");
    console->print("::::::+hyho/:::::----::::::-----::+yos/-ys::::::::");
    console->print("::::::h/`ssys/::::----:::::----::sy-.y  `ss::::::-");
    console->print("-::::oh` `s-:ys/::----:::::----/yo` s-   .ho::::--");
    console->print("--:::y+   `s. :ysssyyyyhyyyyyyyyy///y     +h:::---");
    console->print("---:/h-    .s++/--ohyyyyyyyyyyyhh  ``     .h+:----");
    console->print("----+h.     `   `yhyyyyyyyyyyyyh:          yo----:");
    console->print(":---+h`          /sss+:+syyyso/`           os--:::");
    console->print(":::-+h`                         `.`        oy-::::");
    console->print("::::/s.     ++/`                :::        oy:::::");
    console->print(":::::.                                     ./::::-");
    console->print("--::/o                                     .+::---");
    console->print("----ss         +:              `s-         :h:----");
    console->print("::--yo.::      os              `h-       `/+h:-:::");
    console->print("::/oyh::-      ``                         `oho/:::");
    console->print("::::shy:.`          .::. `::-           `+oho:::--");
    console->print("----:/yyho///:::///:o+:+++::y::////:://osyo/+/----");
    console->print("::----/::///yh::---:/++oooo+/::-----:sh/::-----:::");
    console->print("::::::----:oh-                       `sy:--:::::::");
    console->print("----::::::+h-                         `ys:::------");
    console->print("---------:h+               `--`.-.     -h/---:::::");
    console->print(":::::::::oy`              `s//o+/++     oy::::::::");
    console->print("--------:h+                /o////o-     .h+-------");
    
}


void ofApp::update() {
    if (isRecording) {
        ofSaveFrame();
        recordFrameCounter++;
        
        if (recordFrameCounter >= recordMaxFrames) {
            isRecording = false;
            recordFrameCounter = 0;
        }
    }
}

void ofApp::draw() {
    ofPushMatrix();
    {
        // translate for osx development (smaller windows)
        // use translate to keep all other coordinates accurate when untranslated
        // note: some elements don't work with translate because they are positioned
        // with ofEasyCamera to deal with perspective differences
        ofTranslate(X_OFFSET, Y_OFFSET);
        
        layout.drawBG();
        layout.drawGrid(30);
        // Draw and update components
        if (isDrawing) {
            left.draw();
            right.draw();
            term.draw();
            keyboard.draw();
        }
        console->render();
        
    }
    ofPopMatrix();
    
    // Framerate
    ofSetColor(255);
    ofDrawBitmapString(ofToString(ofGetFrameRate()), 10,10);
}
int mapStringToKeyCode(int key) {
    
    if (key == 49) return 2;
    else if (key == 51) return 3;
    else if (key == 52) return 4;
    else if (key == 53) return 5;
    else if (key == 54) return 6;
    else if (key == 55) return 7;
    else if (key == 56) return 8;
    else if (key == 57) return 9;
    else if (key == 58) return 10;
    else if (key == 48) return 11;
    else if (key == 45) return 12;
    else if (key == 127) return 14;
    
    else if (key == 9) return 15;
    else if (key == 113) return 16;
    else if (key == 119) return 17;
    else if (key == 101) return 18;
    else if (key == 114) return 19;
    else if (key == 116) return 20;
    else if (key == 121) return 21;
    else if (key == 117) return 22;
    else if (key == 105) return 23;
    else if (key == 111) return 24;
    else if (key == 112) return 25;
    else if (key == '[') return 26;
    
    
    else if (key == -1) return 58;
    else if (key == 97) return 30;
    else if (key == 115) return 31;
    else if (key == 100) return 32;
    else if (key == 102) return 33;
    else if (key == 103) return 34;
    else if (key == 104) return 35;
    else if (key == 106) return 36;
    else if (key == 107) return 37;
    else if (key == 108) return 38;
    else if (key == 59) return 39;
    else if (key == 13) return 28;
    
    else if (key == 2304) return 42;
    else if (key == 122) return 44;
    else if (key == 120) return 45;
    else if (key == 99) return 46;
    else if (key == 118) return 47;
    else if (key == 98) return 48;
    else if (key == 110) return 49;
    else if (key == 109) return 50;
    else if (key == 44) return 51;
    else if (key == 46) return 52;
    else if (key == 47) return 53;
    else if(key ==32) return 57;
    
    
    return 0;
}




void ofApp::keyPressed(int key) {
    //  if (key == '.') {
    //    isRecording = !isRecording;
    //    cout << "recording: " << isRecording << endl;
    //  }
    //
    //  if (key == '=')
    //    isDrawing = !isDrawing;
    console->keyPressed(key);
    keyboard.keycode=mapStringToKeyCode(key);
}



void ofApp::keyReleased(int key) { }
void ofApp::mouseMoved(int x, int y ) { }
void ofApp::mouseDragged(int x, int y, int button) { }
void ofApp::mousePressed(int x, int y, int button) { }
void ofApp::mouseReleased(int x, int y, int button) { }
void ofApp::windowResized(int w, int h) { }
void ofApp::gotMessage(ofMessage msg) { }
void ofApp::dragEvent(ofDragInfo dragInfo) { }
