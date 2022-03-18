#
#       srecord - Manipulate EPROM load files
#       Copyright (C) 2008, 2010, 2012 Peter Miller
#
#       This program is free software; you can redistribute it and/or modify
#       it under the terms of the GNU General Public License as published by
#       the Free Software Foundation; either version 3 of the License, or
#       (at your option) any later version.
#
#       This program is distributed in the hope that it will be useful,
#       but WITHOUT ANY WARRANTY; without even the implied warranty of
#       MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#       GNU General Public License for more details.
#
#       You should have received a copy of the GNU General Public License
#       along with this program. If not, see
#       <http://www.gnu.org/licenses/>.
#

if [ "$TEST_SUBJECT" = "fill me in" -o "$TEST_SUBJECT" = "" ]
then
    echo '    You must set the TEST_SUBJECT environment variable' 1>&2
    echo '    at the top of your test script to something' 1>&2
    echo '    descriptive.' 1>&2
    exit 2
fi

#
# Remember where we came from, so that we can refer back to it as necessary.
#
here=$(pwd)
test $? -eq 0 || exit 2
arch=${1-.}
bindir=$here/$arch/bin

#
# We are going to create a temporary directory for running tests within.
# This makes it easy to clean up after tests (just rm -r) and it means
# tests can be run even if the development directory is read only (e.g.
# for reviewers).
#
testdir=/tmp/srecord-$$

#
# The tear_down function is used by all the ways of leaving a test
# script, so that they all clean up in exactly the same way.
#
tear_down()
{
    set +x
    cd $here
    rm -rf $testdir
}

#
# The pass function (command) is used to declare a test to have passed,
# and exit.  The exit code of 0 is dictated by Aegis, so Aegis can know
# the result of running the test.
#
# Note that we don't say what was being tested, because only failed
# tests are interesting, especially when your project get to the point
# of having hundreds of tests.
#
pass()
{
    tear_down
    echo PASSED
    exit 0
}

#
# The fail function (command) is used to declare a test to have failed,
# and exit.  The exit code of 1 is dictated by Aegis, so Aegis can know
# the result of running the test.
#
fail()
{
    tear_down
    echo "FAILED test of $TEST_SUBJECT"
    exit 1
}

#
# The no_result function (command) is used to declare a test to have
# failed in an unexpected way, and exit.  This is used for any case
# where the "scaffolding" of a test does no succeed, effectively making
# the correctedness of the functionality being tested indeterminate.
# The exit code of 2 is dictated by Aegis, so Aegis can know the result
# of running the test.
#
no_result()
{
    tear_down
    echo "NO RESULT for test of $TEST_SUBJECT"
    exit 1
}

#
# Create our testing directory and cd into it.
#
mkdir $testdir
test $? -eq 0 || exit 2
cd $testdir
test $? -eq 0 || no_result

# On cygwin, we need to have diff ignore the CR in CRLF sequences
# otherwise we see diff output which looks to be identical but which
# diff things differs.  And it does, by a carriage return.
if diff --strip-trailing-cr /dev/null /dev/null > /dev/null 2>&1
then
    diffpath=`which diff`
    diff() {
        $diffpath --strip-trailing-cr "$@"
    }
fi
# vim: set ts=8 sw=4 et :
