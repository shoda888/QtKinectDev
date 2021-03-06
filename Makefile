#############################################################################
# Makefile for building: QtKinectRecorder_V3
# Generated by qmake (2.01a) (Qt 4.8.6) on: ? 2 22 06:54:37 2017
# Project:  QtKinectRecorder.pro
# Template: app
# Command: c:\Python27\Lib\site-packages\PyQt4\qmake.exe -o Makefile QtKinectRecorder.pro
#############################################################################

first: release
install: release-install
uninstall: release-uninstall
MAKEFILE      = Makefile
QMAKE         = c:\Python27\Lib\site-packages\PyQt4\qmake.exe
DEL_FILE      = del
CHK_DIR_EXISTS= if not exist
MKDIR         = mkdir
COPY          = copy /y
COPY_FILE     = $(COPY)
COPY_DIR      = xcopy /s /q /y /i
INSTALL_FILE  = $(COPY_FILE)
INSTALL_PROGRAM = $(COPY_FILE)
INSTALL_DIR   = $(COPY_DIR)
DEL_FILE      = del
SYMLINK       = 
DEL_DIR       = rmdir
MOVE          = move
CHK_DIR_EXISTS= if not exist
MKDIR         = mkdir
SUBTARGETS    =  \
		release \
		debug

release: $(MAKEFILE).Release FORCE
	@set MAKEFLAGS=$(MAKEFLAGS)
	$(MAKE) -f $(MAKEFILE).Release
release-make_default: $(MAKEFILE).Release FORCE
	@set MAKEFLAGS=$(MAKEFLAGS)
	$(MAKE) -f $(MAKEFILE).Release 
release-make_first: $(MAKEFILE).Release FORCE
	@set MAKEFLAGS=$(MAKEFLAGS)
	$(MAKE) -f $(MAKEFILE).Release first
release-all: $(MAKEFILE).Release FORCE
	@set MAKEFLAGS=$(MAKEFLAGS)
	$(MAKE) -f $(MAKEFILE).Release all
release-clean: $(MAKEFILE).Release FORCE
	@set MAKEFLAGS=$(MAKEFLAGS)
	$(MAKE) -f $(MAKEFILE).Release clean
release-distclean: $(MAKEFILE).Release FORCE
	@set MAKEFLAGS=$(MAKEFLAGS)
	$(MAKE) -f $(MAKEFILE).Release distclean
release-install: $(MAKEFILE).Release FORCE
	@set MAKEFLAGS=$(MAKEFLAGS)
	$(MAKE) -f $(MAKEFILE).Release install
release-uninstall: $(MAKEFILE).Release FORCE
	@set MAKEFLAGS=$(MAKEFLAGS)
	$(MAKE) -f $(MAKEFILE).Release uninstall
debug: $(MAKEFILE).Debug FORCE
	@set MAKEFLAGS=$(MAKEFLAGS)
	$(MAKE) -f $(MAKEFILE).Debug
debug-make_default: $(MAKEFILE).Debug FORCE
	@set MAKEFLAGS=$(MAKEFLAGS)
	$(MAKE) -f $(MAKEFILE).Debug 
debug-make_first: $(MAKEFILE).Debug FORCE
	@set MAKEFLAGS=$(MAKEFLAGS)
	$(MAKE) -f $(MAKEFILE).Debug first
debug-all: $(MAKEFILE).Debug FORCE
	@set MAKEFLAGS=$(MAKEFLAGS)
	$(MAKE) -f $(MAKEFILE).Debug all
debug-clean: $(MAKEFILE).Debug FORCE
	@set MAKEFLAGS=$(MAKEFLAGS)
	$(MAKE) -f $(MAKEFILE).Debug clean
debug-distclean: $(MAKEFILE).Debug FORCE
	@set MAKEFLAGS=$(MAKEFLAGS)
	$(MAKE) -f $(MAKEFILE).Debug distclean
debug-install: $(MAKEFILE).Debug FORCE
	@set MAKEFLAGS=$(MAKEFLAGS)
	$(MAKE) -f $(MAKEFILE).Debug install
debug-uninstall: $(MAKEFILE).Debug FORCE
	@set MAKEFLAGS=$(MAKEFLAGS)
	$(MAKE) -f $(MAKEFILE).Debug uninstall

