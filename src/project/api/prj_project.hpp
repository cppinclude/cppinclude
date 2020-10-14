#pragma once

#include <stdfwd.hpp>

//------------------------------------------------------------------------------

namespace project {

//------------------------------------------------------------------------------

class Project
{
public:

	using Strings					= stdfwd::vector< std::string >;
	using Path						= stdfs::path;
	using DirPaths					= stdfwd::vector< Path >;
	using IncludeDirIndex			= std::size_t;
	using FileFilterIndex			= std::size_t;

	using PathCallback				= std::function< bool ( const Path & ) >;
	using FileExtensionCallback		= std::function< bool ( std::string_view ) >;

	virtual ~Project() = default;

	virtual void setProjectDir( const Path & _path ) = 0;
    virtual const Path & getProjectDir() const = 0;

	virtual IncludeDirIndex getIncludeDirsCount() const = 0;
	virtual const Path & getIncludeDir( IncludeDirIndex _index ) const = 0;
	virtual void addIncludeDir( const Path & _path ) = 0;
	virtual void addIncludeDirs( const DirPaths & _paths ) = 0;

	virtual bool hasIgnoreDirs() const = 0;
	virtual bool isIgnoredDir( const Path & _path ) const = 0;
	virtual void addIgnoredDir( const Path & _path ) = 0;
	virtual void addIgnoredDirs( const DirPaths & _paths ) = 0;
	virtual void forEachIgnoreDir( PathCallback _callback ) const = 0;

	virtual bool hasCppFileExtensions() const = 0;
	virtual bool isExistsCppExtension( std::string_view _ext ) const = 0;

	virtual void addCppFileExtension( std::string_view _ext ) = 0;
	virtual void addCppFileExtensions( const Strings & _extensions ) = 0;
	virtual void forEachFileExtension( FileExtensionCallback _callback ) const = 0;

	virtual bool getAnalyzeWithoutExtension() const = 0;
	virtual void setAnalyzeWithoutExtension( bool _enable ) = 0;

	virtual bool getIgnoreSystemIncludes() const = 0;
	virtual void setIgnoreSystemIncludes( bool _ignore ) = 0;

	virtual FileFilterIndex getFileFilterCount() const = 0;
	virtual const std::regex & getFileFilter( FileFilterIndex _index ) const = 0;
	virtual void addFileFilter( std::string_view _filter ) = 0;
	virtual void addFileFilters( const Strings & _filters ) = 0;
	virtual bool hasFileFilters() const = 0;
	virtual bool isIgnoredFile( const Path & _path ) const = 0;

	virtual void changeAllPathsToAbsolute( const Path & _currentDir ) = 0;
	virtual void changeAllPathsToAbsolute() = 0;
};

//------------------------------------------------------------------------------

}
