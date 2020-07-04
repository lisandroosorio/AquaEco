/*
  MySQL Connector/Arduino Example : connect by wifi

  This example demonstrates how to connect to a MySQL server from an
  Arduino using an Arduino-compatible Wifi shield. Note that "compatible"
  means it must conform to the Ethernet class library or be a derivative
  with the same classes and methods.
  
  For more information and documentation, visit the wiki:
  https://github.com/ChuckBell/MySQL_Connector_Arduino/wiki.

  INSTRUCTIONS FOR USE

  1) Change the address of the server to the IP address of the MySQL server
  2) Change the user and password to a valid MySQL user and password
  3) Change the SSID and pass to match your WiFi network
  4) Connect a USB cable to your Arduino
  5) Select the correct board and port
  6) Compile and upload the sketch to your Arduino
  7) Once uploaded, open Serial Monitor (use 115200 speed) and observe

  If you do not see messages indicating you have a connection, refer to the
  manual for troubleshooting tips. The most common issues are the server is
  not accessible from the network or the user name and password is incorrect.

  Created by: Dr. Charles A. Bell
*/
#include <ESP8266WiFi.h>           // Use this for WiFi instead of Ethernet.h
#include <MySQL_Connection.h>
#include <MySQL_Cursor.h>
#include <DNSServer.h>
#include <ESP8266WebServer.h>
#include <WiFiManager.h>         // https://github.com/tzapu/WiFiManager
#include <ESP8266HTTPClient.h>


//char hostname[] = "us-cdbr-east-05.cleardb.net";


IPAddress server_addr(3,216,63,226);  // IP of the MySQL *server* here
IPAddress server_addr2(34,192,139,223);
char user[] = "b5e28e9c03204a";              // MySQL user login username
char password[] = "eac67379";        // MySQL user login password

const bool debug=false;

// Sample query
String insert = "";
char use_command[] = "USE heroku_760eff765fa37af";

// WiFi card example
char ssid[] = "OrlandoBois";         // your SSID
char pass[] = "waterynest285";     // your SSID Password

WiFiClient client;                 // Use this for WiFi instead of EthernetClient
MySQL_Connection conn(&client);

void soft_reset(){
  WiFi.disconnect();
  delay(5000);
  ESP.restart();
}

void reconnect(){
  //conn.close();
  if(!conn.connected()){
    
    if (conn.connect(server_addr, 3306, user, password,"heroku_760eff765fa37af")){
      
    }else{
      
      if (conn.connect(server_addr2, 3306, user, password,"heroku_760eff765fa37af")){
        
      }else{
       
        soft_reset();
        
      }
    }
    
    // create MySQL cursor object
    
  }
}