Makefile: QtKinectRecorder.pro  ..\..\..\..\Python27\Lib\site-packages\PyQt4\mkspecs\default\qmake.conf ..\..\..\..\Python27\Lib\site-packages\PyQt4\mkspecs\qconfig.pri \
		..\..\..\..\Python27\Lib\site-packages\PyQt4\mkspecs\modules\qt_webkit_version.pri \
		..\..\..\..\Python27\Lib\site-packages\PyQt4\mkspecs\features\qt_functions.prf \
		..\..\..\..\Python27\Lib\site-packages\PyQt4\mkspecs\features\qt_config.prf \
		..\..\..\..\Python27\Lib\site-packages\PyQt4\mkspecs\win32-msvc2008\qmake.conf \
		..\..\..\..\Python27\Lib\site-packages\PyQt4\mkspecs\features\exclusive_builds.prf \
		..\..\..\..\Python27\Lib\site-packages\PyQt4\mkspecs\features\default_pre.prf \
		..\..\..\..\Python27\Lib\site-packages\PyQt4\mkspecs\features\win32\default_pre.prf \
		..\..\..\..\Python27\Lib\site-packages\PyQt4\mkspecs\features\release.prf \
		..\..\..\..\Python27\Lib\site-packages\PyQt4\mkspecs\features\debug_and_release.prf \
		..\..\..\..\Python27\Lib\site-packages\PyQt4\mkspecs\features\default_post.prf \
		..\..\..\..\Python27\Lib\site-packages\PyQt4\mkspecs\features\win32\default_post.prf \
		..\..\..\..\Python27\Lib\site-packages\PyQt4\mkspecs\features\win32\rtti.prf \
		..\..\..\..\Python27\Lib\site-packages\PyQt4\mkspecs\features\win32\exceptions.prf \
		..\..\..\..\Python27\Lib\site-packages\PyQt4\mkspecs\features\win32\stl.prf \
		..\..\..\..\Python27\Lib\site-packages\PyQt4\mkspecs\features\shared.prf \
		..\..\..\..\Python27\Lib\site-packages\PyQt4\mkspecs\features\win32\embed_manifest_exe.prf \
		..\..\..\..\Python27\Lib\site-packages\PyQt4\mkspecs\features\win32\embed_manifest_dll.prf \
		..\..\..\..\Python27\Lib\site-packages\PyQt4\mkspecs\features\warn_on.prf \
		..\..\..\..\Python27\Lib\site-packages\PyQt4\mkspecs\features\qt.prf \
		..\..\..\..\Python27\Lib\site-packages\PyQt4\mkspecs\features\win32\opengl.prf \
		..\..\..\..\Python27\Lib\site-packages\PyQt4\mkspecs\features\win32\thread.prf \
		..\..\..\..\Python27\Lib\site-packages\PyQt4\mkspecs\features\moc.prf \
		..\..\..\..\Python27\Lib\site-packages\PyQt4\mkspecs\features\win32\windows.prf \
		..\..\..\..\Python27\Lib\site-packages\PyQt4\mkspecs\features\resources.prf \
		..\..\..\..\Python27\Lib\site-packages\PyQt4\mkspecs\features\uic.prf \
		..\..\..\..\Python27\Lib\site-packages\PyQt4\mkspecs\features\yacc.prf \
		..\..\..\..\Python27\Lib\site-packages\PyQt4\mkspecs\features\lex.prf \
		..\..\..\..\Python27\Lib\site-packages\PyQt4\mkspecs\features\include_source_dir.prf
	$(QMAKE) -o Makefile QtKinectRecorder.pro
