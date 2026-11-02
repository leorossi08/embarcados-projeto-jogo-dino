# Jogo Dino para AT89C52

Projeto acadêmico de um jogo inspirado no T-Rex, escrito em C para o
microcontrolador AT89C52. O firmware controla um LCD 16x2 em modo de 4 bits,
desenha o dinossauro e o cacto como caracteres personalizados e usa um botão
para pular.

## Conteúdo

- [`main.c`](main.c): código-fonte do firmware.

## Hardware esperado

- Microcontrolador AT89C52 (família 8051).
- LCD compatível com HD44780, 16x2, conectado ao P1 em modo de 4 bits.
- Botão de pulo no P3.4, ativo em nível baixo.
- Cristal de 11,0592 MHz, usado como referência para o atraso do firmware.

As conexões dos pinos estão definidas diretamente no início do código-fonte.
Confira também as ligações e a configuração elétrica do circuito antes de
gravar o firmware.

## Compilação e execução

O código usa extensões do compilador Keil C51 e o cabeçalho `reg51.h`; não é
um projeto C portátil para GCC. Para compilar, crie um projeto para o AT89C52
no Keil µVision, adicione `main.c` e gere o arquivo de firmware.
O código pode então ser carregado em uma montagem compatível no Proteus ou no
hardware.

Este repositório guarda apenas o código-fonte. Não inclui esquema do circuito,
projeto de simulação, ferramentas de compilação ou binário pronto.
