#pragma once

#include <type_traits>
#include <vector>

//------------------------------------------------------------------------------

namespace tools {

//------------------------------------------------------------------------------

template <typename T>
using is_vector =
	std::is_same<
		T,
		std::vector< typename T::value_type	>
	>
;

//------------------------------------------------------------------------------

}
