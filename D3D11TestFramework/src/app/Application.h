#pragma once
#include "../dxcore/core/Window.h"


class Application {
public:
	Application() : wnd(props) {}

	int Go() {
		while (true)
		{
			if (const auto ecode = wnd.ProcessMessages())
			{
				// if return optional has value, means we're quitting so return exit code
				return *ecode;
			}
		}
		return 0;
	}

private:
	DXCore::WindowProps props;
	DXCore::Window wnd;
};


