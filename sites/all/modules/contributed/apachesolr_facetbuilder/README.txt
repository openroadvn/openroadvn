
-------------------------------------------------------------------------------
Apache Solr Facet Builder for Drupal 6.x
  by Ronan Dowling, Gorton Studios - ronan (at) gortonstudios (dot) com
-------------------------------------------------------------------------------

DESCRIPTION:
This module allows administrators to dynamically build Apache Solr search facets
without writing any code.

Search facets are a way to let your users filter search results in interesting
ways. Some facets included with Apache Solr are filtering by content author or
by content type. Facet Builder allows you to create your own facets, describing
each filter option using the Views UI.

INSTALLATION:
Put the module in your drupal modules directory and enable it in
  admin/build/modules. 

REQUIREMENTS:
Views: http://drupal.org/project/views
Apache Solr: http://drupal.org/project/apachesolr

CREATING A NEW FACET:
Here are the steps to create a new facet filter for Apache Solr. As an example,
imagine we wanted to filter our site by it's various sections: 'About Us',
'Articles', 'News' and 'Events', but these sections are not accurately described
by any of the available Apache Solr facets.

1. Install and enable this module, Views and Views UI (You will also need to
enable and set up Apache Solr).

2. Create a new View to represent your facet type (eg: 'Filter by Section').

3. Set the name under Basic Settings of of the Default display to the name of
your facet type. In our example the name would be 'Section'.

4. Create one or more displays in the view of type 'facet'. Each of these facets
will represent a different option in your facet type. In our example we might
create 'About Us', 'Articles', 'News', 'Events'.

5. Override the filters in each facet display and use them to specify what
content should appear in each facet. For our example lets say 'About Us'
contains all content of type Page with the taxonomy term 'About', 'Articles'
contains any node of type 'Blog Post' or 'Artictle', 'News' is any Article with
the 'Article' taxonomy term and 'Events' is any node whose 'Event Date' field is
not empty. You can check if each facet contains the appropriate content by using
the Preview function of the Views UI. You may also add arguments to your facets
as long as they have some sort of default argument handling.

Your new facet filter will be available to Apache Solr and will act as any other
available facet. You will need to enable it in the Apache Solr settings as you
would any other new filter:

1. Go to the Enabled filters tab of the Apache Solr module settings
(admin/settings/apachesolr/enabled-filters) and enable your new facet (in our
example it will be called 'Apache Solr Facet Builder: Filter by Section
(view_name)'

2. Enable the facet block for your new facet at admin/build/blocks

3. Reindex your content.

This module was developed by 
  Gorton Studios 
and sponsored by 
  The Southern Poverty Law Center
