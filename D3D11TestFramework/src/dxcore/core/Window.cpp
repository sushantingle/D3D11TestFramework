#include "Window.h"
#include <iostream>

namespace DXCore {

	Window* Window::m_Instance = nullptr;

	Window::Window(const WindowProps& props)
	{
		hInst = GetModuleHandle(nullptr);

		WNDCLASSEX wc = { 0 };
		wc.cbSize = sizeof(wc);
		wc.style = CS_OWNDC;
		wc.lpfnWndProc = HandleMsgSetup;
		wc.cbClsExtra = 0;
		wc.cbWndExtra = 0;
		wc.hInstance = hInst;
		wc.hIcon = static_cast<HICON>(LoadImage(
			hInst, nullptr,
			IMAGE_ICON, 32, 32, 0
		));
		wc.hCursor = nullptr;
		wc.hbrBackground = nullptr;
		wc.lpszMenuName = nullptr;
		wc.lpszClassName = "Window class";
		wc.hIconSm = static_cast<HICON>(LoadImage(
			hInst, MAKEINTRESOURCE(IDI_APPLICATION),
			IMAGE_ICON, 16, 16, 0
		));
		RegisterClassEx(&wc);

		// calculate window size based on desired client region size
		RECT wr;
		wr.left = 100;
		wr.right = props.Width + wr.left;
		wr.top = 100;
		wr.bottom = props.Height + wr.top;
		if (AdjustWindowRect(&wr, WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU, FALSE) == 0)
		{
			std::cout << "Failed to adjust rect" << std::endl;
			return;
		}
		// create window & get hWnd
		hWnd = CreateWindow(
			"Window class", props.Title.c_str(),
			WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU,
			CW_USEDEFAULT, CW_USEDEFAULT, wr.right - wr.left, wr.bottom - wr.top,
			nullptr, nullptr, hInst, this
		);
		// check for error
		if (hWnd == nullptr)
		{
			std::cout << "Failed to adjust rect" << std::endl;
			return;
		}
		// newly created windows start off as hidden
		ShowWindow(hWnd, SW_SHOWDEFAULT);

		m_DxCore = std::make_unique<Dx11>(hWnd, props.Width, props.Height);

		m_Instance = this;
	}
	Window::~Window()
	{
		DestroyWindow(hWnd);
	}
	void Window::OnUpdate()
	{
		m_DxCore->ClearBuffer(1.0f, 0.0f, 0.0f);
		m_Scene->OnUpdate();
		m_DxCore->EndFrame();
	}
	void Window::SetVSync(bool enabled)
	{
	}
	bool Window::IsVSync() const
	{
		return false;
	}
	void Window::Init(const WindowProps& props)
	{
	}
	void Window::Shutdown()
	{
	}

	std::optional<int> Window::ProcessMessages()
	{
		MSG msg;
		// while queue has messages, remove and dispatch them (but do not block on empty queue)
		while (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
		{
			// check for quit because peekmessage does not signal this via return val
			if (msg.message == WM_QUIT)
			{
				// return optional wrapping int (arg to PostQuitMessage is in wparam) signals quit
				return (int)msg.wParam;
			}

			// TranslateMessage will post auxilliary WM_CHAR messages from key msgs
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		// return empty optional when not quitting app
		return {};
	}

	LRESULT CALLBACK Window::HandleMsgSetup(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept
	{
		// use create parameter passed in from CreateWindow() to store window class pointer at WinAPI side
		if (msg == WM_NCCREATE)
		{
			// extract ptr to window class from creation data
			const CREATESTRUCTW* const pCreate = reinterpret_cast<CREATESTRUCTW*>(lParam);
			Window* const pWnd = static_cast<Window*>(pCreate->lpCreateParams);
			// set WinAPI-managed user data to store ptr to window instance
			SetWindowLongPtr(hWnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(pWnd));
			// set message proc to normal (non-setup) handler now that setup is finished
			SetWindowLongPtr(hWnd, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(&Window::HandleMsgThunk));
			// forward message to window instance handler
			return pWnd->HandleMsg(hWnd, msg, wParam, lParam);
		}
		// if we get a message before the WM_NCCREATE message, handle with default handler
		return DefWindowProc(hWnd, msg, wParam, lParam);
	}

	LRESULT CALLBACK Window::HandleMsgThunk(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept
	{
		// retrieve ptr to window instance
		Window* const pWnd = reinterpret_cast<Window*>(GetWindowLongPtr(hWnd, GWLP_USERDATA));
		// forward message to window instance handler
		return pWnd->HandleMsg(hWnd, msg, wParam, lParam);
	}

	LRESULT Window::HandleMsg(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept
	{
		switch (msg)
		{
			// we don't want the DefProc to handle this message because
	// we want our destructor to destroy the window, so return 0 instead of break
		case WM_CLOSE:
			PostQuitMessage(0);
			return 0;
		}

		return DefWindowProc(hWnd, msg, wParam, lParam);
	}
}