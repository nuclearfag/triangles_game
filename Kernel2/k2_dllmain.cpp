#include "pch.h"

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <shellapi.h>
#ifdef min
#undef min
#endif
#ifdef max
#undef max
#endif
#include <wrl.h>
using namespace Microsoft::WRL;
#include <d3d12.h>
#include <dxgi1_6.h>
#include <d3dcompiler.h>
#include <DirectXMath.h>
#include "d3dx12.h"
#include <algorithm>
#include <cassert>
#include <chrono>

namespace Kernel2
{
	class Variables
	{
	public:
		Variables(const short numFrames = 3)
		{
			g_BackBuffers = new ComPtr<ID3D12Resource>[numFrames];
			g_CommandAllocator = new ComPtr<ID3D12CommandAllocator>[numFrames];
			g_FrameFenceValues = new uint64_t[numFrames];
		}
		~Variables()
		{
			memset(this, 0, sizeof(*this));
		}
		const uint8_t g_NumFrames = 3;
		bool g_UseWarp = false;
		bool g_IsInitialized = false;
		uint32_t g_ClientWidth = 1920U;
		uint32_t g_ClientHeight = 1080U;
		HWND g_Hwnd;
		RECT g_WindowRect;
		ComPtr<ID3D12Device2> g_Device;
		ComPtr<ID3D12CommandQueue> g_CommandQueue;
		ComPtr<IDXGISwapChain4> g_SwapChain;
		ComPtr<ID3D12Resource> *g_BackBuffers;
		ComPtr<ID3D12GraphicsCommandList> g_CommandList;
		ComPtr<ID3D12CommandAllocator> *g_CommandAllocator/*[g_NumFrames]*/;
		ComPtr<ID3D12DescriptorHeap> g_RTVDescriptorHeap;
		uint32_t g_RTVDescriptorSize;
		uint32_t g_CurrentBackBufferIndex;
		ComPtr<ID3D12Fence> g_Fence;
		uint64_t g_FenceValue = 0;
		uint64_t* g_FrameFenceValues/*[g_NumFrames] = {}*/;
		HANDLE g_FenceEvent;
	};

	_declspec(selectany) Variables* vars = new Variables();

	namespace Debugging
	{
		_declspec(dllexport) inline auto throwIfFailed(HRESULT hr)
		{
			if (FAILED(hr))
			{
				throw std::exception();
			}
		}

		_declspec(dllexport) void enableDebugLayer()
		{
			ComPtr<ID3D12Debug> debugInterface;
			throwIfFailed(D3D12GetDebugInterface(IID_PPV_ARGS(&debugInterface)));
			debugInterface->EnableDebugLayer();
		}

		_declspec(dllexport) void parseCommandLineArguments()
		{
			int argc;
			wchar_t** argv = CommandLineToArgvW(GetCommandLineW(), &argc);
			for (size_t i = 0; i < argc; ++i)
			{
				if (!wcscmp(argv[i], L"-w") || !wcscmp(argv[i], L"--width"))
				{
					vars->g_ClientWidth = wcstol(argv[++i], nullptr, 10);
				}
				if (!wcscmp(argv[i], L"-h") || !wcscmp(argv[i], L"--height"))
				{
					vars->g_ClientHeight = wcstol(argv[++i], nullptr, 10);
				}
				if (!wcscmp(argv[i], L"-warp") || !wcscmp(argv[i], L"--warp"))
				{
					vars->g_UseWarp = true;
				}
			}
			LocalFree(argv);
		}
	}

	

