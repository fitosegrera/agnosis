#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    ofSetFullscreen(true);
    
    camera.setFov(71);  // this will actually do nothing since 60 is the default
    camera.setFarClip(100000);
    camera.setNearClip(1);
    camera.setPosition(ofVec3f(0,0,0));
    prefix = "shanghai2-28-08-16/memory_";
    
    memoCount = 50;
    for (int i = 0 ; i < memoCount;i++){
        
        // Load Image Texture
        //
        ofTexture tempTex;
        ofLoadImage(tempTex, prefix+ofToString(i+1)+".jpg");
        textures.push_back(tempTex);
        
        // Load JSON
        //
        ofxJSONElement tempJSON;
        tempJSON.open(prefix+ofToString(i+1)+".txt");
        JSONObjects.push_back(tempJSON);
        
//       memoMeshs.push_back(createMemoMesh(tempTex, 15, 15));
        
        
        // Translations Calculation
        //
        float xRange,yRange,zRange;
        xRange = zRange = yRange = 3000;
        float tX = ofMap(tempJSON["accelerometer"]["x"].asFloat(),-5,5,-1*xRange,1*xRange);
        float tY = ofMap(tempJSON["accelerometer"]["y"].asFloat(),5,15,-1*yRange,1*yRange);
        float tZ = ofMap(tempJSON["accelerometer"]["z"].asFloat(),-5,5,-1*zRange,1*zRange);
        ofVec3f tempTranslation(tX,tY,tZ);
        translations.push_back(tempTranslation);
        
        // Rotations Calculation
        //
        float rX = tempJSON["orientation"]["x"].asFloat();
        float rY = tempJSON["orientation"]["y="].asFloat();
        float rZ = tempJSON["orientation"]["z"].asFloat();
        float rSpeed = ofMap(tempTranslation.length(),0,yRange*2.5,0,0.1);
        ofVec3f tempR = ofVec3f(rX,rY,rZ).normalize();
        ofVec4f tempRotation(tempR.x,tempR.y,tempR.z,rSpeed);
        rotations.push_back(tempRotation);
    }
    
    
    
//    // Create 2D AlphaMask  (Not necessary for the 3d floating objects)
//    alphamask.begin();
//    ofClear(0,0,0,0);
//    ofBackground(0);
//    for(int i = 0; i < contourFinder.nBlobs; i++) {
//        ofSetColor(255);
//        ofxCvBlob tempBlob = contourFinder.blobs.at(i);
//        ofBeginShape();
//        ofVertices(tempBlob.pts);
//        ofEndShape();
//    }
//    alphamask.end();
//    grayImg.getTexture().setAlphaMask(alphamask.getTexture());
//    maskTexture = grayImg.getTexture();
    
    
    // Generate the 3D Objects based on a piece of memory
    //
    maskMeshes.clear();
    for(int i = 0 ; i < memoCount ; i++){
        ofPixels pix;
        ofLoadImage(pix, prefix+ofToString(i+1)+".jpg");
        // Get one blob randomly
        ofxCvBlob blob =  getOneBlobFromImage(pix, JSONObjects.at(i));
        // Create Mask Mesh
        ofMesh tempMesh = createMeshByBlob(blob, JSONObjects.at(i));
        maskMeshes.push_back(tempMesh);
    }
    
//    imgIndex = 0;
//    
//    flag1 = true;
//    flag2 = true;
//    
    
    radius = 1;
    showBg = false;
    // glEnable(GL_DEPTH_TEST);

    bg.load("bg.jpg");
}

//--------------------------------------------------------------
void ofApp::update(){
    
}

