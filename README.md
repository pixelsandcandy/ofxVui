## View + UI System for openFrameworks
###### *DEPENDENCIES*
* **ofxEasing** » https://github.com/arturoc/ofxEasing
* **ofxWinTouchHook** » https://github.com/trentbrooks/ofxWinTouchHook (optional)
  1. add Preprocessor Macro **USING_ofxWinTouchHook**
  2. call **VUI::EnableTouch()**
***

###### *QUICK NOTE* 😅

So everything works but sorry the code is a little bit of spaghetti at the moment. ofxVUI was originally split into two different addons (one for ViewManager and one for GUI) but ended up merging the two - mainly so that GUI Mouse/Touch events still work when the View is scaled/rotated but also because I wanted to simplify things. I'll slowly clean up the code but for now have been focusing on making sure that everything works as intended, creating example projects to learn from, and adding useful features.


***
# **ofxVui**
### View Manager
*VUI Mouse/Touch Events still work when scaling and rotating view*
```
• Resolution
• Scale
• Rotation
```
  
  
### UI System
```
Elements
  • Element
  • Element->MakeToggle()
  • ToggleGroup
  • Text
  • TextField
    
Animation Tween
  • x
  • y
  • width
  • height
  • scale
  • opacity
  • rotation
  • ease/easing
  
Animation Events
  • ANIMATE_START
  • ANIMATE_STEP
  • ANIMATE_COMPLETE
    
Mouse Events
  • MOUSE_OVER
  • MOUSE_OUT
  • MOUSE_PRESSED
  • MOUSE_MOVED
  • MOUSE_DRAGGED
  • MOUSE_RELEASED
  • MOUSE_CLICK
  • MOUSE_DOUBLE_CLICK
    
Touch Events
  • TOUCH_DOWN
  • TOUCH_UP
  • TOUCH_TAP
  • TOUCH_DOUBLE_TAP
    
Other Events
  • STATE_CHANGE
  • TEXT_CHANGE <Text/TextField>
  • VALUE_CHANGE <Element->MakeToggle()>
  • TOGGLE_CHANGE <ToggleGroup>
  • SUBMIT <TextField>
  • FOCUS <TextField>
  • UNFOCUS <TextField>
    
UI States
  • STATE_UP
  • STATE_OVER
  • STATE_DOWN
  
UI StyleSheet  
  • Manages fonts
  • Manages image assets
  
UI StyleSheet: Styles
  • width
  • height
  • scale
  • opacity
  • rotation
  • backgroundImage
  • backgroundColor
  • anchorPoint
  • offset
  • color <Text/TextField>
  • padding <Text/TextField>
  • textAlign <Text/TextField>
  • font <Text/TextField>
```
***
## Code Examples
There are a bunch of example projects to check out but here are some basic examples.

### UI Animation
*For more info checkout the **example-Animation** project/code*
* *Animate( Float seconds, String params );*

```c++
// basic
elementPtr->Animate(1.25, "{x: 160, width: 150, height: 50, scale: 1.75, rotation: -135, opacity: .75, ease: Elastic.easeOut }");

// add listener - method A
elementPtr->Animate(1.25, "{x: 160, width: 150, height: 50, scale: 1.75, rotation: -135, opacity: .75, ease: Elastic.easeOut }", this, &ofApp::vuiEventHandler);

// add listener - method B
Tween* tweenPtr = elementPtr->Animate(1.25, "{x: 160, width: 150, height: 50, scale: 1.75, rotation: -135, opacity: .75, ease: Elastic.easeOut }");
ofAddListener( tweenPtr->onStep, this, &ListenerClass::vuiEventHandler );
ofAddListener( tweenPtr->onComplete, this, &ListenerClass::vuiEventHandler );

// eventHandler
ListenerClass::vuiEventHandler(vuiEventArgs& evt){
  if ( evt.element == elementPtr ){
    if ( evt.eventType == VUI_EVENT_ANIMATE_COMPLETE ) /* do stuff */;
    else if ( evt.eventType == VUI_EVENT_ANIMATE_STEP ){
      ofLog() << evt.tween->GetProgress();
    }
  }
}

/* easing functions

  Back.easeIn
  Back.easeOut
  Back.easeInOut
  Bounce.easeIn
  Bounce.easeOut
  Bounce.easeInOut
  Circ.easeIn
  Circ.easeOut
  Circ.easeInOut
  Cubic.easeIn
  Cubic.easeOut
  Cubic.easeInOut
  Elastic.easeIn
  Elastic.easeOut
  Elastic.easeInOut
  Exp.easeIn
  Exp.easeOut
  Exp.easeInOut
  Linear.easeIn
  Linear.easeOut
  Linear.easeInOut
  Linear.easeNone
  Quad.easeIn
  Quad.easeOut
  Quad.easeInOut
  
*/
  
```

