#ifndef PANEL_H
#define PANEL_H

#include "uiObject.h"

class Panel : public UIObject
{
    private:
        //UIObject m_parent;
        //bool m_isHidden;
        //float m_centerX;
        //float m_centerY;
        //float m_sizeX;
        //float m_sizeY;
        //float m_inactiveColor[3];
        //float m_highlightedColor[3];
        //float m_pressedColor[3];
        //void (*m_callbackPtr)();
        //UIObjectState m_state;
        
    public:
        Panel() : UIObject() {}
        
        Panel(GLFWwindow * window) : UIObject(window) {}
        
        Panel(GLFWwindow * window, UIObject * parent) : UIObject(window, parent) {}
        
        Panel(GLFWwindow * window, void (* callbackPtr)()) : UIObject(window, callbackPtr) {}
        
        Panel(GLFWwindow * window, UIObject * parent, void (* callbackPtr)()) : UIObject(window, parent, callbackPtr) {}
        
        Panel(GLFWwindow * window, void (UIObject::* internalCallbackPtr)()) : UIObject(window, internalCallbackPtr) {}
        
        Panel(GLFWwindow * window, UIObject * parent, void (UIObject::* internalCallbackPtr)()) : UIObject(window, parent, internalCallbackPtr) {}
        
        ~Panel() {}
};

#endif
