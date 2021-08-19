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


#ifndef CanvasChart_h
#define CanvasChart_h

#include <Arduino.h>

class CanvasChart
{
private:
    uint16_t m_points;
    uint16_t m_currentPoint = 0;

    uint64_t* m_Xvalue;
    float* m_YvalueA;
    float* m_YvalueB;

    uint16_t m_Yleft_min_index;
    uint16_t m_Yleft_max_index;
    uint16_t m_Yright_min_index;
    uint16_t m_Yright_max_index;

    String m_legendFont = "serif";
    uint8_t m_legendFontSize = 12;
    uint8_t m_X_LegendPoints = 5;
    uint8_t m_Yleft_LegendPoints = 2;
    uint8_t m_Yright_LegendPoints = 2;
    String m_colorLeft = "red";
    String m_colorRight = "green";
    uint8_t m_lineWidthLeft = 2;
    uint8_t m_lineWidthRight = 2;
    String m_labelLeft;
    String m_labelRight;
    uint8_t m_lineWidth = 1;
    String m_timeFormat = "";
    bool m_extraData = false;
    bool m_ready = false;

    

    void set(uint16_t position, uint64_t X, float A, float B);
    void set(uint16_t position, uint64_t X, float A);
    void searchMIN(float* array, float newData, uint16_t & index);
    void searchMAX(float* array, float newData, uint16_t & index);
    uint16_t remap(float X, float in_min, float in_max, uint16_t out_min, uint16_t out_max);
    uint16_t remap(uint16_t X, uint16_t in_min, uint16_t in_max, uint16_t out_min, uint16_t out_max);
    uint16_t remap(uint64_t X, uint64_t in_min, uint64_t in_max, uint16_t out_min, uint16_t out_max);
    void updateTimeStrings(String & valueS, String & commandS, String & abbrData, String Data);
    void addTimeFormat(String & updatedString);
    void addTable(String & updatedString, String color, uint8_t lineWidth, float minValue, float maxValue, uint8_t lPoints,
        uint16_t width, uint16_t height, uint64_t* arrayX, float* arrayY, bool rightLine = false);

public:

    CanvasChart(uint16_t points, bool extraData = false);
    ~CanvasChart();
    void push(uint64_t X, float A);
    void push(uint64_t X, float A, float B);
    void setLegendFont(String font, uint8_t size);
    void setXLegendPoints(uint8_t intervale);
    void setYLegendPoints(uint8_t intervaleLeft, uint8_t intervaleRight);
    void setYLegendPoints(uint8_t intervaleLeft);
    void setLeftYstyle(String lineColor, uint8_t lineWidth, String label = "");
    void setRightYstyle(String lineColor, uint8_t lineWidth, String label = "");
    void setTimeFormat(String TimeFormat);
    String getChart(uint16_t width, uint16_t height);
    bool ready();

};

#endif