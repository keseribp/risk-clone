#ifndef CANVAS_H
#define CANVAS_H

#include "uiObject.h"

class Canvas : public UIObject
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
        Canvas() : UIObject() {}
        
        Canvas(GLFWwindow * window) : UIObject(window) {}
        
        Canvas(GLFWwindow * window, UIObject * parent) : UIObject(window, parent) {}
        
        Canvas(GLFWwindow * window, void (* callbackPtr)()) : UIObject(window, callbackPtr) {}
        
        Canvas(GLFWwindow * window, UIObject * parent, void (* callbackPtr)()) : UIObject(window, parent, callbackPtr) {}
        
        Canvas(GLFWwindow * window, void (UIObject::* internalCallbackPtr)()) : UIObject(window, internalCallbackPtr) {}
        
        Canvas(GLFWwindow * window, UIObject * parent, void (UIObject::* internalCallbackPtr)()) : UIObject(window, parent, internalCallbackPtr) {}
        
        ~Canvas() {}
};

#endif
