/* $Id: README.txt,v 1.2 2008/08/20 18:09:24 philipnet Exp $ */


Contents of this File
=====================
 * Introduction
 * Maintainers
 * Installation and configuration
 * Testing and troubleshooting
 * Extended use


Introduction
============

The Listhandler module allows you to connect mailing lists to forums and vice
versa.

It works in conjunction with the Mailhandler module. Mailhandler receives an
email and then asks Listhandler if the received email is part of a list. If
the email is from a mailing list associated with a forum on your site then
Listhandler adds the received email to the forum.

Additionally Listhandler will take every post to a forum and, as long as that
forum is associated with a mailing list, post the message to it. Checks are
made to ensure that a continuous cycle of emails and posts aren't generated.

Mailhandler and Listhandler can deal with attachments in emails and in forum
posts. See below for configuration details.

The Listhandler administration page allows you to set an 'anonymous user'
email address and configure the mailing list attributes.



Maintainers
===========
Current Maintainer:
Philip Ludlam - Drupal 5 and 6 - (http://www.philipnet.com/about)

Previous maintainer:
Gerhard Killesreiter - Drupal 4.6 and 4.7 - (http://drupal.org/services#gerhard)



Installation and configuration
==============================

1) Listhandler works best with an already working mailing list. Please set one
   up and ensure that it is working. Do note that setting one up is outside
   the scope of this document.

2) Listhandler requires Mailhandler to be installed and configured first.
   Both modules can be downloaded from the drupal.org website from
   http://drupal.org/project/mailhandler and
   http://drupal.org/project/listhandler
   If you have not, please download them and extract the contents of
   the archives into your modules directory.

3) Within your Drupal website, enable the Mailhandler and Listhandler modules
   in Administer -> Site building -> Modules.
   N.B. Please enable Mailhandler first, otherwise Listhandler will fail to
   enable cleanly.
   In the same section, please ensure that the Forum and Comment modules are
   also enabled.

4) Create a forum that you wish to be the repository for your list's messages.
   Administer -> Content management -> Forums -> Add forum
   Enter a name, description and set the Parent (leave as "<root>" if unsure)
   and click on "Submit".
   In the listing, click on the forum name you just created. Make a note of the
   URL you are sent to. The number at the end of the URL is the 'TID' (Topic
   IDentification). You will need this later.

5) Configure a mailbox
   Within your Drupal website go to Administer -> Content management ->
   Mailhandler
   Click on "Add mailbox"
   In the form presented, set the values as indicated:
     E-mail address:
       Email address subscribed to mailing list. This should be the address
       associated with the mailbox configured below,
       e.g. my-subscribed-mailbox@example.com

     Second email address:
       Email address of the mailing list, e.g.
       my-mailing-list-address@example.com

     Folder:
       This is the folder for my-subscribed-mailbox@example.com
       Set this to "INBOX" (without the quotes) unless another value is
       more suitable for your environment.

     POP3 or IMAP Mailbox:
       This is the protocol used to connect to
       my-subscribed-mailbox@example.com
       Pick which ever suits your environment. Choose POP3 if unsure.

     Mailbox domain:
       This is the full name of the email server containing
       my-subscribed-mailbox@example.com
       Talk to your service provider if you are unsure.

     Mailbox port:
       Set this to 110 for POP3 or 143 if using IMAP.
       Other port numbers can be used depending on your mailserver
       configuration.

     Mailbox username:
       This is the username for the my-subscribed-mailbox@example.com mailbox


     Mailbox password:
       This is the password for the my-subscribed-mailbox@example.com mailbox
       Please note that this is stored as plain text. Please don't use something
       sensitive.

     Extra commands:
       Leave blank unless you run into problems.
       A common value is "/notls", without the quotes.

     Mime preference:
       Set this to: HTML

     Security:
       Set this to: Disabled

     Send error replies:
       Set this to: Disabled

     From header:
       Set this to a common header always present in emails sent by your mailing
       list software.

     Default commands:
       Set this to the following three lines:
         tid: 123
         status: 1

       Where 123 is the numeric taxonomy id of the forum you created in step 4.
       The line "status: 1" sets the created node/comment as published.

     Signature separator:
       Set this to "-- ", without the quotes.
       (Yes, there is a space at the end)

     Delete messages after they are processed?
       Check this option

     Cron processing:
       Set this to: Enabled

     Input format:
       (Click on "Import format:" to expand the section)
       Leave the default as "Filtered HTML", unless you have another preference


