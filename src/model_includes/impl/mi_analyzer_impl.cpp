#include "model_includes/impl/mi_analyzer_impl.hpp"

#include "model_includes/api/enums/mi_file_type.hpp"
#include "model_includes/api/enums/mi_include_status.hpp"
#include "model_includes/api/enums/mi_include_type.hpp"
#include "model_includes/api/mi_file.hpp"
#include "model_includes/api/mi_model.hpp"
#include "model_includes/impl/mi_analyzer_context.hpp"
#include "model_includes/impl/mi_model_impl.hpp"
#include "model_includes/impl/mi_resolver.hpp"
#include "model_includes/impl/mi_std_library.hpp"

#include "parser/api/pr_include_file.hpp"
#include "parser/api/pr_parser.hpp"

#include "fs/api/enums/fs_item_type.hpp"
#include "fs/api/fs_exceptions.hpp"
#include "fs/api/fs_file.hpp"
#include "fs/api/fs_file_system.hpp"

#include "project/api/prj_project.hpp"

#include "exception/ih/exc_internal_error.hpp"

#include "cmake_project/api/cprj_project.hpp"

#include <functional>
#include <memory>
#include <optional>
#include <string_view>
#include <vector>

//------------------------------------------------------------------------------

namespace model_includes {

//------------------------------------------------------------------------------

AnalyzerImpl::AnalyzerImpl(
	const fs::FileSystem & _fs,
	const parser::Parser & _parser
)
	:	m_fs{ _fs }
	,	m_parser{ _parser }
{
}

//------------------------------------------------------------------------------

AnalyzerImpl::ModelPtr AnalyzerImpl::analyze(
	const project::Project & _project
) const
{
	ModelPtr modelPtr = initModel( _project );
	AnalyzerContext context{ _project, *modelPtr };

	analyzeFolder( context, context.getProjectDir() );
	return modelPtr;
}

//------------------------------------------------------------------------------

AnalyzerImpl::ModelPtr AnalyzerImpl::analyze(
	const project::Project & _project,
	const cmake_project::Project & _cmakeProject
) const
{
	ModelPtr modelPtr = initModel( _project );
	AnalyzerContext context{ _project, _cmakeProject, *modelPtr };

	context.setNeedAnalyzeResolvedIncludes( true );

	_cmakeProject.forEachFilePath(
		[&]( const Path & _path )
		{
			if( !context.isFileInIgnoreDir( _path ) )
			{
				context.setCurrentCMakeSourceFile( _path );

				analyzeFile( context, _path );

				while( auto pathOpt = context.popResolvedFile() )
				{
					analyzeFile( context, *pathOpt );
				}
			}

			return true;
		}
	);

	return modelPtr;
}

//------------------------------------------------------------------------------

AnalyzerImpl::ModelPtr AnalyzerImpl::initModel( const project::Project & _project )
{
	ModelPtr modelPtr{ new ModelImpl };
	const Path & projectDir = _project.getProjectDir();
	INTERNAL_CHECK_WARRING( !projectDir.empty() );

	modelPtr->setProjectDir( projectDir );
	return modelPtr;
}

//------------------------------------------------------------------------------

void AnalyzerImpl::analyzeFolder(
	AnalyzerContext & _context,
	const Path & _folderPath
) const
{
	if( _context.isIgnoredDir( _folderPath ) )
	{
		return;
	}

	m_fs.forEachItem(
		_folderPath,
		[&]( const Path & _path, fs::ItemType _type )
		{
			static_assert( static_cast< int >( fs::ItemType::Count ) == 2 );
			switch( _type )
			{
				case fs::ItemType::Folder:
					analyzeFolder( _context, _path );
					break;

				case fs::ItemType::File:
					analyzeFile( _context, _path );
					break;

				default:
					INTERNAL_CHECK_WARRING( false );
			}
		}
	);
}

//------------------------------------------------------------------------------

void AnalyzerImpl::analyzeFile(
	AnalyzerContext & _context,
	const Path & _path
) const
{
	if( !_context.isCppFile( _path ) )
	{
		return;
	}

	if( _context.isIgnoredFile( _path ) )
	{
		return;
	}

	try
	{
		auto filePtr = m_fs.openFile( _path );
		INTERNAL_CHECK_WARRING( filePtr );
		if( !filePtr )
		{
			return;
		}

		auto includesFiles = m_parser.parseFile( *filePtr );
		analyzeIncludeFiles( _context, _path, includesFiles );
	}
	catch( const fs::Exception & _exception )
	{
		std::cout << _exception.what() << std::endl;
		return;
	}
}

//------------------------------------------------------------------------------

void AnalyzerImpl::analyzeIncludeFiles(
	AnalyzerContext & _context,
	const Path & _path,
	const IncludeFiles & _includesFile
) const
{
	const FileType fileType = getFileType( _path );
	File & file = _context.takeModel().ensureFile( _path, fileType );
	for( const parser::IncludeFile & includeFile : _includesFile )
	{
		analyzeIncludeFile( _context, _path, includeFile, file );
	}
}

//------------------------------------------------------------------------------

void AnalyzerImpl::analyzeIncludeFile(
	AnalyzerContext & _context,
	const Path & _path,
	const parser::IncludeFile & _includeFile,
	File & _file
) const
{
	if( _context.isIgnoreIncludeFile( _includeFile ) )
	{
		return;
	}

	ResolvedPath pair = resolvePath( _context, _path, _includeFile );

	const auto & includedFilePath = pair.first;
	if( _context.isIgnoredFile( includedFilePath ) )
	{
		return;
	}

	FileType fileType = getFileType( includedFilePath );

	Model & model = _context.takeModel();
	File & includedFile = model.ensureFile( includedFilePath, fileType );

	const parser::IncludeFileLocation & location = _includeFile.getLocation();
	Model::IncludeLocationInfo locationInfo{
		location.getLineNumber(),
		location.getBegin(),
		location.getEnd()
	};

	const IncludeType type = getIncludeType( _includeFile );
	const IncludeStatus status = getIncludeStatus( includedFile, pair );
	const auto & include = model.createInclude(
		locationInfo,
		_file,
		includedFile,
		status,
		type
	);

	postProcesNewInclude( _context, include );
}

//------------------------------------------------------------------------------

void AnalyzerImpl::postProcesNewInclude(
	AnalyzerContext & _context,
	const model_includes::Include & _include
)
{
	using namespace model_includes;

	if( !_context.isNeedAnalyzeResolvedIncludes() )
	{
		return;
	}

	static_assert( static_cast< int >( IncludeStatus::Count ) == 2 );
	const IncludeStatus includeStatus = _include.getStatus();

	switch( includeStatus )
	{
		case IncludeStatus::Unresolved:
			return;

		case IncludeStatus::Resolved:
		{
			const File & destinationFile = _include.getDestinationFile();

			static_assert( static_cast< int >( FileType::Count ) == 2 );
			const FileType type = destinationFile.getType();
			switch( type )
			{
				case FileType::ProjectFile:
				{
					_context.addResolvedFile( destinationFile.getPath() );
				}
				break;

				case FileType::StdLibraryFile:
					return;

				case FileType::Count:
				{
					INTERNAL_CHECK_WARRING( false );
					return;
				}
			}
		}
		break;

		case IncludeStatus::Count:
		{
			INTERNAL_CHECK_WARRING( false );
			return;
		}
	}
}

//------------------------------------------------------------------------------

FileType AnalyzerImpl::getFileType( const Path & _path ) const
{
	return Resolver::resolveFileType( _path );
}

//------------------------------------------------------------------------------

IncludeType AnalyzerImpl::getIncludeType(
	const parser::IncludeFile & _includesFile
) const
{
	static_assert( static_cast< int >( IncludeType::Count ) == 2 );
	return _includesFile.isSystem() ? IncludeType::System : IncludeType::User;
}

//------------------------------------------------------------------------------

IncludeStatus AnalyzerImpl::getIncludeStatus(
	const File & _file,
	const ResolvedPath & _pair
) const
{
	static_assert( static_cast< int >( FileType::Count ) == 2 );
	if( _file.getType() == FileType::StdLibraryFile )
	{
		return IncludeStatus::Resolved;
	}

	const bool isResolved = _pair.second;
	return isResolved ? IncludeStatus::Resolved : IncludeStatus::Unresolved;
}

//------------------------------------------------------------------------------

AnalyzerImpl::ResolvedPath AnalyzerImpl::resolvePath(
	const AnalyzerContext & _context,
	const Path & _currentFile,
	const parser::IncludeFile & _includeFile
) const
{
	std::string_view includeFileName = _includeFile.getName();
	Path filePath = includeFileName;
	bool isResolved = false;
	const Resolver & resolver = ensureResolver();

	auto pathOpt = resolver.resolvePath(
		_context.getProject(),
		_context.getCMakeProject(),
		_currentFile,
		_includeFile.getName(),
		_context.getCurrentCMakeSourceFile()
	);

	if( pathOpt )
	{
		filePath = *pathOpt;
		isResolved = true;
	}

	return ResolvedPath{ filePath, isResolved };
}

//------------------------------------------------------------------------------

Resolver & AnalyzerImpl::ensureResolver() const
{
	if( !m_resolver )
	{
		m_resolver = std::make_unique< Resolver >( m_fs );
	}

	return *m_resolver;
}

//------------------------------------------------------------------------------

}
