#include "bmp.h"
#include <cstdlib>
#include <ctime>

BMP::BMP(DIBType dibType, unsigned int width, unsigned int height)
{
    BMP::deviceIndependentBitmapHeader = (unsigned int)dibType;
    BMP::SetImageSize(width, height);

    BMP::fileDataLength = BMP::bitmapFileHeader + BMP::deviceIndependentBitmapHeader + BMP::pixelArray;
    BMP::fileData = new char[BMP::fileDataLength];

    BMP::bitmapFileLength = BMP::fileDataLength;

    BMP::InitializeHeaderPortion();
    BMP::InitializeDeviceIndependentBitmapHeaderPortion();
}

void BMP::SetImageSize(unsigned int width, unsigned int height)
{
    BMP::width = width;
    BMP::height = height;

    unsigned int padding = (4 - ((BMP::width * BMP::bytesPerPixel) % 4)) % 4;
    unsigned int rowSize = (BMP::width * BMP::bytesPerPixel) + padding;

    BMP::pixelArray = rowSize * height;
}

void BMP::InitializeHeaderPortion()
{
    //First 2 bytes are reserved for BM
    fileData[0] = 0x42; //ASCII 'B'
    fileData[1] = 0x4D; //ASCII 'M
    //Next 4 bytes tell us the file size
    fileData[2] = (char)(BMP::fileDataLength & 0xFF);
    fileData[3] = (char)((BMP::fileDataLength>> 8) & 0xFF);
    fileData[4] = (char)((BMP::fileDataLength >> 16) & 0xFF);
    fileData[5] = (char)((BMP::fileDataLength >> 24) & 0xFF);
    //Next 4 bytes can be set to 0
    fileData[6] = 0x00;
    fileData[7] = 0x00;
    fileData[8] = 0x00;
    fileData[9] = 0x00;
    //Next 4 bytes tell us the offset to where the pixel array is located, NOTE: The offset may change depending on DIBType and color tables etc
    unsigned int offset = BMP::bitmapFileHeader + BMP::deviceIndependentBitmapHeader;
    fileData[10] = (char)(offset & 0xFF);
    fileData[11] = (char)((offset >> 8) & 0xFF);
    fileData[12] = (char)((offset >> 16) & (0xFF));
    fileData[13] = (char)((offset >> 24) & (0xFF));
}

void BMP::InitializeDeviceIndependentBitmapHeaderPortion()
{
    //First 4 bytes are the size of the header portion
    fileData[14] = (char)(0x28 & 0xFF);
    fileData[15] = (char)((0x28 >> 8) & 0xFF);
    fileData[16] = (char)((0x28 >> 16) & 0xFF);
    fileData[17] = (char)((0x28 >> 24) & 0xFF);
    //Next 4 bytes are the bitmap width in pixels
    fileData[18] = (char)(width & 0xFF);
    fileData[19] = (char)((width >> 8) & 0xFF);
    fileData[20] = (char)((width >> 16) & 0xFF);
    fileData[21] = (char)((width >> 24) & 0xFF);
    //Next 4 bytes are the bitmap height in pixels
    fileData[22] = (char)(height & 0xFF);
    fileData[23] = (char)((height >> 8) & 0xFF);
    fileData[24] = (char)((height >> 16) & 0xFF);
    fileData[25] = (char)((height >> 24) & 0xFF);
    //Next 2 bytes are the number of color planes (must be 1)
    fileData[26] = (char)(0x01 & 0xFF);
    fileData[27] = (char)((0x01 >> 8) & 0xFF);
    //Next 2 bytes are the number of bits per pixel, typical values (1,4,8,16,24,32)
    fileData[28] = (char)(bitsPerPixel & 0xFF);
    fileData[29] = (char)((bitsPerPixel >> 8) & 0xFF);
    //Next 4 bytes are the compression method used, in our case we use no compression
    fileData[30] = (char)(0x00 & 0xFF);
    fileData[31] = (char)((0x00 >> 8) & 0xFF);
    fileData[32] = (char)((0x00 >> 16) & 0xFF);
    fileData[33] = (char)((0x00 >> 24) & 0xFF);
    //The next 4 bytes are the image size
    unsigned int imageSize = (width * height * bitsPerPixel) / 8;
    fileData[34] = (char)(imageSize & 0xFF);
    fileData[35] = (char)((imageSize >> 8) & 0xFF);
    fileData[36] = (char)((imageSize >> 16) & 0xFF);
    fileData[37] = (char)((imageSize >> 24) & 0xFF);
    //The next 4 bytes are the horizontal resolution of the image [pixel per metre]
    fileData[38] = (char)(resolution & 0xFF);
    fileData[39] = (char)((resolution >> 8) & 0xFF);
    fileData[40] = (char)((resolution >> 16) & 0xFF);
    fileData[41] = (char)((resolution >> 24) & 0xFF);
    //The next 4 bytes are the veritcal resolution of the image [pixel per metre]
    fileData[42] = (char)(resolution & 0xFF);
    fileData[43] = (char)((resolution >> 8) & 0xFF);
    fileData[44] = (char)((resolution >> 16) & 0xFF);
    fileData[45] = (char)((resolution >> 24) & 0xFF);
    //The next 4 bytes are the numbers of colors in the color palette, if set to 0 it defaults to 2^n
    fileData[46] = (char)(0x00 & 0xFF);
    fileData[47] = (char)((0x00 >> 8) & 0xFF);
    fileData[48] = (char)((0x00 >> 16) & 0xFF);
    fileData[49] = (char)((0x00 >> 24) & 0xFF);
    //The next 4 bytes are the number of important colors used, (will be set to 0 in our case)
    fileData[50] = (char)(0x00 & 0xFF);
    fileData[51] = (char)((0x00 >> 8) & 0xFF);
    fileData[52] = (char)((0x00 >> 16) & 0xFF);
    fileData[53] = (char)((0x00 >> 24) & 0xFF);
}

void BMP::CreateRandomImage()
{
    std::srand(std::time(NULL));

    int pixelArrayOffset = (int)(BMP::bitmapFileHeader + BMP::deviceIndependentBitmapHeader);

    for (int i = pixelArrayOffset; i < BMP::fileDataLength; i += 3)
    {
        fileData[i] = (char)(std::rand() % 256);
        fileData[i+1] = (char)(std::rand() % 256); // Fixed the typo here
        fileData[i+2] = (char)(std::rand() % 256);
    }
}

char* BMP::ReturnData()
{
    return BMP::fileData;
}