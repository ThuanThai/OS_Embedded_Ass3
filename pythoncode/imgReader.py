import cv2

# Load an image from file
image_path = 'fire5_180.png'  # Replace with the path to your image file
image = cv2.imread(image_path)

print(image.shape)
# print(image[200, 200])
height, width, channels = image.shape
print(height)
print(width)

colorStr = ""

for x in range(height):
    colorStr += "{"
    for y in range(width):
        # print(image[x, y])
        if (image[x][y][0] > 240 and image[x][y][1] > 240 and image[x][y][2] > 240 ):
            hex_color = "0x00FFFFFF"
        else:
            hex_color = '0x00{:02X}{:02X}{:02X}'.format(image[x][y][0], image[x][y][1], image[x][y][2])
        if (y + 1 == width):
            colorStr += hex_color + "}"
        else:
            colorStr += hex_color + ","
        # print(hex_color)
    if (x + 1 != height):
        colorStr += ",\n"

with open('fire5_180.txt', 'w') as file:
    # Write text to the file
    file.write(colorStr)

# if image is None:
#     print("Error: Unable to load the image.")
# else:
#     # Get the dimensions of the image (height, width, and number of color channels)
#     height, width, channels = image.shape

#     print(f"Image dimensions: Height={height}, Width={width}, Channels={channels}")

#     # Loop through all the pixels in the image
#     for y in range(height):
#         for x in range(width):
#             # Get the pixel value at the current (x, y) position
#             pixel_value = image[y, x]
            
#             # You can access individual color channels like this:
#             # blue_channel = pixel_value[0]
#             # green_channel = pixel_value[1]
#             # red_channel = pixel_value[2]

#             # Process or analyze the pixel value as needed
#             # For example, you can print the pixel value
#             print(f"Pixel at ({x}, {y}): {pixel_value}")

# Close the image window (optional)
cv2.destroyAllWindows()





