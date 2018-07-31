<?php

	$connect = mysqli_connect("localhost", "root", "sql123", "prj");

	// create a table in mysql - below is SQL preview of table creation query
	// CREATE TABLE `prj`.`logtable` ( `date` VARCHAR(10) NOT NULL , `time` VARCHAR(10) NOT NULL , `locID` VARCHAR(3) NOT NULL , `readings` DOUBLE NOT NULL , `unit` VARCHAR(3) NOT NULL ) ENGINE = InnoDB;
	$locID =  $_GET['loc'];
	$reading = $_GET['value'];

	if($reading&$locID)
	{
		$date = date("d/m/Y");
		$time = date("h:i:sa");
		
		// log data into database
		mysqli_query($connect,"INSERT INTO logtable values('$date', '$time', '$locID', '$reading', 'ppm')");
		
	}	

?>