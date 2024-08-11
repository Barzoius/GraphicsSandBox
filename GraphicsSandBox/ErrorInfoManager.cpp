#include "ErrorInfoManager.h"
#include "Window.h"

#pragma comment(lib, "dxguid.lib")

#define GFX_THROW_NO_INFO(hrcall) if( FAILED( hr = (hrcall) ) ) throw Graphics::HrException( __LINE__,__FILE__,hr )


ErrorInfoManager::ErrorInfoManager()
{

    typedef HRESULT(WINAPI* DXGIGetDebugInterface)(REFIID, void**);

    ///LOAD THE DLL
    const auto LDD = LoadLibraryEx("dxgidebug.dll", nullptr, LOAD_LIBRARY_SEARCH_SYSTEM32);

    if (LDD == nullptr)
    {
        throw HWND_LAST_EXCEPT();
    }

    const auto DxgiGetDebugInterface = reinterpret_cast<DXGIGetDebugInterface>(
        reinterpret_cast<void*>(GetProcAddress(LDD, "DXGIGetDebugInterface")));

    if (DxgiGetDebugInterface == nullptr)
    {
        throw HWND_LAST_EXCEPT();
    }

    HRESULT hr;

    GFX_THROW_NO_INFO(DxgiGetDebugInterface(__uuidof(IDXGIInfoQueue), reinterpret_cast<void**>(&pDXGIInfoQueue)));
}


ErrorInfoManager::~ErrorInfoManager()
{
    if (pDXGIInfoQueue != nullptr)
    {
        pDXGIInfoQueue->Release();
    }
}

void ErrorInfoManager::Set() noexcept
{
    ///Sets the index of next to only retrive the messages after this call
    next = pDXGIInfoQueue->GetNumStoredMessages(DXGI_DEBUG_ALL);
}


std::vector<std::string> ErrorInfoManager::GetMessages() const
{
    std::vector<std::string> messages;
    const auto end = pDXGIInfoQueue->GetNumStoredMessages(DXGI_DEBUG_ALL);

    for (auto i = next; i < end; i++)
    {
        HRESULT hr;
        SIZE_T messLength;

        ///stores the message i's length in messLength (in bytes)
        GFX_THROW_NO_INFO(pDXGIInfoQueue->GetMessage(DXGI_DEBUG_ALL, i, nullptr, &messLength));

        ///allocate the needed memory
        auto bytes = std::make_unique<byte[]>(messLength);
        auto pMessage = reinterpret_cast<DXGI_INFO_QUEUE_MESSAGE*>(bytes.get());

        GFX_THROW_NO_INFO(pDXGIInfoQueue->GetMessage(DXGI_DEBUG_ALL, i, pMessage, &messLength));
        messages.emplace_back(pMessage->pDescription);

    }

    return messages;
}