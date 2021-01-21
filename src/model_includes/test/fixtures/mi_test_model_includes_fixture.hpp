#pragma once

#include "test_tools/boost_predicate_wrapper.hpp"

#include <stdfwd.hpp>

//------------------------------------------------------------------------------

namespace fs {
	class FileSystemAccessor;
	class FileSystem;
	class File;
}

namespace model_includes {
	class ModelIncludesAccessor;
	class Model;
	class Analyzer;
	class Resolver;
	enum class FileType;
}

namespace parser {
	class ParserAccessor;
	class Parser;
}

namespace project {
	class ProjectAccessor;
	class Project;
}

namespace cmake_project {
	class Project;
	class Accessor;
}

//------------------------------------------------------------------------------

namespace model_includes::test {
	class ModelWrapper;

//------------------------------------------------------------------------------

class ModelIncludesFixture
{

public:

	using Path		= stdfs::path;
	using PathOpt	= std::optional< Path >;
	using FilePtr	= stdfwd::shared_ptr< fs::File >;
	using Strings	= std::vector< std::string >;

	ModelIncludesFixture();
	~ModelIncludesFixture();

	void setProjectDir( std::string_view _dir );

	void addFile(			std::string_view _path, std::string_view _text = "" );
	void addFileToProject(	std::string_view _file, std::string_view _text = "" );
	void addFileToCmake(	std::string_view _file );

	void addIncludePath(	std::string_view _file );
	void addIgnoredDir(		std::string_view _dir );
	void addFileFilter(		std::string_view _filter );

	void setIgnoreSystemIncludes( bool _ignoreSystemIncludes );
	void addCppExtensions( const Strings & _extensions );
	void setAnalyzeWithoutExtension( bool _enable );

	void addIncludePathToCMake(	std::string_view _file, std::string_view _includeDir );

	PathOpt resolvePath(
		const Path & _startFile,
		std::string_view _fileName
	);

	PathOpt resolvePath(
		const Path & _startFile,
		std::string_view _fileName,
		const PathOpt & _currentCMakeSource
	);

	FileType resolveFileType( stdfwd::string_view _file );

	BoostPredicate checkFileType(
		FileType _currentFileType,
		FileType _excpectedFile
	);

	ModelWrapper analyze();
	ModelWrapper analyzeCmake();

	ModelWrapper createModel();

private:

	const Path & getProjectDir();

	ModelIncludesAccessor & ensureModelAccesddor();

	parser::ParserAccessor & ensureParserAccessor();
	parser::Parser & ensureParser();

	fs::FileSystemAccessor & ensureFileAccessor();
	fs::FileSystem & ensureFileSystem();

	Resolver & ensureResolver();
	Analyzer & ensureAnalyzer();

	project::ProjectAccessor & ensureProjectAccessor();
	project::Project & ensureProject();

	cmake_project::Accessor & ensureCmakeProjectAccessor();
	cmake_project::Project & ensureCmakeProject();

	std::string toString( FileType _fileType ) const;

private:

	std::unique_ptr< ModelIncludesAccessor > m_modelAccessor;

	std::unique_ptr< fs::FileSystemAccessor > m_fsAccessorPtr;

	std::unique_ptr< parser::ParserAccessor > m_parserAccessorPtr;
	std::unique_ptr< parser::Parser > m_parserPtr;

	std::unique_ptr< Resolver > m_resolverPtr;
	std::unique_ptr< Analyzer > m_analyzerPtr;

	std::unique_ptr< project::ProjectAccessor > m_projectAccessor;
	std::unique_ptr< project::Project > m_project;

	std::unique_ptr< cmake_project::Accessor > m_cmakeProjectAccessor;
	std::unique_ptr< cmake_project::Project > m_cmakeProject;
};

//------------------------------------------------------------------------------

}
