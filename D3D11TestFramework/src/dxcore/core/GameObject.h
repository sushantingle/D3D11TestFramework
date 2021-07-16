#pragma once
//#include "../core/Window.h"

namespace DXCore{
    class GameObject{
    public:
        GameObject(unsigned int id) : m_Id(id) {}
        virtual ~GameObject() = default;

        virtual void OnCreate() {}
        virtual void OnDestroy() {}
        virtual void OnUpdate() {}

        const unsigned int GetID() const {return m_Id;}
    protected:
        unsigned int m_Id;

    };
}