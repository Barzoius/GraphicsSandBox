#pragma once

#include "ModWin.h"
#include <vector>
#include <string>
#include "wrl.h"



class ErrorInfoManager
{
public:
    ErrorInfoManager();
    ~ErrorInfoManager();
    ErrorInfoManager(const ErrorInfoManager&) = delete;
    ErrorInfoManager& operator = (const ErrorInfoManager&) = delete;

    void Set() noexcept;
    std::vector<std::string> GetMessages() const;

private:
    ///index of the last message
    unsigned long long next = 0u;
    /*Microsoft::WRL::ComPtr<IDXGIInfoQueue> pDXGIInfoQueue;*/
    struct IDXGIInfoQueue* pDXGIInfoQueue  = nullptr;

};