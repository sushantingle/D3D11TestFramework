//
// Created by Sushant on 11-07-2021.
//

#ifndef OPENGLESFRAMEWORK_TESTOBJECT_H
#define OPENGLESFRAMEWORK_TESTOBJECT_H
#include "../DXCore/Core/GameObject.h"

class TestObject : public DXCore::GameObject {
public:
    TestObject(unsigned  int id):DXCore::GameObject(id){}
    ~TestObject() {}

    void OnCreate() override;
    void OnDestroy() override;
    void OnUpdate() override;

private:

};

#endif //OPENGLESFRAMEWORK_TESTOBJECT_H
