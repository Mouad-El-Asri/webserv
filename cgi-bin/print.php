<?php
    
    $names = array("REQUEST_METHOD", "REDIRECT_STATUS", "Content_Type", "Content-Length", "Server_Name", "Url_Path", "Path_Info");
    for ($i=0; $i < 8; $i++) { 
        echo getenv($names[$i]);
        echo "\r\n";
    }

?>