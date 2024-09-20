#pragma once

#include "Bindable.h"
#include "BindableCodex.h"


#include <memory>
#include <type_traits>
#include <unordered_map>

namespace Bind
{
    class Codex
    {
    public:
        template<class T, typename...Params>
        static std::shared_ptr<T> Resolve(Graphics& gfx, Params&&...p) noexcept
        {
            static_assert(std::is_base_of<Bindable, T>::value, "Can only resolve classes derived from Bindable");
            return Get().Resolve_<T>(gfx, std::forward<Params>(p)...);
        }

    private:
        template<class T, typename...Params>
        std::shared_ptr<T> Resolve_(Graphics& gfx, Params&&...p) noexcept
        {
            const auto key = T::GenerateUID(std::forward<Params>(p)...);
            const auto exits = binds.find(key);

            if (exits == binds.end())
            {
                auto bindable = std::make_shared<T>(gfx, std::forward<Params>(p)...);
                binds[key] = bindable;
                return bindable;
            }
            else
            {
                return std::static_pointer_cast<T>(exits->second);
            }
        }

        static Codex& Get()
        {
            static Codex codex;
            return codex;
        }
    private:
        std::unordered_map<std::string, std::shared_ptr<Bindable>> binds;
    };

}