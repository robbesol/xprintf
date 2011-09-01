# Hey Emacs, this is a -*- makefile -*-
#
# To rebuild project do "make clean" then "make all".
#

DOXYGEN_INPUTS += . ../include
DOXYGEN_INPUTS += . ../src

DOXYGEN_IMAGE_PATH += .

DOXYGEN_EXAMPLES_DIR := examples
INCLUDEDIRS += $(DOXYGEN_INPUTS)
# ../../libdebug/Nodebug/include ../../libdebug/include

include doxygen.mk
