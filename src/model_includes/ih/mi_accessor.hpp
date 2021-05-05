#pragma once

#include <stdfwd/memory>

//------------------------------------------------------------------------------

namespace fs {
	class FileSystem;
}

namespace parser {
	class Parser;
}

//------------------------------------------------------------------------------

namespace model_includes {
	class Analyzer;
	class Model;

//------------------------------------------------------------------------------

class ModelIncludesAccessor
{
public:

	using ModelPtr		= stdfwd::unique_ptr< Model >;
	using AnalyzerPtr	= stdfwd::unique_ptr< Analyzer >;

	virtual ~ModelIncludesAccessor() = default;

	virtual AnalyzerPtr createAnalyzer(
		const fs::FileSystem & _fs,
		const parser::Parser & _parser
	) = 0;

	virtual ModelPtr createEmptyModel() = 0;
};

//------------------------------------------------------------------------------

}
