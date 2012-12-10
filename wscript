#! /usr/bin/env python
# encoding: utf-8

import os

APPNAME = 'gauge'
VERSION = '3.0.0'

def recurse_helper(ctx, name):

    if not ctx.has_dependency_path(name):
        ctx.fatal('Load a tool to find %s as system dependency' % name)
    else:
        p = ctx.dependency_path(name)
        ctx.recurse(p)

def options(opt):

    import waflib.extras.wurf_dependency_bundle as bundle
    import waflib.extras.wurf_dependency_resolve as resolve

    bundle.add_dependency(opt,
        resolve.ResolveGitMajorVersion(
            name='waf-tools',
            git_repository = 'git://github.com/steinwurf/external-waf-tools.git',
            major_version = 1))

    bundle.add_dependency(opt,
        resolve.ResolveGitMajorVersion(
            name = 'boost',
            git_repository = 'git://github.com/steinwurf/external-boost.git',
            major_version = 4))

    opt.load('wurf_dependency_bundle')
    opt.load('wurf_tools')

def configure(conf):

    if conf.is_toplevel():

        conf.load('wurf_dependency_bundle')
        conf.load_external_tool('mkspec', 'wurf_cxx_mkspec_tool')

        recurse_helper(conf, 'boost')

def build(bld):

    bld.stlib(features = 'cxx',
	      source   = bld.path.ant_glob('src/gauge/*.cpp'),
	      target   = 'gauge',
              export_includes = ['src'],
              use = ['boost_chrono', 'boost_system', 'boost_program_options'])

    if bld.is_toplevel():
        recurse_helper(bld, 'boost')

        bld.recurse('examples/sample_benchmarks')

