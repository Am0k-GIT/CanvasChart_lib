#include <Arduino.h>
#include <Esp.h>
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <CanvasChart.h>

const char ssid[] = "MyWiFi";
const char pass[] = "MyPassword";

const char HTML_head[] PROGMEM = R"(<!DOCTYPE html> 
    <meta http-equiv='Content-Type' content=text/html; charset='utf-8'>
    <title>CanvasChart Library Example Page</title>
    <style>
        form {margin: 0 auto;width: 800px;padding: 1em;border: 1px solid #ccc;border-radius: 1em; background-color: rgb(247, 245, 240);}
        h1 {text-align: center;}
        h3 {text-align: left;}
        span.mt {color: Teal;}
        span.arg {color: Purple;}
    </style>
    </head>
    <body bgcolor='DimGrey'>
    <form>
    <h1> CanvasChart Library </h1>
)";

const char HTML_end[] PROGMEM = R"(
    </form>
    </body>
</html>)";

const char HTML_main[] PROGMEM = R"(
    <h3> Library methods: </h3>
    <p><span class="mt">CanvasChart newChart</span> (<span class="arg">uint16_t points, bool extraData</span>); - creating an object of the class 
    (points - the number of points of reference in the stack (FIFO), extraData - using an additional data column (by default false)).</p>
    <p><span class="mt">void CanvasChart.setLegendFont</span> (<span class="arg">String font, uint8_t size</span>); - set font and size for axis labels.
    Optional method.</p>
    <p><span class="mt">void CanvasChart.setXLegendPoints</span> (<span class="arg">uint8_t intervaleA</span>); - set the number of samples on the X-axis.
    Optional method.</p>
    <p><span class="mt">void CanvasChart.setYLegendPoints</span> (<span class="arg">uint8_t intervaleLeft, uint8_t intervaleRight</span>); - set the number 
    of samples for the left and right Y-axes (intervaleRight optional with extraData = true). Optional method.</p>
    <p><span class="mt">void CanvasChart.setLeftYstyle</span> (<span class="arg">String lineColor, uint8_t lineWidth</span>); - set the line color, 
    width and top label for the first (left) graph. Optional method.</p>
    <p><span class="mt">void CanvasChart.setRightYstyle</span> (<span class="arg">String lineColor, uint8_t lineWidth</span>); - set the line color, 
    width and top label for the additional (right) graph. Optional method.</p>
    <p><span class="mt">void CanvasChart.setTimeFormat</span> (<span class="arg">String TimeFormat</span>); - set the time formatting type for epoch time
    (preset YY - year, MO - month, DD - day of the month, HH -  hour, MM - minutes, SS - seconds, MS - milliseconds, e.g. "HH:MM:SS.MS". 
    You can use any separators). Optional method.</p>
    <p><span class="mt">bool CanvasChart.ready</span> (); - return true if the chart can be displayed (3 or more points).</p>
    <p><span class="mt">void CanvasChart.push</span> (<span class="arg">uint64_t X, float A, float B</span>); - add a row of data to the stack 
    (X - X column of data (e.g. epoch time), A - Y-letf column of data, B - Y-right column of data (optional with extraData = true).</p>
    <p><span class="mt">String CanvasChart.getChart</span> (<span class="arg">uint16_t width, uint16_t height</span>); - return a string with the 
    script text for drawing a chart with dimensions width / height.</p>
    <h3> Library version: 1.0.0</h3>
    <p><a href="https://github.com/Am0k-GIT/CanvasChart_lib">GitHub page</a></p>)";

WiFiClient espClient;
ESP8266WebServer HttpServer(80);

CanvasChart canv(50, true);

void handleRoot() 
{
        String str(FPSTR(HTML_head));
        if (canv.ready())
                str += canv.getChart(800,600);
        else
                str += R"(<h1> Chart not ready </h1>
                )";
        str += FPSTR(HTML_main);
        str += FPSTR(HTML_end);
        HttpServer.send ( 200, "text/html", str );
}

void handleNotFound() 
{
        HttpServer.send(404, "text/plain", "404: Not found");
}

void setup_WiFi()
{
        WiFi.mode( WIFI_STA);
        delay(10);
        WiFi.begin(ssid, pass);
        uint8_t attempts = 0;
        while (WiFi.status() != WL_CONNECTED && attempts < 50) {
                attempts++;
                delay(200);
        }
}

void setup_HTTPserver() 
{
        HttpServer.on("/", handleRoot);
        HttpServer.onNotFound(handleNotFound);
        HttpServer.begin();
}

void setup() {
        setup_WiFi();
        setup_HTTPserver();
        canv.setXLegendPoints(8);
        canv.setYLegendPoints(5, 4);
        canv.setLegendFont("Trebuchet MS", 16);
        canv.setLeftYstyle("orange", 5, "Temperature, C");
        canv.setRightYstyle("blue", 5, "Pressure, mmHg");
        canv.setTimeFormat("HH:MM:SS");
        canv.push(1628870600000, 16.00, 760.10);
        canv.push(1628870700000, 15.97, 760.12);
        canv.push(1628870800000, 15.90, 760.14);
        canv.push(1628870900000, 15.75, 760.17);
        canv.push(1628871000000, 15.62, 760.18);
        canv.push(1628871100000, 15.34, 760.22);
        canv.push(1628871200000, 15.25, 760.28);
        canv.push(1628871300000, 15.20, 760.34);
        canv.push(1628871400000, 15.12, 760.41);
        canv.push(1628871500000, 14.98, 760.49);
        canv.push(1628871600000, 14.72, 760.62);
        canv.push(1628871700000, 14.34, 760.70);
        canv.push(1628871800000, 14.18, 760.76);
        canv.push(1628871900000, 14.03, 760.81);
        canv.push(1628872000000, 13.97, 760.84);
        canv.push(1628872100000, 13.94, 760.83);
        canv.push(1628872200000, 13.99, 760.80);
        canv.push(1628872300000, 14.02, 760.77);
        canv.push(1628872400000, 14.11, 760.73);
        canv.push(1628872500000, 14.19, 760.69);
        canv.push(1628872600000, 14.26, 760.66);
        canv.push(1628872700000, 14.28, 760.64);
        canv.push(1628872800000, 14.29, 760.62);
        canv.push(1628872900000, 14.30, 760.61);
        canv.push(1628873000000, 14.29, 760.60);
        canv.push(1628873100000, 14.30, 760.59);
        canv.push(1628873200000, 14.31, 760.60);
}

void loop() {
        HttpServer.handleClient();
}