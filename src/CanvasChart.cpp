/*
 Canvas Chart library.
 Pavel "Am0k" Kolesov
 https://github.com/Am0k-GIT/CanvasChart_lib

 This program is free software: you can redistribute it and/or modify it under the terms of the GNU
 General Public License as published by the Free Software Foundation, either version 3 of the
 License, or (at your option) any later version. This program is distributed in the hope that it
 will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more details. You should
 have received a copy of the GNU General Public License along with this program.  If not, see
 <http://www.gnu.org/licenses/>.
*/

#include "CanvasChart.h"
#include <Arduino.h>

const char HTML_canvasH_p1[] PROGMEM = R"(<canvas id="canv" width=")";

const char HTML_canvasH_p2[] PROGMEM = R"(">
	Your browser doesn't support canvas.
</canvas>)";

const char HTML_scriptH[] PROGMEM = R"(
<script type="text/javascript">
var c=document.getElementById("canv");
var ctx=c.getContext("2d");
ctx.fillStyle = "black";
ctx.strokeStyle = "black";
ctx.lineJoin = "round";)";

const char HTML_var[] PROGMEM = R"(var )";

const char HTML_let[] PROGMEM = R"(let )";

const char HTML_Sfont[] PROGMEM = R"(ctx.font = ")";

const char HTML_SlineWidth[] PROGMEM = R"(ctx.lineWidth = )";

const char HTML_count[] PROGMEM = R"(let count)";

const char HTML_lineTo[] PROGMEM = R"(ctx.lineTo()";

const char HTML_moveTo[] PROGMEM = R"(ctx.moveTo()";

const char HTML_newDate[] PROGMEM = R"( = new Date()";

const char HTML_TimeLines_p1[] PROGMEM = R"(for(let i = 0; i < countX - 1; i++) {
var tMIN = minT.getTime();
var tMAX = maxT.getTime();
Tstamp = new Date(tMIN + i * (tMAX - tMIN)/ countX);)";

const char HTML_TimeLines_canvHeight[] PROGMEM = R"(i* (canv.width - 20 * 2) / (countX - 1))";

const char HTML_timeGet[] PROGMEM = R"( = Tstamp.get)";

const char HTML_strokeStyle[] PROGMEM = R"(ctx.strokeStyle = ")";

const char HTML_beginPath[] PROGMEM = R"(ctx.beginPath();)";

const char HTML_DataLines_p0[] PROGMEM = R"(for (let i = 1; i < countY)";

const char HTML_DataLines_p1[] PROGMEM = R"(; i++) {
vS = String)";

const char HTML_fillText[] PROGMEM = R"(ctx.fillText()";


const char HTML_DataLines_canvHeight[] PROGMEM = R"(canv.height - (i * (canv.height - 20 * 2) / countY)";

const char HTML_stroke[] PROGMEM = R"(ctx.stroke();)";

const char HTML_endScript[] PROGMEM = R"(</script>)";


CanvasChart::CanvasChart(uint16_t points, bool extraData)
{
    m_points = points;
    m_extraData = extraData;
    m_Xvalue = new uint64_t[points];
    m_YvalueA = new float[points];
    if (m_extraData)
    {
        m_YvalueB = new float[points];
    }
}

CanvasChart::~CanvasChart()
{
    delete[] m_Xvalue;
    delete[] m_YvalueA;
    delete[] m_YvalueB;
}

uint16_t CanvasChart::remap(float X, float in_min, float in_max, uint16_t out_min, uint16_t out_max)
{
    return ((X - in_min) * (out_max - out_min)) / (in_max - in_min) + out_min;
}

uint16_t CanvasChart::remap(uint16_t X, uint16_t in_min, uint16_t in_max, uint16_t out_min, uint16_t out_max)
{
    return ((X - in_min) * (out_max - out_min)) / (in_max - in_min) + out_min;
}

uint16_t CanvasChart::remap(uint64_t X, uint64_t in_min, uint64_t in_max, uint16_t out_min, uint16_t out_max)
{
    return ((X - in_min) * (out_max - out_min)) / (in_max - in_min) + out_min;
}

void CanvasChart::set(uint16_t position, uint64_t X, float A, float B)
{
    m_Xvalue[position] = X;
    m_YvalueA[position] = A;
    m_YvalueB[position] = B;
}

void CanvasChart::set(uint16_t position, uint64_t X, float A)
{
    m_Xvalue[position] = X;
    m_YvalueA[position] = A;
}

