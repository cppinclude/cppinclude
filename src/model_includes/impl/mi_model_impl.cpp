#include "model_includes/impl/mi_model_impl.hpp"

#include "model_includes/impl/mi_file_impl.hpp"
#include "model_includes/impl/mi_include_impl.hpp"
#include "model_includes/impl/mi_include_location_impl.hpp"

#include "exception/ih/exc_internal_error.hpp"

#include <memory>
#include <functional>

//------------------------------------------------------------------------------

namespace model_includes {

//------------------------------------------------------------------------------

ModelImpl::ModelImpl() = default;
ModelImpl::~ModelImpl() = default;

//------------------------------------------------------------------------------

std::size_t ModelImpl::getFilesCount() const
{
	return m_files.size();
}

//------------------------------------------------------------------------------

void ModelImpl::forEachFile( FileCallback _callback ) const
{
	for( const auto & pair : m_files )
	{
		const File * file = pair.second.get();
		INTERNAL_CHECK_WARRING( file );
		if( file )
		{
			if( !_callback( *file ) )
				break;
		}
	}
}

//------------------------------------------------------------------------------

void ModelImpl::setProjectDir( const std::filesystem::path & _path )
{
	m_projectDir = _path;
}

//------------------------------------------------------------------------------

const std::filesystem::path & ModelImpl::getProjectDir() const
{
	return m_projectDir;
}

//------------------------------------------------------------------------------

File & ModelImpl::ensureFile(
	const std::filesystem::path & _filePath,
	FileType _fileType
)
{
	const auto path = _filePath.lexically_normal();
	auto pair = m_files.try_emplace( path, new FileImpl{ path, _fileType } );
	auto it = pair.first;
	FilePtr & filePtr = it->second;
	INTERNAL_CHECK_ERROR( filePtr );
	return *filePtr;
}

//------------------------------------------------------------------------------

const File * ModelImpl::findFile( const std::filesystem::path & _filePath ) const
{
	if( auto it = m_files.find( _filePath ); it != m_files.end() )
		return it->second.get();

	return nullptr;
}

//------------------------------------------------------------------------------

void ModelImpl::forEachInclude( IncludeCallback _callback ) const
{
	for( const IncludePtr & includePtr : m_includes )
	{
		INTERNAL_CHECK_WARRING( includePtr );
		if( !includePtr )
			continue;

		if( !_callback( *includePtr ) )
			break;
	}
}

//------------------------------------------------------------------------------

const Include & ModelImpl::createInclude(
	const IncludeLocationInfo & _location,
	File & _sourceFile,
	File & _destinationFile,
	IncludeStatus _status,
	IncludeType _type
)
{
	IncludeLocationPtr location = createIncludeLocation( _location );
	IncludePtr includePtr{
		new IncludeImpl{
			std::move( location ),
			_sourceFile,
			_destinationFile,
			_status,
			_type
		}
	};
	INTERNAL_CHECK_WARRING( includePtr );
	const Include & include = *includePtr;

	m_includes.emplace_back( std::move( includePtr ) );

	_sourceFile.addInclude( include );
	_destinationFile.addInclude( include );

	return include;
}

//------------------------------------------------------------------------------

ModelImpl::IncludeLocationPtr ModelImpl::createIncludeLocation(
	const IncludeLocationInfo & _location
) const
{
	return IncludeLocationPtr{
		new IncludeLocationImpl{
			_location.m_line,
			_location.m_begin,
			_location.m_end
		}
	};
}

//------------------------------------------------------------------------------

}