void setup()
{
  
  Serial.begin(115200);
  while (!Serial); // wait for serial port to connect. Needed for Leonardo only


  if (WiFi.SSID() != "") 
  {
      // try to connect using the saved credentials.
      // time out after 10 seconds.
      
      int count = 0;
      WiFi.mode(WIFI_STA);
      while ( (WiFi.status()!=WL_CONNECTED)   && ( count< 20)   )
      { 
            
            count++;  
            delay(500);
      }
  }
   
   
  if ( WiFi.status()!=WL_CONNECTED   )  
  {

       char APname[] = "Fish Tank Setup";
       
   
       WiFiManager wifiManager;
       wifiManager.startConfigPortal( APname);  
        
       // Reset may be required.
       // ESP.reset();
  }
 
  
  // Begin WiFi section
  //if(debug)Serial.printf("\nConnecting to %s", ssid);
  //WiFi.begin(ssid, pass);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    
  }

  // print out info about the connection:


  // getting public ip
  HTTPClient http;
  http.begin("http://api.ipify.org/");
  int code = http.GET();
  String ip;
  if (code>0){
   
    ip = http.getString();
    
  }else{
   
    soft_reset();
  }
  http.end();

  

  if (conn.connect(server_addr, 3306, user, password,"heroku_760eff765fa37af")){
    
  }else{
    
    if (conn.connect(server_addr2, 3306, user, password,"heroku_760eff765fa37af")){
     
    }else{
     
      soft_reset();
      
    }
  }

  MySQL_Cursor* cursor = new MySQL_Cursor(&conn);
  
  row_values *row = NULL;

  //cursor->execute("USE heroku_760eff765fa37af");
  //cursor->get_columns();

  insert = "SELECT identifycode FROM identifiers WHERE ip='";//SELECT identifycode FROM identifiers WHERE ip='233.34534.34.4' AND identifycode='34:54:6';
  insert += ip + "' AND identifycode='";
  insert += WiFi.macAddress() + "';";

  cursor->execute(insert.c_str());
  //if(debug)Serial.println("debug 9");
  // Fetch the columns (required) but we don't use them.
  cursor->get_columns();
  // Read the row (we are only expecting the one)

  int test=0;
  do {
    row = cursor->get_next_row();
    if (row != NULL) {
      ++test;
      int i=0;
      while(row->values[i]!=NULL){

        ++i;
      }

    }
  } while (row != NULL);
  if(test==0){
   
    // check if just the identifycode is there. if its there but ip is not, issue UPDATE to change ip
    insert = "INSERT INTO identifiers (ip, identifycode) VALUES ('";
    insert += ip + "', '" + WiFi.macAddress() + "');";

    reconnect();
    cursor->execute(insert.c_str());
    cursor->get_columns();
    do {
      row = cursor->get_next_row();
      if (row != NULL) {
        ++test;
        int i=0;
        while(row->values[i]!=NULL){

          ++i;
        }

      }
    } while (row != NULL);
  }else{

  }

  insert = "SELECT identifycode FROM ledcontrol WHERE identifycode='";
  insert += WiFi.macAddress() + "';";

  reconnect();
  cursor->execute(insert.c_str());
  //if(debug)Serial.println("debug 9");
  // Fetch the columns (required) but we don't use them.
  cursor->get_columns();
  // Read the row (we are only expecting the one)
  test=0;
  do {
    row = cursor->get_next_row();
    if (row != NULL) {
      ++test;
      int i=0;
      while(row->values[i]!=NULL){

        ++i;
      }

    }
  } while (row != NULL);
  if(test==0){
   
    insert = "INSERT INTO ledcontrol (identifycode, control, StartTime, EndTime) VALUES ('";
    insert += WiFi.macAddress() + "', 0, '08:00:00', '08:00:00');";
  
    reconnect();
    cursor->execute(insert.c_str());
    cursor->get_columns();
    do {
      row = cursor->get_next_row();
      if (row != NULL) {
        ++test;
        int i=0;
        while(row->values[i]!=NULL){
        
          ++i;
        }
      
      }
    } while (row != NULL);
  }else{
  
  }

  insert = "SELECT identifycode FROM motorcontrol WHERE identifycode='";
  insert += WiFi.macAddress() + "';";

  reconnect();
  cursor->execute(insert.c_str());
  //if(debug)Serial.println("debug 9");
  // Fetch the columns (required) but we don't use them.
  cursor->get_columns();
  // Read the row (we are only expecting the one)
  test=0;
  do {
    row = cursor->get_next_row();
    if (row != NULL) {
      ++test;
      int i=0;
      while(row->values[i]!=NULL){

        ++i;
      }

    }
  } while (row != NULL);
  if(test==0){
   
    insert = "INSERT INTO motorcontrol (identifycode, StartTime, control) VALUES ('";
    insert += WiFi.macAddress() + "', '08:00:00', 0);";
    

    reconnect();
    cursor->execute(insert.c_str());
    cursor->get_columns();
    do {
      row = cursor->get_next_row();
      if (row != NULL) {
        ++test;
        int i=0;
        while(row->values[i]!=NULL){
          if(debug)Serial.printf(row->values[i]);
          if(debug)Serial.printf(", ");
          ++i;
        }
        if(debug)Serial.printf("\n");
      }
    } while (row != NULL);
  }else{
    if(debug)Serial.println("MOTORCONTROL FOUND.");
  }
  
  insert = "SELECT identifycode FROM phsensor WHERE identifycode='";
  insert += WiFi.macAddress() + "';";
  if(debug)Serial.println(insert.c_str());
  reconnect();
  cursor->execute(insert.c_str());
  //if(debug)Serial.println("debug 9");
  // Fetch the columns (required) but we don't use them.
  cursor->get_columns();
  // Read the row (we are only expecting the one)
  test=0;
  do {
    row = cursor->get_next_row();
    if (row != NULL) {
      ++test;
      int i=0;
      while(row->values[i]!=NULL){
        if(debug)Serial.printf(row->values[i]);
        if(debug)Serial.printf(", ");
        ++i;
      }
      if(debug)Serial.printf("\n");
    }
  } while (row != NULL);
  if(test==0){
    if(debug)Serial.println("PHSENSOR NOT FOUND. INSERTING.");
    insert = "INSERT INTO phsensor (identifycode, ph) VALUES ('";
    insert += WiFi.macAddress() + "', 0);";
    if(debug)Serial.println(insert.c_str());
    reconnect();
    cursor->execute(insert.c_str());
    cursor->get_columns();
    do {
      row = cursor->get_next_row();
      if (row != NULL) {
        ++test;
        int i=0;
        while(row->values[i]!=NULL){
          if(debug)Serial.printf(row->values[i]);
          if(debug)Serial.printf(", ");
          ++i;
        }
        if(debug)Serial.printf("\n");
      }
    } while (row != NULL);
  }else{
    if(debug)Serial.println("PHSENSOR FOUND.");
  }

  insert = "SELECT identifycode FROM temperaturesensor WHERE identifycode='";
  insert += WiFi.macAddress() + "';";
  if(debug)Serial.println(insert.c_str());
  reconnect();
  cursor->execute(insert.c_str());
  //if(debug)Serial.println("debug 9");
  // Fetch the columns (required) but we don't use them.
  cursor->get_columns();
  // Read the row (we are only expecting the one)
  test=0;
  do {
    row = cursor->get_next_row();
    if (row != NULL) {
      ++test;
      int i=0;
      while(row->values[i]!=NULL){
        if(debug)Serial.printf(row->values[i]);
        if(debug)Serial.printf(", ");
        ++i;
      }
      if(debug)Serial.printf("\n");
    }
  } while (row != NULL);
  if(test==0){
    if(debug)Serial.println("TEMPERATURESENSOR NOT FOUND. INSERTING.");
    insert = "INSERT INTO temperaturesensor (identifycode, temperature) VALUES ('";
    insert += WiFi.macAddress() + "', 0);";
    if(debug)Serial.println(insert.c_str());
    reconnect();
    cursor->execute(insert.c_str());
    cursor->get_columns();
    do {
      row = cursor->get_next_row();
      if (row != NULL) {
        ++test;
        int i=0;
        while(row->values[i]!=NULL){
          if(debug)Serial.printf(row->values[i]);
          if(debug)Serial.printf(", ");
          ++i;
        }
        if(debug)Serial.printf("\n");
      }
    } while (row != NULL);
  }else{
    if(debug)Serial.println("TEMPERATURESENSOR FOUND.");
  }

  insert = "SELECT identifycode FROM turbiditysensor WHERE identifycode='";
  insert += WiFi.macAddress() + "';";
  if(debug)Serial.println(insert.c_str());
  reconnect();
  cursor->execute(insert.c_str());
  //if(debug)Serial.println("debug 9");
  // Fetch the columns (required) but we don't use them.
  cursor->get_columns();
  // Read the row (we are only expecting the one)
  test=0;
  do {
    row = cursor->get_next_row();
    if (row != NULL) {
      ++test;
      int i=0;
      while(row->values[i]!=NULL){
        if(debug)Serial.printf(row->values[i]);
        if(debug)Serial.printf(", ");
        ++i;
      }
      if(debug)Serial.printf("\n");
    }
  } while (row != NULL);
  if(test==0){
    if(debug)Serial.println("TURBIDITYSENSOR NOT FOUND. INSERTING.");
    insert = "INSERT INTO turbiditysensor (identifycode, turbidity) VALUES ('";
    insert += WiFi.macAddress() + "', 0);";
    
    if(debug)Serial.println(insert.c_str());
    reconnect();
    cursor->execute(insert.c_str());
    cursor->get_columns();
    do {
      row = cursor->get_next_row();
      if (row != NULL) {
        ++test;
        int i=0;
        while(row->values[i]!=NULL){
          if(debug)Serial.printf(row->values[i]);
          if(debug)Serial.printf(", ");
          ++i;
        }
        if(debug)Serial.printf("\n");
      }
    } while (row != NULL);
  }else{
    if(debug)Serial.println("TURBIDITYSENSOR FOUND.");
  }
  
}

