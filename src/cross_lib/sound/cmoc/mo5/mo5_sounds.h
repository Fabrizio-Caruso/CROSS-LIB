

/*
LDA   <$C0          ; PIA systeme
  ANDA  #$FB          ; clear mute bit
  STA   <$C0          ; modification PIA
  LDA   <$CF          ; lecture registre de ctrl B
  ANDA  #$FB          ; raz bit 2
  STA   <$CF          ; selection DDRB
  LDB   #$3F          ; set bits 0-5
  STB   <$CD          ; bits CNA en sortie
  ORA   #$04          ; set b2
  STA   <$CF          ; selection PB
Inviato da Dino Oggi alle ore 15:06
Come si inizializza il registro CNA? Io nella versione "semplice" (non mi preoccupo di ripristinare lo stato in uscita), faccio (DP = $ E7 per TO e $ A7 per MO):
CODIFICATO: SELEZIONA TUTTO

  LDA   <$C0          ; PIA systeme
  ANDA  #$FB          ; clear mute bit
  STA   <$C0          ; modification PIA
  LDA   <$CF          ; lecture registre de ctrl B
  ANDA  #$FB          ; raz bit 2
  STA   <$CF          ; selection DDRB
  LDB   #$3F          ; set bits 0-5
  STB   <$CD          ; bits CNA en sortie
  ORA   #$04          ; set b2
  STA   <$CF          ; selection PB
Un'altra cosa: hai bloccato le interruzioni all'inizio del tuo programma (ORCC # $ 50), altrimenti disturberanno il tuo tempismo e confonderanno il tuo ascolto : lol:
*/

#ifndef  __MO5_SOUNDS
#define  __MO5_SOUNDS

#define BEEP_ASCII() PUTCH(7)

#define EXPLOSION_SOUND() BEEP_ASCII()

#define ZAP_SOUND() BEEP_ASCII()

#define SHOOT_SOUND() BEEP_ASCII()

#define PING_SOUND() BEEP_ASCII()

#define TICK_SOUND() BEEP_ASCII()

#define TOCK_SOUND() BEEP_ASCII()

#endif // __MO5_SOUNDS

