#pragma once

#include <stdfwd.hpp>

#include <memory>

//------------------------------------------------------------------------------

namespace compilation_db {
	class Database;
	class Loader;
	class Accessor;
}

//------------------------------------------------------------------------------

namespace cmake_project {
	class Project;
	class Loader;
	class Accessor;
}

//------------------------------------------------------------------------------

namespace cmake_project::test {

//------------------------------------------------------------------------------

class LoaderFixture
{
public:
	LoaderFixture();
	~LoaderFixture();

	using Strings = stdfwd::set< std::string >;

	void addCommand(
		std::string_view _directory,
		std::string_view _command,
		std::string_view _file
	);

	void addDefaultComandForFile( std::string_view _file );
	void addComandWithIncludes(
		std::string_view _file,
		const Strings & _includes
	);

	void load();

	Strings getResultsFiles() const;
	Strings getResultsIncludes( std::string_view _file ) const;

	Strings toExceptedFiles( const Strings & _files ) const;
	Strings toExceptedIncludes( const Strings & _dirs ) const;

private:
	using Path = stdfs::path;

	std::string getDefaultFolder() const;

	Loader & ensureLoader();
	Accessor & ensureCmakeProjectAccessor();

	compilation_db::Database & ensureCompilationDb();
	compilation_db::Loader & ensureCompilationDbLoader();
	compilation_db::Accessor & ensureCompilationDbAccessor();

private:
	std::unique_ptr< Project > m_projectPtr;
	std::unique_ptr< Loader > m_loaderPtr;
	std::unique_ptr< Accessor > m_cmakeProjectAccessorPtr;

	std::unique_ptr< compilation_db::Database > m_dbPtr;
	std::unique_ptr< compilation_db::Loader > m_dbLoaderPtr;
	std::unique_ptr< compilation_db::Accessor > m_dbAccessorPtr;
};

//------------------------------------------------------------------------------

}

//------------------------------------------------------------------------------
