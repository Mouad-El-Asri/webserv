<?php
// Generate a large string of 1000 words
$words = str_repeat("133337 133371337 138888 137 1337 1337 1337 1337 1337 1337 1337 1337  ", 1);


// Printing out the retrieved values

echo "Content-Length: 100000000000000\r\n\r\n";

// Set the content type header manually
$method = $_SERVER['REQUEST_METHOD'];
$mE = $_SERVER['PATH_INFO'];
$anas = $_SERVER['QUERY_STRING'];
echo "QUERY_STRING: $anas <br>";
echo "PATH_INFO: $mE <br>";
echo "method: $method <br>";
// Print the words
echo $words;
?>
