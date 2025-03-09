project(
	'snake',
	'cpp',
	default_options: [
		'cpp_std=c++20'
	]
)
executable(
	'snake',
	files(
		[
			'src/main.cpp',
			'src/snake/map.cpp',
			'src/snake/window.cpp'
		]
	),
	dependencies: dependency('gtkmm-4.0'),
	include_directories: include_directories('include')
)
