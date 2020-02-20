#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "bmp.h"

void metadata(BITMAPFILEHEADER bf, BITMAPINFOHEADER bi, FILE *output, float n, int nPadding, int nWidth, int nHeight);

int main(int argc, char *argv[])
{
    // ensure proper usage
    if (argc != 4)
    {
        fprintf(stderr, "Usage: ./resize n infile outfile\n");
        return 1;
    }

    // get positive float n <= 100 by which the image is to be enlarged
    char *factor = argv[1];
    float n;
    sscanf(factor, "%f", &n);
    if (n <= 0 || n >= 100)
    {
        return 5;
    }
    printf("%f\n", n);

    // create pointers to command line arguments
    char *input = argv[2];
    char *output = argv[3];

    // open input file
    FILE *inptr = fopen(input, "r");
    if (inptr == NULL)
    {
        fprintf(stderr, "Could not open %s.\n", input);
        return 2;
    }

    // open output file
    FILE *outptr = fopen(output, "w");
    if (outptr == NULL)
    {
        fclose(inptr);
        fprintf(stderr, "Could not create %s.\n", output);
        return 3;
    }

    // read infile's BITMAPFILEHEADER - FREAD -- read "1" data block, of "sizeof" no. of bytes, from inptr, and stores the data block in datastruct bf of type BITMAPFILEHEADER.
    BITMAPFILEHEADER bf;
    fread(&bf, sizeof(BITMAPFILEHEADER), 1, inptr);

    // read infile's BITMAPINFOHEADER
    BITMAPINFOHEADER bi;
    fread(&bi, sizeof(BITMAPINFOHEADER), 1, inptr);

    // ensure infile is (likely) a 24-bit uncompressed BMP 4.0 - check validity of infile's metadata
    if (bf.bfType != 0x4d42 || bf.bfOffBits != 54 || bi.biSize != 40 ||
        bi.biBitCount != 24 || bi.biCompression != 0)
    {
        fclose(outptr);
        fclose(inptr);
        fprintf(stderr, "Unsupported file format.\n");
        return 4;
    }

    //determine new (pixel) height & width of image - rounded to nearest integer
    int nHeightActual = bi.biHeight * n;
    int nHeight = fabsf(bi.biHeight * n);
    int nWidth = bi.biWidth * n;
    printf("%d\n", nHeight);
    printf("%d\n", nWidth);

    // determine padding for scanlines (formula for padding - don't think too much just use it lol)
    int oPadding = (4 - (bi.biWidth * sizeof(RGBTRIPLE)) % 4) % 4;

    int nPadding = (4 - (nWidth * sizeof(RGBTRIPLE)) % 4) % 4;

    //function to write (updated) metadata to outfile
    metadata(bf, bi, outptr, n, nPadding, nWidth, nHeightActual);

    // if image is being enlarged
    if (n > 1 && n < 100)
    {
        // iterate over infile's scanlines
        for (int i = 0, biHeight = abs(bi.biHeight); i < biHeight; i++)
        {
            RGBTRIPLE *scanline = malloc(nWidth * sizeof(RGBTRIPLE));
            int d = 0;

            // iterate over pixels in scanline
            for (int j = 0; j < bi.biWidth; j++)
            {
                // temp storage
                RGBTRIPLE triple;

                // read RGBTRIPLE from infile
                fread(&triple, sizeof(RGBTRIPLE), 1, inptr);

                // copy each pixel(ie RGBTRIPLE) 'n' times to scanline malloc
                for (int c = 0; c < n; c++)
                {
                    scanline[c + d] = triple;
                }
                d += n;
            }

            // write scanline malloc to outfile n times (aka scale vertically)
            for (int b = 0; b < n; b++)
            {
                fwrite(scanline, nWidth * sizeof(RGBTRIPLE), 1, outptr);
                //add in padding for each scanline
                for (int k = 0; k < nPadding; k++)
                {
                    fputc(0x00, outptr);
                }
            }
            // skip file position indicator over infile padding, if any
            fseek(inptr, oPadding, SEEK_CUR);

            // lastly, free
            free(scanline);
        }
    }

    else //ie if image is being shrunk
    {
    // determine skip factor
    int skip = 1/n - 1;
        // iterate over (new) scanlines
        for (int i = 0, biHeight = nHeight; i < biHeight; i++)
        {
            RGBTRIPLE *scanline = malloc(nWidth * sizeof(RGBTRIPLE));
            // iterate over (new) pixels in scanline
            for (int j = 0; j < nWidth; j++)
            {
                // temp storage
                RGBTRIPLE triple;

                // read RGBTRIPLE from infile && skip pixels by factor 'skip'
                fread(&triple, sizeof(RGBTRIPLE), 1, inptr);
                fseek(inptr, skip * sizeof(RGBTRIPLE), SEEK_CUR);

                // write RGBTRIPLE to outfile
                scanline[j] = triple;
            }

            //write scanline malloc to outfile
            fwrite(scanline, nWidth * sizeof(RGBTRIPLE), 1, outptr);

            //add in padding for each scanline
            for (int k = 0; k < nPadding; k++)
            {
                fputc(0x00, outptr);
            }

            // skip file position indicator over infile padding && by 'skip' no. of scanlines
            fseek(inptr, oPadding + skip * (bi.biWidth * sizeof(RGBTRIPLE) + oPadding), SEEK_CUR);

            // lastly, free
            free(scanline);
        }
    }


    // close infile
    fclose(inptr);

    // close outfile
    fclose(outptr);

    // success
    return 0;
}

//metadata updating function
void metadata(BITMAPFILEHEADER bf, BITMAPINFOHEADER bi, FILE *outptr, float n, int nPadding, int nWidth, int nHeight)
{
    // Update individual metadata values
    bi.biWidth = nWidth;
    bi.biHeight = nHeight;
    bi.biSizeImage = (bi.biWidth * sizeof(RGBTRIPLE) + nPadding) * abs(bi.biHeight);
    bf.bfSize = bi.biSizeImage + sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);

    // Write updated metadata to outptr
    fwrite(&bf, sizeof(BITMAPFILEHEADER), 1, outptr);
    fwrite(&bi, sizeof(BITMAPINFOHEADER), 1, outptr);
}
