#include "reporter/api/rp_factory.hpp"

//------------------------------------------------------------------------------

namespace reporter {

//------------------------------------------------------------------------------

class FactoryImpl final : public Factory
{
public:
	ReporterPtr createReporter( ReporterKind _kind ) override;

	ReporterPtr createDumpReporter() override;
	ReporterPtr createUnresolvedReporter() override;
	ReporterPtr createMostImpactReporter() override;
	ReporterPtr createUnincludedReporter() override;
	ReporterPtr createDifferentTypeReporter() override;

	SettingsPtr createSettings() override;
};

//------------------------------------------------------------------------------

}
