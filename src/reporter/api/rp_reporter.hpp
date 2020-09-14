#pragma once

#include "stdfwd.hpp"

#include <stdio.h>

//------------------------------------------------------------------------------

namespace model_includes {
	class Model;
}

//------------------------------------------------------------------------------

namespace reporter {
	enum class ReporterKind;

//------------------------------------------------------------------------------

class Reporter
{
public:

	virtual ~Reporter() = default;

	virtual ReporterKind getKind() const = 0;

	virtual void setMaxFilesCount( size_t _count ) = 0;
	virtual void setMaxDetailsCount( size_t _count ) = 0;
	virtual void setShowStdFile( bool _enable ) = 0;

	virtual void report(
		const model_includes::Model & _model,
		std::ostream & _stream
	) = 0;
};

//------------------------------------------------------------------------------

}
