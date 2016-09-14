#ifndef LABEL_H
#define LABEL_H

#include "uiObject.h"
#include <string.h>
#include <string>

class Label : public UIObject
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
        Label() : UIObject() {}
        
        Label(GLFWwindow * window) : UIObject(window) {}
        
        Label(GLFWwindow * window, UIObject * parent) : UIObject(window, parent) {}
        
        Label(GLFWwindow * window, void (* callbackPtr)()) : UIObject(window, callbackPtr) {}
        
        Label(GLFWwindow * window, UIObject * parent, void (* callbackPtr)()) : UIObject(window, parent, callbackPtr) {}
        
        Label(GLFWwindow * window, void (UIObject::* internalCallbackPtr)()) : UIObject(window, internalCallbackPtr) {}
        
        Label(GLFWwindow * window, UIObject * parent, void (UIObject::* internalCallbackPtr)()) : UIObject(window, parent, internalCallbackPtr) {}
        
        ~Label() {}
        
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
