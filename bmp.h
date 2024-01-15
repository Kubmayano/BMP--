#pragma once

class BMP
{
    public:
    /*
    * BITMAPCOREHEADER = Original DIB header [12 bytes]
    * BITMAPINFOHEADER = Most common DIB header [40 bytes]
    * BITMAPV4HEADER = Adds additional color space information [108 bytes]
    * BITMAPV5HEADER = Latest DIB header [124 bytes]
    * 
    * Apparently the most common is the BITMAPINFOHEADER
    */
    enum DIBType
    {
        BITMAPCOREHEADER = 12,
        BITMAPINFOHEADER = 40,
        BITMAPV4HEADER = 108,
        BITMAPV5HEADER = 124,
    };

    void CreateRandomImage();
    char* ReturnData();

    unsigned int bitmapFileLength;

    BMP(DIBType DIBType, unsigned int width, unsigned int height);

    private:

    void SetImageSize(unsigned int width, unsigned int height);
    void InitializeHeaderPortion();
    void InitializeDeviceIndependentBitmapHeaderPortion();

    char* fileData;
    unsigned int bitmapFileHeader = 14; //This is always 14 bytes
    unsigned int fileDataLength; //Length of our fileData array since its dynamically allocated
    unsigned int deviceIndependentBitmapHeader;
    unsigned int pixelArray;
    unsigned int width;
    unsigned int height;
    unsigned int bitsPerPixel = 24; //This gives us 8 bits for R, G, B respectively meaning we have no alpha channels
    unsigned int bytesPerPixel = 3;
    unsigned int resolution = 2835; //This is equivalent to 72DPI
};