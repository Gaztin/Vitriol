
libraries = { }

function library( name )
	group 'Libraries'
	project( name )

	appid '%{settings.bundle_namespace}.libs.%{string.lower(prj.name)}'
	kind 'StaticLib'
	links( libraries )
	location 'build/%{_ACTION}'

	files {
		'src/%{prj.name}/**.cpp',
		'src/%{prj.name}/**.h',
	}

	filter 'system:macosx or ios'
		files { 'src/%{prj.name}/**.mm' }

	filter { 'system:macosx or ios', 'files:**.cpp' }
		compileas 'Objective-C++'

	filter { }

	table.insert( libraries, name )
end
