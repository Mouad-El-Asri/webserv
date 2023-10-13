<?php
	session_start();
	if ($_SESSION["name"])
		echo "Already the name var is set";
	else
	{
	$_SESSION["name"] = "charaf";
	if ($_SESSION["name"])
		echo "Sessions has been initialized with the name " . $_SESSION["name"];
	else
		echo "There is an error is session initiallization";
	}

?>
