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

master_suite=$(top_srcdir)/tests/master_suite.cc

check_PROGRAMS=\
	basic_channel_test \
	basic_subchannel_test \
	channel_base_test

basic_channel_test_SOURCES=$(master_suite) \
	basic_channel_test.cc test_types.h
basic_channel_test_LDFLAGS=$(BOOST_UNIT_TEST_FRAMEWORK_LDFLAGS)
basic_channel_test_LDADD=$(BOOST_UNIT_TEST_FRAMEWORK_LIBS)

basic_subchannel_test_SOURCES=$(master_suite) \
	basic_subchannel_test.cc test_types.h
basic_subchannel_test_LDFLAGS=$(BOOST_UNIT_TEST_FRAMEWORK_LDFLAGS)
basic_subchannel_test_LDADD=$(BOOST_UNIT_TEST_FRAMEWORK_LIBS)

channel_base_test_SOURCES=$(master_suite) \
	channel_base_test.cc test_types.h
channel_base_test_LDFLAGS=$(BOOST_UNIT_TEST_FRAMEWORK_LDFLAGS)
channel_base_test_LDADD=$(BOOST_UNIT_TEST_FRAMEWORK_LIBS)

AM_CPPFLAGS=-pedantic -ansi -Wall -Werror -Wno-unused-local-typedefs -I$(top_srcdir)/qsat $(BOOST_CPPFLAGS)

TESTS=\
	basic_channel_test \
	basic_subchannel_test \
	channel_base_test

