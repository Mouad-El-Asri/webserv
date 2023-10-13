<?php
echo "Content-Length: 900\r\n\r\n";
?>
<html>
<body>

<form action="" method="get">
Name: <input type="text" name="name"><br>
E-mail: <input type="text" name="email"><br>
<input type="submit">
<?php  
    echo "You submitted a name that is " . $_GET["name"];
    ?>
</form>

</body>
</html>