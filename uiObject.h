#ifndef UIOBJECT_H
#define UIOBJECT_H

#include <GL/glew.h>
#include <GLFW/glfw3.h>
//#include <functional>

typedef unsigned int uint;

enum UIObjectState {
    UIOBJECT_INACTIVE = 0,
    UIOBJECT_HIGHLIGHTED = 1,
    UIOBJECT_PRESSED = 2
};

class UIObject
{
    protected:
        GLFWwindow * m_window;
        UIObject * m_parent;
        bool m_isHidden;
        float m_centerX;
        float m_centerY;
        float m_sizeX;
        float m_sizeY;
        float m_inactiveColor[3];
        float m_highlightedColor[3];
        float m_pressedColor[3];
        void (* m_callbackPtr)();
        void (UIObject::* m_internalCallbackPtr)();
        UIObjectState m_state;
        
    public:
        UIObject()
        {
            m_parent = this;
            m_window = nullptr;
            m_callbackPtr = nullFunction;
            m_internalCallbackPtr = &UIObject::internalNullFunction;
        }
    
        UIObject(GLFWwindow * window)
        {
            initialize(window);
        }
        
        UIObject(GLFWwindow * window, UIObject * parent)
        {
            initialize(window, parent);
        }
        
        UIObject(GLFWwindow * window, void (*callbackPtr)())
        {
            initialize(window, callbackPtr);
        }
        
        UIObject(GLFWwindow * window, UIObject * parent, void (*callbackPtr)())
        {
            initialize(window, parent, callbackPtr);
        }
        
        UIObject(GLFWwindow * window, void (UIObject::* internalCallbackPtr)())
        {
            initialize(window, internalCallbackPtr);
        }
        
        UIObject(GLFWwindow * window, UIObject * parent, void (UIObject::* internalCallbackPtr)())
        {
            initialize(window, parent, internalCallbackPtr);
        }
        
        void initialize(GLFWwindow * window)
        {
            m_parent = this;
            m_window = window;
            m_callbackPtr = nullFunction;
            m_internalCallbackPtr = &UIObject::internalNullFunction;
        }
        
        void initialize(GLFWwindow * window, UIObject * parent)
        {
            m_parent = parent;
            m_window = window;
            m_callbackPtr = nullFunction;
            m_internalCallbackPtr = &UIObject::internalNullFunction;
        }
        
        void initialize(GLFWwindow * window, void (*callbackPtr)())
        {
            m_parent = this;
            m_window = window;
            m_callbackPtr = callbackPtr;
            m_internalCallbackPtr = &UIObject::wrappedCallback;
        }
        
        void initialize(GLFWwindow * window, UIObject * parent, void (*callbackPtr)())
        {
            m_parent = parent;
            m_window = window;
            m_callbackPtr = callbackPtr;
            m_internalCallbackPtr = &UIObject::wrappedCallback;
        }
        
        void initialize(GLFWwindow * window, void (UIObject::* internalCallbackPtr)())
        {
            m_parent = this;
            m_window = window;
            m_callbackPtr = nullFunction;
            m_internalCallbackPtr = internalCallbackPtr;
        }
        
        void initialize(GLFWwindow * window, UIObject * parent, void (UIObject::* internalCallbackPtr)())
        {
            m_parent = parent;
            m_window = window;
            m_callbackPtr = nullFunction;
            m_internalCallbackPtr = internalCallbackPtr;
        }
        
        void callback()
        {
            (this->*m_internalCallbackPtr)();
        }
        
        void callback(UIObject * uiObject)
        {
            (this->*m_internalCallbackPtr)();
        }
        
        void wrappedCallback()
        {
            m_callbackPtr();
        }
        
        static void nullFunction() {}
        
        void internalNullFunction() {}
        
        void setCallback(void (* callbackPtr)()) 
        {
            m_callbackPtr = callbackPtr;
            m_internalCallbackPtr = &UIObject::wrappedCallback;
        }
        
        void setCallback(void (UIObject::* internalCallbackPtr)()) 
        {
            m_callbackPtr = nullFunction;
            m_internalCallbackPtr = internalCallbackPtr;
        }
        
        void setParent(UIObject * parent)
        {
            m_parent = parent;
        }
        
        void setIsHidden(bool isHidden)
        {
            m_isHidden = isHidden;
        }
        
        void setCenterXY(float centerX, float centerY)
        {
            m_centerX = centerX;
            m_centerY = centerY;
        }
        
        void setSizeXY(float sizeX, float sizeY)
        {
            m_sizeX = sizeX;
            m_sizeY = sizeY;
        }
        
        void setState(UIObjectState state)
        {
            m_state = state;
        }
        
        void setInactiveColor(float inactiveColor[3])
        {
            m_inactiveColor[0] = inactiveColor[0];
            m_inactiveColor[1] = inactiveColor[1];
            m_inactiveColor[2] = inactiveColor[2];
        }
        
        void setHighlightedColor(float highlightedColor[3])
        {
            m_highlightedColor[0] = highlightedColor[0];
            m_highlightedColor[1] = highlightedColor[1];
            m_highlightedColor[2] = highlightedColor[2];
        }
        
        void setPressedColor(float pressedColor[3])
        {
            m_pressedColor[0] = pressedColor[0];
            m_pressedColor[1] = pressedColor[1];
            m_pressedColor[2] = pressedColor[2];
        }
};

#endif
