#pragma once

#include <stdfwd.hpp>

#include <string>
#include <unordered_set>

//------------------------------------------------------------------------------

namespace model_includes {

//------------------------------------------------------------------------------

class StdLibrary
{
public:
	static const StdLibrary & getInstance();

	bool isExists( std::string_view _name ) const;

private:
	StdLibrary();

	void init();

	void initCppLibraties();
	void initCLibraries();

	void initConceptsLibrary();
	void initCoroutinesLibrary();
	void initUtilitiesLibrary();
	void initDynamicMemoryLibrary();
	void initNumberLimitLibrary();
	void initErrorHandlingLibrary();
	void initStringLibraryLibrary();
	void initContainersLibrary();
	void initIteratorsLibrary();
	void initRangesLibrary();
	void initAlgorithmsLibrary();
	void initNumericsLibrary();
	void initIOLibrary();
	void initLocalizationLibrary();
	void initRegexLibrary();
	void initAtomicLibrary();
	void initThreadLibrary();
	void initFsLibrary();
	void initCCompatibilityLibraries();
	void initEmptyCHeaders();
	void initMeaninglessCHeaders();
	void initUnsupportedCHeaders();

	using Headers = std::initializer_list< std::string >;
	void addHeaders( const Headers & _headers );

private:
	std::unordered_set< std::string > m_stdFiles;
};

//------------------------------------------------------------------------------

}
