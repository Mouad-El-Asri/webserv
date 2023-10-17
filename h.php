
<?php

echo "Content-Type: text/plain\r\n";
echo "Content-Length: 1\r\n\r\n";
echo "Method used: " . $_SERVER['REQUEST_METHOD'] . "\n";

echo "Path of cgi named PATH_INFO: " . (isset($_SERVER['PATH_INFO']) ? $_SERVER['PATH_INFO'] : '') . "\n";

echo "Query String: " . $_SERVER['QUERY_STRING'] . "\n";
?>