void CanvasChart::searchMIN(float* array, float newData, uint16_t & index)
{
    if (index > 0)
    {
        index--;
        if (newData < array[index])
            index = m_points - 1;
    }
    else
    {
        for (uint16_t i = 1; i < m_points; i++)
        {
            if (array[i] < array[index])
            index = i;
        }
    }
}

void CanvasChart::searchMAX(float* array, float newData, uint16_t & index)
{
    if (index > 0)
    {
        index--;
        if (newData > array[index])
            index = m_points - 1;
    }
    else
    {
        for (uint16_t i = 1; i < m_points; i++)
        {
            if (array[i] > array[index])
            index = i;
        }
    }
}

void CanvasChart::updateTimeStrings(String & valueS, String & commandS, String & abbrData, String Data)
{
    valueS += R"(var )";
    valueS += abbrData;
    valueS += FPSTR(HTML_timeGet);
    valueS += Data;
    valueS += R"(();)";
    valueS += "\n";
    if (abbrData == "MM" || abbrData == "SS")
    {
        commandS += R"((()";
        commandS += abbrData;
        commandS += R"( < 10) ? "0" : "") + )";
    }
    commandS += abbrData;
}

void CanvasChart::addTimeFormat(String & updatedString)
{
    String valStr = "";
	String comStr = "var vS = ";
    if (m_timeFormat.length() == 0)
    {
        valStr += R"(var tTT = Tstamp.getTime();)";
        valStr += "\n";
        comStr += R"(tTT)";

    }
	for (uint8_t i = 0; i < m_timeFormat.length(); i++)
	{
		String buffer = m_timeFormat.substring(i, i + 2);
		if (buffer == "YY")
		{
            updateTimeStrings(valStr, comStr, buffer, "FullYear");
			i++;
		}
		else if (buffer == "MO")
		{
            updateTimeStrings(valStr, comStr, buffer, "Month");
			i++;
		}
        else if (buffer == "DD")
		{
            updateTimeStrings(valStr, comStr, buffer, "Date");
			i++;
		}
        else if (buffer == "HH")
		{
            updateTimeStrings(valStr, comStr, buffer, "Hours");
			i++;
		}
        else if (buffer == "MM")
		{
            updateTimeStrings(valStr, comStr, buffer, "Minutes");
			i++;
		}
        else if (buffer == "SS")
		{
            updateTimeStrings(valStr, comStr, buffer, "Seconds");
			i++;
		}
        else if (buffer == "MS")
		{
            updateTimeStrings(valStr, comStr, buffer, "Milliseconds");
			i++;
		}
		else
		{
			comStr += R"(")";
			comStr += m_timeFormat[i];
			comStr += R"(")";
		}
		if (i < (m_timeFormat.length() - 1))
			comStr += R"( + )";
	}
    comStr += ";";
    comStr += "\n";

    updatedString += valStr + comStr;
}

void CanvasChart::setLegendFont(String font, uint8_t size)
{
    m_legendFont = font;
    m_legendFontSize = size;
}

void CanvasChart::setXLegendPoints(uint8_t intervale)
{
    m_X_LegendPoints = intervale;
}

void CanvasChart::setYLegendPoints(uint8_t intervaleLeft, uint8_t intervaleRight)
{
    m_Yleft_LegendPoints = intervaleLeft;
    m_Yright_LegendPoints = intervaleRight;
}

void CanvasChart::setYLegendPoints(uint8_t intervaleLeft)
{
    m_Yleft_LegendPoints = intervaleLeft;
}

void CanvasChart::setLeftYstyle(String lineColor, uint8_t lineWidth, String label)
{
    m_colorLeft = lineColor;
    m_lineWidthLeft = lineWidth;
    m_labelLeft = label;
}

void CanvasChart::setRightYstyle(String lineColor, uint8_t lineWidth, String label)
{
    m_colorRight = lineColor;
    m_lineWidthRight = lineWidth;
    m_labelRight = label;
}

void CanvasChart::setTimeFormat(String TimeFormat)
{
    m_timeFormat = TimeFormat;
}

