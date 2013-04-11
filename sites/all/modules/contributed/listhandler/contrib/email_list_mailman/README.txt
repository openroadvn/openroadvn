This module allows you to connect listhandler lists with Mailman. Mail is sent from Drupal to Mailman,
which then broadcasts it to the email list. Users then reply via email list or on web forum and their
email colleagues get the reply via email. All this is archived in the corresponding Drupal forum.

REQUIREMENTS
---------------
In addition to Mailman 2.1.6+, you must have installed and configured THE MYSQL ADAPTER FROM http://www.orenet.co.uk/opensource/MailmanMysql/. You should configure this application in 'wide' mode (i.e. many tables, not a single flat one).

INSTALL
---------------
- Edit your settings.php and add a new $db_url called 'mailman'. Your existing $db_url should be named 'default'. See the docs in settings.php for configuring multiple databases in Drupal.
- Install this module as usual
- Install and configure listhandler/mailhandler. You may setup as many lists as you want.
- Visit admin/settings/email_list_mailman and setup the mapping between listhandler lists and Mailman list name
- Visit the Mailman web UI and create a list named as specified above.
- Subscribe the email address for your Mailhandler mailbox to the new Mailman mailing list. Use the usual Mailman admin web apges for this.

LIMITATIONS
---------------
- In order to setup a new list, you must do the usual mailhandler/listhandler stuff and also create a new list in Mailman using its web UI or the 'newlist' python script
