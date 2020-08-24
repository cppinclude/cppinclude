#include "model_includes/ih/mi_accessor.hpp"

#include <memory>

//------------------------------------------------------------------------------

namespace model_includes {

//------------------------------------------------------------------------------

class ModelIncludesAccessorImpl final : public ModelIncludesAccessor
{
public:

	ModelIncludesAccessorImpl();
	~ModelIncludesAccessorImpl() override;

	AnalyzerPtr createAnalyzer(
		const fs::FileSystem & _fs,
		const parser::Parser & _parser
	) override;

	ModelPtr createEmptyModel() override;

private:

	std::unique_ptr< Analyzer >	m_analyzer;
};

//------------------------------------------------------------------------------

}
