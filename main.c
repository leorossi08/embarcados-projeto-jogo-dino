#include <reg51.h>

sfr LCD_Port = 0x90; /* P1 port as data port */
sbit rs = P1 ^ 3; /* Register select pin */
sbit rw = P1 ^ 0; /* Read/Write pin */
sbit en = P1 ^ 2; /* Enable pin */
sbit botao = P3 ^ 4; /* Botão de pulo do dino */

unsigned char dinossauro_pulando = 0; // Indica se o dinossauro está pulando
unsigned char altura_pulo = 0; // Altura atual do pulo do dinossauro
unsigned int score = 0; // Pontuação
unsigned int velocidade = 100; // Velocidade inicial do jogo

/* Function to provide delay Apprx 1ms with 11.0592 MHz crystal */
void delay(unsigned int count)
{
    int i, j;
    for(i = 0; i < count; i++)
        for(j = 0; j < 112; j++);
}

void LCD_Char(char char_data) /* LCD data write function */
{
    LCD_Port = (LCD_Port & 0x0F) | (char_data & 0xF0); /* Send upper nibble */
    rs = 1; /* Data reg. */
    rw = 0; /* Write operation */
    en = 1;
    delay(1);
    en = 0;
    delay(2);

    LCD_Port = (LCD_Port & 0x0F) | (char_data << 4); /* Send lower nibble */
    en = 1;
    delay(1);
    en = 0;
    delay(5);
}

void LCD_Command(char cmnd) /* LCD 16x2 command function */
{
    LCD_Port = (LCD_Port & 0x0F) | (cmnd & 0xF0); /* Send upper nibble */
    rs = 0; /* Command reg. */
    rw = 0; /* Write operation */
    en = 1;
    delay(1);
    en = 0;
    delay(2);

    LCD_Port = (LCD_Port & 0x0F) | (cmnd << 4); /* Send lower nibble */
    en = 1; /* Enable pulse */
    delay(1);
    en = 0;
    delay(5);
}

void LCD_String(char* str) /* Send string to LCD function */
{
    int i;
    for(i = 0; str[i] != 0; i++) /* Send each char of string till the NULL */
        LCD_Char(str[i]); /* Call LCD data write */
}

void LCD_String_xy(char row, char pos, char *str)
{
    if(row == 0)
        LCD_Command((pos & 0x0F) | 0x80);
    else if (row == 1)
        LCD_Command((pos & 0x0F) | 0xC0);
    LCD_String(str); /* Call LCD string function */
}

void LCD_Init(void) /* LCD Initialize function */
{
    delay(20); /* LCD Power ON Initialization time >15ms */
    LCD_Command(0x02); /* 4 bit mode */
    LCD_Command(0x28); /* Initialization of 16X2 LCD in 4bit mode */
    LCD_Command(0x0C); /* Display ON Cursor OFF */
    LCD_Command(0x06); /* Auto Increment cursor */
    LCD_Command(0x01); /* Clear display */
    LCD_Command(0x80); /* Cursor at home position */
}


void LCD_CustomChar(unsigned char loc, unsigned char *customChar)
{
    unsigned char i;
    
    // Command to set CGRAM address
    LCD_Command(0x40 + (loc * 8)); // 0x40 is the base address for CGRAM
    
    for(i = 0; i < 8; i++)
    {
        LCD_Char(customChar[i]); // Write each row of the custom character
    }
}

void pular_dino() {
    if (!dinossauro_pulando) {
        dinossauro_pulando = 1;
        altura_pulo = 1;
    }
}

void atualizar_dino() {
    if (dinossauro_pulando) {
        if (altura_pulo >= 2) {
            dinossauro_pulando = 0;
            altura_pulo = 0;
        } else {
            altura_pulo++;
        }
    }
}

void int_to_str(unsigned int num, char* str)
{
    unsigned char i = 0;
    unsigned char j;
    unsigned char temp;
    
    // Handle 0 explicitly
    if (num == 0) {
        str[i++] = '0';
        str[i] = '\0';
        return;
    }
    
    // Convert integer to string
    while (num != 0) {
        str[i++] = (num % 10) + '0';
        num /= 10;
    }
    
    // Reverse the string
    str[i] = '\0';
    for (j = 0; j < i / 2; j++) {
        temp = str[j];
        str[j] = str[i - j - 1];
        str[i - j - 1] = temp;
    }
}

void atualizar_score()
{
    char score_str[10];
    int_to_str(score, score_str); // Converte o score para string
    LCD_String_xy(0, 0, "Score:"); // Exibe a pontuação
    LCD_String_xy(0, 7, "     "); // Limpa dígitos anteriores
    LCD_String_xy(0, 7, score_str);
}

void main(void)
{
    int pos;
    unsigned char cacto[8] = {0x04, 0x15, 0x15, 0x1D, 0x07, 0x04, 0x04, 0x04};
    unsigned char dino[8] = {0x01, 0x02, 0x06, 0x0E, 0x1E, 0x0E, 0x0A, 0x00};
    unsigned char botao_segurado = 0;

    LCD_Init();
    LCD_CustomChar(0, cacto);
    LCD_CustomChar(1, dino);
    atualizar_score();

    while(1) {
        for(pos = 15; pos >= 0; pos--) {
            if (botao == 0) {
                if (!botao_segurado) {
                    pular_dino();
                }
                botao_segurado = 1;
            } else {
                botao_segurado = 0;
            }

            if (pos < 15) {
                LCD_Command(0xC0 + (pos + 1)); // Move para a posição anterior do cacto
                LCD_Char(' '); // Apaga o cacto anterior
            }
            
            if (dinossauro_pulando) {
                LCD_Command(0xC0 + 1);
                LCD_Char(' '); // Limpa posição do dinossauro na linha 1
                LCD_Command(0x80 + 1);
            } else {
                LCD_Command(0x80 + 1);
                LCD_Char(' '); // Limpa posição do dinossauro na linha 2
                LCD_Command(0xC0 + 1);
            }
            
            LCD_Char(1); // Desenha o dinossauro
            LCD_Command(0xC0 + pos);
            LCD_Char(0); // Desenha o cacto
            
            delay(velocidade);

            if (pos == 1) {
                if (!dinossauro_pulando) {
                    LCD_Command(0x01);
                    LCD_String_xy(0, 4, "Game Over");
                    while(1);
                }
            }

            if (pos == 0) {
                LCD_Command(0xC0);
                LCD_Char(' ');
                score++;
                atualizar_score();

                if (score % 5 == 0 && velocidade > 50) {
                    velocidade -= 10;
                }
            }

            atualizar_dino();
        }
    }
}
