#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
    // Check command-line arguments
    if (argc != 2)
    {
        printf("Usage: ./recover FILE\n");
        return 1;
    }

    // Open memory card
    FILE *card = fopen(argv[1], "r");

    if (card == NULL)
    {
        printf("Could not open file.\n");
        return 1;
    }

    // Buffer for one block
    uint8_t buffer[512];

    // Keep track of JPEG files
    int jpeg_count = 0;

    // Current JPEG file
    FILE *image = NULL;

    // Read card block by block
    while (fread(buffer, 1, 512, card) == 512)
    {
        // Check if this block starts a JPEG
        if (buffer[0] == 0xff &&
            buffer[1] == 0xd8 &&
            buffer[2] == 0xff &&
            (buffer[3] & 0xf0) == 0xe0)
        {
            // Close previous JPEG if one is already open
            if (image != NULL)
            {
                fclose(image);
            }

            // Create filename
            char filename[8];

            sprintf(filename, "%03i.jpg", jpeg_count);

            // Open new JPEG
            image = fopen(filename, "w");

            if (image == NULL)
            {
                fclose(card);
                return 1;
            }

            jpeg_count++;
        }

        // If we're inside a JPEG, write the block
        if (image != NULL)
        {
            fwrite(buffer, 1, 512, image);
        }
    }

    // Close files
    if (image != NULL)
    {
        fclose(image);
    }

    fclose(card);

    return 0;
}