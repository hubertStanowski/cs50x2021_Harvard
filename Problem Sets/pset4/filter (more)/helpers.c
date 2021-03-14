#include <math.h>
#include "helpers.h"

// Convert image to grayscale
void grayscale(int height, int width, RGBTRIPLE image[height][width])
{
    float shade;

    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            shade = round((image[i][j].rgbtBlue + image[i][j].rgbtGreen + image[i][j].rgbtRed) / 3.0);
            image[i][j].rgbtBlue = shade;
            image[i][j].rgbtGreen = shade;
            image[i][j].rgbtRed = shade;
        }
    }

    return;
}

// Reflect image horizontally
void reflect(int height, int width, RGBTRIPLE image[height][width])
{

    RGBTRIPLE temp;

    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width / 2 ; j++)
        {
            temp = image[i][j];
            image[i][j].rgbtBlue = image[i][width - j - 1].rgbtBlue;
            image[i][j].rgbtGreen = image[i][width - j - 1].rgbtGreen;
            image[i][j].rgbtRed = image[i][width - j - 1].rgbtRed;

            image[i][width - j - 1].rgbtBlue = temp.rgbtBlue;
            image[i][width - j - 1].rgbtGreen = temp.rgbtGreen;
            image[i][width - j - 1].rgbtRed = temp.rgbtRed;
        }


    }

    return;
}

// Blur image
void blur(int height, int width, RGBTRIPLE image[height][width])
{
    int avgRed, avgGreen, avgBlue;
    float counter = 0;
    RGBTRIPLE tmp_img[height][width];

    // Making a temporary compy of the image
    for (int h = 0; h < height; h++)
    {
        for (int w = 0; w < width; w++)
        {
            tmp_img[h][w] = image[h][w];
        }
    }


    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            // Reseting variables
            avgRed = 0;
            avgGreen = 0;
            avgBlue = 0;
            counter = 0;

            // Checking whether each pixel is a part of the image
            // (within height x width grid)


            // Row 1

            if (i - 1 >= 0 && j - 1 >= 0)
            {

                avgRed += tmp_img[i - 1][j - 1].rgbtRed;
                avgGreen += tmp_img[i - 1][j - 1].rgbtGreen;
                avgBlue += tmp_img[i - 1][j - 1].rgbtBlue;
                counter++;
            }

            if (i - 1 >= 0)
            {

                avgRed += tmp_img[i - 1][j].rgbtRed;
                avgGreen += tmp_img[i - 1][j].rgbtGreen;
                avgBlue += tmp_img[i - 1][j].rgbtBlue;
                counter++;
            }

            if (i - 1 >= 0 && j + 1 < width)
            {
                avgRed += tmp_img[i - 1][j + 1].rgbtRed;
                avgGreen += tmp_img[i - 1][j + 1].rgbtGreen;
                avgBlue += tmp_img[i - 1][j + 1].rgbtBlue;
                counter++;
            }

            // Row 2

            if (j - 1 >= 0)
            {

                avgRed += tmp_img[i][j - 1].rgbtRed;
                avgGreen += tmp_img[i][j - 1].rgbtGreen;
                avgBlue += tmp_img[i][j - 1].rgbtBlue;
                counter++;
            }

            // Always True
            avgRed += tmp_img[i][j].rgbtRed;
            avgGreen += tmp_img[i][j].rgbtGreen;
            avgBlue += tmp_img[i][j].rgbtBlue;
            counter++;


            if (j + 1 < width)
            {

                avgRed += tmp_img[i][j + 1].rgbtRed;
                avgGreen += tmp_img[i][j + 1].rgbtGreen;
                avgBlue += tmp_img[i][j + 1].rgbtBlue;
                counter++;
            }

            // Row 3

            if (i + 1 < height && j - 1 >= 0)
            {

                avgRed += tmp_img[i + 1][j - 1].rgbtRed;
                avgGreen += tmp_img[i + 1][j - 1].rgbtGreen;
                avgBlue += tmp_img[i + 1][j - 1].rgbtBlue;
                counter++;
            }

            if (i + 1 < height)
            {

                avgRed += tmp_img[i + 1][j].rgbtRed;
                avgGreen += tmp_img[i + 1][j].rgbtGreen;
                avgBlue += tmp_img[i + 1][j].rgbtBlue;
                counter++;
            }

            if (i + 1 < height && j + 1 < width)
            {

                avgRed += tmp_img[i + 1][j + 1].rgbtRed;
                avgGreen += tmp_img[i + 1][j + 1].rgbtGreen;
                avgBlue += tmp_img[i + 1][j + 1].rgbtBlue;
                counter++;
            }

            // Avaraging

            avgRed = round(avgRed / counter);
            avgGreen = round(avgGreen / counter);
            avgBlue = round(avgBlue / counter);


            // Assigning

            image[i][j].rgbtRed = avgRed;
            image[i][j].rgbtGreen = avgGreen;
            image[i][j].rgbtBlue = avgBlue;

        }

    }

    // Ending

    return;
}

// Detect edges

