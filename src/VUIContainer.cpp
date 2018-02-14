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
    
};
