#include <stdint.h>
#include <stdio.h>
#include <string.h>

int main(int argc, char* argv[])
{
    const uint8_t sync[] = {
            1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0x3C,0x5A
    };
    while(--argc > 0)
    {
        uint8_t l;
        char* path = argv[argc];
        FILE *in, *out;
        in = fopen(path, "rb");
        out = fopen("FILE.K5", "wb");

        // HEADER BLOCK
        fwrite(sync, 1, sizeof(sync), out);

        uint8_t head[17];
        l = 0;
        head[l++] = 0; head[l++] = 16;
        for(int i = 0; l < 13;)
        {
                if(i >= strlen(path)) break;
                if(path[i] == '.') i++;
                if(path[i] == '/') {
                        l = 2;
                        i++;
                        continue;
                }
                head[l++] = path[i++];
        }

        while(l < 13) head[l++] = ' ';

        head[l++] = 2;
        head[l++] = 0;
        head[l++] = 0;
        head[l] = 0;

        for(int i = 2; i < l; i++)
                head[l] += head[i];
        head[l] = -head[l];
        l++;
        fwrite(head, 1, 17, out);

        // DATA BLOCKS
        for(;;)
        {
                uint8_t buf[254];
                l = fread(buf, 1, 254, in);
                if(l <= 0) break;

                fwrite(sync, 1, sizeof(sync), out);
                head[0] = 1;
                head[1] = l + 2;
                fwrite(head, 1, 2, out);
                fwrite(buf, 1, l, out);

                head[0] = 0;
                while(l > 0)
                        head[0] -= buf[--l];
                fwrite(head, 1, 1, out);
        }

        // FOOTER BLOCK
        fwrite(sync, 1, sizeof(sync), out);
        head[0] = 255;
        head[1] = 2;
        head[2] = 0;
        fwrite(head, 1, 3, out);

        fclose(in);
        fclose(out);

    }
}
