project('pomodoro', 'cpp',
	default_options: ['cpp_std=c++14']
)
cxx = meson.get_compiler('cpp')
libnotify = dependency('libnotify')
boost     = dependency('boost', modules: ['system'])
threads   = dependency('threads')

executable('pomodoro', 'main.cc', 'Pomodoro.cc', 'NotifyService.cc' , dependencies: [libnotify, boost, threads])