### UI Events
*For more info checkout the **example-Events** project/code*
```c++
// add listener
ofAddListener( elementPtr->onMouseClick, this, &ListenerClass::vuiEventHandler );

// eventHandler
ListenerClass::vuiEventHandler(vuiEventArgs& evt){
  if ( evt.element == elementPtr ){
    if ( evt.eventType == VUI_EVENT_MOUSE_CLICK ) /* do stuff */;
  }
}

/* events

  VUI_EVENT_MOUSE_OVER
  VUI_EVENT_MOUSE_OUT
  VUI_EVENT_MOUSE_PRESSED
  VUI_EVENT_MOUSE_MOVED
  VUI_EVENT_MOUSE_DRAGGED
  VUI_EVENT_MOUSE_RELEASED
  VUI_EVENT_MOUSE_CLICK
  VUI_EVENT_MOUSE_DOUBLE_CLICK

  VUI_EVENT_TOUCH_DOWN
  VUI_EVENT_TOUCH_UP
  VUI_EVENT_TOUCH_TAP
  VUI_EVENT_TOUCH_DOUBLE_TAP

  VUI_EVENT_STATE_CHANGE
  VUI_EVENT_TEXT_CHANGE
  VUI_EVENT_VALUE_CHANGE
  VUI_EVENT_TOGGLE_CHANGE
  VUI_EVENT_SUBMIT

  VUI_EVENT_FOCUS
  VUI_EVENT_UNFOCUS

  VUI_EVENT_ANIMATE_COMPLETE
  VUI_EVENT_ANIMATE_STEP
  VUI_EVENT_ANIMATE_START
  
*/

```

### UI Styling
*For more info checkout the **example-StylingElements** project/code*
```c++
StyleSheet* ss;
Element* buttonA;
Element* buttonB;
Text* label;

// --------------------------------------------
void setup(){
  string styles = R"(
    [Images>
      btn-up: path/to/image.jpg;
      btn-over: path/to/image-over.jpg;
      btn-down: path/to/image-down.jpg;
    ]
    
    [.button>
      width: 120;
      height: 36;
      bgImage: btn-up, FIT;
      
      &:over{
        bgImage: btn-over, FIT;
      }
      
      &:down{
        bgImage: btn-down, FIT;
      }
    ]
    
    [#buttonB>
      scale: 1.25;
      opacity: .9;
    ]
    
    [.text>
      font: path/to/fontfile.ttf,16;
      color: #dddddd;
      padding: 10;
      bg: #ffffff;
    ]
  )";

  ss = new StyleSheet( styles );

  label = new Text( 20, 20, ss, ".text" );
  // true = automatically set width/height to text
  label->SetText( "Stuff", true );
  
  buttonA = new Element( 20, 40, ss, ".button" );
  buttonB = new Element( 20, 80, ss, ".button", "#buttonB" );
  
}

// --------------------------------------------
void draw(){
  label->Render();
  buttonA->Render();
  buttonB->Render();
}


```

### View Manager
*For more info checkout the **example-ViewManager** project/code*

```c++
// ---------------------------------------- main.cpp
VUI::SetResolution(1080,1920,.4);
// VUI::RotateView( VUI_ROTATE_90_CCW );
ofSetupOpenGL(VUI::GetWindowWidth(),VUI::GetWindowHeight(),OF_WINDOW);


// ---------------------------------------- create View
#include "ofxVui.h"
using namespace VUI;

class MyView : public View {
  /* stuff */
}


// ---------------------------------------- ofApp.cpp
#include "MyView.h"

void setup(){
  //VUI::EnableTouch();
  
  VUI::AddView( "view-name", new MyView() );
  
  // can call from anywhere
  VUI::SetView( "view-name" );
}

void draw(){
  VUI::Render();
}

```


