This directory contains all of the files related to the
project_release.module, which provides a "Project release" node type
so that projects can have releases. Each release has a version string,
a title, a body of text, and a file associated with it. Once this
module is enabled and releases are created, the pages to browse for
projects on your site will have download links, as will the project
nodes themselves. If the project_issue.module is installed, issues
will have a version string for any projects that have releases.

Project release nodes can be integrated with a CVS repository by
enabling the cvslog module (http://drupal.org/project/cvslog).  This
turns the form for adding a new release into a multi-page wizard.
Users first select a valid CVS tag or branch, which then
automatically fills in the version information based on site-specific
code that understands your local tag and branch naming conventions.
This functionality is only available in the DRUPAL-4-7--2 branch of
the contributions/module/cvslog directory.

If you enable this CVS integration, you can setup a script that
periodically queries the database for new release nodes and creates
release packages based on the specified CVS tags or branches. An
example script (the one in use on drupal.org to package contributions)
is provided in the package-release-nodes.php script.

Send feature requests and bug reports to the issue tracking system for
the project module: http://drupal.org/node/add/project_issue/project,
and specify "Releases" as the component.

The project_release.module was originally written by Derek Wright
"dww" (http://drupal.org/user/46549) as part of the new system for
releasing Drupal contributions. For background, see:

http://drupal.org/node/77562
http://drupal.org/node/58066
http://drupal.org/node/75053
http://drupal.org/node/86694

$Id: README.txt,v 1.4 2007/08/22 16:30:52 thehunmonkgroup Exp $
$Name: DRUPAL-6--1-0-ALPHA4 $
