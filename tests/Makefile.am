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

noinst_HEADERS=


qsat_dir=$(top_srcdir)/qsat
qsat_tests=\
	$(qsat_dir)/tests/test_types.h \
	$(qsat_dir)/tests/basic_channel_test.cc \
	$(qsat_dir)/tests/basic_subchannel_test.cc \
	$(qsat_dir)/tests/channel_base_test.cc


check_PROGRAMS= \
	lemma_test


lemma_test_SOURCES=\
	master_suite.cc \
	$(qsat_tests)

lemma_test_LDFLAGS=$(BOOST_UNIT_TEST_FRAMEWORK_LDFLAGS)
lemma_test_LDADD=$(BOOST_UNIT_TEST_FRAMEWORK_LIBS)

AM_CPPFLAGS=-pedantic -ansi -Wall -Werror \
	-I$(top_srcdir) \
	-I$(qsat_dir) \
	$(BOOST_CPPFLAGS)

TESTS= \
	lemma_test_wrapper.sh

EXTRA_DIST= \
	lemma_test_wrapper.sh

CLEANFILES= \
	lemma_test.xml

DISTCLEANFILES= \
	lemma_test.xml
