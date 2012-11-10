#  Copyright (c) 2012, Mike Tegtmeyer
#  All rights reserved.
#  Redistribution and use in source and binary forms, with or without
#  modification, are permitted provided that the following conditions are met:
# 
#      * Redistributions of source code must retain the above copyright
#        notice, this list of conditions and the following disclaimer.
#      * Redistributions in binary form must reproduce the above copyright
#        notice, this list of conditions and the following disclaimer in the
#        documentation and/or other materials provided with the distribution.
#      * Neither the name of the author nor the names of its contributors may
#        be used to endorse or promote products derived from this software
#        without specific prior written permission.
# 
#  THIS SOFTWARE IS PROVIDED BY THE AUTHOR AND CONTRIBUTORS ``AS IS'' AND ANY
#  EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
#  WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
#  DISCLAIMED. IN NO EVENT SHALL THE REGENTS AND CONTRIBUTORS BE LIABLE FOR ANY
#  DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
#  (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
#  LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
#  ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
#  (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
#  THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

AC_PREREQ(2.59)
AC_INIT(lemma, 0.0.1)
AC_CONFIG_SRCDIR([Makefile.am])
AM_CONFIG_HEADER(config.h)
AC_CONFIG_AUX_DIR(config)
AC_CANONICAL_SYSTEM
AM_INIT_AUTOMAKE([foreign tar-pax])
AC_CONFIG_MACRO_DIR([m4])

# Checks for programs.

AC_PROG_CC
AC_PROG_CXX
AC_PROG_INSTALL
AC_PROG_LIBTOOL

# We need boost
BOOST_REQUIRE([1.46])
BOOST_VARIANT
BOOST_TEST

# Check for single testsuite                                                    
AC_ARG_WITH([single-testsuite],
[AS_HELP_STRING([--with-single-testsuite],
        [single testsuite with XML output @<:@default=no@:>@])],
[case "${withval}" in
  yes) ;;
  no) ;;
  *) AC_MSG_ERROR([bad value ${withval} for --with-single-testsuite]) ;;
esac],[with_single_testsuite=no])
AM_CONDITIONAL([SINGLE_TESTSUITE], [test "x$with_single_testsuite" == xyes])

AC_CONFIG_FILES( \
		 lemma.pc \
		 qsat/Makefile \
		 qsat/tests/Makefile \
		 attribute_map/Makefile \
		 tests/Makefile \
		 Makefile)

AC_OUTPUT
