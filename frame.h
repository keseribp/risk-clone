#ifndef FRAME_H
#define FRAME_H

#include "uiObject.h"

class Frame : public UIObject
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
    
        Frame() : UIObject() {}
        
        Frame(GLFWwindow * window) : UIObject(window) {}
        
        Frame(GLFWwindow * window, UIObject * parent) : UIObject(window, parent) {}
        
        Frame(GLFWwindow * window, void (* callbackPtr)()) : UIObject(window, callbackPtr) {}
        
        Frame(GLFWwindow * window, UIObject * parent, void (* callbackPtr)()) : UIObject(window, parent, callbackPtr) {}
        
        Frame(GLFWwindow * window, void (UIObject::* internalCallbackPtr)()) : UIObject(window, internalCallbackPtr) {}
        
        Frame(GLFWwindow * window, UIObject * parent, void (UIObject::* internalCallbackPtr)()) : UIObject(window, parent, internalCallbackPtr) {}
        
        ~Frame() {}
        
        /*
        Frame();
        
        Frame(GLFWWindow * window);
        
        Frame(GLFWWindow * window, UIObject * parent);
        
        Frame(GLFWwindow * window, void (* callbackPtr)());
        
        Frame(GLFWwindow * window, UIObject * parent, void (* callbackPtr)());
        
        Frame(GLFWwindow * window, void (UIObject::* internalCallbackPtr)());
        
        Frame(GLFWwindow * window, UIObject * parent, void (UIObject::* internalCallbackPtr)());
        
        ~Frame();
        */
};

#endif
