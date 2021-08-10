#pragma once

#include <memory>
#include <stdfwd/string_view>

//------------------------------------------------------------------------------

namespace fs
{
class File;
}

//------------------------------------------------------------------------------

namespace parser::test
{
//------------------------------------------------------------------------------

class FileWrapper
{
public:
	using FilePtr = std::shared_ptr< fs::File >;

	~FileWrapper();

	FileWrapper & operator<<( std::string_view _str );

	void setFile( FilePtr && _file );
	const fs::File & getFile() const;

private:
	FilePtr m_file;
};

//------------------------------------------------------------------------------

}
