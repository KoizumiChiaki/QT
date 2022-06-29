#ifndef _QTHWK_GAMEMAP_H_
#define _QTHWK_GAMEMAP_H_

#include "constants.h"
#include <QImage>
#include <QPixmap>
#include <QPainter>
#include <QDebug>

#include <cstdio>
#include <string>

namespace __gameMap
{
    using std::string;

    const string dir1 = "..\\QT\\resources\\images\\base\\";
    const string dir2 = "..\\QT\\resources\\images\\environment\\";
    const string dir3 = "..\\QT\\resources\\images\\background\\";
    const string themeName[6] = { "desert", "end", "mountains", "nether", "ocean", "plain" };
    enum theme { desert, the_end, mountains, nether, ocean, plain } nowTheme;
    enum block { empty, solid, liquid, platform };

    // 48 * 36 screen
    void setTheme(theme x){nowTheme = x;}

    class gameMapStructure
    {
    private:
        string source;
        int map[screenHeight][screenWidth];
        theme themeType;
        block sta[screenHeight][screenWidth];
        const enum block typ[25] = {
        empty, solid, empty, solid, solid, empty, liquid, liquid, solid, solid,
        liquid, solid, solid, solid, solid, solid, solid, solid, empty, liquid,
        solid };
    public:
        void mapInit() //load the map
        {
            themeType = nowTheme;
            source = dir1 + themeName[(unsigned int)nowTheme] + ".txt";
            FILE* Input = fopen(source.c_str(), "r");
            for (int i = screenHeight - 1; i >= 0; i--)
            {
                int tmp;
                for (int j = 0; j < screenWidth; j++)
                {
                    fscanf(Input, "%d", &tmp);
                    map[i][j] = tmp;
                    sta[i][j] = typ[map[i][j]];
                }
            }
        }
        enum block getBlockType(int x, int y) //(x, y)
        {
            if (x < 0 || x >= screenWidth || y < 0 || y >= screenHeight)
                return solid;
            return sta[y][x];//0:empty, 1:solid, 2:liquid, 3:platform
        }
        QImage getWholeMapSolid() //print the whole map
        {
            QImage ret(screenWidth * 16, screenHeight * 16, QImage::Format_RGBA8888);
            QPixmap tmp((dir3 + std::to_string((unsigned int)themeType) + ".png").c_str());
            QPainter painter(&ret);
            painter.drawPixmap(0, 0, screenWidth * 16, screenHeight * 16, tmp);
            for (int i = 0; i < screenHeight; i++)
            {
                for (int j = 0; j < screenWidth; j++)
                {
                    if (sta[i][j] != liquid)
                    {
                        tmp.load((dir2 + std::to_string(map[i][j]) + ".png").c_str());
                        painter.drawPixmap(j * 16, (screenHeight - 1 - i) * 16, 16, 16, tmp);
                    }
                }
            }
            return ret;
        }
        QImage getWholeMapLiquid() //print the whole map
        {
            QImage ret(screenWidth * 16, screenHeight * 16, QImage::Format_RGBA8888);
            QPixmap tmp;
            QPainter painter(&ret);
            painter.setCompositionMode(QPainter::CompositionMode_Source);
            for (int i = 0; i < screenHeight; i++)
            {
                for (int j = 0; j < screenWidth; j++)
                {
                    if (sta[i][j] == liquid)
                    {
                        tmp.load((dir2 + std::to_string(map[i][j]) + ".png").c_str());
                        painter.drawPixmap(j * 16, (screenHeight - 1 - i) * 16, 16, 16, tmp);
                    }
                }
            }
            painter.setCompositionMode(QPainter::CompositionMode_DestinationIn);
            painter.fillRect(ret.rect(), QColor(0, 0, 0, 170));
            return ret;
        }
    }gameMap;
}
using __gameMap::setTheme;
using __gameMap::gameMap;
using __gameMap::desert;
using __gameMap::the_end;
using __gameMap::mountains;
using __gameMap::nether;
using __gameMap::ocean;
using __gameMap::plain;
using __gameMap::empty;
using __gameMap::solid;
using __gameMap::liquid;
using __gameMap::platform;

#endif // _QTHWK_GAMEMAP_H_
