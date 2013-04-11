<?php

 $result = db_query('SELECT * FROM {variable}');
    while ($variable = db_fetch_object($result)) {
      if(!$tmp = unserialize($variable->value)) {
     //   db_query("DELETE from {variable} where name = '%s'",$variable->name);
        print "Invalid var: " . $variable->name . "\n";
      }
    }
?>
