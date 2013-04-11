This module allows teams to track outstanding items which need
resolution. It provides e-mail notifications to members about updates
to items.  Similar to many issue tracking systems.

For installation instructions, see INSTALL.txt.

For instructions when upgrading to newer versions, see UPGRADE.txt.

CAVEATS:

The filter which automatically turns references to project issues into
links conflicts with filter caching in the following ways:

1. If either a) anonymous users don't have access rights to view issues,
   or b) a node access module is enabled, users may be able to see 
   cached titles of project issues they should not have access to - 
   in case of such conflict it is advised not to enable the filter.

2. Upon issue status edit, the strikethrough and link titles of the 
   reference links will be outdated for the period of time that
   filter output is cached.

Send feature requests and bug reports to the issue tracking system for
the project module: http://drupal.org/node/add/project_issue/project_issue.
A TODO list can be found at http://groups.drupal.org/node/5489

The project family of modules is currently being co-maintained by:
- Derek Wright (http://drupal.org/user/46549) a.k.a. "dww"
- Chad Phillips (http://drupal.org/user/22079) a.k.a. "hunmonk"

$Id: README.txt,v 1.4 2008/01/01 03:54:39 thehunmonkgroup Exp $
$Name: DRUPAL-6--1-0-ALPHA4 $
