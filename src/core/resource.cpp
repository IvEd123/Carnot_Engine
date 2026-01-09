#include "resource.h"

using namespace carnot;

bool Resource::load(std::filesystem::path path)
{
	const bool res = load(path);
	m_state = res ? State::Ready : State::Failed;
	return res;
}

bool Resource::free()
{
	const bool res = free();
	m_state = res ? State::Empty : State::Failed;
	return res;
}

std::string carnot::Resource::id() const
{
	return m_id;
}
