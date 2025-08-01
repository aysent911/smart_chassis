#include <Arduino.h>
#include <stdbool.h>
#include <string.h>
char ESP_response[50];
char mode[] = "setup";
char command = 9;
bool is_request = false;
void setup() {
  // put your setup code here, to run once:
  DDRD |= (0x20)|(0x80);  //set PD5(P-Channel1) and PD7(N-channel2) as output
  DDRB |= (0x01)|(0x04);  //set PB0(N-Channel1) and PB2(P-Channel2) as output
  //Hand brake/parking
  PORTD &= 0xDF;          //turn off P-Channel1
  PORTB &= 0xFB;          //turn off P-Channel2
  PORTD &= 0x7F;          //turn on N-Channel2
  PORTB &= 0xFE;          //turn on N-Channel1
  uart_init();
  
  sei();  
  DDRC |= (0x04)|(0x01);           //set PC0(Power) and PC2(Brake Light) as output
  DDRD |= 0x08;                  //set PD3 as Reverse Light output
  DDRB |= 0x08;                   //set PB3 as Reverse Light output       
  PORTC |= 0x01;                  //power output
  
  delay(5000);  //wait for devices to power up
  strcpy(mode, "main");
  //turn on Brake Light
  PORTC |= 0x04;
  //turn off reverese lights
  PORTD &= 0xF7;
  PORTB &= 0xF7;
}

void loop() {
  if(is_request){
    switch(command){
      case 'A':  //NEUTRAL
        PORTD &= 0xDF;          //turn off P-Channel1
        PORTB &= 0xFB;          //turn off P-Channel2
        PORTD |= 0x80;          //turn off N-Channel2
        PORTB |= 0x01;          //turn off N-Channel1
        //turn off Brake Light
        PORTC &= 0xFB;
        //turn off reverese lights
        PORTD &= 0xF7;
        PORTB &= 0xF7;
        is_request = false;
        break;
      
      case '1':  //REVERSE
        //neutral
        PORTD &= 0xDF;          //turn off P-Channel1
        PORTB &= 0xFB;          //turn off P-Channel2
        PORTD |= 0x80;          //turn off N-Channel2
        PORTB |= 0x01;          //turn off N-Channel1
        delay(25);
        //turn off Brake Light
        PORTC &= 0xFB;
        //turn on reverese lights
        PORTD |= 0x08;
        PORTB |= 0x08;
        //reverse
        PORTD |= 0x20;          //turn on P-Channel1
        PORTB &= 0xFE;          //turn on N-Channel1
        is_request = false;
        break;
  
      case '7': //Drive 1
        //neutral
        PORTD &= 0xDF;          //turn off P-Channel1
        PORTB &= 0xFB;          //turn off P-Channel2
        PORTD |= 0x80;          //turn off N-Channel2
        PORTB |= 0x01;          //turn off N-Channel1
        delay(25);
        //turn off Brake Light
        PORTC &= 0xFB;
        //turn off reverese lights
        PORTD &= 0xF7;
        PORTB &= 0xF7;
        //Drive 1
        PORTB |= 0x04;          //turn on P-Channel2
        PORTD &= 0x7F;          //turn on N-Channel2
        is_request = false;
        break;
        
      case '9': //Hand break/parking
        //neutral
        PORTD &= 0xDF;          //turn off P-Channel1
        PORTB &= 0xFB;          //turn off P-Channel2
        PORTD |= 0x80;          //turn off N-Channel2
        PORTB |= 0x01;          //turn off N-Channel1
        delay(25);
        //turn on Brake Light
        PORTC |= 0x04;
        //turn off reverese lights
        PORTD &= 0xF7;
        PORTB &= 0xF7;
        //hand brake/parking
        PORTD &= 0x7F;          //turn on N-Channel2
        PORTB &= 0xFE;          //turn on N-Channel1
        is_request = false;
        break;
        
      default:
        delay(25);
    }
  }
  delay(50);
}
//uart receive complete ISR
ISR(USART_RX_vect){
  char received_byte = UDR0;
  if (strcmp(mode,"setup") == 0){
    if (received_byte == '\n'){
      //clear ESP_response
      for(uint8_t i = 0; i < sizeof(ESP_response);i++){
        ESP_response[i] = '\0';
      }
    }else if((received_byte>=32)&&(received_byte<128)){
      strncat(ESP_response,&received_byte, 1);  
    }
  }
  else if (strcmp(mode,"main") == 0){
    //process request
    if(((received_byte >= '1')&&(received_byte <= '9'))||(received_byte == 'A')){
      command = received_byte;
      is_request = true;
    }
  }
}
void uart_init(){
  //clear RXC, TXC
  UCSR0A &= 0x3F;
  
  //enable RXC, RX and TX
  UCSR0B=(1<<RXCIE0)|(1<<RXEN0)|(1<<TXEN0);  
 
  //8bits,1 stop bit,asynchronous mode //,even parity
  UCSR0C=(1<<UCSZ01)|(1<<UCSZ00); 
  
  //9600 baud at 16MHz, UBRR = 103
  UBRR0L=103;
}
