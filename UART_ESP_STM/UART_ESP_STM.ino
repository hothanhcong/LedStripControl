#include <ESP8266WiFi.h>
//#include <WiFi.h>
#include <WebSocketsClient.h>
#include <stdio.h>
#include <string.h>
#define MAX_LENGTH 50 

// char* ssid = "AGV-CONNECT";
// char* password = "agv123456";
//const char* ssid = "Cong";
//const char* password = "11111111";
const char* ssid = "BAN SP- THACO INDUSTRIES";
const char* password = "BSP@2023@";
const char* webSocketServerAddress = "10.14.17.45";
const int webSocketServerPort = 8080;

WebSocketsClient webSocket;

void setup() {
  Serial.begin(115200);

  // Kết nối đến mạng WiFi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to WiFi");

  // Kết nối đến máy chủ WebSocket
  webSocket.begin(webSocketServerAddress, webSocketServerPort, "/");
  webSocket.onEvent(webSocketEvent);
}

void loop() {
  webSocket.loop();
}

void webSocketEvent(WStype_t type, uint8_t* payload, size_t length) {
  switch (type) {
    case WStype_DISCONNECTED:
      Serial.println("WebSocket disconnected");
      break;
    case WStype_CONNECTED:
      Serial.println("WebSocket connected");
      break;
    case WStype_TEXT:
      String data = "";
      for (size_t i = 0; i < length; i++) {
        data += (char)payload[i];
      }
      // Kiểm tra nếu tin nhắn có nhãn là "IP_ADDRESSES"
      // Nếu có, bỏ qua
      if (data.indexOf("\"label\":\"IP_ADDRESSES\"") != -1) {
        break;
      } else {
        // Nếu không, thực hiện xử lý dữ liệu
        handleServerData(payload);
      }
      break;
  }
}

void handleServerData(uint8_t* payload) {
  // Thực hiện xử lý dữ liệu nhận được từ server ở đây
//Serial.println(((char*)payload));
char outputString[MAX_LENGTH] = {0};
processString((char*)payload, outputString);
} 

void processString(char *inputString, char *outputString) {
    // Duyệt qua chuỗi
    int index = 0;
    while (*inputString != '\0' && index < MAX_LENGTH - 1) {
        // Nếu là số
        if (*inputString >= '0' && *inputString <= '9') {
            char number[4] = {0};  // Chuỗi lưu trữ số, bao gồm một ký tự null

            // Lấy và lưu trữ số vào chuỗi
            int numberIndex = 0;
            while (*inputString >= '0' && *inputString <= '9' && numberIndex < 3) {
                number[numberIndex++] = *inputString++;
            }

            // Thêm số 0 vào trước nếu số có ít hơn 3 chữ số
            for (int i = 0; i < 3 - numberIndex; ++i) {
                outputString[index++] = '0';
            }

            // Copy số vào chuỗi kết quả
            for (int i = 0; i < numberIndex; ++i) {
                outputString[index++] = number[i];
            }
        }

        // Nếu là dấu phẩy, thêm vào chuỗi
        if (*inputString == ',') {
            outputString[index++] = ',';
        }

        // Di chuyển con trỏ đến ký tự tiếp theo
        inputString++;
    }

    // Đảm bảo kết thúc chuỗi kết quả
    outputString[index++] = ',';
    outputString[index] = '\0';
    Serial.print(outputString);
}




//========================
/*
#include <stdio.h>
#include <string.h>

#define MAX_LENGTH 256  // Độ dài tối đa của chuỗi kết quả, điều chỉnh nếu cần

void processString(char *inputString, char *outputString) {
    // Duyệt qua chuỗi
    int index = 0;
    while (*inputString != '\0' && index < MAX_LENGTH - 1) {
        // Nếu là số
        if (*inputString >= '0' && *inputString <= '9') {
            char number[4] = {0};  // Chuỗi lưu trữ số, bao gồm một ký tự null

            // Lấy và lưu trữ số vào chuỗi
            int numberIndex = 0;
            while (*inputString >= '0' && *inputString <= '9' && numberIndex < 3) {
                number[numberIndex++] = *inputString++;
            }

            // Thêm số 0 vào trước nếu số có ít hơn 3 chữ số
            for (int i = 0; i < 3 - numberIndex; ++i) {
                outputString[index++] = '0';
            }

            // Copy số vào chuỗi kết quả
            for (int i = 0; i < numberIndex; ++i) {
                outputString[index++] = number[i];
            }
        }

        // Nếu là dấu phẩy, thêm vào chuỗi
        if (*inputString == ',') {
            outputString[index++] = ',';
        }

        // Di chuyển con trỏ đến ký tự tiếp theo
        inputString++;
    }

    // Đảm bảo kết thúc chuỗi kết quả
    outputString[index] = '\0';
    printf("%s\n", outputString);
}

int main() {
    char inputString[] = "[16, 60, 136, 4, 3]";
    char outputString[MAX_LENGTH] = {0};  // Chuỗi lưu trữ kết quả

    // Gọi hàm xử lý chuỗi
    processString(inputString, outputString);

    return 0;
}
*/