//--------------------------------------------------------------
void ofApp::draw(){
    
    ofBackground(0);
//    camera.begin();
//    for (int i = 0 ; i < 20;i++){
//        ofPushMatrix();
//        ofTranslate(0,0,50*i);
//        textures.at(i).bind();
//        memoMeshs.at(i).draw();
////        createMemoMesh(textures.at(i), 1, 1)->draw();
//        textures.at(i).unbind();
//        ofPopMatrix();
//
//    }
//    camera.end();
    
//        grayImg.getTexture().setAlphaMask(alphamask.getTexture());
//        maskTexture = grayImg.getTexture();
//    
//        ofSetColor(255);
//        switch(imgIndex){
//    
//            case 0: break;
//            case 1: grayImgCopy.draw(0,0);break;
//            case 2: grayImg.draw(0,0);break;
//            case 3: colorImg.draw(0, 0); break;
//            case 4: maskTexture.draw(0,0);
//        }

    if(showBg){
        glDisable(GL_DEPTH_TEST);
        bg.resize(ofGetWidth(), ofGetHeight());
        bg.draw(0, 0, ofGetWidth(), ofGetHeight());
    }
    
    glEnable(GL_DEPTH_TEST);
    camera.begin();
    for(int i = 0 ; i< memoCount;i++){
        textures.at(i).bind();
        
        ofPushMatrix();
        ofRotate(sin(i)*180 +ofGetFrameNum()*rotations.at(i).w,rotations.at(i).x,rotations.at(i).y,rotations.at(i).z);
        ofTranslate(translations.at(i) * radius);
        maskMeshes.at(i).draw();
        ofPopMatrix();
        textures.at(i).unbind();
    }
    camera.end();


    
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
//    if(key == ' '){
//        imgIndex = (imgIndex+1) % 5;
//    }
//    
//    if(key == '1'){
//        flag1 = !flag1;
//    }
//    
//    if(key == '2'){
//        flag2 = !flag2;
//    }
//    
    if(key == 'b'){
        showBg = !showBg;
    }

    if(key == '='){
        radius = radius+0.2;
    }
    
    if(key == '-'){
        radius = radius - 0.2;
    }
    
    if(key == 'r'){
        maskMeshes.clear();
        for(int i = 0 ; i < memoCount ; i++){
            ofPixels pix;
            ofLoadImage(pix, prefix+ofToString(i+1)+".jpg");
            // Get one blob randomly
            ofxCvBlob blob =  getOneBlobFromImage(pix , JSONObjects.at(i));
            // Create Masked Mesh
            ofMesh tempMesh = createMeshByBlob(blob,JSONObjects.at(i));
            maskMeshes.push_back(tempMesh);
        }
    }
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



ofxCvBlob ofApp::getOneBlobFromImage(ofPixels& pix, ofxJSONElement& data){
    
    ofxCvContourFinder contourFinder;
    ofxCvGrayscaleImage grayImg,grayImgCopy;
    ofxCvColorImage colorImg;
   
    grayImg.allocate(pix.getWidth(),pix.getHeight());
    colorImg.setFromPixels(pix);
    grayImg = colorImg;
    
    
    float rd1,rd2,rd3;
    rd1 = pix.getWidth()*pix.getHeight()*0.01;
    rd2 = pix.getWidth()*pix.getHeight()*ofMap(data["attention"].asFloat(),20,80,0.2,1);
    rd3 = 15;

    do {
        int blobsConsidered = 1;
        grayImgCopy = grayImg;
        grayImgCopy.adaptiveThreshold(int(rd3));
        contourFinder.findContours(grayImgCopy, min(rd1,rd2), max(rd1,rd2), blobsConsidered, true, true);
        
        rd3 += 10;
    } while (contourFinder.blobs.size() < 1);
    
    ofxCvBlob blob =  contourFinder.blobs.at(0);
    ofxCvBlob blobProcessed;
    int ptCount = blob.nPts;
    int ptCountProcessed = 200;
    
    if(ptCount > ptCountProcessed){
        for(int i = 0 ; i < ptCount;){
            blobProcessed.pts.push_back(blob.pts.at(i));
            i = i + ptCount/ptCountProcessed;
        }
    }else{
        blobProcessed = blob;
    }
    
    blobProcessed.nPts = blobProcessed.pts.size();
    return blobProcessed;
}



ofMesh ofApp::createMeshByBlob(ofxCvBlob& blob , ofxJSONElement& data){
    
    ofMesh maskMesh;
    maskMesh.setMode(OF_PRIMITIVE_TRIANGLES);
    int pointNum = blob.nPts;
    int layerNum = ofRandom(8)+4;
    float layerDistance  = 50*(pow(ofRandom(1),64)*15+1);

    float frontSurfaceZ , backSurfaceZ;
    
    for(int j = 0 ; j < layerNum;j++){
        float rd = ofRandom(0.6);

        float offsetx = pow(ofRandom(1),3)*layerDistance*layerNum*(ofRandom(1)-0.5) * ofMap(data["meditation"].asFloat(),1,100,0.2,1);
        float offsety = pow(ofRandom(1),3)*layerDistance*layerNum*(ofRandom(1)-0.5) * ofMap(data["meditation"].asFloat(),1,100,0.2,1);
        
        for(int i = 0 ; i < pointNum;i++ ){
            float randomZ = (layerDistance*(j-layerNum/2))+layerDistance*(rd+0.7);
            maskMesh.addVertex(ofVec3f(blob.pts.at(i).x+offsetx, blob.pts.at(i).y+offsety, randomZ));
            maskMesh.addTexCoord(ofVec2f(blob.pts.at(i).x,blob.pts.at(i).y));
            
            if(j == 0)          frontSurfaceZ = randomZ;
            if(j == layerNum-1) backSurfaceZ = randomZ;
        }
    }
    
    
//    // Create Surface in two ends  (Center to all points) (Glich)
//    //
//    maskMesh.addVertex(ofVec3f(maskMesh.getCentroid().x,maskMesh.getCentroid().y,frontSurfaceZ));
//    maskMesh.addVertex(ofVec3f(maskMesh.getCentroid().x,maskMesh.getCentroid().y,backSurfaceZ));
//    maskMesh.addTexCoord(ofVec2f(maskMesh.getCentroid().x,maskMesh.getCentroid().y));
//    maskMesh.addTexCoord(ofVec2f(maskMesh.getCentroid().x,maskMesh.getCentroid().y));
//    
//    int maskVerticesNum = maskMesh.getNumVertices();
//    for(int i = 0 ; i < pointNum-1;i++){
//        maskMesh.addIndex(maskVerticesNum-2);
//        maskMesh.addIndex(i);
//        maskMesh.addIndex(i+1);
//        
//        maskMesh.addIndex(maskVerticesNum-1);
//        maskMesh.addIndex((layerNum-1)*blob.nPts+i);
//        maskMesh.addIndex((layerNum-1)*blob.nPts+i+1);
//    }
    

 
    // Create Surface in two ends (First point to the rest points) (No glich)
    //
    for(int i = 0 ; i < pointNum-2;i++){
        maskMesh.addIndex(0);
        maskMesh.addIndex(i+1);
        maskMesh.addIndex(i+2);
        
        maskMesh.addIndex((layerNum-1)*blob.nPts);
        maskMesh.addIndex((layerNum-1)*blob.nPts+i+1);
        maskMesh.addIndex((layerNum-1)*blob.nPts+i+2);
    }
    
    
    // Create Color lines
    //
    for(int j = 0 ; j < layerNum-1;j++){
        for(int i = 0 ; i < pointNum-1; i++){
            maskMesh.addIndex(    j*pointNum + i);
            maskMesh.addIndex(    j*pointNum + i+1);
            maskMesh.addIndex((j+1)*pointNum + i+1);
            
            maskMesh.addIndex(    j*pointNum + i);
            maskMesh.addIndex((j+1)*pointNum + i);
            maskMesh.addIndex((j+1)*pointNum + i+1);
        }
        
        maskMesh.addIndex((j+1)*pointNum-1);
        maskMesh.addIndex(    j*pointNum);
        maskMesh.addIndex((j+1)*pointNum);
        
        maskMesh.addIndex((j+1)*pointNum-1);
        maskMesh.addIndex((j+2)*pointNum-1);
        maskMesh.addIndex((j+1)*pointNum);
        
    }
    return maskMesh;
}


//
//ofMesh ofApp::createMemoMesh(ofTexture &tex, float n1, float n2){
//    ofMesh theMesh;
//    float h = tex.getHeight();
//    float w = tex.getWidth();
//    float hsegs = h/n1;
//    float wsegs = w/n2;
//
//    theMesh.setMode(OF_PRIMITIVE_TRIANGLES);
//    theMesh.enableTextures();
//
//    for(int i = 0 ; i < n1 ; i++){
//        for (int j = 0 ; j < n2 ; j++){
//            theMesh.addVertex(ofVec3f(-w/2 + j * wsegs, h/2 - i * hsegs, (ofRandom(100)-5)));
//            theMesh.addVertex(ofVec3f(-w/2 + (j+1) * wsegs, h/2 - i * hsegs, +(ofRandom(100)-5)));
//            theMesh.addVertex(ofVec3f(-w/2 + (j+1) * wsegs, h/2 - (i+1) * hsegs, +(ofRandom(100)-5)));
//            theMesh.addVertex(ofVec3f(-w/2 + j * wsegs, h/2 - (i+1) * hsegs, +(ofRandom(100)-5)));
//
//            theMesh.addIndex((i*n2+j)*4+0);
//            theMesh.addIndex((i*n2+j)*4+1);
//            theMesh.addIndex((i*n2+j)*4+2);
//            theMesh.addIndex((i*n2+j)*4+0);
//            theMesh.addIndex((i*n2+j)*4+3);
//            theMesh.addIndex((i*n2+j)*4+2);
//
//            if(ofRandom(1)>0){
//                theMesh.addTexCoord(ofVec2f(j * wsegs , i * hsegs));
//                theMesh.addTexCoord(ofVec2f((j+1) * wsegs , i * hsegs));
//                theMesh.addTexCoord(ofVec2f((j+1) * wsegs , (i+1) * hsegs));
//                theMesh.addTexCoord(ofVec2f(j * wsegs , (i+1) * hsegs));
//            }else{
//                theMesh.addTexCoord(ofVec2f(j * wsegs , i * hsegs));
//                theMesh.addTexCoord(ofVec2f((j) * wsegs , i * hsegs));
//                theMesh.addTexCoord(ofVec2f((j) * wsegs , (i) * hsegs));
//                theMesh.addTexCoord(ofVec2f(j * wsegs , (i) * hsegs));
//            }
//        }
//    }
//    return theMesh;
//
//}

