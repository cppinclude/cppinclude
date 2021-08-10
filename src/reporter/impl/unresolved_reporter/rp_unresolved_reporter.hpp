#pragma once

#include "reporter/impl/rp_base_reporter_impl.hpp"

#include <stdfwd/set>
#include <stdfwd/unordered_map>
#include <stdfwd/vector>

//------------------------------------------------------------------------------

namespace model_includes
{
class Include;
class File;
}

//------------------------------------------------------------------------------

namespace reporter
{
struct IncludesBySourceSorter;
class FileWithCountContainer;

//------------------------------------------------------------------------------

class UnresolvedReporter final : public BaseReporterImpl
{
	using BaseClass = BaseReporterImpl;

public:
	explicit UnresolvedReporter( SettingsPtr && _settingsPtr );

	void report(
		const model_includes::Model & _model, std::ostream & _stream ) override;

	ReporterKind getKind() const override;

private:
	using UnorderedIncludes = stdfwd::vector< const model_includes::Include * >;
	using OrderedIncludes =
		stdfwd::set< const model_includes::Include *, IncludesBySourceSorter >;
	using UnorderedIncludesByDestination = stdfwd::
		unordered_map< const model_includes::File *, UnorderedIncludes >;

	using DestinationFileByCount = FileWithCountContainer;

	void collectIncludes(
		const model_includes::Model & _model,
		UnorderedIncludesByDestination & _unorderedIncludes ) const;

	static DestinationFileByCount orderDestinationByCount(
		const UnorderedIncludesByDestination & _unorderedIncludes );

	bool isUnresolvedInclude( const model_includes::Include & _include ) const;

	void report(
		const UnorderedIncludesByDestination & _unorderedIncludesByDestination,
		const DestinationFileByCount & _destinationFileByCount,
		const Path & _projectDir,
		std::ostream & _stream ) const;

	void report(
		const model_includes::Include & _include,
		const Path & _projectDir,
		std::ostream & _stream ) const;

	void reportDestinationFile(
		const model_includes::File & _file,
		size_t _number,
		const Path & _projectDir,
		std::ostream & _stream ) const;

	void reportSourceFiles(
		const UnorderedIncludes & _includes,
		const Path & _projectDir,
		std::ostream & _stream ) const;

	void reportSourceFile(
		const model_includes::Include & _include,
		size_t _number,
		const Path & _projectDir,
		std::ostream & _stream ) const;
};

//------------------------------------------------------------------------------

}
