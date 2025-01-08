#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <time.h>

const char *ssid = "Aiot";  // 와이파이 이름
const char *pass = "smartaiot";      // 와이파이 비밀번호
const char *thingId = "ESP8266-thing";          // 사물 이름 (thing ID) 
const char *host = "a2olxjnp5wqtr4-ats.iot.ap-northeast-2.amazonaws.com"; // AWS IoT Core 주소

const char cert_str[] PROGMEM = R"EOF(
-----BEGIN CERTIFICATE-----
MIIDWTCCAkGgAwIBAgIUH53PlerUna8PCTEiHz5OUH5eujkwDQYJKoZIhvcNAQEL
BQAwTTFLMEkGA1UECwxCQW1hem9uIFdlYiBTZXJ2aWNlcyBPPUFtYXpvbi5jb20g
SW5jLiBMPVNlYXR0bGUgU1Q9V2FzaGluZ3RvbiBDPVVTMB4XDTI0MTIwNTEwMDAw
NloXDTQ5MTIzMTIzNTk1OVowHjEcMBoGA1UEAwwTQVdTIElvVCBDZXJ0aWZpY2F0
ZTCCASIwDQYJKoZIhvcNAQEBBQADggEPADCCAQoCggEBAL7kG4j6yUGlbvBGmTyp
d/G4qUAAITIoxb5b+x1XazF9rrWrJ7TTW/lkMv5Z98GZt3Jw6/l+hoYUzQQx6syc
PwN/KJr1NilDrqDoL6Ivemrq+jq7I19VFKpO3xBNZjSD6xMQ7Qe//749vCOZ0Sc/
vhcaxGn8Mqn0IK5SPXwBI2I1eNWTNrQA2f6vrhzU5kwXgqIuQI9TjXHIvJfIXnHA
69YC2Uc5OXAql4/kqL01O13fJ9MrYltJwkHdgVq6XuBr4T4uiOdtqYaMlbHRbG+K
ZoMBvI2NqLhzcw9AMtTPV4Q5WHIs3+9wD0Juzg7oWUSCD0aHSSJujUklp5giYB3u
9T0CAwEAAaNgMF4wHwYDVR0jBBgwFoAUU6JXAptlVN2Kr53g4knH7qOVnM8wHQYD
VR0OBBYEFIQMYsBwaTQAA7uiDUxylLWg2iNoMAwGA1UdEwEB/wQCMAAwDgYDVR0P
AQH/BAQDAgeAMA0GCSqGSIb3DQEBCwUAA4IBAQBjEmo48rj1j2u3mr55AJ/QHCL4
kW2UmeNopmExL5nAobhTfjQtcwQpdWXo76+Jq4HHrVLHel7G6o8hKdaIZIjcKoNp
+0k4hQXlKw38JE0UrAgWscalvYRMd+i8VfQ67hGMxAW+5DwUgzGtUsnwgrTszBM3
0whA83LZSqy5WDXZ2tCiu2SF279r4rNu3pVTmDZci0xWFr54GChRVqybz7aTy/lE
crw/qv4OoGZJPCKDpC3HtnLkXjDRWYQTb/X+ulw+Ya4lurOeJ6BkBq8H3FysGq8M
UeCsZTMdiXzMhGdKCbU0Ln+i9UMn7qkJhq+p8x5sp9g3XXX42ZmHvOhKuuoi
-----END CERTIFICATE-----

)EOF";
 
