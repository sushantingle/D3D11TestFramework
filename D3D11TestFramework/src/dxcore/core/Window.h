#pragma once
#include "../DxCore.h"
#include "../events/Event.h"
#include "Dx11.h"
#include "Scene.h"

namespace DXCore {
	struct WindowProps
	{
		std::string Title;
		uint32_t Width;
		uint32_t Height;

		WindowProps(const std::string& title = "DirectX Sandbox",
			uint32_t width = 1280,
			uint32_t height = 720)
			: Title(title), Width(width), Height(height)
		{
		}
	};

	class Window
	{
	public:
		using EventCallbackFn = std::function<void(Event&)>;

		Window(const WindowProps& props);
		virtual ~Window();

		void OnUpdate();

		inline uint32_t GetWidth() const { return m_Data.Width; }
		inline uint32_t GetHeight() const { return m_Data.Height; }

		// Window attributes
		inline void SetEventCallback(const EventCallbackFn& callback) { m_Data.EventCallback = callback; }
		void SetVSync(bool enabled);
		bool IsVSync() const;

		std::optional<int> ProcessMessages();

		static LRESULT CALLBACK HandleMsgSetup(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept;

		static LRESULT CALLBACK HandleMsgThunk(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept;

		LRESULT HandleMsg(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept;

		//inline virtual void* GetNativeWindow() const { return m_Window; }
		const wrl::ComPtr<ID3D11Device>& GetDxDevice() const { return m_DxCore->GetDevice(); }
		const wrl::ComPtr<ID3D11DeviceContext>& GetDxContext() const { return m_DxCore->GetContext(); }

		void RegisterScene(std::unique_ptr<Scene> scene) { m_Scene = std::move(scene); }

		static Window* GetInstance() { return m_Instance; }

	private:
		virtual void Init(const WindowProps& props);
		virtual void Shutdown();
	private:
		
		HINSTANCE hInst;
		HWND hWnd;

		struct WindowData
		{
			std::string Title;
			uint32_t Width, Height;
			bool VSync;

			EventCallbackFn EventCallback;
		};

		WindowData m_Data;
		std::unique_ptr<Dx11> m_DxCore;
		std::unique_ptr<Scene> m_Scene;

		static Window* m_Instance;
	};


}