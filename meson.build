project('vong', 'c', version: '0.1', default_options: [
  'c_std=c23',
  'warning_level=3',
  'b_ndebug=if-release'
])

bt = get_option('buildtype')
if bt == 'release'
  add_project_arguments('werror=true', language: 'c')
endif 

inc = include_directories('include')

subdir('src')