#include "ofxVui.h"
#include "VUISlider.h"

namespace VUI {
    
    void Slider::SetBar( StyleSheet *ss, string styleBarSelector, string styleBarBgSelector){
        if ( bar == NULL ){
            barContainer = new Element(0,0, ss, styleBarBgSelector );
            barContainer->SetInteractive(false);
            AddChild(barContainer);
            
            bar = new Element(0,0, ss, styleBarSelector );
            bar->SetInteractive(false);
            barContainer->AddChild(bar);
            
            ofAddListener( onMouseReleased, this, &Slider::_vuiEventHandler );
            ofAddListener( onMousePressed, this, &Slider::_vuiEventHandler );
            ofAddListener( onMouseDragged, this, &Slider::_vuiEventHandler );
            ofAddListener( onMouseClick, this, &Slider::_vuiEventHandler );
            
            ofAddListener( onMouseOver, this, &Slider::_vuiEventHandler );
            ofAddListener( onMouseOut, this, &Slider::_vuiEventHandler );
            
        } else {
            bar->Setup(0,0, ss, styleBarSelector );
            barContainer->Setup(0,0, ss, styleBarBgSelector );
        }
        UpdateBarStyle();
    }
    
};
