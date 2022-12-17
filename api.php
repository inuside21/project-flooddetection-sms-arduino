<?php

    // database
    // =========================================================
    // Declare
    require_once 'twilio-php-main/src/Twilio/autoload.php'; 
    $connection = mysqli_connect("localhost","u684904720_bahadb","pass","u684904720_bahadb");

    // SMS
    use \Twilio\Rest\Client; 

    // Date
    date_default_timezone_set("Asia/Manila");
    $date = new DateTime();
    $dateResult = $date->format('Y-m-d H:i:s');
    $dateOnlyResult = $date->format('Y-m-d');
    $dateOnlyResultYearMonth = $date->format('Y-m');
    $dateOnlyResultYear = $date->format('Y');
    $dateOnlyResultHour24 = $date->format('H');

    // check
    if (!isset($_POST['mode'])) {
        echo json_encode(array("status" => "error", "message" => "Mode Error"));
        exit();
    }


    /*
        $last_id = mysqli_insert_id($connection);
    */




    /*
        ======================================
        MODES
        ======================================
    */
    // Send SMS
    // =======================================
    if ($_POST['mode'] == 'sendsms')
    {
        $resData = JSONGet();

        // check
        {
            $sql="select * FROM setting_tbl"; 
            $rsgetacc=mysqli_query($connection,$sql);
            while ($rowsgetacc = mysqli_fetch_object($rsgetacc))
            {
                if ((int) $rowsgetacc->set_lastupdate > strtotime($dateResult))
                {
                    return;
                }
            }
        }

        $getMessage = $_POST['aMsg'];
        $getWater = explode(',', $getMessage)[0];
        $getTemp = explode(',', $getMessage)[1];

        $setMessage = "Bantay Baha - Notification - ";
        $newMessage = "";

        // check
        if ((int) $getWater <= 10)
        {
            $waterPercent = 10.0 - (float) $getWater;
            $waterPercent = $waterPercent / 10.0;
            $waterPercent = $waterPercent * 100;

            $newMessage = $setMessage . "Orange Alert! Water level is increase by " . $waterPercent . "% at your place. Current Temperature is " . $getTemp . "C.";
        }
        if ((int) $getWater <= 5)
        {
            $waterPercent = 10.0 - (float) $getWater;
            $waterPercent = $waterPercent / 10.0;
            $waterPercent = $waterPercent * 100;

            $newMessage = $setMessage . "Red Alert! Water level is increase by " . $waterPercent . "% at your place. Current Temperature is " . $getTemp . "C.";
        }
        if ((int) $getWater <= 0)
        {
            return;
        }
        
        // check
        if ($newMessage != "")
        {
            // send sms
            $sid    = "AC16adcfbf4923a8bef9ba757b127b94c3"; 
            $token  = "ebb96699fd9ecc937b806a4c54386169"; 
            $twilio = new Client($sid, $token); 
            $message = $twilio->messages 
                            ->create("+639614335484", // to 
                                    array(  
                                        "messagingServiceSid" => "MGe3c17c34547d372724eaf8b04420e692",      
                                        "body" => $newMessage 
                                    )  
                                ); 

            // save
            $newtime = strtotime($dateResult) + 60;
            $sql="update setting_tbl set set_lastupdate = '" . $newtime . "'"; 
            $rsgetacc=mysqli_query($connection,$sql);   
        }

        /*
        // send email
        $to = "";
        $subject = "Bantay Baha - Notification";
        $txt = "
                <center>
                    " . $setMessage . "
                </center>
        ";        
        $headers = "MIME-Version: 1.0\r\n";
        $headers .= "Content-Type: text/html; charset=ISO-8859-1\r\n";
        $headers .= "From: admin@martorenzo.click";
        mail($to,$subject,$txt,$headers);
        */
    }

    // Test Post
    // =======================================
    if ($_POST['mode'] == 'testpost')
    {
        $resData = JSONGet();
        echo "recevied: " . $_POST['testmsg'];
    }

    /*
        ======================================
        FUNCTIONS
        ======================================
    */

    // JSON - Get
    // ---------------------------------------
    function JSONGet()
    {
        // get json
        $json = file_get_contents('php://input');
        $data = json_decode($json);
        return $data;
    }

    // JSON - Set     
    // ---------------------------------------
    function JSONSet($resStatus, $resTitle = "", $resMsg = "", $resData = "")
    {
        /*
            status:
                ok      - success
                error   - error

            title:
                return any notif title

            message:
                return any notif msg
            
            data:
                return any result
        */
        echo json_encode(array("status" => $resStatus, "title" => $resTitle, "message" => $resMsg, "data" => $resData));
        exit();
    }

    // IDs
    // ---------------------------------------
    function GUID()
    {
        if (function_exists('com_create_guid') === true)
        {
            return trim(com_create_guid(), '{}');
        }

        return sprintf('%04X%04X', mt_rand(0, 65535), mt_rand(0, 65535));
    }
?>