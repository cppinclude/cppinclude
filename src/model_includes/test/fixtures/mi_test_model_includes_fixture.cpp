#include "model_includes/test/fixtures/mi_test_model_includes_fixture.hpp"

#include "model_includes/test/fixtures/wrappers/mi_test_model_wrapper.hpp"

#include "model_includes/api/mi_model.hpp"
#include "model_includes/api/enums/mi_file_type.hpp"
#include "model_includes/impl/mi_resolver.hpp"
#include "model_includes/impl/mi_analyzer_impl.hpp"
#include "model_includes/ih/mi_accessor_impl.hpp"

#include "parser/api/pr_parser.hpp"
#include "parser/ih/pr_accessor_impl.hpp"

#include "fs/api/fs_file_system.hpp"
#include "fs/api/fs_factory.hpp"
#include "fs/api/fs_file.hpp"
#include "fs/ih/fs_accessor_impl.hpp"

#include "project/api/prj_project.hpp"
#include "project/ih/prj_project_accesso_impl.hpp"

#include "exception/ih/exc_internal_error.hpp"

#include <string_view>
#include <optional>
#include <std_fs>

//------------------------------------------------------------------------------

namespace model_includes::test {

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
	std::string_view _file,
	std::string_view _text
)
{
	Path filePath = getProjectDir() / _file;
	addFile( filePath.string(), _text );
}

//------------------------------------------------------------------------------

void ModelIncludesFixture::addFile(
	std::string_view _path,
	std::string_view _text
)
{
	Path filePath{ _path };
	filePath = stdfs::lexically_normal( filePath );
	auto filePtr = ensureFileSystem().createFile( filePath );
	INTERNAL_CHECK_ERROR( filePtr );
	(*filePtr) << _text;
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

ModelIncludesFixture::PathOpt ModelIncludesFixture::resolvePath(
	const Path & _startFile,
	stdfwd::string_view _fileName
)
{
	return ensureResolver().resolvePath( _startFile, _fileName );
}

//------------------------------------------------------------------------------

FileType ModelIncludesFixture::resolveFileType( stdfwd::string_view _file )
{
	return ensureResolver().resolveFileType( _file );
}

//------------------------------------------------------------------------------

BoostPredicate ModelIncludesFixture::checkFileType(
	FileType _currentFileType,
	FileType _excpectedFile
)
{
	if( _currentFileType == _excpectedFile )
		return true;

	std::stringstream stream;
	stream
		<< "\n"
		<< "Current file type: "
		<< toString( _currentFileType )
		<<  "but expected : "
		<< toString( _excpectedFile )
	;
	return stream.str();
}

//------------------------------------------------------------------------------

ModelWrapper ModelIncludesFixture::analyze()
{
	ensureProject().changeAllPathsToAbsolute();
	return ensureAnalyzer().analyze( ensureProject() );
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
		m_modelAccessor.reset( new ModelIncludesAccessorImpl );

	return *m_modelAccessor;
}

//------------------------------------------------------------------------------

parser::ParserAccessor & ModelIncludesFixture::ensureParserAccessor()
{
	if( !m_parserAccessorPtr )
		m_parserAccessorPtr.reset( new parser::ParserAccessorImpl );

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
		m_fsAccessorPtr.reset( new fs::FileSystemAccessorImpl );

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
		m_resolverPtr.reset(
			new Resolver{ ensureFileSystem(), ensureProject() }
	);

	return *m_resolverPtr;
}

//------------------------------------------------------------------------------

Analyzer & ModelIncludesFixture::ensureAnalyzer()
{
	if( !m_analyzerPtr )
	{
		auto newAnalyzer = ensureModelAccesddor().createAnalyzer(
			ensureFileSystem(),
			ensureParser()
		);
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
		m_projectAccessor.reset( new project::ProjectAccessorImpl );

	return *m_projectAccessor;
}

//------------------------------------------------------------------------------

project::Project & ModelIncludesFixture::ensureProject()
{
	if( !m_project )
	{
		auto newProject{ ensureProjectAccessor().createProject() };
		m_project.swap( newProject );
	}
	return *m_project;
}

//------------------------------------------------------------------------------

std::string ModelIncludesFixture::toString( FileType _fileType ) const
{
	static_assert( static_cast< int >( FileType::Count ) == 2 );
	switch( _fileType )
	{
		case FileType::ProjectFile		: return "ProjectFile";
		case FileType::StdLibraryFile	: return "StdLibraryFile";
		default:
			INTERNAL_CHECK_WARRING( false );
			return "";
	};
}

//------------------------------------------------------------------------------

}
