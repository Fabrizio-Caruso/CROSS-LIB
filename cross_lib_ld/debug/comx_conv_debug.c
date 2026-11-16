

#define BASE_ADDR 0xF800

typedef unsigned char uint8_t;
typedef unsigned short uint16_t;

void main(){

    uint16_t foo;
    uint16_t bar = 42;
    
    foo = (uint16_t) BASE_ADDR + (uint16_t) bar;
}


