#pragma once

#include <memory>

//------------------------------------------------------------------------------

namespace application {

//------------------------------------------------------------------------------

template< class _Plugin > class PluginPtr
{
public:
	template< class _PluginImpl > _Plugin & ensure()
	{
		if( !m_plugin )
			m_plugin.reset( new _PluginImpl );

		return *m_plugin;
	}

private:
	std::unique_ptr< _Plugin > m_plugin;
};

//------------------------------------------------------------------------------

}