// Device Certificate                                               //change this
const char key_str[] PROGMEM = R"KEY(
-----BEGIN RSA PRIVATE KEY-----
MIIEowIBAAKCAQEAvuQbiPrJQaVu8EaZPKl38bipQAAhMijFvlv7HVdrMX2utasn
tNNb+WQy/ln3wZm3cnDr+X6GhhTNBDHqzJw/A38omvU2KUOuoOgvoi96aur6Orsj
X1UUqk7fEE1mNIPrExDtB7//vj28I5nRJz++FxrEafwyqfQgrlI9fAEjYjV41ZM2
tADZ/q+uHNTmTBeCoi5Aj1ONcci8l8heccDr1gLZRzk5cCqXj+SovTU7Xd8n0yti
W0nCQd2BWrpe4GvhPi6I522phoyVsdFsb4pmgwG8jY2ouHNzD0Ay1M9XhDlYcizf
73APQm7ODuhZRIIPRodJIm6NSSWnmCJgHe71PQIDAQABAoIBAB9D4TaoTLpRzD/S
cFq9LwYKRr5Sxx0eDHVHP3Po04fBnBHpPJGjZJcOUEPUnH27HY7C7ANrM8FWOtYL
DvgAR1ndQ9tNeld73GhCXzrjJ9vHLMDd0cdVehMC5AoZgznquV/gD9KLIeoY1m3G
Tblt+UZzfeQWCH5Jm+ixU5yx2wfEjd1rP9B7KOeRmLXWpoZUGknALGWjP2N/7VPc
U+Pn2NPvIxlRymwBNCth41lvUloAKgbud79Q4YduuAgo6jn04S0TG+forSHeETqd
dv3JAeHB1Nv+QYyAfXVXJaiqW3Mf21JyyAqoutcP18fw+3AT4SPSkX+OOmBm2T/R
v3KYFkECgYEA4f1RO5x1LE6fNgReDcZo9nGaUvvCvKExF1qXz/0DciWucWjvGNie
MYJXgwdt+yRfYs0PwyPcVeZV5k8XLry/Fo0lDQJgL2pp64zYn/7i5C6FbnY34Vod
0r5c6zY9+mqW687gP2pDDrRlUwkiZretZ4e15BFEipF5dVx9nqOm9qUCgYEA2D2X
YVEwuzeEayKLQJPtyBpjCHutUK2AllB330zSim+y0y4RkAWA2fKBNINMAIPZcWGt
kHuS/WcE7GYZhRH//9RHnLwpKkvy5FIgTXvEqTvtbqBAhBhPwp2SvXzkW2U0wQKN
Mxy+l1YFsTaxsLNK0ve6zT0Nug6ZOGr8HCUmWLkCgYA+7JHZz06w75NVLxIhM8W2
GRunT4KBk0BgCFGNEB4NgoB4A4fPm0Ew9+H2EfOFojMNB5wD8dA/+sqgPhVuZg+u
784JOUge67piY+1NX2M9tdZhU3lrvfPf4jFtLMVFHLi5H/lArw3M4l30KO7zOuGF
mjwDLOuwyaDQFNjFezvaIQKBgFK8RhZLDT0kgbrIDcWYk+rzBZ8ySt7Kc5+PjD3Y
09l54U7hmoCjSmiZW4wvDD5KNEEoYVMdpODlsMGOUwdSGsPUgc3bRFjVlEp9jbR3
vYgO0Mp58BVag/HvfjyX83twnp2Wj1XWEqoulKD4T0ZPYzqS5Fswl/bvLPqkHG/c
Wu6pAoGBAK55sxLUGM6mQV9baoFH2xTNLGG3Zw/5ro76KdgYNs8MZ726+4PKFVNS
BkNX1n9OwjcKGQ1IQcsVNB2nRoTUWHmUDRpUmLbn549jHONW1SAmiHztxFnHGmr+
t/sONJ9cxiAI+I1ygWyemvBC7iY2Fg5pbck1N4vgnpBzrpHAiRVx
-----END RSA PRIVATE KEY-----

)KEY";
 
