#pragma once

#include "test_tools/boost_predicate_wrapper.hpp"

#include "model_includes/api/enums/mi_include_status.hpp"
#include "model_includes/api/enums/mi_include_type.hpp"

#include <stdfwd.hpp>

//------------------------------------------------------------------------------

namespace model_includes {
	class Model;
	class File;
}

//------------------------------------------------------------------------------

namespace model_includes::test {
	class FileWrapper;

//------------------------------------------------------------------------------

class ModelWrapper
{
public:

	using ModelPtr	= stdfwd::unique_ptr< Model >;
	using Path		= stdfs::path;

	ModelWrapper( ModelPtr && _modelPtr );
	~ModelWrapper();

	bool isAvailable() const;

	BoostPredicate checkFilesCount( std::size_t _expectCount ) const;
	FileWrapper findFile( std::string_view _path ) const;

	struct LocationInfo
	{
		std::size_t m_line;
		std::size_t m_begin;
		std::size_t m_end;
	};

	void addInclude(
		std::string_view _sourceFile,
		std::string_view _destinationFile,
		IncludeStatus _status = IncludeStatus::Resolved,
		IncludeType _type = IncludeType::User,
		const LocationInfo & _location = {1,1,1}
	);

private:
	const Model & getModel() const;
	Model & takeModel();

	std::string dump() const;

private:
	ModelPtr m_modelPtr;
};

//------------------------------------------------------------------------------

}
