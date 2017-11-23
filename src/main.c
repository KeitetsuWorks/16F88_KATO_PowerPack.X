/**
 * @file        main.c
 * @brief       Model Railway Controller
 * @author      Keitetsu
 * @date        2014/02/11
 * @copyright   Copyright (c) 2014 Keitetsu
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <xc.h>

#define _XTAL_FREQ 10000000 // 10MHz

#define PR2_DATA 0x7F
#define T2_DIV_BY_1 0b00000000
#define T2_DIV_BY_16 0b00000010
#define CCP_PWM 0b00001100

// 16F88
#pragma config BOREN = ON, CPD = OFF, CCPMX = RB3, DEBUG = OFF, WRT = OFF, FOSC = HS, MCLRE = ON, WDTE = OFF, CP = OFF, LVP = OFF, PWRTE = ON

void setADCChannel(unsigned char channel);
unsigned int readADCValue();
void initPWM1(unsigned char pr2, unsigned char t2ckps);
void setPWM1Duty(unsigned int duty);

void main()
{
    static bit freq_mode;
    unsigned char light, rate;
    unsigned int speed, duty1;
    
    PORTA = 0x00;           // PORTAを初期化
    PORTB = 0x00;           // PORTBを初期化
    
    TRISA = 0b00000111;     // PORTAの入出力設定
    TRISB = 0b00000000;     // PORTBの入出力設定
    
    ANSEL = 0b00000011;     // AN0, AN1を有効化
    ADCON0 = 0b01000001;    // Fosc/16, ADON
    ADCON1 = 0b11000000;    // 右詰め, AVdd, AVss
    
    freq_mode = 0;          // 制御変数を初期化
    light = 0;
    speed = 0;
    rate = 0;
    duty1 = 0;
    
    initPWM1(PR2_DATA, T2_DIV_BY_1);    // CCP1をPWMモードに設定
    setPWM1Duty(duty1);                 // デューティ値を初期化（0-511）

    while(1){
        if(RA2 != freq_mode){   // PWM周波数切替スイッチを確認
            if(RA2 == 0){       // 20kHz
                initPWM1(PR2_DATA, T2_DIV_BY_1);
                freq_mode = 0b0;
            }
            else{               // 1kHz
                initPWM1(PR2_DATA, T2_DIV_BY_16);
                freq_mode = 0b1;
            }
        }
        
        setADCChannel(1);               // A/D変換対象チャンネルを設定
        __delay_us(30);                 // 30マイクロ秒タイマ
        light = (readADCValue() >> 3);  // 常点灯設定値（7bitに縮小）
        
        setADCChannel(0);               // A/D変換対象チャンネルを設定
        __delay_us(30);                 // 30マイクロ秒タイマ
        speed = (readADCValue() << 6);  // 速度設定値（16bitに拡張）

        rate = 65535 / (511 - light);   // 速度設定値1段階当たりのデューティ比の増分を計算
        duty1 = light + speed / rate;   // デューティ比を計算

        if(duty1 > 511){        // デューティ比が最大値を超過した場合
            duty1 = 511;
        }
        
        setPWM1Duty(duty1);     // デューティ比を設定
    }
}

void setADCChannel(unsigned char channel)
{
    ADCON0 &= 0b11000111;       // CHS<2:0>を初期化
    ADCON0 |= (channel << 3);
}

unsigned int readADCValue()
{
    GO_nDONE = 0b1;
    while(GO_nDONE);
    return ((ADRESH << 8) + ADRESL);
}

void initPWM1(unsigned char pr2, unsigned char t2ckps)
{
    PR2 = pr2;
    CCP1CON = CCP_PWM;
    setPWM1Duty(0);
    T2CON = t2ckps;
    T2CON |= 0x04;
}

void setPWM1Duty(unsigned int duty)
{
    CCPR1L = duty >> 2;
    CCP1CON &= 0b11001111;
    CCP1CON |= 0b00110000 & (duty << 4);
}
