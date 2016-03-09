# Makefile for codegen with Balerion.

subdir = src/backend/codegen
top_builddir = ../../..
include $(top_builddir)/src/Makefile.global

OBJS = codegen_manager.o codegen_wrapper.o codegen.o slot_deform_tuple_codegen.o

override CXXFLAGS := -std=c++11 $(CXXFLAGS) -fno-rtti

include $(top_srcdir)/src/backend/common.mk
