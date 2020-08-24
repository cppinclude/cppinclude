#pragma once

#include "parser/test/fixtures/pr_test_file_wrapper.hpp"

#include <vector>

//------------------------------------------------------------------------------

namespace fs {
	class Factory;
	class File;
}

//------------------------------------------------------------------------------

namespace parser {
	class IncludeFile;
}

//------------------------------------------------------------------------------

namespace parser::test {

//------------------------------------------------------------------------------

class ParserFixture
{
public:

	using IncludeFiles = std::vector< IncludeFile >;

	ParserFixture();
	~ParserFixture();

	FileWrapper & file();
	void parse();
	const IncludeFiles & getResults() const;

	std::string getTempFileName() const;

private:

	void init();

private:

	FileWrapper m_fileWrapper;
	std::unique_ptr< fs::Factory > m_factory;

	IncludeFiles m_files;

};

//------------------------------------------------------------------------------

}
