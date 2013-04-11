// $Id: README.txt

The taxonomy system will be used to highlight contents. 
The module isa_highlight create a new taxonomy "highlight". In this taxonomy, one term represents a community. 
Theses terms are automatically created with the NAT module. So to highlight a content in a community A this content must be 
associated with the term that represents the community A. 
The module isa_highlight allows to add a link "highlight this content" under the contents. This links displays a form with the community of the current user. 
To enhance the ergonomy, the module popups can be used to display this form in a popup instead of a new page. 
Isa_highlight handles his own mysql table "isa_highlight" to have more information about the highlights : who has highlighted a content, when, ... 

Recommended Modules
-------------------
- Popups

Configuration
-------------
- install the modules NAT, isa_highlight (and popups). 
- Set the configuration of the NAT module : associate the vocabulary "highlight" to the content type "community" and make a synchronization to create all the 
terms associated to the existing communities. 
- Set the paramaters of the vocabulary "highlight" : select the content types you can tag with this vocabulary. 

