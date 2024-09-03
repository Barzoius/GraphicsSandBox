#include "Bindable.h"

namespace Bind
{
	ID3D11DeviceContext* Bindable::GetContext(Graphics& gfx) noexcept
	{
		return gfx.pContext.Get();
	}

	ID3D11Device* Bindable::GetDevice(Graphics& gfx) noexcept
	{
		return gfx.pDevice.Get();
	}

	ErrorInfoManager& Bindable::GetInfoManager(Graphics& gfx) noexcept
	{

#ifndef NDEBUG
		return gfx.infoManager;
#else
		throw std::logic_error("CANT ACCESS infoManager IN RELEASE MODE");
#endif
	}

}