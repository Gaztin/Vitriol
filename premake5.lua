require 'third_party/premake-android-studio'
require 'premake/app'
require 'premake/customizations'
require 'premake/defaults'
require 'premake/library'
require 'premake/options'
require 'premake/settings'
require 'premake/target'
require 'premake/utils'

workspace( settings.workspace_name )
platforms( utils.get_platforms() )
configurations { 'Debug', 'Release' }

library 'Vitriol'

app 'TestServer'
	filter 'system:windows'
		links {
			'Ws2_32',
		}

-- Set last app as startup
workspace( settings.workspace_name )
startproject( apps[ #apps ] )
