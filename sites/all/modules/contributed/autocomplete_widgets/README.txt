;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; Autocomplete Widgets for CCK Text and Number fields
;; $Id: README.txt,v 1.1.2.2 2009/08/16 13:11:41 markuspetrux Exp $
;;
;; Module Author: markus_petrux (http://drupal.org/user/39593)
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

OVERVIEW
========

This module adds 2 autocomplete widgets for CCK fields.

- Autocomplete for allowed values list: This widget can be used for Text and
  Number fields and it takes candidate values from the defined list of Allowed
  values of the fields.

- Autocomplete for existing field data: This widget can be used for Text only
  and it takes candidate values from existing values in the database for that
  field.

Both widgets allow you to choose the size of the text element and the method
used to match values between 'Starts with' and 'Contains'.

When the Internationalization module [1] is enabled, the 'Autocomplete for
existing field data' widget also provides an option to filter values by the
language assigned to their corresponding nodes. This option allows you to
provide a different set of allowed values per language.

[1] http://drupal.org/project/i18n


REQUIREMENTS
============

- CCK (http://drupal.org/project/cck)
- CCK Text and/or Number modules provided by CCK itself.


INSTALLATION
============

- Copy all contents of this package to your modules directory preserving
  subdirectory structure.

- Goto Administer > Site building > Modules to install this module.

- Create or edit content types and start using the widgets for your Text and/or
  Number fields. :)
