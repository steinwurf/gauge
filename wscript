#! /usr/bin/env python
# encoding: utf-8

import os

APPNAME = 'gauge'
VERSION = '2.0.0'

def recurse_helper(ctx, name):

    if not ctx.has_dependency_path(name):
        ctx.fatal('Load a tool to find %s as system dependency' % name)
    else:
        p = ctx.dependency_path(name)
        ctx.recurse(p)

def options(opt):
    opt.load('waf_unit_test_v2')
    opt.load('wurf_cxx_mkspec')
    opt.load('dependency_bundle')

    import waflib.extras.dependency_bundle as bundle
    import waflib.extras.dependency_resolve as resolve

    bundle.add_dependency(opt,
        resolve.ResolveGitMajorVersion(
            name = 'boost',
            git_repository = 'git://github.com/steinwurf/external-boost.git',
            major_version = 3))

    bundle.add_dependency(opt,
        resolve.ResolveGitMajorVersion(
            name='mkspec',
            git_repository = 'git://github.com/steinwurf/external-waf-mkspec.git',
            major_version = 2))


def configure(conf):

    if conf.is_toplevel():
        conf.load('dependency_bundle')
        conf.load('waf_unit_test_v2')
        conf.load('wurf_cxx_mkspec')

        recurse_helper(conf, 'boost')

def build(bld):

    if bld.is_toplevel():

        recurse_helper(bld, 'boost')
        bld.recurse('examples/sample_benchmarks')

    bld.stlib(features = 'cxx',
	      source   = bld.path.ant_glob('src/gauge/*.cpp'),
	      target   = 'gauge',
              export_includes = ['src'],
              use = ['boost_chrono', 'boost_system', 'boost_program_options'])



