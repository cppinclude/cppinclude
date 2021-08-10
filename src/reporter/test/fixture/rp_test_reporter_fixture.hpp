#pragma once

#include "model_includes/api/enums/mi_file_type.hpp"
#include "model_includes/api/enums/mi_include_status.hpp"
#include "model_includes/api/enums/mi_include_type.hpp"

#include <memory>
#include <std_fs>
#include <stdfwd/string_view>

//------------------------------------------------------------------------------

namespace model_includes
{
class Model;
class ModelIncludesAccessor;
class File;
}

//------------------------------------------------------------------------------

namespace reporter
{
class ReporterAccessor;
class Factory;
class Reporter;
class Settings;
}

//------------------------------------------------------------------------------

namespace reporter::test
{
//------------------------------------------------------------------------------

class ReporterFixture
{
public:
	ReporterFixture();
	~ReporterFixture();

	using IncludeStatus = model_includes::IncludeStatus;
	using IncludeType = model_includes::IncludeType;
	using FileType = model_includes::FileType;
	using Path = stdfs::path;

	struct LocationInfo
	{
		std::size_t m_line;
		std::size_t m_begin;
		std::size_t m_end;
	};

	void addInclude(
		std::string_view _sourceFile,
		std::string_view _destinationFile,
		IncludeType _type );
	void addUserInclude(
		std::string_view _sourceFile, std::string_view _destinationFile );
	void addSystemInclude(
		std::string_view _sourceFile, std::string_view _destinationFile );
	void addInclude(
		std::string_view _sourceFile,
		std::string_view _destinationFile,
		IncludeStatus _status = IncludeStatus::Resolved,
		IncludeType _type = IncludeType::User,
		const LocationInfo & _location = { 1, 1, 1 } );

	void setMaxFilesCount( int _count );
	void setMaxDetailsCount( int _count );
	void setShowStdFiles( bool _enable );
	void setShowOnlyStdHeaders( bool _enable );
	void setShowDetails( bool _enable );

	void setProjectDir( std::string_view _dir );
	Path getProjectDir();

	std::string runDumpReporter();
	std::string runUnresolvedReporter();
	std::string runMostImpactReporter();
	std::string runUnincludedReporter();
	std::string runDifferentTypeReport();

	model_includes::File & addFile(
		std::string_view _file,
		model_includes::FileType _type = FileType::ProjectFile );

	model_includes::File & addFileToProject( std::string_view _file );

	static std::string toPath( std::string_view _str );

	void setSystemThousandsSeparator( char _separator );

private:
	ReporterAccessor & ensureReportAccessor();
	Factory & getFactory();
	Settings & ensureSettings();

	std::string runReporter( Reporter & _reporter );

	model_includes::ModelIncludesAccessor & getModelAccessor();
	model_includes::Model & getModel();

private:
	std::unique_ptr< ReporterAccessor > m_accessor;
	std::unique_ptr< Settings > m_settings;
	std::unique_ptr< model_includes::ModelIncludesAccessor > m_modelAccessor;
	std::unique_ptr< model_includes::Model > m_model;
};

//------------------------------------------------------------------------------

}
