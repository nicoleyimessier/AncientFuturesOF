#pragma once

#include "Page.h"
#include "ofMain.h"

class PageIntro : public Page {
  public:
    PageIntro();
    ~PageIntro();

    void setup(); 
    void update( float dt ); 
    void draw(); 
    

  private:
};