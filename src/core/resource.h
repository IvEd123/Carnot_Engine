#pragma once

#include <string>
#include <filesystem>

namespace carnot {

	class Resource {
	public:
		enum class State : uint8_t {
			Empty = 0,
			Ready,
			Loading,
			Failed
		};

		virtual ~Resource() = default;

		virtual bool load(std::filesystem::path) final;
		virtual bool free() final;

		std::string id() const;
	protected:
		explicit Resource(const std::string& id) :
			m_id(id) 
		{}

		virtual bool load_impl(std::filesystem::path) = 0;
		virtual bool free_impl() = 0;


	private:
		State m_state = State::Empty;
		std::string m_id;
	};
};
