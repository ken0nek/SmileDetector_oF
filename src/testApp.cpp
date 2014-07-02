#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){
    // listen on the given port
	cout << "listening for osc messages on port " << PORT << "\n";
	receiver.setup(PORT);
    
	ofBackground(255, 255, 255);
    
    smileNumber = 0;
    preSmileNumber = 0;
    
    myImage.loadImage("gakki.jpg");
    
    upSound.loadSound("up.mp3");
    upSound.setLoop(false);
    upSound.setSpeed(300);
    
    downSound.loadSound("down.mp3");
    downSound.setLoop(false);
    downSound.setSpeed(300);
    
    clearSound.loadSound("kira.mp3");
    clearSound.setLoop(false);
    
    voice.loadSound("gakki.mp3");
    voice.setLoop(false);
    voice.setSpeed(1.0);

    int baud = 9600;
    //mySerial.enumerateDevices();
    // mySerial.setup("/dev/tty.usbmodem1411", baud); //open the first device and talk to it at 57600 baud
    mySerial.listDevices();
    vector <ofSerialDeviceInfo> deviceList = mySerial.getDeviceList();
    mySerial.setup(0, baud);
    if( mySerial.setup() ){
        printf("serial is setup!");
    }
    
    
    // ofSetBackgroundAuto(false);
    
//    // デバイスの接続するポートとスピードを設定
//    
//    arduino.connect("/dev/tty.usbmodem1411", 1000); // need to customize
//    
//    // Arduinoとの接続が確立するのを監視するリスナー
//    
//    // 接続が確立したら、setupArduino()を実行
//    
//    ofAddListener(arduino.EInitialized, this, &testApp::setupArduino);
//    
//    // 接続の確立したかどうかのフラグ
//    
//    bSetupArduino!= false;
    
}

//--------------------------------------------------------------
void testApp::update(){
	// check for waiting messages
	while(receiver.hasWaitingMessages()){
		// get the next message
		ofxOscMessage m;
		receiver.getNextMessage(&m);
        
		// check for mouse moved message
		if(m.getAddress() == "/smile/number"){
            preSmileNumber = smileNumber;
            smileNumber = m.getArgAsInt32(0);
            cout << "smile number is " << smileNumber << "\n";
            if (smileNumber > 493) {
                clearSound.play();
                sleep(2);
                voice.play();
            } else {
                clearSound.stop();
            }
        }
        
        int diff = smileNumber - preSmileNumber;
        
        if (diff > 0) {
            mySerial.writeByte('0');
            upSound.play();
        } else {
            downSound.play();
            mySerial.writeByte('1');
        }
	}
}


//--------------------------------------------------------------
void testApp::draw(){
    
//    unsigned char * pixels = myImage.getPixels();
//    
//    for (int i = 0; i < 10; i++) {
//        int x = (int)arc4random_uniform(myImage.width);
//        int y = (int)arc4random_uniform(myImage.height);
//        ofColor color = myImage.getColor(x, y);
//        ofSetColor(color, smileNumber);
//        ofCircle(x, y, 5);
//    }
    
    unsigned char * pixels = myImage.getPixels();
    
    int width = ofGetWidth();
    int height = ofGetHeight();
    int mosaicSize = 20;
    float alpha =  (smileNumber  * 1.0) / 500 * 255;
    
    for (int i = 0; i < width; i += mosaicSize) {
        for (int j = 0; j < height; j += mosaicSize) {
            int r = pixels[j * width * 3 + i * 3];
            int g = pixels[j * width * 3 + i * 3 + 1];
            int b = pixels[j * width * 3 + i * 3 + 2];
            ofSetColor(r, g, b, alpha * alpha  / 255);
            ofFill();
            ofRect(i, j, mosaicSize, mosaicSize);
            ofNoFill();
            ofSetColor(255, 255, 255);
        }
    }
    
    ofEnableAlphaBlending();
    ofSetColor(255,255,255, alpha * alpha / 255);
    myImage.draw(0, 0);
}

//--------------------------------------------------------------
void testApp::keyPressed(int key){
    mySerial.writeByte(key);
    printf(" is pressed");
}

//--------------------------------------------------------------
void testApp::keyReleased(int key){

}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void testApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void testApp::dragEvent(ofDragInfo dragInfo){ 

}
