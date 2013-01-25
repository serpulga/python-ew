from distutils.core import setup, Extension

libs = '/path/to/ew/lib/'
includes = '/path/to/ew/includes/'

module = Extension(name = 'tracebuf2module', 
                   sources = ['tracebuf2module.c'],
		           include_dirs = [includes],
                   libraries = [libs],
		           extra_objects = ['%sringwriter.o' % libs, '%sringreader.o' % libs, '%stransport.o' % libs, '%sgetutil.o' % libs, '%skom.o' % libs,
				   '%ssleep_ew.o' % libs, '%slogit.o' % libs, '%stime_ew.o' % libs, '%sswap.o' % libs],
		           extra_compile_args = ['-m32', '-Dlinux', '-D__i386', '-D_LINUX', '-D_INTEL', '-D_USE_SCHED', '-D_USE_PTHREADS', '-D_USE_TERMIOS'],
		           extra_link_args = ['-lm', '-lpthread'])

setup (name = 'tracebuf2module',
       version = '1.0', 
       description = 'Writes/reads a TYPE_TRACEBUF2 message into/from a ring.',
       ext_modules = [module])
