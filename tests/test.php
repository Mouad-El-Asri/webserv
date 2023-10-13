<?php
	session_start();
    $_SESSION["name"] = "charaf";
    if ($_SESSION["name"])
        echo "Sessions has been initialized with the name " . $_SESSION["name"];
    else
        echo "There is an error is session initiallization";

?>
