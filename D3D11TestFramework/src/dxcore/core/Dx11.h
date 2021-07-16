#pragma once
#include "../DxCore.h"


namespace DXCore{
	class Dx11 {

	public:
		Dx11(HWND hwnd, int width, int height);
		Dx11(const Dx11&) = delete;
		void operator=(const Dx11&) = delete;
		~Dx11() = default;

		void ClearBuffer(float red, float green, float blue);
		void EndFrame();

		const wrl::ComPtr<ID3D11Device>& GetDevice() const { return pDevice; }
		const wrl::ComPtr<ID3D11DeviceContext>& GetContext() const { return pContext; }
	private:

		Microsoft::WRL::ComPtr<ID3D11Device> pDevice;
		Microsoft::WRL::ComPtr<IDXGISwapChain> pSwap;
		Microsoft::WRL::ComPtr<ID3D11DeviceContext> pContext;
		Microsoft::WRL::ComPtr<ID3D11RenderTargetView> pTarget;
		Microsoft::WRL::ComPtr<ID3D11DepthStencilView> pDSV;
};
}