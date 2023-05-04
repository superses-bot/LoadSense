<?php
header('Content-Type: application/json');

$machine1Color = 'black';
$machine2Color = 'black';

// Assuming the messages are stored in separate files for each machine
if (file_exists('M5Stack_1_message.txt')) {
    $message1 = trim(file_get_contents('M5Stack_1_message.txt'));
    if ($message1 === 'true') {
        $machine1Color = 'green';
    } else if ($message1 === 'false') {
        $machine1Color = 'red';
    }
}

if (file_exists('M5Stack_2_message.txt')) {
    $message2 = trim(file_get_contents('M5Stack_2_message.txt'));
    if ($message2 === 'true') {
        $machine2Color = 'green';
    } else if ($message2 === 'false') {
        $machine2Color = 'red';
    }
}

echo json_encode(['machine1Color' => $machine1Color, 'machine2Color' => $machine2Color]);
