// 参考
// https://t49.hatenablog.jp/entry/2022/12/26/004959

#include <Arduino.h>
#include <SPI.h>

#define USING_16BIT 0 // 16bitモードの場合は1にするが、動作しなかった

void setup()
{
    Serial.begin(115200);

    Serial.println("setup");

    delay(3000);

    Serial.println("booting");

    SPI.setRX(0);
    SPI.setCS(1);
    SPI.setSCK(2);
    SPI.setTX(3);

    SPI.begin();

    Serial.println("software reset");

    SPI.beginTransaction(SPISettings(1000000, MSBFIRST, SPI_MODE0));

    // ソフトウェアリセット
    SPI.transfer(0xFF);
    SPI.transfer(0xFF);
    SPI.transfer(0xFF);
    SPI.transfer(0xFF);

    SPI.endTransaction();

#if USING_16BIT
    SPI.beginTransaction(SPISettings(1000000, MSBFIRST, SPI_MODE0));

    // 16bitモード
    SPI.transfer(0x02);
    SPI.transfer(0x80);

    SPI.endTransaction();
#else
    // SPI.transfer(0x02);
    // SPI.transfer(0x00);
#endif

    SPI.beginTransaction(SPISettings(1000000, MSBFIRST, SPI_MODE0));

    // 連続読み取りモード
    SPI.transfer(0x54);

    delay(500);
}

void loop()
{
    Serial.println("loop");
    uint16_t uiVal;
    float fVal;
    int32_t iVal;

    SPI.beginTransaction(SPISettings(1000000, MSBFIRST, SPI_MODE0));

    uiVal = (uint16_t)SPI.transfer(0) << 8; // AD変換値 上位
    uiVal |= SPI.transfer(0);               // AD変換値 下位

    Serial.printf("uiVal: %04X\r\n", uiVal);

    SPI.endTransaction();

#if USING_16BIT
    // 16bit
    if (uiVal & 0x1000)
    {
        iVal = uiVal - 0x10000;
    }
    else
    {
        iVal = uiVal;
    }
    fVal = (float)iVal / 128.0;
#else
    // 13bit
    uiVal = uiVal >> 3;

    if (uiVal & 0x1000)
    {
        iVal = uiVal - 0x2000;
    }
    else
    {
        iVal = uiVal;
    }
    fVal = (float)iVal / 16.0;
#endif
    Serial.println(fVal, 4);

    delay(1000);
}