#include "model_includes/impl/mi_analyzer_impl.hpp"

#include "model_includes/api/mi_model.hpp"
#include "model_includes/api/mi_file.hpp"
#include "model_includes/api/enums/mi_file_type.hpp"
#include "model_includes/api/enums/mi_include_type.hpp"
#include "model_includes/api/enums/mi_include_status.hpp"
#include "model_includes/impl/mi_model_impl.hpp"
#include "model_includes/impl/mi_std_library.hpp"
#include "model_includes/impl/mi_resolver.hpp"

#include "parser/api/pr_parser.hpp"
#include "parser/api/pr_include_file.hpp"

#include "fs/api/fs_file_system.hpp"
#include "fs/api/fs_file.hpp"
#include "fs/api/enums/fs_item_type.hpp"
#include "fs/api/exceptions/fs_exceptions.hpp"

#include "project/api/prj_project.hpp"

#include "exception/ih/exc_internal_error.hpp"

#include <memory>
#include <string_view>
#include <vector>
#include <functional>
#include <optional>

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
	ModelPtr modelPtr{ new ModelImpl };
	const Path & projectDir = _project.getProjectDir();
	INTERNAL_CHECK_WARRING( !projectDir.empty() );

	modelPtr->setProjectDir( projectDir );

	IgnoredFiles ignoreFiles;
	analyzeFolder( _project, projectDir, *modelPtr, ignoreFiles );
	return modelPtr;
}

//------------------------------------------------------------------------------

void AnalyzerImpl::analyzeFolder(
	const Project & _project,
	const Path & _folderPath,
	Model & _model,
	IgnoredFiles & _ignoreFiles
) const
{
	if( _project.isIgnoredDir( _folderPath ) )
		return;

	m_fs.forEachItem(
		_folderPath,
		[&]( const Path & _path, fs::ItemType _type )
		{
			static_assert( static_cast< int >( fs::ItemType::Count ) == 2 );
			switch ( _type )
			{
				case fs::ItemType::Folder :
					analyzeFolder( _project, _path, _model, _ignoreFiles );
					break;

				case fs::ItemType::File :
					analyzeFile( _project, _path, _model, _ignoreFiles );
					break;

				default:
					INTERNAL_CHECK_WARRING( false );
			}
		}
	);
}

//------------------------------------------------------------------------------

void AnalyzerImpl::analyzeFile(
	const Project & _project,
	const Path & _path,
	Model & _model,
	IgnoredFiles & _ignoredFiles
) const
{
	if( !isCppFile( _project, _path ) )
		return;

	if( isIgnoredFile( _path, _project, _model, _ignoredFiles ) )
		return;

	try
	{
		auto filePtr = m_fs.openFile( _path );
		INTERNAL_CHECK_WARRING( filePtr );
		if( !filePtr )
			return;

		auto includesFiles = m_parser.parseFile( *filePtr );
		analyzeIncludeFiles( _project, _path, includesFiles, _model, _ignoredFiles );
	}
	catch( const fs::Exception & _exception )
	{
		std::cerr << _exception.what() << std::endl;
		return;
	}

}

//------------------------------------------------------------------------------

void AnalyzerImpl::analyzeIncludeFiles(
	const Project & _project,
	const Path & _path,
	const IncludeFiles & _includesFile,
	Model & _model,
	IgnoredFiles & _ignoreFiles
) const
{
	const FileType fileType = getFileType( _path );
	File & file = _model.ensureFile( _path, fileType );
	for( const parser::IncludeFile & includeFile : _includesFile )
	{
		analyzeIncludeFile( _project, _path, includeFile, file, _model, _ignoreFiles );
	}
}

//------------------------------------------------------------------------------

void AnalyzerImpl::analyzeIncludeFile(
	const Project & _project,
	const Path & _path,
	const parser::IncludeFile & _includesFile,
	File & _file,
	Model & _model,
	IgnoredFiles & _ignoreFiles
) const
{
	if( _project.getIgnoreSystemIncludes() && _includesFile.isSystem() )
	{
		return;
	}

	ResolvedPath pair = resolvePath( _project, _path, _includesFile );

	const auto & includedFilePath = pair.first;
	if( isIgnoredFile( includedFilePath, _project, _model, _ignoreFiles ) )
		return;

	FileType fileType	= getFileType( includedFilePath );
	File & includedFile	= _model.ensureFile( includedFilePath, fileType );

	const parser::IncludeFileLocation & location = _includesFile.getLocation();
	Model::IncludeLocationInfo locationInfo{
		location.getLineNumber(),
		location.getBegin(),
		location.getEnd()
	};

	const IncludeType type		= getIncludeType( _includesFile );
	const IncludeStatus status	= getIncludeStatus( includedFile, pair );
	_model.createInclude( locationInfo, _file, includedFile, status, type );
}

//------------------------------------------------------------------------------

bool AnalyzerImpl::isCppFile( const Project & _project, const Path & _path ) const
{
	if( !_project.hasCppFileExtensions() )
		return true;

	const Path extension = _path.extension();

	if( extension.empty() && _project.getAnalyzeWithoutExtension() )
		return true;

	const bool result = _project.isExistsCppExtension( extension.string() );
	return result;
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
	return _includesFile.isSystem() ? IncludeType::System : IncludeType::User ;
}

//------------------------------------------------------------------------------

IncludeStatus AnalyzerImpl::getIncludeStatus(
	const File & _file,
	const ResolvedPath & _pair
) const
{
	static_assert( static_cast< int >( FileType::Count ) == 2 );
	if( _file.getType() == FileType::StdLibraryFile )
		return IncludeStatus::Resolved;

	const bool isResolved = _pair.second;
	return isResolved ? IncludeStatus::Resolved : IncludeStatus::Unresolved;
}

//------------------------------------------------------------------------------

bool AnalyzerImpl::isIgnoredFile(
	const Path & _path,
	const Project & _project,
	const Model & _model,
	IgnoredFiles & _ignoreFiles
)
{
	if( _model.findFile( _path ) )
		return false;

	if( _ignoreFiles.count( _path ) )
		return true;

	if( _project.isIgnoredFile( _path ) )
	{
		_ignoreFiles.insert( _path );
		return true;
	}

	return false;
}

//------------------------------------------------------------------------------

AnalyzerImpl::ResolvedPath AnalyzerImpl::resolvePath(
	const Project & _project,
	const Path & _currentFile,
	const parser::IncludeFile & _includeFile
) const
{
	std::string_view includeFileName = _includeFile.getName();
	Path filePath = includeFileName;
	bool isResolved = false;
	Resolver resolver{ m_fs, _project };

	auto pathOpt = resolver.resolvePath(
		_currentFile,
		_includeFile.getName()
	);

	if( pathOpt )
	{
		filePath = *pathOpt;
		isResolved = true;
	}
	return ResolvedPath{ filePath,isResolved };
}

//------------------------------------------------------------------------------

}
