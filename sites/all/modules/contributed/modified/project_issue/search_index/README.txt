$Id: README.txt,v 1.1 2009/02/23 23:28:23 dww Exp $

This directory contains the project_issue_search_index module.  If
your site is running the core search module and regularly indexes all
nodes, you should *not* enable this module.  This module is only
useful for sites that:

1) do not maintain the full search index of all nodes
2) wish to use text-based filters on the advanced issue search default views

For example, this module is useful is if your site relies on some
other search index (for example, http://drupal.org/project/apachesolr)
and you've installed the http://drupal.org/project/coresearches patch
to disable indexing all nodes into the core search index.

