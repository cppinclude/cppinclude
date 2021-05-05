#pragma once

#include "test_tools/boost_predicate_wrapper.hpp"

#include <stdfwd/vector>
#include <std_fs>

#include <memory>

//------------------------------------------------------------------------------

namespace project {
	class Project;
	class ProjectAccessor;
}

//------------------------------------------------------------------------------

namespace project::test {

//------------------------------------------------------------------------------

class ProjectFixture
{
public:

	using Strings	= stdfwd::vector< std::string >;
	using Path		= stdfs::path;
	using Paths		= stdfwd::vector< Path >;

	ProjectFixture();
	~ProjectFixture();

	Project & getProject();
	const Project & getProject() const;

	void setProjectDir( const Path & _path );
	const Path & getProjectDir() const;

	void addIncludeDirs( const Strings & _dirs );
	Paths getIncludeDirs() const;

	void addIgnoreDirs( const Strings & _dirs );
	bool isIgnoreDir( std::string_view _path );
	Paths getIgnoreDirs() const;

	void addFileFilter( std::string_view _filter );
	bool isIgnoredFile( std::string_view _path ) const;

	void addCppExtensions( const Strings & _ext );
	bool isExistsCppExtension( std::string_view _ext ) const;

	void changeAllPathsToAbsolute();
	Path toAbsolutePath( const Path & _path );

	static Path getCurrentPath();

	static std::string toString( const Strings & _strings );
	static Paths toPaths( const Strings & _strings );

private:
	ProjectAccessor & getProjectAccessor();

private:
	std::unique_ptr< ProjectAccessor > m_accessor;
	std::unique_ptr< Project > m_project;
};

//------------------------------------------------------------------------------

}
