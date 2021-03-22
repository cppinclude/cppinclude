#include "model_includes/test/fixtures/wrappers/mi_test_file_wrapper.hpp"

#include "model_includes/test/fixtures/wrappers/mi_test_include_wrapper.hpp"

#include "model_includes/api/mi_file.hpp"
#include "model_includes/api/mi_include.hpp"

#include "exception/ih/exc_internal_error.hpp"

#include <std_fs>

//------------------------------------------------------------------------------

namespace model_includes::test {

//------------------------------------------------------------------------------

FileWrapper::FileWrapper( const File * _file  )
	:	m_file{ _file }
{
}

//------------------------------------------------------------------------------

bool FileWrapper::isAvailable() const
{
	return m_file != nullptr;
}

//------------------------------------------------------------------------------

BoostPredicate FileWrapper::checkIncludesCount( int _exceptCount ) const
{
	const File::IncludeIndex count = getFile().getIncludesCount();
	if( count == static_cast< File::IncludeIndex >( _exceptCount ) )
	{
		return true;
	}

	return dumpIncludes();
}

//------------------------------------------------------------------------------

BoostPredicate FileWrapper::checkIncludesCountRecursive( int _exceptCount ) const
{
	const File::IncludeIndex count = getFile().getIncludeFilesCountRecursive();
	if( count == static_cast< File::IncludeIndex >( _exceptCount ) )
	{
		return true;
	}

	return dumpIncludes();
}

//------------------------------------------------------------------------------

IncludeWrapper FileWrapper::getInclude( int _index ) const
{
	return getFile().getInclude( static_cast< File::IncludeIndex >( _index ) );
}

//------------------------------------------------------------------------------

BoostPredicate FileWrapper::checkIncludedByCount( int _exceptCount ) const
{
	const File::IncludeIndex count = getFile().getIncludedByCount();
	if( count == static_cast< File::IncludeIndex >( _exceptCount ) )
	{
		return true;
	}

	return dumpIncludedBy();
}

//------------------------------------------------------------------------------

BoostPredicate FileWrapper::checkIncludedByCountRecursive( int _exceptCount ) const
{
	const File::IncludeIndex count = getFile().getIncludedByFilesCountRecursive();
	if( count == static_cast< File::IncludeIndex >( _exceptCount ) )
	{
		return true;
	}

	return dumpIncludedBy();
}

//------------------------------------------------------------------------------

IncludeWrapper FileWrapper::getIncludedBy( std::string_view _sourcePath ) const
{
	Path sourcePath = _sourcePath;
	sourcePath = stdfs::lexically_normal( sourcePath );
	const File & file = getFile();
	for( File::IncludeIndex i = 0; i < file.getIncludedByCount(); ++i )
	{
		const Include & include = file.getIncludedBy( i );
		const File & sourceFile = include.getSourceFile();
		if( sourceFile.getPath() == sourcePath )
		{
			return include;
		}
	}
	return IncludeWrapper{};
}

//------------------------------------------------------------------------------

const File & FileWrapper::getFile() const
{
	INTERNAL_CHECK_ERROR( m_file );
	return *m_file;
}

//------------------------------------------------------------------------------

std::string FileWrapper::dumpIncludes() const
{
	std::stringstream result;
	const File & file = getFile();
	const File::IncludeIndex count = file.getIncludesCount();
	result << '\n';
	result << "count: " << count << std::endl;
	for( File::IncludeIndex i = 0; i < count; ++i )
	{
		const Include & include = file.getInclude( i );
		result << i << " : " << dump( include ) << std::endl;
	}
	return result.str();
}

//------------------------------------------------------------------------------

std::string FileWrapper::dumpIncludedBy() const
{
	std::stringstream result;
	const File & file = getFile();
	const File::IncludeIndex count = file.getIncludedByCount();
	result << '\n';
	result << "count: " << count << std::endl;
	for( File::IncludeIndex i = 0; i < count; ++i )
	{
		const Include & include = file.getIncludedBy( i );
		result << i << " : " << dump( include ) << std::endl;
	}
	return result.str();
}

//------------------------------------------------------------------------------

std::string FileWrapper::dump( const Include & _include ) const
{
	IncludeWrapper wrapper{ _include };
	return wrapper.dump();
}

//------------------------------------------------------------------------------

}
