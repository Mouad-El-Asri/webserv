<html>
<?php
while(1);
    $names = array("REQUEST_METHOD", "REDIRECT_STATUS", "Content_Type", "Content-Length", "Server_Name", "Url_Path", "Path_Info", "PHP_SELF");
    for ($i=0; $i < 9; $i++) { 
        echo getenv($names[$i]);
        echo "\r\n";
    }
?>
</html>