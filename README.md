O loop do sistema se inicia dando digitalRead no LDR. caso o LDR seja maior que o threshold, ele ficara em modo noturno isso, isso é. o led amarelo piscará a cada 1 segundo.

Caso o LDR fique de dia, existe um switch case para alteranr estados, que possuem propriedades para ligar e desligar os leds.![image](https://github.com/user-attachments/assets/fe9185b3-6465-45eb-b57f-2d88643b1a9a)

Além disso, foi inplementado um botão com debounce de dalay com 200milisegundos que quando é apertado classifica um booleano como verdadeiro. Quando esse booleano é verdadeiro ele conta 1 segundo com millis e passa pro estado de sinal aberto caso esteja no estado de sinal fechado.

Caso esse botão seja apertado 3x ele faz uma requisião get para o google. e reseta a contagem, passando para o proximo estado.