void CanvasChart::push(uint64_t X, float A, float B)
{
    if (m_extraData) 
    {
        if (m_currentPoint == 0)
        {
            m_Yleft_min_index = 0;
            m_Yleft_max_index = 0;
            m_Yright_min_index = 0;
            m_Yright_max_index = 0;
        }
        if (m_currentPoint < m_points)
        {
            set(m_currentPoint, X, A, B);
            if (A < m_YvalueA[m_Yleft_min_index])
                m_Yleft_min_index = m_currentPoint;
            if (A > m_YvalueA[m_Yleft_max_index])
                m_Yleft_max_index = m_currentPoint;
            if (B < m_YvalueB[m_Yright_min_index])
                m_Yright_min_index = m_currentPoint;
            if (B > m_YvalueB[m_Yright_max_index])
                m_Yright_max_index = m_currentPoint;     
            m_currentPoint++;
        }
        else
        {
            for (uint16_t i = 0; i < m_points - 1; i++)
            {
                m_Xvalue[i] = m_Xvalue[i + 1];
                m_YvalueA[i] = m_YvalueA[i + 1];
                m_YvalueB[i] = m_YvalueB[i + 1];
            }
            set(m_currentPoint - 1, X, A, B);
            searchMIN(m_YvalueA, A, m_Yleft_min_index);
            searchMAX(m_YvalueA, A, m_Yleft_max_index);
            searchMIN(m_YvalueB, B, m_Yright_min_index);
            searchMAX(m_YvalueB, B, m_Yright_max_index);
        }
        if (m_currentPoint > 2)
            m_ready = true;
    }
}

void CanvasChart::push(uint64_t X, float A)
{
    m_extraData = false;
    m_Yleft_min_index = 0;
    m_Yleft_max_index = 0;
    if (m_currentPoint < m_points)
    {
        set(m_currentPoint, X, A);
        if (A < m_YvalueA[m_Yleft_min_index])
            m_Yleft_min_index = m_currentPoint;
        if (A > m_YvalueA[m_Yleft_max_index])
            m_Yleft_max_index = m_currentPoint;
        m_currentPoint++;
    }
    else
    {
        for (uint16_t i = 0; i < m_points - 1; i++)
        {
            m_Xvalue[i] = m_Xvalue[i + 1];
            m_YvalueA[i] = m_YvalueA[i + 1];
        }
        set(m_currentPoint - 1, X, A);
        searchMIN(m_YvalueA, A, m_Yleft_min_index);
        searchMAX(m_YvalueA, A, m_Yleft_max_index);
    }
    if (m_currentPoint > 2)
        m_ready = true;
}

void CanvasChart::addTable(String & updatedString, String color, uint8_t lineWidth, float minValue, float maxValue, uint8_t lPoints,
    uint16_t width, uint16_t height, uint64_t *arrayX, float *arrayY, bool rightLine)
{
    String set;
    if (rightLine)
        set = "b";
    else
        set = "a";
    updatedString += FPSTR(HTML_strokeStyle);
    updatedString += color;
    updatedString += R"(";)";
    updatedString += "\n";

    updatedString += FPSTR(HTML_SlineWidth);
    updatedString += String(lineWidth);
    updatedString += R"(;)";
    updatedString += "\n";

    updatedString += FPSTR(HTML_let);
    updatedString += R"(minY)";
    updatedString += set;
    updatedString += R"( = )";
    updatedString += String(minValue);
    updatedString += R"(;)";
    updatedString += "\n";

    updatedString += FPSTR(HTML_let);
    updatedString += R"(maxY)";
    updatedString += set;
    updatedString += R"( = )";
    updatedString += String(maxValue);
    updatedString += R"(;)";
    updatedString += "\n";

    updatedString += FPSTR(HTML_count);
    updatedString += R"(Y)";
    updatedString += set;
    updatedString += R"( = )";
    updatedString += String(lPoints);
    updatedString += R"(;)";
    updatedString += "\n";

    updatedString += FPSTR(HTML_fillText);
    updatedString += R"(")";
    if (rightLine)
    {
        updatedString += m_labelRight;
        updatedString += R"(", canv.width - ctx.measureText(")";
        updatedString += m_labelRight;
        updatedString += R"(").width)";
    }
    else
    {
        updatedString += m_labelLeft;
        updatedString += R"(", 1)";
    }
    updatedString += R"(, 20);)";
    updatedString += "\n";

    updatedString += FPSTR(HTML_DataLines_p0);
    updatedString += set;
    updatedString += FPSTR(HTML_DataLines_p1);
    updatedString += R"(((minY)";
    updatedString += set;
    updatedString += R"( + i*(maxY)";
    updatedString += set;
    updatedString += R"( - minY)";
    updatedString += set;
    updatedString += R"()/countY)";
    updatedString += set;
    updatedString += R"().toFixed( 2 ));)";
    updatedString += "\n";

    updatedString += FPSTR(HTML_fillText);
    updatedString += R"(vS, )";
    if (rightLine)
        updatedString += R"(canv.width - ctx.measureText(vS).width)";
    else
        updatedString += R"(1)";
    updatedString += R"(, )";
    updatedString += FPSTR(HTML_DataLines_canvHeight);
    updatedString += set;
    updatedString += R"());)";
    updatedString += "\n";

    updatedString += FPSTR(HTML_beginPath);
    updatedString += "\n";

    updatedString += FPSTR(HTML_moveTo);
    int lineBase;
    if (rightLine)
        lineBase = width - 20;
    else
        lineBase = 0;
    updatedString += String(lineBase);
    updatedString += R"(, )";
    updatedString += FPSTR(HTML_DataLines_canvHeight);
    updatedString += set;
    updatedString += R"( + 20));)";
    updatedString += "\n";

    updatedString += FPSTR(HTML_lineTo);
    updatedString += String(lineBase + 20);
    updatedString += R"(, )";
    updatedString += FPSTR(HTML_DataLines_canvHeight);
    updatedString += set;
    updatedString += R"( + 20));)";
    updatedString += "\n";

    updatedString += FPSTR(HTML_stroke);
    updatedString += "\n";

    updatedString += R"(})";
    updatedString += "\n";

    updatedString += FPSTR(HTML_beginPath);
    updatedString += "\n";
    for (int i = 0; i < m_currentPoint; i++)
    {
        updatedString += FPSTR(HTML_lineTo);
        updatedString += String(remap(arrayX[i], arrayX[0], arrayX[m_currentPoint - 1], 20, width - 20));
        updatedString += R"(, )";
        updatedString += String(height - remap(arrayY[i], minValue, maxValue, 20, height - 20));
        updatedString += R"();)";
        updatedString += "\n";
    }
    updatedString += FPSTR(HTML_stroke);
    updatedString += "\n";
}



