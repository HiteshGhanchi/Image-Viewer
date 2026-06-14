
// gcc iv.c -o iv `sdl2-config --cflags --libs` -> I can compile the code using this command
// linker flags are provided by sdl2-config --libs, and the compiler flags are provided by sdl2-config --cflags.
// without this linker cannot know where to find the .so file -> compilation shows error: undefined reference to `SDL_CreateWindow'

#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_timer.h>



int main(int argc, char *argv[]) {

    // printf("Hello world!\n");
    // printf("The value of argc is: %d\n", argc);
    // for(int i=0; i < argc; i++) {
    //     printf("argv[%d]: %s\n", i, argv[i]);
    // }

    // So the image to be processed is going to be passed as following:
    // ./iv <image_path>

    if(argc < 2) {
        printf("Please provide the path to the image as an argument.\n");
        printf("Usage: ./iv <image_path>\n");
        return 1;
    }

    char *image_path = argv[1];

    // Now I need to first read the image file properly
    // https://www.ece.ualberta.ca/~elliott/ee552/studentAppNotes/2003_w/misc/bmp_file_format/bmp_file_format.htm

    // read the values from this file about height, width and the pixel data and such


    // V1: I am going to implement the image viewer using SDL2 library, which is a cross-platform development library designed 
    // to provide low level access to audio, keyboard, mouse, joystick, and graphics hardware via OpenGL and Direct3D. 
    // It is used by video playback software, emulators, and popular games.

    // https://wiki.libsdl.org/SDL2/Installation : using this guide to install SDL2 on my system
    // Ok so I learned the following:
    /*
    We create Window and then can show image using the following things:
    1) WindowSurface: SDL_GetWindowSurface(window) - This function returns a pointer to the window's framebuffer surface.
    Here, it is handled by CPU and is not hardware accelerated. It is suitable for simple 2D graphics and software rendering.
    2) Renderer: Here, we cannot actually manipulate the pixels directly. Instead, we use SDL_Renderer to draw textures and shapes onto the window.
    This is done by GPU using Vulkan or OpenGL. It is suitable for more complex graphics and hardware-accelerated rendering.
    3) Texture: Sits on top of the renderer and represents an image that can be drawn onto the window. We can create a texture from an image file using SDL_CreateTextureFromSurface(renderer, surface).
    
    */
    int width = 800;
    int height = 600;

    if(SDL_Init(SDL_INIT_VIDEO) != 0) {
        printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
        return 1;
    }

    // Here, I am suppose to get all the values using glibc functions 
    // file 
    FILE *file = fopen(image_path, "rb");
    if(file == NULL) {
        printf("Could not open file: %s\n", image_path);
        return 1;
    }

    // File size is given in the 4 bytes of the file offset of 2 bytes, so I can read that first
    char signature[2];
    fread(signature, sizeof(char), 2, file);
    int file_size; // Here, I am reading in int, so Endian is automatically handled by the system
    fread(&file_size, sizeof(int), 1, file);
    
    printf("Signature: %c%c\n", signature[0], signature[1]);
    printf("File size: %d bytes\n", file_size);

    fseek(file, 12, SEEK_CUR);
    fread(&width, sizeof(int), 1, file);
    fread(&height, sizeof(int), 1, file);

    printf("Width: %d\n", width);
    printf("Height: %d\n", height);

    SDL_Window *pwindow = SDL_CreateWindow("Image Viewer",SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, SDL_WINDOW_SHOWN);

    if(pwindow == NULL) {
        printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
        return 1;
    }

    SDL_Surface * psurface = SDL_GetWindowSurface(pwindow);

    if(psurface == NULL) {
        printf("Surface could not be created! SDL_Error: %s\n", SDL_GetError());
        return 1;
    }

    // making all the pixels of the surface white
    // https://wiki.libsdl.org/SDL2/SDL_FillRect
    // http://wiki.libsdl.org/SDL2/SDL_MapRGB
    // const SDL_Rect rect = {0, 0, 1, 1};
    // SDL_FillRect(psurface, &rect, SDL_MapRGB(psurface->format, 255, 0, 0));

    // Here, I am putting the pixel data of the image into the surface

    // Now based on the compression and such, I will decide stuff ok

    fseek(file, 2, SEEK_CUR); 
    uint16_t bits_per_pixel;
    fread(&bits_per_pixel, sizeof(uint16_t), 1, file);

    printf("Bits per pixel: %d\n", bits_per_pixel);

    // if(bpp == 1) {
    //     read_1bit(file, palette, width, height, pixels);
    // } else if(bpp == 4 && compression == 0) {
    //     read_4bit_raw(file, palette, width, height, pixels);
    // } else if(bpp == 4 && compression == 2) {
    //     read_4bit_rle(file, palette, width, height, pixels);
    // } else if(bpp == 8 && compression == 0) {
    //     read_8bit_raw(file, palette, width, height, pixels);
    // } else if(bpp == 8 && compression == 1) {
    //     read_8bit_rle(file, palette, width, height, pixels);
    // } else if(bpp == 16 && compression == 0) {
    //     read_16bit_rgb(file, width, height, pixels);
    // } else if(bpp == 16 && compression == 3) {
    //     read_16bit_bitfields(file, masks, width, height, pixels);
    // } else if(bpp == 24) {
    //     read_24bit(file, width, height, pixels);
    // } else {
    //     printf("unsupported format\n");
    //     return 1;
    // }
    


    if(bits_per_pixel == 24){
        int dataoffset;
        fseek(file, 10, SEEK_SET);
        fread(&dataoffset, sizeof(int), 1, file);

        // now I have all the pixel data
        fseek(file, dataoffset, SEEK_SET);

        uint8_t bgr[3];

        int raw_row = width * 3;  // 24 bit = 3 bytes per pixel
        int padding = (4 - (raw_row % 4)) % 4;  

        for(int y = 0;y<height;y++) {
            for(int x = 0;x<width;x++) {
                fread(bgr, 1, 3, file);

                // Now I have the RGB values of the pixel at (x,y)
                // I need to put this pixel in the surface at the correct position
                uint32_t pixel_color = SDL_MapRGB(psurface->format, bgr[2], bgr[1], bgr[0]);
                SDL_Rect rect = {x, height - y - 1, 1, 1};  // images are stored bottom to top, so we need to flip the y coordinate
                SDL_FillRect(psurface, &rect, pixel_color);
            }
            fseek(file, padding, SEEK_CUR); // Skip the padding bytes at the end of each row
        }

    }

    // Update the window surface to reflect the changes made to the surface
    SDL_UpdateWindowSurface(pwindow);
    SDL_Event e;
    int running = 1;
    while(running) {
        while(SDL_PollEvent(&e)) {
            if(e.type == SDL_QUIT) running = 0;
            if(e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_ESCAPE) running = 0;
        }
    }
    fclose(file); // Close the file after reading the image data
    SDL_DestroyWindow(pwindow); // Clean up and destroy the window -> memory leak if not done


    return 0;
}