float sensorData[] = {
  0,//temp
  0,//turb
  0 //phph
  };
String sensors[] = {
  "temperature",
  "turbidity",
  "ph"
};

void updateDatabase(String sensor,float data){
  reconnect();
  if(conn.connected()){
    MySQL_Cursor* cursor = new MySQL_Cursor(&conn);
    //if(debug)Serial.println("debug 4");
    row_values *row = NULL;
    //if(debug)Serial.println("debug 5");
    
    //cursor->execute("USE heroku_760eff765fa37af");
    //if(debug)Serial.println("debug 6");
    // Fetch the columns (required) but we don't use them.
    //cursor->get_columns();
    //cursor->close();
    //if(debug)Serial.println("debug 8");
    insert = "UPDATE ";
    insert += sensor + "sensor set " + sensor + "=" + data + " where identifycode='" +WiFi.macAddress() + "';";
    if(debug)Serial.println(insert.c_str());
    cursor->execute(insert.c_str());
    //if(debug)Serial.println("debug 9");
    // Fetch the columns (required) but we don't use them.
    cursor->get_columns();
    // Read the row (we are only expecting the one)
    do {
      row = cursor->get_next_row();
      if (row != NULL) {
        int i=0;
        while(row->values[i]!=NULL){
          if(debug)Serial.printf(row->values[i]);
          if(debug)Serial.printf(", ");
          ++i;
        }
        if(debug)Serial.printf("\n");
      }
    } while (row != NULL);
    // Now we close the cursor to free any memory
    cursor->close();
  
   
  }
}

