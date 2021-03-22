#pragma once

//------------------------------------------------------------------------------

namespace model_includes {
	class File;
	class IncludeLocation;
	enum class IncludeType;
	enum class IncludeStatus;

//------------------------------------------------------------------------------

class Include
{
public:
	virtual ~Include() = default;

	virtual const IncludeLocation & getLocation() const = 0;

	virtual const File & getSourceFile() const = 0;
	virtual const File & getDestinationFile() const = 0;

	virtual IncludeStatus getStatus() const = 0;
	virtual IncludeType getType() const = 0;
};

//------------------------------------------------------------------------------

}
