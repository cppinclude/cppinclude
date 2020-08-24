#pragma once

#include "model_includes/api/enums/mi_include_status.hpp"
#include "model_includes/api/enums/mi_include_type.hpp"

#include <stdfwd.hpp>
#include <memory>

//------------------------------------------------------------------------------

namespace model_includes {
	class Model;
	class ModelIncludesAccessor;
}

//------------------------------------------------------------------------------

namespace reporter {
	class ReporterAccessor;
	class Factory;
	class Reporter;
}

//------------------------------------------------------------------------------

namespace reporter::test {

//------------------------------------------------------------------------------

class ReporterFixture
{
public:

	ReporterFixture();
	~ReporterFixture();

	using IncludeStatus = model_includes::IncludeStatus;
	using IncludeType = model_includes::IncludeType;

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

	void setMaxFilesCount( int _count );
	void setMaxDetailsCount( int _count );

	void setProjectDir( std::string_view _dir );

	std::string runDumpReporter();
	std::string runUnresolvedReporter();
	std::string runMostImpactReporter();

	static std::string toPath( std::string_view _str );

private:

	ReporterAccessor & getAccessor();
	Factory & getFactory();

	std::string runReporter( Reporter & _reporter );

	model_includes::ModelIncludesAccessor & getModelAccessor();
	model_includes::Model & getModel();

private:

	int m_maxFilesCount;
	int m_maxDetailsCount;

	std::unique_ptr< ReporterAccessor > m_accessor;
	std::unique_ptr< model_includes::ModelIncludesAccessor > m_modelAccessor;
	std::unique_ptr< model_includes::Model > m_model;
};

//------------------------------------------------------------------------------

}