// Device Private Key                                               //change this
const char ca_str[] PROGMEM = R"KEY(
-----BEGIN CERTIFICATE-----
MIIDQTCCAimgAwIBAgITBmyfz5m/jAo54vB4ikPmljZbyjANBgkqhkiG9w0BAQsF
ADA5MQswCQYDVQQGEwJVUzEPMA0GA1UEChMGQW1hem9uMRkwFwYDVQQDExBBbWF6
b24gUm9vdCBDQSAxMB4XDTE1MDUyNjAwMDAwMFoXDTM4MDExNzAwMDAwMFowOTEL
MAkGA1UEBhMCVVMxDzANBgNVBAoTBkFtYXpvbjEZMBcGA1UEAxMQQW1hem9uIFJv
b3QgQ0EgMTCCASIwDQYJKoZIhvcNAQEBBQADggEPADCCAQoCggEBALJ4gHHKeNXj
ca9HgFB0fW7Y14h29Jlo91ghYPl0hAEvrAIthtOgQ3pOsqTQNroBvo3bSMgHFzZM
9O6II8c+6zf1tRn4SWiw3te5djgdYZ6k/oI2peVKVuRF4fn9tBb6dNqcmzU5L/qw
IFAGbHrQgLKm+a/sRxmPUDgH3KKHOVj4utWp+UhnMJbulHheb4mjUcAwhmahRWa6
VOujw5H5SNz/0egwLX0tdHA114gk957EWW67c4cX8jJGKLhD+rcdqsq08p8kDi1L
93FcXmn/6pUCyziKrlA4b9v7LWIbxcceVOF34GfID5yHI9Y/QCB/IIDEgEw+OyQm
jgSubJrIqg0CAwEAAaNCMEAwDwYDVR0TAQH/BAUwAwEB/zAOBgNVHQ8BAf8EBAMC
AYYwHQYDVR0OBBYEFIQYzIU07LwMlJQuCFmcx7IQTgoIMA0GCSqGSIb3DQEBCwUA
A4IBAQCY8jdaQZChGsV2USggNiMOruYou6r4lK5IpDB/G/wkjUu0yKGX9rbxenDI
U5PMCCjjmCXPI6T53iHTfIUJrU6adTrCC2qJeHZERxhlbI1Bjjt/msv0tadQ1wUs
N+gDS63pYaACbvXy8MWy7Vu33PqUXHeeE6V/Uq2V8viTO96LXFvKWlJbYK8U90vv
o/ufQJVtMVT8QtPHRh8jrdkPSHCa2XV4cdFyQzR1bldZwgJcJmApzyMZFo6IQ6XU
5MsI+yMRQ+hDKXJioaldXgjUkK642M4UwtBV8ob2xJNDd2ZhwLnoQdeXeGADbkpy
rqXRfboQnoZsG4q5WTP468SQvvG5
-----END CERTIFICATE-----
)KEY";

#define SENSOR_PIN A0  // SZH-HWS001 SIG 핀 연결

void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }
  Serial.println();
}

X509List ca(ca_str);
X509List cert(cert_str);
PrivateKey key(key_str);
WiFiClientSecure wifiClient;
PubSubClient client(host, 8883, callback, wifiClient); // MQTT 포트 번호를 8883으로 설정

void reconnect() {
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    if (client.connect(thingId)) {
      Serial.println("connected");
      client.publish("outTopic", "hello world");
      client.subscribe("inTopic");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");

      char buf[256];
      wifiClient.getLastSSLError(buf, 256);
      Serial.print("WiFiClientSecure SSL error: ");
      Serial.println(buf);

      delay(5000);
    }
  }
}

void setClock() {
  configTime(3 * 3600, 0, "pool.ntp.org", "time.nist.gov");

  Serial.print("Waiting for NTP time sync: ");
  time_t now = time(nullptr);
  while (now < 8 * 3600 * 2) {
    delay(500);
    Serial.print(".");
    now = time(nullptr);
  }
  Serial.println("");
  struct tm timeinfo;
  gmtime_r(&now, &timeinfo);
  Serial.print("Current time: ");
  Serial.print(asctime(&timeinfo));
}

void setup() {
  Serial.begin(115200);
  Serial.setDebugOutput(true);
  Serial.println();
  Serial.println();

  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, pass);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");

  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());

  wifiClient.setTrustAnchors(&ca);
  wifiClient.setClientRSACert(&cert, &key);
  Serial.println("Certifications and key are set");

  setClock();
  client.setCallback(callback);

  pinMode(SENSOR_PIN, INPUT);  // 심박센서 핀 설정
}

long lastMsg = 0;
char msg[50];
int heartRateValue = 0;

void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();

  long now = millis();
  if (now - lastMsg > 5000) {
    lastMsg = now;
    int sensorValue = analogRead(SENSOR_PIN);  // 심박 데이터 읽기
    heartRateValue = map(sensorValue, 0, 1023, 0, 100);  // 센서 값을 0에서 100 사이로 변환
    snprintf(msg, 75, "Heart Rate: %d", heartRateValue);
    Serial.print("Publish message: ");
    Serial.println(msg);
    client.publish("outTopic", msg);
    Serial.print("Heap: ");
    Serial.println(ESP.getFreeHeap());  // Low heap can cause problems
  }
}