<?php
    
    $names = array("REQUEST_METHOD", "REDIRECT_STATUS", "Content_Type", "Content-Length", "Server_Name", "Path");
    for ($i=0; $i < 6; $i++) { 
        echo getenv($names[$i]);
        echo "\r\n";
    }

?>