void edges(int height, int width, RGBTRIPLE image[height][width])
{
    RGBTRIPLE tmp_img[height][width];
    float GxRed = 0, GxGreen = 0, GxBlue = 0;
    float GyRed = 0, GyGreen = 0, GyBlue = 0;
    int ValueRed, ValueGreen, ValueBlue;

    // Making a temporary compy of the image
    for (int h = 0; h < height; h++)
    {
        for (int w = 0; w < width; w++)
        {
            tmp_img[h][w] = image[h][w];
        }
    }


    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            // Reseting Gx & Gy
            GxRed = 0;
            GxGreen = 0;
            GxBlue = 0;
            GyRed = 0;
            GyGreen = 0;
            GyBlue = 0;

            // Computing Gx

            // Left Side (Gx)
            if (j - 1 >= 0)
            {
                if (i - 1 >= 0)
                {
                    // i-1 x j-1
                    GxRed += -1 * tmp_img[i - 1][j - 1].rgbtRed;
                    GxGreen += -1 * tmp_img[i - 1][j - 1].rgbtGreen;
                    GxBlue += -1 * tmp_img[i - 1][j - 1].rgbtBlue;

                }

                // i x j-1
                GxRed += -2 * tmp_img[i][j - 1].rgbtRed;
                GxGreen += -2 * tmp_img[i][j - 1].rgbtGreen;
                GxBlue += -2 * tmp_img[i][j - 1].rgbtBlue;

                if (i + 1 < height)
                {
                    // i+1 x j-1
                    GxRed += -1 * tmp_img[i + 1][j - 1].rgbtRed;
                    GxGreen += -1 * tmp_img[i + 1][j - 1].rgbtGreen;
                    GxBlue += -1 * tmp_img[i + 1][j - 1].rgbtBlue;

                }
            }

            // Right Side (Gx)
            if (j + 1 < width)
            {
                if (i - 1 >= 0)
                {
                    // i-1 x j+1
                    GxRed += 1 * tmp_img[i - 1][j + 1].rgbtRed;
                    GxGreen += 1 * tmp_img[i - 1][j + 1].rgbtGreen;
                    GxBlue += 1 * tmp_img[i - 1][j + 1].rgbtBlue;

                }

                // i x j+1
                GxRed += 2 * tmp_img[i][j + 1].rgbtRed;
                GxGreen += 2 * tmp_img[i][j + 1].rgbtGreen;
                GxBlue += 2 * tmp_img[i][j + 1].rgbtBlue;

                if (i + 1 < height)
                {
                    // i+1 x j+1
                    GxRed += 1 * tmp_img[i + 1][j + 1].rgbtRed;
                    GxGreen += 1 * tmp_img[i + 1][j + 1].rgbtGreen;
                    GxBlue += 1 * tmp_img[i + 1][j + 1].rgbtBlue;

                }
            }


            // Computing Gy

            // Top (Gy)
            if (i - 1 >= 0)
            {
                if (j - 1 >= 0)
                {
                    // i-1 x j-1
                    GyRed += -1 * tmp_img[i - 1][j - 1].rgbtRed;
                    GyGreen += -1 * tmp_img[i - 1][j - 1].rgbtGreen;
                    GyBlue += -1 * tmp_img[i - 1][j - 1].rgbtBlue;
                }

                // i-1 x j
                GyRed += -2 * tmp_img[i - 1][j].rgbtRed;
                GyGreen += -2 * tmp_img[i - 1][j].rgbtGreen;
                GyBlue += -2 * tmp_img[i - 1][j].rgbtBlue;

                if (j + 1 < width)
                {
                    // i-1 x j+1
                    GyRed += -1 * tmp_img[i - 1][j + 1].rgbtRed;
                    GyGreen += -1 * tmp_img[i - 1][j + 1].rgbtGreen;
                    GyBlue += -1 * tmp_img[i - 1][j + 1].rgbtBlue;
                }

            }

            // Bottom (Gy)
            if (i + 1 < height)
            {
                if (j - 1 >= 0)
                {
                    // i+1 x j-1
                    GyRed += 1 * tmp_img[i + 1][j - 1].rgbtRed;
                    GyGreen += 1 * tmp_img[i + 1][j - 1].rgbtGreen;
                    GyBlue += 1 * tmp_img[i + 1][j - 1].rgbtBlue;
                }

                // i+1 x j
                GyRed += 2 * tmp_img[i + 1][j].rgbtRed;
                GyGreen += 2 * tmp_img[i + 1][j].rgbtGreen;
                GyBlue += 2 * tmp_img[i + 1][j].rgbtBlue;

                if (j + 1 < width)
                {
                    // i+1 x j+1
                    GyRed += 1 * tmp_img[i + 1][j + 1].rgbtRed;
                    GyGreen += 1 * tmp_img[i + 1][j + 1].rgbtGreen;
                    GyBlue += 1 * tmp_img[i + 1][j + 1].rgbtBlue;
                }

            }

            // Calculating Values using Sobel formula (Gx^2 + Gy^2)
            ValueRed = round(sqrt((GxRed * GxRed) + (GyRed * GyRed)));
            ValueGreen = round(sqrt((GxGreen * GxGreen) + (GyGreen * GyGreen)));
            ValueBlue = round(sqrt((GxBlue * GxBlue) + (GyBlue * GyBlue)));


            // Capping Values at 255
            if (ValueRed > 255)
            {
                ValueRed = 255;
            }

            if (ValueGreen > 255)
            {
                ValueGreen = 255;
            }

            if (ValueBlue > 255)
            {
                ValueBlue = 255;
            }

            // Updating the image
            image[i][j].rgbtRed = ValueRed;
            image[i][j].rgbtGreen = ValueGreen;
            image[i][j].rgbtBlue = ValueBlue;


        }

    }

    return;
}
