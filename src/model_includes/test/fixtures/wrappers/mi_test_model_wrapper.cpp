#include "model_includes/test/fixtures/wrappers/mi_test_model_wrapper.hpp"

#include "model_includes/test/fixtures/wrappers/mi_test_file_wrapper.hpp"

#include "model_includes/api/enums/mi_file_type.hpp"
#include "model_includes/api/mi_file.hpp"
#include "model_includes/api/mi_model.hpp"

#include "exception/ih/exc_internal_error.hpp"

#include <memory>
#include <set>
#include <std_fs>

//------------------------------------------------------------------------------

namespace model_includes::test
{
//------------------------------------------------------------------------------

ModelWrapper::ModelWrapper( ModelPtr && _modelPtr )
	: m_modelPtr{ std::move( _modelPtr ) }
{
}

//------------------------------------------------------------------------------

ModelWrapper::~ModelWrapper() = default;

//------------------------------------------------------------------------------

bool ModelWrapper::isAvailable() const
{
	return m_modelPtr != nullptr;
}

//------------------------------------------------------------------------------

BoostPredicate ModelWrapper::checkFilesCount( std::size_t _expectCount ) const
{
	if( getModel().getFilesCount() == _expectCount )
	{
		return true;
	}

	return dump();
}

//------------------------------------------------------------------------------

FileWrapper ModelWrapper::findFile( std::string_view _path ) const
{
	Path path{ _path };
	path = stdfs::lexically_normal( path );
	return getModel().findFile( path );
}

//------------------------------------------------------------------------------

void ModelWrapper::addInclude(
	std::string_view _sourceFile,
	std::string_view _destinationFile,
	IncludeStatus _status,
	IncludeType _type,
	const LocationInfo & _location )
{
	Model & model = takeModel();

	FileType defaultType = FileType::ProjectFile;

	File & sourceFile = model.ensureFile( _sourceFile, defaultType );
	File & destinationFile = model.ensureFile( _destinationFile, defaultType );

	Model::IncludeLocationInfo location{
		_location.m_line, _location.m_begin, _location.m_end };

	model.createInclude(
		location, sourceFile, destinationFile, _status, _type );
}

//------------------------------------------------------------------------------

const Model & ModelWrapper::getModel() const
{
	INTERNAL_CHECK_ERROR( m_modelPtr );
	return *m_modelPtr;
}

//------------------------------------------------------------------------------

Model & ModelWrapper::takeModel()
{
	INTERNAL_CHECK_ERROR( m_modelPtr );
	return *m_modelPtr;
}

//------------------------------------------------------------------------------

std::string ModelWrapper::dump() const
{
	std::set< std::string > paths;
	getModel().forEachFile( [&]( const File & _file ) {
		const auto pair = paths.insert( _file.getPath().string() );
		const bool inserted = pair.second;
		INTERNAL_CHECK_WARRING( inserted );
		return true;
	} );

	std::stringstream result;
	result << '\n';
	int i = 1;
	result << "Files count: " << paths.size() << '\n';
	for( const auto & path: paths )
	{
		result << i++ << ": " << path << '\n';
	}

	return result.str();
}

//------------------------------------------------------------------------------

}
