#pragma once
#include "../DxCore.h"
#include "GameObject.h"

namespace DXCore{
    class Scene{
    public:
        Scene();
        virtual ~Scene();

        void AddObject(GameObject* obj);
        void RemoveObject(unsigned int id);

        void OnUpdate();

    private:
        std::vector<GameObject*> m_Container;
    };
}
