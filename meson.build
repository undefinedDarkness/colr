project('colr', 'c')

# Add Math Library
cc = meson.get_compiler('c')

deps = [ dependency('gtk+-3.0'), dependency('x11'), cc.find_library('m', required: false) ]

src = [
  'src/app.c',
  'src/util.c',
  'src/ui/ui.c',
  'src/color/color.c',
  'src/ui/colorspace.c',
  # 'src/ui/editor.c',
  'src/ui/editor-hsv.c',

  'resources/resources.c'
]

executable('colr', sources : src, dependencies : deps)

