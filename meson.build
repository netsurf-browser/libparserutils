project('parserutils', 'c',
  version: '0.2.5',
  default_options: ['c_std=c99', 'warning_level=2'])

cc = meson.get_compiler('c')

# Compiler flags
cflags = ['-D_BSD_SOURCE', '-D_DEFAULT_SOURCE', '-pedantic']
if cc.get_id() != 'gcc' or cc.version().split('.')[0].to_int() >= 3
  cflags += ['-Wstrict-prototypes', '-Wmissing-prototypes', '-Wmissing-declarations', '-Wnested-externs']
endif
if host_machine.system() != 'haiku'
  cflags += ['-Werror']
endif

# Generated aliases.inc
perl = find_program('perl', required: true)
gen_aliases = custom_target('aliases_generator',
  output: 'aliases.inc',
  input: 'build/make-aliases.pl',
  command: [perl, '@INPUT@', meson.build_root(), meson.source_root() + '/src/charset', meson.source_root() + '/build/Aliases'],
  depend_files: ['build/Aliases'],
  build_by_default: true,
  console: true
)

# Source files
parserutils_sources = files(
  'src/charset/aliases.c',
  'src/charset/codec.c',
  'src/charset/codecs/codec_8859.c',
  'src/charset/codecs/codec_ascii.c',
  'src/charset/codecs/codec_ext8.c',
  'src/charset/codecs/codec_utf16.c',
  'src/charset/codecs/codec_utf8.c',
  'src/charset/encodings/utf16.c',
  'src/charset/encodings/utf8.c',
  'src/input/filter.c',
  'src/input/inputstream.c',
  'src/utils/buffer.c',
  'src/utils/errors.c',
  'src/utils/stack.c',
  'src/utils/vector.c'
)

# Include directories
inc = include_directories('include', 'src')
test_inc = include_directories('include', 'src', 'test')

# Build the static library
parserutils_lib = static_library('parserutils',
  sources: [parserutils_sources, gen_aliases],
  include_directories: inc,
  c_args: cflags,
  install: true
)

# Install headers
install_headers(
  'include/parserutils/errors.h',
  'include/parserutils/functypes.h',
  'include/parserutils/parserutils.h',
  'include/parserutils/types.h',
  subdir: 'parserutils'
)
install_headers(
  'include/parserutils/charset/codec.h',
  'include/parserutils/charset/mibenum.h',
  'include/parserutils/charset/utf16.h',
  'include/parserutils/charset/utf8.h',
  subdir: 'parserutils/charset'
)
install_headers(
  'include/parserutils/input/inputstream.h',
  subdir: 'parserutils/input'
)
install_headers(
  'include/parserutils/utils/buffer.h',
  'include/parserutils/utils/stack.h',
  'include/parserutils/utils/vector.h',
  subdir: 'parserutils/utils'
)

# Pkg-config file
pkg = import('pkgconfig')
pkg.generate(parserutils_lib,
  name: 'libparserutils',
  description: 'Parser utility library',
  version: '0.2.5',
  subdirs: 'parserutils'
)

# Tests
if get_option('tests')
  perl = find_program('perl', required: false)
  if perl.found()
    test_sources = [
      ['aliases', files('test/aliases.c')],
      ['cscodec-8859', files('test/cscodec-8859.c')],
      ['cscodec-ext8', files('test/cscodec-ext8.c')],
      ['cscodec-utf16', files('test/cscodec-utf16.c')],
      ['cscodec-utf8', files('test/cscodec-utf8.c')],
      ['filter', files('test/filter.c')],
      ['inputstream', files('test/inputstream.c')],
      ['buffer-discard', files('test/regression/buffer-discard.c')],
      ['filter-badenc-segv', files('test/regression/filter-badenc-segv.c')],
      ['filter-segv', files('test/regression/filter-segv.c')],
      ['stream-nomem', files('test/regression/stream-nomem.c')]
    ]

    test_common = files('test/testutils.c')
    test_executables = []
    foreach test : test_sources
      test_name = test[0]
      test_src = test[1]
      exe = executable('parserutils_test_' + test_name,
        sources: test_src + test_common,
        include_directories: test_inc,
        link_with: parserutils_lib,
        c_args: cflags,
        install: false
      )
      test_executables += [exe]
    endforeach

    test('parserutils_tests', perl,
      args: [
        meson.source_root() + '/test/testrunner.pl',
        meson.build_root(),                    # <builddir>
        meson.source_root() + '/test',         # <testdir>
        'parserutils_test_',                   # <prefix>
        ''                                     # <exeext>
      ],
      workdir: meson.source_root() + '/test'
    )
  endif
endif