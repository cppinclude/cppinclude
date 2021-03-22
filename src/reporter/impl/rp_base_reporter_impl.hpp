#pragma once

#include "reporter/api/rp_reporter.hpp"

#include <memory>

//------------------------------------------------------------------------------

namespace reporter {

//------------------------------------------------------------------------------

class BaseReporterImpl : public Reporter
{
public:
	using SettingsPtr = std::unique_ptr< Settings >;

	BaseReporterImpl( SettingsPtr && _settingsPtr );
	~BaseReporterImpl();

	const Settings & getSettings() const override;
	void copySettings( const Settings & _settings ) override;

protected:
	using Path = stdfs::path;
	using CountType = size_t;

	CountType getMaxFilesCount() const;
	CountType getMaxDetailsCount() const;
	bool getShowStdFiles() const;

	static std::string getPathWithoutProject(
		const Path & _filePath,
		const Path & _dirPath
	);

	static bool isFromSameDirectory( const Path & _path1, const Path & _path2 );

	static Path getCommonPath( const Path & _path1, const Path & _path2 );

	bool isLimitFiles( CountType _currentNumber ) const;
	bool isLimitFilesWithOriginSize(
		CountType _currentNumber,
		CountType _originSize
	) const;
	void printFileLimitLine(
		CountType _filesCount,
		std::ostream & _stream
	) const;

	bool isLimitDetails( CountType _currentNumber ) const;
	void printDetailsLimitLine(
		CountType _detailsCount,
		std::ostream & _stream
	) const;

private:
	static bool isLimit( CountType _currentNumber, CountType _limit );

private:
	SettingsPtr m_settings;
};

//------------------------------------------------------------------------------

}
