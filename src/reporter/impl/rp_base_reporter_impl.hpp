#pragma once

#include "reporter/api/rp_reporter.hpp"

//------------------------------------------------------------------------------

namespace reporter {

//------------------------------------------------------------------------------

class BaseReporterImpl : public Reporter
{
public:

	BaseReporterImpl();

	void setMaxFilesCount( size_t _count ) override;
	void setMaxDetailsCount( size_t _count ) override;
	void setShowStdFile( bool _enable ) override;

protected:

	size_t getMaxFilesCount() const;
	size_t getMaxDetailsCount() const;
	bool getShowStdFiles() const;

	std::string getPathWithoutProject(
		const std::filesystem::path & _filePath,
		const std::filesystem::path & _dirPath
	) const;

private:

	size_t m_maxFilesCount;
	size_t m_maxDetailsCount;
	bool m_showStdFile;
};

//------------------------------------------------------------------------------

}
