#pragma once

#include "test_tools/boost_predicate_wrapper.hpp"

#include <stdfwd.hpp>

//------------------------------------------------------------------------------

namespace model_includes {
	class File;
	class Include;
}

//------------------------------------------------------------------------------

namespace model_includes::test {
	class IncludeWrapper;

//------------------------------------------------------------------------------

class FileWrapper
{
public:

	FileWrapper( const File * _file  );

	bool isAvailable() const;

	BoostPredicate checkIncludesCount( int _exceptCount ) const;
	BoostPredicate checkIncludesCountRecursive( int _exceptCount ) const;
	IncludeWrapper getInclude( int _index ) const;

	BoostPredicate checkIncludedByCount( int _exceptCount ) const;
	BoostPredicate checkIncludedByCountRecursive( int _exceptCount ) const;
	IncludeWrapper getIncludedBy( std::string_view _sourcePath ) const;

private:

	using Path	= stdfs::path;

	const File & getFile() const;

	std::string dumpIncludes() const;
	std::string dumpIncludedBy() const;
	std::string dump( const Include & _include ) const;

private:

	const File * m_file;
};

//------------------------------------------------------------------------------

}
