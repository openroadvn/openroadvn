// $Id: betterselect.js,v 1.2.2.1 2009/02/27 11:22:00 marktheunissen Exp $

Drupal.behaviors.initBetterSelect = function(context) {
    
    }


$(document).ready(function(){

    $('.treeview-default').treeview({
        collapsed: true,
        unique:false,
        animated: "medium"
    })

    
    
    // open branches containing checked boxes
    $('.form-checkbox.betterselect').each(
        function() {
            if(this.checked)
            {
                currentNode = this;
                while(currentNode != false) {
                    if($(currentNode).hasClass('treeview-default')) currentNode =  false;
                    else {
                        $(currentNode).parent().parent().parent().show();
                        $(currentNode).parent().parent().parent().siblings('div.hitarea').removeClass('expandable-hitarea');
                        $(currentNode).parent().parent().parent().siblings('div.hitarea').addClass('collapsable-hitarea');
                        currentNode = $(currentNode).parent();
                    }
                }
            }

        }
        )
}
)


