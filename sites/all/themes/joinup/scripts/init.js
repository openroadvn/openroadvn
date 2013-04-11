/*  $Name: ISA - Integrated Collaborative Platform - jQuery Initialization $
	$File: init.js $
	$Date: 2011-02-03 $
	$Modify date: 2011-08-05 $
	$Version: 006 $
 */

// Initialize the slideshow when the DOM is ready
$(document).ready(        
    function (){
        if (document.getElementById('homebox') != null) {
            addEvent(window, "load", resizeDiv);
            addEvent(window, "resize", resizeDiv);
        }
     
        // The Cycle Plugin is a slideshow that supports many different types of transition effects for the "Lasted News" displayed in the homepage.
        sColspanContentClass = 'colspan-6 first last';
        sColspanCycleClass = 'colspan-5 first last';
        $('#block-views-News_view-block_1').find('.view-content')
        .addClass(sColspanContentClass)
        .before('<div class="view-cycle ' + sColspanCycleClass + '">')
        .cycle(
        {
            fx:			'scrollUp'
            ,	
            speed:		100
            ,	
            timeout:	5000
            ,	
            pager:		'.view-cycle'
            ,	
            pause:		1
        }
        );
			
        aTitle = Array();
        aCreated = Array();
        $('#block-views-News_view-block_1').find('.fields').each(
            function (index)
            {
                aCreated[index] = '<span class="cycle-field-created">' + $(this).children('.field-created').html() + '</span>';
                aTitle[index] = '<span class="cycle-field-title">' + $(this).children('.field-title').find('a').html() + '</span>';
                $('.view-cycle').children('a').eq(index).html(aCreated[index] + aTitle[index]);
            }
            );
		
        // Show/hide the content of the filter region (fieldset) on text legend click.
        $('#filter').find('.collapse-processed').children('a').click( function () {
            $('#filter-region').toggleClass('collapsed');
            return false;
        } );
		
        // Display/hide the Quick Actions dropdown menu with a sliding motion.
        $('div.buttons').find('a.action.propose-your').click( function () {
            $('div.quick-actions').children('ul.links').slideToggle();
            return false;
        });



        // Show/Hide the requiered/depends related projects
        $('#related-projects-select').change(
            function ()
            {
                var value = $(this).val();
                var theDiv = $('.' + value);
                //				alert(theDiv);
                //				alert(value);
                //				$(theDiv).toggle(
                //					function()
                //					{
                //						$(this).removeClass('hidden');
                //					}
                //				),
                //				function ()
                //				{
                //					$(this).addClass('hidden');
                //				}
                $('.related-projects-required').addClass('accessibility-info');
                $('.related-projects-depends').addClass('accessibility-info');
                theDiv.removeClass('accessibility-info');
            }
            );
		
        // Expand/Collapse text using the "Read more" or "Hide text" button.
        aViews = Array(
            Array('view-project-node', 66)
            ,	Array('view-homepage', 120)
            );
        function fnToggleReadMore(sActionButton, sToogleClass, sActionClass)
        {
            if (sToogleClass == 'view-footer') $(sActionButton).parents('.' + sToogleClass).prev().toggleClass('expanded');
            else $(sActionButton).parents('.' + sToogleClass).next().toggleClass('expanded');
            $(sActionButton).parents('.' + sToogleClass).hide();
            $(sActionButton).parents('.view').children('.' + sActionClass).show();
        }
        $('.read-more').click( function () {
            fnToggleReadMore(this, 'view-footer', 'view-header');
            return false;
        } );
        $('.reduce').click( function () {
            fnToggleReadMore(this, 'view-header', 'view-footer');
            return false;
        } );
        for (key=0; aViews.lenght; key++)
        {
            $('.' + aViews[key][0]).each( function (index) {
                var iContentHeight = $(this).height();
                if (iContentHeight < aViews[key][1]) $(this).find('.view-footer').hide();
            } );
        }
    
        $(window).resize(function() {
            if(iBeginShare!=undefined)
                iBeginShare.hide();
        });
        
    }
  
  

  
  
    );
  
function advanced_search(facet) {
    var hrefval  = $('#adsearch').attr('href').valueOf() + "/";
    var val = $('#edit-keys').val();
    if (val == undefined) {
        val = $('#edit-text').val();
    } 
    if (facet != '0'){
        var facet = '?filters=sm_facetbuilder_facet_node_type%3A"facet_node_type%3A'+facet+'"&retain-filters=1';
        $('#adsearch').attr("href", hrefval + val + facet);
    }else{
        $('#adsearch').attr("href", hrefval + val);
    }
};

function resizeDiv(){
    var doc_width = (document.documentElement.clientWidth);

    document.getElementById('homebox').style.width = (doc_width * 0.8 - 200)+'px';
    $('div.field-comments-comment').width((doc_width * 0.216) + 'px');
    $('div.views-field-subject').width((doc_width * 0.216) + 'px');
    $('div.views-field-nid-2').width((doc_width * 0.107) + 'px');
    $('div.views-field-created').width((doc_width * 0.107) + 'px');
    $('div.views-field-type').width((doc_width * 0.107) + 'px');
};

function addEvent(element, type, listener){
    if(element.addEventListener){
        element.addEventListener(type, listener, false);
    }else if(element.attachEvent){
        element.attachEvent("on"+type, listener);
    }
};