String CanvasChart::getChart(uint16_t width, uint16_t height)
{
    String buffer = "";

    buffer += FPSTR(HTML_canvasH_p1);
    buffer += String(width);
    buffer += R"(" height=")";
    buffer += String(height);
    buffer += FPSTR(HTML_canvasH_p2);
    buffer += FPSTR(HTML_scriptH);
    buffer += "\n";

    buffer += FPSTR(HTML_Sfont);
    buffer += String(m_legendFontSize);
    buffer += R"(px )";
    buffer += m_legendFont;
    buffer += R"(";)";
    buffer += "\n";

    buffer += FPSTR(HTML_SlineWidth);
    buffer += String(m_lineWidth);
    buffer += R"(;)";
    buffer += "\n";

    buffer += FPSTR(HTML_count);
    buffer += R"(X = )";
    buffer += String(m_X_LegendPoints);
    buffer += R"(;)";
    buffer += "\n";


    buffer += FPSTR(HTML_var);
    buffer += R"(minT)";
    buffer += FPSTR(HTML_newDate);
    buffer += String(m_Xvalue[0]);
    buffer += R"();)";
    buffer += "\n";

    buffer += FPSTR(HTML_var);
    buffer += R"(maxT)";
    buffer += FPSTR(HTML_newDate);
    buffer += String(m_Xvalue[m_currentPoint - 1]);
    buffer += R"();)";
    buffer += "\n";

    buffer += FPSTR(HTML_TimeLines_p1);
    buffer += "\n";
    addTimeFormat(buffer);

    buffer += FPSTR(HTML_fillText);
    buffer += R"(vS, )";
    buffer += FPSTR(HTML_TimeLines_canvHeight);
    buffer += R"( + 24, canv.height - 2);)";
    buffer += "\n";

    buffer += FPSTR(HTML_beginPath);
    buffer += "\n";

    buffer += FPSTR(HTML_moveTo);
    buffer += FPSTR(HTML_TimeLines_canvHeight);
    buffer += R"( + 20, canv.height );)";
    buffer += "\n";

    buffer += FPSTR(HTML_lineTo);
    buffer += FPSTR(HTML_TimeLines_canvHeight);
    buffer += R"( + 20, canv.height - 20);)";
    buffer += "\n";

    buffer += FPSTR(HTML_stroke);
    buffer += "\n";

    buffer += R"(})";
    buffer += "\n";

    addTable(buffer, m_colorLeft, m_lineWidthLeft, m_YvalueA[m_Yleft_min_index], m_YvalueA[m_Yleft_max_index], m_Yleft_LegendPoints, width, height, m_Xvalue, m_YvalueA);
    if (m_extraData)
        addTable(buffer, m_colorRight, m_lineWidthRight, m_YvalueB[m_Yright_min_index], m_YvalueB[m_Yright_max_index], m_Yright_LegendPoints, width, height, m_Xvalue, m_YvalueB, true);

    buffer += FPSTR(HTML_endScript);

    return buffer;
}

bool CanvasChart::ready()
{
    return (m_ready);
}