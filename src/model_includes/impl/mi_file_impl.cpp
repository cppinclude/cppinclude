#include "model_includes/impl/mi_file_impl.hpp"

#include "model_includes/api/mi_include.hpp"

#include "exception/ih/exc_internal_error.hpp"

#include <std_fs>
#include <functional>
#include <list>
#include <unordered_set>

//------------------------------------------------------------------------------

namespace model_includes {

//------------------------------------------------------------------------------

FileImpl::FileImpl( const Path & _path, FileType _type )
	:	m_path{ _path }
	,	m_type{ _type }
{
}

//------------------------------------------------------------------------------

FileImpl::Path FileImpl::getPath() const
{
	return m_path;
}

//------------------------------------------------------------------------------

FileType FileImpl::getType() const
{
	return m_type;
}

//------------------------------------------------------------------------------

void FileImpl::addInclude( const Include & _include )
{
	if( &_include.getSourceFile() == this )
		m_includes.push_back( &_include );
	else if( &_include.getDestinationFile() == this )
		m_includedBy.push_back( &_include );
	else
		INTERNAL_CHECK_WARRING( false );
}

//------------------------------------------------------------------------------

File::IncludeIndex FileImpl::getIncludesCount() const
{
	return m_includes.size();
}

//------------------------------------------------------------------------------

File::IncludeIndex FileImpl::getIncludeFilesCountRecursive() const
{
	return getCountRecursive(
		&File::getIncludesCount,
		&File::getInclude,
		&Include::getDestinationFile
	);
}

//------------------------------------------------------------------------------

const Include & FileImpl::getInclude( IncludeIndex _index ) const
{
	const Include * include = m_includes.at( _index );
	INTERNAL_CHECK_ERROR( include );
	return *include;
}

//------------------------------------------------------------------------------

File::IncludeIndex FileImpl::getIncludedByCount() const
{
	return m_includedBy.size();
}

//------------------------------------------------------------------------------

File::IncludeIndex FileImpl::getIncludedByFilesCountRecursive() const
{
	return getCountRecursive(
		&File::getIncludedByCount,
		&File::getIncludedBy,
		&Include::getSourceFile
	);
}

//------------------------------------------------------------------------------

const Include & FileImpl::getIncludedBy( IncludeIndex _index ) const
{
	const Include * include = m_includedBy.at( _index );
	INTERNAL_CHECK_ERROR( include );
	return *include;
}

//------------------------------------------------------------------------------

FileImpl::IncludeIndex FileImpl::getCountRecursive(
	IncludeIndex ( File::*_getCount )() const,
	const Include & ( File::*_getInclude )( IncludeIndex _index ) const,
	const File & ( Include::*_getFile )() const
) const
{
	IncludeIndex result = 0;

	std::list< const File * > files;
	std::unordered_set< const File * > uniqFiles;

	files.push_back( this );
	uniqFiles.insert( this );

	while( !files.empty() )
	{
		const File * currentFilePtr = files.front();
		files.pop_front();
		INTERNAL_CHECK_WARRING( currentFilePtr );
		if( !currentFilePtr )
			continue;

		const IncludeIndex count = (*currentFilePtr.*_getCount)();
		for( IncludeIndex i = 0; i < count; ++i )
		{
			const Include & include = (*currentFilePtr.*_getInclude)( i );
			const File & file = ( include.*_getFile )();

			if( auto pair = uniqFiles.insert( &file ); pair.second )
			{
				++result;
				files.push_back( &file );
			}
		}
	}
	return result;
}

//------------------------------------------------------------------------------

}
