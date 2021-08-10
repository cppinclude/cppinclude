#pragma once

#include "model_includes/api/mi_include.hpp"

#include <memory>

//------------------------------------------------------------------------------

namespace model_includes
{
//------------------------------------------------------------------------------

class IncludeImpl final : public Include
{
public:
	using IncludeLocationPtr = std::unique_ptr< IncludeLocation >;

	IncludeImpl(
		IncludeLocationPtr && _location,
		const File & _sourceFile,
		const File & _destinationFile,
		IncludeStatus _status,
		IncludeType _type );

	~IncludeImpl();

	const IncludeLocation & getLocation() const override;

	const File & getSourceFile() const override;
	const File & getDestinationFile() const override;

	IncludeStatus getStatus() const override;
	IncludeType getType() const override;

private:
	IncludeLocationPtr m_location;

	const File & m_sourceFile;
	const File & m_destinationFile;

	IncludeStatus m_status;
	IncludeType m_type;
};

//------------------------------------------------------------------------------

}
