#include <SPI.h>
#include <Ethernet.h>
#include <avr/wdt.h>

#define DHCP_NO_STATUS 0
#define DHCP_RENEW_FAIL 1
#define DHCP_RENEW_OK 2
#define DHCP_REBIND_FAIL 3
#define DHCP_REBIND_OK 4
#define MAX_CMD_LENGTH 64
#define OUT_PIN 2

byte mac[] = { 0x00, 0x08, 0xdc, 0x03, 0x13, 0x37 };
EthernetServer server = EthernetServer(23);
EthernetClient client;
boolean connected = false;
String command = "";

void setup() {
  wdt_enable(WDTO_8S);
  pinMode(OUT_PIN, OUTPUT);
  digitalWrite(OUT_PIN, LOW);
  Ethernet.begin(mac);
  server.begin(); }

void loop() {
  switch (Ethernet.maintain()) {
  case DHCP_RENEW_FAIL:
  case DHCP_REBIND_FAIL:
    break;
  default:
    wdt_reset();
    break; }
  client = server.available();
  if (client) {
    if (!connected) {
      client.flush();
      connected = true; }
    if (client.available() > 0) {
      catenate(client.read()); } } }

void catenate(char c) {
  switch (c) {
    case '\n':
    case '\r':
      execute();
      break;
    default:
      command += c;
      if(command.length() >= MAX_CMD_LENGTH) {
        execute(); }
      break; } }

void execute() {
  writeString("");
  writeString("ACK COMMAND \"" + command + "\"");
  if(command == "OK") {
    digitalWrite(OUT_PIN, HIGH);
    writeString("STATE OK");
  } else {
    digitalWrite(OUT_PIN, LOW);
    writeString("STATE NOT OK"); }
  command = "";
  client.stop();
  connected = false; }

void writeString(String input) {
  input += "\r\n";
  int inLen = input.length() + 1;
  char buffer[inLen];
  input.toCharArray(buffer, inLen);
  server.write(buffer, inLen); }
