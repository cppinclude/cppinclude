#include "parser/test/fixtures/pr_test_fixture.hpp"

#include "fs/impl/fs_factory_impl.hpp"
#include "fs/api/fs_file_system.hpp"

#include "parser/api/pr_include_file.hpp"
#include "parser/impl/pr_parser_impl.hpp"

#include "exception/ih/exc_internal_error.hpp"

#include <filesystem>

//------------------------------------------------------------------------------

namespace parser::test {

//------------------------------------------------------------------------------

ParserFixture::ParserFixture()
{
	init();
}

//------------------------------------------------------------------------------

ParserFixture::~ParserFixture() = default;

//------------------------------------------------------------------------------

void ParserFixture::init()
{
	m_factory.reset( new fs::FactoryImpl );
	INTERNAL_CHECK_ERROR( m_factory );
	fs::FileSystem & fs = m_factory->getMemoryFileSystem();

	const std::string tempFilePath = "/" + getTempFileName();

	m_fileWrapper.setFile( fs.createFile( tempFilePath ) );
}

//------------------------------------------------------------------------------

FileWrapper & ParserFixture::file()
{
	return m_fileWrapper;
}

//------------------------------------------------------------------------------

void ParserFixture::parse()
{
	ParserImpl parser;
	m_files = parser.parseFile( m_fileWrapper.getFile() );
}

//------------------------------------------------------------------------------

const ParserFixture::IncludeFiles & ParserFixture::getResults() const
{
	return m_files;
}

//------------------------------------------------------------------------------

std::string ParserFixture::getTempFileName() const
{
	return "temp.cpp";
}

//------------------------------------------------------------------------------

}
