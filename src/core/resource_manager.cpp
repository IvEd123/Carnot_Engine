// ResourceManager.cpp
#include "resource_manager.h"

using namespace carnot;

uint32_t ResourceManager::generateId() {
    return m_nextId++;
}

void ResourceManager::unloadAll() {
    for (auto& [_, entry] : m_resources) {
        entry.resource->free();
    }
    m_resources.clear();
    m_nameToId.clear();
}

template<typename T, typename... Args>
ResourceHandle<T> ResourceManager::create(const std::string& id, Args&&... args) {
    static_assert(std::is_base_of_v<Resource, T>);

    // ”же существует Ч увеличиваем refCount
    if (auto it = m_nameToId.find(id); it != m_nameToId.end()) {
        auto& entry = m_resources[it->second];
        ++entry.refCount;
        return ResourceHandle<T>(it->second);
    }

    uint32_t handleId = generateId();

    auto resource = std::make_unique<T>(id, std::forward<Args>(args)...);

    Entry entry;
    entry.resource = std::move(resource);
    entry.refCount = 1;

    m_resources.emplace(handleId, std::move(entry));
    m_nameToId.emplace(id, handleId);

    return ResourceHandle<T>(handleId);
}

template<typename T>
T* ResourceManager::get(ResourceHandle<T> handle) {
    static_assert(std::is_base_of_v<Resource, T>);

    auto it = m_resources.find(handle.id());
    if (it == m_resources.end())
        return nullptr;

    return static_cast<T*>(it->second.resource.get());
}

template<typename T>
void ResourceManager::destroy(ResourceHandle<T> handle) {
    static_assert(std::is_base_of_v<Resource, T>);

    auto it = m_resources.find(handle.id());
    if (it == m_resources.end())
        return;

    auto& entry = it->second;
    assert(entry.refCount > 0);
    --entry.refCount;

    if (entry.refCount == 0) {
        entry.resource->unload();
        m_nameToId.erase(entry.resource->getId());
        m_resources.erase(it);
    }
}
