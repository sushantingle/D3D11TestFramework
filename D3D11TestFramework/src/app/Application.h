#pragma once
#include "../dxcore/core/Window.h"
#include "../dxcore/core/Scene.h"
#include "TestObject.h"

class Application {
public:
	Application() : wnd(props) {
		std::unique_ptr<DXCore::Scene> scene = std::make_unique<DXCore::Scene>();
		TestObject* obj = new TestObject(0);
		scene->AddObject(obj);

		wnd.RegisterScene(std::move(scene));
	}

	int Go() {
		while (true)
		{
			if (const auto ecode = wnd.ProcessMessages())
			{
				// if return optional has value, means we're quitting so return exit code
				return *ecode;
			}
			wnd.OnUpdate();
		}
		return 0;
	}

private:
	DXCore::WindowProps props;
	DXCore::Window wnd;
};


