#pragma once

#include <std_fs>
#include <stdfwd/functional>
#include <stdfwd/size_t>

//------------------------------------------------------------------------------

namespace cmake_project
{
//------------------------------------------------------------------------------

class Project
{
public:
	using CountType = std::size_t;
	using Path = stdfs::path;
	using PathCallback = std::function< bool( const Path & ) >;

	virtual ~Project() = default;

	virtual CountType getFilePathsCount() const = 0;
	virtual void forEachFilePath( PathCallback _callback ) const = 0;

	virtual void
	forEachIncludes( const Path & _file, PathCallback _callback ) const = 0;

	virtual void addFilePath( const Path & _file ) = 0;
	virtual void
	addIncludeToFile( const Path & _file, const Path & _include ) = 0;
};

//------------------------------------------------------------------------------

}
