#include "parser/test/fixtures/pr_test_file_wrapper.hpp"

#include "fs/api/fs_file.hpp"

#include "exception/ih/exc_internal_error.hpp"

#include <string_view>

//------------------------------------------------------------------------------

namespace parser::test {

//------------------------------------------------------------------------------

FileWrapper::~FileWrapper() = default;

//------------------------------------------------------------------------------

FileWrapper & FileWrapper::operator<<( std::string_view _str )
{
	INTERNAL_CHECK_ERROR( m_file );
	(*m_file) << _str << "\n";
	return *this;
}

//------------------------------------------------------------------------------

void FileWrapper::setFile( FilePtr && _file )
{
	m_file.swap( _file );
}

//------------------------------------------------------------------------------

const fs::File & FileWrapper::getFile() const
{
	INTERNAL_CHECK_ERROR( m_file );
	return *m_file;
}

//------------------------------------------------------------------------------

}


