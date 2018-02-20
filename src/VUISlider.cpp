#include "ofxVui.h"
#include "VUISlider.h"

namespace VUI {
    
    void Slider::SetBar( StyleSheet *ss, string styleBarSelector, string styleBarBgSelector){
        if ( bar == NULL ){
            SetInteractive(false);
            
            barContainer = new Element(padding.left, padding.top, ss, styleBarBgSelector );
            //barContainer->SetInteractive(false);
            AddChild(barContainer);
            
            bar = new Element(0,0, ss, styleBarSelector );
            bar->SetInteractive(false);
            barContainer->AddChild(bar);
            
            ofAddListener( barContainer->onMouseReleased, this, &Slider::_vuiEventHandler );
            ofAddListener( barContainer->onMousePressed, this, &Slider::_vuiEventHandler );
            ofAddListener( barContainer->onMouseDragged, this, &Slider::_vuiEventHandler );
            ofAddListener( barContainer->onMouseClick, this, &Slider::_vuiEventHandler );
            
            ofAddListener( barContainer->onMouseOver, this, &Slider::_vuiEventHandler );
            ofAddListener( barContainer->onMouseOut, this, &Slider::_vuiEventHandler );
            
        } else {
            bar->Setup(0,0, ss, styleBarSelector );
            barContainer->Setup(padding.left,padding.top, ss, styleBarBgSelector );
        }
        UpdateBarStyle();
    }
    
};
