<?php
// Generate a large string of 1000 words
$words = str_repeat("133337 133371337 138888 137 1337 1337 1337 1337 1337 1337 1337 1337  ", 10);


// Printing out the retrieved values

// Set the content length header manually
echo "Content-Length: " . strlen($words) . "\r\n\r\n";

// Set the content type header manually
$method = $_SERVER['REQUEST_METHOD'];
$mE = $_SERVER['PATH_INFO'];
echo "PATH_INFO: $mE <br>";
echo "method: $method <br>";
// Print the words
echo $words;
?>