	namespace Graphics
	{
		_declspec(dllexport) ComPtr<IDXGIAdapter4> getAdapter(bool WARP)
		{
			ComPtr<IDXGIFactory4> dxgiFactory;
			uint32_t createFactoryFlags = 0;
			//createFactoryFlags = DXGI_CREATE_FACTORY_DEBUG;
			Debugging::throwIfFailed(CreateDXGIFactory2(createFactoryFlags, IID_PPV_ARGS(&dxgiFactory)));
			ComPtr<IDXGIAdapter1> dxgiAdapter1;
			ComPtr<IDXGIAdapter4> dxgiAdapter4;

			if (WARP)
			{
				Debugging::throwIfFailed(dxgiFactory->EnumWarpAdapter(IID_PPV_ARGS(&dxgiAdapter1)));
				Debugging::throwIfFailed(dxgiAdapter1.As(&dxgiAdapter4));
			}
			else
			{
				SIZE_T maxDedicatedVideoMemory = 0;
				for (UINT i = 0; dxgiFactory->EnumAdapters1(i, &dxgiAdapter1) != DXGI_ERROR_NOT_FOUND; ++i)
				{
					DXGI_ADAPTER_DESC1 dxgiAdapterDesc1;
					dxgiAdapter1->GetDesc1(&dxgiAdapterDesc1);

					// Check to see if the adapter can create a D3D12 device without actually 
					// creating it. The adapter with the largest dedicated video memory
					// is favored.
					if ((dxgiAdapterDesc1.Flags & DXGI_ADAPTER_FLAG_SOFTWARE) == 0 &&
						SUCCEEDED(D3D12CreateDevice(dxgiAdapter1.Get(),
							D3D_FEATURE_LEVEL_11_0, __uuidof(ID3D12Device), nullptr)) &&
						dxgiAdapterDesc1.DedicatedVideoMemory > maxDedicatedVideoMemory)
					{
						maxDedicatedVideoMemory = dxgiAdapterDesc1.DedicatedVideoMemory;
						Debugging::throwIfFailed(dxgiAdapter1.As(&dxgiAdapter4));
					}
				}
			}

			return dxgiAdapter4;
		}

		_declspec(dllexport) ComPtr<ID3D12Device2> createDevice(ComPtr<IDXGIAdapter4> adapter)
		{
			ComPtr<ID3D12Device2> d3d12Device2;
			Debugging::throwIfFailed(D3D12CreateDevice(adapter.Get(), D3D_FEATURE_LEVEL_11_0, IID_PPV_ARGS(&d3d12Device2)));
			ComPtr<ID3D12InfoQueue> pInfoQueue;
			if (SUCCEEDED(d3d12Device2.As(&pInfoQueue)))
			{
				pInfoQueue->SetBreakOnSeverity(D3D12_MESSAGE_SEVERITY_CORRUPTION, TRUE);
				pInfoQueue->SetBreakOnSeverity(D3D12_MESSAGE_SEVERITY_ERROR, TRUE);
				pInfoQueue->SetBreakOnSeverity(D3D12_MESSAGE_SEVERITY_WARNING, TRUE);

				// Suppress whole categories of messages
				//D3D12_MESSAGE_CATEGORY Categories[] = {};

				// Suppress messages based on their severity level
				D3D12_MESSAGE_SEVERITY Severities[] =
				{
					D3D12_MESSAGE_SEVERITY_INFO
				};

				// Suppress individual messages by their ID
				D3D12_MESSAGE_ID DenyIds[] = {
					D3D12_MESSAGE_ID_CLEARRENDERTARGETVIEW_MISMATCHINGCLEARVALUE,   // I'm really not sure how to avoid this message.
					D3D12_MESSAGE_ID_MAP_INVALID_NULLRANGE,                         // This warning occurs when using capture frame while graphics debugging.
					D3D12_MESSAGE_ID_UNMAP_INVALID_NULLRANGE,                       // This warning occurs when using capture frame while graphics debugging.
				};

				D3D12_INFO_QUEUE_FILTER NewFilter = {};
				//NewFilter.DenyList.NumCategories = _countof(Categories);
				//NewFilter.DenyList.pCategoryList = Categories;
				NewFilter.DenyList.NumSeverities = _countof(Severities);
				NewFilter.DenyList.pSeverityList = Severities;
				NewFilter.DenyList.NumIDs = _countof(DenyIds);
				NewFilter.DenyList.pIDList = DenyIds;

				Debugging::throwIfFailed(pInfoQueue->PushStorageFilter(&NewFilter));
			}
			return d3d12Device2;
		}

