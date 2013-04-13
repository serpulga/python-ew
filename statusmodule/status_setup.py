import os

from distutils.core import setup, Extension


EW_HOME = os.getenv('EW_HOME') 
libs = EW_HOME + '/lib/'
includes = EW_HOME + '/includes/'

module = Extension(name = 'statusmodule', 
                   sources = ['statusmodule.c'],
                   include_dirs = [includes],
                   libraries = [libs],
                   extra_objects = ['%sringwriter.o' % libs, '%sringreader.o' % libs, '%stransport.o' % libs, '%sgetutil.o' % libs, '%skom.o' % libs,
                   '%ssleep_ew.o' % libs, '%slogit.o' % libs, '%stime_ew.o' % libs, '%sswap.o' % libs],
                   extra_compile_args = ['-Dlinux', '-D__i386', '-D_LINUX', '-D_INTEL', '-D_USE_SCHED', '-D_USE_PTHREADS', '-D_USE_TERMIOS'],
                   extra_link_args = ['-lm', '-lpthread'])

setup (name = 'statusmodule',
       version = '1.0', 
       description = 'Reads TYPE_STATUS message from a ring.',
       ext_modules = [module])
