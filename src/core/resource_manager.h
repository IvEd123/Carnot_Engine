// ResourceManager.h
#pragma once

#include "resource.h"
#include "resource_handle.h"

#include <unordered_map>
#include <memory>
#include <string>
#include <typeindex>
#include <cassert>

namespace carnot {

    class ResourceManager {
    public:
        template<typename T, typename... Args>
        ResourceHandle<T> create(const std::string& id, Args&&... args);

        template<typename T>
        T* get(ResourceHandle<T> handle);

        template<typename T>
        void destroy(ResourceHandle<T> handle);

        void unloadAll();

    private:
        struct Entry {
            std::unique_ptr<Resource> resource;
            uint32_t refCount{ 0 };
        };

        uint32_t generateId();

    private:
        uint32_t m_nextId{ 1 };

        // handle_id -> resource
        std::unordered_map<uint32_t, Entry> m_resources;

        // logical id -> handle_id
        std::unordered_map<std::string, uint32_t> m_nameToId;
    };

}
