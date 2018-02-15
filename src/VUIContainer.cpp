#include "ofxVui.h"
#include "VUIContainer.h"

namespace VUI {
    
    void Container::SetScrollbar( StyleSheet *ss, string styleSelector){
        scrollbar->Setup("calc(100%-" + ofToString(scrollbarPadding+scrollbarWidth) + ")", scrollbarPadding, ss,styleSelector);
        
        UpdateScrollbarStyle();
    }
    
    void Container::Setup(){
        horzContainer = new Element(0, 0 );
        horzContainer->SetInteractive(false);
        
        vertContainer = new Element(0, 0 );
        vertContainer->SetInteractive(false);
        
        container = new Element(0,0 );
        container->SetInteractive(false);
        
        UpdateContainerStyle();
        
        horzContainer->AddChild(vertContainer);
        vertContainer->AddChild(container);
        Element::AddChild(horzContainer);
        
        scrollbar = new Element("calc(100%-" + ofToString(scrollbarPadding+scrollbarWidth) + ")", scrollbarPadding, VUI::GetStyleSheet("VUIStyleSheet"), ".Container-scrollbar" );
        scrollbar->SetInteractive(false);
        scrollbar->SetOpacity(0);
        Element::AddChild(scrollbar);
        
        UpdateContainerStyle();
    }
    
    void Container::_vuiEventHandler(vuiEventArgs& evt){
#ifdef USING_ofxTouchPadScroll
        if ( !IsMouseInside() ) return;
        
        if ( evt.eventType == VUI_EVENT_TOUCHPAD_SCROLL_END ){
            scrollbar->Animate(.3,"{delay:.4,opacity:0}");
            return;
        }
        
        if ( evt.eventType == VUI_EVENT_TOUCHPAD_SCROLL_START ){
            scrollbar->StopTween();
            startScrollPos = container->GetPosition();
            scrollbar->SetOpacity(1.0);
        } else if ( evt.eventType == VUI_EVENT_TOUCHPAD_SCROLL_INERTIA || evt.eventType == VUI_EVENT_TOUCHPAD_SCROLL ) {
            if ( evt.eventType == VUI_EVENT_TOUCHPAD_SCROLL_INERTIA ) startScrollPos = container->GetPosition();
            tempScrollPos.set(startScrollPos);
            
            tempScrollPos.y += evt.touchPadScroll.y*VUI::divideDpi;
            
            if ( tempScrollPos.y < -scrollDist.y ) tempScrollPos.y = -scrollDist.y;
            else if ( tempScrollPos.y > padding.top*VUI::dpi ) tempScrollPos.y = padding.top*VUI::dpi;
            
            container->SetPositionY(tempScrollPos.y*VUI::divideDpi);
            
            float perc = ((tempScrollPos.y)/(-scrollDist.y+padding.top*VUI::dpi));
            if ( perc < 0.0 ) perc = 0.0;
            
            scrollbar->SetPositionY( (perc*scrollbarDist) + scrollbarPadding );
            
        }
#endif
        
    }
    
};
