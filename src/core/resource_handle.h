#pragma once

#include <cstdint>

namespace carnot 
{

    template<typename T>
    class ResourceHandle {
    public:
        ResourceHandle() = default;
        explicit ResourceHandle(uint32_t id) : m_id(id) {}

        uint32_t id() const noexcept { return m_id; }
        bool valid() const noexcept { return m_id != 0; }

        bool operator==(const ResourceHandle& other) const noexcept {
            return m_id == other.m_id;
        }

    private:
        uint32_t m_id{ 0 }; // 0 is invalid
    };

}
