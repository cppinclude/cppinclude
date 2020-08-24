#include "model_includes/ih/mi_accessor_impl.hpp"

#include "model_includes/impl/mi_analyzer_impl.hpp"
#include "model_includes/impl/mi_model_impl.hpp"

//------------------------------------------------------------------------------

namespace model_includes {

//------------------------------------------------------------------------------

ModelIncludesAccessorImpl::ModelIncludesAccessorImpl() = default;
ModelIncludesAccessorImpl::~ModelIncludesAccessorImpl() = default;

//------------------------------------------------------------------------------

ModelIncludesAccessorImpl::AnalyzerPtr ModelIncludesAccessorImpl::createAnalyzer(
	const fs::FileSystem & _fs,
	const parser::Parser & _parser
)
{
	return AnalyzerPtr{ new AnalyzerImpl{ _fs, _parser } };
}

//------------------------------------------------------------------------------

ModelIncludesAccessorImpl::ModelPtr ModelIncludesAccessorImpl::createEmptyModel()
{
	return ModelPtr{ new ModelImpl };
}

//------------------------------------------------------------------------------

}