void sendNewData(){

    updateDatabase(sensors[0],sensorData[0]);
    updateDatabase(sensors[1],sensorData[1]);
    updateDatabase(sensors[2],sensorData[2]);
    //Serial.print("ESP updated database with temp %f turb %f ph %f",sensorData[0],sensorData[1],sensorData[2]);


  
}

bool updated=false;

String first;
String second;
String third;

void loop()
{
  
  if(debug)Serial.print("loop\n");

while(Serial.available()){
      first  = Serial.readStringUntil(',');
   // Serial.read(); //next character is comma, so skip it using this
     second = Serial.readStringUntil(',');
   // Serial.read();
     third  = Serial.readStringUntil('\n');
    //parse your data here. example:
    //double x = Double.parseDouble(first);
 
      
    
}
   sensorData[0] = first.toFloat();     // convert this part to a float
     sensorData[1] = second.toFloat();  
      sensorData[2] = third.toFloat();  
    String test2 = "ESP RECEIVED and sending:" + first + " " + second + " " + third + "\n";
    Serial.print(test2);
    
   // Serial.print(test);    
 // while(Serial.available()){
   // String serialData = Serial.readStringUntil('\n');
  //  if(debug)Serial.println("I got this data:\n");
  //  if(debug)Serial.println(serialData);
    //if(serialData.substring(0,4)=="Read"){
      //sensor temp 95
      //sensor turb 10
      //sensor phph 1.6
      
//      if(serialData.substring(5,16)=="temperature"){
//        Serial.print("ESP received temperature reading: ");
//        Serial.print(serialData.substring(17));
//        Serial.print("\n");
//        sensorData[0]=serialData.substring(17).toFloat();
//        updated=true;
//      }
//      if(serialData.substring(5,14)=="turbidity"){
//        Serial.print("ESP received turbidity reading: ");
//        Serial.print(serialData.substring(15));
//        Serial.print("\n");
//        sensorData[1]=serialData.substring(15).toFloat();
//        updated=true;
//      }
//      if(serialData.substring(5,7)=="ph"){
//        Serial.print("ESP received ph reading: ");
//        Serial.print(serialData.substring(8));
//        Serial.print("\n");
//        sensorData[2]=serialData.substring(8).toFloat();
//        updated=true;
//      }
//    }else{
//      if(serialData.substring(0,3)=="ESP"){
//        //ignore writeback
//      }else{
//        Serial.println("ESP received unrecognised command.");
//      }
//    }
 // }

    sendNewData();

  //delay(1000);
}