		_declspec(dllexport) ComPtr<ID3D12CommandQueue> createCommandQueue(ComPtr<ID3D12Device2> device, D3D12_COMMAND_LIST_TYPE type)
		{
			ComPtr<ID3D12CommandQueue> d3d12CommandQueue;

			D3D12_COMMAND_QUEUE_DESC desc = {};
			desc.Type = type;
			desc.Priority = D3D12_COMMAND_QUEUE_PRIORITY_NORMAL;
			desc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
			desc.NodeMask = 0;

			Debugging::throwIfFailed(device->CreateCommandQueue(&desc, IID_PPV_ARGS(&d3d12CommandQueue)));

			return d3d12CommandQueue;
		}

		_declspec(dllexport) bool checkTearingSupport()
		{
			BOOL allowTearing = FALSE;

			// Rather than create the DXGI 1.5 factory interface directly, we create the
			// DXGI 1.4 interface and query for the 1.5 interface. This is to enable the 
			// graphics debugging tools which will not support the 1.5 factory interface 
			// until a future update.
			ComPtr<IDXGIFactory4> factory4;
			if (SUCCEEDED(CreateDXGIFactory1(IID_PPV_ARGS(&factory4))))
			{
				ComPtr<IDXGIFactory5> factory5;
				if (SUCCEEDED(factory4.As(&factory5)))
				{
					if (FAILED(factory5->CheckFeatureSupport(
						DXGI_FEATURE_PRESENT_ALLOW_TEARING,
						&allowTearing, sizeof(allowTearing))))
					{
						allowTearing = FALSE;
					}
				}
			}

			return allowTearing = TRUE;
		}

		_declspec(dllexport) ComPtr<IDXGISwapChain4> CreateSwapChain(HWND hWnd,
			ComPtr<ID3D12CommandQueue> commandQueue,
			uint32_t width, uint32_t height, uint32_t bufferCount)
		{
			ComPtr<IDXGISwapChain4> dxgiSwapChain4;
			ComPtr<IDXGIFactory4> dxgiFactory4;
			UINT createFactoryFlags = 0;
			createFactoryFlags = DXGI_CREATE_FACTORY_DEBUG;

			Debugging::throwIfFailed(CreateDXGIFactory2(createFactoryFlags, IID_PPV_ARGS(&dxgiFactory4)));

			DXGI_SWAP_CHAIN_DESC1 swapChainDesc = {};
			swapChainDesc.Width = width;
			swapChainDesc.Height = height;
			swapChainDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
			swapChainDesc.Stereo = FALSE;
			swapChainDesc.SampleDesc = { 1, 0 };
			swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
			swapChainDesc.BufferCount = bufferCount;
			swapChainDesc.Scaling = DXGI_SCALING_STRETCH;
			swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
			swapChainDesc.AlphaMode = DXGI_ALPHA_MODE_UNSPECIFIED;
			// It is recommended to always allow tearing if tearing support is available.
			swapChainDesc.Flags = checkTearingSupport() ? DXGI_SWAP_CHAIN_FLAG_ALLOW_TEARING : 0;

			ComPtr<IDXGISwapChain1> swapChain1;
			Debugging::throwIfFailed(dxgiFactory4->CreateSwapChainForHwnd(
				commandQueue.Get(),
				hWnd,
				&swapChainDesc,
				nullptr,
				nullptr,
				&swapChain1));

			// Disable the Alt+Enter fullscreen toggle feature. Switching to fullscreen
			// will be handled manually.
			Debugging::throwIfFailed(dxgiFactory4->MakeWindowAssociation(hWnd, DXGI_MWA_NO_ALT_ENTER));

			Debugging::throwIfFailed(swapChain1.As(&dxgiSwapChain4));

			return dxgiSwapChain4;
		}

		_declspec(dllexport) ComPtr<ID3D12DescriptorHeap> CreateDescriptorHeap(ComPtr<ID3D12Device2> device,
			D3D12_DESCRIPTOR_HEAP_TYPE type, uint32_t numDescriptors)
		{
			ComPtr<ID3D12DescriptorHeap> descriptorHeap;

			D3D12_DESCRIPTOR_HEAP_DESC desc = {};
			desc.NumDescriptors = numDescriptors;
			desc.Type = type;

			Debugging::throwIfFailed(device->CreateDescriptorHeap(&desc, IID_PPV_ARGS(&descriptorHeap)));

			return descriptorHeap;
		}

