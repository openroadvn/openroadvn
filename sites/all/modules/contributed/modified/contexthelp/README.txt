/* $Id: README.txt,v 1.2.2.17 2010/01/26 14:32:31 darrenferguson Exp $ */

The contexthelp module allows you to add contextual help to any Drupal site and provides 
an Import and Export mechanizm to make the moving of help between sites as easy and as painless
as possible. This module provides two pieces of help. 

1. Actual Context Help - will display help for the current page you are on
2. All Context Help - will retrieve all help for your site and display it in accordian fashion
3. FAQs for the site - allows the user to display frequently asked questions for the site

INSTALLATION
------------

Please check INSTALL.txt for instructions on module Installation

CONFIGURATION
-------------

In order to utilize the context help module you should go to the Admin / Site Configuration / 
Context Help page in order to let the module know which node type it should use as the 
context help node type and which node type it should use as the FAQs node type. These are the 
node types that were created from the CONTEXTHELPCCK and FAQCCK files that you imported above
Once this is done the module is ready to be utilized.


DEPENDENCIES
------------
Module is written purely for Drupal 6.x core.

Module depends on CCK and Views modules for functionality


CREDITS
-------
Developed and maintained by Darren Ferguson <http://www.openbandlabs.com/crew/darrenferguson>
Development sponsored by OpenBand, a subsidiary of M.C.Dean, Inc. <http://www.openbandlabs.com/>

