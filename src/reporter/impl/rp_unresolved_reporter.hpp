#pragma once

#include "reporter/impl/rp_base_reporter_impl.hpp"

#include <functional>

//------------------------------------------------------------------------------

namespace model_includes {
	class Include;
	class File;
}

//------------------------------------------------------------------------------

namespace reporter {

//------------------------------------------------------------------------------

class UnresolvedReporter final : public BaseReporterImpl
{
public:

	void report(
		const model_includes::Model & _model,
		std::ostream & _stream
	) override;

	ReporterKind getKind() const override;

private:

	class SourceOrder
	{
	public:
		bool operator()(
			const model_includes::Include * _r,
			const model_includes::Include * _l
		) const;
	};

	using UnorderedIncludes = stdfwd::vector< const model_includes::Include * >;
	using OrderedIncludes = stdfwd::set<
		const model_includes::Include *,
		SourceOrder
	>;
	using UnorderedIncludesByDestination = stdfwd::unordered_map<
		const model_includes::File *,
		UnorderedIncludes
	>;

	struct DestinationFileInfo
	{
		const model_includes::File * m_file;
		std::size_t m_count;
	};

	class DestinationFileInfoorder
	{
	public:
		bool operator()(
			const DestinationFileInfo & _r,
			const DestinationFileInfo & _l
		) const;
	};

	using DestinationFileByCount = stdfwd::set<
		DestinationFileInfo,
		DestinationFileInfoorder
	>;

	void collectIncludes(
		const model_includes::Model & _model,
		UnorderedIncludesByDestination & _unorderedIncludes
	) const;

	static DestinationFileByCount orderDestinationByCount(
		const UnorderedIncludesByDestination & _unorderedIncludes
	);

	bool isUnresolvedInclude( const model_includes::Include & _include ) const;

	void report(
		const UnorderedIncludesByDestination & _unorderedIncludesByDestination,
		const DestinationFileByCount & _destinationFileByCount,
		const Path & _projectDir,
		std::ostream & _stream
	) const;

	void report(
		const model_includes::Include & _include,
		const Path & _projectDir,
		std::ostream & _stream
	) const;

	void reportDestinationFile(
		const model_includes::File & _file,
		size_t _number,
		const Path & _projectDir,
		std::ostream & _stream
	) const;

	void reportSourceFiles(
		const UnorderedIncludes & _includes,
		const Path & _projectDir,
		std::ostream & _stream
	) const;

	void reportSourceFile(
		const model_includes::Include & _include,
		size_t _number,
		const Path & _projectDir,
		std::ostream & _stream
	) const;
};

//------------------------------------------------------------------------------

}