		_declspec(dllexport) void UpdateRenderTargetViews(ComPtr<ID3D12Device2> device,
			ComPtr<IDXGISwapChain4> swapChain, ComPtr<ID3D12DescriptorHeap> descriptorHeap)
		{
			auto rtvDescriptorSize = device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);

			CD3DX12_CPU_DESCRIPTOR_HANDLE rtvHandle(descriptorHeap->GetCPUDescriptorHandleForHeapStart());

			for (int i = 0; i < vars->g_NumFrames; ++i)
			{
				ComPtr<ID3D12Resource> backBuffer;
				Debugging::throwIfFailed(swapChain->GetBuffer(i, IID_PPV_ARGS(&backBuffer)));

				device->CreateRenderTargetView(backBuffer.Get(), nullptr, rtvHandle);

				vars->g_BackBuffers[i] = backBuffer;

				rtvHandle.Offset(rtvDescriptorSize);
			}
		}

		_declspec(dllexport) ComPtr<ID3D12CommandAllocator> CreateCommandAllocator(ComPtr<ID3D12Device2> device,
			D3D12_COMMAND_LIST_TYPE type)
		{
			ComPtr<ID3D12CommandAllocator> commandAllocator;
			Debugging::throwIfFailed(device->CreateCommandAllocator(type, IID_PPV_ARGS(&commandAllocator)));

			return commandAllocator;
		}

		_declspec(dllexport) ComPtr<ID3D12GraphicsCommandList> CreateCommandList(ComPtr<ID3D12Device2> device,
			ComPtr<ID3D12CommandAllocator> commandAllocator, D3D12_COMMAND_LIST_TYPE type)
		{
			ComPtr<ID3D12GraphicsCommandList> commandList;
			Debugging::throwIfFailed(device->CreateCommandList(0, type, commandAllocator.Get(), nullptr, IID_PPV_ARGS(&commandList)));

			Debugging::throwIfFailed(commandList->Close());

			return commandList;
		}

		_declspec(dllexport) ComPtr<ID3D12Fence> CreateFence(ComPtr<ID3D12Device2> device)
		{
			ComPtr<ID3D12Fence> fence;

			Debugging::throwIfFailed(device->CreateFence(0, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&fence)));

			return fence;
		}

		_declspec(dllexport) HANDLE CreateEventHandle()
		{
			HANDLE fenceEvent;

			fenceEvent = ::CreateEvent(NULL, FALSE, FALSE, NULL);
			assert(fenceEvent && "Failed to create fence event.");

			return fenceEvent;
		}

		_declspec(dllexport) uint64_t Signal(ComPtr<ID3D12CommandQueue> commandQueue, ComPtr<ID3D12Fence> fence,
			uint64_t& fenceValue)
		{
			uint64_t fenceValueForSignal = ++fenceValue;
			Debugging::throwIfFailed(commandQueue->Signal(fence.Get(), fenceValueForSignal));

			return fenceValueForSignal;
		}

		_declspec(dllexport) void WaitForFenceValue(ComPtr<ID3D12Fence> fence, uint64_t fenceValue, HANDLE fenceEvent,
			std::chrono::milliseconds duration = std::chrono::milliseconds::max())
		{
			if (fence->GetCompletedValue() < fenceValue)
			{
				Debugging::throwIfFailed(fence->SetEventOnCompletion(fenceValue, fenceEvent));
				::WaitForSingleObject(fenceEvent, static_cast<DWORD>(duration.count()));
			}
		}

		_declspec(dllexport) void Flush(ComPtr<ID3D12CommandQueue> commandQueue, ComPtr<ID3D12Fence> fence,
			uint64_t& fenceValue, HANDLE fenceEvent)
		{
			uint64_t fenceValueForSignal = Signal(commandQueue, fence, fenceValue);
			WaitForFenceValue(fence, fenceValueForSignal, fenceEvent);
		}
	}

	namespace Math
	{

	}

	namespace Internet
	{

	}

	namespace Sound
	{

	}

	namespace Memory
	{

	}

	namespace Logic
	{

	}

	namespace Physics
	{

	}
}

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
                     )
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}

