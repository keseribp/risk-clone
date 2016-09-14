#ifndef RADIOGROUP_H
#define RADIOGROUP_H

#include "uiObject.h"
#include "button.h"
#include "label.h"
#include <array>
#include <vector>
#include <memory>
#include <assert.h>

class RadioGroup : public UIObject
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
        //void (UIObject::* m_internalCallbackPtr)();
        //UIObjectState m_state;
        std::vector<UIObject *> m_members;
        std::vector<void *> m_memberCallbacks;
        
        UIObject * m_childPtr;
        
    public:
        RadioGroup() : UIObject() {}
        
        RadioGroup(GLFWwindow * window) : UIObject(window) {}
        
        RadioGroup(GLFWwindow * window, UIObject * parent) : UIObject(window, parent) {}
        /*
        RadioGroup(GLFWwindow * window, UIObject * parent, std::vector<UIObject *> & members) : UIObject(window, parent)
        {
            initialize(window, parent, members);
        }
        */
        /*
        RadioGroup(GLFWwindow * window, UIObject * parent, Button * members, uint size) : UIObject(window, parent)
        {
            initialize(window, parent, members, size);
        }
        RadioGroup(GLFWwindow * window, UIObject * parent, Label * members, uint size) : UIObject(window, parent)
        {
            initialize(window, parent, members, size);
        }
        */
        template<class T>
        RadioGroup(GLFWwindow * window, UIObject * parent, T * members, uint size) : UIObject(window, parent)
        {
            assert((std::is_base_of<UIObject, T>::value));//TODO or static_assert ?
            
            initialize(window, parent, members, size);
        }
        
        /*
        RadioGroup(GLFWwindow * window, UIObject * parent, std::vector<UIObject> & members) : UIObject(window, parent)
        {
            initialize(window, parent, members);
        }
        */
        /*
        template<std::size_t SIZE>
        RadioGroup(GLFWwindow * window, UIObject * parent, std::array<UIObject, SIZE> & members) : UIObject(window, parent)
        {
            initialize(window, parent, members);
        }
        
        template<uint SIZE>
        RadioGroup(GLFWwindow * window, UIObject * parent, std::array<UIObject, SIZE> & members) : UIObject(window, parent)
        {
            initialize(window, parent, members);
        }
        */
        ~RadioGroup() {}
        /*
        void initialize(GLFWwindow * window, UIObject * parent, std::vector<UIObject *> & members)
        {
            for (UIObject * m : members)
            {
                includeMember(m);
            }
        }
        */
        
        /*
        void initialize(GLFWwindow * window, UIObject * parent, Button * members, uint size)
        {
            for (uint i = 0; i < size; i++)
            {
                //printf("&members[%i] = %p\n", i, &members[i]);
                //includeMember(&members[i]);
                printf("std::addressof(members[%i]) = %p\n", i, std::addressof(members[i]));
                includeMember(std::addressof(members[i]));
            }
        }
        void initialize(GLFWwindow * window, UIObject * parent, Label * members, uint size)
        {
            for (uint i = 0; i < size; i++)
            {
                //printf("&members[%i] = %p\n", i, &members[i]);
                //includeMember(&members[i]);
                printf("std::addressof(members[%i]) = %p\n", i, std::addressof(members[i]));
                includeMember(std::addressof(members[i]));
            }
        }
        */
        template<class T>
        void initialize(GLFWwindow * window, UIObject * parent, T * members, uint size)
        {
            assert((std::is_base_of<UIObject, T>::value));//TODO or static_assert ?
            for (uint i = 0; i < size; i++)
            {
                //printf("&members[%i] = %p\n", i, &members[i]);
                //includeMember(&members[i]);
                //printf("std::addressof(members[%i]) = %p\n", i, std::addressof(members[i]));
                //printf("std::addressof(*(members + %i)) = %p\n", i, std::addressof(*(members + i)));
                //includeMember(std::addressof(*(members + i)));
                includeMember(std::addressof(members[i]));
            }
        }
        
        /*
        void initialize(GLFWwindow * window, UIObject * parent, std::vector<UIObject> & members)
        {
            for (UIObject m : members)
            {
                includeMember(&m);
            }
        }
        */
        /*
        template<std::size_t SIZE>
        void initialize(GLFWwindow * window, UIObject * parent, std::array<UIObject, SIZE> & members)
        {
            for (UIObject m : members)
            {
                includeMember(&m);
            }
        }
        
        template<uint SIZE>
        void initialize(GLFWwindow * window, UIObject * parent, std::array<UIObject, SIZE> & members)
        {
            for (UIObject m : members)
            {
                includeMember(&m);
            }
        }
        */
        void callback(UIObject * uiObject)
        {
            for (UIObject * m : m_members)
            {
                m->setState(UIOBJECT_INACTIVE);
                if (m == uiObject) m->setState(UIOBJECT_PRESSED); 
            }           
        }
        
        //void newCallback()
        void wrappedCallback()
        {
            m_parent->callback(m_childPtr);
            m_childPtr->callback();
        }    
        
        void includeMember(UIObject * uiObject)
        {
            m_members.push_back(uiObject);
            uiObject->setParent(this);
            m_childPtr = uiObject;
            uiObject->setCallback(&UIObject::wrappedCallback);
        }
        //TODO should have a remove and some getters as well
};

#endif
