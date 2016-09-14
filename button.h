#ifndef BUTTON_H
#define BUTTON_H

#include "uiObject.h"
#include <string.h>
#include <string>

/*
enum ButtonStatus {
    BUTTON_INACTIVE = 0,
    BUTTON_HIGHLIGHTED = 1,
    BUTTON_PRESSED = 2,
    BUTTON_HIDDEN = 3
};
*/

class Button : public UIObject
{
    private:
        //UIObject m_parent;
        std::string m_label;
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
        
    //private:
    //    void callback() {(*m_callbackPtr)();}
        
    public:
        Button() : UIObject() {}
        
        Button(GLFWwindow * window) : UIObject(window) {}
        
        Button(GLFWwindow * window, UIObject * parent) : UIObject(window, parent) {}
        
        Button(GLFWwindow * window, void (* callbackPtr)()) : UIObject(window, callbackPtr) {}
        
        Button(GLFWwindow * window, UIObject * parent, void (* callbackPtr)()) : UIObject(window, parent, callbackPtr) {}
        
        Button(GLFWwindow * window, void (UIObject::* internalCallbackPtr)()) : UIObject(window, internalCallbackPtr) {}
        
        Button(GLFWwindow * window, UIObject * parent, void (UIObject::* internalCallbackPtr)()) : UIObject(window, parent, internalCallbackPtr) {}
        
        ~Button() {}
        
        void setLabel(std::string label)
        {
            m_label = label;
        }
        
        std::string getLabel()
        {
            return m_label;
        }
};

#endif
