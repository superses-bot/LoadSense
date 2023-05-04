<?php
    header('Content-Type: application/json');

    // Check if the 'message' and 'id' parameters are set in the GET request
    if (isset($_GET['message']) && isset($_GET['id'])) {
        $message = $_GET['message'];
        $m5stack_id = $_GET['id'];

        // Ensure the message is either 'true' or 'false'
        if ($message === 'true' || $message === 'false') {
            // Save or process the message as needed
            // For example, save the message to a file named by the M5Stack ID:
            file_put_contents($m5stack_id . '_message.txt', $message);

            // Respond with a JSON success message
            echo json_encode(['success' => true, 'message' => 'Message received and saved.']);
        } else {
            // Respond with a JSON error message
            echo json_encode(['success' => false, 'message' => 'Invalid message value.']);
        }
    } else {
        // Respond with a JSON error message
        echo json_encode(['success' => false, 'message' => 'Missing message or id parameter.']);
    }
?>