6) Repeat steps 4 to 5 for each forum/mailing list you wish to configure.


7) Configure permissions
   Within your Drupal website, allow authenticated users to post to forums:
   Administer -> Roles
   Against "authenticated user", click on "edit permissions"
   Under "Forum module", check "create forum topics" and "edit own forum
   topics"
   Click on "Save permissions" (at the bottom of the page)

8) Configure Listhandler
   Within your Drupal website go to Administer -> Content management ->
   Listhandler

   In the form presented, set the values as indicated:
   Admin address:
     This is the email address used by Drupal to send forum posts made by
     anonymous users to mailing lists. Set this to "anonymous@example.com"
     or similar and subscribe it to your mailing lists. Set this user to not
     receive posts or otherwise it might be unsubscribed.

   Strip title:
     Vales to be stripped from email subject lines, before posting them as forum
     topics/comments. Make sure to include any mailing list prefixes, like "[my-
     list]". Separate multiple values with commas.

   Account status:
     When mailing list users post, Listhandler creates an account for them on
     your Drupal site. You can configure these account to be Blocked or Allowed
     when created.
     Set as "Allowed" if you want those people to be able to log on to your
     Drupal site. Set as "Blocked" if you don't.

   Attachments as link:
     Send attachments as links instead of MIME attachments. It affects only
     mails generated by forum posts.
     N.B. You will need to have the Upload module enabled and allow users to attach files to forum posts

   Mailing list and Prefix
     For each mailing list configured, you can set the [prefix] title that the
     mailing list software appends to the subject line of each email. Set it
     here to help Listhandler pick the right forum that posts should go to.
     It is also recommended that you set "Strip title" to include mailing list
     prefix as well.


9) For each configured mailbox, subscribe it to the appropriate mailing list.


Testing and troubleshooting
===========================

All being well, you won't have any problems. But if you do, here's some helpful
troubleshooting techniques:

1) Mail is only collected by Mailhandler whenever cron.php is run.
   You can initiate a Cron run by going to http://www.example.com/cron.php
   If you haven't setup a Cron schedule on your Drupal site, go to
   http://drupal.org/getting-started/5/install/cron for help.


2) Ensure the mailing list is working as expected. Ensure that you are
   subscribed and that posts you send to it, get sent back to you.

3) Use a webmail client, Thunderbird, Outlook or other email client to examine
   the mailbox of "my-subscribed-mailbox@example.com".
   Is it receiving posts from the mailing list?
   Is there a subscription confirmation pending?

4) Examine the mailing list configuration to see if there are bounce emails from
   "my-subscribed-mailbox@example.com".

5) Look at logs in Drupal: Administer -> Logs -> Recent log entries
   to see if Mailhandler and/or Listhandler have any problems.

6) Examine your mailserver logs to ensure that emails are being received and
   email is being collected from the mailbox successfully.

7) Confirm that you have
     tid: <n>
   in the Mailhandler configuration.
   Without it Listhandler will not be asked to handle incoming emails.


Extended use
============

1) Add links for uploaded files:
   a) Enable the Upload module, download and enable the Comment Upload module
   b) Within your Drupal website go to Administer -> Content management ->
      Mailhandler
   c) Tick Attachments as link
   d) Click on Save configuration
   Please note that Listhandler doesn't work with modules that modify the path
   that uploads are saved to. There is no plan to add this functionality.

2) Allow more HTML tags, but still protect your site:
   a) Install the SafeHTML and UnWrap modules and enable them.
   b) Create an Input filter called "Mailinglist Filter" through Administer ->
      Site configuration -> Input formats.
   c) Apply that to Mailhandler posts through Administer -> Content Management
      -> Against <Mailbox name>, click "Edit"
   d) Scroll to the bottom
   e) Click on "Input filter" and select "Mailinglist Filter".
   f) Click on "Update mailbox"

3) In step #2 of the main instructions you could set up a folder for each
   mailing list instead of a mailbox for each one. However documenting such a
   configuration is beyond the scope of this document and you are responsible
   for testing it to ensure it works.

4) Allow users to post images that appear inline.
   See http://www.venturacottage.com/adding-content-site-email for how to enable
   Mailhandler to cope with attached images.