..\..\..\..\Python27\Lib\site-packages\PyQt4\mkspecs\qconfig.pri:
..\..\..\..\Python27\Lib\site-packages\PyQt4\mkspecs\modules\qt_webkit_version.pri:
..\..\..\..\Python27\Lib\site-packages\PyQt4\mkspecs\features\qt_functions.prf:
..\..\..\..\Python27\Lib\site-packages\PyQt4\mkspecs\features\qt_config.prf:
..\..\..\..\Python27\Lib\site-packages\PyQt4\mkspecs\win32-msvc2008\qmake.conf:
..\..\..\..\Python27\Lib\site-packages\PyQt4\mkspecs\features\exclusive_builds.prf:
..\..\..\..\Python27\Lib\site-packages\PyQt4\mkspecs\features\default_pre.prf:
..\..\..\..\Python27\Lib\site-packages\PyQt4\mkspecs\features\win32\default_pre.prf:
..\..\..\..\Python27\Lib\site-packages\PyQt4\mkspecs\features\release.prf:
..\..\..\..\Python27\Lib\site-packages\PyQt4\mkspecs\features\debug_and_release.prf:
..\..\..\..\Python27\Lib\site-packages\PyQt4\mkspecs\features\default_post.prf:
..\..\..\..\Python27\Lib\site-packages\PyQt4\mkspecs\features\win32\default_post.prf:
..\..\..\..\Python27\Lib\site-packages\PyQt4\mkspecs\features\win32\rtti.prf:
..\..\..\..\Python27\Lib\site-packages\PyQt4\mkspecs\features\win32\exceptions.prf:
..\..\..\..\Python27\Lib\site-packages\PyQt4\mkspecs\features\win32\stl.prf:
..\..\..\..\Python27\Lib\site-packages\PyQt4\mkspecs\features\shared.prf:
..\..\..\..\Python27\Lib\site-packages\PyQt4\mkspecs\features\win32\embed_manifest_exe.prf:
..\..\..\..\Python27\Lib\site-packages\PyQt4\mkspecs\features\win32\embed_manifest_dll.prf:
..\..\..\..\Python27\Lib\site-packages\PyQt4\mkspecs\features\warn_on.prf:
..\..\..\..\Python27\Lib\site-packages\PyQt4\mkspecs\features\qt.prf:
..\..\..\..\Python27\Lib\site-packages\PyQt4\mkspecs\features\win32\opengl.prf:
..\..\..\..\Python27\Lib\site-packages\PyQt4\mkspecs\features\win32\thread.prf:
..\..\..\..\Python27\Lib\site-packages\PyQt4\mkspecs\features\moc.prf:
..\..\..\..\Python27\Lib\site-packages\PyQt4\mkspecs\features\win32\windows.prf:
..\..\..\..\Python27\Lib\site-packages\PyQt4\mkspecs\features\resources.prf:
..\..\..\..\Python27\Lib\site-packages\PyQt4\mkspecs\features\uic.prf:
..\..\..\..\Python27\Lib\site-packages\PyQt4\mkspecs\features\yacc.prf:
..\..\..\..\Python27\Lib\site-packages\PyQt4\mkspecs\features\lex.prf:
..\..\..\..\Python27\Lib\site-packages\PyQt4\mkspecs\features\include_source_dir.prf:
qmake: qmake_all FORCE
	@$(QMAKE) -o Makefile QtKinectRecorder.pro

qmake_all: FORCE

make_default: release-make_default debug-make_default FORCE
make_first: release-make_first debug-make_first FORCE
all: release-all debug-all FORCE
clean: release-clean debug-clean FORCE
	-$(DEL_FILE) ".\QtKinectRecorder_V3.intermediate.manifest"
	-$(DEL_FILE) QtKinectRecorder_V3.exp
distclean: release-distclean debug-distclean FORCE
	-$(DEL_FILE) Makefile

check: first

release-mocclean: $(MAKEFILE).Release
	@set MAKEFLAGS=$(MAKEFLAGS)
	$(MAKE) -f $(MAKEFILE).Release mocclean
debug-mocclean: $(MAKEFILE).Debug
	@set MAKEFLAGS=$(MAKEFLAGS)
	$(MAKE) -f $(MAKEFILE).Debug mocclean
mocclean: release-mocclean debug-mocclean

release-mocables: $(MAKEFILE).Release
	@set MAKEFLAGS=$(MAKEFLAGS)
	$(MAKE) -f $(MAKEFILE).Release mocables
debug-mocables: $(MAKEFILE).Debug
	@set MAKEFLAGS=$(MAKEFLAGS)
	$(MAKE) -f $(MAKEFILE).Debug mocables
mocables: release-mocables debug-mocables
FORCE:

$(MAKEFILE).Release: Makefile
$(MAKEFILE).Debug: Makefile
