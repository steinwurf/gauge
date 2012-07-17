#! /usr/bin/env python
# encoding: utf-8

import os

APPNAME = 'gauge'
VERSION = '1.0.0'

def recurse_helper(ctx, name):
    if not ctx.has_dependency_path(name):
        ctx.fatal('Load a tool to find %s as system dependency' % name)
    else:
        p = ctx.dependency_path(name)
        ctx.recurse(p)

def options(opt):
    opt.load('toolchain_cxx')
    opt.load('dependency_bundle')

    import waflib.extras.dependency_bundle as bundle
    import waflib.extras.dependency_resolve as resolve

    bundle.add_dependency(opt,
        resolve.ResolveGitMajorVersion(
            name = 'boost',
            git_repository = 'git://github.com/steinwurf/external-boost.git',
            major_version = 1))

def configure(conf):

    if conf.is_toplevel():
        conf.load('toolchain_cxx')
        conf.load('dependency_bundle')

        recurse_helper(conf, 'boost')

def build(bld):

    if bld.is_toplevel():

        recurse_helper(bld, 'boost')
        bld.recurse('examples/sample_benchmarks')

    bld.stlib(features = 'cxx',
	      source   = bld.path.ant_glob('src/gauge/*.cpp'),
	      target   = 'gauge',
              cxxflags = bld.toolchain_cxx_flags(),
              export_includes = ['src'],
              use = ['boost_chrono', 'boost_system', 'boost_program_options'])



