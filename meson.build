project(
  'RenderMarkdown',
  'cpp',
  version : '0.1',
  meson_version : '>= 1.3.0',
  default_options : ['warning_level=3', 'cpp_std=c++20'],
)

dependencies = [
  dependency('Magick++', required: true),
  dependency('boost', modules: ['program_options'], required: true),
]

exe = executable(
  'RenderMarkdown',
  'src/main.cpp',
  'src/macros_defines.h',
  'src/make_image.cpp',
  'src/make_image.h',
  'src/parse_markdown.cpp',
  'src/parse_markdown.h',
  'src/render_markdown.cpp',
  'src/render_markdown.h',
  'src/test.cpp',
  'src/test.h',
  install : true,
  dependencies : dependencies,
)

test('basic', exe, args: ['--test'], workdir: meson.global_source_root())
