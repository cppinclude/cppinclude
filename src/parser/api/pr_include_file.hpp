#pragma once

#include "parser/api/pr_include_file_location.hpp"

#include <string>

//------------------------------------------------------------------------------

namespace parser {

//------------------------------------------------------------------------------

class IncludeFile
{
public:
	IncludeFile(
			const IncludeFileLocation & _location,
			std::string_view _name,
			bool _isSystem
	)
		:	m_location{ _location }
		,	m_name{ _name }
		,	m_isSystem{ _isSystem }

	{
	}

	const IncludeFileLocation & getLocation() const
	{
		return m_location;
	}
	std::string_view getName() const
	{
		return m_name;
	}
	bool isSystem() const
	{
		return m_isSystem;
	}

private:
	const IncludeFileLocation m_location;
	const std::string m_name;
	const bool m_isSystem;
};

//------------------------------------------------------------------------------

}
