#pragma once

#include "reporter/impl/tools/rp_includes_by_source_sorter.hpp"

#include <stdfwd.hpp>

#include <set>

//------------------------------------------------------------------------------

namespace model_includes {
	class Include;
	class File;
}

//------------------------------------------------------------------------------

namespace reporter {

//------------------------------------------------------------------------------

class SortedIncludesBySourceContainer
{
public:

	using Include			= model_includes::Include;
	using IncludeCallback	= std::function< bool ( const Include & ) >;

	using File				= model_includes::File;
	using FileCallback		= std::function< bool ( const File & ) >;

	void insert( const Include & _include );

	void forEachInclude( const IncludeCallback & _callback ) const;
	void forEachSource( const FileCallback & _callback ) const;

	std::size_t getCount() const;

private:
	using Includes = std::set< const Include *, IncludesBySourceSorter >;

	Includes m_includes;
};

//------------------------------------------------------------------------------

}
