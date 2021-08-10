#include "model_includes/test/fixtures/mi_test_model_includes_fixture.hpp"

#include "model_includes/test/fixtures/wrappers/mi_test_model_wrapper.hpp"

#include "model_includes/api/enums/mi_file_type.hpp"
#include "model_includes/api/mi_model.hpp"
#include "model_includes/ih/mi_accessor_impl.hpp"
#include "model_includes/impl/mi_analyzer_impl.hpp"
#include "model_includes/impl/mi_resolver.hpp"

#include "parser/api/pr_parser.hpp"
#include "parser/ih/pr_accessor_impl.hpp"

#include "fs/api/fs_factory.hpp"
#include "fs/api/fs_file.hpp"
#include "fs/api/fs_file_system.hpp"
#include "fs/ih/fs_accessor_impl.hpp"

#include "project/api/prj_project.hpp"
#include "project/ih/prj_project_accesso_impl.hpp"

#include "cmake_project/api/cprj_loader.hpp"
#include "cmake_project/api/cprj_project.hpp"
#include "cmake_project/ih/cprj_accessor_impl.hpp"

#include "exception/ih/exc_internal_error.hpp"

#include <optional>
#include <std_fs>
#include <string_view>

//------------------------------------------------------------------------------

namespace model_includes::test
{
//------------------------------------------------------------------------------

ModelIncludesFixture::ModelIncludesFixture() = default;
ModelIncludesFixture::~ModelIncludesFixture() = default;

//------------------------------------------------------------------------------

void ModelIncludesFixture::setProjectDir( std::string_view _dir )
{
	ensureProject().setProjectDir( _dir );
}

//------------------------------------------------------------------------------

void ModelIncludesFixture::addFileToProject(
	std::string_view _file, std::string_view _text )
{
	Path filePath = getProjectDir() / _file;
	addFile( filePath.string(), _text );
}

//------------------------------------------------------------------------------

void ModelIncludesFixture::addFileToCmake( std::string_view _file )
{
	ensureCmakeProject().addFilePath( _file );
}

//------------------------------------------------------------------------------

void ModelIncludesFixture::addFile(
	std::string_view _path, std::string_view _text )
{
	Path filePath{ _path };
	filePath = stdfs::lexically_normal( filePath );
	auto filePtr = ensureFileSystem().createFile( filePath );
	INTERNAL_CHECK_ERROR( filePtr );
	( *filePtr ) << _text;
}

//------------------------------------------------------------------------------

void ModelIncludesFixture::addIncludePath( std::string_view _path )
{
	ensureProject().addIncludeDir( _path );
}

//------------------------------------------------------------------------------

void ModelIncludesFixture::addIgnoredDir( std::string_view _dir )
{
	ensureProject().addIgnoredDir( _dir );
}

//------------------------------------------------------------------------------

void ModelIncludesFixture::addFileFilter( std::string_view _filter )
{
	ensureProject().addFileFilter( _filter );
}

//------------------------------------------------------------------------------

void ModelIncludesFixture::setIgnoreSystemIncludes( bool _ignoreSystemIncludes )
{
	ensureProject().setIgnoreSystemIncludes( _ignoreSystemIncludes );
}

//------------------------------------------------------------------------------

void ModelIncludesFixture::addCppExtensions( const Strings & _extensions )
{
	ensureProject().addCppFileExtensions( _extensions );
}

//------------------------------------------------------------------------------

void ModelIncludesFixture::setAnalyzeWithoutExtension( bool _enable )
{
	ensureProject().setAnalyzeWithoutExtension( _enable );
}

//------------------------------------------------------------------------------

void ModelIncludesFixture::addIncludePathToCMake(
	std::string_view _file, std::string_view _includeDir )
{
	ensureCmakeProject().addIncludeToFile( _file, _includeDir );
}

//------------------------------------------------------------------------------

ModelIncludesFixture::PathOpt ModelIncludesFixture::resolvePath(
	const Path & _startFile, std::string_view _fileName )
{
	return resolvePath( _startFile, _fileName, std::nullopt );
}

//------------------------------------------------------------------------------

ModelIncludesFixture::PathOpt ModelIncludesFixture::resolvePath(
	const Path & _startFile,
	stdfwd::string_view _fileName,
	const PathOpt & _currentCMakeFile )
{
	return ensureResolver().resolvePath(
		ensureProject(), m_cmakeProject.get(), _startFile, _fileName,
		_currentCMakeFile );
}

//------------------------------------------------------------------------------

FileType ModelIncludesFixture::resolveFileType( stdfwd::string_view _file )
{
	return Resolver::resolveFileType( _file );
}

//------------------------------------------------------------------------------

BoostPredicate ModelIncludesFixture::checkFileType(
	FileType _currentFileType, FileType _excpectedFile )
{
	if( _currentFileType == _excpectedFile )
	{
		return true;
	}

	std::stringstream stream;
	stream << "\n"
		   << "Current file type: " << toString( _currentFileType )
		   << "but expected : " << toString( _excpectedFile );
	return stream.str();
}

//------------------------------------------------------------------------------

ModelWrapper ModelIncludesFixture::analyze()
{
	project::Project & project = ensureProject();
	project.changeAllPathsToAbsolute();

	return ensureAnalyzer().analyze( project );
}

//------------------------------------------------------------------------------

ModelWrapper ModelIncludesFixture::analyzeCmake()
{
	project::Project & project = ensureProject();
	project.changeAllPathsToAbsolute();

	return ensureAnalyzer().analyze( project, ensureCmakeProject() );
}

//------------------------------------------------------------------------------

ModelWrapper ModelIncludesFixture::createModel()
{
	return ensureModelAccesddor().createEmptyModel();
}

//------------------------------------------------------------------------------

ModelIncludesAccessor & ModelIncludesFixture::ensureModelAccesddor()
{
	if( !m_modelAccessor )
	{
		m_modelAccessor = std::make_unique< ModelIncludesAccessorImpl >();
	}

	return *m_modelAccessor;
}

//------------------------------------------------------------------------------

parser::ParserAccessor & ModelIncludesFixture::ensureParserAccessor()
{
	if( !m_parserAccessorPtr )
	{
		m_parserAccessorPtr = std::make_unique< parser::ParserAccessorImpl >();
	}

	return *m_parserAccessorPtr;
}

//------------------------------------------------------------------------------

parser::Parser & ModelIncludesFixture::ensureParser()
{
	if( !m_parserPtr )
	{
		auto newPerser = ensureParserAccessor().createParser();
		m_parserPtr.swap( newPerser );
	}

	return *m_parserPtr;
}

//------------------------------------------------------------------------------

fs::FileSystemAccessor & ModelIncludesFixture::ensureFileAccessor()
{
	if( !m_fsAccessorPtr )
	{
		m_fsAccessorPtr = std::make_unique< fs::FileSystemAccessorImpl >();
	}

	return *m_fsAccessorPtr;
}

//------------------------------------------------------------------------------

fs::FileSystem & ModelIncludesFixture::ensureFileSystem()
{
	return ensureFileAccessor().getFactory().getMemoryFileSystem();
}

//------------------------------------------------------------------------------

Resolver & ModelIncludesFixture::ensureResolver()
{
	if( !m_resolverPtr )
	{
		m_resolverPtr = std::make_unique< Resolver >( ensureFileSystem() );
	}

	return *m_resolverPtr;
}

//------------------------------------------------------------------------------

Analyzer & ModelIncludesFixture::ensureAnalyzer()
{
	if( !m_analyzerPtr )
	{
		auto newAnalyzer = ensureModelAccesddor().createAnalyzer(
			ensureFileSystem(), ensureParser() );
		m_analyzerPtr.swap( newAnalyzer );
	}

	return *m_analyzerPtr;
}

//------------------------------------------------------------------------------

const ModelIncludesFixture::Path & ModelIncludesFixture::getProjectDir()
{
	return ensureProject().getProjectDir();
}

//------------------------------------------------------------------------------

project::ProjectAccessor & ModelIncludesFixture::ensureProjectAccessor()
{
	if( !m_projectAccessor )
	{
		m_projectAccessor = std::make_unique< project::ProjectAccessorImpl >();
	}

	return *m_projectAccessor;
}

//------------------------------------------------------------------------------

project::Project & ModelIncludesFixture::ensureProject()
{
	if( !m_project )
	{
		m_project = ensureProjectAccessor().createProject();
	}

	return *m_project;
}

//------------------------------------------------------------------------------

cmake_project::Accessor & ModelIncludesFixture::ensureCmakeProjectAccessor()
{
	if( !m_cmakeProjectAccessor )
	{
		m_cmakeProjectAccessor =
			std::make_unique< cmake_project::AccessorImpl >();
	}

	return *m_cmakeProjectAccessor;
}

//------------------------------------------------------------------------------

cmake_project::Project & ModelIncludesFixture::ensureCmakeProject()
{
	if( !m_cmakeProject )
	{
		auto loaderPtr = ensureCmakeProjectAccessor().createLoader();
		INTERNAL_CHECK_ERROR( loaderPtr );
		m_cmakeProject = loaderPtr->createEmptyProject();
	}

	return *m_cmakeProject;
}

//------------------------------------------------------------------------------

std::string ModelIncludesFixture::toString( FileType _fileType ) const
{
	static_assert( static_cast< int >( FileType::Count ) == 2 );
	switch( _fileType )
	{
		case FileType::ProjectFile:
			return "ProjectFile";
		case FileType::StdLibraryFile:
			return "StdLibraryFile";
		default:
			INTERNAL_CHECK_WARRING( false );
			return "";
	};
}

//------------------------------------------------------------------------------

}
