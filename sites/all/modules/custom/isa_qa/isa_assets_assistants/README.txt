Module description
==================


To modify the description or issues related to a question.

This is how works some functions in the module

*Generate the form

Example
$question = array(1,2);
$title = array("1. Requirements Analysis",$question);

That means that for the 1. Requirements Analysis you have two questions
to find in the table qa_question

So to create the form you browse $titles

When you submit you insert in the table project_issues
the descrition from get_description.


$description[1][0] = array('label'=>'1.1.1 Identify typical requirements',
                          'description'=> 'Consider the following typical requirements and specify them should they be relevant to your data:
* Specific technical standards must be supported. Which ones?
* You are bound to certain legal frameworks and conditions. Which ones?
* The data shall be compatible to an existing data schema or certain data exchange standards. Which ones?

This is an auto-generated Asset Development Assistant. Please refer to the library item '.l("Asset Development Assistant","asset_assistant").' for further information.
');  
    
  $description[1][1] = array('label'=>'1.1.2 Consider non-functional requirements',
                          'description'=> 'Your proposed data structures may vary according to non-functional requirements like:

* data quality
* frequency and volume of data exchange
* security constraints
* given infrastructure

If applicable, specify these standards and parameters of the intended data exchange as additional requirements on your list.


This is an auto-generated Asset Development Assistant. Please refer to the library item '.l("Asset Development Assistant","asset_assistant").' for further information
');





Example use
===========


Installation
============
Simply drop the module in /sites/all/modules and enable. 

TODO
====

