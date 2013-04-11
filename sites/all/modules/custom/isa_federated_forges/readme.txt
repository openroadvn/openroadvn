==================
=  INSTALLATION  =
==================

REQUIREMENTS
------------
This module requires both a PHP module and some basic Drupal modules

PHP module:
-> OpenSSH

Drupal modules:
-> CCK
-> Content Copy
-> Link
-> Node Reference
-> ImageField
-> Text
-> Option Widgets
-> FileField



INSTALLATION
------------
Once all dependencies are set, just install the module. It will add two content types (Federated Forge & Federated Project) and two permissions.




CONFIGURATION
-------------
After installation, there's not much configuration to do.

Mail configuration:
Go to <yoursite>/admin/settings/forges and fill in the form (Subject is the subject of the mail, body the actual mail)


Permissions:
Set all roles that need to be able to alter the mail settings to "edit forges mail"
Set all roles that need to receive a mail after each cron run to "receive forges mail"


Content types:
You can also add some CCK fields to the content types if you want. Just make sure the ones that've been created, remain there.