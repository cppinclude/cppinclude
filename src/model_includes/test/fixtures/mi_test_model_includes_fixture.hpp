#pragma once

#include "test_tools/boost_predicate_wrapper.hpp"

#include <stdfwd.hpp>

//------------------------------------------------------------------------------

namespace fs {
	class FileSystemAccessor;
	class FileSystem;
	class File;
}

//------------------------------------------------------------------------------

namespace model_includes {
	class ModelIncludesAccessor;
	class Model;
	class Analyzer;
	class Resolver;
	enum class FileType;
}

//------------------------------------------------------------------------------

namespace parser {
	class ParserAccessor;
	class Parser;
}

//------------------------------------------------------------------------------

namespace project {
	class ProjectAccessor;
	class Project;
}

//------------------------------------------------------------------------------

namespace model_includes::test {
	class ModelWrapper;

//------------------------------------------------------------------------------

class ModelIncludesFixture
{

public:

	using PathOpt	= std::optional< std::filesystem::path >;
	using FilePtr	= stdfwd::shared_ptr< fs::File >;

	ModelIncludesFixture();
	~ModelIncludesFixture();

	void setProjectDir( std::string_view _dir );

	void addFile(			std::string_view _path, std::string_view _text = "" );
	void addFileToProject(	std::string_view _file, std::string_view _text = "" );

	void addIncludePath(	std::string_view _file );
	void addIgnoredDir(		std::string_view _dir );
	void addFileFilter(		std::string_view _filter );
	void setIgnoreSystemIncludes( bool _ignoreSystemIncludes );

	PathOpt resolvePath(
		const std::filesystem::path & _startFile,
		std::string_view _fileName
	);

	FileType resolveFileType( stdfwd::string_view _file );

	BoostPredicate checkFileType(
		FileType _currentFileType,
		FileType _excpectedFile
	);

	ModelWrapper analyze();

	ModelWrapper createModel();

private:

	const std::filesystem::path & getProjectDir();

	ModelIncludesAccessor & ensureModelAccesddor();

	parser::ParserAccessor & ensureParserAccessor();
	parser::Parser & ensureParser();

	fs::FileSystemAccessor & ensureFileAccessor();
	fs::FileSystem & ensureFileSystem();

	Resolver & ensureResolver();
	Analyzer & ensureAnalyzer();

	project::ProjectAccessor & ensureProjectAccessor();
	project::Project & ensureProject();

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
};

//------------------------------------------------------------------------------

}
