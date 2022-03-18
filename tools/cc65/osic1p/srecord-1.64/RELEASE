Release(SRecord)                                                Release(SRecord)

NAME
       How to release SRecord

ORIGIN
       Peter Miller's release instructions

COMPLETE CURRENT BRANCH DEVELOPMENT
       When making a release, I create the next branch.
       For example if the current branch is 1.62, then I would aenbr 1.63

       develop end the current branch
            aede -p srecord.1 -c 62

       note it is awaiting integration
            aefinish -p srecord.1 -c 62

       then re-arrange the shortcut

            aegis -remove-project-alias srecord.cur
            segis -new-projet-alias srecord-1.63 srecord.cur
            ae_p srecord.cur

UPDATING THE CHANGE LOG
       tkenc to create change 10.
       aedb 10
       aecd 10
       aenf etc/new.1.63.so

       This will have a useful content example.
       But we are using post-ordered releases (e. 1.62.D099 < 1.63) so we go
       back and clean up the release note for the previous branch

            aecp etc/new.62.so

       There is a command in the file, which is useful for seeding the initial
       change log entries.
       Edit to taste.
       1. Tridge suggests always acknowledge contributors, and including their
          email address.
       2. Lately I have written the not-brief description for change sets so
          that they read well as changelog entries
          ("There is a new srec_cat -example option, that may be used to ...")


       now build and check that that
                less archive/change_log.txt
       reads well.  the same information is available in the refence manual,

                evince arhive/srecord-*.pdf

UPDATE SOURCEFORGE AND WEBSITE
       The "archive" directory is actually the new contents of the web site.
       Later you use the rsync command to send it there.

       finish development and integrate
           aefinish && aefinish && cd

       WE actually release 1.63.D001.

       My /home partition has a user called archives that owns all my projects

           cd /home/arhives/srecord

       now we update the aegis.repo on sf.net

           ./rsync-sourceorge
              (the name could be changed to be more informative, if you like)

           ./rsync-web-site

       This does what it says, but also uses rsync to the sf.net download
       facility.

UPDATING LAUNCHPAD
       Thirdly it invokes ppa.sh to upload change sets to launchpad-ppa you may
       need to customize these scripts for your SF or LP account.

       A note about ppa.sh

       I put ~pm~ into the version number, so that the ppa source of the
       package is clear, in the "dpkg -l" output.

       Lastly I have a /home/archives/ubuntu.release.names file that contains
       the constantly changing list of supported PPA release names.  Shared
       across a whole bunch of projects.


       If you ever want to check the debian packaging:

          cook debian-package

       or

          cook integration-build-targets if you want to see what the archive/
          directory (new web site) looks like.

USEFUL TOOLS
       You can get tardy from my LP PPA
           https://launchpad.net/~pmiller-opensource/+archive/ppa

       In my junkyard project/package is a mountain of crap
       Incluing
           copfix $(slecf)
                for updating the copyright notices in source files

           fix-white-space $(aelcf)
                for getting rid of trailing white space

           fix-vim-mode-lines $(aelcf)
                the last line of the source files set the buffer's mode.

       The bleeding edge Aegis package repo is at
           https://launchpad.net/~pmiller-opensource/+archive/aegis
           think of it as a rolling release candidate.

COPYRIGHT
        This file is Copyright (C) 2014 Peter Miller & Scott Finneran

        The SRecord package is distributed in the hope that it will be useful,
        but WITHOUT ANY WARRANTY; without even the implied warranty of
        MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
        General Public License for more details.

        It should be in the LICENSE file included with this distribution.
