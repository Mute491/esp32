
String apIndexPage = "<html>\
    <head>\
        <title>Chippin' in!</title>\
        <style>\
            body {\
                background-color: black;\
                color: white;\
                max-width: 100%;\
                margin: auto;\
                font-family: Arial, sans-serif;\
                padding: 20px;\
                box-sizing: border-box;\
            }\
            #author {\
                font-family: 'Courier New', Courier, monospace;\
                text-align: center;\
                margin-bottom: 20px;\
            }\
            form{\
                width:100%;\
                height:70%;\
            }\
            input[type='submit'] {\
                cursor: pointer;\
                background-color: black;\
                color: white;\
                border: 1px solid green;\
                padding: 5px 10px;\
                text-transform: uppercase;\
                font-weight: bold;\
                width: 100%;\
                padding: 10px;\
                font-size: 16px;\
            }\
        </style>\
    </head>\
    <body>\
        <pre id='author'>\n\
______  _________ ___________________\n\
___   |/  /___/ // /___  __/__  ____/\n\
__  /|_/ /_ _  _  __/_  /  __  __/   \n\
_  /  / / /_  _  __/_  /   _  /___   \n\
/_/  /_/   /_//_/   /_/    /_____/   \n\
        </pre>\
        <form action='/attackWF' method='get'>\
            <input type='submit' name='WF' value='WiFi'>\
        </form>\
        <form action='/attackBT' method='get'>\
            <input type='submit' name='BT' value='Bluetooth'>\
        </form>\
        <form action='/attackURB' method='get'>\
            <input type='submit' name='URB' value='Usb Rubber Ducky'>\
        </form>\
    </body>\
</html>";

String selectNetworkPage="<html>\
  <head>\
    <title>Chippin' in!</title>\
    <style>\
      body {\
        background-color: black;\
        color: white;\
        max-width: 100%;\
        margin: auto;\
        font-family: Arial, sans-serif;\
        padding: 20px;\
        box-sizing: border-box;\
      }\
      #author {\
        font-family: 'Courier New', Courier, monospace;\
        text-align: center;\
        margin-bottom: 20px;\
      }\
      table {\
        width: 100%;\
        border-collapse: collapse;\
        margin: 20px 0;\
      }\
      th, td {\
        border: 1px solid green;\
        padding: 8px;\
        text-align: left;\
      }\
      th {\
        background-color: #333;\
      }\
      input[type='submit'] {\
        cursor: pointer;\
        background-color: black;\
        color: white;\
        border: 1px solid green;\
        padding: 5px 10px;\
        text-transform: uppercase;\
        font-weight: bold;\
      }\
      @media (max-width: 768px) {\
        table, th, td {\
          font-size: 14px;\
        }\
      }\
      @media (max-width: 480px) {\
        table {\
          font-size: 12px;\
        }\
        th, td {\
          padding: 6px;\
        }\
        #author {\
          font-size: 16px;\
        }\
        input[type='submit'] {\
          width: 100%;\
          padding: 10px;\
          font-size: 16px;\
        }\
      }\
    </style>\
  </head>\
  <body>\
  <pre id='author'>\
  ______  _________ ___________________\n\
  ___   |/  /___/ // /___  __/__  ____/\n\
  __  /|_/ /_ _  _  __/_  /  __  __/   \n\
  _  /  / / /_  _  __/_  /   _  /___   \n\
  /_/  /_/   /_//_/   /_/    /_____/   \n\
  </pre>\
  <table>\
    <tr>\
      <th>Network SSID</th>\
      <th>Select Network</th>\
    </tr>";

String etCaptivePortalPage = "<html><body> Captive Portal </body></html>";

String selectLogicBomb = "<html>\
    <head>\
        <title>Chippin' in!</title>\
        <style>\
            body {\
                background-color: black;\
                color: white;\
                max-width: 100%;\
                margin: auto;\
                font-family: Arial, sans-serif;\
                padding: 20px;\
                box-sizing: border-box;\
            }\
            #author {\
                font-family: 'Courier New', Courier, monospace;\
                text-align: center;\
                margin-bottom: 20px;\
            }\
            form {\
                width: 100%;\
                height: 70%;\
            }\
            input[type='submit'] {\
                cursor: pointer;\
                background-color: black;\
                color: white;\
                border: 1px solid green;\
                padding: 10px;\
                text-transform: uppercase;\
                font-weight: bold;\
                width: 100%;\
                font-size: 16px;\
                transition: 0.3s;\
            }\
            input[type='submit']:hover {\
                background-color: green;\
                color: black;\
            }\
            .radio-group {\
                margin-bottom: 20px;\
            }\
            label {\
                display: block;\
                padding: 10px;\
                margin-bottom: 5px;\
                border: 1px solid #444;\
                border-left: 5px solid green;\
                background-color: #111;\
                cursor: pointer;\
                transition: background-color 0.2s, transform 0.2s;\
            }\
            input[type='radio'] {\
                display: none;\
            }\
            input[type='radio']:checked + label {\
                background-color: green;\
                color: black;\
                transform: scale(1.02);\
            }\
        </style>\
    </head>\
    <body>\
        <pre id='author'>\n\
########  ######## ########  ##        #######  ##    ##\n\
##     ## ##       ##     ## ##       ##     ##  ##  ## \n\
##     ## ##       ##     ## ##       ##     ##   ####  \n\
##     ## ######   ########  ##       ##     ##    ##   \n\
##     ## ##       ##        ##       ##     ##    ##   \n\
##     ## ##       ##        ##       ##     ##    ##   \n\
########  ######## ##        ########  #######     ##   \n\
\n\
##        #######   ######   ####  ######               \n\
##       ##     ## ##    ##   ##  ##    ##              \n\
##       ##     ## ##         ##  ##                    \n\
##       ##     ## ##   ####  ##  ##                    \n\
##       ##     ## ##    ##   ##  ##                    \n\
##       ##     ## ##    ##   ##  ##    ##              \n\
########  #######   ######   ####  ######               \n\
\n\
########   #######  ##     ## ########                  \n\
##     ## ##     ## ###   ### ##     ##                 \n\
##     ## ##     ## #### #### ##     ##                 \n\
########  ##     ## ## ### ## ########                  \n\
##     ## ##     ## ##     ## ##     ##                 \n\
##     ## ##     ## ##     ## ##     ##                 \n\
########   #######  ##     ## ########                  \n\
        </pre>\ 
        <form action='/startURD' method='post'>\
            <div class='radio-group'>\
                <input type='radio' id='payload1' name='payload' value='0'>\
                <label for='payload1'>Download Reverse Shell</label>\
                <input type='radio' id='payload2' name='payload' value='1'>\
                <label for='payload2'>Coming soon...</label>\
                <input type='radio' id='payload3' name='payload' value='2'>\
                <label for='payload3'>Coming soon...</label>\
                <input type='radio' id='payload4' name='payload' value='3'>\
                <label for='payload4'>Coming soon...</label>\
            </div>\
            <input type='submit' value='Deploy Payload'>\
        </form>\
    </body>\
</html>";